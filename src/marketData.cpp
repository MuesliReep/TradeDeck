#include "marketData.h"

MarketData::MarketData(Config *C) {
  c = C;
}

MarketData::~MarketData() {

}

// Parses a new market trade data set and merges it with the existing set(if any)
void MarketData::parseRawTradeData(QJsonObject rawData) {

}

// Parses new market depth data and overwrites current data
void MarketData::parseRawDepthData(QJsonObject rawData) {

  QJsonArray asks; // Sell orders
  QJsonArray bids; // Buy orders

  // Retrieve both arrays from JSON object
  asks = rawData.value("asks").toArray();
  bids = rawData.value("bids").toArray();

  // TODO: load into lists
}

// Parses new market ticker data and overwrites current data
void MarketData::parseRawTickerData(QJsonObject rawData) {

}

// Saves the current market data set to a specified file
void MarketData::loadTradeDataFromFile() {

}

// Loads a market data set from a specified file
void MarketData::saveTradeDataToFile() {

}
