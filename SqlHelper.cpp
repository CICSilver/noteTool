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

QList<DataModel> SqlHelper::GetDataRecords(QString connName)
{
	QList<DataModel> modelList;
	QSqlQuery query = Where(tableName::Data, "1=1");
	while (query.next())
	{
		DataModel model;
		model.SetId(query.value(table_data::id).toInt());
		model.SetDate(query.value(table_data::date).toString());
		modelList.append(model);
	}
	QSqlDatabase::database(connName).close();
	return modelList;
}

QList<WordModel> SqlHelper::GetWordRecords(int data_id, QString connName)
{
	QList<WordModel> modelList;
	QString _where = data_id == -1 ? "1=1" : "data_id=" + data_id;
	QSqlQuery query = Where(tableName::Word, _where);
	while (query.next())
	{
		WordModel model;
		model.SetWord(query.value(table_word::word).toString());
		model.SetTranslation(query.value(table_word::translation).toString());
		model.SetRoot(query.value(table_word::root).toString());
		model.SetSentence(query.value(table_word::sentence).toString());
		modelList.append(model);
	}
	return modelList;
}

void SqlHelper::Init()
{
	QString sql = "select name from sqlite_master where type='table' and name='Word'";
	if (m_connName.isEmpty())
	{
		qDebug() << "无打开的数据库连接！";
	}
	QString connName = m_connName.at(0);
	QSqlDatabase db = QSqlDatabase::database(connName);
	auto query = Execute(sql, connName);
	if (!query.next())
	{
		// 未查询到表，需要初始化
		RunSqlScript("init.sql");
		qDebug() << "Init database";
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
		QString errMsg = query.lastError().text();
		if (errMsg.contains("UNIQUE"))
		{
			// TODO: 处理重复插入错误
			DepulicateError();
		}
	}
	return query;
}
