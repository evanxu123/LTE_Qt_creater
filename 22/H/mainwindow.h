#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QList>
#include <QtWidgets>
#include <QDialog>
#include<QAbstractSocket>
#include<QDialog>
#include"mythread.h"
#include <estimating_diagram.h>
#include <est_phase.h>
#include <constellationdiagram.h>
#include <H/dialog.h>
#include <snr.h>
class TextEdit;
class QMenu;
class QTcpSocket;
class QTcpServer;
namespace Ui {
class MainWindow;
}

/*namespace Ui {
    class Server;
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Estimating_Diagram ESTMObject;
    EST_Phase ESTPObject;
    Dialog FFTObject;
    ConstellationDiagram QAMObject;
    SNR SNRObject;

//int status=0;
    struct b{
     int na;
     float fb;
             }d;

   /*struct bag
    {
        float s1;
        float s2;
        float s3;
        float s4;
        float s5;
        float s6;
        float s7;
        float s8;
        float s9;
        float s10;
        float s11;
        float s12;
        float s13;
        float s14;
        float s15;
        float s16;
        float s17;
        float s18;
        float s19;
        float s20;
        float s21;
        float s22;
        float s23;
        float s24;
    }up,down,A;*/
    //int j=0;
   // int o=0;

       // int e[512][2];
        void sleep(unsigned int msec)
            {
                QTime dieTime=QTime::currentTime().addMSecs(msec);
                while(QTime::currentTime()<dieTime);
                QCoreApplication::processEvents(QEventLoop::AllEvents,100);
            }
private slots:

    void on_pushButton_clicked();

    void timerUpDate();
     private:
    QTcpSocket *tcpSocket;
    QString message;
    QTcpServer *tcpServer;
    // 用来存放数据的大小信息
    quint16 blockSize;

    Ui::MainWindow *ui;
    QVBoxLayout* layout;
    QMenuBar* menuBar;
    QAction* actionNew;


    //QMenu *fileMenu;
public:
        void on_pushopenfile_clicked();
private:
              MyThread thread;
private slots:
    // void newFile();
   // void on_pushButton_graphic_clicked();
    void on_pushButton_3_clicked();
       void loadFile(const QString &fileName);
    void open();


    void on_uconfirm_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_receive_clicked();

    void sendL();
    void connectL();
    void Receive();

    void FFT_Diagram();
    //void on_pushButton_choosewave_clicked();
   // void on_pushButton_choosewave_clicked();
    void QAM_Diagram();
    void EstimatingDiagram();
    void on_pushButton_connect_2_clicked();
    void on_pushButton_connect_3_clicked();

  //  void on_pushButton_graphic_3_clicked();
   // void on_comboBox_3_activated(const QString &arg1);
    void on_pushButton_14_clicked();
   // void on_comboBox_2_activated(const QString &arg1);
    //void on_Bandwidth_activated(const QString &arg1);
    //void on_comboBox_activated(const QString &arg1);
   // void on_comboBox_9_activated(const QString &arg1);
    //void on_comboBox_2_currentIndexChanged(int index);
   // void on_comboBox_2_activated(int index);
    //void on_comboBox_5_activated(int index);
    void on_comboBox_6_activated(int index);
   // void on_comboBox_7_activated(const QString &arg1);
    void on_comboBox_7_activated(int index);
    void on_comboBox_11_activated(int index);
    void on_comboBox_8_activated(int index);
    void on_dconfirm_clicked();
    //void on_pushButton_12_clicked();
    void on_pushButton_connect_4_clicked();
    //void on_comboBox_11_activated(const QString &arg1);
    void on_pushButton_12_clicked();
    void on_pushButton_connect_5_clicked();
};
#endif // MAINWINDOW_H
