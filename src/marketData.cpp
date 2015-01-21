#include "marketData.h"

MarketData::MarketData() {
}

MarketData::MarketData(Config *C) {
  c = C;
  // tradeDataFileName = "btce_USD-BTC.json"; // TODO: should be created by exchangeNot according to market
}

MarketData::~MarketData() {

}

// Analyzes current trade data and creates a plottable format
void MarketData::analyzeTradeData() {

  // TODO: Check if a list is already active, else create one
  priceList.clear();

  // Determine the oldest allowed trade according to the number of data points & time interval
  int maxTimeStampAge   = dataPoints * dataLength;
  int currentTimeStamp  = QDateTime::currentDateTime().toTime_t();
  int maxTimeStamp      = currentTimeStamp - maxTimeStampAge;

  // Work through the data list
  // Go from old to new

  // Find the oldest timestamp in the trade data
  int i;
  for(i = 0; i < tradeData.size(); i++) {

    if(tradeData[i].getTimeStamp() < maxTimeStamp) {

      i--; // -1 because current timestamp will not be used
      break;
    }
  }

  int minIntervalStamp = maxTimeStamp; // The youngest timestamp
  int maxIntervalStamp = maxTimeStamp; // The oldest timestamp

  int x = i; // Keep track of where we are in the tradeData list so we dont look at values twice

  // For each time interval calculate the opening and closing prices.
  // Also calculate the average price
  for(int j = dataPoints; j < dataPoints; j++) {

    // Determine the min and max timestamps for this time interval
    minIntervalStamp = maxTimeStamp + dataLength * (j+1);
    maxIntervalStamp = maxTimeStamp + dataLength * j;

    // Create a list with all the trades that occured in this time interval
    QList<Trade> intervalTrades;

    for(x; x >= 0; x--) {

      if(!(tradeData[x].getTimeStamp() >= maxIntervalStamp && tradeData[x].getTimeStamp() <= minIntervalStamp)) {

        x++;
        break;
      }

      intervalTrades.prepend(tradeData[x]);
    }

    // If no trades took place or no data is available for the time interval
    // use the previous value. If no previous value is available use zero.
    if(!intervalTrades.size()>0) {

      // Check if there is a previous value to use
      if(!priceList.size()>0) {

        priceList.prepend(DataPoint(maxIntervalStamp,0,0,0));
      } else { // Use the previous value

        priceList.prepend(priceList[j+1]);
      }
    }

    // Create a dataPoint from the intervalTrades list
    double  open      = intervalTrades[intervalTrades.size()-1].getPrice();
    double  close     = intervalTrades[0].getPrice();
    uint    timeStamp = intervalTrades[0].getTimeStamp();
    double  average;

    double sum = 0;
    for(int k=0; k < intervalTrades.size(); k++)
      sum += intervalTrades[k].getPrice();

    average = sum / intervalTrades.size();

    DataPoint dataPoint(timeStamp, open, close, average);
  }

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
    // uint type         = (uint)tradeObject.value("type").toInt();
    double price      = tradeObject.value("price").toDouble();
    double amount     = tradeObject.value("amount").toDouble();
    uint tradeID      = (uint)tradeObject.value("tid").toInt();

    Trade trade(price, amount, tradeID, timeStamp);

    // Insert the new trade object into the trade data list
    tradeData.insert(0,trade);
  }

  // Save the new data to file // TODO: should this be run every update?
  saveTradeDataToFile();

  // Now analyze the new data
  analyzeTradeData(); // TODO: called from exchangeBot?
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
bool MarketData::loadTradeDataFromFile() {

  bool result = false;

  tradeData.clear();

  // Open file and read the data
  QFile file(tradeDataFileName);
  QJsonDocument json;

  if(file.open(QFile::ReadOnly)) {

    QJsonParseError error;

    json = QJsonDocument().fromJson(file.readAll(), &error);

    // Check if JSON was correctly parsed
    if (error.error == QJsonParseError::NoError)
      result = true;
    else {

      qDebug() << error.errorString();
      return result;
    }

    // Read JSON values
    QJsonObject object    = json.object();
    QJsonArray marketData = object.value("marketData").toArray();

    // Convert the JSON array to a Trade List
    for(int i = 0; i < marketData.size(); i++) {

      // Convert the JSON values
      QJsonObject tradeObject = marketData[i].toObject();

      double  price     = tradeObject.value("price").toDouble();
      double  amount    = tradeObject.value("amount").toDouble();
      uint    tradeID   = (uint)tradeObject.value("tradeID").toInt();
      uint    timeStamp = (uint)tradeObject.value("timeStamp").toInt();

      // Create a trade object from the converted values
      Trade trade(price, amount, tradeID, timeStamp);

      // Append the new trade object to the trade list
      tradeData.append(trade);
    }
  }

  return result;
}

// Loads a market data set from a specified file
void MarketData::saveTradeDataToFile() {

    if(!tradeData.size()>0)
        return;

  QJsonObject object;
  QJsonArray marketData;

  // Insert the currently stored trade data into the JSON array
  for(int i = 0; i < tradeData.size(); i++) {

    QJsonObject tradeObject;
    Trade trade = tradeData[i];

    tradeObject.insert("price", QJsonValue(trade.getPrice()));
    tradeObject.insert("amount", QJsonValue(trade.getAmount()));
    tradeObject.insert("tradeID", QJsonValue((int)trade.getTradeID()));
    tradeObject.insert("timeStamp", QJsonValue((int)trade.getTimeStamp()));

    marketData.append(tradeObject);
  }

  // Insert the marketData array to the JSON object
  object.insert("marketData", QJsonValue(marketData));

  // Create a JSON document from the newly created JSON object
  QJsonDocument document(object);

  // Save the JSON document to file
  QFile file(tradeDataFileName);
  file.open(QFile::WriteOnly);
  file.write(document.toJson(QJsonDocument::Indented));
}

// Returns the oldest stored trade
uint MarketData::getOldestTrade() {

  if(!tradeData.size()>0)
    return 0;

  return tradeData[tradeData.size()-1].getTimeStamp();
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
