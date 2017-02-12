#ifndef CONSTELLATIONDIAGRAM_H
#define CONSTELLATIONDIAGRAM_H
#include <QtGui/QPainter>
#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include<QCoreApplication>

namespace Ui {
class ConstellationDiagram;
}

class ConstellationDiagram : public QDialog
{
    Q_OBJECT

public:
    explicit ConstellationDiagram(QWidget *parent = 0);
    ~ConstellationDiagram();

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

    int MaxOfArray(int XOY, int array[7168][2]);
    int XMax;
    int YMax;
    // int XMax = MaxOfArray(0, ca.a);
    // int YMax = MaxOfArray(1, ca.a);

    void paintEvent(QPaintEvent *);
    void paintEventa(QPaintEvent *);
    void sleep(unsigned int msec)
    {
        QTime dieTime=QTime::currentTime().addMSecs(msec);
        while(QTime::currentTime()<dieTime);
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }

    double receiver[14][512][2];
    int grid_num[2];
    double interval[2];
    double transfered_coordinates[14][512][2];
    int valued_coordinates_interval[2];

private:
    Ui::ConstellationDiagram *ui;

    unsigned int t;
    int p;

    QPainterPath *path;
};

#endif // CONSTELLATIONDIAGRAM_H
