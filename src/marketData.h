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
  DataPoint(uint TimeStamp, double Open, double Close, double High, double Low, double Average, double Volume) {

    timeStamp = TimeStamp;
    open      = Open;
    close     = Close;
    average   = Average;
    high      = High;
    low       = Low;
    volume    = Volume;
  }

  double  getOpen()       { return open; }
  double  getClose()      { return close; }
  uint    getTimeStamp()  { return timeStamp; }
  double  getAverage()    { return average; }
  double  getHigh()       { return high; }
  double  getLow()        { return low; }
  double  getVolume()     { return volume;}

private:
  double  open;
  double  close;
  double  high;
  double  low;
  uint    timeStamp;
  double  average;
  double  volume;

};

class MarketData
{

public:
  MarketData();
  MarketData(Config *C, bool loadFromFile = false);
  ~MarketData();

  void parseRawTradeData(QJsonArray *rawData);
  void parseRawDepthData(QJsonObject *rawData);
  void parseRawTickerData(QJsonObject *rawData);

  bool loadTradeDataFromFile(uint maxAge);
  void saveTradeDataToFile();

  uint getOldestTrade();

  QList<Order>      getAsks();
  QList<Order>      getBids();
  QList<Trade>      getTrades();
  QList<DataPoint>  getPriceList();
  QList<QList<double> >     getMAList();

private:
  Config *c;
  // Ticker t;
  QString tradeDataFileName = "btce_USD-BTC.json";

  QList<Order> asks;
  QList<Order> bids;

  QList<Trade> tradeData;
  QList<DataPoint> priceList;

  QList<QList<double> > MAList;

  int dataPoints  = 250;
  int binSize     = 1*60;

  void binTradeData();
  uint findClosestBin();
  QList<double> runEMA(int weight);
  QList<double> runSMA(int size);

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
