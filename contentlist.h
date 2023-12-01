#pragma once

#include <QListWidget>
#include "SqlHelper.h"
class SqlHelper;
class ContentList : public QListWidget
{
	Q_OBJECT

public:
	ContentList(QWidget *parent = nullptr);
	~ContentList();

	// 从数据库更新单词列表
	void Update();
private:
	SqlHelper* helper;
};
