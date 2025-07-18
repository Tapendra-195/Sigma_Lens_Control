#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <math.h>
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
  
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
	       
private slots:
  
  void toggleConnect();
  //  void sendCommand();
  void readSerialData();
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
  void handleMessage(QString msg);//Parses message received from lens and sets values.
  void appendDebugLine(const QString& line, int maxLines = 2000);
  QString getFocus(uint16_t lensPos);
  float getFNumber(uint16_t aperture);
  bool mDebug = false;
  bool mPowerState = true;//indicates on
  uint16_t mLensPos = 0;
  uint16_t mAperture = 0;
  QString mCameraID = "Unknown";
  QStringList logLines;
  const int maxLines = 500;
  Ui::MainWindow *ui;
  QSerialPort *serial;
  void refreshPortList();  
};
#endif // MAINWINDOW_H
