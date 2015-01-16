#include "trade.h"

Trade::Trade() {

}

Trade::Trade(uint Type, double Price, double Amount, uint TradeID, uint TimeStamp) {

  type      = Type;
  price     = Price;
  amount    = Amount;
  tradeID   = TradeID;
  timeStamp = TimeStamp;
}

Trade::~Trade() {

}

// Getters

uint    Trade::getType()      { return type; }
double  Trade::getPrice()     { return price; }
double  Trade::getAmount()    { return amount; }
uint    Trade::getTradeID()   { return tradeID; }
uint    Trade::getTimeStamp() { return timeStamp; }
