#include "H/mainwindow.h"
#include "ui_mainwindow.h"
#include "H/dialog.h"
#include "estimating_diagram.h"
#include "constellationdiagram.h"
#include "snr.h"
#include "est_phase.h"
#include"QDebug"
#include<QTimer>
#include <fstream>
#include <QFile>
#include<QPainter>
#include<qmath.h>
#include<QDialog>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QList>
#include <QtNetwork>
#include <QtNetwork>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>//close()
#include<netinet/in.h>//struct sockaddr_in
#include<arpa/inet.h>//inet_ntoa
#include<sys/ioctl.h>
#include<net/if.h>
#include"struct_type.h"
#include"mythread.h"
#include "QString"
#include <QDebug>
#include <unistd.h>
#include<sstream>
#include "iostream"
using namespace std;
#define DEST_PORT 6666
#define DEST_IP_ADDRESS "127.0.0.1"//地址

int write_output(const char *fname,const char *vname,void *data,int length,int dec,char format) ;      //Output Function
FILE *fd;
PHY_Data_Send sa;
PHY_Data_Send sc;
PHY_Data_Send e;
PHY_Data_Send *n[10];
PHY_Data_Send trigger_for_receive;
char recv_buf[102400];

int recv_len = sizeof(PHY_Data_Send);
int ShowFlag;
int sock_fd;
int status=0;
int sub_frame=0;//used to point the position of stack
int i = 0;
int refreshed = 0; // judge whether new data has been received while painting
int Mrefreshed = 0; // judge whether new data has been received while accumulating SNR & EST data for letting the diagram going forwards
double GlobalSNR[100] = {0};
double GlobalQAM[14][300][2];
int flag=0;

void powerNormalization(double array[14][300][2])
{
    FILE *fp;
    fp = fopen("hsq.txt", "w+");
    for(int i=0; i<14; i++)
    {
        double psum = 0,  avgp = 0;
        for(int j=0; j < 300 ;j++)
        {
                psum+= exp(array[i][j][0]) + exp(array[i][j][1]);
        }
        avgp = sqrt(psum / 300);
        if(avgp != 0)
        {
                for(int j=0; j < 300 ;j++)
                {
                        array[i][j][0] = (array[i][j][0])/avgp;
                        array[i][j][1] = (array[i][j][1])/avgp;
                }
        }
        else
                ;
    }
    for(int i=0; i<14; i++)
    {
        for(int j=0; j<300; j++)
        {
            fprintf(fp, "x=%f\ny=%f\n", array[i][j][0], array[i][j][1]);
        }
    }
    fclose(fp);
}

void  process_infoL(int s);
 struct d
     {
       int a[10][7168][2];
     }c;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    /*tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    tcpServer = new QTcpServer(this);
    // 使用了IPv4的本地主机地址，等价于QHostAddress("127.0.0.1")
    if (!tcpServer->listen(QHostAddress::LocalHost, 6666)) {
        qDebug() << tcpServer->errorString();
        close();
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendMessage()));
    */
    setWindowTitle("4G LTE SDR测试平台");
    //layout = new QVBoxLayout(this);

    // 1:For memu
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 4000, 24));

    QMenu* menu1 = menuBar->addMenu(tr("&文件"));
    QMenu* menu12 = menu1->addMenu(tr("&新建"));
    actionNew = menu12->addAction(tr("&工程"));
    actionNew = menu12->addAction(tr("文件"));
    QMenu* menu13 = menu1->addMenu(tr("&打开"));
    actionNew = menu13->addAction(tr("文件"));
    actionNew = menu13->addAction(tr("工程"));
    //QMenu* menu14 = menu1->addMenu(tr("&关闭"));
    QMenu* menu21 = menuBar->addMenu(tr("&编辑"));
    actionNew = menu21->addAction(tr("取消"));
    actionNew = menu21->addAction(tr("确认"));
    //QMenu* menu22 = menu21->addMenu(tr("&选择"));
    //QMenu* menu31 = menuBar->addMenu(tr("&调试"));
    //QMenu* menu41 = menuBar->addMenu(tr("&工具"));
    QMenu* menu51 = menuBar->addMenu(tr("&版本"));
    actionNew = menu51->addAction(tr("&Abort"));
    actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menuBar->show();
   //layout->setMenuBar(menuBar);
   //setLayout(layout);
    QTimer *timer=new QTimer(this);

   //新建定时器
    connect (timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
   //关联定时器计满信号和相应的槽函数
    timer->start(1000);
   //定时器开始计时，间隔1000毫秒
    timerUpDate();

    ui->lineEdit_5->setVisible(false);
    ui->lineEdit_6->setVisible(false);
    ui->lineEdit_7->setVisible(false);
    ui->lineEdit_8->setVisible(false);
    ui->lineEdit_9->setVisible(false);
    ui->lineEdit_10->setVisible(false);
    ui->lineEdit_11->setVisible(false);
    ui->lineEdit_12->setVisible(false);
    ui->lineEdit_13->setVisible(false);
    ui->lineEdit_14->setVisible(false);
    ui->lineEdit_15->setVisible(false);
    ui->lineEdit_16->setVisible(false);
    ui->lineEdit_17->setVisible(false);
    ui->lineEdit_18->setVisible(false);
    ui->lineEdit_19->setVisible(false);
    ui->lineEdit_20->setVisible(false);
    ui->lineEdit_21->setVisible(false);
    ui->lineEdit_24->setVisible(false);
    ui->lineEdit_5->setValidator(new QDoubleValidator);
    ui->lineEdit_6->setValidator(new QIntValidator);
    ui->lineEdit_7->setValidator(new QDoubleValidator);
    ui->lineEdit_8->setValidator(new QIntValidator);
    ui->lineEdit_9->setValidator(new QDoubleValidator);
    ui->lineEdit_10->setValidator(new QDoubleValidator);
    ui->lineEdit_11->setValidator(new QIntValidator);
    ui->lineEdit_12->setValidator(new QIntValidator);
    ui->lineEdit_13->setValidator(new QDoubleValidator);
    ui->lineEdit_14->setValidator(new QIntValidator);
    ui->lineEdit_15->setValidator(new QDoubleValidator);
    ui->lineEdit_16->setValidator(new QIntValidator);
    ui->lineEdit_17->setValidator(new QDoubleValidator);
    ui->lineEdit_18->setValidator(new QDoubleValidator);
    ui->lineEdit_19->setValidator(new QIntValidator);
    ui->lineEdit_20->setValidator(new QIntValidator);
    ui->lineEdit_21->setValidator(new QIntValidator);
    ui->lineEdit_24->setValidator(new QIntValidator);
}


MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
    stopped = false;
}

int FrmIdx = 0;

