#pragma once

#include <QObject>
#include <QThread>
#include <QWebSocketServer>
#include <QWebSocket>
#include "SearchApi.h"
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
		m_api = SearchApi::Instance();
	}
	~WebsocketWorker()
	{
		if(m_webSocketServer)
		{
			delete m_webSocketServer;
			m_webSocketServer = nullptr;
		}
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

public slots:
	void doWork()
	{
		InitWebSocketServer();
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
		m_api->FetchWord(message);
		if (socket) {
			socket->sendTextMessage(message);
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
	SearchApi* m_api;
};

class WebsocketServer  : public QObject
{
	Q_OBJECT

public:
	WebsocketServer(QObject *parent = nullptr);
	~WebsocketServer();

signals:
	void Start();
private:
	QThread m_workThread;
	WebsocketWorker* m_pWorker;
};
