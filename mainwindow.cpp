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
#include <QShortcut>
#include "SqlHelper.h"
#include "model/translationModel.h"
#include "WordDao.h"
#include "DataDao.h"
#include "TranslationDao.h"
using namespace dbtable;
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	helper = SqlHelper::Instance();
	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setIcon(QIcon(":/MainWIndow/resources/ico/tray_icon.png"));
	dataDao = dao::DataDao::Instance();
	wordDao = dao::WordDao::Instance();
	translationDao = dao::TranslationDao::Instance();

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
	// 绑定搜索快捷键
	BindShotCuts();

	connect(ui.dateList, &ContentList::itemDoubleClicked, this, &MainWindow::onDateListItemDoubleClicked);
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
	// 
	QString curDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	ShowWordsByDate(curDate);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
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
	QList<WordRecord> recordList = ui.wordTable->Pack();
	for (WordRecord& record : recordList)
	{
		record.word.SetDataId(data_id);
		helper->Insert<WordModel>(word::tableName, record.word);
		query = helper->Where(word::tableName, QString("%1='%2'").arg(word::word).arg(record.word.GetWord()));
		query.next();
		int word_id = query.value(word::id).toInt();
		for(auto& transModel: record.transList)
		{
			transModel.SetWordId(word_id);
			int transId = translationDao->Exists(word_id, transModel.GetSubId());
			if (transId != -1)
			{
				// 已存在，更新
				translationDao->UpdateWithId(transId, transModel);
			}
			else
			{
				// 不存在，插入
				helper->Insert<TranslationModel>(translation::tableName, transModel);
			}
		}
	}
}

void MainWindow::BindShotCuts()
{
	QShortcut* shortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
	connect(shortcut, &QShortcut::activated, this, [&]()
	{
		qDebug() << "Search word";
	});

	connect(new QShortcut(QKeySequence("Ctrl+S"), this), &QShortcut::activated, this, [&]()
	{
		Save();
	});
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
			this->hide();
			m_trayIcon->show();
			event->ignore();
		}
	}
}

void MainWindow::ShowWordsByDate(QString curDate)
{
	ui.wordTable->clear();
	int date_id = dataDao->GetDataModelByDate(curDate).GetId();
	for(auto const& word: wordDao->GetWordModelByDataId(date_id))
	{
		 QList<TranslationModel> transList = translationDao->GetTranslationModelByWordId(word.GetId());
		 ui.wordTable->AppendWordRecord(WordRecord(word, transList));
	}
}

void MainWindow::onOpenActionTriggered(bool checked)
{
}

void MainWindow::onDateListItemDoubleClicked(QListWidgetItem* item)
{
	ShowWordsByDate(item->text());
}

void MainWindow::OnShowSentence()
{
	// 显示例句
	CellBtn* btn = qobject_cast<CellBtn*>(sender());
	if (!btn)
		return;
}
