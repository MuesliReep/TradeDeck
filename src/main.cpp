#include "mainwindow.h"
#include <QApplication>

#include "Downloader.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    // w.show();

    Downloader d;
    d.doDownload();

    Config c;
    c.setLastLoaded();

    return a.exec();
}
