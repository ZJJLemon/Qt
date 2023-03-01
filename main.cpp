#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    qInstallMessageHandler(MainWindow::outputMessage);//注册为QT消息处理函数
    MainWindow w;
    w.show();
    return a.exec();


}
