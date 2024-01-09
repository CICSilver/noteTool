#pragma once
#include <QObject>
#include <QSqlQuery>
#include <QVariant>
#include "dbDefines.h"
//#include "translationModel.h"
class WordModel : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString word MEMBER m_word READ GetWord WRITE SetWord)
		Q_PROPERTY(int data_id MEMBER m_data_id READ GetDataId WRITE SetDataId)
public:
	WordModel(QObject* parent = nullptr) : QObject(parent)
	{
		m_data_id = -1;
	};
	WordModel(const QSqlQuery& query)
	{
		m_word = query.value("word").toString();
		m_data_id = query.value("data_id").toInt();
	}
	~WordModel() {};
	WordModel(const WordModel& other)
		: m_word(other.m_word),
		m_data_id(other.m_data_id)
	{}

	// Getters and setters
	QString GetWord() const { return m_word; };
	void SetWord(const QString& word) { m_word = word; }
	int GetDataId() const { return m_data_id; }
	void SetDataId(int data_id) { m_data_id = data_id; }

	bool isEmpty() const
	{
		return m_word.isEmpty();
	}

private:
	QString m_word;
	int m_data_id;
};