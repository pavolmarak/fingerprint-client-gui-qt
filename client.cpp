#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    if(this->scannerInit(this->hscanner)==false){
        qDebug() << "Error during scanner init.";
        exit(-1);
    }
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
    int width, height;
    unsigned char* img_data = this->scannerCapture(width,height);
    if(img_data == nullptr){
        qDebug() << "Scanner error.";
        ui->suprema_log->append("Scanner error.");
        return;
    }
    QImage fing_img(img_data,width,height,QImage::Format_Grayscale8);
    ui->suprema_fingerprint_img->setPixmap(QPixmap::fromImage(fing_img));

    QByteArray byteCount;
    QByteArray imgWidth, imgHeight;
    QByteArray serverWindowID;

    QDataStream dataStream1(&byteCount, QIODevice::WriteOnly);
    QDataStream dataStream2(&imgWidth, QIODevice::WriteOnly);
    QDataStream dataStream3(&imgHeight, QIODevice::WriteOnly);
    QDataStream dataStream4(&serverWindowID, QIODevice::WriteOnly);

    quint32 bc = SUPREMA_IMG_SIZE+CONTROL_DATA_SIZE;
    quint32 iw = SUPREMA_IMG_WIDTH;
    quint32 ih = SUPREMA_IMG_HEIGHT;
    quint32 swid = 1;

    dataStream1 << bc;
    dataStream2 << iw;
    dataStream3 << ih;
    dataStream4 << swid;

    this->socket.write(byteCount); // number of bytes
    this->socket.write(imgWidth); // image width
    this->socket.write(imgHeight); // image height
    this->socket.write(serverWindowID); // server window ID

    this->socket.write((const char*)img_data,sizeof(unsigned char)*width*height);
    free(img_data);
    ui->save_image_button->setEnabled(true);
}

void Client::on_suprema_scan_button2_clicked()
{
    if(this->socket.state()!=QTcpSocket::ConnectedState){
        qWarning() << "No TCP connection to server.";
        ui->suprema_log->append("No TCP connection to server.");
        return;
    }
    int width, height;
    unsigned char* img_data = this->scannerCapture(width,height);
    if(img_data == nullptr){
        qDebug() << "Scanner error.";
        ui->suprema_log->append("Scanner error.");
        return;
    }
    QImage fing_img(img_data,width,height,QImage::Format_Grayscale8);
    ui->suprema_fingerprint_img2->setPixmap(QPixmap::fromImage(fing_img));

    QByteArray byteCount;
    QByteArray imgWidth, imgHeight;
    QByteArray serverWindowID;

    QDataStream dataStream1(&byteCount, QIODevice::WriteOnly);
    QDataStream dataStream2(&imgWidth, QIODevice::WriteOnly);
    QDataStream dataStream3(&imgHeight, QIODevice::WriteOnly);
    QDataStream dataStream4(&serverWindowID, QIODevice::WriteOnly);

    quint32 bc = SUPREMA_IMG_SIZE+CONTROL_DATA_SIZE;
    quint32 iw = SUPREMA_IMG_WIDTH;
    quint32 ih = SUPREMA_IMG_HEIGHT;
    quint32 swid = 2;

    dataStream1 << bc;
    dataStream2 << iw;
    dataStream3 << ih;
    dataStream4 << swid;

    this->socket.write(byteCount); // number of bytes
    this->socket.write(imgWidth); // image width
    this->socket.write(imgHeight); // image height
    this->socket.write(serverWindowID); // server window ID

    this->socket.write((const char*)img_data,sizeof(unsigned char)*width*height);
    free(img_data);
    ui->save_image_button2->setEnabled(true);

}

void Client::on_save_image_button_clicked()
{
    qDebug() << ui->suprema_fingerprint_img->grab().save(QFileDialog::getSaveFileName(nullptr,"Save image as"));
}

void Client::on_save_image_button2_clicked()
{
    qDebug() << ui->suprema_fingerprint_img2->grab().save(QFileDialog::getSaveFileName(nullptr,"Save image as"));
}

bool Client::scannerInit(HUFScanner& hscanner)
{
    if(UFS_Init()==UFS_OK){
        qDebug() << "Suprema BioMini Slim initialized successfully.";
        ui->suprema_log->append("Suprema BioMini Slim initialized successfully.");
        int scannerNumber;
        UFS_GetScannerNumber(&scannerNumber);
        if(scannerNumber==0){
            return false;
        }
        qDebug() << "Number of scanners: " << scannerNumber;
        if(UFS_GetScannerHandle(scannerNumber-1,&hscanner)== UFS_OK){
            qDebug() << "Scanner handle obtained.";
            ui->suprema_log->append("Scanner handle obtained.");
            return true;
        }
        else{
            qDebug() << "SCANNER HANDLE: Fail.";
            ui->suprema_log->append("SCANNER HANDLE: Fail.");
        }
    }
    else{
        qDebug() << "INIT: Fail.";
        ui->suprema_log->append("INIT: Fail.");
    }
    return false;
}

