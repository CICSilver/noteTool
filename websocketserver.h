#pragma once

#include <QObject>
#include <QThread>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>
class WebsocketWorker : public QObject
{
	Q_OBJECT

public:
	WebsocketWorker(QObject *parent = nullptr)
	{
		m_webSocketServer = new QWebSocketServer(
			QStringLiteral("My WebSocket Server"),
			QWebSocketServer::NonSecureMode, 
			this);
		//connect(&m_timer, &QTimer::timeout, this, &WebsocketWorker::CheckConnection);
	}
	~WebsocketWorker()
	{
		m_webSocketServer->deleteLater();
	}

	void InitWebSocketServer()
	{
		m_webSocketServer = new QWebSocketServer(QStringLiteral("My WebSocket Server"), QWebSocketServer::NonSecureMode, this);

		if (m_webSocketServer->listen(QHostAddress::Any, 58585)) 
		{
			connect(m_webSocketServer, &QWebSocketServer::newConnection, this, &WebsocketWorker::onNewConnection);
			qDebug() << "[Capture Server] started on port 58585.";
		}
		else 
		{
			qDebug() << "[Capture Server] start failed.";
		}
	}
signals:
	void wordCapturedSignal(QString word);
public slots:
	void doWork()
	{
		InitWebSocketServer();
		//m_timer.start(30000);	// 30秒检查一次连接状态
	}

	// 检查存储的连接状态,删除已经断开的连接
	void CheckConnection()
	{
		for (auto& socket : m_clients)
		{
			if (socket->state() != QAbstractSocket::ConnectedState)
			{
				m_clients.removeAll(socket);
				socket->deleteLater();
			}
		}
	}
	void onNewConnection()
	{
		QWebSocket* socket = m_webSocketServer->nextPendingConnection();
		qDebug() << "[Capture Server] new Connection...";
		connect(socket, &QWebSocket::textMessageReceived, this, &WebsocketWorker::processTextMessage);
		connect(socket, &QWebSocket::disconnected, this, &WebsocketWorker::socketDisconnected);
		m_clients << socket;
	}

	void processTextMessage(QString message)
	{
		QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
		qDebug() << "[Capture Server] receive new message:" << message;
		// 去除包含的空格
		message = message.trimmed();
		if (!message.isEmpty())
		{
			emit wordCapturedSignal(message);
			if (socket) {
				socket->sendTextMessage(QString("capture sucess, capture word:%1").arg(message));
			}
		}
	}
	void socketDisconnected()
	{
		QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
		if (socket) {
			m_clients.removeAll(socket);
			socket->deleteLater();
		}
	}

private:
	QWebSocketServer* m_webSocketServer;
	QList<QWebSocket*> m_clients;
	QTimer m_timer;
};

class WebsocketServer  : public QObject
{
	Q_OBJECT

public:
	WebsocketServer(QObject *parent = nullptr);
	~WebsocketServer();
signals:
	void Start();
	void wordCapturedSignal(QString word);
private:
	QThread m_workThread;
	WebsocketWorker* m_pWorker;
};
