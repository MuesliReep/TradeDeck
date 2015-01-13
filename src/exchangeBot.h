#ifndef EXCHANGEBOT_H
#define EXCHANGEBOT_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include "downloader.h"
#include "config.h"
#include "marketData.h"


class ExchangeBot : public QObject
{
  Q_OBJECT
public:
  explicit ExchangeBot(QObject *parent = 0);
  ~ExchangeBot();

  void setConfig(Config *C);

  void updateMarketTrades(uint limit);
  void updateMarketDepth();
  void updateMarketTicker();

  bool checkCoolDownExpiration(bool reset);

private:
  Downloader  d;
  Config      *c;
  MarketData  m;

  bool getObjectFromDocument(QNetworkReply *reply, QJsonObject *object);

public slots:
  void depthDataReply(QNetworkReply *reply);
  void tickerDataReply(QNetworkReply *reply);
  void tradeDataReply(QNetworkReply *reply);

private slots:

};

#endif // EXCHANGEBOT_H
