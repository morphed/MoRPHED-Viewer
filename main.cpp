#include "viewer_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Viewer_MainWindow w;
    w.show();

    return a.exec();
}
