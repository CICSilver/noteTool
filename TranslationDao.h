#pragma once
#include "SqlHelper.h"
#include "TranslationModel.h"
namespace dao
{
	class TranslationDao
	{
	public:
		static TranslationDao* Instance()
		{
			static TranslationDao instance;
			return &instance;
		}
		QList<TranslationModel> GetAllTranslation();
		TranslationModel GetTranslationDaoByWordId(int word_id);
		TranslationModel GetTranslationDaoById(int id);

	private:
		TranslationDao()
		{
			helper = SqlHelper::Instance();
		}
		~TranslationDao() {};
		SqlHelper* helper;
	};
}

