#include "contentlist.h"
#include "SqlHelper.h"
#include "datamodel.h"
#include "WordDao.h"
#include "DataDao.h"
#include "ExamWindow.h"
#include <QMouseEvent>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>
ContentList::ContentList(QWidget *parent)
	: QListWidget(parent)
{
	helper = SqlHelper::Instance();
	m_examWindow = nullptr;
	InitContextMenu();
	// 初始化全部记录
	Update();
}

ContentList::~ContentList()
{
	if(m_contextMenu)
	{
		delete m_contextMenu;
		m_contextMenu = nullptr;
	}
}

void ContentList::InitContextMenu()
{
	m_contextMenu = new QMenu(this);
	QAction* deleteAction = new QAction("删除", this);
	connect(deleteAction, &QAction::triggered, this, &ContentList::onDeleteActionTriggered);

	QAction* examAction = new QAction("测验", this);
	connect(examAction, &QAction::triggered, this, &ContentList::onExamActionTriggered);
	m_contextMenu->addAction(deleteAction);
	m_contextMenu->addAction(examAction);
}

void ContentList::Update()
{
	clear();
	auto dataDao = dao::DataDao::Instance();
	auto list = dataDao->GetAllData();
	for(auto const& dataModel : list)
	{
		QListWidgetItem* item = new QListWidgetItem(dataModel.GetDate());
		item->setData(Qt::UserRole, dataModel.GetId());
		this->addItem(item);
	}
}

void ContentList::contextMenuEvent(QContextMenuEvent* event)
{
	QListWidgetItem* item = this->itemAt(event->pos());
	if(item)
	{
		QPoint pos = event->globalPos();
		pos.setX(pos.x() + 1);  // 将菜单向右移动一点
		m_contextMenu->exec(pos);
	}else
	{
		event->ignore();
	}
}

void ContentList::mouseDoubleClickEvent(QMouseEvent* event)
{
	if(event->button() == Qt::RightButton)
	{
		event->ignore();
		return;
	}
	QListWidget::mouseDoubleClickEvent(event);
}

void ContentList::onDeleteActionTriggered()
{
	int ret = QMessageBox::warning(this, "警告", "是否确认删除?", "取消", "确认");
	if (ret == QMessageBox::Accepted)
	{
		emit deleteDate();
	}
}

void ContentList::onExamActionTriggered()
{
	if(!m_examWindow)
		m_examWindow = new ExamWindow();
	m_examWindow->show();
}
