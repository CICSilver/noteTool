#include "mainwindow.h"
#include "cellbtn.h"
#include <QTableWidget>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include <iostream>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include "SqlHelper.h"
#include "model/translationModel.h"
using namespace dbtable;
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	helper = SqlHelper::Instance();
	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setIcon(QIcon(":/MainWIndow/resources/ico/tray_icon.png"));
	InitLayout();
	InitTable();
	m_trayIcon->show();
#ifdef _DEBUG
	//WordModel word;
	//word.SetWord("abadon");
	//word.SetTranslation("放弃");
	//word.SetRoot("don");
	//ui.wordTable->AppendWordRecord(word);
#endif
	connect(ui.openAction, &QAction::triggered, this, &MainWindow::onOpenActionTriggered);
	// 系统托盘
	connect(m_trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason)
		{
			if (reason == QSystemTrayIcon::Trigger)
			{
				// 还原窗口设置
				this->setWindowState(Qt::WindowActive);
				this->showNormal();
			}
		});
	// 清除输入框焦点
	connect(ui.wordTable, &WordTableWidget::TableClicked, this, [this]()
		{
			ui.lineEdit->clearFocus();
			//ui.wordTable->setFocus();
		});
	this->installEventFilter(ui.wordTable);
}

MainWindow::~MainWindow()
{}

void MainWindow::InitLayout()
{
	ui.centralWidget->setContentsMargins(QMargins(0, 0, 0, 0));
	ui.openAction->setText("打开");
	ui.saveAction->setText("保存");
}

void MainWindow::InitTable()
{
	// 初始化表格数据
	// 从数据库查找当天内容，若找到则写入
	// 先通过日期筛选当日单词和释义
	// 再根据释义关联的单词id添加
	QString curDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QList<WordModel> wordList = GetWordListByDate(curDate);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	// 关闭时自动保存
	Save();
	QMainWindow::closeEvent(e);
}

void MainWindow::Save()
{
	DataModel dataModel;
	dataModel.SetDate(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
	helper->Insert<DataModel>(data::tableName, dataModel);
	QSqlQuery query = helper->Where(data::tableName, "1=1");
	query.last();
	int data_id = query.value(data::id).toInt();
	qDebug() << data_id;
	QList<WordModel> word_list = ui.wordTable->Pack();
	for (WordModel& wordModel : word_list)
	{
		wordModel.SetDataId(data_id);
		helper->Insert<WordModel>(word::tableName, wordModel);
	}
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
	//qDebug() << "MainWindow::mousePressEvent";
	if(childAt(event->pos()) != ui.lineEdit)
	{
		ui.lineEdit->clearFocus();
	}
	QMainWindow::mousePressEvent(event);
}

void MainWindow::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::WindowStateChange)
	{
		if(this->windowState() & Qt::WindowMinimized)
		{
			qDebug() << "MainWindow::changeEvent";
			this->hide();
			m_trayIcon->show();
			qDebug() << m_trayIcon->isVisible();
			event->ignore();
		}
	}
}

QList<WordModel> MainWindow::GetWordListByDate(QString curDate)
{
	QList<WordModel> wordList;
	
	QSqlQuery query = helper->Where(dbtable::data::tableName, dbtable::data::date + "='" + curDate + "'");
	if (query.next())
	{
		QString data_id = query.value(dbtable::data::id).toString();
		QSqlQuery wordQuery = helper->Where(dbtable::word::tableName, dbtable::word::data_id + "='" + data_id + "'");
		while (wordQuery.next())
		{
			WordModel model;
			QString whereTransSql = dbtable::translation::word_id + "='" + wordQuery.value(dbtable::word::id).toString() + "'";
			QSqlQuery transQuery = helper->Where(dbtable::translation::tableName, whereTransSql);
			// 翻译应该唯一,只取第一个查询结果
			if (transQuery.next())
			{
				TranslationModel transModel;
				transModel.SetWordId(transQuery.value(dbtable::translation::word_id).toInt());
			}
			model.SetWord(wordQuery.value(dbtable::word::word).toString());


			// 根据获取到的单词插入记录
			ui.wordTable->AppendWordRecord(model);
		}
	}
	helper->CloseDB();
	return wordList;
}

void MainWindow::onOpenActionTriggered(bool checked)
{
}

void MainWindow::onCellClicked()
{
	qDebug() << "onCellClicked";
}

void MainWindow::OnShowSentence()
{
	// 显示例句
	CellBtn* btn = qobject_cast<CellBtn*>(sender());
	if (!btn)
		return;
}
