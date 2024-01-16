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

	QString toHtml(const FetchedWordInfo& info);
public slots:
	void onWordFetched(FetchedWordInfo wordInfo);

private:
	Ui::SearchWindowClass ui;
	SearchApi* m_api;
};
