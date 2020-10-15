#include "server.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    server s;
    s.show();
    return a.exec();
}