unsigned char* Client::scannerCapture(int& width, int& height)
{
    int status;
    char* errStr = (char*)calloc(1000,sizeof(char));
    if((status=UFS_CaptureSingleImage(this->hscanner)) == UFS_OK){
        qDebug() << "Image captured.";
        ui->suprema_log->append("Image captured.");
        int resolution;
        UFS_GetCaptureImageBufferInfo(this->hscanner,&width,&height,&resolution);
        qDebug() << width << " x " << height;
        ui->suprema_log->append(QString::number(width) + " x " + QString::number(height));
        unsigned char * pdata = (unsigned char*)malloc(width*height*sizeof(unsigned char));
        UFS_GetCaptureImageBuffer(this->hscanner, pdata);
        free(errStr);
        return pdata;
    }
    else{
        UFS_GetErrorString(status,errStr);
        qDebug() << errStr;
        ui->suprema_log->append(errStr);
    }
    free(errStr);
    return nullptr;
}

void Client::on_load_image_button_clicked()
{
    if(this->socket.state()!=QTcpSocket::ConnectedState){
        qWarning() << "No TCP connection to server.";
        ui->suprema_log->append("No TCP connection to server.");
        return;
    }
    QString opnFile = QFileDialog::getOpenFileName(nullptr,"Please select a fingerprint image");
    if(opnFile==""){
        qDebug() << "No file selected.";
        return;
    }
    QImage opnImage(opnFile);
    ui->suprema_fingerprint_img->setPixmap(QPixmap::fromImage(opnImage));

    QByteArray byteCount;
    QByteArray imgWidth, imgHeight;
    QByteArray serverWindowID;

    QDataStream dataStream1(&byteCount, QIODevice::WriteOnly);
    QDataStream dataStream2(&imgWidth, QIODevice::WriteOnly);
    QDataStream dataStream3(&imgHeight, QIODevice::WriteOnly);
    QDataStream dataStream4(&serverWindowID, QIODevice::WriteOnly);

    quint32 bc = opnImage.byteCount()+CONTROL_DATA_SIZE;
    quint32 iw = opnImage.width();
    quint32 ih = opnImage.height();
    quint32 swid = 1;

    dataStream1 << bc;
    dataStream2 << iw;
    dataStream3 << ih;
    dataStream4 << swid;

    this->socket.write(byteCount); // number of bytes
    this->socket.write(imgWidth); // image width
    this->socket.write(imgHeight); // image height
    this->socket.write(serverWindowID); // server window ID

    this->socket.write((const char*)opnImage.bits(),sizeof(unsigned char)*opnImage.width()*opnImage.height());
    ui->save_image_button->setEnabled(true);
}

void Client::on_load_image_button2_clicked()
{
    if(this->socket.state()!=QTcpSocket::ConnectedState){
        qWarning() << "No TCP connection to server.";
        ui->suprema_log->append("No TCP connection to server.");
        return;
    }
    QString opnFile = QFileDialog::getOpenFileName(nullptr,"Please select a fingerprint image");
    if(opnFile==""){
        qDebug() << "No file selected.";
        return;
    }
    QImage opnImage(opnFile);
    ui->suprema_fingerprint_img2->setPixmap(QPixmap::fromImage(opnImage));

    QByteArray byteCount;
    QByteArray imgWidth, imgHeight;
    QByteArray serverWindowID;

    QDataStream dataStream1(&byteCount, QIODevice::WriteOnly);
    QDataStream dataStream2(&imgWidth, QIODevice::WriteOnly);
    QDataStream dataStream3(&imgHeight, QIODevice::WriteOnly);
    QDataStream dataStream4(&serverWindowID, QIODevice::WriteOnly);

    quint32 bc = opnImage.byteCount()+CONTROL_DATA_SIZE;
    quint32 iw = opnImage.width();
    quint32 ih = opnImage.height();
    quint32 swid = 2;

    dataStream1 << bc;
    dataStream2 << iw;
    dataStream3 << ih;
    dataStream4 << swid;

    this->socket.write(byteCount); // number of bytes
    this->socket.write(imgWidth); // image width
    this->socket.write(imgHeight); // image height
    this->socket.write(serverWindowID); // server window ID

    this->socket.write((const char*)opnImage.bits(),sizeof(unsigned char)*opnImage.width()*opnImage.height());
    ui->save_image_button2->setEnabled(true);
}
