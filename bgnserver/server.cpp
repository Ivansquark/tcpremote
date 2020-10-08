#include "server.hpp"

server::server(QWidget* parent):QDialog(parent)
{
    init();
}

server::~server() {
    delete timer;
    delete recorder;
}

void server::init() {
    setGeometry(0,0,200,200);
    VertLayout = new QVBoxLayout(this);
    ButExit = new QPushButton("Exit",this);    
    connect(ButExit,&QPushButton::clicked,this,&server::onButExit);
    ButCreate = new QPushButton("Create",this);
    connect(ButCreate,&QPushButton::clicked,this,&server::onButCreate);
    ButRecord = new QPushButton("Record",this);
    connect(ButRecord,&QPushButton::clicked,this,&server::onButRecord);
    ButSend = new QPushButton("Send",this);
    connect(ButSend,&QPushButton::clicked,this,&server::onButSend);
    LabCreate = new QLabel("",this);
    LabState = new QLabel("Disconnected",this);
    IPconnected = new QLabel("",this);
    VertLayout->addWidget(LabCreate);
    VertLayout->addWidget(LabState);
    VertLayout->addWidget(IPconnected);
    VertLayout->addWidget(ButCreate);
    VertLayout->addWidget(ButRecord);
    VertLayout->addWidget(ButSend);
    VertLayout->addWidget(ButExit);
    setLayout(VertLayout);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&server::timeOut);
}

void server::onButCreate() {
    Ser = new QTcpServer(this);
    connect(Ser,&QTcpServer::newConnection,this,&server::newConnection);
    if(!Ser->listen(QHostAddress::Any,Port)) {
        QMessageBox::critical(this,"Warning","Server not created" + Ser->errorString());
        Ser->close();
        return;
    } else {
        LabCreate->setText("Created on port: " + QString::number(Port));
    }
    QList<QHostAddress> listIP = QNetworkInterface::allAddresses();
    QString IPs;
    for(int i=0; i<listIP.count();i++) {
        if(!listIP[i].isLoopback()) {
            if(listIP[i].protocol() == QAbstractSocket::IPv4Protocol) {
                IPs += listIP[i].toString() + " | ";
            }
        }
    }
    IPconnected->setText(IPs);
}

void server::newConnection()
{
    LabState->setText("Connected");
    /*!< указываем клиентский сокет сервера на подключенного клиента >*/
    Client = Ser->nextPendingConnection();
    connect(Client,&QTcpSocket::disconnected,this, &server::clientDisconnected);
    connect(Client,&QTcpSocket::readyRead,this,&server::readClient);
    ReceivedData = new QByteArray;
}

void server::sendToClient(QTcpSocket *sock, QImage *img)
{
    if(sock->state() == QTcpSocket::ConnectedState ) {
        QByteArray byteArr;
        QDataStream out(&byteArr,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_12);
        out<<quint32(0)<<*img; // записываем размер изображения и само изображение
        out.device()->seek(0); //переходим на начало потока
        qDebug()<<"Size = "<<(quint32*)(byteArr.size() - sizeof(quint32));
        out<<(quint32)(byteArr.size() - sizeof(uint32_t)); //записываем в начало пакета его размер
        quint32 written = sock->write(byteArr);
        sock->waitForBytesWritten(-1);
        qDebug()<<"written "<<written<<" of " <<byteArr.size();
    }
}

QByteArray server::readClient() {
    QTcpSocket* clientSock = static_cast<QTcpSocket*>(QObject::sender()); //данные от клиента
    QDataStream in(clientSock);
    in.setVersion(QDataStream::Qt_5_12);
    QByteArray receivedArr;
    for(uint16_t i=0; i<clientSock->bytesAvailable();i++) {
        in>>receivedArr;
    }
    return receivedArr;
}

void server::clientDisconnected() {
    LabState->setText("Disconnected");
    Client->deleteLater();
}

void server::onButSend()
{
    timer->start(100);
}

void server::timeOut()
{
    QImage& img = recorder->getImage();
    sendToClient(Client,&img);
}

void server::onButRecord() {
    recorder = new DesktopRecorder;
}

void server::onButExit() {
    close();
}
