#pragma once

#include <QListWidget>
#include "SqlHelper.h"

class ExamWindow;
class SqlHelper;
class ContentList : public QListWidget
{
	Q_OBJECT

public:
	ContentList(QWidget *parent = nullptr);
	~ContentList();

	void InitContextMenu();
	// 从数据库更新单词列表
	void Update();
	void contextMenuEvent(QContextMenuEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
	void deleteDate();
	
protected slots:
	void onDeleteActionTriggered();
	void onExamActionTriggered();
private:
	SqlHelper* helper;
	QMenu* m_contextMenu;
	ExamWindow* m_examWindow;
};
