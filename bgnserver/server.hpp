#ifndef SERVER_HPP
#define SERVER_HPP



#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QImage>
#include <QtNetwork>
#include <X11/extensions/XTest.h>
#include "desktoprecorder.h"

class server: public QDialog
{
    Q_OBJECT
public:
    explicit server(QWidget* parent = nullptr);
    ~server();
    server(const server& s) = delete;
    server* operator= (const server& s) = delete;
private slots:
    void onButExit();
    void onButCreate();
    void newConnection();
    void clientDisconnected();
    void timeOut();
    /*!< читаем от клиента события от мыши и клавиатуры >*/
    void readClient();
private:
    QTcpServer* Ser{nullptr};
    QTcpSocket* Client{nullptr};
    QNetworkInterface* networkSession{nullptr};
    QVBoxLayout* VertLayout{nullptr};
    QPushButton* ButExit{nullptr};
    QPushButton* ButCreate{nullptr};    
    QLabel* LabCreate{nullptr};
    QLabel* LabState{nullptr};
    QLabel* IPconnected{nullptr};
    DesktopRecorder* recorder{nullptr};
    QTimer* timer{nullptr};

    void init();    
    /*!< передаем клиенту картинки с отображением рабочего стола >*/
    void sendToClient(QTcpSocket* sock, QImage* img);
    void sendEvtMouseLeftClick(QPoint& pos);

    static constexpr uint16_t Port = 55555;
};

#endif // SERVER_HPP
