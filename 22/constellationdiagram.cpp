#include "constellationdiagram.h"
#include "ui_constellationdiagram.h"
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

int ConstellationDiagram::MaxOfArray(int XOY, int array[7168][2])
{
    int max = array[0][XOY];
    for(int i=0; i<7168; i++)
    {
        if(fabs(array[i][XOY]) > max)
            max = fabs(array[i][XOY]);
    }
    return max;
}

ConstellationDiagram::ConstellationDiagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConstellationDiagram)
{
    ui->setupUi(this);
    p = 0;
    t = 10;
    Point.setX(0);
    Point.setY(0);
    path = new QPainterPath;
}

ConstellationDiagram::~ConstellationDiagram()
{
    delete ui;
}

//1: Event for invocation
void ConstellationDiagram::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //填充界面背景为白色
    painter.fillRect(rect(), Qt::white);

    // headlines
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black,2));
    QRectF QAMrect(0, 0, width(), height());
    painter.drawText(QAMrect, Qt::AlignHCenter, tr("QAM Diagram"));

    /*******************paint*******************/
    // set font size
    font.setPixelSize(12);
    painter.setFont(font);

    // transfer and calculate essential parameters
    grid_num[0] = 2;
    grid_num[1] = 2;
    interval[0] = (width()-width()/16*2)/(double)grid_num[0];
    interval[1] = (height()-height()/16*2)/(double)grid_num[1];
    for(int i=0; i<14; i++)
    {
        for(int j=0; j<512; j++)
        {
            //transfered_coordinates[i][j][0] = width()*0.75-0.1*interval[0] * receiver[i][j][0];
            //transfered_coordinates[i][j][1] = height()*0.75+0.1*interval[1] * receiver[i][j][1];


        }
    }
    valued_coordinates_interval[0] = 200/20;
    valued_coordinates_interval[1] = 200/20;
    painter.setPen(QPen(Qt::black,0.5,Qt::DotLine));
    for(int i=1; i<20; i++) // vertical lines
    {
        painter.drawLine(QPoint(width()/16+(width()-width()/16*2)/20*i,height()/16.0), QPoint(width()/16+(width()-width()/16*2)/20*i,height()*15.0/16.0));
    }
    for(int i=1; i<20; i++) // horizontal lines
    {
        painter.drawLine(QPoint(width()/16,height()/16+(height()-height()/16*2)/20*i), QPoint(width()/16*15,height()/16+(height()-height()/16*2)/20*i));
    }
    painter.setPen(QPen(Qt::red, 2));
    for(int i=1; i<20; i++) // Y axis
    {
        if(i!=10)
            painter.drawText(width()/2,height()/16+(height()-height()/16*2)/20*i,tr("%3").arg(((10-i)*0.01*valued_coordinates_interval[1])));
    }
    for(int i=1; i<20; i++) // X axis
    {
        painter.drawText(width()/16+(width()-width()/16*2)/20*i,height()/2,tr("%3").arg(((i-10)*0.01*valued_coordinates_interval[0])));
    }

    // paint diagram


    painter.setPen(QPen(Qt::blue, 2));
    for(int j=1; j<300; j++)
    {
        if((fabs(transfered_coordinates[0][j][0]-width()/4*3)<3) && (fabs(transfered_coordinates[0][j][1]-height()/3)<3))
            ;
        else if(transfered_coordinates[0][j][0]>width()/2 && transfered_coordinates[0][j][1]<height()*2/3.0)
            painter.drawEllipse(transfered_coordinates[0][j][0]-1.2, transfered_coordinates[0][j][1]-1.2, 2.4, 2.4);
    }
 //   QPointF pointf[4]={(0.25*width(),0.25*height()),(0.25*width(),0.75*height()),(0.75*width(),0.25*height()),(0.75*width(),0.75*height())};

 //   (width()/4,3*height()/4),(3*width()/4,height()/4),(3*width()/4,3*height()/4)
    int num=rand()%6;
    int num2=rand()%8;
       painter.setPen(QPen(Qt::blue, 6));
    {painter.drawPoint(width()/4+width()/16+num/5,height()/4+height()/30+num2/3);
     painter.drawPoint(width()/4+width()/16+num2/3,3*height()/4-height()/20+num/5);
      painter.drawPoint(3*width()/4+num/3,height()/4+height()/30+num2/3);
       painter.drawPoint(3*width()/4+num2/5,3*height()/4-height()/20+num/5);}
    update();
}



    // update();



void ConstellationDiagram::paintEventa(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::red);
    painter.drawRect(0,0,100,100);
    painter.setBrush(Qt::yellow);
    painter.drawRect(-50,-50,100,100);
    update();
}