void MyThread::run()
{
    while (!stopped)
    {
            int recv_num = 0, once_len,tol_len = recv_len;
            while(tol_len != 0)
            {
                once_len = recv(sock_fd,(recv_buf +recv_len - tol_len),tol_len,0);
                //printf(" received  %d  bytes this time\r\n", once_len);
                tol_len -= once_len;
                recv_num += once_len;
            }

            //printf("one package received, length: %d\r\n", recv_num);
            if(recv_num < 0)
            {
                    perror("recv");
                    exit(1);
                  //  delay();
            }
            else if(recv_num !=0 )
            {
                    recv_buf[recv_num]='\0';
                    //printf("recv %d bytes\n",recv_len);

                    memcpy(&e,recv_buf,sizeof( struct PHY_Data_Send));
                    refreshed = 1;
                    Mrefreshed = 1;
                    if(Mrefreshed == 1)
                    {
                        // accumulate SNR data
                        for(int i=0; i<99; i++)
                        {
                            GlobalSNR[i] = GlobalSNR[i+1];
                        }
                        double sum = 0;
                        for(int i=0; i<14; i++)
                        {
                            sum = sum + e.snr_data[i];
                        }
                        GlobalSNR[99] = sum/14.0;


                        // QAM short to double
                        for(int i=0; i<14; i++)
                        {
                            for(int j=0; j<300; j++)
                            {
                                GlobalQAM[i][j][0] = e.cons_dia[i][j][0];
                                GlobalQAM[i][j][1] = e.cons_dia[i][j][1];
                            }
                        }
                        //powerNormalization(GlobalQAM);
                        for(int i=0; i<14; i++)
                        {
                            double psum = 0,  avgp = 0;
                            for(int j=0; j < 300 ;j++)
                            {
                                    psum += (GlobalQAM[i][j][0])*(GlobalQAM[i][j][0]) + (GlobalQAM[i][j][1])*(GlobalQAM[i][j][1]);
                            }
                            avgp = sqrt(psum / 168);   // after changing the number of rb,  it should change from 300 to 168
                            //cout<<"avgp="<<avgp<<endl;
                            //cout<<"psum="<<psum<<endl;
                            if(avgp != 0)
                            {
                                    for(int j=0; j < 300 ;j++)
                                    {
                                            GlobalQAM[i][j][0] = (GlobalQAM[i][j][0])/avgp;
                                            GlobalQAM[i][j][1] = (GlobalQAM[i][j][1])/avgp;
                                    }
                            }
                            else
                                    ;
                        }
                        FILE *fpPN;
                        fpPN = fopen("hsq.txt", "w+");
                        for(int i=0; i<14; i++)
                        {
                            for(int j=0; j<300; j++)
                            {
                                fprintf(fpPN, "x=%f\ny=%f\n", GlobalQAM[i][j][0], GlobalQAM[i][j][1]);
                            }
                        }
                        fclose(fpPN);

//                        // accumulate EST data
//                        for(int i=8; i>=0; i--)
//                        {
//                            for(int j=0; j<14; j++)
//                            {
//                                for(int k=0; k<512; k++)
//                                {
//                                    GlobalESTdatas[14*(i+1)+j][k][0] = GlobalESTdatas[14*i+j][k][0];
//                                    GlobalESTdatas[14*(i+1)+j][k][1] = GlobalESTdatas[14*i+j][k][1];
//                                }
//                            }
//                        }
//                        for(int i=0; i<14; i++)
//                        {
//                            for(int j=0; j<512; j++)
//                            {
//                                GlobalESTdatas[i][j][0] = e.chan_info[i][j][0];
//                                GlobalESTdatas[i][j][1] = e.chan_info[i][j][1];
//                            }
//                        }

                        Mrefreshed = 0;
                    }
                    //printf("fb:%f\n",e.buffer.fb);
                    //printf("na:%d\n",e.buffer.na);
                    memcpy(c.a[FrmIdx],e.buffer.FFTBuffer,7168*4);
                    //printf("UU:%d\n",FrmIdx);

                   FILE *fp;
                   short loop;
                   if(1)  // 处理信道估计的包
                   {
//                       fp=fopen("ESTdata.txt", "w+");
//                       fprintf(fp,"count:%d       ",e.send_c);
//                       fprintf(fp,"时隙：%d，天线号：%d， OFDM符号： %d，%d\r\n",e.eNB_offset,e.antenna,e.ofdm1,e.ofdm2);
//                       fprintf(fp,"原始数据：");

                       for(int i=0; i<14; i++)
                       {
                           for(loop = 0; loop<512; loop++)
                           {
                               if(loop == 0)
                               {
                                   //fprintf(fp,"\r\n%d:     %d+%di  ",loop<<1,e.chan_info[i][loop][0],e.chan_info[i][loop][1]);
                                   continue;
                               }
                               if((loop%10) == 0 )
                               {
                                   //fprintf(fp,"\r\n%d:     %d+%di  ",loop<<1,e.chan_info[i][loop][0],e.chan_info[i][loop][1]);
                               }
                               else
                               {
                                   //fprintf(fp,"%d+%di  ",e.chan_info[i][loop][0],e.chan_info[i][loop][1]);
                               }
                                 //printf("fb:%d\n",e.chan_info[2]);
                                 //fprintf(fp,"%d\r\n%d\r\n",e.chan_info[loop<<1],e.chan_info[(loop<<1)+1]);
                           }
                       }
                       //fprintf(fp,"\r\none frame end\r\n\r\n\r\n");
                       //fclose(fp);
                   }

                   if(1)//处理星座图的包
                   {
//                       fp=fopen("QAMdata.txt", "w+");
//                       fprintf(fp,"count:%d       ",e.send_c2);
//                       fprintf(fp,"data：\r\n");
                       for(int i=0; i<14; i++)
                       {
                           for (int j = 0; j < 300; j++)
                           {
                               //fprintf(fp, "%d %d\n", e.cons_dia[i][j][0],e.cons_dia[i][j][1]);
                           }
                       }
                       //fclose(fp);
                   }
                   FrmIdx++;
                   ShowFlag=1;
                   if(FrmIdx > 9)
                   {
                         FrmIdx = 0;
                   }
             }
       }
}

void MyThread::stop()
{
    stopped = true;
}

void delay(int x)
{
    int i,j;
   for(i=0;i<x;i++)
        for(j=0;j<110;j++);
}


void MainWindow::connectL()//socket 链接
{
    struct sockaddr_in addr_serv;//服务器端地址
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd < 0)
    {
            perror("sock");
            exit(1);
    }
    else
    {
            printf("sock sucessful:\n");
    }
    memset(&addr_serv,0,sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port =  htons(DEST_PORT);
    addr_serv.sin_addr.s_addr = inet_addr(DEST_IP_ADDRESS);
    if( ::connect(sock_fd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr)) < 0)//三次握手
    {
                      perror("connect");
                      printf("connect (%d)\n",errno);
                      exit(1);
   }
    else
    {
            printf("connect sucessful\n");
            flag=1;
   }

}

void process_info(int s)
{
               int send_num;
               //这里添加从qt 发送到netbeans 中的数据
                sa.buffer.na=1;
                sa.He.length=sizeof(struct PHY_Data_Send );
                 char c[sizeof (struct PHY_Data_Send )];
                 for(int i=0; i<10; i++)
                 {
                     cout<<sa.upParameters[i]<<endl;
                 }

               memcpy(c,&sa,sizeof(struct PHY_Data_Send));

                   printf("begin send\n");
                   send_num = send(s,c,(sizeof (struct PHY_Data_Send)),0);//发送函数
                          if (send_num < 0)
                          {
                                  perror("send");
                                  exit(1);
                          }
                          else
                          {
                                printf("Send Sucess(send button)!\n");
                          }
}

