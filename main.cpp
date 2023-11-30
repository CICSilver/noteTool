#include "notetool.h"
#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#ifdef _DEBUG
#include <Windows.h>
#include "datamodel.h"
#include "SqlHelper.h"
#include <QMetaObject>
#include <QDebug>
#endif // _DEBUG

#include <qdebug.h>
int main(int argc, char* argv[])
{
	QTextCodec* codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForLocale(codec);
	// 初始化数据库
	auto helper = SqlHelper::Instance();
	helper->Init();
	//DataModel dataModel;
	//dataModel.SetDate("2020-1-1");
	////dataModel.SetId()
	//helper->Insert<DataModel>("Data", dataModel);
#ifdef _DEBUG
	SetConsoleOutputCP(CP_UTF8);
#endif // _DEBUG

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
