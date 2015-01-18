#ifndef EXCHANGEBOT_H
#define EXCHANGEBOT_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include "downloader.h"
#include "config.h"
#include "marketData.h"
#include "tradeHistory.h"


class ExchangeBot : public QObject
{
  Q_OBJECT
public:
  explicit ExchangeBot(QObject *parent = 0);
  ~ExchangeBot();

  void startBot();
  void setConfig(Config *C);

  MarketData* getMarketData();

private:
  Downloader  d;
  Config      *c;
  MarketData  m;
  QTimer *timer;
  QNetworkAccessManager* depthDownloadManager;
  QNetworkAccessManager* tickerDownloadManager;
  QNetworkAccessManager* tradeDownloadManager;

  void updateMarketTrades(uint limit);
  void updateMarketDepth();
  void updateMarketTicker();

  bool getObjectFromDocument(QNetworkReply *reply, QJsonObject *object);
  bool checkCoolDownExpiration(bool reset);

public slots:
  void depthDataReply(QNetworkReply *reply);
  void tickerDataReply(QNetworkReply *reply);
  void tradeDataReply(QNetworkReply *reply);

private slots:
  void marketUpdateTick();

signals:
  void sendNewMarketData(int dataType);

};

#endif // EXCHANGEBOT_H
