#include "mainwindow.h"
#include <QApplication>

#include "config.h"
#include "exchangeBot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load configuration from file (if any)
    Config c;
    c.loadConfigFromFile();

    // Create a market bot
    ExchangeBot e;
    e.setConfig(&c);
    e.updateMarketDepth(); // TODO: remove

    // TODO: pass config & marketdata to gui.
    MainWindow w;
    w.show();

    return a.exec();
}
