#include "dboxclient.h"
#include "ui_dboxclient.h"

DboxClient::DboxClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DboxClient)
{
    ui->setupUi(this);
    QObject::connect(&(this->socket), SIGNAL(connected()), this, SLOT(connectedSlot()));
    QObject::connect(&(this->socket), SIGNAL(readyRead()), this, SLOT(readSlot()));
    QObject::connect(&(this->socket), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSlot(QAbstractSocket::SocketError)));
    QObject::connect(&(this->socket), SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    QObject::connect(&(this->socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(changedSlot(QAbstractSocket::SocketState)));
}

DboxClient::~DboxClient()
{
    delete ui;
}

void DboxClient::on_connect_button_clicked()
{
    this->socket.connectToHost(QHostAddress(ui->remote_ip->text()),ui->remote_port->text().toUInt());
}

void DboxClient::connectedSlot()
{
    qDebug() << "Connection established to " << this->socket.peerAddress().toString();
    ui->statusBar->showMessage("Connection established to " + this->socket.peerAddress().toString());
    this->socket.write("Hello from client");

// ***** SENSOR PART *****

//    UFS_STATUS result_op;
//    result_op = UFS_Init();
//    if(result_op == UFS_OK){
//        qDebug() << "Scanner initialized.";
//        int scannerID=0;
//        HUFScanner hscanner;
//        result_op = UFS_GetScannerHandle(scannerID,&hscanner);
//        if(result_op == UFS_OK){
//            qDebug() << "Scanner handle obtained.";
//            result_op = UFS_CaptureSingleImage(hscanner);
//            if(result_op == UFS_OK){
//                qDebug() << "Image captured.";
//                int pwidth, pheight, presolution;
//                UFS_GetCaptureImageBufferInfo(hscanner,&pwidth,&pheight,&presolution);
//                qDebug() << pwidth << " " << pheight;
//                unsigned char * pdata = (unsigned char*)malloc(pwidth*pheight*sizeof(unsigned char));
//                UFS_GetCaptureImageBuffer(hscanner, pdata);
//                cv::Mat imgcv(pheight, pwidth, CV_8UC1, pdata, pwidth);
//                cv::imwrite("fingerprint.png",imgcv);
//                UFS_Uninit();
//            }
//            else{
//                qWarning() << "Image capturing failed.";
//            }
//        }
//        else{
//            qWarning() << "Scanner handle retrieval failed.";
//        }

//    }
//    else{
//        qWarning() << "Scanner is not initialized";
//    }

}

void DboxClient::readSlot()
{
    QByteArray message = this->socket.readAll();
    qDebug() << "Message from server: " << message;
}

void DboxClient::errorSlot(QAbstractSocket::SocketError error)
{
    qDebug() << this->socket.errorString();
    ui->statusBar->showMessage(this->socket.errorString());
}

void DboxClient::disconnectedSlot()
{
    qDebug() << "Disconnected from server.";
    ui->statusBar->showMessage("Disconnected from server.");
}

void DboxClient::changedSlot(QAbstractSocket::SocketState)
{
    qDebug() << "State changed.";
    ui->statusBar->showMessage("State changed.");
}
