#ifndef DRAWPOINT_H
#define DRAWPOINT_H

#include <QMainWindow>
#include<QChart>
#include<QChartView>
#include<QValueAxis>
#include<QSplineSeries>
#include<QPen>

QT_CHARTS_USE_NAMESPACE

class drawPoint : public QMainWindow
{
    Q_OBJECT
public:
    explicit drawPoint(QWidget *parent = nullptr);

signals:

};

#endif // DRAWPOINT_H
