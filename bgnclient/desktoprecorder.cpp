#include "desktoprecorder.h"
#include "ui_desktoprecorder.h"

DesktopRecorder::DesktopRecorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DesktopRecorder)
{
    ui->setupUi(this);
    init();
}

DesktopRecorder::~DesktopRecorder()
{
    delete client;
    delete timer;
    delete ui;
}

void DesktopRecorder::init() {
    //lab = new QLabel(this);
    //lab->setGeometry(40,40,100,100);
    //lab->show();
    client = new Client;
    connect(client,SIGNAL(connectionSignal()),this,SLOT(connectedToServer()));
    LabState = new QLabel(this);
    LabState->setGeometry(1020,600,80,30);
    LabState->show();
    LabState->setText(client->getState());
    this->setGeometry(0,0,1500,900);
    butConnect = new QPushButton("Connect",this);
    butConnect->setGeometry(1200,600,70,40);
    butConnect->show();
    connect(butConnect,&QPushButton::clicked,this,&DesktopRecorder::onButConnect);
    butDisconnect = new QPushButton("Disconnect",this);
    butDisconnect->setGeometry(1200,650,70,40);
    butDisconnect->show();
    connect(butDisconnect,&QPushButton::clicked,this,&DesktopRecorder::onButDisonnect);
    view = new QGraphicsView(this);
    view->setGeometry(0,0,800,480);
    ui->pushButton->setGeometry(1200,800,100,100);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&DesktopRecorder::timeOut);

    scene = new QGraphicsScene(this);
}

void DesktopRecorder::onButConnect() {
    client->connectToServer();
}
void DesktopRecorder::onButDisonnect() {
    client->disconnectFromServer();
    LabState->setText(client->getState());
}


void DesktopRecorder::on_pushButton_clicked()
{
    close();
}


void DesktopRecorder::timeOut()
{
    //QImage im;
    //lab->setPixmap(QPixmap::fromImage(im));    
    scene->addPixmap(QPixmap::fromImage(*client->img));    
    view->setScene(scene);
    LabState->setText(client->getState());
}

void DesktopRecorder::connectedToServer()
{
    if(client->IsConnected){
        timer->start(100);
    } else {
        timer->stop();
        scene->clear();
    }
}



