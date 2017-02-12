#include "showThread.h"
#include "constellationdiagram.h"
#include "iostream"
using namespace std;
#include <QDebug>

showThread::showThread(QObject *parent) :
    QThread(parent)
{
    stopped = false;

}

void showThread::QAM_Diagram()
{
    ConstellationDiagram *constellation=new ConstellationDiagram;
    constellation->choose = 5;
    int num_sub_frame = 0;
    constellation->show();
    int replicate[10][7168][2];
    FILE *fp;
    fp = fopen("/home/yaha/5m_eNB_2014/Data", "r+");
    if(fp == NULL)
    {
        cout<<"can't open file"<<endl;
        exit(1);
    }
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<7168; j++)
        {
            fscanf(fp, "%d%d", &replicate[i][j][0], &replicate[i][j][1]);
        }
    }
    while(1)
    {
        for(int i=0; i<7168; i++)
        {
            constellation->ca.a[i][0] = replicate[num_sub_frame][i][0];
            constellation->ca.a[i][1] = replicate[num_sub_frame][i][1];
        }
        sleep(1);
        num_sub_frame++;
        if(num_sub_frame>  9)
        {
            num_sub_frame = 0;
        }
    }
}

void showThread::run()
{
    int i = 0;
    while (!stopped) {

         switch(key)
         {
            // FFT
            case 0:
             break;
            // QAM
            case 1:
             QAM_Diagram();
             break;
            // EST
            case 2:
             QAM_Diagram();
             break;
         }
    }
    stopped = false;
}

void showThread::stop()
{
    stopped = true;
}
