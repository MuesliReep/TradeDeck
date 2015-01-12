#ifndef EXCHANGEBOT_H
#define EXCHANGEBOT_H

#include <QDateTime>

#include "config.h"
#include "marketData.h"
#include "downloader.h"

class ExchangeBot
{

public:
  ExchangeBot(Config *C, Downloader *D, MarketData *M);
  ~ExchangeBot();

  void updateMarketTrades(uint limit);
  void updateMarketDepth();
  void updateMarketTicker();

  bool checkCoolDownExpiration(bool reset);

private:
  Downloader  *d;
  Config      *c;
  MarketData  *m;
};

#endif // EXCHANGEBOT_H
