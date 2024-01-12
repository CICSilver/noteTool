#include "contentlist.h"
#include "SqlHelper.h"
#include "datamodel.h"
#include "WordDao.h"
#include "DataDao.h"
#include <QMouseEvent>
#include <QMenu>
#include <QDebug>
ContentList::ContentList(QWidget *parent)
	: QListWidget(parent)
{
	helper = SqlHelper::Instance();
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
	m_contextMenu->addAction(deleteAction);
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
	qDebug() << "mouseDoubleClickEvent";
	if(event->button() == Qt::RightButton)
	{
		qDebug() << "right double clicked";
		event->ignore();
		return;
	}
	QListWidget::mouseDoubleClickEvent(event);
}

void ContentList::onDeleteActionTriggered()
{
	emit deleteDate();
}
