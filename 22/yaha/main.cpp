#include "yaha.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    yaha w;
    w.show();

    return a.exec();
}
