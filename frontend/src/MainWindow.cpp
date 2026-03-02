#include "../include/MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QSettings>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mSerial(new QSerialPort(this)),
      mSock(new QTcpSocket(this)),
      io(nullptr),
      mCamSock(new QTcpSocket(this))
{
  ui->setupUi(this);

  QSettings settings("HyperK", "SigmaControl");

  ui->portComboBox->setEditText(
      settings.value("lensEndpoint", "tcp:192.168.137.175:5000").toString());

  ui->cameraEndpointEdit->setText(
      settings.value("cameraEndpoint", "tcp:192.168.137.175:5001").toString());

  refreshPortList();
  reset();

  ui->debugBox->setReadOnly(true);

  // ---- UI connections (unchanged) ----
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);

  connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refreshPortList);
  connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::toggleConnect);
  connect(ui->debugButton, &QPushButton::clicked, this, &MainWindow::toggleDebug);
  connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::togglePower);

  connect(ui->apertureValueSlider, &QSlider::valueChanged, this, &MainWindow::updateAperture);
  connect(ui->incrementApertureButton, &QPushButton::clicked, this, &MainWindow::incrementAperture);
  connect(ui->decrementApertureButton, &QPushButton::clicked, this, &MainWindow::decrementAperture);

  connect(ui->incrementFocusButton, &QPushButton::clicked, this, &MainWindow::incrementFocus);
  connect(ui->decrementFocusButton, &QPushButton::clicked, this, &MainWindow::decrementFocus);

  connect(ui->focusValueSlider, &QSlider::valueChanged, this, &MainWindow::updateFocus);
  connect(ui->setApertureButton, &QPushButton::clicked, this, &MainWindow::setAperture);
  connect(ui->setFocusButton, &QPushButton::clicked, this, &MainWindow::setFocus);

  // ---- Transport connections ----

  // Serial readyRead
  connect(mSerial, &QSerialPort::readyRead,
          this, &MainWindow::onIoReadyRead);

  // TCP readyRead
  connect(mSock, &QTcpSocket::readyRead,
          this, &MainWindow::onIoReadyRead);

  // Serial errors
  connect(mSerial, &QSerialPort::errorOccurred,
          this, &MainWindow::handleSerialError);

  // TCP errors
  connect(mSock, &QTcpSocket::errorOccurred,
          this, [this](auto)
          {
                ui->statusbar->showMessage("TCP error");
                allowControl(false); });

  connect(mSock, &QTcpSocket::disconnected,
          this, [this]()
          {
                ui->statusbar->showMessage("TCP disconnected");
                allowControl(false); });

  // ---- Camera UI connections ----
  connect(ui->camConnectButton, &QPushButton::clicked,
          this, &MainWindow::camToggleConnect);
  connect(ui->camStatusButton, &QPushButton::clicked,
          this, &MainWindow::camStatus);
  connect(ui->camCaptureButton, &QPushButton::clicked,
          this, &MainWindow::camCapture);

  // ---- Camera transport connections ----
  connect(mCamSock, &QTcpSocket::readyRead,
          this, &MainWindow::onCamReadyRead);

  connect(mCamSock, &QTcpSocket::errorOccurred,
          this, [this](auto)
          {
            ui->statusbar->showMessage("Camera TCP error");
            mCamConnected = false;
            setCameraUiEnabled(false);
            ui->camConnectButton->setText("Connect Camera"); });

  connect(mCamSock, &QTcpSocket::disconnected,
          this, [this]()
          {
            ui->statusbar->showMessage("Camera TCP disconnected");
            mCamConnected = false;
            setCameraUiEnabled(false);
            ui->camConnectButton->setText("Connect Camera"); });

  ui->portComboBox->setEditable(true);
  mTelemetryTimer = new QTimer(this);
  connect(mTelemetryTimer, &QTimer::timeout,
          this, &MainWindow::updateTelemetryUi);
  mTelemetryTimer->start(5000); // 5000 ms = every 5 seconds

  // Camera defaults + disable until connected
  setCameraUiEnabled(false);
}

