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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionThis_program_was_written_by_Tapendra_sldkfjlksdjflk_slkdfj_lksjdf_lsakdfjlk_jsdf_slkdfjlksd_jfa;
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QFormLayout *formLayout;
    QFrame *frame_2;
    QFormLayout *formLayout_2;
    QLabel *label_2;
    QLabel *curFocusLabel;
    QLabel *label_3;
    QLabel *curApertureLabel;
    QLabel *label_temp;
    QLabel *curTempLabel;
    QLabel *label_pressure;
    QLabel *curPressureLabel;
    QLabel *label_humidity;
    QLabel *curHumidityLabel;
    QLabel *label_accel;
    QLabel *curAccelLabel;
    QLabel *label_gyro;
    QLabel *curGyroLabel;
    QLabel *label_mag;
    QLabel *curMagLabel;
    QLabel *label_bmag;
    QLabel *curBmagLabel;
    QLabel *label_heading;
    QLabel *curHeadingLabel;
    QFrame *frame_7;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *powerButton;
    QFrame *frame_6;
    QFormLayout *formLayout_4;
    QLabel *label;
    QComboBox *portComboBox;
    QPushButton *refreshButton;
    QPushButton *connectButton;
    QLabel *label_6;
    QLabel *cameraIDLabel;
    QFrame *frame_5;
    QFormLayout *formLayout_3;
    QLabel *apertureValueLabel;
    QLabel *label_4;
    QFrame *frame_9;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *decrementApertureButton;
    QSlider *apertureValueSlider;
    QPushButton *incrementApertureButton;
    QPushButton *setApertureButton;
    QLabel *focusValueLabel;
    QLabel *label_5;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *decrementFocusButton;
    QSlider *focusValueSlider;
    QPushButton *incrementFocusButton;
    QPushButton *setFocusButton;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_8;
    QLabel *curStateLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_7;
    QPushButton *debugButton;
    QPlainTextEdit *debugBox;
    QMenuBar *menubar;
    QMenu *menuAbout;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(844, 719);
        MainWindow->setMinimumSize(QSize(600, 500));
        actionThis_program_was_written_by_Tapendra_sldkfjlksdjflk_slkdfj_lksjdf_lsakdfjlk_jsdf_slkdfjlksd_jfa = new QAction(MainWindow);
        actionThis_program_was_written_by_Tapendra_sldkfjlksdjflk_slkdfj_lksjdf_lsakdfjlk_jsdf_slkdfjlksd_jfa->setObjectName("actionThis_program_was_written_by_Tapendra_sldkfjlksdjflk_slkdfj_lksjdf_lsakdfjlk_jsdf_slkdfjlksd_jfa");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(frame);
        formLayout->setObjectName("formLayout");
        frame_2 = new QFrame(frame);
        frame_2->setObjectName("frame_2");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy1);
        frame_2->setMinimumSize(QSize(200, 0));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Plain);
        formLayout_2 = new QFormLayout(frame_2);
        formLayout_2->setObjectName("formLayout_2");
        label_2 = new QLabel(frame_2);
        label_2->setObjectName("label_2");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, label_2);

        curFocusLabel = new QLabel(frame_2);
        curFocusLabel->setObjectName("curFocusLabel");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, curFocusLabel);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName("label_3");

        formLayout_2->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        curApertureLabel = new QLabel(frame_2);
        curApertureLabel->setObjectName("curApertureLabel");

        formLayout_2->setWidget(2, QFormLayout::ItemRole::FieldRole, curApertureLabel);

        label_temp = new QLabel(frame_2);
        label_temp->setObjectName("label_temp");

        formLayout_2->setWidget(3, QFormLayout::ItemRole::LabelRole, label_temp);

        curTempLabel = new QLabel(frame_2);
        curTempLabel->setObjectName("curTempLabel");

        formLayout_2->setWidget(3, QFormLayout::ItemRole::FieldRole, curTempLabel);

        label_pressure = new QLabel(frame_2);
        label_pressure->setObjectName("label_pressure");

        formLayout_2->setWidget(4, QFormLayout::ItemRole::LabelRole, label_pressure);

        curPressureLabel = new QLabel(frame_2);
        curPressureLabel->setObjectName("curPressureLabel");

        formLayout_2->setWidget(4, QFormLayout::ItemRole::FieldRole, curPressureLabel);

        label_humidity = new QLabel(frame_2);
        label_humidity->setObjectName("label_humidity");

        formLayout_2->setWidget(5, QFormLayout::ItemRole::LabelRole, label_humidity);

        curHumidityLabel = new QLabel(frame_2);
        curHumidityLabel->setObjectName("curHumidityLabel");

        formLayout_2->setWidget(5, QFormLayout::ItemRole::FieldRole, curHumidityLabel);

        label_accel = new QLabel(frame_2);
        label_accel->setObjectName("label_accel");

        formLayout_2->setWidget(6, QFormLayout::ItemRole::LabelRole, label_accel);

        curAccelLabel = new QLabel(frame_2);
        curAccelLabel->setObjectName("curAccelLabel");

        formLayout_2->setWidget(6, QFormLayout::ItemRole::FieldRole, curAccelLabel);

        label_gyro = new QLabel(frame_2);
        label_gyro->setObjectName("label_gyro");

        formLayout_2->setWidget(7, QFormLayout::ItemRole::LabelRole, label_gyro);

        curGyroLabel = new QLabel(frame_2);
        curGyroLabel->setObjectName("curGyroLabel");

        formLayout_2->setWidget(7, QFormLayout::ItemRole::FieldRole, curGyroLabel);

        label_mag = new QLabel(frame_2);
        label_mag->setObjectName("label_mag");

        formLayout_2->setWidget(8, QFormLayout::ItemRole::LabelRole, label_mag);

        curMagLabel = new QLabel(frame_2);
        curMagLabel->setObjectName("curMagLabel");

        formLayout_2->setWidget(8, QFormLayout::ItemRole::FieldRole, curMagLabel);

        label_bmag = new QLabel(frame_2);
        label_bmag->setObjectName("label_bmag");

        formLayout_2->setWidget(9, QFormLayout::ItemRole::LabelRole, label_bmag);

        curBmagLabel = new QLabel(frame_2);
        curBmagLabel->setObjectName("curBmagLabel");

        formLayout_2->setWidget(9, QFormLayout::ItemRole::FieldRole, curBmagLabel);

        label_heading = new QLabel(frame_2);
        label_heading->setObjectName("label_heading");

        formLayout_2->setWidget(10, QFormLayout::ItemRole::LabelRole, label_heading);

        curHeadingLabel = new QLabel(frame_2);
        curHeadingLabel->setObjectName("curHeadingLabel");

        formLayout_2->setWidget(10, QFormLayout::ItemRole::FieldRole, curHeadingLabel);


        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, frame_2);

        frame_7 = new QFrame(frame);
        frame_7->setObjectName("frame_7");
        frame_7->setFrameShape(QFrame::NoFrame);
        frame_7->setFrameShadow(QFrame::Plain);
        horizontalLayout_3 = new QHBoxLayout(frame_7);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer = new QSpacerItem(325, 17, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        powerButton = new QPushButton(frame_7);
        powerButton->setObjectName("powerButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(powerButton->sizePolicy().hasHeightForWidth());
        powerButton->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(powerButton);


        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, frame_7);

        frame_6 = new QFrame(frame);
        frame_6->setObjectName("frame_6");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy3);
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Plain);
        formLayout_4 = new QFormLayout(frame_6);
        formLayout_4->setObjectName("formLayout_4");
        label = new QLabel(frame_6);
        label->setObjectName("label");

        formLayout_4->setWidget(1, QFormLayout::ItemRole::LabelRole, label);

        portComboBox = new QComboBox(frame_6);
        portComboBox->setObjectName("portComboBox");

        formLayout_4->setWidget(1, QFormLayout::ItemRole::FieldRole, portComboBox);

        refreshButton = new QPushButton(frame_6);
        refreshButton->setObjectName("refreshButton");

        formLayout_4->setWidget(3, QFormLayout::ItemRole::LabelRole, refreshButton);

        connectButton = new QPushButton(frame_6);
        connectButton->setObjectName("connectButton");

        formLayout_4->setWidget(3, QFormLayout::ItemRole::FieldRole, connectButton);

        label_6 = new QLabel(frame_6);
        label_6->setObjectName("label_6");

        formLayout_4->setWidget(0, QFormLayout::ItemRole::LabelRole, label_6);

        cameraIDLabel = new QLabel(frame_6);
        cameraIDLabel->setObjectName("cameraIDLabel");

        formLayout_4->setWidget(0, QFormLayout::ItemRole::FieldRole, cameraIDLabel);


        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, frame_6);

        frame_5 = new QFrame(frame);
        frame_5->setObjectName("frame_5");
        frame_5->setMinimumSize(QSize(300, 0));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        formLayout_3 = new QFormLayout(frame_5);
        formLayout_3->setObjectName("formLayout_3");
        apertureValueLabel = new QLabel(frame_5);
        apertureValueLabel->setObjectName("apertureValueLabel");
        apertureValueLabel->setAutoFillBackground(false);
        apertureValueLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::ItemRole::FieldRole, apertureValueLabel);

        label_4 = new QLabel(frame_5);
        label_4->setObjectName("label_4");

        formLayout_3->setWidget(1, QFormLayout::ItemRole::LabelRole, label_4);

        frame_9 = new QFrame(frame_5);
        frame_9->setObjectName("frame_9");
        frame_9->setFrameShape(QFrame::NoFrame);
        frame_9->setFrameShadow(QFrame::Plain);
        horizontalLayout_4 = new QHBoxLayout(frame_9);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        decrementApertureButton = new QPushButton(frame_9);
        decrementApertureButton->setObjectName("decrementApertureButton");
        decrementApertureButton->setMinimumSize(QSize(20, 0));
        decrementApertureButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_4->addWidget(decrementApertureButton);

        apertureValueSlider = new QSlider(frame_9);
        apertureValueSlider->setObjectName("apertureValueSlider");
        apertureValueSlider->setMinimumSize(QSize(100, 0));
        apertureValueSlider->setLayoutDirection(Qt::LeftToRight);
        apertureValueSlider->setMinimum(4352);
        apertureValueSlider->setMaximum(6144);
        apertureValueSlider->setSingleStep(1);
        apertureValueSlider->setValue(4352);
        apertureValueSlider->setTracking(true);
        apertureValueSlider->setOrientation(Qt::Horizontal);
        apertureValueSlider->setInvertedAppearance(false);
        apertureValueSlider->setInvertedControls(false);

        horizontalLayout_4->addWidget(apertureValueSlider);

        incrementApertureButton = new QPushButton(frame_9);
        incrementApertureButton->setObjectName("incrementApertureButton");
        incrementApertureButton->setMinimumSize(QSize(20, 0));
        incrementApertureButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_4->addWidget(incrementApertureButton);

        setApertureButton = new QPushButton(frame_9);
        setApertureButton->setObjectName("setApertureButton");
        setApertureButton->setMinimumSize(QSize(40, 0));
        setApertureButton->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_4->addWidget(setApertureButton);


        formLayout_3->setWidget(1, QFormLayout::ItemRole::FieldRole, frame_9);

        focusValueLabel = new QLabel(frame_5);
        focusValueLabel->setObjectName("focusValueLabel");
        focusValueLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_3->setWidget(2, QFormLayout::ItemRole::FieldRole, focusValueLabel);

        label_5 = new QLabel(frame_5);
        label_5->setObjectName("label_5");

        formLayout_3->setWidget(3, QFormLayout::ItemRole::LabelRole, label_5);

        frame_4 = new QFrame(frame_5);
        frame_4->setObjectName("frame_4");
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame_4);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        decrementFocusButton = new QPushButton(frame_4);
        decrementFocusButton->setObjectName("decrementFocusButton");
        decrementFocusButton->setMinimumSize(QSize(20, 0));
        decrementFocusButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_2->addWidget(decrementFocusButton);

        focusValueSlider = new QSlider(frame_4);
        focusValueSlider->setObjectName("focusValueSlider");
        focusValueSlider->setMinimumSize(QSize(100, 0));
        focusValueSlider->setLayoutDirection(Qt::LeftToRight);
        focusValueSlider->setMinimum(8270);
        focusValueSlider->setMaximum(12815);
        focusValueSlider->setSingleStep(1);
        focusValueSlider->setValue(8270);
        focusValueSlider->setTracking(true);
        focusValueSlider->setOrientation(Qt::Horizontal);
        focusValueSlider->setInvertedAppearance(false);
        focusValueSlider->setInvertedControls(false);

        horizontalLayout_2->addWidget(focusValueSlider);

        incrementFocusButton = new QPushButton(frame_4);
        incrementFocusButton->setObjectName("incrementFocusButton");
        incrementFocusButton->setMinimumSize(QSize(20, 0));
        incrementFocusButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_2->addWidget(incrementFocusButton);

        setFocusButton = new QPushButton(frame_4);
        setFocusButton->setObjectName("setFocusButton");
        setFocusButton->setMinimumSize(QSize(40, 0));
        setFocusButton->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(setFocusButton);


        formLayout_3->setWidget(3, QFormLayout::ItemRole::FieldRole, frame_4);


        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, frame_5);


        verticalLayout->addWidget(frame);

        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Plain);
        horizontalLayout = new QHBoxLayout(frame_3);
        horizontalLayout->setObjectName("horizontalLayout");
        label_8 = new QLabel(frame_3);
        label_8->setObjectName("label_8");

        horizontalLayout->addWidget(label_8);

        curStateLabel = new QLabel(frame_3);
        curStateLabel->setObjectName("curStateLabel");

        horizontalLayout->addWidget(curStateLabel);

        horizontalSpacer_2 = new QSpacerItem(747, 24, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_7 = new QLabel(frame_3);
        label_7->setObjectName("label_7");

        horizontalLayout->addWidget(label_7);

        debugButton = new QPushButton(frame_3);
        debugButton->setObjectName("debugButton");
        debugButton->setMinimumSize(QSize(40, 0));
        debugButton->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(debugButton);


        verticalLayout->addWidget(frame_3);

        debugBox = new QPlainTextEdit(centralwidget);
        debugBox->setObjectName("debugBox");

        verticalLayout->addWidget(debugBox);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 844, 24));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName("menuAbout");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuAbout->menuAction());
        menuAbout->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\316\243 Control", nullptr));
        actionThis_program_was_written_by_Tapendra_sldkfjlksdjflk_slkdfj_lksjdf_lsakdfjlk_jsdf_slkdfjlksd_jfa->setText(QCoreApplication::translate("MainWindow", "This program was written by Tapendra sldkfjlksdjflk slkdfj lksjdf  lsakdfjlk jsdf slkdfjlksd jfa", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Focus:", nullptr));
        curFocusLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Aperture:", nullptr));
        curApertureLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_temp->setText(QCoreApplication::translate("MainWindow", "Temp:", nullptr));
        curTempLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_pressure->setText(QCoreApplication::translate("MainWindow", "Pressure:", nullptr));
        curPressureLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_humidity->setText(QCoreApplication::translate("MainWindow", "Humidity:", nullptr));
        curHumidityLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_accel->setText(QCoreApplication::translate("MainWindow", "Accel (m/s\302\262):", nullptr));
        curAccelLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_gyro->setText(QCoreApplication::translate("MainWindow", "Gyro (dps):", nullptr));
        curGyroLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_mag->setText(QCoreApplication::translate("MainWindow", "Mag (\302\265T):", nullptr));
        curMagLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_bmag->setText(QCoreApplication::translate("MainWindow", "|B| (\302\265T):", nullptr));
        curBmagLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_heading->setText(QCoreApplication::translate("MainWindow", "Heading (deg):", nullptr));
        curHeadingLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        powerButton->setText(QCoreApplication::translate("MainWindow", "Shut Down", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Select Port", nullptr));
        refreshButton->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Cam #", nullptr));
        cameraIDLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        apertureValueLabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Aperture", nullptr));
        decrementApertureButton->setText(QCoreApplication::translate("MainWindow", "<", nullptr));
        incrementApertureButton->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
        setApertureButton->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        focusValueLabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Focus", nullptr));
        decrementFocusButton->setText(QCoreApplication::translate("MainWindow", "<", nullptr));
        incrementFocusButton->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
        setFocusButton->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "State : ", nullptr));
        curStateLabel->setText(QCoreApplication::translate("MainWindow", "Unknown", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Debug", nullptr));
        debugButton->setText(QCoreApplication::translate("MainWindow", "|>", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
