#include "../include/MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serial(new QSerialPort(this)) {
    ui->setupUi(this);

    refreshPortList();

    reset();


    ui->debugBox->setReadOnly(true);

    
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
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

    connect(serial, &QSerialPort::errorOccurred, this, &MainWindow::handleSerialError);

}

MainWindow::~MainWindow() {
    if (serial->isOpen()) serial->close();
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

void MainWindow::reset(){
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
  
  mLensPos = 0;
  mAperture = 0;
  mDebug = false;
  logLines.clear();
  
  allowControl(false);
}

void MainWindow::refreshPortList() {
  ui->portComboBox->clear();

    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        // Option 1: Match by manufacturer or description
        QString desc = port.description().toLower();
        QString manufacturer = port.manufacturer().toLower();

        if (desc.contains("teensy") || manufacturer.contains("teensy")) {
            ui->portComboBox->addItem(port.portName());
        }
    }
}

void MainWindow::toggleConnect()
{
  if(ui->connectButton->text() == "Connect")
    {
      bool success = openSerialPort();
      if(success)
	{
	  
	  ui->connectButton->setText("Disconnect");
	}
    }
  else if(ui->connectButton->text() == "Disconnect")
    {
      bool success = closeSerialPort();
      if(success)
	{
	  ui->connectButton->setText("Connect");
	  reset();
	}
    }
  
}

bool MainWindow::closeSerialPort()
{
  if (serial->isOpen())
    {
      serial->close();
      ui->statusbar->showMessage("Disconnected");
      return true;
    }

  return false;
}

bool MainWindow::openSerialPort() {
  if (serial->isOpen()) serial->close();

    serial->setPortName(ui->portComboBox->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    if (!serial->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Error", "Failed to open port.");
        return false;
    }

    //Activate changing focus and aperture
    allowControl(true);

    ui->statusbar->showMessage("Connected to " + serial->portName());

    return true;
}


void MainWindow::setAperture() {
  QString cmd = "SA " + QString::number(ui->apertureValueSlider->value());

  sendCommand(cmd);
}

void MainWindow::toggleDebug() {
  mDebug = !mDebug;
  if(mDebug)
    {
      ui->debugButton->setText("| |");
    }
  else
    {
      ui->debugButton->setText("| >");
    }
}

void MainWindow::setFocus() {
  QString cmd = "SF " + QString::number(ui->focusValueSlider->value()); //ui->commandEdit->text().trimmed();
  
    sendCommand(cmd);
    
}

void MainWindow::togglePower() {
  QString cmd = "#$#?";
  if(ui->powerButton->text() == "Power On")
    {
      cmd = "ON";
    }
  else
    {
      cmd = "OFF";
    }
  
  sendCommand(cmd);
}

void MainWindow::sendCommand(QString cmd){
  if (!serial->isOpen()) {
    QMessageBox::critical(this, "Error", "Port is closed.");
    return;
  }
  
  if (cmd.isEmpty()) return;
  
  serial->write(( cmd + "\n").toUtf8());

  //display sent command
  if(mDebug)
    {
      appendDebugLine(">> " + cmd);
    }
}



void MainWindow::appendDebugLine(const QString& line, int maxLines) {
  // .cpp
  if (mDebug) {
    logLines << line;
    if (logLines.size() > maxLines)
      logLines.removeFirst();
    
    ui->debugBox->setPlainText(logLines.join('\n'));
    ui->debugBox->moveCursor(QTextCursor::End);
  }
}

void MainWindow::handleMessage(QString msg)
{
  QStringList parts = msg.split(',');
 
  QMap<QString, QString> keyValueMap;
  
  for (const QString &part : parts) {
    if (part.contains(':')) {
      QStringList pair = part.split(':', Qt::SkipEmptyParts);
      if (pair.size() == 2) {
	keyValueMap.insert(pair[0], pair[1]);
      }
    } else {
      // Handle free-form message (no colon)
      keyValueMap.insert("Message", part);
    }
  }

  QString lensState = keyValueMap.value("S");
  if(lensState == "Off")
    {
      ui->powerButton->setText("Power On");
      //mPowerState = false;
    }
  else
    {
      ui->powerButton->setText("Shut Down");
      //mPowerState = true;
    }
  
  // Example: Access the values
  uint16_t curAperture = static_cast<uint16_t>(keyValueMap.value("A").toInt());
  uint16_t curLensPos = static_cast<uint16_t>(keyValueMap.value("F").toInt());
    
  ui->cameraIDLabel->setText(keyValueMap.value("ID"));
  ui->curStateLabel->setText(keyValueMap.value("S"));
  
  if(curAperture != mAperture)
    {
      mAperture = curAperture;
      ui->curApertureLabel->setText("f/" + QString::number(getFNumber(mAperture)) + " | 0x"+QString::number(mAperture, 16).toUpper());
    }

  if(curLensPos != mLensPos)
    {
      mLensPos = curLensPos;
      ui->curFocusLabel->setText(getFocus(mLensPos)+" m | 0x" + QString::number(mLensPos, 16).toUpper());
    }
}

void MainWindow::readSerialData() {
  QByteArray data = serial->readAll();
  if(mDebug)
    {
      appendDebugLine("<< " + QString::fromUtf8(data));
    }
  
  handleMessage(QString::fromUtf8(data));
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "About", "Author: Tapendra BC \nWritten in: Summer 2025 \nU of W \nClient GUI program to set the focus and the aperture of sigma Art lens using Teensy 4.0 microcontroller. Writen in QT framework. Email: tapendra.320@gmail.com. Supervisor: Dr. Blair Jameison. Source Code: https://github.com/Tapendra-195/Sigma_Lens_Control");
}


QString MainWindow::getFocus(uint16_t lensPos)
{
  float v = 5.518144e-5*lensPos - 0.381395; //image distance
  float focus = 3*v/(20*v-3); //find object distance(NOT THE FOCAL LENGTH, WHICH IS A CONSTANT, 15mm)
  QString focusToDisplay = (focus>0)?QString::number(focus):"infinity";

  return focusToDisplay;
}

float MainWindow::getFNumber(uint16_t aperture)
{
  float fNumber = pow(sqrt(2), (aperture/256.0) - 16);
  
  return fNumber;
}

void MainWindow::updateAperture(){
  uint16_t aperture = ui->apertureValueSlider->value();
  ui->apertureValueLabel->setText("f/" + QString::number(getFNumber(aperture)) + " | 0x"+QString::number(aperture,16).toUpper());

}

void MainWindow::updateFocus(){
  int lensPos = ui->focusValueSlider->value();
  ui->focusValueLabel->setText(getFocus(lensPos)+" m | 0x" + QString::number(lensPos, 16).toUpper());
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
  if (error == QSerialPort::ResourceError) {
    QMessageBox::critical(this, "Error", "Either the Lens or Teensy is Disconnected. Click Refresh");
    ui->statusbar->showMessage("Disconnected");
    reset();
  }
}
