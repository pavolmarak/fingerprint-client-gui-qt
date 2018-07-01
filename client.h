#ifndef DBOXCLIENT_H
#define DBOXCLIENT_H

#include <QMainWindow>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "UFScanner.h"
#include "opencv2/world.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

namespace Ui {
class DboxClient;
}

class DboxClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit DboxClient(QWidget *parent = 0);
    ~DboxClient();

public slots:
    void connectedSlot();
    void readSlot();
    void errorSlot(QAbstractSocket::SocketError);
    void disconnectedSlot();
    void changedSlot(QAbstractSocket::SocketState);

private slots:
    void on_connect_button_clicked();

private:
    Ui::DboxClient *ui;
    QTcpSocket socket;
};

#endif // DBOXCLIENT_H
