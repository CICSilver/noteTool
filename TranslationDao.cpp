#include "TranslationDao.h"

QList<TranslationModel> dao::TranslationDao::GetAllTranslation()
{
	QList<TranslationModel> modelList;
	QSqlQuery query = helper->Where(dbtable::translation::tableName, "1=1");
	while (query.next())
	{
		modelList.append(TranslationModel(query));
	}
	return modelList;
}

QList<TranslationModel> dao::TranslationDao::GetTranslationModelByWordId(int word_id)
{
	QList<TranslationModel> list;
	QString sql = QString("%1=%2").arg(dbtable::translation::word_id).arg(word_id);
	QSqlQuery query = helper->Where(dbtable::translation::tableName, sql);
	while (query.next())
	{
		list.append(TranslationModel(query));
	}
	return list;
}

TranslationModel dao::TranslationDao::GetTranslationModelById(int id)
{
	QString sql = dbtable::translation::id + "=" + id;
	QSqlQuery query = helper->Where(dbtable::translation::tableName, sql);
	while (query.next())
	{
		return TranslationModel(query);
	}
	return TranslationModel();
}

void dao::TranslationDao::UpdateWithId(int id, TranslationModel model)
{
	QString where = QString("%1=%2").arg(dbtable::translation::id).arg(id);
	helper->Update<TranslationModel>(dbtable::translation::tableName, where, model);
}

int dao::TranslationDao::Exists(int word_id, int sub_id)
{
	int id = -1;
	QString sql = QString("%1=%2 and %3='%4'")
		.arg(dbtable::translation::word_id)
		.arg(word_id)
		.arg(dbtable::translation::sub_id)
		.arg(sub_id);
	QSqlQuery query = helper->Where(dbtable::translation::tableName, sql);
	if (query.next())
		id = query.value(dbtable::translation::id).toInt();
	return id;
}
