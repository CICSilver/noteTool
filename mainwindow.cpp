#include "mainwindow.h"
#include <QTableWidget>
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitLayout();
	InitTable();
}

MainWindow::~MainWindow()
{}

void MainWindow::InitLayout()
{
	ui.centralWidget->setContentsMargins(QMargins(0, 0, 0, 0));
}

void MainWindow::InitTable()
{
	QTableWidget* pTable = ui.tableWidet;
	pTable->setRowCount(1);
	pTable->setColumnCount(3);
	pTable->setHorizontalHeaderLabels(QStringList() << QStringLiteral("����") << QStringLiteral("����") << QStringLiteral("�ʸ�"));
}

nlohmann::json MainWindow::Save()
{
	return nlohmann::json();
}