void process_infoL(int s)  // tansmit trigger information;
{
                  int send_num;
                  char b[sizeof (struct PHY_Data_Send)];
                  trigger_for_receive.He.type=0;
                  memcpy(b,&trigger_for_receive,sizeof(PHY_Data_Send));
                   send_num = send(s,b,sizeof(struct PHY_Data_Send),0);
                      if (send_num < 0)
                      {
                              perror("send trigger information error\n");
                              exit(1);
                     }
                      else
                      {
                          printf("Send Success!\n");
                      }
}
void process_infoLL(int s)  // tansmit trigger information;
{
                  int send_num;
                  char b[sizeof (struct PHY_Data_Send)];
                  trigger_for_receive.He.type=2;
                  memcpy(b,&trigger_for_receive,sizeof(PHY_Data_Send));
                   send_num = send(s,b,sizeof(struct PHY_Data_Send),0);
                      if (send_num < 0)
                      {
                              perror("send trigger information error\n");
                              exit(1);
                     }
                      else
                      {
                          printf("Stop Receive Data!!!\n");
                      }
}


void MainWindow::sendL()
{
    process_info(sock_fd);
    ::close(sock_fd);
}

void MainWindow::Receive()
{
    process_infoL(sock_fd);
      ::close(sock_fd);
}


void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
         return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::timerUpDate()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间

    update();

    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式

    ui->showtime->setText(str);//在标签上显示时间
}



MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
   int choose;
   sa.buffer.system_paras.TxPower=ui->TxPower->currentIndex();
   sa.buffer.system_paras.RxPower=ui->RxPower->currentIndex();
   sa.buffer.system_paras.RFPoint=ui->RFPoint->currentIndex();
   sa.buffer.system_paras.IFPoint=ui->IFPoint->currentIndex();
   sa.buffer.system_paras.BW=ui->BW->value();
   choose=ui->nb_antennas_tx->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.system_paras.nb_antennas_tx=1;
       break;
   case 2:
       sa.buffer.system_paras.nb_antennas_tx=2;
       break;
   case 3:
       sa.buffer.system_paras.nb_antennas_tx=4;
       break;
   case 4:
       sa.buffer.system_paras.nb_antennas_tx=8;
       break;
   case 5:
       sa.buffer.system_paras.nb_antennas_tx=16;
       break;
   case 6:
       sa.buffer.system_paras.nb_antennas_tx=32;
       break;

   }

   choose=ui->nb_antennas_rx->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.system_paras.nb_antennas_rx=1;
       break;
   case 2:
       sa.buffer.system_paras.nb_antennas_rx=2;
       break;
   case 3:
       sa.buffer.system_paras.nb_antennas_rx=4;
       break;
   case 4:
       sa.buffer.system_paras.nb_antennas_rx=8;
       break;
   case 5:
       sa.buffer.system_paras.nb_antennas_rx=16;
       break;
   case 6:
       sa.buffer.system_paras.nb_antennas_rx=32;
   break;

   }

      sa.buffer.system_paras.frame_type=ui->frame_type->currentIndex();
//系统参数设置


   choose=ui->comboBox_7->currentIndex()+1;
   switch(choose)
   {
   case 1:
      sa.buffer.uplink_paras.Bandwidth=3;
    break;
   case 2:
      sa.buffer.uplink_paras.Bandwidth=5;
    break;
   case 3:
      sa.buffer.uplink_paras.Bandwidth=10;
   break;
   case 4:
      sa.buffer.uplink_paras.Bandwidth=15;
   break;
   case 5:
      sa.buffer.uplink_paras.Bandwidth=20;
   break;
   }

   choose=ui->length_of_slot->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.uplink_paras.length_of_slot=0.5;
       break;
   case 2:
       sa.buffer.uplink_paras.length_of_slot=1;
   break;
   }

   choose=ui->subcarrier_interval->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.uplink_paras.subcarrier_interval=15000;
       break;
   case 2:
       sa.buffer.uplink_paras.subcarrier_interval=7500;
   break;
   }


   sa.buffer.uplink_paras.samples_per_tti=ui->samples_per_tti->currentIndex()*1.288;
   choose=ui->ofdm_symbol_size->currentIndex()+1;
   switch(choose)
   {
   case 1:
      sa.buffer.uplink_paras.ofdm_symbol_size=256;
      break;
   case 2:
      sa.buffer.uplink_paras.ofdm_symbol_size=512;
      break;
   case 3:
      sa.buffer.uplink_paras.ofdm_symbol_size=1024;
      break;
   case 4:
      sa.buffer.uplink_paras.ofdm_symbol_size=1536;
      break;
   case 5:
      sa.buffer.uplink_paras.ofdm_symbol_size=2048;
      break;
   case 6:
      sa.buffer.uplink_paras.ofdm_symbol_size=4096;
    break;

   }


   sa.buffer.uplink_paras.sample_time_per_point=ui->samples_per_tti->currentIndex();
   choose=ui->N_RB_UL->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.uplink_paras.N_RB_UL=6;
       break;
   case 2:
       sa.buffer.uplink_paras.N_RB_UL=13;
       break;
   case 3:
       sa.buffer.uplink_paras.N_RB_UL=25;
       break;
   case 4:
       sa.buffer.uplink_paras.N_RB_UL=50;
       break;
   case 5:
       sa.buffer.uplink_paras.N_RB_UL=75;
       break;
   case 6:
       sa.buffer.uplink_paras.N_RB_UL=100;
       break;
   case 7:
       sa.buffer.uplink_paras.N_RB_UL=110;
   break;
    }

   sa.buffer.uplink_paras.symbols_per_tti=ui->symbols_per_tti->currentIndex()+1;
   switch(choose)
   {
   case 1:
      sa.buffer.uplink_paras.symbols_per_tti=7;
      break;
   case 2:
      sa.buffer.uplink_paras.symbols_per_tti=6;
      break;
   case 3:
      sa.buffer.uplink_paras.symbols_per_tti=3;
   break;
   }
   choose=ui->Ncp_UL->currentIndex()+1;
   switch(choose)
   {
   case 1:
    sa.buffer.uplink_paras.Ncp_UL=36;
    break;
   case 2:
    sa.buffer.uplink_paras.Ncp_UL=40;
    break;
   }

//上行信道

   choose=ui->comboBox_7->currentIndex()+1;
   switch(choose)
   {
   case 1:
     sa.buffer.pusch_paras.Bandwidth=36;//系统带宽
   break;
   }
//PUSCH信道设置
   sa.buffer.srs_paras.enable_flag=ui->enable_flag->currentIndex();
   choose=ui->comboBox_7->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.srs_paras.srs_Bandwidth=36;
   }
