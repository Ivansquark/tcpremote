#include "serverbackend.h"

ServerBackend::ServerBackend(QObject* parent):QObject(parent) {
    //init();
}

ServerBackend::~ServerBackend(){
    delete IPs;
}

void ServerBackend::initSlot() {
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&ServerBackend::timeOut);
    IPs = new QString;
    qDebug()<<"initSlot"<<QThread::currentThread();
}

void ServerBackend::createServerSlot() {
    Ser = new QTcpServer(this);
    connect(Ser,&QTcpServer::newConnection,this,&ServerBackend::newConnection);
    if(!Ser->listen(QHostAddress::Any,Port)) {
        //QMessageBox::critical(this,"Warning","Server not created" + Ser->errorString());
        Ser->close();
        return;
    } else {
        //LabCreate->setText("Created on port: " + QString::number(Port));
    }
    QList<QHostAddress> listIP = QNetworkInterface::allAddresses();
    for(int i=0; i<listIP.count();i++) {
        if(!listIP[i].isLoopback()) {
            if(listIP[i].protocol() == QAbstractSocket::IPv4Protocol) {
                *IPs += listIP[i].toString() + " | ";
            }
        }
    }
    emit sendIP(*IPs);
    qDebug()<<"createServerSlot"<<QThread::currentThread();
}

void ServerBackend::newConnection() {
    /*!< указываем клиентский сокет сервера на подключенного клиента >*/
    Client = Ser->nextPendingConnection();
    connect(Client,&QTcpSocket::disconnected,this, &ServerBackend::clientDisconnected);
    connect(Client,&QTcpSocket::readyRead,this,&ServerBackend::readClient);
    //connect(Client,&QTcpSocket::connected,this,&server::onConnected);
    //LabState->setText("Connected");
    /*!< создаем объект в конструкторе которого запускается съемка рабочего стола за определенное время >*/
    recorder = new DesktopRecorder;
    /*!< запускаем таймер по которому передаем картинку по TCP >*/
    timer->start(100);
}

void ServerBackend::clientDisconnected() {
    recorder->stopRecord();
    timer->stop();
    //LabState->setText("Disconnected");
    //delete Client;
    //Client = nullptr;
    Client->deleteLater();
    delete recorder;
}

void ServerBackend::timeOut() {
    if(Client->state() == QAbstractSocket::ConnectedState) {
        QImage& img = recorder->getImage();
        sendToClient(Client,&img);
    }
}

void ServerBackend::readClient() {
    qDebug()<<"readClient"<<QThread::currentThread();
    QTcpSocket* clientSock = static_cast<QTcpSocket*>(QObject::sender()); //данные от клиента
    QDataStream in(clientSock);
    in.setVersion(QDataStream::Qt_5_11);
    QByteArray receivedArr;
    uint8_t buf[20]={0};
    uint8_t bytes = clientSock->bytesAvailable();
    //qDebug()<<"bytes="<<clientSock->bytesAvailable();

    //qDebug()<<receivedArr;
    /*!< вычитываем сокет >*/
    for(uint16_t i=0; i<bytes;i++) {
        in>>buf[i];//receivedArr;
    }
    uint16_t x=0;
    uint16_t y=0;
    if(!buf[0]) { //если ноль => мышь
        uint8_t pos[4];
        for(int i=0;i<4;i++) {
            pos[i] = buf[i+2];
        }
        x = (pos[1]<<8|pos[0]);
        y = (pos[3]<<8|pos[2]);
        QCursor::setPos(QGuiApplication::primaryScreen(),x,y);
        /*!< если была нажата кнопка мыши >*/
        if(buf[1]) {
            QPoint pos(x,y);
            sendEvtMouseLeftClick(pos);
        } else {
            QCursor::setPos(x,y);
        }
        //qDebug()<< x << y ;
    } else {
        /*!< Клавиатура кнопка: 1-255 >*/
    }
    //QPoint currentCursor = QCursor::pos();
}
void ServerBackend::sendToClient(QTcpSocket *sock, QImage *img) {
    if(sock->state() == QTcpSocket::ConnectedState ) {
        QByteArray byteArr;
        QDataStream out(&byteArr,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_11);
        out<<quint32(0)<<*img; // записываем размер изображения и само изображение
        out.device()->seek(0); //переходим на начало потока
        //qDebug()<<"Size = "<<(quint32*)(byteArr.size() - sizeof(quint32));
        out<<(quint32)(byteArr.size() - sizeof(uint32_t)); //записываем в начало пакета его размер
        //quint32 written = sock->write(byteArr);
        sock->write(byteArr);
        sock->waitForBytesWritten(-1);
        //qDebug()<<"written "<<written<<" of " <<byteArr.size();
    }
}

void ServerBackend::sendEvtMouseLeftClick(const QPoint &pos) {
    QCursor::setPos(pos);
    Display* display = XOpenDisplay(nullptr);
    if(display==nullptr) {return;}
    //XTestFakeButtonEvent(display,Qt::LeftButton,1,-1);
    XFlush(display);
    XCloseDisplay(display);
}
