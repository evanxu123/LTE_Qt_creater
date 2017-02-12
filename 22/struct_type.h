#ifndef STRUCT_TYPE_H
#define STRUCT_TYPE_H

typedef struct
{
int TxPower;//发送功率
int RxPower;//接受功率
int RFPoint;//射频频点
int IFPoint;//中频频点
int BW; //系统带宽
int nb_antennas_tx;//发射天线数
int nb_antennas_rx;//接收天线数
int frame_type;//测试模式
}SYSTEM_PARAMETERS;
typedef struct
{
int Bandwidth;
float length_of_slot;//时隙长度0.5或1ms
int subcarrier_interval;//子载波间隔15kHZ或7.5
double  samples_per_tti;//BW×1.288M
double sample_time_per_point;//取样时间  1/samples_per_tti
int N_RB_UL;//占用自在波数  6,13,25,50,75,100,110
int symbols_per_tti;//每时隙OFDM符号数 7,6,3
int ofdm_symbol_size;//FFT点数 256,512,1024,1536,2048,4096
int Ncp_UL;// CP长度 36或40
}UPLINK_PARAMETERS;
typedef struct
{
int Bandwidth;//占用带宽  单位（RB） <=系统带宽（BW）
}PUSCH_PARAMETERS;
typedef struct
{
int enable_flag;//信道使能 0或1
int srs_Bandwidth;// 占用带宽

}SRS_PARAMETERS;
typedef struct
{
int Bandwidth;//传输带宽3,5,10,15,20
float length_of_slot;//时隙长度0.5或1ms
int subcarrier_interval;//子载波间隔15kHZ或7.5
double samples_per_tti;//BW×1.288M
double sample_time_per_point;//取样时间  1/samples_per_tti
int N_RB_UL;//占用子载波数  6,13,25,50,75,100,110
int symbols_per_tti;//每时隙OFDM符号数 7,6,3
int ofdm_symbol_size;//FFT点数 256,512,1024,1536,2048,4096
int Ncp_UL;// CP长度 36或40
}DOWNLINK_PARAMETERS;
typedef struct
{
int Bandwidth;//PDSCH占用带宽
}PDSCH_PARAMETERS;

typedef struct
{
int Bandwidth;//PDCCH占用带宽
int enable_flag;//信道使能
}PDCCH_PARAMETERS;


typedef struct
{
int enable_flag;//PBCH信道使能
}PBCH_PARAMETERS;

struct PHY_VARS_RECV
{
 int *txdataf;
 int FFTBuffer[7168];
 int na;
 float fb;
SYSTEM_PARAMETERS system_paras;
UPLINK_PARAMETERS uplink_paras;
PUSCH_PARAMETERS pusch_paras;
SRS_PARAMETERS srs_paras;
DOWNLINK_PARAMETERS downlink_paras;
PDSCH_PARAMETERS pdsch_paras;
PDCCH_PARAMETERS pdcch_paras;
PBCH_PARAMETERS pbch_paras;
};

 struct PHY_VARS_SEND{
               int na;
               float fb;
               unsigned char N_RB_DL;
         };


         struct Header {
               char  magic_id[2];
               int length; // header + data
               char type;  // fftdata type = 10;
           };

         struct PHY_Data_Send  {
                  PHY_VARS_RECV buffer;
                  Header He;
                  char magic_id;
                  int cxh_test;//cxh
                  short chan_info[14][300][2];// 信道估计的值
                  unsigned char eNB_offset;  //  小区号
                  unsigned char Ns;  // 时隙
                  unsigned char antenna;  // 天线号
                  unsigned char ofdm1;  // 前时隙OFDM符号（0-6）
                  unsigned char ofdm2; // 后时隙OFDM符号（7-13）
                  int send_c;  //  发送的信道估计的包编号
                  int send_c2; //  发送的星座图的包编号
                  int send_c3; // 发送的FFT图的包编号
                  short cons_dia[14][300][2];  // 星座图的值
                  //double cons_dia[14][300][2];  // 星座图的值
                  short fft_data[1024]; // FFT数据
                  double snr_data[14]; // SNR数据
                  float upParameters[10]; // 从Qt到netbeans的上行参数
                  float downParameters[10]; // 从Qt到netbeans的下行参数
                  bool isUpParameter;
                  bool isDiagram;
                  char texts[1024];
                  char diagram[2048];
              };




#endif // STRUCT_TYPE_H