//SRS信道设置
   choose=ui->comboBox_8->currentIndex()+1;
   switch(choose)
   {
   case 1:
      sa.buffer.downlink_paras.Bandwidth=3;
    break;
   case 2:
      sa.buffer.downlink_paras.Bandwidth=5;
    break;
   case 3:
      sa.buffer.downlink_paras.Bandwidth=10;
   break;
   case 4:
      sa.buffer.downlink_paras.Bandwidth=15;
   break;
   case 5:
      sa.buffer.downlink_paras.Bandwidth=20;
   break;
   }
   choose=ui->down_length_of_slot->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.downlink_paras.length_of_slot=0.5;
       break;
   case 2:
       sa.buffer.downlink_paras.length_of_slot=1;
   break;
   }
   choose=ui->down_subcarrier_interval->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.downlink_paras.subcarrier_interval=15000;
       break;
   case 2:
       sa.buffer.downlink_paras.subcarrier_interval=7500;
   break;
   }
   choose=ui->down_ofdm_symbol_size->currentIndex()+1;
   switch(choose)
   {
   case 1:
      sa.buffer.downlink_paras.ofdm_symbol_size=256;
      break;
   case 2:
      sa.buffer.downlink_paras.ofdm_symbol_size=512;
      break;
   case 3:
      sa.buffer.downlink_paras.ofdm_symbol_size=1024;
      break;
   case 4:
      sa.buffer.downlink_paras.ofdm_symbol_size=1536;
      break;
   case 5:
      sa.buffer.downlink_paras.ofdm_symbol_size=2048;
      break;
   case 6:
      sa.buffer.downlink_paras.ofdm_symbol_size=4096;
   break;
   }
   sa.buffer.downlink_paras.samples_per_tti=(ui->BW->value())*1.288;
  //sa.downlink_paras.ofdm_symbol_size=ui->ofdm_symbol_size->currentIndex();
   sa.buffer.downlink_paras.sample_time_per_point=1/(sa.buffer.downlink_paras.samples_per_tti);

   choose=ui->down_N_RB_UL->currentIndex()+1;
   switch(choose)
   {
   case 1:
       sa.buffer.downlink_paras.N_RB_UL=6;
       break;
   case 2:
       sa.buffer.downlink_paras.N_RB_UL=13;
       break;
   case 3:
       sa.buffer.downlink_paras.N_RB_UL=25;
       break;
   case 4:
       sa.buffer.downlink_paras.N_RB_UL=50;
       break;
   case 5:
       sa.buffer.downlink_paras.N_RB_UL=75;
       break;
   case 6:
       sa.buffer.downlink_paras.N_RB_UL=100;
       break;
   case 7:
       sa.buffer.downlink_paras.N_RB_UL=110;
   break;
    }
choose=ui->down_symbols_per_tti->currentIndex()+1;
switch(choose)
{
    case 1:
        sa.buffer.downlink_paras.symbols_per_tti=7;
        break;
    case 2:
        sa.buffer.downlink_paras.symbols_per_tti=6;
        break;
    case 3:
        sa.buffer.downlink_paras.symbols_per_tti=3;
    break;
}
choose=ui->down_Ncp_UL->currentIndex()+1;
switch(choose)
{
    case 1:
        sa.buffer.downlink_paras.Ncp_UL=36;
        break;
    case 2:
        sa.buffer.downlink_paras.Ncp_UL=40;
    break;
}
//下行信道
   sa.buffer.pdsch_paras.Bandwidth=ui->down_Bandwidth->value();
   sa.buffer.pdcch_paras.Bandwidth=ui->down_Bandwidth_2->value();
   sa.buffer.pdcch_paras.enable_flag=ui->down_enable_flag_2->currentIndex();
//PDSCH信道设置
   sa.buffer.pbch_paras.enable_flag=ui->down_enable_flag->currentIndex();
//PBCH信道设置
}
void MainWindow::on_pushButton_3_clicked()
{
    close();
}


void MainWindow::on_uconfirm_clicked()
{
        /* up.s1=ui->csdk->value();
         up.s2=ui->csdk_unit->currentIndex();
         up.s3=ui->sxcd->value();
         up.s4=ui->sxcd_unit->currentIndex();
         up.s5=ui->zzbjg->value();
         up.s6=ui->zzbjg_unit->currentIndex();
         up.s7=ui->qypl->value();
         up.s8=ui->qypl_unit->currentIndex();
         up.s9=ui->fft->currentIndex();
         up.s10=ui->qysj->currentIndex();
         up.s11=ui->zyzzbs->currentIndex();
         up.s12=ui->ofdm->value();
         up.s13=ui->cp->value();
         up.s14=ui->pucchdk->value();
         up.s15=ui->pucchdk_unit->currentIndex();
         up.s16=ui->puschyh->value();
         up.s17=ui->srssn->value();
         up.s18=ui->srsdk->value();
         up.s19=ui->srsdk_unit->currentIndex();
         up.s20=ui->srsyh->value();
         up.s21=ui->pucchsn->value();
         up.s22=ui->pucchdk->value();
         up.s23=ui->pucchdk_unit->currentIndex();
         up.s24=ui->pucchyh->value();
         down.s1=ui->dcsdk->value();
         down.s2=ui->dcsdk_unit->currentIndex();
         down.s3=ui->dsxcd->value();
         down.s4=ui->dsxcd_unit->currentIndex();
         down.s5=ui->dzzbjg->value();
         down.s6=ui->dzzbjg_unit->currentIndex();
         down.s7=ui->dqypl->value();
         down.s8=ui->dqypl_unit->currentIndex();
         down.s9=ui->dfft->currentIndex();
         down.s10=ui->dqysj->currentIndex();
         down.s11=ui->dzyzzbs->currentIndex();
         down.s12=ui->dofdm->value();
         down.s13=ui->dcp->value();
         down.s14=ui->dpdschdk->value();
         down.s15=ui->dpdschdk_unit->currentIndex();
         down.s16=ui->dpdschyh->value();
         down.s17=ui->dpbchsn->value();
         down.s18=ui->dpdcchsn->value();
         down.s19=ui->dpdcchdk->value();
         down.s20=ui->dpdcchdk_unit->currentIndex();
         down.s21=ui->dpdcchyh->value();
         down.s22=ui->dxxtbsn->value();*/
}


void MainWindow::on_pushButton_connect_clicked()   // connect
{
    connectL();
    ui->pushButton_connect->setEnabled(false);
}
void MainWindow::on_pushButton_send_clicked()       //send
{
    sa.He.type=1;
    sa.isUpParameter = 1;

    // transfer up parameters
    sa.upParameters[0] = ui->comboBox_6->currentIndex();
    sa.upParameters[1] = ui->comboBox_7->currentIndex();
    sa.upParameters[2] = ui->length_of_slot->currentIndex();
    sa.upParameters[3] = ui->subcarrier_interval->currentIndex();
    sa.upParameters[4] = ui->samples_per_tti->currentIndex();
    sa.upParameters[5] = ui->ofdm_symbol_size->currentIndex();
    sa.upParameters[6] = ui->sample_time_per_point->currentIndex();
    sa.upParameters[7] = ui->N_RB_UL->currentIndex();
    sa.upParameters[8] = ui->symbols_per_tti->currentIndex();
    sa.upParameters[9] = ui->Ncp_UL->currentIndex();
//    for(int i=0; i<10; i++)
//    {
//        cout<<sa.upParameters[i]<<endl;
//    }

    // transfer down parameters
    sa.downParameters[0] = ui->comboBox_11->currentIndex();
    sa.downParameters[1] = ui->comboBox_8->currentIndex();
    sa.downParameters[2] = ui->down_length_of_slot->currentIndex();
    sa.downParameters[3] = ui->down_subcarrier_interval->currentIndex();
    sa.downParameters[4] = ui->down_samples_per_tti->currentIndex();
    sa.downParameters[5] = ui->down_ofdm_symbol_size->currentIndex();
    sa.downParameters[6] = ui->down_sample_time_per_point->currentIndex();
    sa.downParameters[7] = ui->down_N_RB_UL->currentIndex();
    sa.downParameters[8] = ui->down_symbols_per_tti->currentIndex();
    sa.downParameters[9] = ui->down_Ncp_UL->currentIndex();

    process_info(sock_fd);
    ui->pushButton_connect->setEnabled(true);
}

