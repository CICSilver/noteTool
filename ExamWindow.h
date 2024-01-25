#pragma once

#include <QWidget>
#include "ui_ExamWindow.h"

class ExamWindow : public QWidget
{
	Q_OBJECT

public:
	ExamWindow(QWidget *parent = nullptr);
	~ExamWindow();

private:
	Ui::ExamWindowClass ui;
};
