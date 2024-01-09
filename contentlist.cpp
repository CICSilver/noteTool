#include "contentlist.h"
#include "SqlHelper.h"
#include "datamodel.h"
#include "WordDao.h"
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
	QList<WordModel> wordList = dao::WordDao::Instance()->GetAllWord();
	for (WordModel const& word : wordList)
	{
		this->addItem(new QListWidgetItem(word.GetWord()));
	}
}
