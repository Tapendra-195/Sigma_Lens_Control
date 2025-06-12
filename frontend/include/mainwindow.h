#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
  
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
	       
private slots:
  void openSerialPort();
  //  void sendCommand();
  void readSerialData();
  void showAbout();
  void updateAperture();
  void updateFocus();
  void reset();
  void allowControl(bool value);
  void setAperture();
  void setFocus();
  void sendCommand(QString cmd);
  void onPowerOn();
  void onShutdown();
private:
  Ui::MainWindow *ui;
  QSerialPort *serial;
  void refreshPortList();
  
  struct ApertureMap{
    float humanAperture; //human readable aperture
    uint16_t lensAperture; //aperture the lens understands
  };
  
  static constexpr std::array<ApertureMap, 22> apertureTable = {{ {16.0f, 0x1800},
								  {14.0f, 0x17AA},
								  {13.0f, 0x1755},
								  {11.0f, 0x1700},
								  {10.0f, 0x16AA},
								  {9.0f, 0x1655},
								  {8.0f, 0x1600},
								  {7.1f, 0x15AA},
								  {6.3f, 0x1555},
								  {5.6f, 0x1500},
								  {5.0f, 0x14AA},
								  {4.5f, 0x1455},
								  {4.0f, 0x1400},
								  {3.5f, 0x13AA},
								  {3.2f, 0x1355},
								  {2.8f, 0x1300},
								  {2.5f, 0x12AA},
								  {2.2f, 0x1255},
								  {2.0f, 0x1200},
								  {1.8f, 0x11AA},
								  {1.6f, 0x1155},
								  {1.4f, 0x1100} }};
  
  
};
#endif // MAINWINDOW_H
