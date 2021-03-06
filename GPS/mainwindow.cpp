#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCursor>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&myWorker, &Worker::ReceiveText, this, &MainWindow::on_ReceiveText);
    connect(&myWorker, &Worker::SendInfo, this, &MainWindow::on_SendInfo);
    myWorker.Initialize();

    setTryIcon();
}

MainWindow::~MainWindow()
{
    delete ui;
    myWorker.bThread = true;
    myWorker.wait();

}

void MainWindow::on_GetData_clicked()
{
    myWorker.onAccept();
    if (!myWorker.isRunning())
    {
        myWorker.start();
    }
}

void MainWindow::on_StopGet_clicked()
{
    myWorker.onStop();
}


void MainWindow::on_ReceiveText(char* commdata, int iLen)
{

    ui->m_GET->moveCursor(QTextCursor::End);
    ui->m_GET->insertPlainText(QStringLiteral("接收： "));
    ui->m_GET->insertPlainText(commdata);
    ui->m_GET->insertPlainText("\n");



    char temp[7] = "";
    for (int j = 0; j < 6; ++j)
    {
        temp[j] = *(commdata + j);
    }

    if (strcmp(temp, "$GPGGA") == 0)
    {
        QString str(commdata);
        qDebug()<<"str--"<<str;
        QStringList list = str.split(',');
        QString time = list.at(1);
        int time_hour = time.mid(0, 2).toInt();
        int time_minute = time.mid(2, 2).toInt();
        int time_second = time.mid(4, 2).toInt();
        int time_msec = time.right(3).toInt();
        QTime qtime(time_hour, time_minute, time_second, time_msec);
        ui->GGATime->setTime(qtime);\
        ui->m_lat->setText(list.at(2));
        ui->m_lon->setText(list.at(4));

        double MyL0 = 108;
        double MyB, MyL, OutMyX, OutMyY;

        PrjPoint_Krasovsky MyPrj;
        MyPrj.SetL0(MyL0);

        MyL = list.at(2).toDouble();
        MyB = list.at(4).toDouble();
        qDebug()<<"L "<<MyL;
        qDebug()<<"B "<<MyB;
        MyPrj.SetBL(MyB, MyL);
        OutMyX = MyPrj.x;
        OutMyY = MyPrj.y;
        ui->m_GAOSx->setText(QString::number(OutMyX));
        ui->m_GAOSy->setText(QString::number(OutMyY));

    }
    else if(strcmp(temp, "$GPTRA") == 0)
    {
        QString str(commdata);
        qDebug()<<"str"<<str;
        QStringList list = str.split(',');
        if (list.size() != 15)
            return;
        QString time = list.at(1);
        qDebug()<<"time"<<time;
        int time_hour = time.mid(0, 2).toInt();
        int time_minute = time.mid(2, 2).toInt();
        int time_second = time.mid(4, 2).toInt();
        int time_msec = time.right(3).toInt();
        qDebug()<<"time_hour"<<time_hour<<time_minute;
        QTime qtime(time_hour, time_minute, time_second, time_msec);
        ui->GGATime->setTime(qtime);

        ui->m_heading->setText(list.at(2));
        ui->m_pitch->setText(list.at(3));
        ui->m_roll->setText(list.at(4));
        ui->m_delay->setText(list.at(7));
    }
}

void MainWindow::on_SendInfo(QString str)
{
    ui->m_GET->insertPlainText(str);
    ui->m_GET->insertPlainText("\n");
}




void MainWindow::on_CleanEdit_clicked()
{
    this->ui->m_GET->clear();
}

void MainWindow::setTryIcon()
{
    QString path = QCoreApplication::applicationDirPath();
    path.append("/GPS.ico");
    qDebug()<<"path"<<path;
    if (!QFileInfo(path).exists())
    {
        on_SendInfo(QStringLiteral("无法找到GPS.ico文件"));
        return;
    }
    QPixmap map;
    map.load(path);
    setWindowIcon(map);
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        trayIcon.setIcon(map);
        connect(&trayIcon, &QSystemTrayIcon::activated,
                this, &MainWindow::on_activated);
        trayIcon.setToolTip(QStringLiteral("GPS串口通讯"));
        trayIcon.show();
    }
}

void MainWindow::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    {
        this->show();
        setWindowState(Qt::WindowActive);
        break;
    }

    }
}
