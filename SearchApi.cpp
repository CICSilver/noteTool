#include "SearchApi.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
QList<QStringList> regularMatch(QString& content, QRegularExpression re)
{
	QString str = content;
    QList<QStringList> ret;
	//QRegularExpression re("<span class=\"trans dtrans dtrans-se  break-cj\" lang=\"zh-Hans\">(.*?)</span>");
	QRegularExpressionMatchIterator matches = re.globalMatch(str);
	while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
		QString matched = match.captured(1);  // 获取第一个捕获组的内容
        matched.replace(QRegularExpression("<[^>]*>"), "");
        matched.replace(QRegularExpression(" {2,}"), "");
        QStringList list = matched.split('\n');
		// 处理 matched
        ret << list;
    }
    return ret;
}

QList<QStringList> ParseAllSentences(QString& content)
{
    return regularMatch(content, QRegularExpression("<div class=\"examp dexamp\">([\\s\\S]*?)</div>"));
}

QString ParseAllZhTranslations(QString& content)
{
    QList<QStringList> ret = regularMatch(content,
        QRegularExpression("<span class=\"trans dtrans dtrans-se  break-cj\" lang=\"zh-Hans\">([\\s\\S]*?)(?!</span></a>)</span>"));
    return ret.size() > 0 ? ret.at(0).at(0) : "";
}

QString ParseAllEnTranslation(QString& content)
{
    QList<QStringList> retList = regularMatch(content,
        QRegularExpression("<div class=\"def ddef_d db\">([\\s\\S]*?)<div class=\"def-body ddef_b\">"));
    QStringList ret = retList.size() > 0 ? retList.at(0) : QStringList();
    QString trans;
    // 拼接非空字符串
    for(auto it = ret.begin(); it != ret.end(); ++it)
    {
        if(!it->isEmpty())
        {
            trans += *it;
        }
    }
    return trans;
}

QString ParseGuideWord(QString& content)
{
    QList<QStringList> ret = regularMatch(content,
        QRegularExpression("\\(<span>([\\s\\S]*?)</span>\\)"));
    return ret.size() > 0 && ret .at(0).size() > 0? ret.at(0).at(0) : "";
}

QString ParsePartOfSpeech(QString& content)
{
    QList<QStringList> ret = regularMatch(content,
		QRegularExpression("<span class=\"pos dpos\".*?>([\\s\\S]*?)</span>"));
    return ret.size() > 0 && ret.at(0).size() > 0 ? ret.at(0).at(0) : "";
}

void ParseBlock(FetchedWordInfo& wordInfo, QString& content)
{
	QString str = content;
    QRegularExpression FirstBlock("<div class=\"pr entry-body__el\">[\\s\\S]*?</div></div></div></div></div>");
    QRegularExpressionMatchIterator FirstMatches = FirstBlock.globalMatch(str);
    while(FirstMatches.hasNext())
    {
        // 多个词性
        SubWordInfo subInfo;
        QRegularExpressionMatch FirstMatch = FirstMatches.next();
        QString FirstMatched = FirstMatch.captured(0);
        QRegularExpression re("<div class=\"pr dsense \">([\\s\\S]*?)</div></div>");
	    QRegularExpressionMatchIterator matches = re.globalMatch(FirstMatched);
        QString partOfSpeech = ParsePartOfSpeech(FirstMatched);
        subInfo.partOfSpeech = partOfSpeech;
        if (matches.hasNext())
        {
            // 有提示词
	        while (matches.hasNext()) {
				WordMeaning meaning;
		        QRegularExpressionMatch match = matches.next();
		        QString matched = match.captured(1);  // 获取第一个捕获组的内容
                QString guideWord = ParseGuideWord(matched);
                meaning.guideWord = guideWord;
                meaning.enTranslation = ParseAllEnTranslation(matched);
                meaning.zhTranslation = ParseAllZhTranslations(matched);
                meaning.SetSentences(ParseAllSentences(matched));
                subInfo.wordMeanings.append(meaning);
	        }
        }
        else
        {
            // 无提示词
            // 直接匹配块
            QRegularExpression re("<div class=\"def-block ddef_block \".*?>([\\s\\S]*?)</div></div>");
		    QRegularExpressionMatchIterator matches = re.globalMatch(FirstMatched);
            while(matches.hasNext())
            {
                WordMeaning meaning;
			    QRegularExpressionMatch match = matches.next();
			    QString matched = match.captured(1);  // 获取第一个捕获组的内容
                meaning.enTranslation = ParseAllEnTranslation(matched);
                meaning.zhTranslation = ParseAllZhTranslations(matched);
                meaning.SetSentences(ParseAllSentences(matched));
                subInfo.wordMeanings.append(meaning);
            }
        }
        //subInfo.wordMeanings.append(meaning);
        wordInfo.subWordInfoList.append(subInfo);
    }
	qDebug() << "match done";
}

void SearchApi::Fetch(QString url)
{
    if (!url.isEmpty()) {
        m_url = url;
    }
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(m_url));

    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        FetchedWordInfo wordInfo;
        wordInfo.word = m_wordToFetch;
        if (reply->error() == QNetworkReply::NoError) {
            QString contents = reply->readAll();
            ParseBlock(wordInfo, contents);
            emit WordFetched(wordInfo);
            //wordInfo.PrintWordInfo();
        }
        reply->deleteLater();
        qDebug() << "fetch done, results save in file test";
    });
}

void SearchApi::FetchWord(QString word)
{
    if(m_url.isEmpty())
    {
        m_url = CambridgeUrl;
    }
    m_wordToFetch = word;
    Fetch(CambridgeUrl + word);
}
