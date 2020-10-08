#include "client.h"

Client::Client()
{
    init();
}
Client::~Client() {
    delete client;
    delete state;
}

const QString &Client::getState()
{
    return *state;
}

void Client::init() {
    state = new QString("Disconnected");
    img = new QImage;
    client = new QTcpSocket(this);
    connect(client,&QTcpSocket::connected,this,&Client::isConnected);
    connect(client,&QTcpSocket::readyRead,this,&Client::readFromServer);
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(client,&QTcpSocket::disconnected,[this](){*state="Disconnected";IsConnected=false;emit connectionSignal();});
}

void Client::connectToServer()
{
    client->connectToHost("10.0.2.15",55555);
}

void Client::disconnectFromServer()
{
    if(client->state() == QAbstractSocket::ConnectedState) {
        client->disconnectFromHost();
    }
}


void Client::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                                            "The host was not found." :
                                            err == QAbstractSocket::RemoteHostClosedError ?
                                                "The remote host is closed." :
                                                err == QAbstractSocket::ConnectionRefusedError?
                                                    "The connection was refused." :
                                                    QString(client->errorString()));
        state->append(strError);
}

void Client::isConnected() {
    *state = "Connected";
    IsConnected = true;
    emit connectionSignal();
}

void Client::readFromServer()
{
    QDataStream in(client);
        in.setVersion(QDataStream::Qt_5_12);
        //initialize data
        QImage image;
        static quint32 imageSize = 0;
        if (0 == imageSize){
            if(client->bytesAvailable() < (qint64)sizeof(quint32)) {
                return; //количество пришедших байт меньше чем переменная в которой записан размер
            }
            in>>imageSize; //получаем размер
            //qDebug()<<"image size = "<<imageSize;
        }
        if(client->bytesAvailable() <imageSize) {
            return; //ждем пока наполнится сокет
        }
        in >> image; //записываем изображение из потока
        imageSize = 0; //обнуляем размер когда все записалось
        if(image.isNull()) {
            qDebug()<<"Image is null";
        } else {
            *img = image;
        }
}

void Client::sendToServer(QByteArray) {

}

