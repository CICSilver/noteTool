#include "contentlist.h"
#include "SqlHelper.h"
#include "datamodel.h"
#include "WordDao.h"
#include "DataDao.h"
ContentList::ContentList(QWidget *parent)
	: QListWidget(parent)
{
	helper = SqlHelper::Instance();
	// 初始化全部记录
	Update();
}

ContentList::~ContentList()
{}

void ContentList::Update()
{
	clear();
	auto dataDao = dao::DataDao::Instance();
	auto list = dataDao->GetAllData();
	for(auto const& dataModel : list)
	{
		this->addItem(new QListWidgetItem(dataModel.GetDate()));
	}
}