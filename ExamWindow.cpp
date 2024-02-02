#include "ExamWindow.h"
#include <algorithm>
#include <random>
ExamWindow::ExamWindow(QList<WordRecord> wordList, QWidget *parent)
	: QWidget(parent), m_wordList(wordList)
{
	ui.setupUi(this);
	m_curIndex = 0;
	Init();
}

ExamWindow::~ExamWindow()
{}

void ExamWindow::Init()
{
	// 打乱单词列表
	ShuffleWordList(m_wordList);
	// 隐藏内容
	SetContentHidden(true);
	// 显示第一个单词
	m_curIndex = 0;
	ShowWord(m_curIndex);
}

void ExamWindow::SetContentHidden(bool isHidden)
{
	ui.zhTrans->setHidden(isHidden);
	ui.sentence->setHidden(isHidden);
	ui.sentenceTrans->setHidden(isHidden);
}

void ExamWindow::ShowWord(int index)
{
	if (index < 0 || index >= m_wordList.size())
	{
		return;
	}
	
}

void ExamWindow::ShuffleWordList(QList<WordRecord>& list)
{
	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(list), std::end(list), rng);
}
