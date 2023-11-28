#pragma once
#include <QObject>
//µ¥´ÊÄ£ÐÍ
// °üº¬´Ê¡¢·­Òë¡¢´Ê¸ù¡¢Àý¾ä
class WordModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString word MEMBER m_word READ GetWord WRITE SetWord)
	Q_PROPERTY(QString translation MEMBER m_translation READ GetTranslation WRITE SetTranslation)
	Q_PROPERTY(QString root MEMBER m_root READ GetRoot WRITE SetRoot)
	Q_PROPERTY(QString sentence MEMBER m_sentence READ GetSentence WRITE SetSentence)
	Q_PROPERTY(int data_id MEMBER m_data_id READ GetDataId WRITE SetDataId)
public:
	WordModel(QObject *parent = nullptr) : QObject(parent)
	{
		m_data_id = -1;
	};
	~WordModel() {};
	WordModel(const WordModel& other)
		: m_word(other.m_word),
		m_translation(other.m_translation),
		m_root(other.m_root),
		m_sentence(other.m_sentence),
		m_data_id(other.m_data_id)
	{}

	// Getters and setters
	QString GetWord() const { return m_word; };
	void SetWord(const QString& word) { m_word = word; }
	QString GetTranslation() const { return m_translation;}
	void SetTranslation(const QString& translation) { m_translation = translation; }
	QString GetRoot() const { return m_root; }
	void SetRoot(const QString& root) { m_root = root; }
	QString GetSentence() const { return m_sentence; }
	void SetSentence(const QString& sentence) { m_sentence = sentence; }
	int GetDataId() const { return m_data_id; }
	void SetDataId(int data_id) { m_data_id = data_id; }

private:
	QString m_word;
	QString m_translation;
	QString m_root;
	QString m_sentence;
	int m_data_id;
};