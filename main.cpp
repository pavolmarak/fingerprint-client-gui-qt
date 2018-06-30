#include "dboxclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DboxClient w;
    w.show();

    return a.exec();
}