void MainWindow::on_pushButton_receive_clicked()     // receive
{
    process_infoL(sock_fd);
    ui-> pushButton_connect->setEnabled(true);
}

//double MainWindow::snr_estimation(short *test5,int size)
//{
//        int index=0;
//        int sum_i=0;
//        int sum_q=0;
//        int avg_i=0;
//        int avg_q=0;
//        short data_i[300];
//        short data_q[300];
//        int noise_pow=0;
//        int signal_pow=0;
//        double snr=0.0;
//        for(int i=0;i<size;i++)
//        {
//            if(test5[i<<1]>0&&test5[(i<<1)+1]>0)
//            {
//                data_i[index]=test5[i<<1];
//                sum_i+=data_i[index];
//                data_q[index]=test5[(i<<1)+1];
//                sum_q+=data_q[index];
//                index++;
//            }
//        }
//        if(index == 0)
//        {
//            return 0;
//        }
//        avg_i=sum_i/index;
//        avg_q=sum_q/index;
//        for(int i=0;i<index;i++)
//        {
//            noise_pow+=(pow(data_i[i]-avg_i,2)+pow(data_q[i]-avg_q,2))/index;
//            signal_pow+=(pow(data_i[i],2)+pow(data_q[i],2))/index;
//        }
//        if(signal_pow == 0)
//            signal_pow = 1;
//        if(noise_pow == 0)
//            noise_pow = 1;
//        snr=double(10*log10(signal_pow/noise_pow));
//        return snr;
//}

void MainWindow::on_pushButton_14_clicked()
{
    ESTMObject.close();
    QAMObject.close();
    FFTObject.close();
    SNRObject.close();
    ESTPObject.close();

    int x=0, y=50;

    // judge whether show or not
    if(ui->checkBox->isChecked() == 1)
    {
        ESTMObject.move(x,y);
        x+=500;
        y+=0;
        ESTMObject.show();
    }
    if(ui->checkBox_2->isChecked() == 1)
    {
        QAMObject.move(x,y);
        x+=450;
        y+=0;
        QAMObject.show();
    }
    if(ui->checkBox_3->isChecked() == 1)
    {
        FFTObject.move(x,y);
        x+=-950;
        y+=400;
        FFTObject.show();
    }
    if(ui->checkBox_4->isChecked() == 1)
    {
        SNRObject.move(x,y);
        x+=500;
        y+=0;
        SNRObject.show();
    }
    if(ui->checkBox_5->isChecked() == 1)
    {
        ESTPObject.move(x,y);
        x+=500;
        y+=0;
        ESTPObject.show();
    }

    /************************transmit data***************************/
    while(ShowFlag)
    {
        while(refreshed == 1)
        {
            // get SNR data
            for(int i=0; i<100; i++)
            {
                SNRObject.snr[i] = GlobalSNR[i];
            }

            // get FFT data
            for(int i=0; i<256; i++)
            {
               FFTObject.receiver[i][0] = e.fft_data[512+i*2];
               FFTObject.receiver[i][1] = e.fft_data[512+i*2+1];
            ;
            }
            for(int i=0; i<256; i++)
            {
                FFTObject.receiver[256+i][0] = e.fft_data[i*2];
                FFTObject.receiver[256+i][1] = e.fft_data[i*2+1];
            ;
            }


            // get EST Magnitude data
            for(int i=0; i<300; i++)
            {
                ESTMObject.receiver[i][0] = e.chan_info[0][i][0];
                ESTMObject.receiver[i][1] = e.chan_info[0][i][1];
            }

            // get EST Phase data
            for(int i=0; i<300; i++)
            {
                ESTPObject.receiver[i][0] = e.chan_info[0][i][0];
                ESTPObject.receiver[i][1] = e.chan_info[0][i][1];
            }

            // get QAM data
            for(int i=0; i<14; i++)
            {
                for(int j=0; j<300; j++)
                {
                    QAMObject.receiver[i][j][0] = GlobalQAM[i][j][0];
                    QAMObject.receiver[i][j][1] = GlobalQAM[i][j][1];
                }
            }
            refreshed = 0;
            ESTMObject.sleep(1);
            QAMObject.sleep(1);

        }
    }
    //ESTMObject->close();
}

void MainWindow::on_pushButton_connect_2_clicked()     //  start
{
    process_infoL(sock_fd);
     thread.start();
    ui->pushButton_connect_2->setEnabled(false);
    ui->pushButton_connect_3->setEnabled(true);
}

void MainWindow::on_pushButton_connect_3_clicked()    //end
{
    process_infoLL(sock_fd);
    if (thread.isRunning())
    {
         thread.exit();
         ShowFlag=0;
         ui->pushButton_connect_2->setEnabled(true);
         ui->pushButton_connect_3->setEnabled(false);
    }

}



void MainWindow::on_comboBox_6_activated(int index)
{
    switch(index)
    {
    case 0:
        ui->length_of_slot->setCurrentIndex(0);
        ui->length_of_slot->setDisabled(1);
        ui->subcarrier_interval->setCurrentIndex(0);
        ui->subcarrier_interval->setDisabled(1);
        ui->samples_per_tti->setCurrentIndex(0);
        ui->samples_per_tti->setDisabled(1);
        ui->ofdm_symbol_size->setCurrentIndex(0);
        ui->ofdm_symbol_size->setDisabled(1);
        ui->sample_time_per_point->setCurrentIndex(0);
        ui->sample_time_per_point->setDisabled(1);
        ui->N_RB_UL->setCurrentIndex(0);
        ui->N_RB_UL->setDisabled(1);
        ui->symbols_per_tti->setCurrentIndex(0);
        ui->symbols_per_tti->setDisabled(1);
        ui->Ncp_UL->setCurrentIndex(0);
        ui->Ncp_UL->setDisabled(1);
        ui->lineEdit_5->setVisible(false);
        ui->lineEdit_6->setVisible(false);
        ui->lineEdit_7->setVisible(false);
        ui->lineEdit_8->setVisible(false);
        ui->lineEdit_9->setVisible(false);
        ui->lineEdit_10->setVisible(false);
        ui->lineEdit_11->setVisible(false);
        ui->lineEdit_12->setVisible(false);
        break;

    case 1:
        ui->length_of_slot->setDisabled(1);
        ui->subcarrier_interval->setDisabled(1);
        ui->samples_per_tti->setDisabled(1);
        ui->ofdm_symbol_size->setDisabled(1);
        ui->sample_time_per_point->setDisabled(1);
        ui->N_RB_UL->setDisabled(1);
        ui->symbols_per_tti->setDisabled(1);
        ui->Ncp_UL->setDisabled(1);
        ui->lineEdit_5->setVisible(false);
        ui->lineEdit_6->setVisible(false);
        ui->lineEdit_7->setVisible(false);
        ui->lineEdit_8->setVisible(false);
        ui->lineEdit_9->setVisible(false);
        ui->lineEdit_10->setVisible(false);
        ui->lineEdit_11->setVisible(false);
        ui->lineEdit_12->setVisible(false);
        break;

    case 2:
        ui->length_of_slot->setDisabled(1);
        ui->subcarrier_interval->setDisabled(1);
        ui->samples_per_tti->setDisabled(1);
        ui->ofdm_symbol_size->setDisabled(1);
        ui->sample_time_per_point->setDisabled(1);
        ui->N_RB_UL->setDisabled(1);
        ui->symbols_per_tti->setDisabled(1);
        ui->Ncp_UL->setDisabled(1);
        ui->lineEdit_5->setVisible(false);
        ui->lineEdit_6->setVisible(false);
        ui->lineEdit_7->setVisible(false);
        ui->lineEdit_8->setVisible(false);
        ui->lineEdit_9->setVisible(false);
        ui->lineEdit_10->setVisible(false);
        ui->lineEdit_11->setVisible(false);
        ui->lineEdit_12->setVisible(false);
        break;

    case 3:
        ui->length_of_slot->setEnabled(1);
        ui->subcarrier_interval->setEnabled(1);
        ui->samples_per_tti->setEnabled(1);
        ui->ofdm_symbol_size->setEnabled(1);
        ui->sample_time_per_point->setEnabled(1);
        ui->N_RB_UL->setEnabled(1);
        ui->symbols_per_tti->setEnabled(1);
        ui->Ncp_UL->setEnabled(1);        
        ui->lineEdit_5->setVisible(true);
        ui->lineEdit_6->setVisible(true);
        ui->lineEdit_7->setVisible(true);
        ui->lineEdit_8->setVisible(true);
        ui->lineEdit_9->setVisible(true);
        ui->lineEdit_10->setVisible(true);
        ui->lineEdit_11->setVisible(true);
        ui->lineEdit_12->setVisible(true);
        break;

      case 4:
        ui->lineEdit_5->setVisible(true);
        ui->lineEdit_6->setVisible(true);
        ui->lineEdit_7->setVisible(true);
        ui->lineEdit_8->setVisible(true);
        ui->lineEdit_9->setVisible(true);
        ui->lineEdit_10->setVisible(true);
        ui->lineEdit_11->setVisible(true);
        ui->lineEdit_12->setVisible(true);
        break;

    }
}


