#include "DataDao.h"
QList<DataModel> dao::DataDao::GetAllData()
{
	QList<DataModel> modelList;
	QSqlQuery query = helper->Where(dbtable::data::tableName, "1=1");
	while (query.next())
	{
		DataModel model;
		model.SetId(query.value(dbtable::data::id).toInt());
		model.SetDate(query.value(dbtable::data::date).toString());
		modelList.append(model);
	}
	helper->CloseDB();
	return modelList;
}

DataModel dao::DataDao::GetDataModelById(int id)
{
	DataModel model;
	QSqlQuery query = helper->Where(dbtable::data::tableName, dbtable::data::id + "=" + QString::number(id));
	while (query.next())
	{
		model.SetId(query.value(dbtable::data::id).toInt());
		model.SetDate(query.value(dbtable::data::date).toString());
	}
	helper->CloseDB();
	return model;
}

DataModel dao::DataDao::GetDataModelByDate(QString date)
{
	DataModel model;
	QSqlQuery query = helper->Where(dbtable::data::tableName, dbtable::data::date + "='" + date + "'");
	while (query.next())
	{
		model.SetId(query.value(dbtable::data::id).toInt());
		model.SetDate(query.value(dbtable::data::date).toString());
	}
	return model;
}
