#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "worker.h"
#include <QSystemTrayIcon>
#include "CoorTrans.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_GetData_clicked();
    void on_StopGet_clicked();
    void on_ReceiveText(char* commdata, int iLen);
    void on_SendInfo(QString);
    void on_CleanEdit_clicked();
    void on_activated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;
    Worker myWorker;

    void setTryIcon();
    QSystemTrayIcon trayIcon;
};

#endif // MAINWINDOW_H
