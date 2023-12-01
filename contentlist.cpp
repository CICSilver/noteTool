#include "contentlist.h"
#include "SqlHelper.h"
#include "datamodel.h"
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
	QList<WordModel> wordList = helper->GetWordRecords();
	for (WordModel const& word : wordList)
	{
		this->addItem(new QListWidgetItem(word.GetWord()));
	}
}
