#pragma once

#include <QtWidgets/QWidget>
#include "ui_notetool.h"
#include <QStandardItemModel>
class QMenuBar;
class NoteTool : public QWidget
{
    Q_OBJECT

public:
    NoteTool(QWidget *parent = nullptr);
    ~NoteTool();
    void Init();


private:
    Ui::NoteToolClass ui;
};
