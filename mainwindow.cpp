#include "mainwindow.h"
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
#include <QMessageBox>
#include <QFile>
#include "SqlHelper.h"
#include "model/translationModel.h"
#include "WordDao.h"
#include "DataDao.h"
#include "TranslationDao.h"
#include "SearchApi.h"

using namespace dbtable;
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_curShowDate = "";
	m_curShowDateId = -1;
	helper = SqlHelper::Instance();
	m_searchWindow = nullptr;
	m_soloWindow = nullptr;
	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setIcon(QIcon(":/MainWIndow/resources/ico/tray_icon.png"));
	dataDao = dao::DataDao::Instance();
	wordDao = dao::WordDao::Instance();
	translationDao = dao::TranslationDao::Instance();

	// 启动捕获服务
	m_captureServer = new WebsocketServer();
	m_captureServer->Start();

	// 初始化布局
	InitLayout();

	// 初始化单词表格
	InitTable();
	m_trayIcon->show();
	helper->SetForeignKeySupport(true);
	// 绑定搜索快捷键
	BindShotCuts();

	connect(this, &MainWindow::DateListItemDoubleClicked, ui.wordTable, &WordTableWidget::onDateListItemDoubleClicked);
	connect(ui.actionSolo, &QAction::triggered, this, &MainWindow::onActionSoloTriggered);
	connect(m_captureServer, &WebsocketServer::wordCapturedSignal, this, &MainWindow::onWordCaptured);
	connect(ui.dateList, &ContentList::deleteDate, this, &MainWindow::onDeleteDate);
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
	DataModel dataModel = dataDao->GetDataModelByDate(curDate);
	if (dataModel.isEmpty())
	{
		//创建日期记录
		Save();
	}
	//ShowWordsByDate(curDate);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	QByteArray curHash = ui.wordTable->Hash();
	if(curHash != m_lastSaveHash)
	{
		int ret = QMessageBox::warning(this, "警告", "内容未保存，是否保存?", "取消", "确认");
		if(ret == QMessageBox::Accepted)
		{
			Save();
		}
	}
	QMainWindow::closeEvent(e);
}

void MainWindow::Save()
{
	// 保存一个备份
	QString backupPath = QApplication::applicationDirPath() + "/../" + helper->GetDBPath() + "backup/";
	QDir backupDir(backupPath);
	if (!backupDir.exists())
	{
		backupDir.mkpath(backupPath);
	}
	QFile::copy(helper->GetFullDBPath(), backupPath + helper->GetDBName());

	// 保存当前数据
	m_lastSaveHash = ui.wordTable->Hash();
	DataModel dataModel;
	// 如果当前有选中的日期，则获取选择的日期
	QString dateStr = m_curShowDate.isEmpty() ? QDateTime::currentDateTime().toString("yyyy-MM-dd") : m_curShowDate;
	dataModel.SetDate(dateStr);
	helper->Insert<DataModel>(data::tableName, dataModel);
	//QSqlQuery query = helper->Where(data::tableName, "1=1");
	//query.last();
	int data_id = dataDao->GetDataModelByDate(dateStr).GetId();
	QList<WordRecord> recordList = ui.wordTable->Pack();
	// 获取数据库中的全部单词记录，删除不存在于recordList中的记录
	auto WordListInDB = wordDao->GetWordModelByDataId(data_id);
	for (auto& wordModel : WordListInDB)
	{
		bool isExist = false;
		for (auto& record : recordList)
		{
			// 判断其是否存在于recordList中
			isExist = (record.word.GetWord() == wordModel.GetWord());
			if (isExist) break;
		}
		if (!isExist)
		{
			// 删除单词
			wordDao->DeleteByWord(wordModel.GetWord());
		}
	}
	for (WordRecord& record : recordList)
	{
		record.word.SetDataId(data_id);
		helper->Insert<WordModel>(word::tableName, record.word);
		QSqlQuery query = helper->Where(word::tableName, QString("%1='%2'").arg(word::word).arg(record.word.GetWord()));
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
	ui.dateList->Update();
}

void MainWindow::BindShotCuts()
{
	QShortcut* shortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
	connect(shortcut, &QShortcut::activated, this, [&]()
	{
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
	int date_id = dataDao->GetDataModelByDate(curDate).GetId();
	if(m_curShowDateId != -1 && m_curShowDateId == date_id)
		return;
	m_curShowDateId = date_id;
	ui.wordTable->Init();
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
	m_curShowDate = item->text();
	ShowWordsByDate(m_curShowDate);
	emit DateListItemDoubleClicked();
}

void MainWindow::onDeleteDate()
{
	int date_id = ui.dateList->currentItem()->data(Qt::UserRole).toInt();

	// 删除日期记录
	dataDao->DeleteById(date_id);

	ui.dateList->Update();
	ui.wordTable->Init();
}

void MainWindow::onWordCaptured(QString word)
{
	if(!m_searchWindow)
		m_searchWindow = new SearchWindow();
	m_searchWindow->Clear();
	m_searchWindow->FetchWord(word);
}

void MainWindow::onActionSoloTriggered()
{
	if(!m_soloWindow)
		m_soloWindow = new SoloWindow();
	m_soloWindow->show();
}

void MainWindow::OnShowSentence()
{
	// 显示例句
	CellBtn* btn = qobject_cast<CellBtn*>(sender());
	if (!btn)
		return;
}
