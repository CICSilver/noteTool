#pragma once

#include <QWidget>
#include "ui_searchwindow.h"
#include "SearchApi.h"

class SearchWindow : public QWidget
{
	Q_OBJECT

public:
	SearchWindow(QWidget *parent = nullptr);
	~SearchWindow();

	void Clear();
	QString toHtml(const FetchedWordInfo& info);
	void FetchWord(QString word);
public slots:
	void onWordFetched(FetchedWordInfo wordInfo);

private:
	Ui::SearchWindowClass ui;
	SearchApi* m_api;
};
