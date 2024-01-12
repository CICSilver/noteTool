#pragma once
#include "SqlHelper.h"
#include "model/datamodel.h"
namespace dao
{
	class DataDao
	{
	public:
		static DataDao* Instance()
		{
			static DataDao instance;
			return &instance;
		}
		QList<DataModel> GetAllData();
		DataModel GetDataModelById(int id);
		DataModel GetDataModelByDate(QString date);

		void DeleteByDate(QString date);
		void DeleteById(int id);

	private:
		DataDao()
		{
			helper = SqlHelper::Instance();
		}
		~DataDao() {}
		SqlHelper* helper;
	};
}

