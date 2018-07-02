#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client;
    client.setWindowIcon(QIcon("client.png"));
    client.show();

    return a.exec();
}
