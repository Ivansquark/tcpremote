#include "client.h"

Client::Client()
{
    init();
}
Client::~Client() {
    delete client;
    delete state;
    delete timer;
    delete mouse;
    delete keyboard;
}

const QString &Client::getState()
{
    return *state;
}

void Client::init() {
    state = new QString("Disconnected");
    img = new QImage;
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout, this, &Client::timeOut);
    client = new QTcpSocket(this);
    connect(client,&QTcpSocket::connected,this,&Client::isConnected);
    connect(client,&QTcpSocket::readyRead,this,&Client::readFromServer);
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(client,&QTcpSocket::disconnected,[this]() {
        *state="Disconnected";
        IsConnected=false;
        emit connectionSignal();
        timer->stop();
    });
    mouse = new Mouse;
    keyboard = new Keyboard;
}

void Client::connectToServer()
{
    //client->connectToHost("10.0.2.15",55555);
    client->connectToHost("169.254.13.88",55555);
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
    timer->start(100);
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

void Client::timeOut()
{
    if((client->state() == QAbstractSocket::ConnectedState)) {
        who = keyboard->whatKeyPressed();
        QByteArray arr;
        arr.append(who);
        if(!who) { //клавиатура не нажималась => отсылаем мышь
            arr.append(mouse->leftButPressed);
            mouse->getPos();
            for(uint8_t i=0; i<sizeof(Mouse::Pos); i++) {
                arr.append(*(reinterpret_cast<uint8_t*>(&mouse->pos)+i));
            }
        } else { //была нажата кнопка клавиатуры
            //ничего не делаем массив уже заполнен номером нажатой клавиши
        }        
        sendToServer(arr);
    }
}


void Client::sendToServer(QByteArray data) {
    if(client->state() == QAbstractSocket::ConnectedState) {
        client->write(data);
    }
}

