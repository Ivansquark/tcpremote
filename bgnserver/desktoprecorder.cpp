#include "desktoprecorder.h"

DesktopRecorder::DesktopRecorder(QObject* parent): QObject(parent)
{
    init();
    //timer->setSingleShot(true);
    timer->start(100);
}
DesktopRecorder::~DesktopRecorder() {
    delete timer;
    delete currentImage;
}

QImage& DesktopRecorder::getImage()
{
    /*!< возврашаем адрес объекта в куче содершащий скриншот рабочего стола >*/
    return *currentImage;
}

void DesktopRecorder::stopRecord()
{
    timer->stop();
}

void DesktopRecorder::init() {
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&DesktopRecorder::timeOut);
    currentImage = new QImage;
    enableCursorCapture(true);
}

QImage DesktopRecorder::make_screenshot() const
{
    QDesktopWidget* desktop = QApplication::desktop();
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geom = screen->geometry();
    QScreen* scr = QApplication::primaryScreen();
    auto pix = scr->grabWindow(desktop->winId(),geom.x(),geom.y(),geom.width(),geom.height());
    if(m_cursorCaptureEnabled) {
        Cursor cursor = captureCursor();
        if(!cursor.img.isNull()) {
            QPainter p;
            p.begin(&pix);
            p.drawImage(cursor.pos, cursor.img);
            p.end();
        }
    }
    return pix.toImage();
}

void DesktopRecorder::enableCursorCapture(bool enabled)
{
    m_cursorCaptureEnabled = enabled;
}

void DesktopRecorder::timeOut()
{
    /*!< копируем рисунок из стека в кучу и перемасштабируем его для уменьшения размера >*/
    *currentImage = make_screenshot().scaled(800,600,Qt::KeepAspectRatio,Qt::SmoothTransformation);
}

DesktopRecorder::Cursor DesktopRecorder::captureCursor() const
{
    Cursor cursor;
        /*!< создаем объект, хранящий параметры текущего курсора >*/
        if(XFixesCursorImage* curImage = XFixesGetCursorImage(QX11Info::display())) {
            cursor.buf.resize(curImage->width * curImage->height);
            /*!< заполняем буфер пикселями курсора >*/
            for(int i=0; i<cursor.buf.size();i++) {
                cursor.buf[i] = curImage->pixels[i]&0xFFFFFFFF;
            }
            /*!< заполняем изображение курсора >*/
            cursor.img = QImage(
                        reinterpret_cast<const uchar*>(cursor.buf.data()),
                        curImage->width,
                        curImage->height,
                        QImage::Format_ARGB32_Premultiplied
                        );
            /*!< вычитаем из текущей позиции курсора hotspot within cursor >*/
            cursor.pos = QCursor::pos() - QPoint(curImage->xhot,curImage->yhot);
            XFree(curImage); //free allocated memory
        }
        return cursor;
}
