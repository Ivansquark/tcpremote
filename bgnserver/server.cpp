#include "server.hpp"

server::server(QWidget* parent):QDialog(parent) {
    init();
}

server::~server() {
    delete thread;
}

void server::init() {
    setGeometry(0,0,200,200);
    VertLayout = new QVBoxLayout(this);
    ButExit = new QPushButton("Exit",this);    
    connect(ButExit,&QPushButton::clicked,this,&server::onButExit);
    ButCreate = new QPushButton("Create",this);
    connect(ButCreate,&QPushButton::clicked,this,&server::onButCreate);
    LabCreate = new QLabel("",this);
    LabState = new QLabel("Disconnected",this);
    IPconnected = new QLabel("",this);
    VertLayout->addWidget(LabCreate);
    VertLayout->addWidget(LabState);
    VertLayout->addWidget(IPconnected);
    VertLayout->addWidget(ButCreate);
    VertLayout->addWidget(ButExit);
    setLayout(VertLayout);
    backend = new ServerBackend;
    connect(this,&server::createSignal,backend,&ServerBackend::createServerSlot);
    connect(backend,&ServerBackend::sendIP,this,&server::setIP);
    thread = new QThread;
    connect(thread,&QThread::started,backend,&ServerBackend::initSlot);
}

void server::onButCreate() {

    backend->moveToThread(thread);
    thread->start();
    thread->setPriority(QThread::LowPriority);
    emit createSignal();
    qDebug()<<"onButCreate"<<QThread::currentThread();
}

void server::setIP(const QString& IPs) {
    IPconnected->setText(IPs);
}

void server::onButExit() {
    thread->quit();
    thread->wait();
    close();
}
