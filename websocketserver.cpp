#include "websocketserver.h"

WebsocketServer::WebsocketServer(QObject *parent)
	: QObject(parent)
{
	m_pWorker = new WebsocketWorker();
	m_pWorker->moveToThread(&m_workThread);
	connect(&m_workThread, &QThread::finished, m_pWorker, &QObject::deleteLater);
	connect(this, &WebsocketServer::Start, m_pWorker, &WebsocketWorker::doWork);
	m_workThread.start();
}

WebsocketServer::~WebsocketServer()
{
	m_workThread.quit();
	m_workThread.wait();
	if (m_pWorker)
	{
		delete m_pWorker;
		m_pWorker = nullptr;
	}
}
