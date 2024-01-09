#pragma once
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
// 剑桥词典查询URL
constexpr char CambridgeUrl[] = "https://dictionary.cambridge.org/zhs/%E8%AF%8D%E5%85%B8/%E8%8B%B1%E8%AF%AD-%E6%B1%89%E8%AF%AD-%E7%AE%80%E4%BD%93/";

class SearchApi : public QObject
{
	Q_OBJECT
public:
	static SearchApi* Instance()
	{
		static SearchApi api;
		return &api;
	}
	void SetUrl(QString url) { m_url = url; };
	void Fetch(QString url = "");
	void FetchWord(QString word);
private:
	SearchApi(QString url = "", QObject* parent = nullptr)
	: QObject(parent)
	{
		m_url = url.isEmpty() ?
			CambridgeUrl :
			url;
	}
	~SearchApi() 
	{
	}
	QString m_url;
};

