#pragma once

#include <QtWidgets/QWidget>
#include "ui_notetool.h"

class NoteTool : public QWidget
{
    Q_OBJECT

public:
    NoteTool(QWidget *parent = nullptr);
    ~NoteTool();

private:
    Ui::NoteToolClass ui;
};