MainWindow::~MainWindow()
{
  if (mSerial && mSerial->isOpen())
    mSerial->close();

  if (mSock && mSock->isOpen())
    mSock->close();

  if (mCamSock && mCamSock->isOpen())
    mCamSock->close();

  delete ui;
}
void MainWindow::allowControl(bool value)
{
  ui->powerButton->setEnabled(value);
  ui->debugButton->setEnabled(value);
  ui->apertureValueSlider->setEnabled(value);
  ui->focusValueSlider->setEnabled(value);
  ui->setApertureButton->setEnabled(value);
  ui->setFocusButton->setEnabled(value);
  ui->incrementApertureButton->setEnabled(value);
  ui->decrementApertureButton->setEnabled(value);
  ui->incrementFocusButton->setEnabled(value);
  ui->decrementFocusButton->setEnabled(value);
}

void MainWindow::reset()
{
  ui->apertureValueSlider->setValue(0);
  ui->focusValueSlider->setValue(0);
  ui->cameraIDLabel->setText("Unknown");
  ui->apertureValueLabel->setText("f 1.41421 | 0x1100");
  ui->focusValueLabel->setText("infinity m | 0x204E");
  ui->curFocusLabel->setText("Unknown");
  ui->curApertureLabel->setText("Unknown");
  ui->debugButton->setText("| >");
  ui->connectButton->setText("Connect");
  ui->curStateLabel->setText("Unknown");
  ui->curTempLabel->setText("Unknown");
  ui->curPressureLabel->setText("Unknown");
  ui->curHumidityLabel->setText("Unknown");

  ui->curAccelLabel->setText("Unknown");
  ui->curGyroLabel->setText("Unknown");
  ui->curMagLabel->setText("Unknown");
  ui->curBmagLabel->setText("Unknown");
  ui->curHeadingLabel->setText("Unknown");
  mLensPos = 0;
  mAperture = 0;
  mDebug = false;
  logLines.clear();

  allowControl(false);
}

void MainWindow::refreshPortList()
{
  QSettings settings("HyperK", "SigmaControl");

  // Prefer the saved lens endpoint; fall back to whatever is currently in the box
  QString saved = settings.value("lensEndpoint", "").toString().trimmed();
  QString currentText = ui->portComboBox->currentText().trimmed();
  QString restoreText = !saved.isEmpty() ? saved : currentText;

  ui->portComboBox->clear();

  const auto ports = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo &port : ports)
  {
    QString desc = port.description().toLower();
    QString manufacturer = port.manufacturer().toLower();

    if (desc.contains("teensy") || manufacturer.contains("teensy"))
    {
      ui->portComboBox->addItem(port.portName());
    }
  }

  // Keep the combo editable so arbitrary tcp:HOST:PORT works
  ui->portComboBox->setEditable(true);

  // If the restoreText isn't already one of the items, add it so it appears in the dropdown history
  if (!restoreText.isEmpty())
  {
    int idx = ui->portComboBox->findText(restoreText);
    if (idx < 0)
      ui->portComboBox->addItem(restoreText);
    ui->portComboBox->setEditText(restoreText);
  }
}

void MainWindow::toggleConnect()
{
  if (ui->connectButton->text() == "Connect")
  {
    QSettings settings("HyperK", "SigmaControl");
    settings.setValue("lensEndpoint", ui->portComboBox->currentText().trimmed());
    bool success = openSerialPort();
    if (success)
    {

      ui->connectButton->setText("Disconnect");
    }
  }
  else if (ui->connectButton->text() == "Disconnect")
  {
    bool success = closeSerialPort();
    if (success)
    {
      ui->connectButton->setText("Connect");
      reset();
    }
  }
}

bool MainWindow::closeSerialPort()
{
  bool wasOpen = false;
  mHaveTelemetry = false;
  mLastKV.clear();

  if (mSerial && mSerial->isOpen())
  {
    mSerial->close();
    wasOpen = true;
  }

  if (mSock && mSock->isOpen())
  {
    mSock->close();
    wasOpen = true;
  }

  io = nullptr;
  rxBuf.clear();

  if (wasOpen)
  {
    allowControl(false);
    ui->statusbar->showMessage("Disconnected");
  }

  return wasOpen;
}

