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

QT_CHARTS_USE_NAMESPACE
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString currentText;
    void scanSerialPort();  //串口扫描
    void SerialInit(); //数据等位置初始化
    void serialPortReadyRead();// 数据读取



    void drawpoints();
    QList<int> getArrY();
    QList<int> getArrX();
    int ReceiveFlag=0;
private:
    Ui::MainWindow *ui;

    QChart *chart;
    QSerialPort *serialPort;
    QSplineSeries *splineseries1;
    QSplineSeries *splineseries2;
    QSplineSeries *splineseries3;
    QSplineSeries *splineseries4;
    QSplineSeries *splineseries5;
    QSplineSeries *splineseries6;
    QValueAxis *valueAxisX;
    QValueAxis *valueAxisY;
    QList<int> arrx;
    QList<int> arry1;
    QList<int> arry2;
    QList<int> arry3;
    QList<int> arry4;
    QList<int> arry5;
    QList<int> arry6;

    QList<QString> serialList;



private slots:
    void timeReadData();
    void on_chart_clicked();
    void on_serial_clicked();
    void on_Data_clicked();
    void on_BackHome_clicked();
    void on_serialSelect_activated(int index);
    void on_c1_toggled(bool checked);
    void on_c2_toggled(bool checked);
    void on_c3_toggled(bool checked);
    void on_c4_toggled(bool checked);
    void on_c5_toggled(bool checked);
    void on_c6_toggled(bool checked);
    void on_Link_clicked();
    void on_CutLink_clicked();
    void on_TextClear_clicked();
    void on_TextReceive_toggled(bool checked);
    void on_SendText_clicked();
};
#endif // MAINWINDOW_H
