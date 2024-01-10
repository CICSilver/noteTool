#pragma once

#include <QMainWindow>
#include <QPushButton>
#include "ui_mainwindow.h"
#include "datamodel.h"

namespace dao
{
	class WordDao;
	class TranslationDao;
	class DataDao;
}

class QSystemTrayIcon;
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
	void BindShotCuts();

	void mousePressEvent(QMouseEvent *event) override;
	void changeEvent(QEvent *event) override;


	// "yyyy-MM-dd"
	void ShowWordsByDate(QString curDate);

	MainWindow& operator<<(int i)
	{
		qDebug() << i;
		return *this;
	}
protected slots:
	void OnShowSentence();
	void onOpenActionTriggered(bool checked);
	void onDateListItemDoubleClicked(QListWidgetItem* item);
private:
	Ui::MainWindowClass ui;
	DataModel m_dataModel;
	SqlHelper* helper;
	dao::DataDao* dataDao;
	dao::WordDao* wordDao;
	dao::TranslationDao* translationDao;
	QSystemTrayIcon *m_trayIcon;

};
