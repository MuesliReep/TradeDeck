#ifndef EXCHANGEBOT_H
#define EXCHANGEBOT_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <QMessageAuthenticationCode>
#include <QCryptographicHash>

#include "downloader.h"
#include "config.h"
#include "marketData.h"
#include "marketHistory.h"

class Balance {

public:
  Balance(QString Currency, double Amount) { currency = Currency; amount = Amount; }
  QString getCurrency() { return currency; }
  double getAmount() { return amount; }
private:
  QString currency;
  double amount;
};

class ExchangeBot : public QObject
{
  Q_OBJECT
public:
  ExchangeBot(QObject *parent = 0);
  virtual ~ExchangeBot();

  virtual void startBot() = 0;
  void setConfig(Config *C);

protected:
  Config     *c;
  MarketData  m;

  QTimer *timer;
  QTimer *timer2;

  uint lastNonce;

  QList<Order>    activeOrders;
  QList<Order>    orderHistory;
  QList<Balance>  balances;

  void createNonce(QByteArray *nonce);
  void createMilliNonce(QByteArray *nonce);

  bool getObjectFromDocument(QNetworkReply *reply, QJsonObject *object);
  bool checkCoolDownExpiration(bool reset);

public slots:
  // UI signals
  virtual void receiveCancelOrder(uint orderID) = 0;
  virtual void receiveCreateOrder(int type, double price, double amount) = 0;
  void checkBalance(int currency, double amount, bool *result);

signals:
  void sendActiveOrders(QList<Order> activeOrders);
  void sendOrderHistory(QList<Order> orderHistory);
  void sendBalances(QList<Balance> balances);
  void sendTicker(Ticker ticker);
  void sendTradeDepth(TradeDepth tradeDepth);
  void sendTradeHistory(QList<Trade> tradeData, QList<DataPoint> binnedTradeData, QList<QList<double> > MAList);
  void sendTransactionHistory();
  void sendMessage(int type, QString message);

};
#endif // EXCHANGEBOT_H
