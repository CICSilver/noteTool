#pragma once

#include <QMainWindow>
#include <QPushButton>
#include "ui_mainwindow.h"
#include "datamodel.h"
class CellBtn;
class SqlHelper;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void InitLayout();
	void InitTable();
	void closeEvent(QCloseEvent* e) override;
	void Save();

	void mousePressEvent(QMouseEvent *event) override;

protected slots:
	void OnShowSentence();
	void onOpenActionTriggered(bool checked);
	void onCellClicked();
private:
	Ui::MainWindowClass ui;
	DataModel m_dataModel;
	SqlHelper* helper;

};
