/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "DevicePanel.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_root;
    QHBoxLayout *horizontalLayout_top;
    QGroupBox *group_status;
    QGridLayout *grid_status;
    QLabel *label_statusHeader;
    QLabel *label_col1;
    QLabel *label_col2;
    QLabel *label_col3;
    QLabel *label_col4;
    QLabel *label_col5;
    QLabel *label_col6;
    QLabel *label_col7;
    QLabel *label_col8;
    QLabel *label_lensRow;
    QFrame *led_lens1;
    QFrame *led_lens2;
    QFrame *led_lens3;
    QFrame *led_lens4;
    QFrame *led_lens5;
    QFrame *led_lens6;
    QFrame *led_lens7;
    QFrame *led_lens8;
    QLabel *label_camRow;
    QFrame *led_cam1;
    QFrame *led_cam2;
    QFrame *led_cam3;
    QFrame *led_cam4;
    QFrame *led_cam5;
    QFrame *led_cam6;
    QFrame *led_cam7;
    QFrame *led_cam8;
    QGroupBox *group_global;
    QHBoxLayout *horizontalLayout_global;
    QCheckBox *check_onlyConnected;
    QCheckBox *check_skipBusy;
    QPushButton *btn_applyCameraToAll;
    QPushButton *btn_applyLensToAll;
    QPushButton *btn_applyAllToAll;
    QPushButton *btn_captureAll;
    QProgressBar *progress_captureAll;
    QLabel *label_captureAllStatus;
    QSplitter *splitter_main;
    QTabWidget *tabWidget_devices;
    QWidget *tab_slot1;
    QVBoxLayout *verticalLayout_slot1;
    DevicePanel *devicePanel1;
    QWidget *tab_slot2;
    QVBoxLayout *verticalLayout_slot2;
    DevicePanel *devicePanel2;
    QWidget *tab_slot3;
    QVBoxLayout *verticalLayout_slot3;
    DevicePanel *devicePanel3;
    QWidget *tab_slot4;
    QVBoxLayout *verticalLayout_slot4;
    DevicePanel *devicePanel4;
    QWidget *tab_slot5;
    QVBoxLayout *verticalLayout_slot5;
    DevicePanel *devicePanel5;
    QWidget *tab_slot6;
    QVBoxLayout *verticalLayout_slot6;
    DevicePanel *devicePanel6;
    QWidget *tab_slot7;
    QVBoxLayout *verticalLayout_slot7;
    DevicePanel *devicePanel7;
    QWidget *tab_slot8;
    QVBoxLayout *verticalLayout_slot8;
    DevicePanel *devicePanel8;
    QWidget *widget_preview;
    QVBoxLayout *verticalLayout_preview;
    QGroupBox *group_preview;
    QVBoxLayout *verticalLayout_previewBox;
    QLabel *label_mainPreview;
    QGroupBox *group_debug;
    QVBoxLayout *verticalLayout_debugBox;
    QPlainTextEdit *text_mainDebug;
    QMenuBar *menubar;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 850);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_root = new QVBoxLayout(centralwidget);
        verticalLayout_root->setSpacing(8);
        verticalLayout_root->setObjectName("verticalLayout_root");
        verticalLayout_root->setContentsMargins(8, 8, 8, 8);
        horizontalLayout_top = new QHBoxLayout();
        horizontalLayout_top->setSpacing(10);
        horizontalLayout_top->setObjectName("horizontalLayout_top");
        group_status = new QGroupBox(centralwidget);
        group_status->setObjectName("group_status");
        grid_status = new QGridLayout(group_status);
        grid_status->setObjectName("grid_status");
        grid_status->setHorizontalSpacing(6);
        grid_status->setVerticalSpacing(4);
        label_statusHeader = new QLabel(group_status);
        label_statusHeader->setObjectName("label_statusHeader");

        grid_status->addWidget(label_statusHeader, 0, 0, 1, 1);

        label_col1 = new QLabel(group_status);
        label_col1->setObjectName("label_col1");
        label_col1->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col1, 0, 1, 1, 1);

        label_col2 = new QLabel(group_status);
        label_col2->setObjectName("label_col2");
        label_col2->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col2, 0, 2, 1, 1);

        label_col3 = new QLabel(group_status);
        label_col3->setObjectName("label_col3");
        label_col3->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col3, 0, 3, 1, 1);

        label_col4 = new QLabel(group_status);
        label_col4->setObjectName("label_col4");
        label_col4->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col4, 0, 4, 1, 1);

        label_col5 = new QLabel(group_status);
        label_col5->setObjectName("label_col5");
        label_col5->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col5, 0, 5, 1, 1);

        label_col6 = new QLabel(group_status);
        label_col6->setObjectName("label_col6");
        label_col6->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col6, 0, 6, 1, 1);

        label_col7 = new QLabel(group_status);
        label_col7->setObjectName("label_col7");
        label_col7->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col7, 0, 7, 1, 1);

        label_col8 = new QLabel(group_status);
        label_col8->setObjectName("label_col8");
        label_col8->setAlignment(Qt::AlignCenter);

        grid_status->addWidget(label_col8, 0, 8, 1, 1);

        label_lensRow = new QLabel(group_status);
        label_lensRow->setObjectName("label_lensRow");

        grid_status->addWidget(label_lensRow, 1, 0, 1, 1);

        led_lens1 = new QFrame(group_status);
        led_lens1->setObjectName("led_lens1");
        led_lens1->setMinimumSize(QSize(14, 14));
        led_lens1->setMaximumSize(QSize(14, 14));
        led_lens1->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens1, 1, 1, 1, 1);

        led_lens2 = new QFrame(group_status);
        led_lens2->setObjectName("led_lens2");
        led_lens2->setMinimumSize(QSize(14, 14));
        led_lens2->setMaximumSize(QSize(14, 14));
        led_lens2->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens2, 1, 2, 1, 1);

        led_lens3 = new QFrame(group_status);
        led_lens3->setObjectName("led_lens3");
        led_lens3->setMinimumSize(QSize(14, 14));
        led_lens3->setMaximumSize(QSize(14, 14));
        led_lens3->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens3, 1, 3, 1, 1);

        led_lens4 = new QFrame(group_status);
        led_lens4->setObjectName("led_lens4");
        led_lens4->setMinimumSize(QSize(14, 14));
        led_lens4->setMaximumSize(QSize(14, 14));
        led_lens4->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens4, 1, 4, 1, 1);

        led_lens5 = new QFrame(group_status);
        led_lens5->setObjectName("led_lens5");
        led_lens5->setMinimumSize(QSize(14, 14));
        led_lens5->setMaximumSize(QSize(14, 14));
        led_lens5->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens5, 1, 5, 1, 1);

        led_lens6 = new QFrame(group_status);
        led_lens6->setObjectName("led_lens6");
        led_lens6->setMinimumSize(QSize(14, 14));
        led_lens6->setMaximumSize(QSize(14, 14));
        led_lens6->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens6, 1, 6, 1, 1);

        led_lens7 = new QFrame(group_status);
        led_lens7->setObjectName("led_lens7");
        led_lens7->setMinimumSize(QSize(14, 14));
        led_lens7->setMaximumSize(QSize(14, 14));
        led_lens7->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens7, 1, 7, 1, 1);

        led_lens8 = new QFrame(group_status);
        led_lens8->setObjectName("led_lens8");
        led_lens8->setMinimumSize(QSize(14, 14));
        led_lens8->setMaximumSize(QSize(14, 14));
        led_lens8->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_lens8, 1, 8, 1, 1);

        label_camRow = new QLabel(group_status);
        label_camRow->setObjectName("label_camRow");

        grid_status->addWidget(label_camRow, 2, 0, 1, 1);

        led_cam1 = new QFrame(group_status);
        led_cam1->setObjectName("led_cam1");
        led_cam1->setMinimumSize(QSize(14, 14));
        led_cam1->setMaximumSize(QSize(14, 14));
        led_cam1->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam1, 2, 1, 1, 1);

        led_cam2 = new QFrame(group_status);
        led_cam2->setObjectName("led_cam2");
        led_cam2->setMinimumSize(QSize(14, 14));
        led_cam2->setMaximumSize(QSize(14, 14));
        led_cam2->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam2, 2, 2, 1, 1);

        led_cam3 = new QFrame(group_status);
        led_cam3->setObjectName("led_cam3");
        led_cam3->setMinimumSize(QSize(14, 14));
        led_cam3->setMaximumSize(QSize(14, 14));
        led_cam3->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam3, 2, 3, 1, 1);

        led_cam4 = new QFrame(group_status);
        led_cam4->setObjectName("led_cam4");
        led_cam4->setMinimumSize(QSize(14, 14));
        led_cam4->setMaximumSize(QSize(14, 14));
        led_cam4->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam4, 2, 4, 1, 1);

        led_cam5 = new QFrame(group_status);
        led_cam5->setObjectName("led_cam5");
        led_cam5->setMinimumSize(QSize(14, 14));
        led_cam5->setMaximumSize(QSize(14, 14));
        led_cam5->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam5, 2, 5, 1, 1);

        led_cam6 = new QFrame(group_status);
        led_cam6->setObjectName("led_cam6");
        led_cam6->setMinimumSize(QSize(14, 14));
        led_cam6->setMaximumSize(QSize(14, 14));
        led_cam6->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam6, 2, 6, 1, 1);

        led_cam7 = new QFrame(group_status);
        led_cam7->setObjectName("led_cam7");
        led_cam7->setMinimumSize(QSize(14, 14));
        led_cam7->setMaximumSize(QSize(14, 14));
        led_cam7->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam7, 2, 7, 1, 1);

        led_cam8 = new QFrame(group_status);
        led_cam8->setObjectName("led_cam8");
        led_cam8->setMinimumSize(QSize(14, 14));
        led_cam8->setMaximumSize(QSize(14, 14));
        led_cam8->setStyleSheet(QString::fromUtf8("background: #111; border: 1px solid #333; border-radius: 7px;"));

        grid_status->addWidget(led_cam8, 2, 8, 1, 1);


        horizontalLayout_top->addWidget(group_status);

        group_global = new QGroupBox(centralwidget);
        group_global->setObjectName("group_global");
        horizontalLayout_global = new QHBoxLayout(group_global);
        horizontalLayout_global->setSpacing(8);
        horizontalLayout_global->setObjectName("horizontalLayout_global");
        check_onlyConnected = new QCheckBox(group_global);
        check_onlyConnected->setObjectName("check_onlyConnected");
        check_onlyConnected->setChecked(true);

        horizontalLayout_global->addWidget(check_onlyConnected);

        check_skipBusy = new QCheckBox(group_global);
        check_skipBusy->setObjectName("check_skipBusy");
        check_skipBusy->setChecked(true);

        horizontalLayout_global->addWidget(check_skipBusy);

        btn_applyCameraToAll = new QPushButton(group_global);
        btn_applyCameraToAll->setObjectName("btn_applyCameraToAll");

        horizontalLayout_global->addWidget(btn_applyCameraToAll);

        btn_applyLensToAll = new QPushButton(group_global);
        btn_applyLensToAll->setObjectName("btn_applyLensToAll");

        horizontalLayout_global->addWidget(btn_applyLensToAll);

        btn_applyAllToAll = new QPushButton(group_global);
        btn_applyAllToAll->setObjectName("btn_applyAllToAll");

        horizontalLayout_global->addWidget(btn_applyAllToAll);

        btn_captureAll = new QPushButton(group_global);
        btn_captureAll->setObjectName("btn_captureAll");

        horizontalLayout_global->addWidget(btn_captureAll);

        progress_captureAll = new QProgressBar(group_global);
        progress_captureAll->setObjectName("progress_captureAll");
        progress_captureAll->setMaximum(100);
        progress_captureAll->setValue(0);
        progress_captureAll->setTextVisible(true);
        progress_captureAll->setMinimumWidth(140);

        horizontalLayout_global->addWidget(progress_captureAll);

        label_captureAllStatus = new QLabel(group_global);
        label_captureAllStatus->setObjectName("label_captureAllStatus");
        label_captureAllStatus->setMinimumWidth(160);

        horizontalLayout_global->addWidget(label_captureAllStatus);


        horizontalLayout_top->addWidget(group_global);


        verticalLayout_root->addLayout(horizontalLayout_top);

        splitter_main = new QSplitter(centralwidget);
        splitter_main->setObjectName("splitter_main");
        splitter_main->setOrientation(Qt::Horizontal);
        splitter_main->setChildrenCollapsible(false);
        tabWidget_devices = new QTabWidget(splitter_main);
        tabWidget_devices->setObjectName("tabWidget_devices");
        tab_slot1 = new QWidget();
        tab_slot1->setObjectName("tab_slot1");
        verticalLayout_slot1 = new QVBoxLayout(tab_slot1);
        verticalLayout_slot1->setObjectName("verticalLayout_slot1");
        devicePanel1 = new DevicePanel(tab_slot1);
        devicePanel1->setObjectName("devicePanel1");

        verticalLayout_slot1->addWidget(devicePanel1);

        tabWidget_devices->addTab(tab_slot1, QString());
        tab_slot2 = new QWidget();
        tab_slot2->setObjectName("tab_slot2");
        verticalLayout_slot2 = new QVBoxLayout(tab_slot2);
        verticalLayout_slot2->setObjectName("verticalLayout_slot2");
        devicePanel2 = new DevicePanel(tab_slot2);
        devicePanel2->setObjectName("devicePanel2");

        verticalLayout_slot2->addWidget(devicePanel2);

        tabWidget_devices->addTab(tab_slot2, QString());
        tab_slot3 = new QWidget();
        tab_slot3->setObjectName("tab_slot3");
        verticalLayout_slot3 = new QVBoxLayout(tab_slot3);
        verticalLayout_slot3->setObjectName("verticalLayout_slot3");
        devicePanel3 = new DevicePanel(tab_slot3);
        devicePanel3->setObjectName("devicePanel3");

        verticalLayout_slot3->addWidget(devicePanel3);

        tabWidget_devices->addTab(tab_slot3, QString());
        tab_slot4 = new QWidget();
        tab_slot4->setObjectName("tab_slot4");
        verticalLayout_slot4 = new QVBoxLayout(tab_slot4);
        verticalLayout_slot4->setObjectName("verticalLayout_slot4");
        devicePanel4 = new DevicePanel(tab_slot4);
        devicePanel4->setObjectName("devicePanel4");

        verticalLayout_slot4->addWidget(devicePanel4);

        tabWidget_devices->addTab(tab_slot4, QString());
        tab_slot5 = new QWidget();
        tab_slot5->setObjectName("tab_slot5");
        verticalLayout_slot5 = new QVBoxLayout(tab_slot5);
        verticalLayout_slot5->setObjectName("verticalLayout_slot5");
        devicePanel5 = new DevicePanel(tab_slot5);
        devicePanel5->setObjectName("devicePanel5");

        verticalLayout_slot5->addWidget(devicePanel5);

        tabWidget_devices->addTab(tab_slot5, QString());
        tab_slot6 = new QWidget();
        tab_slot6->setObjectName("tab_slot6");
        verticalLayout_slot6 = new QVBoxLayout(tab_slot6);
        verticalLayout_slot6->setObjectName("verticalLayout_slot6");
        devicePanel6 = new DevicePanel(tab_slot6);
        devicePanel6->setObjectName("devicePanel6");

        verticalLayout_slot6->addWidget(devicePanel6);

        tabWidget_devices->addTab(tab_slot6, QString());
        tab_slot7 = new QWidget();
        tab_slot7->setObjectName("tab_slot7");
        verticalLayout_slot7 = new QVBoxLayout(tab_slot7);
        verticalLayout_slot7->setObjectName("verticalLayout_slot7");
        devicePanel7 = new DevicePanel(tab_slot7);
        devicePanel7->setObjectName("devicePanel7");

        verticalLayout_slot7->addWidget(devicePanel7);

        tabWidget_devices->addTab(tab_slot7, QString());
        tab_slot8 = new QWidget();
        tab_slot8->setObjectName("tab_slot8");
        verticalLayout_slot8 = new QVBoxLayout(tab_slot8);
        verticalLayout_slot8->setObjectName("verticalLayout_slot8");
        devicePanel8 = new DevicePanel(tab_slot8);
        devicePanel8->setObjectName("devicePanel8");

        verticalLayout_slot8->addWidget(devicePanel8);

        tabWidget_devices->addTab(tab_slot8, QString());
        splitter_main->addWidget(tabWidget_devices);
        widget_preview = new QWidget(splitter_main);
        widget_preview->setObjectName("widget_preview");
        verticalLayout_preview = new QVBoxLayout(widget_preview);
        verticalLayout_preview->setObjectName("verticalLayout_preview");
        verticalLayout_preview->setContentsMargins(0, 0, 0, 0);
        group_preview = new QGroupBox(widget_preview);
        group_preview->setObjectName("group_preview");
        verticalLayout_previewBox = new QVBoxLayout(group_preview);
        verticalLayout_previewBox->setObjectName("verticalLayout_previewBox");
        label_mainPreview = new QLabel(group_preview);
        label_mainPreview->setObjectName("label_mainPreview");
        label_mainPreview->setAlignment(Qt::AlignCenter);
        label_mainPreview->setMinimumSize(QSize(420, 420));
        label_mainPreview->setFrameShape(QFrame::StyledPanel);
        label_mainPreview->setScaledContents(true);

        verticalLayout_previewBox->addWidget(label_mainPreview);


        verticalLayout_preview->addWidget(group_preview);

        group_debug = new QGroupBox(widget_preview);
        group_debug->setObjectName("group_debug");
        verticalLayout_debugBox = new QVBoxLayout(group_debug);
        verticalLayout_debugBox->setObjectName("verticalLayout_debugBox");
        text_mainDebug = new QPlainTextEdit(group_debug);
        text_mainDebug->setObjectName("text_mainDebug");
        text_mainDebug->setReadOnly(true);
        text_mainDebug->setMinimumSize(QSize(420, 180));

        verticalLayout_debugBox->addWidget(text_mainDebug);


        verticalLayout_preview->addWidget(group_debug);

        splitter_main->addWidget(widget_preview);

        verticalLayout_root->addWidget(splitter_main);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        tabWidget_devices->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\316\243 Control", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        group_status->setTitle(QCoreApplication::translate("MainWindow", "Status", nullptr));
        label_statusHeader->setText(QString());
        label_col1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label_col2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_col3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        label_col4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        label_col5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        label_col6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        label_col7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        label_col8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        label_lensRow->setText(QCoreApplication::translate("MainWindow", "Lens", nullptr));
        label_camRow->setText(QCoreApplication::translate("MainWindow", "Cam", nullptr));
        group_global->setTitle(QCoreApplication::translate("MainWindow", "Global controls", nullptr));
        check_onlyConnected->setText(QCoreApplication::translate("MainWindow", "Only connected", nullptr));
        check_skipBusy->setText(QCoreApplication::translate("MainWindow", "Skip busy", nullptr));
        btn_applyCameraToAll->setText(QCoreApplication::translate("MainWindow", "Apply camera settings", nullptr));
        btn_applyLensToAll->setText(QCoreApplication::translate("MainWindow", "Apply lens settings", nullptr));
        btn_applyAllToAll->setText(QCoreApplication::translate("MainWindow", "Apply all settings", nullptr));
        btn_captureAll->setText(QCoreApplication::translate("MainWindow", "Capture all", nullptr));
        label_captureAllStatus->setText(QString());
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot1), QCoreApplication::translate("MainWindow", "Slot 1", nullptr));
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot2), QCoreApplication::translate("MainWindow", "Slot 2", nullptr));
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot3), QCoreApplication::translate("MainWindow", "Slot 3", nullptr));
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot4), QCoreApplication::translate("MainWindow", "Slot 4", nullptr));
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot5), QCoreApplication::translate("MainWindow", "Slot 5", nullptr));
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot6), QCoreApplication::translate("MainWindow", "Slot 6", nullptr));
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot7), QCoreApplication::translate("MainWindow", "Slot 7", nullptr));
        tabWidget_devices->setTabText(tabWidget_devices->indexOf(tab_slot8), QCoreApplication::translate("MainWindow", "Slot 8", nullptr));
        group_preview->setTitle(QCoreApplication::translate("MainWindow", "Preview", nullptr));
        label_mainPreview->setText(QCoreApplication::translate("MainWindow", "Preview will appear here (active slot).", nullptr));
        group_debug->setTitle(QCoreApplication::translate("MainWindow", "Debug Output", nullptr));
        text_mainDebug->setPlaceholderText(QCoreApplication::translate("MainWindow", "Bridge logs, capture messages, and active-slot debug output can appear here.", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
