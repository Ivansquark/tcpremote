#ifndef SERVER_HPP
#define SERVER_HPP

#include <QThread>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include "serverbackend.h"

class server: public QDialog
{
    Q_OBJECT
public:
    explicit server(QWidget* parent = nullptr);
    ~server();
    server(const server& s) = delete;
    server* operator= (const server& s) = delete;
signals:
    void createSignal();
private slots:
    void onButExit();
    void onButCreate();
    void setIP(const QString&);
private:
    QVBoxLayout* VertLayout{nullptr};
    QPushButton* ButExit{nullptr};
    QPushButton* ButCreate{nullptr};    
    QLabel* LabCreate{nullptr};
    QLabel* LabState{nullptr};
    QLabel* IPconnected{nullptr};
    ServerBackend* backend{nullptr};
    /*!< объект управляющий исполнением в новом потоке >*/
    QThread* thread{nullptr};
    void init();        
};

#endif // SERVER_HPP
