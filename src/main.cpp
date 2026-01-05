#include "mainwindow.h"
#include <QApplication>
//#include <QDebug>
#include <QFile>
//#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/style/style.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("open qss success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("Open failed");
    }

    MainWindow w;
    w.showMaximized();
//    qDebug() << w.width()<<"   "<<w.height();

    // 获取主屏幕的可用几何区域（不包括任务栏等系统区域）
//    QRect availableGeometry = QApplication::primaryScreen()->availableGeometry();
//    int screenWidth = availableGeometry.width();
//    int screenHeight = availableGeometry.height();
//    qDebug() << "屏幕可用区域:" << screenWidth << "x" << screenHeight;

    return a.exec();
}
