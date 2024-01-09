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

WordModel dao::WordDao::GetWordModelByDataId(int data_id)
{
	WordModel model;
	QSqlQuery query = helper->Where(dbtable::word::tableName, QString("data_id=%1").arg(data_id));
	while(query.next())
	{
		model.SetWord(query.value(dbtable::word::word).toString());
		model.SetDataId(query.value(dbtable::word::data_id).toInt());
	}
	return model;
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
