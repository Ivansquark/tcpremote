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
    void onButRecord();
    void newConnection();
    void clientDisconnected();
    void onButSend();
    void timeOut();
    /*!< читаем от клиента события от мыши и клавиатуры >*/
    QByteArray readClient();
private:
    QTcpServer* Ser{nullptr};
    QTcpSocket* Client{nullptr};
    QNetworkInterface* networkSession{nullptr};
    QVBoxLayout* VertLayout{nullptr};
    QPushButton* ButExit{nullptr};
    QPushButton* ButCreate{nullptr};
    QPushButton* ButRecord{nullptr};
    QPushButton* ButSend{nullptr};
    QLabel* LabCreate{nullptr};
    QLabel* LabState{nullptr};
    QLabel* IPconnected{nullptr};
    QByteArray* ReceivedData{nullptr};
    DesktopRecorder* recorder{nullptr};
    QTimer* timer{nullptr};

    void init();    
    /*!< передаем клиенту картинки с отображением рабочего стола >*/
    void sendToClient(QTcpSocket* sock, QImage* img);

    static constexpr uint16_t Port = 55555;
};

#endif // SERVER_HPP
