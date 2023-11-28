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

void SqlHelper::Execute(QString sql, QString connName)
{
	QSqlDatabase db = QSqlDatabase::database(connName);
	qDebug() << db.isOpen();
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
}
