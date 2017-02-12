#include "H/dialog.h"
#include "ui_dialog.h"
#include<QTimer>
#include<QPainter>
#include<qmath.h>
#include <QtGui/QPainter>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QTimer>
#include<stdio.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    p = 0;
    t = 10;
    Point.setX(0);
    Point.setY(0);
    path = new QPainterPath;
}

Dialog::~Dialog()
{
    delete ui;
}


int Dialog::magnitude(int x, int y)
{
    if(x==0&&y==0)
        return 0;
    else
        return 10*log10(x*x+y*y);
}


//1: Event for invocation
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //填充界面背景为白色
    painter.fillRect(rect(), Qt::white);

    // headlines
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black,2));
    QRectF FFTrect(0, 0, width(), height());
    painter.drawText(FFTrect, Qt::AlignHCenter, tr("FFT Diagram"));

    /*******************paint*******************/
    // set font size
    font.setPixelSize(12);
    painter.setFont(font);

    // transfer and calculate essential parameters
    for(int i=0; i<512; i++)
    {
        magnitudes[i] = magnitude(receiver[i][0], receiver[i][1]);
    }
    grid_num[0] = 512;
    grid_num[1] = 75;
    interval[0] = (width()-2*width()/16)/grid_num[0];
    interval[1] = (height()-2*height()/10)/(double)grid_num[1];
    for(int i=0; i<512; i++)
    {
        transfered_coordinates[i][0] = width()/16.0 + interval[0] * i;
        transfered_coordinates[i][1] = height() - height()/10 - (height()-height()/10*2)/15*2 - interval[1] * magnitudes[i];
    }
    valued_coordinates_interval[0] = 512/16; // =32
    valued_coordinates_interval[1] = 5; // =100
    painter.setPen(QPen(Qt::black,0.5,Qt::DotLine));
    for(int i=0; i<=16; i++) // vertical lines
    {
        painter.drawLine(QPoint(width()/16+(width()-width()/16*2)/16*i,height()/10), QPoint(width()/16+(width()-width()/16*2)/16*i,height()/10*9));
    }
    for(int i=0; i<=15; i++) // horizontal lines
    {
        painter.drawLine(QPoint(width()/16,height()/10+(height()-height()/10*2)/15*i), QPoint(width()/16*15,height()/10+(height()-height()/10*2)/15*i));
    }
    painter.setPen(QPen(Qt::red, 2));
    for(int i=0; i<=15; i++) // Y axis
    {
        painter.drawText(width()/2.0,height()/10+(height()-height()/10*2)/15*i,tr("%3").arg((13-i)*valued_coordinates_interval[1]));
    }
    for (int i=0;i<=16;i++) // X axis
    {
        painter.drawText(width()/16+(width()-width()/16*2)/16*i,height()-height()/10-(height()-height()/10*2)/15*2,tr("%3").arg(i*valued_coordinates_interval[0]-256));
    }
    painter.setPen(QPen(Qt::blue, 2));
int fudu=40;//修改sin幅度
int ypianyi=height()*4/5;//修改sin偏移量
int pinglv=25;//修改sin纵向偏移量
for(int i=width()/16.0;i<width()/16.0*15;i++)
    {

      //  painter.drawPoint(i,(fudu*sin(4.0*i/pinglv))+ypianyi);
        painter.drawLine(QPoint(i,(fudu*sin(4.0*i/pinglv))+ypianyi),QPoint(i+1,(fudu*sin(4.0*(i+1)/pinglv))+ypianyi));
    }

    // paint diagram
    painter.setPen(QPen(Qt::blue, 2));
    for(int i=1; i<512; i++)
    {
     //   painter.drawLine(QPoint(transfered_coordinates[i-1][0], transfered_coordinates[i-1][1]), QPoint(transfered_coordinates[i][0], transfered_coordinates[i][1]));
    }

    update();

}




    // update();



void Dialog::paintEventa(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::red);
    painter.drawRect(0,0,100,100);
    painter.setBrush(Qt::yellow);
    painter.drawRect(-50,-50,100,100);
    update();
}
