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
    e.startBot();

    // TODO: pass list of bots to window
    MainWindow w;
    w.setExchangeBots(&e);
    // w.setStyleSheet("QMainWindow {background: rgb(50, 53, 54);}");
    w.show();

    return a.exec();
}