void MainWindow::on_comboBox_7_activated(int index)
{
    switch(ui->comboBox_6->currentIndex())
    {
    // LTE
    case 1:
    {
        switch(index)
        {
        case 0:
            ui->length_of_slot->setCurrentIndex(0);
            ui->subcarrier_interval->setCurrentIndex(0);
            ui->samples_per_tti->setCurrentIndex(0);
            ui->ofdm_symbol_size->setCurrentIndex(0);
            ui->sample_time_per_point->setCurrentIndex(0);
            ui->N_RB_UL->setCurrentIndex(0);
            ui->symbols_per_tti->setCurrentIndex(0);
            ui->Ncp_UL->setCurrentIndex(0);
            break;

        // 5M
        case 1:
            ui->length_of_slot->setCurrentIndex(1);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(1);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(1);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(1);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(1);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(1);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(1);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(1);
            ui->Ncp_UL->setDisabled(1);
            break;

        // 10M
        case 2:
            ui->length_of_slot->setCurrentIndex(2);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(2);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(2);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(2);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(2);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(2);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(2);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(2);
            ui->Ncp_UL->setDisabled(1);
            break;

        // 15M
        case 3:
            ui->length_of_slot->setCurrentIndex(3);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(3);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(3);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(3);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(3);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(3);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(3);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(3);
            ui->Ncp_UL->setDisabled(1);
            break;

        // 20M
        case 4:
            ui->length_of_slot->setCurrentIndex(4);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(4);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(4);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(4);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(4);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(4);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(4);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(4);
            ui->Ncp_UL->setDisabled(1);
            break;
        }
    }

    // WIFI
    case 2:
    {
        switch(index)
        {
        case 0:
            ui->length_of_slot->setCurrentIndex(0);
            ui->subcarrier_interval->setCurrentIndex(0);
            ui->samples_per_tti->setCurrentIndex(0);
            ui->ofdm_symbol_size->setCurrentIndex(0);
            ui->sample_time_per_point->setCurrentIndex(0);
            ui->N_RB_UL->setCurrentIndex(0);
            ui->symbols_per_tti->setCurrentIndex(0);
            ui->Ncp_UL->setCurrentIndex(0);
            break;

        // 5M
        case 1:
            ui->length_of_slot->setCurrentIndex(1);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(1);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(1);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(1);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(1);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(1);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(1);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(1);
            ui->Ncp_UL->setDisabled(1);
            break;

        // 10M
        case 2:
            ui->length_of_slot->setCurrentIndex(2);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(2);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(2);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(2);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(2);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(2);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(2);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(2);
            ui->Ncp_UL->setDisabled(1);
            break;

        // 15M
        case 3:
            ui->length_of_slot->setCurrentIndex(3);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(3);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(3);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(3);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(3);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(3);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(3);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(3);
            ui->Ncp_UL->setDisabled(1);
            break;

        // 20M
        case 4:
            ui->length_of_slot->setCurrentIndex(4);
            ui->length_of_slot->setDisabled(1);
            ui->subcarrier_interval->setCurrentIndex(4);
            ui->subcarrier_interval->setDisabled(1);
            ui->samples_per_tti->setCurrentIndex(4);
            ui->samples_per_tti->setDisabled(1);
            ui->ofdm_symbol_size->setCurrentIndex(4);
            ui->ofdm_symbol_size->setDisabled(1);
            ui->sample_time_per_point->setCurrentIndex(4);
            ui->sample_time_per_point->setDisabled(1);
            ui->N_RB_UL->setCurrentIndex(4);
            ui->N_RB_UL->setDisabled(1);
            ui->symbols_per_tti->setCurrentIndex(4);
            ui->symbols_per_tti->setDisabled(1);
            ui->Ncp_UL->setCurrentIndex(4);
            ui->Ncp_UL->setDisabled(1);
            break;
        }
    }

    }
}

