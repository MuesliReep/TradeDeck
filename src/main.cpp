#include "mainwindow.h"
#include <QApplication>

#include "config.h"
#include "exchangeBot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase fontDatabase;
    // int fontNum = fontDatabase.addApplicationFont(":/resources/fonts/Roboto-Regular.ttf");
    // int font = fontDatabase.addApplicationFont(":/resources/fonts/Roboto-Medium.ttf");
    // int font = fontDatabase.addApplicationFont(":/resources/fonts/Roboto-Thin.ttf");

    int fontNum = fontDatabase.addApplicationFont(":/resources/fonts/Arimo-Regular.ttf");
    // int fontNum = fontDatabase.addApplicationFont(":/resources/fonts/Arimo-Bold.ttf");

    qDebug() << "font families: " << fontDatabase.applicationFontFamilies(fontNum);
    qDebug() << "font styles: " << fontDatabase.styles("roboto");

    // Set font for application
    QFont font = fontDatabase.font("Arimo","Normal",8);
    font.setStyleStrategy(QFont::PreferAntialias);
    // font.setStyleStrategy(QFont::NoAntialias);
    a.setFont(font);

    // Load configuration from file (if any)
    Config c;
    c.loadConfigFromFile();

    // Create a market bot
    ExchangeBot *e = new ExchangeBot_btce();
    e->setConfig(&c);
    e->startBot();

    // TODO: pass list of bots to window
    MainWindow w(&c);
    w.setExchangeBots(e);
    w.setStyleSheet("QMainWindow {background: rgb(21, 35, 44);} QListWidget {background: rgb(30, 43, 52);} QListWidget::item { color: rgb(183,190,195); background-color:transparent; }");

    w.show();

    return a.exec();
}
