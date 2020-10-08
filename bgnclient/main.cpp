#include "desktoprecorder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DesktopRecorder* recorder = new DesktopRecorder;
    recorder->setModal(true);
    recorder->show();
    return a.exec();
}
