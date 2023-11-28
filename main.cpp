#include "notetool.h"
#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#ifdef _DEBUG
#include "datamodel.h"
#include "SqlHelper.h"
#include <QMetaObject>
#include <QDebug>
#endif // _DEBUG

#include <qdebug.h>
int main(int argc, char *argv[])
{
    //QTextCodec* codec = QTextCodec::codecForName("GBK");
    //QTextCodec::setCodecForLocale(codec);
#ifdef _DEBUG
    auto helper = SqlHelper::Instance();
    helper->RunSqlScript();
    //DataModel model;
    //model.SetDate("2023-01-01");
    //model.SetId(1);
    //helper->Insert<DataModel>("Word", model);
#endif // _DEBUG

    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    return a.exec();
}
