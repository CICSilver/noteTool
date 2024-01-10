#pragma once
#include <QString>
namespace dbtable
{
	namespace data
	{
		const QString tableName = "Data";
		const QString id = "id";
		const QString date = "date";
	}
	namespace word
	{
		const QString tableName = "Word";
		const QString data_id = "data_id";
		const QString id = "id";
		const QString word = "word";
	}
	namespace translation
	{
		const QString tableName = "Translation";
		const QString word_id = "word_id";
		const QString sub_id = "sub_id";
		const QString id = "id";
		const QString zhTranslation = "zh_translation";
		const QString enTranslation = "en_translation";
		const QString sentence = "sentence";
		const QString root = "root";
	}
}