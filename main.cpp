#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include "datamodel.h"
#include "SqlHelper.h"
#ifdef _DEBUG
#include <Windows.h>
#include <QMetaObject>
#include <QDebug>
#include <iostream>
#include "WordDao.h"
#include "SearchApi.h"
#include "serverthread.h"
#include "websocketserver.h"
#include "searchwindow.h"
#endif // _DEBUG

#include <qdebug.h>
int main(int argc, char* argv[])
{
	QTextCodec* codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForLocale(codec);
	// 初始化数据库
	auto helper = SqlHelper::Instance();
	helper->Init();
	qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString();
#ifdef _DEBUG
	SetConsoleOutputCP(CP_UTF8);
#endif // _DEBUG
	QApplication a(argc, argv);
	//MainWindow w;
	//w.show();

	SearchWindow w;
	//w.show();
	/*WebsocketServer server;
	server.Start();*/

	//SearchApi* api = SearchApi::Instance();
	//api->FetchWord("test");
	return a.exec();
}
