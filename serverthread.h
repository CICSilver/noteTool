#pragma once

#include <QThread>
#include <QUdpSocket>
class UdpWorker : public QObject
{
	Q_OBJECT
public:
	UdpWorker()
	{
		m_pSocket = nullptr;
		m_bStop = false;
	}
	~UdpWorker()
	{
		if (m_pSocket)
		{
			delete m_pSocket;
			m_pSocket = nullptr;
		}
		qDebug() << "~UdpWorker";
	}
	void doWork()
	{
		qDebug() << "serverThread" << QThread::currentThread();
		m_pSocket = new QUdpSocket();
		if (!m_pSocket->bind(QHostAddress::Any, 58585)) {
			// 处理错误
			qDebug() << "[Capture Server] bind error";
			return;
		}
		connect(m_pSocket, &QUdpSocket::readyRead, this, &UdpWorker::handleReadyRead);
		qDebug() << "[Capture Server] listen success, server running....";
	}
public slots:
	void handleReadyRead()
	{
		QUdpSocket *socket = qobject_cast<QUdpSocket *>(sender());
		qDebug() << "handleReadyRead" << QThread::currentThread();
		while (socket->hasPendingDatagrams()) {
			QByteArray datagram;
			datagram.resize(socket->pendingDatagramSize());
			QHostAddress sender;
			quint16 senderPort;

			socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

			// 在这里处理数据
			// 例如，你可以打印数据，或者将数据保存到文件
			qDebug() << "Received datagram: " << datagram;
		}
	}

	void Stop()
	{
		m_bStop = true;
	}

public:
	QUdpSocket* m_pSocket;
	bool m_bStop;
};


class ServerThread  : public QObject
{
	Q_OBJECT

public:
	ServerThread(QObject *parent = nullptr);
	~ServerThread();

signals:
	void Start();
	void Stop();
public:


private:
	UdpWorker* m_worker;
	QThread m_thread;
};
