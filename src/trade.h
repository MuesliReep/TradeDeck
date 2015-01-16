#ifndef TRADE_H
#define TRADE_H

#include <QtGlobal>

class Trade
{

public:
  Trade();
  Trade(uint Type, double Price, double Amount, uint TradeID, uint TimeStamp);
  ~Trade();

  uint    getType();
  double  getPrice();
  double  getAmount();
  uint    getTradeID();
  uint    getTimeStamp();

private:
  uint type; // 0 = ask, 1 = bid
  double price;
  double amount;
  uint tradeID;
  uint timeStamp;
};

#endif // TRADE_H
