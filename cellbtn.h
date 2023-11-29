#pragma once

#include <QPushButton>
#include "ui_cellbtn.h"

class CellBtn : public QPushButton
{
	Q_OBJECT

public:
	CellBtn(int row, QWidget*parent = nullptr);
	~CellBtn();

signals:
	void ShowSentence(int i);

private:
	Ui::CellBtnClass ui;
	int m_row;
};
