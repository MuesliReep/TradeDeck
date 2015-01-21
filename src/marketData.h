#ifndef MARKETDATA_H
#define MARKETDATA_H

#include <QList>
#include <QString>
#include <QFile>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include "config.h"
#include "order.h"
#include "trade.h"

class Ticker;

class DataPoint {

public:
  DataPoint(uint TimeStamp, double Open, double Close, double Average) {

    timeStamp = TimeStamp;
    open      = Open;
    close     = Close;
    average   = Average;
  }

  double  getOpen()       { return open; }
  double  getClose()      { return close; }
  uint    getTimeStamp()  { return timeStamp; }
  double  getAverage()    { return average; }

private:
  double  open;
  double  close;
  uint    timeStamp;
  double  average;

};

class MarketData
{

public:
  MarketData();
  MarketData(Config *C);
  ~MarketData();

  void parseRawTradeData(QJsonArray *rawData);
  void parseRawDepthData(QJsonObject *rawData);
  void parseRawTickerData(QJsonObject *rawData);

  bool loadTradeDataFromFile();
  void saveTradeDataToFile();

  uint getOldestTrade();

  QList<Order> getAsks();
  QList<Order> getBids();
  QList<Trade> getTrades();

private:
  Config *c;
  // Ticker t;
  QString tradeDataFileName = "btce_USD-BTC.json";

  QList<Order> asks;
  QList<Order> bids;

  QList<Trade> tradeData;

  int dataPoints = 250;
  int dataLength = 3*60;

  void analyzeTradeData();
  QList<DataPoint> priceList;
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
