#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include "datamodel.h"
#include "SqlHelper.h"
#include <Windows.h>
#include <QMetaObject>
#include <QDebug>
#ifdef _DEBUG
#include <iostream>
#include "WordDao.h"
#include "SearchApi.h"
#include "serverthread.h"
#include "websocketserver.h"
#include "searchwindow.h"
#include "SoloWindow.h"
#endif // _DEBUG

#include <qdebug.h>
int main(int argc, char* argv[])
{
#ifndef _DEBUG
	//QCoreApplication::addLibraryPath("./sqldrivers");
	//qDebug() << QSqlDatabase::drivers();

	// 确保唯一性
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"noteTool");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(hMutex);
		return 1;
	}
#endif

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
	SoloWindow w;
	w.show();

	return a.exec();
}
