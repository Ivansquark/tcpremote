#ifndef DESKTOPRECORDER_H
#define DESKTOPRECORDER_H

#include <QtCore>
#include <QScreen>
#include <QDesktopWidget>
#include <QDialog>
#include <QTimer>
#include <QVarLengthArray>
#include <QPainter>
#include <QGuiApplication>
#include <QApplication>
#include "X11/extensions/Xfixes.h"
#include "QX11Info"

class DesktopRecorder: public QObject
{
    Q_OBJECT
public:
    DesktopRecorder(QObject* parent = nullptr);
    ~DesktopRecorder();
    /*!< структура хранит изображение иконки курсора а также позицию верхнего левого
     *   угла, где курсор должен быть отрисован >*/
    struct Cursor {
        QImage img;
        QPoint pos;
        QVarLengthArray<quint32> buf;
    };

    QImage& getImage();

private slots:
    void timeOut();
private:
    QTimer* timer{nullptr};
    QImage* currentImage{nullptr};
    /*!< захват и копирование текущего курсора >*/
    Cursor captureCursor() const;

    bool m_cursorCaptureEnabled{false};

    void init();
    void enableCursorCapture(bool enabled =true);
    QImage make_screenshot() const;
};

#endif // DESKTOPRECORDER_H
