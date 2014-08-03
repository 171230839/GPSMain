#ifndef SENDWORKER_H
#define SENDWORKER_H

#include <QThread>

class SendWorker : public QThread
{
    Q_OBJECT
public:
    explicit SendWorker(QObject *parent = 0);

    void run();
signals:

public slots:

private:
    bool bThread;

};

#endif // SENDWORKER_H
