#include "trade.h"

Trade::Trade() {

}

Trade::Trade(QString TradePair, uint Type, double Price, double Amount, uint TradeID, uint TimeStamp) {

  tradePair = TradePair;
  type      = Type;
  price     = Price;
  amount    = Amount;
  tradeID   = TradeID;
  timeStamp = TimeStamp;
}

Trade::~Trade() {

}

// Getters & Setters
// TODO: setters neccessary? 

QString Trade::getTradePair() { return tradePair; }
uint Trade::getType() { return type; }
double Trade::getPrice() { return price; }
double Trade::getAmount() { return amount; }
uint Trade::getTradeID() { return tradeID; }
uint Trade::getTimeStamp() { return timeStamp; }

void Trade::setTradePair(QString TradePair) { tradePair = TradePair; }
void Trade::setType(uint Type) { type = Type; }
void Trade::setPrice(double Price) { price = Price; }
void Trade::setAmount(double Amount) { amount = Amount; }
void Trade::setTradeID(uint TradeID) { tradeID = TradeID; }
void Trade::setTimeStamp(uint TimeStamp) { timeStamp = TimeStamp; }
