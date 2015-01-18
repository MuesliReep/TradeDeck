#include "tradeHistory.h"

TradeHistory::TradeHistory(QObject *parent) :
QObject(parent) {

  source = -1;
}

TradeHistory::~TradeHistory() {

}

// Sets the source for the trade history data
void TradeHistory::setConfig(Config *C) {

  c = C;

  // Get the historical data source from the config file
  // 0 = private source (Custom server for later date)
  // 1 = bitcoincharts  (Less fun because no trade IDs are supplied)
  source = c->getHistorySource();

// TODO: set individual cooldown settings
// TODO: save config to config file
}

// Loads historical trade data from a set data source
void TradeHistory::loadDataSince(uint timeStamp) {

}

// Appends historical trade data to an existing data set
void TradeHistory::addTradeDataTo(QList<Trade> *tradeData) {

  // TODO: check which list is longer to determine fastest way of appending new data
}

// Checks if the current cool down timer has expired
// Can additionally reset the timer
bool TradeHistory::checkCoolDownExpiration(bool reset) {

  // The result to be returned
  bool result = false;

  // Check the current time and compare it to the saved timestamp
  QDateTime time(QDateTime::currentDateTime());
  if(time.toTime_t() > c->getHistoryLastLoadedTimeStamp()+c->getHistoryCoolDownTime()) {

    // If the timer needs to be reset
    if(reset) {

      c->setHistoryLastLoadedTimeStamp(source);
      c->saveConfigToFile();
    }

    return true;
  }

  return false;
}
