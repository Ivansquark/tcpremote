#ifndef CLIENT_H
#define CLIENT_H

#include "mouse.h"
#include "keyboard.h"

#include <QtNetwork>
#include <QDataStream>
#include <QTcpSocket>
#include <QObject>
#include <QImage>
#include <QTimer>


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
    bool IsConnected{false};
signals:
    void connectionSignal();

private slots:
    void isConnected();
    void readFromServer();
    void slotError(QAbstractSocket::SocketError err);
    /*!< передем серверу протокол событий от мыши и клавиатуры >*/
    void timeOut();
private:
    QTcpSocket* client{nullptr};
    QString* state{nullptr};    
    QTimer* timer{nullptr};
    Mouse* mouse{nullptr};
    Keyboard* keyboard{nullptr};

    uint8_t who=0;

    void init();
};

#endif // CLIENT_H
