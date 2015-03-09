#ifndef EXCHANGEBOT_BTCE_H
#define EXCHANGEBOT_BTCE_H

#include "exchangeBot.h"

#include <QObject>

class ExchangeTask {

public:
  ExchangeTask(int Task);
  ExchangeTask(int Task, QList<QString> Attributes);
  int getTask();
  QList<QString> getTaskAttributes();

private:
  int task;
  QList<QString> attributes;
};

class ExchangeBot_btce : public ExchangeBot
{
  Q_OBJECT
public:
  ExchangeBot_btce(QObject *parent = 0) : ExchangeBot(parent) {}
  // ~ExchangeBot_btce();

  MarketData* getMarketData();

  void startBot();

private:
  Downloader  d;

  int apiQueue;
  int pApiQueue;

  QList<ExchangeTask> exchangeTasks;

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

  void executeExchangeTask(ExchangeTask exchangeTask);
  void addExchangeTask(ExchangeTask exchangeTask, bool priority = false);
  void getInfoTask(ExchangeTask *exchangeTask);
  void createTradeTask(ExchangeTask *exchangeTask);
  void getActiveOrdersTask(ExchangeTask *exchangeTask);
  void getOrderInfoTask(ExchangeTask *exchangeTask);
  void cancelOrderTask(ExchangeTask *exchangeTask);
  void updateTradeHistoryTask(ExchangeTask *exchangeTask);
  void updateTransactionHistoryTask(ExchangeTask *exchangeTask);

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

public slots:

  void infoDataReply(QNetworkReply *reply);
  void createTradeDataReply(QNetworkReply *reply);
  void activeOrdersDataReply(QNetworkReply *reply);
  void orderInfoDataReply(QNetworkReply *reply);
  void cancelOrderDataReply(QNetworkReply *reply);
  void orderHistoryDataReply(QNetworkReply *reply);
  void transHistoryDataReply(QNetworkReply *reply);

  void depthDataReply(QNetworkReply *reply);
  void tickerDataReply(QNetworkReply *reply);
  void tradeDataReply(QNetworkReply *reply);

  void receiveCancelOrder(uint orderID);
  void receiveCreateOrder(int type, double price, double amount);

private slots:
  void updateTick();
  void updateTick2();

signals:

};

#endif // EXCHANGEBOT_BTCE_H
