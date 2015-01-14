#ifndef MARKETDATA_H
#define MARKETDATA_H

#include <QList>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "config.h"
#include "order.h"

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

private:
  Config *c;

  QList<Order> asks;
  QList<Order> bids;
};

class Ticker {

public:
  // TODO: Constructor

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
