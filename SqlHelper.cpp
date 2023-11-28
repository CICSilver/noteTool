#include "SqlHelper.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
void SqlHelper::RunSqlScript(QString scriptPath, QString connName)
{
	QFile scriptFile(scriptPath);
	if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Could not open SQL script file: " << scriptPath;
		return;
	}
	QTextStream in(&scriptFile);
	QString sql = in.readAll();
	scriptFile.close();
	QStringList sqlStatements = sql.split(";", QString::SkipEmptyParts);
	for (const QString& statement : sqlStatements)
	{
		if (!statement.trimmed().isEmpty())
		{
			Execute(statement, connName);
		}
	}
}

void SqlHelper::AddDatabase(QString connName)
{
}

void SqlHelper::Init()
{
	QString sql = "select name from sqlite_master where type='table' and name='word'";
	if (m_connName.isEmpty())
	{
		qDebug() << "�޴򿪵����ݿ����ӣ�";
	}
	QString connName = m_connName.at(0);
	QSqlDatabase db = QSqlDatabase::database(connName);
	auto query = Execute(sql, connName);
	if (query.next())
	{
		// δ��ѯ������Ҫ��ʼ��
		// ��ɾ�����б�ṹ��Ȼ�����²���
		return;
	}
}

QSqlQuery SqlHelper::Execute(QString sql, QString connName)
{
	QSqlDatabase db = QSqlDatabase::database(connName);
	QSqlQuery query(db);
	if (!db.isValid() || !db.open())
	{
		qDebug() << "Insert failed. beacuse: " << db.lastError();
	}
	if (!query.exec(sql))
	{
		qDebug() << "Failed to execute SQL statement: " << sql << ", error: " << query.lastError();
	}
	db.close();
	return query;
}
