#pragma once
#include <qstring.h>
#include <QObject>
#include "wordModel.h"
// ÿ�ռ�¼��������ģ��
// �������ڡ�����ģ���б�
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
	~DataModel() {};

	// getters and setters
	int GetId() const { return m_id; }
	void SetId(int id) { m_id = id; }
	QString GetDate() const { return m_date; }
	void SetDate(const QString& date) { m_date = date; }

private:
	int m_id;
	QString m_date;

};

