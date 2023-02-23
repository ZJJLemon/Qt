#include "drawpoint.h"

drawPoint::drawPoint(QWidget *parent)
    : QMainWindow{parent}
{
    //创建图表
    QChart *chart=new QChart();
    //创建坐标轴
    QValueAxis* valueAxisX = new QValueAxis();
    QValueAxis* valueAxisY = new QValueAxis();
    //设置坐标轴范围

    valueAxisX->setRange(0,100);
    valueAxisY->setRange(0,300);

    //设置坐标轴标题和显示格式
    valueAxisX->setTitleText("Time(s)");
    valueAxisY->setTitleText("temp(℃)");

    valueAxisX->setLabelFormat("%d");
    valueAxisY->setLabelFormat("%d");   //设定 x y为整数格式

    //图表添加坐标轴
    chart->addAxis(valueAxisX,Qt::AlignBottom);
    chart->addAxis(valueAxisY,Qt::AlignLeft);

    //设置图表标题和图例显示
    chart->setTitle("Test chart");
    chart->legend()->setVisible(true);  //true 图例可见  false 不可见

    //曲线创建
    QSplineSeries *splineseries = new QSplineSeries();

    splineseries->append(0,20);
    splineseries->append(20,50);
    splineseries->append(40,80);
    splineseries->append(60,100);
    splineseries->append(80,60);
    splineseries->append(100,40);

    //设置曲线颜色
    QPen pen(QColor("#ff0000"));
    splineseries->setPen(pen);

    //图表添加曲线
    chart->addSeries(splineseries);

    //曲线数据与坐标轴连接
    splineseries->attachAxis(valueAxisX);
    splineseries->attachAxis(valueAxisY);

    //将图表放置到图表视图中
    //ui.chartview.setchart(chart);



}
