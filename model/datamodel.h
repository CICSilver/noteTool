#pragma once
#include <qstring.h>
#include <QObject>
#include <QVariant>
#include <QSqlQuery>
// 每日记录单词数据模型
// 包含日期、单词模型列表
class DataModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int id MEMBER m_id READ GetId WRITE SetId)
	Q_PROPERTY(QString date MEMBER m_date READ GetDate WRITE SetDate)


public:
	explicit DataModel(QObject* parent = nullptr):QObject(parent) 
	{
		m_id = -1;
	};
	DataModel(const QSqlQuery& query)
	{
		m_id = query.value("id").toInt();
		m_date = query.value("date").toString();
	}
	~DataModel() {};
	DataModel(const DataModel& other)
		: m_id(other.m_id),
		m_date(other.m_date) {}
	bool isEmpty()
	{
		return m_id == -1 && m_date.isEmpty();
	}
	// getters and setters
	int GetId() const { return m_id; }
	void SetId(int id) { m_id = id; }
	QString GetDate() const { return m_date; }
	void SetDate(const QString& date) { m_date = date; }

private:
	int m_id;
	QString m_date;

};

