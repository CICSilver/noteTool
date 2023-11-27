#include "notetool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NoteTool w;
    w.show();
    return a.exec();
}
