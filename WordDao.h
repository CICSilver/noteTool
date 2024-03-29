#pragma once
#include "SqlHelper.h"
#include "WordModel.h"
namespace dao
{
	class WordDao
	{
	public:
		static WordDao* Instance()
		{
			static WordDao instance;
			return &instance;
		}
		QList<WordModel> GetAllWord();
		QList<WordModel> GetWordModelByDataId(int data_id);
		WordModel GetWordModelById(int id);

		void DeleteByDataId(int data_id);
		void DeleteByWord(QString word);
	private:
		WordDao()
		{
			helper = SqlHelper::Instance();
		}
		~WordDao() {}
		SqlHelper* helper;
	};
};

