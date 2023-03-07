#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial_TimerStart();
    drawpoints(X_Range,Y_Range);



}

MainWindow::~MainWindow()
{
    delete ui;
}

// ///界面切换部分程序

void MainWindow::on_serial_clicked() //串口界面
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->port_select->clear();
    scanSerialPort();
}


void MainWindow::on_temp_clicked() //温度界面
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_fluorescence_clicked() //荧光界面
{
    ui->stackedWidget->setCurrentIndex(2);
}

// //////温度图表部分程序
void MainWindow::drawpoints(int X_Ran,int Y_Ran) //图表创建
{
    X_Ran = X_Range;
    Y_Ran = Y_Range;
    //创建图表
    chart=new QChart();
    //创建坐标轴
    valueAxisX = new QValueAxis();
    valueAxisY = new QValueAxis();
    //设置坐标轴范围

    valueAxisX->setRange(0,X_Ran);
    valueAxisY->setRange(0,Y_Ran);

    //设置坐标轴标题和显示格式
    valueAxisX->setTitleText("Time(s)");
    valueAxisY->setTitleText("temp(C)");

    valueAxisX->setLabelFormat("%d");
    valueAxisY->setLabelFormat("%d");   //设定 x y为整数格式

    //图表添加坐标轴
    chart->addAxis(valueAxisX,Qt::AlignBottom);
    chart->addAxis(valueAxisY,Qt::AlignLeft);

    //设置图表标题和图例显示
    chart->setTitle("Temp chart");
    chart->legend()->setVisible(true);  //true 图例可见  false 不可见



    //曲线创建
    splineseries1 = new QSplineSeries();


    // arrx=getArrX();
    // arry1=getArrY();
    //arry2=getArrY();
    //for (int i = 0; i < arrx.size(); i++)
    //{
    //   splineseries1->append(arrx[i], arry1[i]);
    //   splineseries2->append(arrx[i], arry2[i]);

    // }


    //设置曲线颜色

    QPen pen(QColor("#ff0000"),2); //前面为颜色，后面为粗细
    splineseries1->setPen(pen);


    //图表添加曲线
    chart->addSeries(splineseries1);



    //曲线数据与坐标轴连接
    splineseries1->attachAxis(valueAxisX);
    splineseries1->attachAxis(valueAxisY);



    //将图表放置到图表视图中

    ui->chart_view->setChart(chart);  //！！！
    splineseries1->hide();

}


void MainWindow::drawCurve()
{
    splineseries1->append(x_k, now_Temp);
    x_k++;
    if(X_Range<=x_k)
    {
        X_Range+=50;
        valueAxisX->setRange(0,X_Range);

    }
}
// ///////  串口部分程序

void MainWindow::serial_TimerStart() //定时器及部分开机初始化
{

    serialPort = new QSerialPort(this);
    QTimer *timer2 = new QTimer(this);// 定时器2进行定时扫描串口读取数据
    timer2->setSingleShot(false);//设定为循环定时
    timer2->start(1000);//1s

    connect(timer2,SIGNAL(timeout()),this,SLOT(timeReadData()));
    ui->stackedWidget->setCurrentIndex(0);
    ui->Receive_msg->setReadOnly(true); //设置窗口只读不可编辑
    ui->Receive_msg->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);//设置读取到消息自动换行
    currentText = ui->Receive_msg->toPlainText();

}

void MainWindow::scanSerialPort()// //串口扫描函数
{

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->port_select->addItem(info.portName());
        //qDebug()<<info.portName();
    }

}

void MainWindow::SerialInit() //串口初始化（主要为数据位，停止位，奇偶校验位）
{

    //设置数据位 8位
    serialPort->setDataBits(QSerialPort::Data8);
    // 停止位 1
    serialPort->setStopBits(QSerialPort::OneStop);
    //设置奇偶校验 无
    serialPort->setParity(QSerialPort::NoParity);
}

void MainWindow::serialPortReadyRead() //串口数据读取
{
    buf = serialPort->readAll();
    //判断是否为非空，若为空则不读取
    if(!buf.isNull())
    {
        currentText.append(QString(buf)+"\n");
        ui->Receive_msg->setPlainText(currentText);
    }
}

void MainWindow::Rx_handle() //串口数据处理函数
{
    int i=0;
    int j=0;
    if(buf.size()>=18)
    {
        for(i=0;i<buf.size();i++)
        {
            if(buf.at(i)==0xFF)
            {
                if(buf.at(i+16)==0x0D&&buf.at(i+17)==0x0A)
                {
                    switch(buf.at(i+5))
                    {
                    case 0x41:Handle_Flag=1;break;
                    case 0x42:Handle_Flag=2;break;
                    default:Handle_Flag=0;break;
                    }
                    i=i+6;
                    for (j=0;j<8;j++)
                    {
                        Data_buf[j]=buf[i+j];
                    }
                    i+=7;
                }
                else
                    Handle_Flag=0;
            }
            else
            {
                Handle_Flag=0;
            }
            switch (Handle_Flag)
            {
            case 1:Temp_handle();break;

            case 2:Flourse_handle();break;

            default: break;
            }
        }
    }
    buf.clear();
}

