#include "cellbtn.h"
#include <QTableWidget>
#include <QDebug>
CellBtn::CellBtn(int row, QWidget* parent)
	: QPushButton(parent),
	m_row(row)
{
	ui.setupUi(this);
	connect(this, &CellBtn::clicked, this, [this]()
		{
			emit ShowSentence();
		});
}

CellBtn::~CellBtn()
{}

void CellBtn::mousePressEvent(QMouseEvent* e)
{
	QTableWidget* pTable = qobject_cast<QTableWidget*>(parentWidget());
	if (pTable)
	{
		// 取消当前选择
		pTable->clearSelection();
	}
	return QPushButton::mousePressEvent(e);
}
