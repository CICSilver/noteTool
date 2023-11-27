#include "notetool.h"
#include <QMenuBar>
NoteTool::NoteTool(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.table->setRowCount(20);
    ui.table->setColumnCount(3);
    ui.table->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Éú´Ê") << QStringLiteral("´Ê¸ù") << QStringLiteral("½âÊÍ"));
    Init();
}

NoteTool::~NoteTool()
{}

void NoteTool::Init()
{

}
