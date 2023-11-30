#pragma once

#include <QPushButton>
#include "ui_cellbtn.h"

class CellBtn : public QPushButton
{
	Q_OBJECT

public:
	CellBtn(int row, QWidget* parent = nullptr);
	~CellBtn();

	void mousePressEvent(QMouseEvent* e) override;
	int GetRow() { return m_row; }

signals:
	void ShowSentence();

private:
	Ui::CellBtnClass ui;
	int m_row;
};
