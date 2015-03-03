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


class ExchangeBot : public QObject
{
  Q_OBJECT
public:
  explicit ExchangeBot(QObject *parent = 0);
  ~ExchangeBot();

  void startBot();
  void setConfig(Config *C);

  double getUSDBalance(); // TODO: Balances should be stored dynamically to allow more than 2
  double getBTCBalance();

  MarketData* getMarketData();
  QList<Order> getActiveOrders();
  void sendCancelOrder(uint orderID); // TODO: should this be a signal/slot from UI?

private:
  Downloader  d;
  Config      *c;
  MarketData  m;
  QTimer *timer;
  QTimer *timer2;

  int pApiQueue;
  double USDBalance;
  double BTCBalance;
  QList<Order> activeOrders;

  QNetworkAccessManager* infoDownloadManager;
  QNetworkAccessManager* createTradeDownloadManager;
  QNetworkAccessManager* activeOrdersDownloadManager;
  QNetworkAccessManager* orderInfoDownloadManager;
  QNetworkAccessManager* cancelOrderDownloadManager;
  QNetworkAccessManager* tradeHistoryDownloadManager;
  QNetworkAccessManager* transHistoryDownloadManager;

  QNetworkAccessManager* depthDownloadManager;
  QNetworkAccessManager* tickerDownloadManager;
  QNetworkAccessManager* tradeDownloadManager;

  // Private API:

  void getInfo();
  void createTrade(QString pair, int type, double price, double amount);
  void getActiveOrders(QString pair);
  void getOrderInfo(uint orderID);
  void cancelOrder(uint OrderID);
  void updateTradeHistory();
  void updateTransactionHistory();
  bool checkSuccess(QJsonObject *object);
  QString getRequestErrorMessage(QJsonObject *object);

  void parseInfoData(QJsonObject *object);
  bool parseActiveOrdersData(QJsonObject *object);

  // Public API:

  void updateMarketTrades(uint limit);
  void updateMarketDepth();
  void updateMarketTicker();

  bool getObjectFromDocument(QNetworkReply *reply, QJsonObject *object);
  bool checkCoolDownExpiration(bool reset);

public slots:

  void infoDataReply(QNetworkReply *reply);
  void createTradeDataReply(QNetworkReply *reply);
  void activeOrdersDataReply(QNetworkReply *reply);
  void orderInfoDataReply(QNetworkReply *reply);
  void cancelOrderDataReply(QNetworkReply *reply);
  void tradeHistoryDataReply(QNetworkReply *reply);
  void transHistoryDataReply(QNetworkReply *reply);

  void depthDataReply(QNetworkReply *reply);
  void tickerDataReply(QNetworkReply *reply);
  void tradeDataReply(QNetworkReply *reply);

  void receiveTradeRequest(int type, double price, double amount);

private slots:
  void marketUpdateTick();
  void privateUpdateTick();

signals:
  void sendNewMarketData(int dataType);

};

#endif // EXCHANGEBOT_H
