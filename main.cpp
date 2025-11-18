#include "mainwindow.h"

#include <QApplication>
#include <QQmlEngine>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int ret = a.exec();
    qDebug() << "Event loop exited with code:" << ret;
    return ret;
}
