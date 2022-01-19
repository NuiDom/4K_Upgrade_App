#include "upgradePIC.h"
#include <QStringList>
#include <stdint.h>
#include "upgradePIC.h"
#include <QByteArray>
#include <QStringRef>
#include <delay.h>

QString fileName = "/home/dnutt/upgradeFile.txt";
QSerialPort serial;

bool  upgradeNow = false;
static int FilePosition = 0;
char usbCmd[20] = "";

upgradePIC::upgradePIC(QObject *parent) : QObject(parent)
{

}

upgradePIC::~upgradePIC()
{

}

int upgradePIC::ProgramPIC()
{
    QFile myFile(fileName);

    if(!myFile.open(QFile::ReadOnly))
    {
        qDebug() << "Could not open upgrade file";
        return(-1);
    }
    qDebug() << "Upgrade file open";

    QByteArray BinFile = myFile.readAll();
    int FileSize = myFile.size();

    int x=0;
    QByteArray Block64;
    QByteArray incoming;

//    serial.clear();
//    serial.write("UPGRADE");
    msdelay(50);
//    serial.waitForReadyRead(500);
//    serial.waitForReadyRead(500);
//    serial.waitForReadyRead(500);
//    serial.waitForReadyRead(500);

//    incoming = serial.readAll();
//    qDebug() << incoming;
//    sscanf(incoming, "%s", usbCmd);
//    if(strcmp(usbCmd,"NextRow")==0){
//            upgradeNow = true;
//    }
////    while(upgradeNow == true){
//        qDebug() << "Response Received";
//        msdelay(1000);
        Block64 = BinFile.mid(FilePosition,64);
        qDebug() << Block64;
        serial.clear();
        serial.write(Block64);
        FilePosition +=64;

        serial.waitForReadyRead(1000);
        serial.waitForReadyRead(1000);

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"64")==0){
            qDebug() << "Response Received";

            serial.clear();
            Block64 = BinFile.mid(FilePosition,64);
            qDebug() << Block64;
            serial.write(Block64);
            FilePosition +=64;
        }

//        if(serial.waitForReadyRead(3000)==false){
//            qDebug() << "No response from PIC";
//            return;
//        }

        serial.waitForReadyRead(1000);
        serial.waitForReadyRead(1000);

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"64")==0){
            qDebug() << "Response Received";
            serial.clear();
            Block64 = BinFile.mid(FilePosition,64);
            qDebug() << Block64;
            serial.write(Block64);
            FilePosition +=64;
        }
//         upgradeNow = false;
//    }

    return(0);
}

void upgradePIC::ReadPIC()
{
    serial.clear(QSerialPort::AllDirections);
    serial.write("READ_MEM");
    qDebug() << "READ_MEM";

    serial.waitForReadyRead(2000);

    QByteArray data = serial.readAll();
    qDebug() << data;
    writeToFile(fileName, data);
}

void upgradePIC::writeToFile(QString file, QString output)
{
    QFile mFile(file);
    if(!mFile.open(QFile::ReadWrite))
    {
        qDebug() << "Could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out << output;

    mFile.flush();
}
