#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QDataStream>
#include <QTcpSocket>
#include <QObject>
#include <QImage>

class Client: public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();

    const QString& getState();
    void connectToServer();
    void disconnectFromServer();
    void sendToServer(QByteArray);
    QImage* img{nullptr};
private slots:
    void isConnected();
    void readFromServer();
    void slotError(QAbstractSocket::SocketError err);
private:
    QTcpSocket* client{nullptr};
    QString* state{nullptr};    

    void init();
};

#endif // CLIENT_H
