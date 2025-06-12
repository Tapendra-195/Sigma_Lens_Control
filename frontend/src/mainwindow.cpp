#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serial(new QSerialPort(this)) {
    ui->setupUi(this);

    refreshPortList();

    reset();
    allowControl(false);
    
    
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout); 
    
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refreshPortList);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::openSerialPort);
    connect(ui->shutdownButton, &QPushButton::clicked, this, &MainWindow::onShutdown);
    connect(ui->powerOnButton, &QPushButton::clicked, this, &MainWindow::onPowerOn);
    connect(ui->apertureValueSlider, &QSlider::valueChanged, this, &MainWindow::updateAperture);
    connect(ui->focusValueSlider, &QSlider::valueChanged, this, &MainWindow::updateFocus);
    connect(ui->setApertureButton, &QPushButton::clicked, this, &MainWindow::setAperture);
    connect(ui->setFocusButton, &QPushButton::clicked, this, &MainWindow::setFocus);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
}

MainWindow::~MainWindow() {
    if (serial->isOpen()) serial->close();
    delete ui;
}

/*
void MainWindow::refreshPortList() {
  ui->portComboBox->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports)
        ui->portComboBox->addItem(port.portName());
}
*/

void MainWindow::allowControl(bool value){
    ui->apertureValueSlider->setEnabled(value);
    ui->focusValueSlider->setEnabled(value);
    ui->setApertureButton->setEnabled(value);
    ui->setFocusButton->setEnabled(value);
}

void MainWindow::reset(){
  ui->apertureValueSlider->setValue(0);
  ui->focusValueSlider->setValue(0);
  ui->apertureValueLabel->setText(QString::number(0));
  ui->focusValueLabel->setText(QString::number(0));
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

        // Option 2: Match by known Vendor ID/Product ID
        // Teensy boards (e.g., 16C0:0483 for Teensy 3.x)
        /*
        if (port.vendorIdentifier() == 0x16C0 && port.productIdentifier() == 0x0483) {
            ui->portComboBox->addItem(port.portName());
        }
        */
    }
}

void MainWindow::openSerialPort() {
  if (serial->isOpen()) serial->close();

    serial->setPortName(ui->portComboBox->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    if (!serial->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Error", "Failed to open port.");
        return;
    }
    
    //Activate changing focus and aperture
    reset();
    allowControl(true);

    ui->statusbar->showMessage("Connected to " + serial->portName());
}


void MainWindow::setAperture() {
  QString cmd = "SA " + QString::number(ui->apertureValueSlider->value());

  sendCommand(cmd);
}

void MainWindow::setFocus() {
  QString cmd = "SF " + QString::number(ui->focusValueSlider->value()); //ui->commandEdit->text().trimmed();
  
    sendCommand(cmd);
    
}

void MainWindow::onShutdown() {
  QString cmd = "OFF";

  sendCommand(cmd);
}

void MainWindow::onPowerOn() {
  
  QString cmd = "ON";

  sendCommand(cmd);
}

void MainWindow::sendCommand(QString cmd){
  if (!serial->isOpen()) {
    return;
  }
  
  if (cmd.isEmpty()) return;
  
  serial->write(( cmd + "\n").toUtf8());

  //display sent command
  ui->outputText->append(">> " + cmd);  
}

void MainWindow::readSerialData() {
  QByteArray data = serial->readAll();
    ui->outputText->append("<< " + QString::fromUtf8(data));
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "About", "Author: Tapendra BC \nWritten in: Summer 2025 \nU of W \nClient GUI program to set the focus and the aperture of sigma Art lens using Teensy 4.0 microcontroller. Writen in QT framework. Email: tapendra.320@gmail.com. Supervisor: Dr. Blair Jameison");
}

void MainWindow::updateAperture(){
  int aperture = ui->apertureValueSlider->value();
  float fNumber = pow(sqrt(2), (aperture/256.0) - 16);
  ui->apertureValueLabel->setText("f/" + QString::number(fNumber));

}

void MainWindow::updateFocus(){
  int focusPos = ui->focusValueSlider->value();
  float v = 5.518144e-5*focusPos - 0.381395;
  float focus = 3*v/(20*v-3);
  QString focusToDisplay = (focus>0)?QString::number(focus):"infinity";
  ui->focusValueLabel->setText(focusToDisplay+"/ " + QString::number(focusPos));
}
