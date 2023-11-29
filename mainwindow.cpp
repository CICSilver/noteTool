#include "mainwindow.h"
#include "cellbtn.h"
#include <QTableWidget>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitLayout();
	InitTable();
	connect(ui.tableWidet, &QTableWidget::itemChanged, this, &MainWindow::OnLastRowWrited);
}

MainWindow::~MainWindow()
{}

void MainWindow::InitLayout()
{
	ui.centralWidget->setContentsMargins(QMargins(0, 0, 0, 0));
	ui.openAction->setText(QStringLiteral("打开"));
	ui.saveAction->setText(QStringLiteral("保存"));
}

void MainWindow::InitTable()
{
	QTableWidget* pTable = ui.tableWidet;
	pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	pTable->setRowCount(1);
	pTable->setColumnCount(4);
	pTable->setHorizontalHeaderLabels(QStringList() <<
		QStringLiteral("生词") <<
		QStringLiteral("释义") <<
		QStringLiteral("词根") <<
		QStringLiteral("例句"));
	AddSentenceBtn(0);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	// 关闭时自动保存
	Save();
	QMainWindow::closeEvent(e);
}

void MainWindow::Save()
{
	InitSaveJson();
}

void MainWindow::InitSaveJson()
{
	QFile file(json_path);
	if (!file.exists())
	{
		QJsonObject root;
		root["SaveData"] = QJsonArray();
		if (file.open(QIODevice::WriteOnly))
		{
			qDebug() << "json file not exist, creating...";
			QJsonDocument doc(root);
			file.write(doc.toJson());
			file.close();
		}
		else
		{
			qDebug() << "file open failed";
		}
	}
}

void MainWindow::PackJson()
{
	// 打包表格内容为json
	auto pTable = ui.tableWidet;
	for (int row = 0; row < pTable->rowCount(); ++row)
	{
		QJsonObject record;

		for (int col = 0; col < pTable->colorCount(); ++col)
		{

		}
	}
}

void MainWindow::OnLastRowWrited(QTableWidgetItem* item)
{
	QTableWidget* pTable = ui.tableWidet;
	if (item->row() != pTable->rowCount() - 1)
		return;
	// 改变内容的是最后一行，则增加新行
	int newRowCnt = pTable->rowCount() + 1;
	pTable->setRowCount(newRowCnt);
	AddSentenceBtn(newRowCnt - 1);
}

void MainWindow::OnShowSentence(int row)
{
	// 显示例句
	qDebug() << "show" << row << "sentence";
}

void MainWindow::AddSentenceBtn(int row)
{
	QTableWidget* pTable = ui.tableWidet;
	CellBtn* btn = new CellBtn(row);
	btn->setText(QStringLiteral("显示"));
	m_btn_list.append(btn);
	connect(btn, &CellBtn::ShowSentence, this, &MainWindow::OnShowSentence);
	pTable->setCellWidget(row, pTable->columnCount() - 1, btn);
}