bool MainWindow::openSerialPort()
{
  QString target = ui->portComboBox->currentText().trimmed();

  // close anything currently open
  if (mSerial && mSerial->isOpen())
    mSerial->close();
  if (mSock && mSock->isOpen())
    mSock->close();

  io = nullptr;
  rxBuf.clear();

  // --- TCP mode: tcp:HOST:PORT ---
  if (target.startsWith("tcp:", Qt::CaseInsensitive))
  {
    QString t = target.mid(4);
    QStringList hp = t.split(':');

    if (hp.size() != 2)
    {
      QMessageBox::critical(this, "Error",
                            "Use tcp:HOST:PORT (e.g. tcp:192.168.50.2:5000)");
      return false;
    }

    QString host = hp[0];
    bool ok = false;
    quint16 port = hp[1].toUShort(&ok);
    if (!ok || port == 0)
    {
      QMessageBox::critical(this, "Error", "Invalid TCP port.");
      return false;
    }

    mSock->connectToHost(host, port);
    if (!mSock->waitForConnected(2000))
    {
      QMessageBox::critical(this, "Error", "Failed to connect to TCP bridge.");
      return false;
    }

    io = mSock;
    allowControl(true);
    ui->statusbar->showMessage("Connected (TCP) " + host + ":" + QString::number(port));

    return true;
  }

  // --- Serial mode ---
  mSerial->setPortName(target);
  mSerial->setBaudRate(QSerialPort::Baud9600);

  if (!mSerial->open(QIODevice::ReadWrite))
  {
    QMessageBox::critical(this, "Error", "Failed to open port.");
    return false;
  }

  io = mSerial;
  allowControl(true);
  ui->statusbar->showMessage("Connected (Serial) " + target);
  return true;
}

void MainWindow::setAperture()
{
  QString cmd = "SA " + QString::number(ui->apertureValueSlider->value());

  sendCommand(cmd);
}

void MainWindow::toggleDebug()
{
  mDebug = !mDebug;
  if (mDebug)
  {
    ui->debugButton->setText("| |");
  }
  else
  {
    ui->debugButton->setText("| >");
  }
}

void MainWindow::setFocus()
{
  QString cmd = "SF " + QString::number(ui->focusValueSlider->value()); // ui->commandEdit->text().trimmed();

  sendCommand(cmd);
}

void MainWindow::togglePower()
{
  QString cmd = "#$#?";
  if (ui->powerButton->text() == "Power On")
  {
    cmd = "ON";
  }
  else
  {
    cmd = "OFF";
  }

  sendCommand(cmd);
}

void MainWindow::sendCommand(QString cmd)
{
  if (!io || !io->isOpen())
  {
    QMessageBox::critical(this, "Error", "Connection is closed.");
    return;
  }

  if (cmd.isEmpty())
    return;

  io->write((cmd + "\n").toUtf8());

  // display sent command
  if (mDebug)
  {
    appendDebugLine(">> " + cmd);
  }
}
void MainWindow::appendDebugLine(const QString &line, int maxLines)
{
  // .cpp
  if (mDebug)
  {
    logLines << line;
    if (logLines.size() > maxLines)
      logLines.removeFirst();

    ui->debugBox->setPlainText(logLines.join('\n'));
    ui->debugBox->moveCursor(QTextCursor::End);
  }
}

QString MainWindow::connectionLabel() const
{
  if (mSock && mSock->isOpen())
    return "TCP";

  if (mSerial && mSerial->isOpen())
    return QString("Serial %1").arg(mSerial->portName());

  return "Disconnected";
}

