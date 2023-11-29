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
int main(int argc, char* argv[])
{
	//QTextCodec* codec = QTextCodec::codecForName("GBK");
	//QTextCodec::setCodecForLocale(codec)
	// 初始化数据库
	auto helper = SqlHelper::Instance();
	helper->Init();
#ifdef _DEBUG
#endif // _DEBUG

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
