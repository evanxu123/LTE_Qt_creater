#include "est_phase.h"
#include "ui_est_phase.h"
#include<QTimer>
#include<QPainter>
#include<qmath.h>
#include <QtGui/QPainter>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QTimer>
#include<stdio.h>

EST_Phase::EST_Phase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EST_Phase)
{
    ui->setupUi(this);
}

EST_Phase::~EST_Phase()
{
    delete ui;
}

double EST_Phase::phase(double x, double y)
{
    if(fabs(x)<1)
        x=1;
    double ret =  atan(fabs(y/x))*180.0/3.1415926;
    return ret;
}

void EST_Phase::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //填充界面背景为白色
    painter.fillRect(rect(), Qt::white);

    // headlines
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black,2));
    QRectF ESTPrect(0, 0, width(), height());
    painter.drawText(ESTPrect, Qt::AlignHCenter, tr("EST Phase Diagram"));

    /*******************paint*******************/
    // set font size
    font.setPixelSize(12);
    painter.setFont(font);

    // transfer and calculate essential parameters
    for(int j=0; j<300; j++)
    {
        double a = (double)receiver[j][0];
        double b = (double)receiver[j][1];
        phases[j] = phase(a, b);
    }
    grid_num[0] = 256;
    grid_num[1] = 180;
    interval[0] = (width()-2*width()/16.0)/grid_num[0];
    if(grid_num[1] != 0)
        interval[1] = (height()-2*height()/10)/grid_num[1];
    else
        interval[1] = 0;
    for(int j=0; j<256; j++)
    {
        transfered_coordinates[j][0] = width()/16 + interval[0] * j;
        transfered_coordinates[j][1] = height()/2 - interval[1] * phases[j];
    }
    valued_coordinates_interval[0] = 256/10.0;
    valued_coordinates_interval[1] = 30;
    painter.setPen(QPen(Qt::black,0.5,Qt::DotLine));
    for(int i=0; i<=10; i++) // vertical lines for phase
    {
        painter.drawLine(QPoint(width()/16+(width()-width()/16*2)/10.0*i,height()/10), QPoint(width()/16+(width()-width()/16*2)/10.0*i,height()/10*9));
    }
    for(int i=0; i<=6; i++) // horizontal lines for phase
    {
        painter.drawLine(QPoint(width()/16,height()/10+(height()-height()/10.0*2)/6.0*i), QPoint(width()/16*15,height()/10+(height()-height()/10.0*2)/6.0*i));
    }
    painter.setPen(QPen(Qt::red, 2));
    for(int i=0; i<=6; i++) // Y axis for phase
    {
        painter.drawText(width()/2.0,height()/10+(height()-height()/10.0*2)/6.0*i,tr("%3").arg((3-i)*valued_coordinates_interval[1]));
    }
    for (int i=0;i<=10;i++) // X axis for phase
    {
        painter.drawText(width()/16+(width()-width()/16*2)/10.0*i,height()/2,tr("%3").arg((int)(i*valued_coordinates_interval[0]-128)));
    }

    // paint diagram
    painter.setPen(QPen(Qt::blue, 2));
    for(int j=1; j<256; j++)
    {
        painter.drawLine(QPoint(transfered_coordinates[j-1][0], transfered_coordinates[j-1][1]), QPoint(transfered_coordinates[j][0], transfered_coordinates[j][1]));
        //painter.drawLine(QPoint(width()/16, height()/10+(height()-height()/10.0*2)/6.0*3), QPoint(width()/16*15, height()/10+(height()-height()/10.0*2)/6.0*3));
    }

    update();
}
