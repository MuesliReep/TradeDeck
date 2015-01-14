#include "marketData.h"

MarketData::MarketData() {
}

MarketData::MarketData(Config *C) {
  c = C;
}

MarketData::~MarketData() {

}

// Parses a new market trade data set and merges it with the existing set(if any)
void MarketData::parseRawTradeData(QJsonObject *rawData) {

}

// Parses new market depth data and overwrites current data
void MarketData::parseRawDepthData(QJsonObject *rawData) {

  QJsonArray jAsks; // Sell orders
  QJsonArray jBids; // Buy orders

  // Retrieve both arrays from JSON object
  jAsks = rawData->value("asks").toArray();
  jBids = rawData->value("bids").toArray();

  // Erase both QLists in preperation for new data
  asks.removeAll();
  bids.removeAll();

  // Populate ask orders list
  for(int i=0;i<jAsks.size();i++) {

    double pair1 = jAsks[i].toArray()[0].toDouble();
    double pair2 = jAsks[i].toArray()[1].toDouble();
    asks.append(Order("BTC/USD",pair1,pai2));
  }

  // Populate bid orders list
  for(int i=0;i<jBids.size();i++) { // TODO: order should be in reverse

    double pair1 = jBids[i].toArray()[0].toDouble();
    double pair2 = jBids[i].toArray()[1].toDouble();
    bids.append(Order("BTC/USD",pair1,pai2));
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
