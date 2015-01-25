#include "mainwindow.h"
#include <QApplication>

#include "config.h"
#include "exchangeBot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase fontDatabase;
    // int font = fontDatabase.addApplicationFont(":/resources/fonts/Roboto-Regular.ttf");
    int font = fontDatabase.addApplicationFont(":/resources/fonts/Roboto-Medium.ttf");
    // int font = fontDatabase.addApplicationFont(":/resources/fonts/Roboto-Thin.ttf");

    qDebug() << "font families: " << fontDatabase.applicationFontFamilies(font);
    qDebug() << "font styles: " << fontDatabase.styles("roboto");

    // Set font for application
    a.setFont(fontDatabase.font("Roboto","Normal",8));

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
    w.setStyleSheet("QMainWindow {background: rgb(38, 50, 56);} QListWidget {background: rgb(69,90,100);} QListWidget::item { color: rgb(245,245,245); background-color:transparent; }");

    w.show();

    return a.exec();
}