void MainWindow::Temp_handle()//接受到的温度数据处理
{
    byte temp_arr[4];
    for(int i=0;i<4;i++)
    {
        temp_arr[i]=Data_buf[i];
    }

    float *pt = (float *)temp_arr;
    now_Temp=*pt;
    Data_buf.clear();
}

void MainWindow::SetTemp_handle(float temp)//设定的浮点温度转字节
{
    byte *sp = (byte *)&temp;
    Send_data_handle(TempSet_CMD,Temp_ID);
    Send_data_buf[6]=sp[0];
    Send_data_buf[7]=sp[1];
    Send_data_buf[8]=sp[2];
    Send_data_buf[9]=sp[3];

}

void MainWindow::Flourse_handle() //接收到的荧光数据处理
{


}

void MainWindow::Send_data_handle(byte CMD, byte Target_ID) //发送数据模板
{
    Send_data_buf.resize(18);
    for(int i=0;i<18;i++)
    {
        Send_data_buf[i]=0x00;
    }
    Send_data_buf[0]=0xFF;
    Send_data_buf[1]=0x12;
    Send_data_buf[2]=0x01;
    Send_data_buf[3]=CMD;
    Send_data_buf[4]=Target_ID;
    Send_data_buf[14]=0x0C;
    Send_data_buf[15]=0x0C;
    Send_data_buf[16]=0x0D;
    Send_data_buf[17]=0x0A;

}

void MainWindow::on_serial_link_clicked() //打开串口
{
    serialPort->setPortName(ui->port_select->currentText());//设置串口名
    serialPort->setBaudRate(ui->bound_select->currentText().toInt());//设置波特率
    SerialInit();
    if (serialPort->open(QIODevice::ReadWrite))
    {
        // 打开串口成功
        qDebug()<<ui->port_select->currentText()<<ui->bound_select->currentText()<<"Linked";
    } else {
        // 打开串口失败
        qDebug()<<"Serial connect false";
    }
}

void MainWindow::on_serial_disconnect_clicked() //关闭串口
{ 
    //清空串口接收缓冲区中的数据
    serialPort->clear();
    //关闭串口
    serialPort->close();
    qDebug()<<"Serial disconnect";
}


void MainWindow::on_receive_select_toggled(bool checked)
{
    if(checked)
    {
        qDebug()<<"Start Receive";
        ReceiveFlag=1;
    }

    else
    {
        ReceiveFlag=0;
        qDebug()<<"STOP Receive";
    }

}


void MainWindow::on_clear_receive_clicked()
{
    // 清除屏幕显示
    ui->Receive_msg->clear();
    //清除接收区信息
    currentText.clear();
}


void MainWindow::on_send_button_clicked()
{
    QDateTime dateTime = QDateTime::currentDateTime();//获取当前时间
    QString timeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");//设置时间格式
    QString data = ui->send_msg->text();//获取LineEdit框中的内容
    if(!data.isNull())//读取输入框是否为空，若为空则不发送
    {
        QByteArray sendData = data.toUtf8();//将LineEdit内容转为utf-8编码
        serialPort->write(sendData);//使用串口发送出去
        QString fullMessage = QString("[%1] %2\n").arg(timeStr).arg(data);//合并时间与发送内容
        currentText.append(fullMessage + "\n");
        ui->Receive_msg->setPlainText(currentText);//发送消息到接收栏
        ui->send_msg->clear();//清除输入框内容
    }
}


void MainWindow::timeReadData() // 定时器任务
{
    //开启后读取串口数据
    if(ReceiveFlag==1)
    {
        serialPortReadyRead();
    }
    //更新实时温度曲线
    drawCurve();

}

void MainWindow::on_temp_set_button_clicked() //设定温度确认按钮
{
    if(Temp_OF_Flag==1)
    {
        QString SettempData = ui->temp_set->text();
        if(!SettempData.isNull())
        {
            set_Temp=SettempData.toFloat();
            SetTemp_handle(set_Temp);
            serialPort->write(Send_data_buf);
            qDebug()<<"Set temp:"<<set_Temp<<"successful";
        }
        else
            qDebug()<<"you are not enter Temperature";
    }
    else
    {
        qDebug()<<"You are not open temp switch!!!";
    }
}


void MainWindow::on_temp_start_button_toggled(bool checked)//温度使能按钮
{
    if(checked)
    {
        Send_data_handle(TempOF_CMD,Temp_ID);
        Send_data_buf[6]=0x01;
        serialPort->write(Send_data_buf);
        Temp_OF_Flag=1;
        qDebug()<<"set temp ON";
    }
    else
    {
        Send_data_handle(TempOF_CMD,Temp_ID);
        //Send_data_buf[6]=0x00;
        serialPort->write(Send_data_buf);
        Temp_OF_Flag=0;
        qDebug()<<"set temp OFF";
    }
}


void MainWindow::on_temp_1_toggled(bool checked)
{
    if(checked)
    {
        splineseries1->show();
    }
    else
        splineseries1->hide();
}

