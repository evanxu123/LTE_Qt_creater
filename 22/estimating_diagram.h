#ifndef ESTIMATING_DIAGRAM_H
#define ESTIMATING_DIAGRAM_H

#include <QtGui/QPainter>
#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include<QCoreApplication>

namespace Ui {
class Estimating_Diagram;
}

class Estimating_Diagram : public QDialog
{
    Q_OBJECT

public:
    explicit Estimating_Diagram(QWidget *parent = 0);
    ~Estimating_Diagram();



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

    void paintEventa(QPaintEvent *);
    void sleep(unsigned int msec)
    {
        QTime dieTime=QTime::currentTime().addMSecs(msec);
        while(QTime::currentTime()<dieTime);
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }

    // self defined
    int receiver[512][2];
    int magnitudes[300];
    int grid_num[2];
    double interval[2];
    double transfered_coordinates[300][2];
    double valued_coordinates_interval[2];
    int magnitude(int x, int y);


private:
    Ui::Estimating_Diagram *ui;
    unsigned int t;
    int p;

    QPainterPath *path;
};

#endif // ESTIMATING_DIAGRAM_H
