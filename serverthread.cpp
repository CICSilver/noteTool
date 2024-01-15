#include "serverthread.h"
#include <QUdpSocket>
#include <QDebug>
ServerThread::ServerThread(QObject *parent)
	: QObject(parent)
{
	m_worker = new UdpWorker();
	m_worker->moveToThread(&m_thread);
	connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
	connect(this, &ServerThread::Start, m_worker, &UdpWorker::doWork);
	/*connect(m_worker->m_pSocket, &QUdpSocket::readyRead, m_worker, &UdpWorker::handleReadyRead);*/
	connect(this, &ServerThread::Stop, m_worker, &UdpWorker::Stop);
	m_thread.start();
}

ServerThread::~ServerThread()
{
	emit Stop();
	m_thread.quit();
	m_thread.wait();
	if(m_worker)
	{
		delete m_worker;
		m_worker = nullptr;
	}
}

