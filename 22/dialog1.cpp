#include "dialog1.h"
#include "ui_dialog1.h"
#include<QTimer>
#include<QPainter>
#include<qmath.h>
#include <QtGui/QPainter>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QTimer>
#include<stdio.h>

Dialog1::Dialog1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog1)
{
    ui->setupUi(this);
    Point.setX(0);
    Point.setY(0);
    path = new QPainterPath;
}

Dialog1::~Dialog1()
{
    delete ui;
}


//1: Event for invocation
void Dialog::paintEvent1(QPaintEvent *)
{

    QPainter painter(this);
    //填充界面背景为白色
    painter.fillRect(rect(), Qt::white);
    painter.setPen(QPen(Qt::black,2));

    for (int i=1;i<15;i++)//竖线
    {
        painter.drawLine(QPoint(width()/15*i,0), QPoint(width()/15*i,height()));

    }
    for (int i=1;i<30;i++)//横线
    {
        painter.drawLine(QPoint(width()/15,height()/30*i), QPoint(width(),height()/30*i));
    }
    painter.setPen(QPen(Qt::red, 2));

    for (int i=0;i<30;i++)//横线
    {
        if(i<15)
        painter.drawText(width()/15-20,height()/30*i,tr("%3").arg(15-i));
        if(i>=15)
        painter.drawText(width()/15-23,height()/30*i,tr("%3").arg(15-i));
    }
     for (int i=1;i<15;i++)
     {

         painter.drawText(width()/15*(i+1),height()/2,tr("%3").arg(i));
     }




    //QRectF rect(10.0, 10.0, 380.0, 280.0);
    //painter.setPen(Qt::red);
    //painter.drawRect(rect);
    //painter.setPen(Qt::blue);
    //painter.drawText(rect, Qt::AlignHCenter, tr("AlignHCenter"));
   // painter.drawText(rect, Qt::AlignLeft, tr("AlignLeft"));
   // painter.drawText(rect, Qt::AlignRight, tr("AlignRight"));
    //painter.drawText(rect, Qt::AlignVCenter, tr("AlignVCenter"));
    //painter.drawText(rect, Qt::AlignBottom, tr("AlignBottom"));
    //painter.drawText(rect, Qt::AlignCenter, tr("AlignCenter"));
//

    painter.translate(width()/10,height()/2);
    painter.setPen(QPen(Qt::blue,Qt::DotLine)); //设置画笔形式
    painter.setBrush(QBrush(Qt::red,Qt::SolidPattern)); //设置画刷形式


 switch(1)
    {
   case 1:
       for(int i=0;i<width();i++)
       {
   float n = i+0.3;
    int y1;
    y1=-100*1*height()/1000*sin(20*3.14/width()*n*(1)/5);
    //cout<<height()<<endl;

    //painter.drawPoint(QPoint(n,y1));
    }break;
   /*case 2:
       for(int i=0;i<width();i++)
       {
   float n = i+0.3;
    int y1;
    y1=100*w*height()/1000*sin(20*3.14/width()*n*(f)/5);

    painter.drawPoint(QPoint(n,y1));
    }break;
   case 3:
       for(int k=0;k<width();k++)
       {
           int e;
          int q=(k-k%100)/100;
           if(q%2==0)
                e=0;
            if(q%2==1)
                e=1;
           float n =k+0.3;
           int y1=-height()/10;
           painter.drawPoint(QPoint(n,(y1*e)));

       }break;

case 4:
       for(int k=-2;k<width();)
       {
           k=k+2;
           int e;

           if(k%2==0)
                e=0;
            if(k%2==1)
                e=1;

           int y1=-height()/10;
           painter.drawLine(QPoint(100*k,f*y1*e*w),QPoint(100*k+100,f*y1*w));
           painter.drawLine(QPoint(100*k+100,f*y1*w),QPoint(100*k+200,f*y1*w*e));

       }break;
case 5:

           for(int i=0,j=0,y1,y2;i<512;i++,j++)
       {
            y2=y1;
            float n1,n2;
            n2=n1;
            n1=width()*i/550.0-width()/30;
            int show1=ca.a[i][1];
            y1=-(ca.a[i][1]*ca.a[i][1]+ca.a[i][0]*ca.a[i][0])*(height()/10000000.0)-13;
            painter.drawPoint(QPoint(n1,y1));
            if(i>0)
            {
            painter.drawLine(QPoint(n2,y2),QPoint(n1,y1));
            }
      }
    update();
           break;*/
}






}



    // update();


/*
void Dialog::paintEventa1(QPaintEvent *){
    QPainter painter(this);
    painter.setBrush(Qt::red);
    painter.drawRect(0,0,100,100);
    painter.setBrush(Qt::yellow);
    painter.drawRect(-50,-50,100,100);
    update();


}
*/

