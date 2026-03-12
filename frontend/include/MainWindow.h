#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "DevicePanel.h"
#include <QEvent>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void showAbout();

  // Global controls
  void applyCameraToAll();
  void applyLensToAll();
  void applyAllToAll();
  void captureAll();

  // DevicePanel signals
  void onLensLedChanged(int slot1, DevicePanel::LedState state);
  void onCameraLedChanged(int slot1, DevicePanel::LedState state);
  void onPanelStatusMessage(int slot1, const QString& msg);
  void onPanelCaptureSaved(int slot1, const QString& path);
  void onPanelPreviewSaved(int slot1, const QString& path);
  
private:
  void initPanels();
  DevicePanel* panelAtTab(int tabIndex) const;
  DevicePanel* panelBySlot1(int slot1) const;

  void setLed(QWidget* w, DevicePanel::LedState st);

  void updateCaptureAllEnabled();

  Ui::MainWindow *ui = nullptr;

  QVector<DevicePanel*> mPanels; // size 8, slot1 = index+1

  // capture-all progress bookkeeping
  int mCaptureAllTotal = 0;
  int mCaptureAllDone  = 0;

  QString mLastCapturedTiffPath;

  protected:
  bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif
