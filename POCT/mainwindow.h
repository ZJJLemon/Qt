#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QComboBox>
#include<QDebug>
#include<QSerialPort>
#include<QList>
#include<QSerialPortInfo>
#include<QChart>
#include<QChartView>
#include<QValueAxis>
#include<QSplineSeries>
#include<QPen>
#include<QRandomGenerator>
#include<QDateTime>
#include<QTimer>
#include<QFile>
#include<QFileInfo>
#include<QFileDialog>
#include<QDir>
#include<QMutex>
#include<QTextEdit>
#include<QVector>


QT_CHARTS_USE_NAMESPACE
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef unsigned char byte;

#define Temp_ID 0x41    //温度模块ID
#define Flour_ID 0x43   //荧光模块ID
#define TempSet_CMD 0x03 //温度设定CMD
#define TempOF_CMD 0x01  //温度使能CMD

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void drawpoints(int X_Ran,int Y_Ran);
    void drawCurve();

    QByteArray buf;//串口接收数据
    QByteArray Data_buf; //一帧数据中的8位数据位
    QByteArray Send_data_buf;//发送的一帧数据

    QString currentText;
    int x_k=0;
    int X_Range=50;
    int Y_Range=100;
    int ReceiveFlag=0;
    int Handle_Flag=0;
    int Temp_OF_Flag=0;
    float now_Temp=0;
    float set_Temp=0;

    void scanSerialPort();  //串口扫描
    void SerialInit(); //数据等位置初始化
    void serialPortReadyRead();// 数据读取
    void serial_TimerStart();
    void Rx_handle();
    void Send_data_handle(byte CMD,byte Target_ID);

    void Temp_handle();
    void SetTemp_handle(float temp);



    void Flourse_handle();


private slots:
    void timeReadData();

    void on_serial_clicked();

    void on_temp_clicked();

    void on_fluorescence_clicked();

    void on_serial_link_clicked();

    void on_serial_disconnect_clicked();

    void on_receive_select_toggled(bool checked);

    void on_clear_receive_clicked();

    void on_send_button_clicked();

    void on_temp_set_button_clicked();

    void on_temp_start_button_toggled(bool checked);

    void on_temp_1_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    QChart *chart;
    QSerialPort *serialPort;
    QSplineSeries *splineseries1;
    QValueAxis *valueAxisX;
    QValueAxis *valueAxisY;

    QList<int> arrx;
    QList<float> arry1;


};
#endif // MAINWINDOW_H
