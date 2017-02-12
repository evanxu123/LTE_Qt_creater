#include "snr.h"
#include "ui_snr.h"
#include<QTimer>
#include<QPainter>
#include<qmath.h>
#include <QtGui/QPainter>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QTimer>
#include<stdio.h>

SNR::SNR(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SNR)
{
    ui->setupUi(this);
}

SNR::~SNR()
{
    delete ui;
}

void SNR::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //填充界面背景为白色
    painter.fillRect(rect(), Qt::white);

    // headlines
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black,2));
    QRectF SNRrect(0, 0, width(), height());
    painter.drawText(SNRrect, Qt::AlignHCenter, tr("SNR Diagram"));

    /*******************paint*******************/
    // set font size
    font.setPixelSize(12);
    painter.setFont(font);

    // transfer and calculate essential parameters
    grid_num[0] = 100;
    grid_num[1] = 55;
    interval[0] = (width()-2*width()/16)/(double)grid_num[0];
    if(grid_num[1] != 0)
        interval[1] = (height()-2*height()/10)/(double)grid_num[1];
    else
         interval[1] = 0;

//    if(temp == 0)
//        temp = 40;
//    if(temp == 40)
//        temp = 0;
    for(int i=0; i<100; i++)
    {
        transfered_coordinates[i][0] = width()/16 + interval[0] * i;
        if(snr[i] > 35)
            snr[i] = 35;
        if(snr[i] < 35)
            snr[i] = 30;
       int num=rand()%6;
        transfered_coordinates[i][1] = height()/10 + (height()-height()/10*2)/11.0*8 - interval[1] *0.2*num-2*height()/5;
        if( transfered_coordinates[i][1] < (height()/10) || transfered_coordinates[i][1] >  height())
            transfered_coordinates[i][1] =  (height()/10);
        //transfered_coordinates[i][1] = height()/10 + (height()-height()/10*2)/11.0*8 - interval[1] * 40;
        //printf("%f\n", snr[i]);

        //transfered_coordinates[i][1] = height()/10 + (height()-height()/10*2)/11.0*8 - interval[1] * temp;
    }
    valued_coordinates_interval[0] = 10;
    valued_coordinates_interval[1] = 5;
    painter.setPen(QPen(Qt::black,0.5,Qt::DotLine));
    for(int i=0; i<=10; i++) // vertical lines
    {
        painter.drawLine(QPoint(width()/16+(width()-width()/16*2)/10*i,height()/10), QPoint(width()/16+(width()-width()/16*2)/10*i,height()/10*9));
    }
    for(int i=0; i<=11; i++) // horizontal lines
    {
        painter.drawLine(QPoint(width()/16,height()/10 + (height()-height()/10*2)/11.0*i), QPoint(width()/16*15,height()/10 + (height()-height()/10*2)/11.0*i));
    }
    painter.setPen(QPen(Qt::red, 2));
    for(int i=0; i<=11; i++) // Y axis
    {
        painter.drawText(width()/16,height()/10 + (height()-height()/10*2)/11.0*i,tr("%3").arg((8-i)*valued_coordinates_interval[1]));
    }
    for (int i=0;i<=10;i++) // X axis
    {
        painter.drawText(width()/16+(width()-width()/16*2)/10*i,height()/10 + (height()-height()/10*2)/11.0*8,tr("%3").arg((int)(i*valued_coordinates_interval[0])));
    }

    // paint diagram
    painter.setPen(QPen(Qt::blue, 2));
    for(int i=1; i<100; i++)
    {
        painter.drawLine(QPoint(transfered_coordinates[i-1][0], transfered_coordinates[i-1][1]), QPoint(transfered_coordinates[i][0], transfered_coordinates[i][1]));
    }

    update();
}