void MainWindow::updateTelemetryUi()
{
  if (!io || !io->isOpen())
    return;
  if (!mHaveTelemetry)
    return;

  const auto &keyValueMap = mLastKV;

  // --- BME280 telemetry ---
  const QString tStr = keyValueMap.value("T"); // temperature C
  const QString pStr = keyValueMap.value("P"); // pressure hPa
  const QString hStr = keyValueMap.value("H"); // humidity %

  auto bad = [](const QString &s)
  {
    QString x = s.trimmed().toLower();
    return x.isEmpty() || x == "na" || x == "nan";
  };

  if (!tStr.isEmpty())
    ui->curTempLabel->setText(!bad(tStr) ? (tStr + " °C") : "NA");
  if (!pStr.isEmpty())
    ui->curPressureLabel->setText(!bad(pStr) ? (pStr + " hPa") : "NA");
  if (!hStr.isEmpty())
    ui->curHumidityLabel->setText(!bad(hStr) ? (hStr + " %") : "NA");

  const bool bmeOk = (!tStr.isEmpty() || !pStr.isEmpty() || !hStr.isEmpty()) &&
                     !bad(tStr) && !bad(pStr) && !bad(hStr);

  if (!tStr.isEmpty() || !pStr.isEmpty() || !hStr.isEmpty())
  {
    ui->statusbar->showMessage(QString("Connected (%1) | BME280 %2")
                                   .arg(connectionLabel())
                                   .arg(bmeOk ? "OK" : "NOT FOUND"));
  }

  // --- IMU + MAG telemetry ---
  auto toDoubleSafe = [&](const QString &s, double &out) -> bool
  {
    QString x = s.trimmed();
    if (x.isEmpty())
      return false;
    QString xl = x.toLower();
    if (xl == "na" || xl == "nan")
      return false;
    bool ok = false;
    out = x.toDouble(&ok);
    return ok;
  };

  auto fmt3 = [](double v, int dec = 2)
  { return QString::number(v, 'f', dec); };

  double ax, ay, az;
  if (toDoubleSafe(keyValueMap.value("Ax"), ax) &&
      toDoubleSafe(keyValueMap.value("Ay"), ay) &&
      toDoubleSafe(keyValueMap.value("Az"), az))
  {
    ui->curAccelLabel->setText(QString("Ax:%1  Ay:%2  Az:%3")
                                   .arg(fmt3(ax, 2))
                                   .arg(fmt3(ay, 2))
                                   .arg(fmt3(az, 2)));
  }
  else if (keyValueMap.contains("Ax") || keyValueMap.contains("Ay") || keyValueMap.contains("Az"))
  {
    ui->curAccelLabel->setText("NA");
  }

  double gx, gy, gz;
  if (toDoubleSafe(keyValueMap.value("Gx"), gx) &&
      toDoubleSafe(keyValueMap.value("Gy"), gy) &&
      toDoubleSafe(keyValueMap.value("Gz"), gz))
  {
    ui->curGyroLabel->setText(QString("Gx:%1  Gy:%2  Gz:%3")
                                  .arg(fmt3(gx, 2))
                                  .arg(fmt3(gy, 2))
                                  .arg(fmt3(gz, 2)));
  }
  else if (keyValueMap.contains("Gx") || keyValueMap.contains("Gy") || keyValueMap.contains("Gz"))
  {
    ui->curGyroLabel->setText("NA");
  }

  double bx, by, bz;
  if (toDoubleSafe(keyValueMap.value("Bx"), bx) &&
      toDoubleSafe(keyValueMap.value("By"), by) &&
      toDoubleSafe(keyValueMap.value("Bz"), bz))
  {
    ui->curMagLabel->setText(QString("Bx:%1  By:%2  Bz:%3")
                                 .arg(fmt3(bx, 2))
                                 .arg(fmt3(by, 2))
                                 .arg(fmt3(bz, 2)));

    const double Bmag = std::sqrt(bx * bx + by * by + bz * bz);
    ui->curBmagLabel->setText(QString::number(Bmag, 'f', 1));
  }
  else if (keyValueMap.contains("Bx") || keyValueMap.contains("By") || keyValueMap.contains("Bz"))
  {
    ui->curMagLabel->setText("NA");
    ui->curBmagLabel->setText("NA");
  }

  double hd;
  if (toDoubleSafe(keyValueMap.value("Hd"), hd))
    ui->curHeadingLabel->setText(QString::number(hd, 'f', 1));
  else if (keyValueMap.contains("Hd"))
    ui->curHeadingLabel->setText("NA");
}

