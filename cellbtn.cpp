#include "cellbtn.h"

CellBtn::CellBtn(int row, QWidget* parent)
	: QPushButton(parent),
	m_row(row)
{
	ui.setupUi(this);
	connect(this, &CellBtn::clicked, this, [this]()
		{
			emit ShowSentence(m_row);
		});
}

CellBtn::~CellBtn()
{}
