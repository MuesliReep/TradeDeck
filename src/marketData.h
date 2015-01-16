#ifndef MARKETDATA_H
#define MARKETDATA_H

#include <QList>
#include <QString>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "config.h"
#include "order.h"
#include "trade.h"

class Ticker;

class MarketData
{

public:
  MarketData();
  MarketData(Config *C);
  ~MarketData();

  void parseRawTradeData(QJsonObject *rawData);
  void parseRawDepthData(QJsonObject *rawData);
  void parseRawTickerData(QJsonObject *rawData);

  void loadTradeDataFromFile();
  void saveTradeDataToFile();

  QList<Order> getAsks();
  QList<Order> getBids();

private:
  Config *c;
  // Ticker t;

  QList<Order> asks;
  QList<Order> bids;

  QList<Trade> tradeData;
};

class Ticker {

public:
  Ticker();
  Ticker( double high,    double low,
          double avg,     double vol,
          double vol_cur, double last,
          double buy,     double sell,
          uint updated );

private:
  double high;
  double low;
  double avg;
  double vol;
  double vol_cur;
  double last;
  double buy;
  double sell;
  uint updated;
};

#endif // MARKETDATA_H