void MainWindow::handleMessage(QString msg)
{
  // Parse key:value pairs (same as your code)
  QStringList parts = msg.split(',');
  QMap<QString, QString> keyValueMap;

  for (const QString &part : parts)
  {
    if (part.contains(':'))
    {
      QStringList pair = part.split(':');
      if (pair.size() >= 2)
        keyValueMap.insert(pair[0].trimmed(), pair[1].trimmed());
    }
    else
    {
      keyValueMap.insert("Message", part.trimmed());
    }
  }

  // Cache for slow loop telemetry UI
  mLastKV = keyValueMap;
  mHaveTelemetry = true;

  // ----- FAST lens-related UI updates -----
  const QString lensState = keyValueMap.value("S");
  if (!lensState.isEmpty())
  {
    if (lensState == "Off")
      ui->powerButton->setText("Power On");
    else
      ui->powerButton->setText("Shut Down");

    ui->curStateLabel->setText(lensState);
  }

  const QString idStr = keyValueMap.value("ID");
  if (!idStr.isEmpty())
    ui->cameraIDLabel->setText(idStr);

  // Aperture / focus updates (only if keys exist)
  bool okA = false;
  int a = keyValueMap.value("A").toInt(&okA);
  if (okA)
  {
    uint16_t curAperture = static_cast<uint16_t>(a);
    if (curAperture != mAperture)
    {
      mAperture = curAperture;
      ui->curApertureLabel->setText("f/" + QString::number(getFNumber(mAperture)) +
                                    " | 0x" + QString::number(mAperture, 16).toUpper());
    }
  }

  bool okF = false;
  int f = keyValueMap.value("F").toInt(&okF);
  if (okF)
  {
    uint16_t curLensPos = static_cast<uint16_t>(f);
    if (curLensPos != mLensPos)
    {
      mLensPos = curLensPos;
      ui->curFocusLabel->setText(getFocus(mLensPos) +
                                 " m | 0x" + QString::number(mLensPos, 16).toUpper());
    }
  }
}

void MainWindow::onIoReadyRead()
{
  if (!io)
    return;

  QByteArray data = io->readAll();
  if (mDebug)
  {
    appendDebugLine("<< " + QString::fromUtf8(data));
  }

  rxBuf += QString::fromUtf8(data);

  int idx;
  while ((idx = rxBuf.indexOf('\n')) >= 0)
  {
    QString line = rxBuf.left(idx).trimmed();
    rxBuf.remove(0, idx + 1);
    if (!line.isEmpty())
      handleMessage(line);
  }
}

void MainWindow::showAbout()
{
  QMessageBox::about(this, "About", "Author: Tapendra BC \nWritten in: Summer 2025 \nU of W \nClient GUI program to set the focus and the aperture of sigma Art lens using Teensy 4.0 microcontroller. Writen in QT framework. Email: tapendra.320@gmail.com. Supervisor: Dr. Blair Jameison. Source Code: https://github.com/Tapendra-195/Sigma_Lens_Control");
}

QString MainWindow::getFocus(uint16_t lensPos)
{
  float v = 5.518144e-5 * lensPos - 0.381395; // image distance
  float focus = 3 * v / (20 * v - 3);         // find object distance(NOT THE FOCAL LENGTH, WHICH IS A CONSTANT, 15mm)
  QString focusToDisplay = (focus > 0) ? QString::number(focus) : "infinity";

  return focusToDisplay;
}

float MainWindow::getFNumber(uint16_t aperture)
{
  float fNumber = pow(sqrt(2), (aperture / 256.0) - 16);

  return fNumber;
}

void MainWindow::updateAperture()
{
  uint16_t aperture = ui->apertureValueSlider->value();
  ui->apertureValueLabel->setText("f/" + QString::number(getFNumber(aperture)) + " | 0x" + QString::number(aperture, 16).toUpper());
}

void MainWindow::updateFocus()
{
  int lensPos = ui->focusValueSlider->value();
  ui->focusValueLabel->setText(getFocus(lensPos) + " m | 0x" + QString::number(lensPos, 16).toUpper());
}

void MainWindow::incrementAperture()
{
  int value = ui->apertureValueSlider->value();
  int min = ui->apertureValueSlider->minimum();
  int max = ui->apertureValueSlider->maximum();

  int newValue = value + 1;
  newValue = std::clamp(newValue, min, max);
  ui->apertureValueSlider->setValue(newValue);

  updateAperture();
}

