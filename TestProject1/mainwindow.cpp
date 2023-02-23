#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer2 = new QTimer(this);// 定时器2进行定时扫描串口读取数据
    timer2->setSingleShot(false);//设定为循环定时
    timer2->start(500); // 1000ms
    connect(timer2, SIGNAL(timeout()), this, SLOT(timeReadData()));

    serialPort = new QSerialPort(this);

    ui->stackedWidget->setCurrentIndex(0);

    ui->ReceiveView->setReadOnly(true); //设置窗口只读不可编辑
    ui->ReceiveView->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);//设置读取到消息自动换行
    currentText = ui->ReceiveView->toPlainText();

    drawpoints();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scanSerialPort()// //串口扫描函数
{

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->serialSelect->addItem(info.portName());
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
    QByteArray buf = serialPort->readAll();
    if(!buf.isNull())//判断是否为非空，若为空则不读取
    {
        currentText.append(QString(buf)+"\n");
        ui->ReceiveView->setPlainText(currentText);
    }

}

void MainWindow::getFileList() // 选择文件夹，获取文件名显示列表
{

    QString mFolderPath = QFileDialog::getExistingDirectory(NULL, "Open Folder", "D:/QT");
    if (mFolderPath.isEmpty()) return;
    QDir dir(mFolderPath);
    //设置过滤器
    dir.setFilter(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);//QDir::NoDotAndDotDot - 不显示系统中的特殊文件,例如"."和".."QDir::Files - 显示文件
    // QDir::Dirs 显示文件夹

    // 获取所有文件名到列表中
    QStringList fileNameList = dir.entryList();

    for(int i=0;i<fileNameList.count();i++)
    {

        ui->DataListView->addItem(fileNameList[i]);

    }


}


void MainWindow::drawpoints() //图表创建
{
    //创建图表
    chart=new QChart();
    //创建坐标轴
    valueAxisX = new QValueAxis();
    valueAxisY = new QValueAxis();
    //设置坐标轴范围

    valueAxisX->setRange(0,50);
    valueAxisY->setRange(0,300);

    //设置坐标轴标题和显示格式
    valueAxisX->setTitleText("Time(s)");
    valueAxisY->setTitleText("temp(C)");

    valueAxisX->setLabelFormat("%d");
    valueAxisY->setLabelFormat("%d");   //设定 x y为整数格式

    //图表添加坐标轴
    chart->addAxis(valueAxisX,Qt::AlignBottom);
    chart->addAxis(valueAxisY,Qt::AlignLeft);

    //设置图表标题和图例显示
    chart->setTitle("Test chart");
    chart->legend()->setVisible(true);  //true 图例可见  false 不可见



    //曲线创建
    splineseries1 = new QSplineSeries();
    splineseries2 = new QSplineSeries();
    splineseries3 = new QSplineSeries();
    splineseries4 = new QSplineSeries();
    splineseries5 = new QSplineSeries();
    splineseries6 = new QSplineSeries();

    arrx=getArrX();
    arry1=getArrY();
    arry2=getArrY();
    arry3=getArrY();
    arry4=getArrY();
    arry5=getArrY();
    arry6=getArrY();
    for (int i = 0; i < arrx.size(); i++)
    {
        splineseries1->append(arrx[i], arry1[i]);
        splineseries2->append(arrx[i], arry2[i]);
        splineseries3->append(arrx[i], arry3[i]);
        splineseries4->append(arrx[i], arry4[i]);
        splineseries5->append(arrx[i], arry5[i]);
        splineseries6->append(arrx[i], arry6[i]);
    }


    //设置曲线颜色

    QPen pen(QColor("#ff0000"),2); //前面为颜色，后面为粗细
    splineseries1->setPen(pen);

    pen.setColor("#000000");
    splineseries2->setPen(pen);

    pen.setColor("#00ff00");
    splineseries3->setPen(pen);


    pen.setColor("#ffff7f");
    splineseries4->setPen(pen);

    pen.setColor("#0000ff");
    splineseries5->setPen(pen);

    pen.setColor("#ff00ff");
    splineseries6->setPen(pen);

    //图表添加曲线
    chart->addSeries(splineseries1);
    chart->addSeries(splineseries2);
    chart->addSeries(splineseries3);
    chart->addSeries(splineseries4);
    chart->addSeries(splineseries5);
    chart->addSeries(splineseries6);

    //曲线数据与坐标轴连接
    splineseries1->attachAxis(valueAxisX);
    splineseries1->attachAxis(valueAxisY);
    splineseries2->attachAxis(valueAxisX);
    splineseries2->attachAxis(valueAxisY);
    splineseries3->attachAxis(valueAxisX);
    splineseries3->attachAxis(valueAxisY);
    splineseries4->attachAxis(valueAxisX);
    splineseries4->attachAxis(valueAxisY);
    splineseries5->attachAxis(valueAxisX);
    splineseries5->attachAxis(valueAxisY);
    splineseries6->attachAxis(valueAxisX);
    splineseries6->attachAxis(valueAxisY);

    //将图表放置到图表视图中
    // ui->chart_view->setChart(chart);
    ui->chart_view->setChart(chart);
    splineseries1->hide();
    splineseries2->hide();
    splineseries3->hide();
    splineseries4->hide();
    splineseries5->hide();
    splineseries6->hide();

}

