#ifndef DIALOG_H
#define DIALOG_H
#include <QtGui/QPainter>
#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include<QCoreApplication>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QPoint Point;

    int f;
    int w;
    int choose;
    //char dat[10240];
    int dat[40960];
    struct d
        {
        int a[7168][2];
    }ca;
        void timerUpDate();

    void paintEvent(QPaintEvent *);
    int magnitude(int x, int y);
    void paintEventa(QPaintEvent *);
    void sleep(unsigned int msec)
    {
        QTime dieTime=QTime::currentTime().addMSecs(msec);
        while(QTime::currentTime()<dieTime);
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }

    int receiver[512][2];
    int magnitudes[512];
    int grid_num[2];
    double interval[2];
    int transfered_coordinates[512][2];
    double valued_coordinates_interval[2];


 /* ¶¨Ê±Æ÷²Ûº¯Êý */

private:
    Ui::Dialog *ui;

    unsigned int t;
    int p;

    QPainterPath *path;
    };



#endif // DIALOG_H
