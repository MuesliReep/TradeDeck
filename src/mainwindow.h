#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLocale>
#include <QString>
#include <QTableWidgetItem>
#include <QModelIndex>

#include "exchangeBot.h"
#include "exchangeBot_btce.h"
#include "marketData.h"
#include "order.h"
#include "trade.h"
#include "confirmDialog.h"
#include "settingsDialog.h"

#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  explicit MainWindow(Config *C, QWidget *parent = 0);
  ~MainWindow();

  void setExchangeBots(ExchangeBot *E);

private:
  Ui::MainWindow *ui;

  Config      *c;
  ExchangeBot *e;

  QCPFinancial  *candlesticks;
  QCPGraph      *MA1;
  QCPGraph      *MA2;

  // bool checkBalance(int pair, double amount);
  void calculateMinimumBuyTrade(double sellPrice, double sellAmount, double fee, double *buyPrice, double *buyAmount, double *buyTotal, double profit = 0.00000001);
  double calculateMinimumSellTrade(double sellPrice, double sellAmount, double fee, double profit = 0.00000001);

  uint findClosestBin(uint desiredTime, uint binSize);

  void updateTradeDepth(TradeDepth *t);
  void updateTradeList(QList<Trade> *tradeData);
  void updateTradePlot(QList<DataPoint> *binnedTradeData, QList<QList<double> > *MAList);
  void scaleTradePlot(QList<DataPoint> *binnedTradeData);

  void updateBalances(QList<Balance> *balances);
  void updateOrders(QList<Order> *activeOrders);
  void updateTransactionHistory();

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

  void cancelOrderButtonPressed();
  void settingsButtonPressed();

  void calcValueChanged(int value);
  void calcSellAmountChanged();
  void calcSellPriceChanged();
  void calcSellTotalValueChanged();
  void calcBuyAmountChanged();
  void calcBuyPriceChanged();
  void calcBuyTotalValueChanged();
  void calcFeeValueChanged();
  void calcProfitValueChanged();
  void calcUseButtonPressed();

public slots:
  // ConfirmDialog
  void receiveOrderConfirmed(double price, double amount, int type);

  // ExchangeBot
  void receiveActiveOrders(QList<Order> activeOrders);
  void receiveOrderHistory(QList<Order> orderHistory);
  void receiveBalances(QList<Balance> balances);
  void receiveTicker(Ticker ticker);
  void receiveTradeDepth(TradeDepth tradeDepth);
  void receiveTradeHistory(QList<Trade> tradeData, QList<DataPoint> binnedTradeData, QList<QList<double> > MAList);
  void receiveTransactionHistory();
  void receiveMessage(int type, QString message);;

signals:
  void sendCancelOrder(uint orderID);
  void sendCreateOrder(int type, double price, double amount);
  void checkBalance(int currency, double amount, bool *result);

};

#endif // MAINWINDOW_H