QList<int> MainWindow::getArrY()//获取随机数列表Y
{
    QList<int> ArrY;

    for(int j=0;j<45;j++)
    {
        ArrY.append(QRandomGenerator::global()->bounded(0,250));//0-250之间取随机数
    }

    return ArrY;
}

QList<int> MainWindow::getArrX()//生成一个有序列表X
{
    QList<int> ArrX;
    //生成0-44排序好的列表X
    for(int i=0;i<45;i++)
    {
        ArrX.append(i);
    }
    return ArrX;
}

void MainWindow::timeReadData() // 定时接收串口数据
{
    if(ReceiveFlag==1)
    {
        serialPortReadyRead();
    }

}


//页面切换   //////
void MainWindow::on_chart_clicked() //切换到图表展示页面
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_serial_clicked() //切换到串口通信页面
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->serialSelect->clear();
    scanSerialPort();
}


void MainWindow::on_Data_clicked() //文件展示页面
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_BackHome_clicked() //返回主界面
{
    ui->stackedWidget->setCurrentIndex(0);
}
// /////////////////

void MainWindow::on_serialSelect_activated(int index) //串口端口选择槽函数
{

    qDebug()<<ui->serialSelect->currentText()<<" Selected";

}


void MainWindow::on_c1_toggled(bool checked) //图表1显示与隐藏
{
    if(checked)
    {
        splineseries1->show();
    }
    else
    {
        splineseries1->hide();
    }

}


void MainWindow::on_c2_toggled(bool checked)//图表2显示与隐藏
{
    if(checked)
    {
        splineseries2->show();
    }
    else
    {
        splineseries2->hide();
    }

}


void MainWindow::on_c3_toggled(bool checked)//图表3显示与隐藏
{
    if(checked)
    {
        splineseries3->show();
    }
    else
    {
        splineseries3->hide();
    }

}


void MainWindow::on_c4_toggled(bool checked)//图表4显示与隐藏
{
    if(checked)
    {
        splineseries4->show();
    }
    else
    {
        splineseries4->hide();
    }

}


void MainWindow::on_c5_toggled(bool checked)//图表5显示与隐藏
{
    if(checked)
    {
        splineseries5->show();
    }
    else
    {
        splineseries5->hide();
    }

}


void MainWindow::on_c6_toggled(bool checked)//图表6显示与隐藏
{
    if(checked)
    {
        splineseries6->show();
    }
    else
    {
        splineseries6->hide();
    }

}


void MainWindow::on_Link_clicked() //串口连接按钮操作
{
    serialPort->setPortName(ui->serialSelect->currentText());//设置串口名
    serialPort->setBaudRate(ui->baudSelect->currentText().toInt());//设置波特率
    SerialInit();
    if (serialPort->open(QIODevice::ReadWrite))
    {
        // 打开串口成功
        qDebug()<<ui->serialSelect->currentText()<<ui->baudSelect->currentText()<<"Linked";
    } else {
        // 打开串口失败
        qDebug()<<"Serial connect false";
    }
}


void MainWindow::on_CutLink_clicked() //串口断开连接按钮操作
{
    //关闭串口
    serialPort->clear(); //清空串口接收缓冲区中的数据
    serialPort->close();//关闭串口
    //serialPort->deleteLater();
    qDebug()<<"Serial disconnect";
}


void MainWindow::on_TextClear_clicked()//清空接收栏数据
{
    ui->ReceiveView->clear();
}




void MainWindow::on_TextReceive_toggled(bool checked) //数据接收按钮状态
{
    if(checked)
    {
        qDebug()<<"Start Receive";
        //serialPortReadyRead();
        ReceiveFlag=1;
    }

    else
    {
        ReceiveFlag=0;
        qDebug()<<"STOP Receive";
    }


}



void MainWindow::on_SendText_clicked() //串口发送按钮
{
    QDateTime dateTime = QDateTime::currentDateTime();//获取当前时间
    QString timeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");//设置时间格式
    QString data = ui->SendView->text();//获取LineEdit框中的内容
    if(!data.isNull())//读取输入框是否为空，若为空则不发送
    {

        QByteArray sendData = data.toUtf8();//将LineEdit内容转为utf-8编码
        serialPort->write(sendData);//使用串口发送出去
        QString fullMessage = QString("[%1] %2\n").arg(timeStr).arg(data);//合并时间与发送内容
        currentText.append(fullMessage + "\n");
        ui->ReceiveView->setPlainText(currentText);//发送消息到接收栏
        ui->SendView->clear();//清除输入框内容
    }


}


void MainWindow::on_openFile_clicked() //打开文件夹读取文件信息
{
    getFileList();
}


void MainWindow::on_clearFile_clicked() //清空显示列表
{
    ui->DataListView->clear();
}