// downwards protocol
void MainWindow::on_comboBox_11_activated(int index)
{
    switch(index)
    {
    case 0:
        ui->down_length_of_slot->setCurrentIndex(0);
        ui->down_length_of_slot->setDisabled(1);
        ui->down_subcarrier_interval->setCurrentIndex(0);
        ui->down_subcarrier_interval->setDisabled(1);
        ui->down_samples_per_tti->setCurrentIndex(0);
        ui->down_samples_per_tti->setDisabled(1);
        ui->down_ofdm_symbol_size->setCurrentIndex(0);
        ui->down_ofdm_symbol_size->setDisabled(1);
        ui->down_sample_time_per_point->setCurrentIndex(0);
        ui->down_sample_time_per_point->setDisabled(1);
        ui->down_N_RB_UL->setCurrentIndex(0);
        ui->down_N_RB_UL->setDisabled(1);
        ui->down_symbols_per_tti->setCurrentIndex(0);
        ui->down_symbols_per_tti->setDisabled(1);
        ui->down_Ncp_UL->setCurrentIndex(0);
        ui->down_Ncp_UL->setDisabled(1);
        ui->down_symbols_per_tti_2->setCurrentIndex(0);
        ui->down_symbols_per_tti_2->setDisabled(1);
        ui->lineEdit_13->setVisible(false);
        ui->lineEdit_14->setVisible(false);
        ui->lineEdit_15->setVisible(false);
        ui->lineEdit_16->setVisible(false);
        ui->lineEdit_17->setVisible(false);
        ui->lineEdit_18->setVisible(false);
        ui->lineEdit_19->setVisible(false);
        ui->lineEdit_20->setVisible(false);
        ui->lineEdit_21->setVisible(false);
        ui->lineEdit_24->setVisible(false);


        break;

    case 1:
        ui->down_length_of_slot->setDisabled(1);
        ui->down_subcarrier_interval->setDisabled(1);
        ui->down_samples_per_tti->setDisabled(1);
        ui->down_ofdm_symbol_size->setDisabled(1);
        ui->down_sample_time_per_point->setDisabled(1);
        ui->down_N_RB_UL->setDisabled(1);
        ui->down_symbols_per_tti->setDisabled(1);
        ui->down_Ncp_UL->setDisabled(1);
        ui->down_symbols_per_tti_2->setDisabled(1);
        ui->lineEdit_13->setVisible(false);
        ui->lineEdit_14->setVisible(false);
        ui->lineEdit_15->setVisible(false);
        ui->lineEdit_16->setVisible(false);
        ui->lineEdit_17->setVisible(false);
        ui->lineEdit_18->setVisible(false);
        ui->lineEdit_19->setVisible(false);
        ui->lineEdit_20->setVisible(false);
        ui->lineEdit_21->setVisible(false);
         ui->lineEdit_24->setVisible(false);
        break;

    case 2:
        ui->down_length_of_slot->setDisabled(1);
        ui->down_subcarrier_interval->setDisabled(1);
        ui->down_samples_per_tti->setDisabled(1);
        ui->down_ofdm_symbol_size->setDisabled(1);
        ui->down_sample_time_per_point->setDisabled(1);
        ui->down_N_RB_UL->setDisabled(1);
        ui->down_symbols_per_tti->setDisabled(1);
        ui->down_Ncp_UL->setDisabled(1);
        ui->down_symbols_per_tti_2->setDisabled(1);
        ui->lineEdit_13->setVisible(false);
        ui->lineEdit_14->setVisible(false);
        ui->lineEdit_15->setVisible(false);
        ui->lineEdit_16->setVisible(false);
        ui->lineEdit_17->setVisible(false);
        ui->lineEdit_18->setVisible(false);
        ui->lineEdit_19->setVisible(false);
        ui->lineEdit_20->setVisible(false);
        ui->lineEdit_21->setVisible(false);
         ui->lineEdit_24->setVisible(false);
        break;

    case 3:
        ui->down_length_of_slot->setEnabled(1);
        ui->down_subcarrier_interval->setEnabled(1);
        ui->down_samples_per_tti->setEnabled(1);
        ui->down_ofdm_symbol_size->setEnabled(1);
        ui->down_sample_time_per_point->setEnabled(1);
        ui->down_N_RB_UL->setEnabled(1);
        ui->down_symbols_per_tti->setEnabled(1);
        ui->down_Ncp_UL->setEnabled(1);
        ui->down_symbols_per_tti_2->setEnabled(1);

        ui->lineEdit_13->setVisible(true);
        ui->lineEdit_14->setVisible(true);
        ui->lineEdit_15->setVisible(true);
        ui->lineEdit_16->setVisible(true);
        ui->lineEdit_17->setVisible(true);
        ui->lineEdit_18->setVisible(true);
        ui->lineEdit_19->setVisible(true);
        ui->lineEdit_20->setVisible(true);
        ui->lineEdit_21->setVisible(true);
        ui->lineEdit_24->setVisible(true);
        break;

    }
}

void MainWindow::on_comboBox_8_activated(int index)
{
    switch(ui->comboBox_11->currentIndex())
    {
    // LTE
    case 1:
    {
        switch(index)
        {
        case 0:
            ui->down_length_of_slot->setCurrentIndex(0);
            ui->down_subcarrier_interval->setCurrentIndex(0);
            ui->down_samples_per_tti->setCurrentIndex(0);
            ui->down_ofdm_symbol_size->setCurrentIndex(0);
            ui->down_sample_time_per_point->setCurrentIndex(0);
            ui->down_N_RB_UL->setCurrentIndex(0);
            ui->down_symbols_per_tti->setCurrentIndex(0);
            ui->down_Ncp_UL->setCurrentIndex(0);
            ui->down_symbols_per_tti_2->setCurrentIndex(0);
            break;

        // 5M
        case 1:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(1);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(1);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(1);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(1);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;

        // 10M
        case 2:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(2);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(2);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(2);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(2);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;

        // 15M
        case 3:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(3);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(3);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(3);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(3);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;

        // 20M
        case 4:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(4);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(4);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(4);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(4);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;
        }
    }

    // WIFI
    case 2:
    {
        switch(index)
        {
        case 0:
            ui->down_length_of_slot->setCurrentIndex(0);
            ui->down_subcarrier_interval->setCurrentIndex(0);
            ui->down_samples_per_tti->setCurrentIndex(0);
            ui->down_ofdm_symbol_size->setCurrentIndex(0);
            ui->down_sample_time_per_point->setCurrentIndex(0);
            ui->down_N_RB_UL->setCurrentIndex(0);
            ui->down_symbols_per_tti->setCurrentIndex(0);
            ui->down_Ncp_UL->setCurrentIndex(0);
            ui->down_symbols_per_tti_2->setCurrentIndex(0);
            break;

        // 5M
        case 1:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(1);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(1);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(1);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(1);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;

        // 10M
        case 2:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(2);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(2);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(2);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(2);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;

        // 15M
        case 3:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(3);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(3);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(3);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(3);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;

        // 20M
        case 4:
            ui->down_length_of_slot->setCurrentIndex(1);
            ui->down_length_of_slot->setDisabled(1);
            ui->down_subcarrier_interval->setCurrentIndex(1);
            ui->down_subcarrier_interval->setDisabled(1);
            ui->down_samples_per_tti->setCurrentIndex(4);
            ui->down_samples_per_tti->setDisabled(1);
            ui->down_ofdm_symbol_size->setCurrentIndex(4);
            ui->down_ofdm_symbol_size->setDisabled(1);
            ui->down_sample_time_per_point->setCurrentIndex(1);
            ui->down_sample_time_per_point->setDisabled(1);
            ui->down_N_RB_UL->setCurrentIndex(4);
            ui->down_N_RB_UL->setDisabled(1);
            ui->down_symbols_per_tti->setCurrentIndex(1);
            ui->down_symbols_per_tti->setDisabled(1);
            ui->down_Ncp_UL->setCurrentIndex(4);
            ui->down_Ncp_UL->setDisabled(1);
            ui->down_symbols_per_tti_2->setCurrentIndex(1);
            ui->down_symbols_per_tti_2->setDisabled(1);
            break;


        }
    }

    }
}


void MainWindow::on_dconfirm_clicked()
{
    sa.He.type=1;
    sa.isDiagram = 1;
    QString str = ui->textEdit_2->toPlainText();
    int len = str.length();
    int i;
    for(i = 0; i < len; ++i)
    {
        QChar t = str.at(i);
        sa.texts[i] = t.unicode();
        cout<<sa.texts[i]<<endl;
    }
    sa.texts[i] = '\0';
    process_info(sock_fd);
}

