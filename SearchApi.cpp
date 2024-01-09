#include "SearchApi.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
void regularMatch(QString& content, QRegularExpression re)
{
	QString str = content;
	//QRegularExpression re("<span class=\"trans dtrans dtrans-se  break-cj\" lang=\"zh-Hans\">(.*?)</span>");
	QRegularExpressionMatchIterator matches = re.globalMatch(str);
	while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
		QString matched = match.captured(1);  // 获取第一个捕获组的内容
        matched.replace(QRegularExpression("<[^>]*>"), "");
        matched.replace(QRegularExpression(" {2,}"), "");
        QStringList list = matched.split('\n');
		// 处理 matched
        qDebug() << list;
    }
    
}

void ParseAllSentences(QString& content)
{
    regularMatch(content, QRegularExpression("<div class=\"examp dexamp\">([\\s\\S]*?)</div>"));
}

void ParseAllZhTranslations(QString& content)
{
    regularMatch(content,
        QRegularExpression("<span class=\"trans dtrans dtrans-se  break-cj\" lang=\"zh-Hans\">([\\s\\S]*?)(?!</span></a>)</span>"));
}

void ParseAllEnTranslation(QString& content)
{
    regularMatch(content,
        QRegularExpression("<div class=\"def ddef_d db\">([\\s\\S]*?)<div class=\"def-body ddef_b\">"));
}

void ParseGuideWord(QString& content)
{
    regularMatch(content,
        QRegularExpression("\\(<span>([\\s\\S]*?)</span>\\)"));
}

void ParseBlock(QString& content)
{
	QString str = content;
    QRegularExpression re("<div class=\"pr dsense \">([\\s\\S]*?)</div></div>");
	QRegularExpressionMatchIterator matches = re.globalMatch(str);
	while (matches.hasNext()) {
		QRegularExpressionMatch match = matches.next();
		QString matched = match.captured(1);  // 获取第一个捕获组的内容
        //qDebug() << matched;
        ParseGuideWord(matched);
		ParseAllEnTranslation(matched);
		ParseAllZhTranslations(matched);
		ParseAllSentences(matched);
        qDebug() << " =================== ";
	}
	qDebug() << "match done";
}

void SearchApi::Fetch(QString url)
{
    if (!url.isEmpty()) {
        m_url = url;
    }
    qDebug() << "starting fetch";
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(m_url));

    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString contents = reply->readAll();
            //ParseAllSentences(contents);
            //ParseAllTranslations(contents);
            ParseBlock(contents);
            
            QFile file("test");
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream out(&file);
                out << contents;
                file.close();
            }
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
    Fetch(CambridgeUrl + word);
}
