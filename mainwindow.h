#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "json.hpp"
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void InitLayout();
	void InitTable();
	nlohmann::json Save();

private:
	Ui::MainWindowClass ui;
	nlohmann::json m_data;
};