void MainWindow::decrementAperture()
{
  int value = ui->apertureValueSlider->value();
  int min = ui->apertureValueSlider->minimum();
  int max = ui->apertureValueSlider->maximum();

  int newValue = value - 1;
  newValue = std::clamp(newValue, min, max);
  ui->apertureValueSlider->setValue(newValue);

  updateAperture();
}

void MainWindow::incrementFocus()
{
  int value = ui->focusValueSlider->value();
  int min = ui->focusValueSlider->minimum();
  int max = ui->focusValueSlider->maximum();

  int newValue = value + 1;
  newValue = std::clamp(newValue, min, max);
  ui->focusValueSlider->setValue(newValue);

  updateFocus();
}

void MainWindow::decrementFocus()
{
  int value = ui->focusValueSlider->value();
  int min = ui->focusValueSlider->minimum();
  int max = ui->focusValueSlider->maximum();

  int newValue = value - 1;
  newValue = std::clamp(newValue, min, max);
  ui->focusValueSlider->setValue(newValue);

  updateFocus();
}

void MainWindow::handleSerialError(QSerialPort::SerialPortError error)
{
  if (error == QSerialPort::ResourceError)
  {
    QMessageBox::critical(this, "Error",
                          "Either the Lens or Teensy is Disconnected. Click Refresh");
    closeSerialPort();
    reset();
  }
}

void MainWindow::setCameraUiEnabled(bool en)
{
  // Don’t disable the endpoint field; only action buttons
  ui->camStatusButton->setEnabled(en);
  ui->camCaptureButton->setEnabled(en);

  // Parameter widgets can remain enabled even when disconnected,
  // but feel free to lock them too if you prefer:
  ui->exposureUsSpin->setEnabled(true);
  ui->gainSpin->setEnabled(true);
  ui->countSpin->setEnabled(true);
  ui->formatCombo->setEnabled(true);
  ui->outdirEdit->setEnabled(true);
  ui->prefixEdit->setEnabled(true);
  ui->cameraEndpointEdit->setEnabled(true);
}

bool MainWindow::openCameraTcp(const QString &endpoint)
{
  QString target = endpoint.trimmed();
  if (!target.startsWith("tcp:", Qt::CaseInsensitive))
  {
    QMessageBox::critical(this, "Error",
                          "Camera endpoint must be tcp:HOST:PORT (e.g. tcp:192.168.50.20:5001)");
    return false;
  }

  QString t = target.mid(4);
  QStringList hp = t.split(':');
  if (hp.size() != 2)
  {
    QMessageBox::critical(this, "Error",
                          "Camera endpoint must be tcp:HOST:PORT (e.g. tcp:192.168.50.20:5001)");
    return false;
  }

  QString host = hp[0];
  bool ok = false;
  quint16 port = hp[1].toUShort(&ok);
  if (!ok || port == 0)
  {
    QMessageBox::critical(this, "Error", "Invalid camera TCP port.");
    return false;
  }

  if (mCamSock->isOpen())
    mCamSock->close();

  mCamRxBuf.clear();
  mCamSock->connectToHost(host, port);
  if (!mCamSock->waitForConnected(1500))
  {
    QMessageBox::critical(this, "Error", "Failed to connect to camera TCP bridge.");
    return false;
  }

  mCamConnected = true;
  setCameraUiEnabled(true);
  ui->statusbar->showMessage("Camera connected (TCP) " + host + ":" + QString::number(port));
  return true;
}

void MainWindow::closeCameraTcp()
{
  mCamConnected = false;
  mCamRxBuf.clear();

  if (mCamSock && mCamSock->isOpen())
    mCamSock->close();

  setCameraUiEnabled(false);
  ui->statusbar->showMessage("Camera disconnected");
}

void MainWindow::sendCamJsonLine(const QJsonObject &obj)
{
  if (!mCamSock || !mCamSock->isOpen())
  {
    QMessageBox::critical(this, "Error", "Camera connection is closed.");
    return;
  }

  QJsonDocument doc(obj);
  QByteArray line = doc.toJson(QJsonDocument::Compact);
  line.append('\n');

  mCamSock->write(line);

  if (mDebug)
    appendDebugLine(QString("CAM >> %1").arg(QString::fromUtf8(line).trimmed()));
}

