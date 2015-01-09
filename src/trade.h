#ifndef TRADE_H
#define TRADE_H

#include <QString>

class Trade
{

public:
  Trade();
  Trade(QString TradePair, uint Type, double Price, double Amount, uint TradeID, uint TimeStamp);
  ~Trade();

  QString getTradePair();
  uint getType();
  double getPrice();
  double getAmount();
  uint getTradeID();
  uint getTimeStamp();

  void setTradePair(QString TradePair);
  void setType(uint Type);
  void setPrice(double Price);
  void setAmount(double Amount);
  void setTradeID(uint TradeID);
  void setTimeStamp(uint TimeStamp);

private:
  QString tradePair;
  uint type; // 0 = ask, 1 = bid
  double price;
  double amount;
  uint tradeID;
  uint timeStamp;
};

#endif // TRADE_H
