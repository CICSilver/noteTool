#pragma once

#include <QDialog>
#include "ui_datedialog.h"

class DateDialog : public QDialog
{
	Q_OBJECT

public:
	DateDialog(QWidget *parent = nullptr);
	~DateDialog();

private:
	Ui::DateDialogClass ui;
};
