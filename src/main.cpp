#include "mainwindow.h"
#include <QApplication>

#include "Downloader.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Load configuration from file (if any)
    Config c;
    c.loadConfigFromFile();

    //Set up HTTP downloader
    Downloader d;
    d.setConfig(&c);
    d.doDownload();

    // TODO: pass config & downloader to gui.
    MainWindow w;
    w.show();

    return a.exec();
}
