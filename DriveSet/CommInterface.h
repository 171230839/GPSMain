#ifndef COMMINTERFACE_H
#define COMMINTERFACE_H

#include <QObject>
#include <QThread>
#include <QtPlugin>

class CommInterface : public QThread
{
public:

    virtual void init() = 0;

    virtual void destroy() = 0;
     virtual   void getRegType( QStringList & reglist ) = 0;

    bool m_bStop;
signals:
    virtual void sendFixMsg(QString str) = 0;
    virtual void sendCommMsg(QString str) = 0;
    virtual void insertSendText(unsigned char* commdata, int iLen) = 0;
    virtual void insertReceText(unsigned char* commdata, int iLen) = 0;
    virtual void exit() = 0;
};



#define CommInterface_iid "org.lianyu"

Q_DECLARE_INTERFACE(CommInterface, CommInterface_iid)



#endif // COMMINTERFACE_H
