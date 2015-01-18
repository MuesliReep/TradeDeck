#ifndef TRADEHISTORY_H
#define TRADEHISTORY_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QDateTime>

#include "config.h"
#include "downloader.h"


class TradeHistory : public QObject
{
  Q_OBJECT

public:
  explicit TradeHistory(QObject *parent = 0);
  ~TradeHistory();

private:
  Config *C;
  int source;

  void setConfig(Config *C);
  void loadDataSince(uint timeStamp);
  void addTradeDataTo();
  bool checkCoolDownExpiration(bool reset);

signals:

public slots:

};

#endif // TRADEHISTORY_H
