#pragma once

#include <QMainWindow>
#include <QPushButton>
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonObject>
class CellBtn;
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
	void InitSaveJson();
	void PackJson();

protected slots:
	void OnShowSentence(int row);
	// 当上一行写入数据后，创建下一行
	void OnLastRowWrited(QTableWidgetItem* item);
private:
	Ui::MainWindowClass ui;
	QList<CellBtn*> m_btn_list;
	const QString json_path = "./data/noteTool.json";

	void AddSentenceBtn(int row);
};
