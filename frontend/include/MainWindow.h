#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTcpSocket>
#include <QIODevice>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <math.h>
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui
{
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:

  void toggleConnect();
  void showAbout();
  void updateAperture();
  void updateFocus();
  void setAperture();
  void setFocus();
  void togglePower();
  void incrementAperture();
  void decrementAperture();
  void incrementFocus();
  void decrementFocus();
  void handleSerialError(QSerialPort::SerialPortError error);
  void toggleDebug();

private:
  void reset();
  void allowControl(bool value);
  bool openSerialPort();
  bool closeSerialPort();
  void sendCommand(QString cmd);
  void handleMessage(QString msg); // Parses message received from lens and sets values.
  void appendDebugLine(const QString &line, int maxLines = 2000);
  QString getFocus(uint16_t lensPos);
  float getFNumber(uint16_t aperture);
  bool mDebug = false;
  bool mPowerState = true; // indicates on
  uint16_t mLensPos = 0;
  uint16_t mAperture = 0;
  QString mCameraID = "Unknown";
  QStringList logLines;
  const int maxLines = 500;
  Ui::MainWindow *ui;

  QSerialPort *mSerial = nullptr;
  QTcpSocket *mSock = nullptr;
  QIODevice *io = nullptr; // active transport
  QString rxBuf;           // for newline buffering

  // Telemetry cache
  QMap<QString, QString> mLastKV;
  bool mHaveTelemetry = false;

  // slow UI update timer
  QTimer *mTelemetryTimer = nullptr;

  // helper: display connection name
  QString connectionLabel() const;

  void refreshPortList();

  // Streaming receive state (for capture_stream)
  bool mCamExpectingBinary = false;
  int  mCamExpectBytes = 0;
  QByteArray mCamBinBuf;

  int mCamStreamW = 0;
  int mCamStreamH = 0;
  QString mCamStreamOutDir;
  QString mCamStreamPrefix;
  QString mCamLastFormat;

    // --- ASI camera TCP ---
  QTcpSocket *mCamSock = nullptr;
  QByteArray mCamRxBuf;
  bool mCamBusy = false;
  bool mCamConnected = false;

  bool openCameraTcp(const QString &endpoint);   // tcp:HOST:PORT
  void closeCameraTcp();
  void sendCamJsonLine(const QJsonObject &obj);
  void setCameraUiEnabled(bool en);

  private slots:
    void onIoReadyRead();
    void updateTelemetryUi();
    // --- Camera (ASI) ---
    void camToggleConnect();
    void camStatus();
    void camCapture();
    void onCamReadyRead();  
};
#endif // MAINWINDOW_H
