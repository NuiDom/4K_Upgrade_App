#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serial.h"
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QCoreApplication>
#include<qstring.h>
#include <QDebug>
//#include </opt/poky/omni_qt5/sysroots/cortexa9-vfp-neon-poky-linux-gnueabi/usr/include/qt5/QtCore/qregexp.h>

#define INPUT_PATH "/home/dnutt/Desktop/4K_1.0.txt"

QSerialPort serial;
char usbCmd[10] = "";
char usbCmd2[10] = "";
QString mFile = "/home/dnutt/Desktop/output.bin";
QString m2File = "/home/dnutt/Desktop/FF.txt";

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
    serial.write("UPGRADE");
    qDebug() << "Jump to BL";
    msdelay(50);
//    while(serial.isOpen()){
//        if(!serial.waitForReadyRead(-1)){
//            qDebug() << "waiting for upgrade response";
//        }
//        else{
//            QByteArray datas = serial.readAll();
//            sscanf(datas, "%s", usbCmd);
//            if(strcmp(usbCmd,"OK")==0){
                serial.close();
//            }
//        }
//    }
}


void MainWindow::on_pushButton_3_clicked()
{
    serial.close();
}


void MainWindow::on_pushButton_4_clicked()
{
    serial.write("JUMP");
    qDebug() << "Jump to App";
    msdelay(50);
//    while(serial.isOpen()){
//        if(!serial.waitForReadyRead(-1)){
//            qDebug() << "waiting for jump response";
//        }
//        else{
//            QByteArray datas = serial.readAll();
//            sscanf(datas, "%s", usbCmd);
//            if(strcmp(usbCmd,"OK")==0){
                serial.close();
//            }
//        }
//    }
}

void MainWindow::msdelay(int msec)
{
    QEventLoop loop;

    QTimer::singleShot(msec, &loop, &QEventLoop::quit);

    loop.exec();
}

void MainWindow::writeToFile(QString fileName)
{
    QFile mFile(fileName);
    if(!mFile.open(QFile::WriteOnly))
    {
        qDebug() << "Could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    for(int x=0; x<172; x++)
    {
        out << "F";
    }
    mFile.flush();
    mFile.close();
}

void MainWindow::readFromFile(QString fileName)
{
    QFile mFile(fileName);

    if(!mFile.open(QFile::ReadOnly))
    {
        qDebug() << "Could not open upgrade file";
        return;
    }

    qDebug() << "Upgrade file open";
    QByteArray HEXFile = mFile.readAll();
    int FileSize = mFile.size();

    //----------------------------- Flash Programming ----------------------------
    //----------------------------------------------------------------------------

    int     FilePosition=0;
//    int     n32AddressCounter=0x00;
//    char 	szBuffer[20];
//    char 	szAddress[20];
//    int 	Length;
//    int     Progress=0;

//    int Max = FileSize/192;


//    qDebug() <<"Programming Flash...";

//    do{
//        emit SetProgressBar(Progress++,Max);

//        // Do not allow any addresses outside of FLASH Area
//        if(n32AddressCounter > 0x30000) break;

//        // Program Flash

//        sprintf(szAddress,"000000%x",n32AddressCounter);

//        Length = strlen(szAddress);

//        sprintf(szBuffer,"PF %s ",szAddress+Length-7); //[(Length-7)]);

//        Port->clear_port();
//        Port->write_port(szBuffer);
        serial.clear();
        QByteArray Block192;

        Block192 = HEXFile.mid(FilePosition,10);
        serial.write("Hello");
        serial.write(Block192);
        qDebug() << "Block 1 write";
        FilePosition+=192;
        Block192 = HEXFile.mid(FilePosition,192);
        serial.clear();
        if(!serial.waitForReadyRead(3000) == true){
            qDebug() << "No response after block 1 write ";
            return;
        }

        QByteArray incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"OK")==0){
            serial.write(Block192);
            qDebug() << "Block 2 write";
            FilePosition+=192;
            Block192 = HEXFile.mid(FilePosition,192);
        }
        serial.clear();
        if(!serial.waitForReadyRead(3000) == true){
            qDebug() << "No response after block 2 write ";
            return;
        }

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"OK")==0){
            serial.write(Block192);
            qDebug() << "Block 3 write";
            FilePosition+=192;
            Block192 = HEXFile.mid(FilePosition,192);
        }
        serial.clear();
        if(!serial.waitForReadyRead(2000) == true){
            qDebug() << "No response after block 3 write ";
            return;
        }

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"OK")==0){
            serial.write(Block192);
            qDebug() << "Block 4 write";
            FilePosition+=192;
            Block192 = HEXFile.mid(FilePosition,192);
        }
        serial.clear();
        if(!serial.waitForReadyRead(2000) == true){
            qDebug() << "No response after block 4 write ";
            return;
        }

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"OK")==0){
            serial.write(Block192);
            qDebug() << "Block 5 write";
            FilePosition+=192;
            Block192 = HEXFile.mid(FilePosition,192);
        }
        serial.clear();
        if(!serial.waitForReadyRead(2000) == true){
            qDebug() << "No response after block 5 write ";
            return;
        }

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"OK")==0){
            serial.write(Block192);
            qDebug() << "Block 6 write";
            FilePosition+=192;
            Block192 = HEXFile.mid(FilePosition,192);
        }
        serial.clear();
        if(!serial.waitForReadyRead(2000) == true){
            qDebug() << "No response after block 6 write ";
            return;
        }

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"OK")==0){
            serial.write(Block192);
            qDebug() << "Block 7 write";
            FilePosition+=192;
            Block192 = HEXFile.mid(FilePosition,192);
        }
        serial.clear();
        if(!serial.waitForReadyRead(2000) == true){
            qDebug() << "No response after block 7 write ";
            return;
        }

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"OK")==0){
            serial.write(Block192);
            qDebug() << "Block 8 write";
            FilePosition+=192;
            Block192 = HEXFile.mid(FilePosition,192);
        }

