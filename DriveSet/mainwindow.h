#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>



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
    void on_closeButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_saveButton_clicked();

    void on_helpButton_clicked();


private:
    Ui::MainWindow *ui;
    QSqlDatabase  db;
    int curIndex;
   QString Update;
    QComboBox *combo;
    QVector<int> v_int;
 QStringList regList;



    void AddItem(QComboBox *, QStringList &, QString &);
};

#endif // MAINWINDOW_H
