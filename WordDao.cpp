#include "WordDao.h"

QList<WordModel> dao::WordDao::GetAllWord()
{
	QList<WordModel> wordList;
	QSqlQuery query = helper->Where(dbtable::word::tableName, "1=1");
	while (query.next())
	{
		WordModel model;
		model.SetDataId(query.value(dbtable::word::data_id).toInt());
		model.SetWord(query.value(dbtable::word::word).toString());
		wordList.append(model);
	}
	return wordList;
}

QList<WordModel> dao::WordDao::GetWordModelByDataId(int data_id)
{
	QList<WordModel> modelList;
	QSqlQuery query = helper->Where(dbtable::word::tableName, QString("data_id=%1").arg(data_id));
	while(query.next())
	{
		WordModel model(query);
		modelList.append(model);
	}
	return modelList;
}

WordModel dao::WordDao::GetWordModelById(int id)
{
	WordModel model;
	QSqlQuery query = helper->Where(dbtable::word::tableName, QString("id=%1").arg(id));
	while (query.next())
	{
		model.SetWord(query.value(dbtable::word::word).toString());
		model.SetDataId(query.value(dbtable::word::data_id).toInt());
	}
	return model;
}

void dao::WordDao::DeleteByDataId(int data_id)
{
	helper->Delete(dbtable::word::tableName, QString("data_id=%1").arg(data_id));
	//helper->CloseDB();
}

void dao::WordDao::DeleteByWord(QString word)
{
	helper->Delete(dbtable::word::tableName, QString("word='%1'").arg(word));
}
