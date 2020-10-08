#ifndef DESKTOPRECORDER_H
#define DESKTOPRECORDER_H

#include "client.h"

#include <QScreen>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QVarLengthArray>
#include <QPainter>
#include <QGuiApplication>

#include <X11/extensions/Xfixes.h>
#include <QX11Info>
//#include <QtX11Extras/QX11Info>


namespace Ui {
class DesktopRecorder;
}

class DesktopRecorder : public QDialog
{
    Q_OBJECT

public:
    explicit DesktopRecorder(QWidget *parent = nullptr);
    ~DesktopRecorder();
    /*!< структура хранит изображение иконки курсора а также позицию верхнего левого
     *   угла, где курсор должен быть отрисован >*/
    struct Cursor {
        QImage img;
        QPoint pos;
        QVarLengthArray<quint32> buf;
    };

private slots:
    void on_pushButton_clicked();
    /*!< ScreenShot button >*/
    void on_pushButton_2_clicked();
    void onButConnect();
    void onButDisonnect();
    void timeOut();

private:
    Ui::DesktopRecorder *ui;
    QLabel* lab{nullptr};
    QLabel* LabState{nullptr};
    QPushButton* butConnect{nullptr};
    QPushButton* butDisconnect{nullptr};
    QGraphicsScene* scene{nullptr};
    QGraphicsView* view{nullptr};
    QTimer* timer{nullptr};
    Client* client{nullptr};

    void init();
    QImage make_screenshot() const;
};

#endif // DESKTOPRECORDER_H