void MainWindow::onCamReadyRead()
{
  if (!mCamSock)
    return;

  mCamRxBuf.append(mCamSock->readAll());

  int nl = -1;
  while ((nl = mCamRxBuf.indexOf('\n')) >= 0)
  {
    QByteArray line = mCamRxBuf.left(nl);
    mCamRxBuf.remove(0, nl + 1);

    line = line.trimmed();
    if (line.isEmpty())
      continue;

    if (mDebug)
      appendDebugLine(QString("CAM << %1").arg(QString::fromUtf8(line)));

    // Try parse JSON; if it fails, still show raw
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(line, &err);
    if (err.error != QJsonParseError::NoError)
    {
      ui->statusbar->showMessage("Camera RX (non-JSON): " + QString::fromUtf8(line));
      continue;
    }

    // Minimal Phase-1 behavior: improved state handling
    if (doc.isObject())
    {
      QJsonObject obj = doc.object();

      // ---------- ERROR ----------
      if (obj.contains("ok") && !obj["ok"].toBool())
      {
        QString err = obj.value("error").toString();
        ui->statusbar->showMessage("Camera error: " + err);

        mCamBusy = false;
        ui->camCaptureButton->setEnabled(mCamConnected);
        ui->camStatusButton->setEnabled(mCamConnected);
        return;
      }

      // ---------- CAPTURE COMPLETE ----------
      if (obj.contains("saved"))
      {
        ui->statusbar->showMessage("Camera: capture complete");

        mCamBusy = false;
        ui->camCaptureButton->setEnabled(mCamConnected);
        ui->camStatusButton->setEnabled(mCamConnected);
        return;
      }

      // ---------- GENERIC OK ----------
      if (obj.contains("ok"))
      {
        ui->statusbar->showMessage(
            QString("Camera: ok=%1")
                .arg(obj["ok"].toBool() ? "true" : "false"));
        return;
      }

      // ---------- STATUS ----------
      if (obj.contains("status"))
      {
        ui->statusbar->showMessage("Camera status received");
        return;
      }

      // ---------- FALLBACK ----------
      ui->statusbar->showMessage("Camera response received");
    }
  }
}

// =============================
// ASI Camera (Phase 1) handlers
// =============================

void MainWindow::camToggleConnect()
{
  // We treat "connected" as mCamSock->isOpen()
  const bool isOpen = (mCamSock && mCamSock->isOpen());

  QSettings settings("HyperK", "SigmaControl");
  settings.setValue("cameraEndpoint", ui->cameraEndpointEdit->text());

  if (!isOpen)
  {
    const QString ep = ui->cameraEndpointEdit->text().trimmed();
    if (openCameraTcp(ep))
    {
      ui->camConnectButton->setText("Disconnect Camera");
      setCameraUiEnabled(true);
    }
  }
  else
  {
    closeCameraTcp();
    ui->camConnectButton->setText("Connect Camera");
    setCameraUiEnabled(false);
  }
}

void MainWindow::camStatus()
{
  QJsonObject obj;
  obj["cmd"] = "status";
  sendCamJsonLine(obj);
}

void MainWindow::camCapture()
{
  if (mCamBusy)
    return;
  mCamBusy = true;

  ui->camCaptureButton->setEnabled(false);
  ui->camStatusButton->setEnabled(false);
  ui->statusbar->showMessage("Camera: capturing...");

  // Optional: push exposure/gain first
  {
    QJsonObject setObj;
    setObj["cmd"] = "set";
    setObj["exposure_us"] = ui->exposureUsSpin->value();
    setObj["gain"] = ui->gainSpin->value();
    sendCamJsonLine(setObj);
  }

  QJsonObject capObj;
  capObj["cmd"] = "capture";
  capObj["count"] = ui->countSpin->value();
  capObj["format"] = ui->formatCombo->currentText().trimmed();
  capObj["outdir"] = ui->outdirEdit->text().trimmed();
  capObj["prefix"] = ui->prefixEdit->text().trimmed();
  sendCamJsonLine(capObj);
}