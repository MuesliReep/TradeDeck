#ifndef ORDER_H
#define ORDER_H

#include <QString>

class Order
{

public:
  Order();
  Order(QString Name, double Pair1, double Pair2);
  ~Order();

  QString getOrderName();
  double getPair1();
  double getPair2();

private:
  QString name;
  double pair1;
  double pair2;
};

#endif // ORDER_H
