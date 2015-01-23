#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLocale>
#include <QString>

#include "exchangeBot.h"
#include "marketData.h"
#include "order.h"
#include "trade.h"

#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setExchangeBots(ExchangeBot *E);

private:
    Ui::MainWindow *ui;

    ExchangeBot *e;

    QCPFinancial *candlesticks;

    void updateTradeDepth();
    void updateTradeList();
    void updateTradePlot();
    void scaleTradePlot();

    void setupPlot();

public slots:
  void receiveNewMarketData(int dataType);

};

#endif // MAINWINDOW_H
