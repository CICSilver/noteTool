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

TranslationModel dao::TranslationDao::GetTranslationDaoByWordId(int word_id)
{
	QString sql = dbtable::translation::word_id + "=" + word_id;
	QSqlQuery query = helper->Where(dbtable::translation::tableName, sql);
	while (query.next())
	{
		return TranslationModel(query);
	}
	return TranslationModel();
}

TranslationModel dao::TranslationDao::GetTranslationDaoById(int id)
{
	QString sql = dbtable::translation::id + "=" + id;
	QSqlQuery query = helper->Where(dbtable::translation::tableName, sql);
	while (query.next())
	{
		return TranslationModel(query);
	}
	return TranslationModel();
}
