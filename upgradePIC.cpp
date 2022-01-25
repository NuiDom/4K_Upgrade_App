#include "upgradePIC.h"
#include <QStringList>
#include <stdint.h>
#include "upgradePIC.h"
#include <QByteArray>
#include <QStringRef>
#include <delay.h>

QString fileName = "/home/dnutt/upgradeFile.bin";
QSerialPort serial;

bool usbreadflag = false;
bool usbwriteflag = false;
bool  upgradeNow = false;
static int FilePosition = 0;
char usbCmd[20] = "";
QByteArray PICdata;

upgradePIC::upgradePIC(QObject *parent) : QObject(parent)
{

}

upgradePIC::~upgradePIC()
{

}

void upgradePIC::ErasePIC()
{
    serial.clear();
    serial.write("ERASE_FLASH");
    qDebug() << "ERASE_FLASH";

    serial.waitForReadyRead(2000);              //waits for response, timeout - 2sec

    PICdata = serial.readAll();
    qDebug() << PICdata;
    sscanf(PICdata, "%s", usbCmd);
    if((strcmp(usbCmd,"Done_Erase")==0))              //checks to see if PIC is done
        qDebug() << "Done Erase";
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
//    Block64.resize(4);
//    Block64[0] = 0x12;
//    Block64[1] = 0x34;
//    Block64[2] = 0x56;
//    Block64[3] = 0x78;
    QByteArray incoming;

    serial.clear();
    serial.write("FLASH_UNLOCK");
    msdelay(1000);

    while(usbwriteflag == true){

        Block64 = BinFile.mid(FilePosition,4);
        qDebug() << Block64.toHex();
        serial.clear();
        serial.write(Block64);
        FilePosition +=4;

        serial.waitForReadyRead(3000);

        incoming = serial.readAll();
        sscanf(incoming, "%s", usbCmd);
        if(strcmp(usbCmd,"64")==0)
            qDebug() << "Response Received";
        else if(strcmp(usbCmd,"STOP")==0){
            usbwriteflag = false;
            qDebug() << "Stop sent for upgrade";
        }
//        msdelay(50);
    }
    msdelay(500);
    serial.clear();
    serial.write("FLASH_LOCK");
    qDebug() << "DONE";
    myFile.close();
    return(0);
}

void upgradePIC::ReadPIC()
{
    while(usbreadflag == true){                 //checks flag to start/stop reading
        serial.clear(QSerialPort::AllDirections);
        serial.write("READ_MEM");                   //Sent to PIC to request next word
        qDebug() << "READ_MEM";

        serial.waitForReadyRead(2000);              //waits for response, timeout - 2sec

        PICdata = serial.readAll();
        qDebug() << PICdata.toHex();
        sscanf(PICdata, "%s", usbCmd);
        if((strcmp(usbCmd,"STOP")==0))              //checks to see if PIC is done
            usbreadflag = false;                    //if all words received stops reading
        writeToFile(fileName, PICdata);             //writes each word into upgrade file
    }
}

void upgradePIC::writeToFile(QString file, QByteArray output)
{
    QFile mFile(file);
    if(!mFile.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Could not open file for write";
        return;
    }

    mFile.write(output);

    mFile.close();
}

void upgradePIC::ReadWord()
{
    serial.clear(QSerialPort::AllDirections);
    serial.write("READ_WORD");                   //Sent to PIC to request read word
    qDebug() << "READ_WORD";

    serial.waitForReadyRead(2000);              //waits for response, timeout - 2sec

    PICdata = serial.readAll();
    qDebug() << PICdata.toHex();
}

void upgradePIC::WriteWord()
{
    serial.clear(QSerialPort::AllDirections);
    serial.write("WRITE_WORD");                   //Sent to PIC to request write word
    qDebug().noquote() << "WRITE_WORD";

    serial.waitForReadyRead(2000);              //waits for response, timeout - 2sec

    PICdata = serial.readAll();
    qDebug() << PICdata;
    sscanf(PICdata, "%s", usbCmd);
    if((strcmp(usbCmd,"DONE")==0))              //checks to see if PIC is done
        qDebug() << "Word written";
}
