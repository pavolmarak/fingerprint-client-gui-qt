#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    QObject::connect(&(this->socket), SIGNAL(connected()), this, SLOT(connectedSlot()));
    QObject::connect(&(this->socket), SIGNAL(readyRead()), this, SLOT(readSlot()));
    QObject::connect(&(this->socket), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSlot(QAbstractSocket::SocketError)));
    QObject::connect(&(this->socket), SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    QObject::connect(&(this->socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(changedSlot(QAbstractSocket::SocketState)));
}

Client::~Client()
{
    delete ui;
}

void Client::on_connect_button_clicked()
{
    this->socket.connectToHost(QHostAddress(ui->remote_ip->text()),ui->remote_port->text().toUInt());
}

void Client::connectedSlot()
{
    qDebug() << "Connection established to " << this->socket.peerAddress().toString();
    ui->statusBar->showMessage("Connection established to " + this->socket.peerAddress().toString());
    ui->local_ip->setText(this->socket.localAddress().toString());
    ui->local_port->setText(QString::number(this->socket.localPort()));
    ui->local_socket->setText(QString::number(this->socket.socketDescriptor()));
}

void Client::readSlot()
{
    QByteArray message = this->socket.readAll();
    qDebug() << "Message from server: " << message;
}

void Client::errorSlot(QAbstractSocket::SocketError error)
{
    qDebug() << this->socket.errorString();
    ui->statusBar->showMessage(this->socket.errorString());
}

void Client::disconnectedSlot()
{
    qDebug() << "Disconnected from server.";
    ui->statusBar->showMessage("Disconnected from server.");
    ui->local_ip->setText("");
    ui->local_port->setText("");
    ui->local_socket->setText("");
}

void Client::changedSlot(QAbstractSocket::SocketState)
{
    qDebug() << "State changed.";
    ui->statusBar->showMessage("State changed.");
}

void Client::on_disconnect_button_clicked()
{
    if(this->socket.state() == QTcpSocket::ConnectedState){
        this->socket.disconnectFromHost();
    }

}

void Client::on_suprema_scan_button_clicked()
{
    if(this->socket.state()!=QTcpSocket::ConnectedState){
        qWarning() << "No TCP connection to server.";
        ui->suprema_log->append("No TCP connection to server.");
        return;
    }
    // ***** SENSOR PART *****

    ui->suprema_fingerprint_img->setText("Put your finger on the scanner.");
    ui->suprema_fingerprint_img->setAlignment(Qt::AlignCenter);
    qApp->processEvents();
    UFS_STATUS result_op;
    char* errStr = (char*)calloc(1000,sizeof(char));
    result_op = UFS_Init();
    if(result_op == UFS_OK){
        qDebug() << "Scanner initialized.";
        int scannerID=0;
        HUFScanner hscanner;
        result_op = UFS_GetScannerHandle(scannerID,&hscanner);
        if(result_op == UFS_OK){
            qDebug() << "Scanner handle obtained.";
            result_op = UFS_CaptureSingleImage(hscanner);
            if(result_op == UFS_OK){
                qDebug() << "Image captured.";
                int pwidth, pheight, presolution;
                UFS_GetCaptureImageBufferInfo(hscanner,&pwidth,&pheight,&presolution);
                qDebug() << pwidth << " " << pheight;
                unsigned char * pdata = (unsigned char*)malloc(pwidth*pheight*sizeof(unsigned char));
                UFS_GetCaptureImageBuffer(hscanner, pdata);
                QImage fing_img(pdata,pwidth,pheight,QImage::Format_Grayscale8);
                ui->suprema_fingerprint_img->setPixmap(QPixmap::fromImage(fing_img));
                this->socket.write((const char*)pdata, pwidth*pheight*sizeof(unsigned char));
                free(pdata);
                ui->save_image_button->setEnabled(true);
                ui->suprema_log->append("Last scan: "
                                        + QDateTime::currentDateTime().toString("dd. MMM. yyyy")
                                        + ", "
                                        + QDateTime::currentDateTime().time().toString());
            }
            else{
                UFS_GetErrorString(result_op,errStr);
                qWarning() << errStr;
            }
        }
        else{
            UFS_GetErrorString(result_op,errStr);
            qWarning() << errStr;
        }
        UFS_Uninit();
    }
    else{
        UFS_GetErrorString(result_op,errStr);
        qWarning() << errStr;
    }
    free(errStr);
}

void Client::on_save_image_button_clicked()
{
    qDebug() << ui->suprema_fingerprint_img->grab().save(QFileDialog::getSaveFileName(nullptr,"Save image as"));
}
