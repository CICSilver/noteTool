#pragma once
#include <QObject>
#include <QSqlQuery>
#include <QVariant>
#include "dbDefines.h"
class TranslationModel : public QObject
{
	Q_OBJECT

		Q_PROPERTY(int id MEMBER m_id READ GetId WRITE SetId)
		Q_PROPERTY(int sub_id MEMBER m_sub_id READ GetSubId WRITE SetSubId)
		Q_PROPERTY(QString root MEMBER m_root READ GetRoot WRITE SetRoot)
		Q_PROPERTY(QString zh_translation MEMBER m_zh_translation READ GetZhTranslation WRITE SetZhTranslation)
		Q_PROPERTY(QString en_translation MEMBER m_en_translation READ GetEnTranslation WRITE SetEnTranslation)
		Q_PROPERTY(QString sentence MEMBER m_sentence READ GetSentence WRITE SetSentence)
		Q_PROPERTY(int word_id MEMBER m_word_id READ GetWordId WRITE SetWordId)
public:
	TranslationModel(QObject* parent = nullptr) : QObject(parent)
	{
		m_id = -1;
		m_sub_id = 0;
		m_word_id = -1;
	};
	TranslationModel(QSqlQuery const& query)
	{
		this->m_id = query.value(dbtable::translation::id).toInt();
		this->m_sub_id = query.value(dbtable::translation::sub_id).toInt();
		this->m_word_id = query.value(dbtable::translation::word_id).toInt();
		this->m_zh_translation = query.value(dbtable::translation::zhTranslation).toString();
		this->m_en_translation = query.value(dbtable::translation::enTranslation).toString();
		this->m_sentence = query.value(dbtable::translation::sentence).toString();
		this->m_root = query.value(dbtable::translation::root).toString();
	}
	~TranslationModel() {};
	TranslationModel(const TranslationModel& other)
		:
		m_id(other.m_id),
		m_sub_id(other.m_sub_id),
		m_en_translation(other.m_en_translation),
		m_zh_translation(other.m_zh_translation),
		m_root(other.m_root),
		m_sentence(other.m_sentence),
		m_word_id(other.m_word_id)
	{}
	TranslationModel& operator=(const TranslationModel& other)
	{
		this->m_id = other.m_id;
		this->m_sub_id = other.m_sub_id;
		this->m_word_id = other.m_word_id;
		this->m_zh_translation = other.m_zh_translation;
		this->m_en_translation = other.m_en_translation;
		this->m_root = other.m_root;
		this->m_sentence = other.m_sentence;
		return *this;
	};

	bool isEmpty()
	{
		return m_zh_translation.isEmpty() && 
			m_en_translation.isEmpty() && 
			m_root.isEmpty() && 
			m_sentence.isEmpty();
	}

	// Getter/Setters
	QString GetRoot() const { return m_root; }
	void SetRoot(const QString& root) { m_root = root; }
	QString GetZhTranslation() const { return m_zh_translation; }
	void SetZhTranslation(const QString& zh_translation) { m_zh_translation = zh_translation; }
	QString GetEnTranslation() const { return m_en_translation; }
	void SetEnTranslation(const QString& en_translation) { m_en_translation = en_translation; }
	QString GetSentence() const { return m_sentence; }
	void SetSentence(const QString& sentence) { m_sentence = sentence; }
	int GetWordId() const { return m_word_id; }
	void SetWordId(int word_id) { m_word_id = word_id; }
	int GetId() const { return m_id; }
	void SetId(int id) { m_id = id; }
	int GetSubId() const { return m_sub_id; }
	void SetSubId(int id) { m_sub_id = id; }

	bool isEmpty() const
	{
		return m_zh_translation.isEmpty() && m_en_translation.isEmpty() && m_root.isEmpty() && m_sentence.isEmpty();
	}

private:
	int m_id;
	int m_word_id;
	int m_sub_id;
	QString m_zh_translation;
	QString m_en_translation;
	QString m_sentence;
	QString m_root;
};