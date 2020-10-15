#ifndef SERVERBACKEND_H
#define SERVERBACKEND_H

//#include <QObject>
#include <QTimer>
#include <QNetworkInterface>
#include <QImage>
#include <QtNetwork>
//#include <X11/extensions/XTest.h>
#include "desktoprecorder.h"

class ServerBackend: public QObject {
    Q_OBJECT
public:
    explicit ServerBackend(QObject* parent = nullptr);
    ~ServerBackend();
signals:
    void sendIP(const QString&);
public slots:
    void initSlot();
    void createServerSlot();
private slots:
    void newConnection();
    void clientDisconnected();
    void timeOut();
    /*!< читаем от клиента события от мыши и клавиатуры >*/
    void readClient();
private:
    QTcpServer* Ser{nullptr};
    QTcpSocket* Client{nullptr};
    QNetworkInterface* networkSession{nullptr};
    QTimer* timer{nullptr};
    DesktopRecorder* recorder{nullptr};
    QString* IPs{nullptr};

    //void init();
    /*!< передаем клиенту картинки с отображением рабочего стола >*/
    void sendToClient(QTcpSocket* sock, QImage* img);
    void sendEvtMouseLeftClick(const QPoint& pos);

    static constexpr uint16_t Port = 55555;
};

#endif // SERVERBACKEND_H
