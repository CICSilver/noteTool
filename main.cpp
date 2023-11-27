#include "notetool.h"
#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include <qdebug.h>
int main(int argc, char *argv[])
{
    //QTextCodec* codec = QTextCodec::codecForName("GBK");
    //QTextCodec::setCodecForLocale(codec);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
