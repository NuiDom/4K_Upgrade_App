#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "upgradePIC.h"
#include <delay.h>

//#include </opt/poky/omni_qt5/sysroots/cortexa9-vfp-neon-poky-linux-gnueabi/usr/include/qt5/QtCore/qregexp.h>

#define INPUT_PATH "/home/dnutt/Desktop/4K_1.0.txt"

extern QSerialPort serial;
upgradePIC Upgrade;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial.setPortName("/dev/ttyACM0");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    connect(this, SIGNAL(signalProgramPIC()), &Upgrade, SLOT(ProgramPIC()));
    connect(this, SIGNAL(signalReadPIC()), &Upgrade, SLOT(ReadPIC()));
}

MainWindow::~MainWindow()
{
    delete ui;
    serial.close();
}


void MainWindow::on_pushButton_clicked()
{
    serial.open(QIODevice::ReadWrite);
}


void MainWindow::on_pushButton_2_clicked()
{
    serial.write("BOOTLOADER");
    qDebug() << "Jump to BL";
    msdelay(50);
    serial.close();
}


void MainWindow::on_pushButton_3_clicked()
{
    serial.close();
}


void MainWindow::on_pushButton_4_clicked()
{
    serial.clear();
    serial.write("JUMP");
    qDebug() << "Jump to App";
    msdelay(50);
    serial.close();

}

void MainWindow::readFromFile(QString fileName)
{

}

void MainWindow::on_Upgrade_push_button_clicked()
{
    emit signalProgramPIC();
}


void MainWindow::on_Open_File_push_button_clicked()
{

}


void MainWindow::on_pushButton_Erase_clicked()
{
    serial.clear();
    serial.write("ERASE");
    qDebug() << "ERASE";
}


void MainWindow::on_pushButton_ReadMemory_clicked()
{
    emit signalReadPIC();
}

