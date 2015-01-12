#include "mainwindow.h"
#include <QApplication>

#include "downloader.h"
#include "config.h"
#include "marketData.h"
#include "exchangeBot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load configuration from file (if any)
    Config c;
    c.loadConfigFromFile();

    // Configure the downloader
    Downloader d;
    d.setConfig(&c);

    // Create market data object
    MarketData m(&c);

    // Create a market bot
    ExchangeBot e(&c,&d,&m);
    e.updateMarketDepth(); // TODO: remove

    // TODO: pass config & marketdata to gui.
    MainWindow w;
    w.show();

    return a.exec();
}
