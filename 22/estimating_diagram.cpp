#include "estimating_diagram.h"
#include "ui_estimating_diagram.h"
#include<QTimer>
#include<QPainter>
#include<qmath.h>
#include <QtGui/QPainter>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QTimer>
#include<stdio.h>
#include <iostream>
using namespace std;

Estimating_Diagram::Estimating_Diagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Estimating_Diagram)
{
    ui->setupUi(this);
    p = 0;
    t = 10;
    Point.setX(0);
    Point.setY(0);
    path = new QPainterPath;
}

Estimating_Diagram::~Estimating_Diagram()
{
    delete ui;
}

int Estimating_Diagram::magnitude(int x, int y)
{
    if(x==0&&y==0)
        return 0;
    else
        return 10*log10(x*x+y*y);
}


//1: Event for invocation
void Estimating_Diagram::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //填充界面背景为白色
    painter.fillRect(rect(), Qt::white);

    // headlines
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black,2));
    QRectF ESTMrect(0, 0, width(), height());
    painter.drawText(ESTMrect, Qt::AlignHCenter, tr("EST Magnitude Diagram"));

    /*******************paint*******************/
    // set font size
    font.setPixelSize(12);
    painter.setFont(font);

    // transfer and calculate essential parameters
    for(int j=0; j<300; j++)
    {
        magnitudes[j] = magnitude(receiver[j][0], receiver[j][1]);
    }

    grid_num[0] = 300;
    grid_num[1] = 75;

    interval[0] = (width()-2*width()/16.0)/grid_num[0];
    if(grid_num[1] != 0)
        interval[1] = (height()-2*height()/10)/grid_num[1];
    else
        interval[1] = 0;

    for(int j=0; j<300; j++)
    {
        transfered_coordinates[j][0] = width()/16.0 + interval[0] * j;
        transfered_coordinates[j][1] = height() - height()/10.0 - (height()-height()/10*2)/15.0*2 - interval[1] * magnitudes[j];
    }

    valued_coordinates_interval[0] = 300/10;
    valued_coordinates_interval[1] = 5;

    // paint grids
    painter.setPen(QPen(Qt::black,0.5,Qt::DotLine));
    for(int i=0; i<=10; i++) // vertical lines
    {
        painter.drawLine(QPoint(width()/16+(width()-width()/16.0*2)/10.0*i,height()/10), QPoint(width()/16+(width()-width()/16.0*2)/10.0*i,height()/10*9));
    }
    for(int i=0; i<=15; i++) // horizontal lines
    {
        painter.drawLine(QPoint(width()/16,height()-height()/10-(height()-height()/10.0*2)/15.0*i), QPoint(width()/16*15,height()-height()/10-(height()-height()/10.0*2)/15.0*i));
    }

    // paint coordinates on the axis
    painter.setPen(QPen(Qt::red, 2));
    for(int i=0; i<=15; i++) // Y axis
    {
        if(i!=2)
            painter.drawText(width()/2.0,height()-height()/10-(height()-height()/10.0*2)/15.0*i+12,tr("%3").arg((i-2)*valued_coordinates_interval[1]));
    }
    for (int i=0;i<=10;i++) // X axis
    {
        painter.drawText(width()/16+(width()-width()/16*2)/10.0*i,height()-height()/10-(height()-height()/10*2)/15.0*2,tr("%3").arg((int)(i*valued_coordinates_interval[0]-150)));
    }

    // paint diagram
    painter.setPen(QPen(Qt::blue, 2));
    for(int j=1; j<300; j++)
    {
        painter.drawLine(QPoint(transfered_coordinates[j-1][0], transfered_coordinates[j-1][1]), QPoint(transfered_coordinates[j][0], transfered_coordinates[j][1]));
    }

    update();
}



    // update();



void Estimating_Diagram::paintEventa(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::red);
    painter.drawRect(0,0,100,100);
    painter.setBrush(Qt::yellow);
    painter.drawRect(-50,-50,100,100);
    update();
}
