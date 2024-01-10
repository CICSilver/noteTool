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
		QList<TranslationModel> GetTranslationModelByWordId(int word_id);
		TranslationModel GetTranslationModelById(int id);

		void UpdateWithId(int id, TranslationModel model);
		int Exists(int word_id, int sub_id);

	private:
		TranslationDao()
		{
			helper = SqlHelper::Instance();
		}
		~TranslationDao() {};
		SqlHelper* helper;
	};
}