//        Port->read_port(1000);
//        if(Port->EXT_COMMAND.left(2)!="pf")
//        {
//            emit signalUpgradeDone(DONE_STATUS,"Error Writing to Flash! ");

//            qDebug()<< "Error writing to Flash!";
//            return -1;
//        }

//        n32AddressCounter+=64;

//    }while(FilePosition-192 < FileSize);
//    qDebug() <<"Programming Flash...Done";

    //-------------------------- End of Flash Programming ------------------------
    //----------------------------------------------------------------------------
//    QTextStream in(&mFile);
//    QString mText = in.readAll();


//    qDebug() << mText;
    mFile.close();
}

void MainWindow::on_Upgrade_push_button_clicked()
{
    QByteArray Block1;
    QByteArray Block2;
    QByteArray Block3;
    Block1.resize(64);
    Block2.resize(64);
    Block3.resize(64);
    for(int x=0; x<64; x++){
        Block1[x] = 0x41;
        Block2[x] = 0x41;
        Block3[x] = 0x41;
    }
//    QFile myFile(mFile);

//    if(!myFile.open(QFile::ReadOnly))
//    {
//        qDebug() << "Could not open upgrade file";
//        return;
//    }
//    qDebug() << "Upgrade file open";

//    QByteArray BinFile = myFile.readAll();
//    int FileSize = myFile.size();
//    int     FilePosition=0;

//    serial.clear();

//    QByteArray Block64;
//    Block64 = BinFile.mid(FilePosition,64);
//    qDebug() << Block64;
//    serial.write(Block64);
//    FilePosition +=64;
    qDebug() << Block1;
    serial.write(Block1);
    msdelay(500);
    if(serial.waitForReadyRead(3000)==false){
        qDebug() << "No response from PIC";
        return;
    }
    QByteArray incoming1 = serial.readAll();
    qDebug() << incoming1;
    sscanf(incoming1, "%s", usbCmd);
    if(strcmp(usbCmd,"OK")==0){
        serial.clear();
        qDebug() << Block2;
        serial.write(Block2);
    }
    msdelay(500);
    if(serial.waitForReadyRead(3000)==false){
        qDebug() << "No response from PIC";
        return;
    }

//    for(int y=0; y<10; y++)
//        usbCmd[y] = '\0';
    QByteArray incoming2 = serial.readAll();
    qDebug() << incoming2;
    sscanf(incoming2, "%s", usbCmd2);
    if(strcmp(usbCmd2,"OK")==0){
        serial.clear();
        qDebug() << Block3;
        serial.write(Block3);
    }

//    if(serial.waitForReadyRead(3000)==false){
//        qDebug() << "No response from PIC";
//        return;
//    }
//    QByteArray incoming = serial.readAll();
//    sscanf(incoming, "%s", usbCmd);
//    if(strcmp(usbCmd,"OK")==0){
//        serial.clear();
//        Block64 = BinFile.mid(FilePosition,64);
//        qDebug() << Block64;
//        serial.write(Block64);
//        FilePosition +=64;
//    }

//    if(serial.waitForReadyRead(3000)==false){
//        qDebug() << "No response from PIC";
//        return;
//    }
////    serial.waitForReadyRead(-1);
//    incoming = serial.readAll();
//    sscanf(incoming, "%s", usbCmd);
//    if(strcmp(usbCmd,"OK")==0){
//        serial.clear();
//        Block64 = BinFile.mid(FilePosition,64);
//        qDebug() << Block64;
//        serial.write(Block64);
//        FilePosition +=64;
//    }
}


void MainWindow::on_Open_File_push_button_clicked()
{
    readFromFile(mFile);
}

