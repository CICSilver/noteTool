#pragma once

#include <QWidget>
#include <QList>
#include "wordtablewidget.h"
#include "ui_ExamWindow.h"

class ExamWindow : public QWidget
{
	Q_OBJECT

public:
	ExamWindow(QList<WordRecord> wordList, QWidget *parent = nullptr);
	~ExamWindow();

	void Init();
	void SetContentHidden(bool isHidden);
	void ShowWord(int index);
	// 打乱单词列表
	void ShuffleWordList(QList<WordRecord>& list);
private:
	Ui::ExamWindowClass ui;
	int m_curIndex;
	QList<WordRecord> m_wordList;
	QList<WordRecord> m_wrongList;
};
