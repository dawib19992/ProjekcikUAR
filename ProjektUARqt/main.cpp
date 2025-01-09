#include "ukladautomatycznejregulacji.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UkladAutomatycznejRegulacji w;
    w.show();
    return a.exec();
}
