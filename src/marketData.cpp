#include "marketData.h"

MarketData::MarketData() {
}

MarketData::MarketData(Config *C) {
  c = C;
}

MarketData::~MarketData() {

}

// Parses a new market trade data set and merges it with the existing set(if any)
void MarketData::parseRawTradeData(QJsonArray *rawData) {

  // QJsonArray jTrades;

  // Retrieve array form JSON object
  // jTrades = rawData->value("btc_usd").toArray();

  if(!rawData->size()>0)
    return;

  // Append new trades to the beginning of the trades list
  // Work through the list from oldest to newest
  for(int i=rawData->size();i>=0;i--) {

    uint timeStamp = (uint)rawData->at(i).toObject().value("timestamp").toInt();

    // Check if there is any existing data to begin with
    if(tradeData.size() > 0) {

      // Check if the timestamp is newer than the first in the list
      // otherwise skip it and go to the next
      if(timeStamp < tradeData[0].getTimeStamp())
        continue;

      // If timestamps are the same check the tradeIDs
      // Because more than two trades can happen in one second,
      // the number of trades in that second needs to be known
      if(timeStamp == tradeData[0].getTimeStamp()) {

        // Counter starts at 1 because the first trade has already been found to
        // be in the same timestamp
        int x = 1;

        while(true) {

          if(timeStamp == tradeData[x].getTimeStamp()) {

            x++;
            continue;
          }
          
          break;
        }

        // The number of trades with the same timestamp is now known.
        // Next check the trade IDs

        // Trade ID of the trade we want to insert
        uint tradeID = (uint)rawData->at(i).toObject().value("tid").toInt();

        bool duplicate = false;

        for(int j=0;j<x;j++) {

          if(tradeID == tradeData[j].getTradeID()) { // TODO: Check if historical data, historical data will have timestamp = 0;

            duplicate = true;
            break;
          }
        }

        if(duplicate)
          continue;
      }
    }

    // Create a new trade object from the JSON values
    QJsonObject tradeObject = rawData->at(i).toObject();

    // QString tradePair = "btc_usd";
    uint type         = (uint)tradeObject.value("type").toInt();
    double price      = tradeObject.value("price").toDouble();
    double amount     = tradeObject.value("amount").toDouble();
    uint tradeID      = (uint)tradeObject.value("tid").toInt();

    Trade trade(type, price, amount, tradeID, timeStamp);

    // Insert the new trade object into the trade data list
    tradeData.insert(0,trade);
  }
}

// Parses new market depth data and overwrites current data
void MarketData::parseRawDepthData(QJsonObject *rawData) {

  QJsonArray jAsks; // Sell orders
  QJsonArray jBids; // Buy orders

  // Retrieve both arrays from JSON object
  jAsks = rawData->value("asks").toArray();
  jBids = rawData->value("bids").toArray();

  // Erase both QLists in preperation for new data
  // TODO: Look into appending data to existing list
  asks.clear();
  bids.clear();

  // Populate ask orders list
  for(int i=0;i<jAsks.size();i++) {

    double pair1 = jAsks[i].toArray()[0].toDouble();
    double pair2 = jAsks[i].toArray()[1].toDouble();
    asks.append(Order("BTC/USD",pair1,pair2));
  }

  // Populate bid orders list
  for(int i=0;i<jBids.size();i++) { // TODO: order should be in reverse

    double pair1 = jBids[i].toArray()[0].toDouble();
    double pair2 = jBids[i].toArray()[1].toDouble();
    bids.append(Order("BTC/USD",pair1,pair2));
  }
}

// Parses new market ticker data and overwrites current data
void MarketData::parseRawTickerData(QJsonObject *rawData) {

}

// Saves the current market data set to a specified file
void MarketData::loadTradeDataFromFile() {

}

// Loads a market data set from a specified file
void MarketData::saveTradeDataToFile() {

}

// Returns a pointer to the ask orders list
QList<Order> MarketData::getAsks() {
  return asks;
}

// Returns a pointer to the bid orders list
QList<Order> MarketData::getBids() {
  return bids;
}

QList<Trade> MarketData::getTrades() {
  return tradeData;
}
