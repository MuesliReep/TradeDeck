#include "mainwindow.h"
#include <QApplication>

#include "Downloader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    // w.show();

    Downloader d;
    d.doDownload();

    return a.exec();
}