/*void MainWindow::on_pushButton_12_clicked()
{

    ifstream iRead("/home/yaha/abcd.txt");
    char chRead[100];
    int k[50];
    int p;
    if( !iRead)
    {
      cout << "打开文件失败" << endl;
    }
    else
    {
      //读取，以回车为分隔符
      p=0; //用于指示int数组的下标
      int k[20]; //用于存放int的数组
      char chRead[100]; //读取缓冲区，
      char chTemp;
      int r; //用于指针字符数组缓冲区下标
      p=0;
      r=0;
      //逐个读取字符，遇到回车，说明已读完一个数
      while(iRead.get(chTemp))
      {
       if(chTemp=='\n')
       {
        //遇到Enter，说明读取完了一个数
        k[p] = atoi(chRead);
        cout << k[p] << endl;
        p++;
        //清空原缓冲区
        memset(chRead,0,sizeof(chRead));
        r=0;
       }
       else
       {
        chRead[r]=chTemp;
        r++;
       }
      }
      iRead.close();
    }
    printf("\n");
    system("pause");
    QString b;
    b=QString("%1 %2 %3").arg(k[0]).arg(k[1]).arg(k[2]);
            ui->lineEdit_22->setText(b);


            ui->lineEdit_5->setText( QString::number(k[0]));
            ui->lineEdit_6->setText( QString::number(k[1]));
            ui->lineEdit_7->setText( QString::number(k[2]));
            ui->lineEdit_8->setText( QString::number(k[3]));
            ui->lineEdit_9->setText( QString::number(k[4]));
            ui->lineEdit_10->setText( QString::number(k[5]));
            ui->lineEdit_11->setText( QString::number(k[6]));
            ui->lineEdit_12->setText( QString::number(k[7]));


}*/



void MainWindow::on_pushButton_connect_4_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open Document",
       QDir::currentPath(),
       "Document files (*.txt );;All files(*.*)");
    if (!filename.isNull()) { //用户选择了文件
       // 处理文件
       QMessageBox::information(this, "Document", "Has document", QMessageBox::Ok | QMessageBox::Cancel);
    } else // 用户取消选择

       QMessageBox::information(this, "Document", "No document", QMessageBox::Ok | QMessageBox::Cancel);
            ui->lineEdit_23->setText( filename);


            char*  ch;

            QByteArray ba = filename.toLatin1();

            ch=ba.data();

            ifstream iRead(ch);
            char chRead[100];
            float k[50];
            int p;
            if( !iRead)
            {
              cout << "打开文件失败" << endl;
            }
            else
            {
              //读取，以回车为分隔符
              p=0; //用于指示int数组的下标
              float k[20]; //用于存放int的数组
              char chRead[100]; //读取缓冲区，
              char chTemp;
              int r; //用于指针字符数组缓冲区下标
              p=0;
              r=0;
              //逐个读取字符，遇到回车，说明已读完一个数
              while(iRead.get(chTemp))
              {
               if(chTemp=='\n')
               {
                //遇到Enter，说明读取完了一个数
                k[p] = atoi(chRead);
               // cout << k[p] << endl;
                p++;
                //清空原缓冲区
                memset(chRead,0,sizeof(chRead));
                r=0;
               }
               else
               {
                chRead[r]=chTemp;
                r++;
               }
              }
              iRead.close();
            }
            printf("\n");
            system("pause");


                    ui->lineEdit_5->setText( QString::number(k[0]));
                    ui->lineEdit_6->setText( QString::number(k[1]));
                    ui->lineEdit_7->setText( QString::number(k[2]));
                    ui->lineEdit_8->setText( QString::number(k[3]));
                    ui->lineEdit_9->setText( QString::number(k[4]));
                    ui->lineEdit_10->setText( QString::number(k[5]));
                    ui->lineEdit_11->setText( QString::number(k[6]));
                    ui->lineEdit_12->setText( QString::number(k[7]));

}

void MainWindow::on_pushButton_12_clicked()
{
    QString read[200];
     read[0]= ui->lineEdit_13->text();
     read[1]= ui->lineEdit_14->text();
     read[2]= ui->lineEdit_15->text();
     read[3]= ui->lineEdit_16->text();
     read[4]= ui->lineEdit_17->text();
     read[5]= ui->lineEdit_18->text();
     read[6]= ui->lineEdit_19->text();
     read[7]= ui->lineEdit_20->text();
     read[8]= ui->lineEdit_21->text();
     read[9]= ui->lineEdit_24->text();
     read[10]=ui->down_Bandwidth->text();
     read[11]=ui->dpdschyh->text();
     read[12]=ui->down_enable_flag_2->currentIndex();
     read[13]=ui->down_Bandwidth_2->text();
     read[14]=ui->dpdcchyh->text();


     char*  ch[200];


      ofstream out("/home/yaha/save.txt");
      if (out.is_open())
         {
          for(int i=0;i<50;i++)
          {
              QByteArray ba = read[i].toLatin1();

              ch[i]=ba.data();
              out << ch[i];
              out << "\n";
          }
              out.close();
          }

}




void MainWindow::on_pushButton_connect_5_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open Document",
       QDir::currentPath(),
       "Document files (*.txt );;All files(*.*)");
    if (!filename.isNull()) { //用户选择了文件
       // 处理文件
       QMessageBox::information(this, "Document", "Has document", QMessageBox::Ok | QMessageBox::Cancel);
    } else // 用户取消选择

       QMessageBox::information(this, "Document", "No document", QMessageBox::Ok | QMessageBox::Cancel);
            ui->lineEdit_25->setText( filename);


            char*  ch;

            QByteArray ba = filename.toLatin1();

            ch=ba.data();

            ifstream iRead(ch);
            char chRead[100];
            int k[50];
            int p;
            if( !iRead)
            {
              cout << "打开文件失败" << endl;
            }
            else
            {
              //读取，以回车为分隔符
              p=0; //用于指示int数组的下标
              int k[20]; //用于存放int的数组
              char chRead[100]; //读取缓冲区，
              char chTemp;
              int r; //用于指针字符数组缓冲区下标
              p=0;
              r=0;
              //逐个读取字符，遇到回车，说明已读完一个数
              while(iRead.get(chTemp))
              {
               if(chTemp=='\n')
               {
                //遇到Enter，说明读取完了一个数
                k[p] = atoi(chRead);
             //   cout << k[p] << endl;
                p++;
                //清空原缓冲区
                memset(chRead,0,sizeof(chRead));
                r=0;
               }
               else
               {
                chRead[r]=chTemp;
                r++;
               }
              }
              iRead.close();
            }
            printf("\n");
            system("pause");


                    ui->lineEdit_13->setText( QString::number(k[0]));
                    ui->lineEdit_14->setText( QString::number(k[1]));
                    ui->lineEdit_15->setText( QString::number(k[2]));
                    ui->lineEdit_16->setText( QString::number(k[3]));
                    ui->lineEdit_17->setText( QString::number(k[4]));
                    ui->lineEdit_18->setText( QString::number(k[5]));
                    ui->lineEdit_19->setText( QString::number(k[6]));
                    ui->lineEdit_20->setText( QString::number(k[7]));
                    ui->lineEdit_21->setText( QString::number(k[8]));
                    ui->lineEdit_24->setText( QString::number(k[9]));
                    ui->down_Bandwidth->setValue( k[10]);
                    ui->dpdschyh->setValue( k[11]);
                    ui->down_enable_flag_2->setCurrentIndex( k[12]);
                    ui->down_Bandwidth_2->setValue( k[13]);
                    ui->dpdcchyh->setValue( k[14]);

}

