#pragma once
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
// 剑桥词典查询URL
constexpr char CambridgeUrl[] = "https://dictionary.cambridge.org/zhs/%E8%AF%8D%E5%85%B8/%E8%8B%B1%E8%AF%AD-%E6%B1%89%E8%AF%AD-%E7%AE%80%E4%BD%93/";

struct WordMeaning
{
	void SetSentences(QList<QStringList> fetchedSentences)
	{
		for(auto const& sentence: fetchedSentences)
		{
			if(sentence.size() > 3)
			{
				sentences.insert(sentence.at(0), sentence.at(2));
			}
		}
	}
	void Print()
	{
		qDebug() << "guideWord: " << guideWord;
		qDebug() << "zhTranslation: " << zhTranslation;
		qDebug() << "enTranslation: " << enTranslation;
		for(auto it = sentences.begin(); it != sentences.end(); ++it)
		{
			qDebug() << "sentence: " << it.key();
			qDebug() << "translation: " << it.value();
		}
		qDebug() << " ================================ ";
	}
	QString guideWord;
	QString zhTranslation;
	QString enTranslation;
	QMap<QString, QString> sentences;
};

struct SubWordInfo
{
	void Print()
	{
		qDebug() << QString("partOfSpeech: %1").arg(partOfSpeech);
		for(auto& meaning: wordMeanings)
		{
			meaning.Print();
		}
	}
	QString partOfSpeech;
	QList<WordMeaning> wordMeanings;
};

struct FetchedWordInfo
{
	void Print()
	{
		qDebug() << "===============================================";
		qDebug() << QString("word: %1").arg(word);
		for (auto& subinfo : subWordInfoList)
		{
			subinfo.Print();
			qDebug() << "===============================================";
		}
	}
	QString word;
	QList<SubWordInfo> subWordInfoList;
};

class SearchApi : public QObject
{
	Q_OBJECT
public:
	static SearchApi* Instance()
	{
		static SearchApi api;
		return &api;
	}
	//void SetUrl(QString url) { m_url = url; };
	void FetchWord(QString word);

signals:
	void WordFetched(FetchedWordInfo wordInfo);
public slots:
	void onWordFetched(FetchedWordInfo wordInfo)
	{
		qDebug() << "onWordFetched";
		wordInfo.Print();
	}
private:
	void Fetch(QString url = "");
	SearchApi(QString url = "", QObject* parent = nullptr)
	: QObject(parent)
	{
		m_url = url.isEmpty() ?
			CambridgeUrl :
			url;
		//connect(this, &SearchApi::WordFetched, this, &SearchApi::onWordFetched);
	}
	~SearchApi() 
	{
	}
	QString m_url;
	QString m_wordToFetch;
};

