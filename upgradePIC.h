#ifndef UPGRADEPIC_H
#define UPGRADEPIC_H

#include <QSerialPort>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QDataStream>
#include <QCoreApplication>
#include<qstring.h>
#include <QDebug>
#include <QApplication>
#include <QObject>

#define INFO_STATUS 0
#define DONE_STATUS 1

extern QSerialPort Serial;
extern QByteArray PICdata;
extern bool usbreadflag;
extern bool usbwriteflag;

class upgradePIC : public QObject
{
    Q_OBJECT
public:
     explicit upgradePIC(QObject *parent = 0);
    ~upgradePIC();

public slots:
    void ErasePIC();
    int	ProgramPIC();
    void ReadPIC();
    void writeToFile(QString fileName, QByteArray output);
    void ReadWord();
    void WriteWord();

signals:
    void SetProgressBar(int,int);
    void signalUpgradeDone(int,QString);


public:
    QSerialPort *Port;
    QString szFileName;
};

#endif // UPGRADEPIC_H
