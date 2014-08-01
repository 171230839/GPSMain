#ifndef WORKER_H
#define WORKER_H

#include <QSqlDatabase>
#include "win_qextserialport.h"
#include <QThread>
#include <QScopedPointer>


class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(QThread *parent = 0);
    ~Worker();
    void Initialize();
    void ReadCfgFile();
    void OpenComm();

    void run();

    void onAccept() {bAccept = true;}
    void onStop() {bAccept = false;}

signals:
    void ReceiveText(char* commdata, int iLen);
    void SendInfo(QString);
public:
    bool bThread;
    bool bAccept;

    QSqlDatabase  db;

    //CommSet
    QString m_portNo;
    QString m_baudRate;
    QString m_databits;
    QString m_parity;
    QString m_stopbits;
    QString m_timeout;

    QScopedPointer<Win_QextSerialPort>  myCom;

    void Decoding(char*, int);
};

#endif // WORKER_H
