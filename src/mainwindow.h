#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLocale>
#include <QString>
#include <QTableWidgetItem>

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

    QCPFinancial  *candlesticks;
    QCPGraph      *MA1;
    QCPGraph      *MA2;

    bool checkBalance(int pair, double amount);
    double calculateMinimumBuyTrade(double sellPrice, double sellAmount, double fee);
    double calculateMinimumSellTrade(double sellPrice, double sellAmount, double fee);

    void updateTradeDepth();
    void updateTradeList();
    void updateTradePlot();
    void scaleTradePlot();

    void updateBalances();
    void updateOrders();

    void setupPlot();
    void setupOrdersTable();
    void setupBalancesTable();
    void setupTradesTable();
    void setupAsksTable();
    void setupBidsTable();

    void setupUISignals();

private slots:
  void buyTotalChanged(const QString);
  void buyButtonPressed();
  void sellTotalChanged(const QString);
  void sellButtonPressed();

  void calcSellUpdate();
  void calcBuyUpdate();
  void calcUseButtonPressed();

public slots:
  void receiveNewMarketData(int dataType);

};

#endif // MAINWINDOW_H
