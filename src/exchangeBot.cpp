// The exchange bot gathers data from its specific exchange
// To use multiple exchanges at once, more bots will need to be made

#include "exchangeBot.h"

ExchangeBot::ExchangeBot(Config *C, Downloader *D, MarketData *M) {

  c = C;
  d = D;
  m = M;
}

ExchangeBot::~ExchangeBot() {

}

// Updates the market trades data
void ExchangeBot::updateMarketTrades(uint limit) {

  // Make sure the cool down timer has expired

  // Create the request to download new data

  // Execute the download

}

// Updates the market depth data
void ExchangeBot::updateMarketDepth() {

  // Make sure the cool down timer has expired
  if(!checkCoolDownExpiration(true))
    return;

  // Create the request to download new data
  QNetworkRequest request = d->generateRequest(QUrl("https://btc-e.com/api/3/depth/btc_usd"));

  // Execute the download
  d->doDownload(request);
}

// Updates the market ticker data
void ExchangeBot::updateMarketTicker() {

  // Make sure the cool down timer has expired

  // Create the request to download new data

  // Execute the download

}

// Checks if the current cool down timer has expired
// Can additionally reset the timer
bool ExchangeBot::checkCoolDownExpiration(bool reset) {

  // The result to be returned
  bool result = false;

  // Check the current time and compare it to the saved timestamp
  QDateTime time(QDateTime::currentDateTime());
  if(time.toTime_t() > c->getLastLoadedTimeStamp()+c->getCoolDownTime()) {

    // If the timer needs to be reset
    if(reset) {

      c->setLastLoadedTimeStamp();
      c->saveConfigToFile();
    }

    return true;
  }

  return false;
}
