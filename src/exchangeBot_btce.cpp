// The exchange bot gathers data from its specific exchange
// To use multiple exchanges at once, more bots will need to be made

#include "exchangeBot_btce.h"


// ExchangeBot_btce::~ExchangeBot_btce() {
//
// }

//
void ExchangeBot_btce::updateTick() {

  // Make sure the cool down timer has expired
  if(!checkCoolDownExpiration(true))
    return;

  apiQueue++;

  // Update market data
  switch(apiQueue) {

    case 0:
      updateMarketDepth();
      break;
    case 1:
      updateMarketTrades(1000);
      break;
    default:
      apiQueue = -1;
      break;
  }
}

//
void ExchangeBot_btce::updateTick2() {

  // Work through the list of tasks here
  if(exchangeTasks.size() > 0) {

    // remove task from queue
    ExchangeTask exchangeTask = exchangeTasks.takeFirst();

    // Execute the first task
    executeExchangeTask(exchangeTask);
  }
}

// Starts the Exchange Bot
void ExchangeBot_btce::startBot() {

   infoDownloadManager = new QNetworkAccessManager(this);

  // TODO: gather historical data
  // Check last trade timestamp to fill in the gap between last run and now
  // Also check the oldest trade timestamp to get full data set
  m.getOldestTrade();

  // Set the private API queue
  pApiQueue = -1;
  apiQueue  = -1;
  lastNonce = 0;

  // Add initial tasks to queue
  addExchangeTask(ExchangeTask(0));       // getInfo
  QList<QString> attr; attr.append(QString("btc_usd"));
  addExchangeTask(ExchangeTask(2, attr)); // Active orders

  // Start the interval timers
  timer  = new QTimer(this);
  timer2 = new QTimer(this);

  connect(timer,  SIGNAL(timeout()), this, SLOT(updateTick()));
  connect(timer2, SIGNAL(timeout()), this, SLOT(updateTick2()));

  timer->start(c->getCoolDownTime()*1000);
  timer2->start(1*1100); // TODO: determine correct amount
}

//----------------------------------//
//          Exchange Tasks          //
//----------------------------------//
// 0 = getInfo
// 1 = createTrade
// 2 = getActiveOrders
// 3 = getOrderInfo
// 4 = cancelOrder
// 5 = updateTradeHistory
// 6 = updateTransactionHistory

ExchangeTask::ExchangeTask(int Task) {

  task = Task;
}

ExchangeTask::ExchangeTask(int Task, QList<QString> Attributes) {

  task = Task;
  attributes = Attributes;
}

int ExchangeTask::getTask() { return task; }
QList<QString> ExchangeTask::getTaskAttributes() { return attributes; }

void ExchangeBot_btce::executeExchangeTask(ExchangeTask exchangeTask) {

  switch(exchangeTask.getTask()) {

    case 0: getInfoTask(&exchangeTask);                  break;
    case 1: createTradeTask(&exchangeTask);              break;
    case 2: getActiveOrdersTask(&exchangeTask);          break;
    case 3: getOrderInfoTask(&exchangeTask);             break;
    case 4: cancelOrderTask(&exchangeTask);              break;
    case 5: updateTradeHistoryTask(&exchangeTask);       break;
    case 6: updateTransactionHistoryTask(&exchangeTask); break;
  }
}

void ExchangeBot_btce::addExchangeTask(ExchangeTask exchangeTask, bool priority) {

  if(priority)
    exchangeTasks.prepend(exchangeTask);
  else
    exchangeTasks.append(exchangeTask);
}

void ExchangeBot_btce::getInfoTask(ExchangeTask *exchangeTask)         { getInfo(); addExchangeTask(ExchangeTask(0)); }
void ExchangeBot_btce::createTradeTask(ExchangeTask *exchangeTask)     {

  QString pair    = exchangeTask->getTaskAttributes().at(0);
  uint type       = exchangeTask->getTaskAttributes().at(1).toUInt();
  double price    = exchangeTask->getTaskAttributes().at(2).toDouble();
  double amount   = exchangeTask->getTaskAttributes().at(3).toDouble();

  createTrade(pair, type, price, amount);
}
void ExchangeBot_btce::getActiveOrdersTask(ExchangeTask *exchangeTask) {

  getActiveOrders(exchangeTask->getTaskAttributes().at(0));

  // Add self to queue again
  QList<QString> attr; attr.append(QString("btc_usd"));
  addExchangeTask(ExchangeTask(2, attr));
}
void ExchangeBot_btce::getOrderInfoTask(ExchangeTask *exchangeTask)              { getOrderInfo(exchangeTask->getTaskAttributes().at(0).toUInt()); }
void ExchangeBot_btce::cancelOrderTask(ExchangeTask *exchangeTask)               { cancelOrder(exchangeTask->getTaskAttributes().at(0).toUInt()); }
void ExchangeBot_btce::updateTradeHistoryTask(ExchangeTask *exchangeTask)        { updateTradeHistory(); }
void ExchangeBot_btce::updateTransactionHistoryTask(ExchangeTask *exchangeTask)  { updateTransactionHistory(); }

//----------------------------------//
//      Private API functions       //
//----------------------------------//

//
void ExchangeBot_btce::getInfo() {

  // Create POST data from method and nonce
  QByteArray method("method=getInfo");
  QByteArray nonce;
  createNonce(&nonce);
  nonce.prepend("nonce=");
  QByteArray data(method +"&"+ nonce);

  // Sign the data
  QByteArray sign = QMessageAuthenticationCode::hash(data, c->getApiSecret().toUtf8(), QCryptographicHash::Sha512).toHex();

  // Create request
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/tapi/3/"));

  // Add headers
  d.addHeaderToRequest(&request, QByteArray("Content-type"), QByteArray("application/x-www-form-urlencoded"));
  d.addHeaderToRequest(&request, QByteArray("Key"), c->getApiKey().toUtf8());
  d.addHeaderToRequest(&request, QByteArray("Sign"), sign);

  // Execute download
  // infoDownloadManager = d.doPostDownload(request, data, this, SLOT(infoDataReply(QNetworkReply*)));
  d.doPostDownload(request, infoDownloadManager, data, this, SLOT(infoDataReply(QNetworkReply*)));
  // downloadManagers.append(infoDownloadManager);
}

//
void ExchangeBot_btce::createTrade(QString Pair, int Type, double Price, double Amount) {

  // Create POST data from method and nonce
  QByteArray method("method=Trade");
  QByteArray nonce;
  createNonce(&nonce);
  nonce.prepend("nonce=");

  QByteArray pair("pair=");
  pair.append(Pair);

  QByteArray type("type=");
  if(type == 0)
    type.append("buy");
  else
    type.append("sell");

  QByteArray price("rate=");
  QString sPrice; sPrice.setNum(Price,'f',3);
  price.append(sPrice);

  QByteArray amount("amount=");
  QString sAmount; sAmount.setNum(Amount,'f',8);
  amount.append(sAmount);

  QByteArray data(method +"&"+ nonce +"&"+ pair +"&"+ type +"&"+ price +"&"+ amount); qDebug() << "data: " << data;

  // Sign the data
  QByteArray sign = QMessageAuthenticationCode::hash(data, c->getApiSecret().toUtf8(), QCryptographicHash::Sha512).toHex();

  // Create request
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/tapi/3/"));

  // Add headers
  d.addHeaderToRequest(&request, QByteArray("Content-type"), QByteArray("application/x-www-form-urlencoded"));
  d.addHeaderToRequest(&request, QByteArray("Key"), c->getApiKey().toUtf8());
  d.addHeaderToRequest(&request, QByteArray("Sign"), sign);

  // Execute download
  createTradeDownloadManager = d.doPostDownload(request, data, this, SLOT(createTradeDataReply(QNetworkReply*)));
}

//
void ExchangeBot_btce::getActiveOrders(QString pair) {

  // Create POST data from method and nonce
  QByteArray method("method=ActiveOrders");
  QByteArray nonce;
  createNonce(&nonce);
  nonce.prepend("nonce=");
  QByteArray pairByte(pair.toUtf8());
  pairByte.prepend("pair=");
  QByteArray data(method +"&"+ nonce +"&"+ pairByte);

  // Sign the data
  QByteArray sign = QMessageAuthenticationCode::hash(data, c->getApiSecret().toUtf8(), QCryptographicHash::Sha512).toHex();

  // Create request
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/tapi/3/"));

  // Add headers
  d.addHeaderToRequest(&request, QByteArray("Content-type"), QByteArray("application/x-www-form-urlencoded"));
  d.addHeaderToRequest(&request, QByteArray("Key"), c->getApiKey().toUtf8());
  d.addHeaderToRequest(&request, QByteArray("Sign"), sign);

  // Execute download
  activeOrdersDownloadManager = d.doPostDownload(request, data, this, SLOT(activeOrdersDataReply(QNetworkReply*)));
}

//
void ExchangeBot_btce::getOrderInfo(uint orderID) {

  // Create POST data from method and nonce
  QByteArray method("method=OrderInfo");
  QByteArray nonce;
  createNonce(&nonce);
  nonce.prepend("nonce=");
  QByteArray data(method +"&"+ nonce);

  // Sign the data
  QByteArray sign = QMessageAuthenticationCode::hash(data, c->getApiSecret().toUtf8(), QCryptographicHash::Sha512).toHex();

  // Create request
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/tapi/3/"));

  // Add headers
  d.addHeaderToRequest(&request, QByteArray("Content-type"), QByteArray("application/x-www-form-urlencoded"));
  d.addHeaderToRequest(&request, QByteArray("Key"), c->getApiKey().toUtf8());
  d.addHeaderToRequest(&request, QByteArray("Sign"), sign);

  // Execute download
  orderInfoDownloadManager = d.doPostDownload(request, data, this, SLOT(orderInfoDataReply(QNetworkReply*)));
}

//
void ExchangeBot_btce::cancelOrder(uint OrderID) {

  // Create POST data from method and nonce
  QByteArray method("method=CancelOrder");
  QByteArray nonce;
  createNonce(&nonce);
  nonce.prepend("nonce=");

  QByteArray orderID("order_id=");
  QString orderIDS; orderIDS.setNum(OrderID);
  orderID.append(orderIDS);

  QByteArray data(method +"&"+ nonce +"&"+ orderID);

  // Sign the data
  QByteArray sign = QMessageAuthenticationCode::hash(data, c->getApiSecret().toUtf8(), QCryptographicHash::Sha512).toHex();

  // Create request
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/tapi/3/"));

  // Add headers
  d.addHeaderToRequest(&request, QByteArray("Content-type"), QByteArray("application/x-www-form-urlencoded"));
  d.addHeaderToRequest(&request, QByteArray("Key"), c->getApiKey().toUtf8());
  d.addHeaderToRequest(&request, QByteArray("Sign"), sign);

  // Execute download
  cancelOrderDownloadManager = d.doPostDownload(request, data, this, SLOT(cancelOrderDataReply(QNetworkReply*)));
}

// Updates the users trade history on the exchange
void ExchangeBot_btce::updateTradeHistory() {

  // Set optional parameters
  uint    from    = 0;	       // trade ID, from which the display starts	numerical	0
  uint    count   = 0;	       // the number of trades for display	numerical	1000
  uint    from_id = 0;	       // trade ID, from which the display starts	numerical	0
  uint    end_id  = 0;	       // trade ID on which the display ends	numerical	∞
  bool    order   = true;	     // Sorting	ASC or DESC	DESC
  uint    since   = 0;	       // the time to start the display	UNIX time	0
  uint    end     = 0;	       // the time to end the display	UNIX time	∞
  QString pair    = "btc_usd";

  // Create POST data from method and nonce
  QByteArray method("method=TradeHistory");
  QByteArray nonce;
  createNonce(&nonce);
  nonce.prepend("nonce=");
  QByteArray data(method +"&"+ nonce);

  // Sign the data
  QByteArray sign = QMessageAuthenticationCode::hash(data, c->getApiSecret().toUtf8(), QCryptographicHash::Sha512).toHex();

  // Create request
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/tapi/3/"));

  // Add headers
  d.addHeaderToRequest(&request, QByteArray("Content-type"), QByteArray("application/x-www-form-urlencoded"));
  d.addHeaderToRequest(&request, QByteArray("Key"), c->getApiKey().toUtf8());
  d.addHeaderToRequest(&request, QByteArray("Sign"), sign);

  // Execute download
  tradeHistoryDownloadManager = d.doPostDownload(request, data, this, SLOT(orderHistoryDataReply(QNetworkReply*)));
}

//
void ExchangeBot_btce::updateTransactionHistory() {

  // Set optional parameters
  uint from     = 0;  	// transaction ID, from which the display starts	numerical	0
  uint count    = 0;  	// number of transaction to be displayed	numerical	1000
  uint from_id  = 0;  	// transaction ID, from which the display starts	numerical	0
  uint end_id   = 0;  	// transaction ID on which the display ends	numerical	∞
  uint order    = 0;  	// sorting	ASC or DESC	DESC
  uint since    = 0;  	// the time to start the display	UNIX time	0
  uint end      = 0;    // the time to end the display

  // Create POST data from method and nonce
  QByteArray method("method=TransHistory");
  QByteArray nonce;
  createNonce(&nonce);
  nonce.prepend("nonce=");
  QByteArray data(method +"&"+ nonce);

  // Sign the data
  QByteArray sign = QMessageAuthenticationCode::hash(data, c->getApiSecret().toUtf8(), QCryptographicHash::Sha512).toHex();

  // Create request
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/tapi/3/"));

  // Add headers
  d.addHeaderToRequest(&request, QByteArray("Content-type"), QByteArray("application/x-www-form-urlencoded"));
  d.addHeaderToRequest(&request, QByteArray("Key"), c->getApiKey().toUtf8());
  d.addHeaderToRequest(&request, QByteArray("Sign"), sign);

  // Execute download
  transHistoryDownloadManager = d.doPostDownload(request, data, this, SLOT(transHistoryDataReply(QNetworkReply*)));
}

//
bool ExchangeBot_btce::checkSuccess(QJsonObject *object) {

  bool result = false;

  if(!object->contains("success"))
    return result;

  if(object->value("success").toInt() == 1)
    result = true;

  return result;
}

QString ExchangeBot_btce::getRequestErrorMessage(QJsonObject *object) {

  return object->value("error").toString();
}

//----------------------------------//
//        Public API functions      //
//----------------------------------//

// Updates the market trades data
void ExchangeBot_btce::updateMarketTrades(uint limit) {

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/trades/btc_usd?limit=2000"));

  // Execute the download
  tradeDownloadManager = d.doDownload(request, this, SLOT(tradeDataReply(QNetworkReply*)));
}

// Updates the market depth data
void ExchangeBot_btce::updateMarketDepth() {

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/depth/btc_usd?limit=20"));

  // Execute the download
  depthDownloadManager = d.doDownload(request, this, SLOT(depthDataReply(QNetworkReply*)));
}

// Updates the market ticker data
void ExchangeBot_btce::updateMarketTicker() {

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/ticker/btc_usd"));

  // Execute the download
  tickerDownloadManager = d.doDownload(request, this, SLOT(depthDataReply(tickerDataReply*)));
}

//----------------------------------//
//            Parsers               //
//----------------------------------//

//
void ExchangeBot_btce::parseInfoData(QJsonObject *object) {

  balances.clear();

  balances.append(Balance("usd", object->value("usd").toDouble()));
  balances.append(Balance("btc", object->value("btc").toDouble()));
}

bool ExchangeBot_btce::parseActiveOrdersData(QJsonObject *object) {

  // Clear the current list
  activeOrders.clear();

  QStringList orderIDs = object->keys();

  // If there aren't any active orders return
  if(orderIDs.size() == 0)
    return false;

  for(int i = 0; i < orderIDs.size(); i++) {

    QJsonObject orderObject = object->value(orderIDs[i]).toObject();

    uint orderID    = (uint)orderIDs[i].toInt();
    QString pair    = orderObject.value("pair").toString(); // TODO: split pair
    QString typeS   = orderObject.value("type").toString();
    double amount   = orderObject.value("amount").toDouble();
    double price    = orderObject.value("rate").toDouble();
    uint timeStamp  = (uint)orderObject.value("timestamp_created").toInt();

    uint type = 0;

    if(typeS.contains("sell"))
      type = 1;

    Order order(pair, amount, price, orderID, type, timeStamp);

    activeOrders.append(order);
  }

  return true;
}

//----------------------------------//
//              Slots               //
//----------------------------------//

void ExchangeBot_btce::infoDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject infoData;
    QJsonObject fundsData;
    int openOrders;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      infoData = jsonObj.value("return").toObject();
      fundsData = infoData.value("funds").toObject();
      openOrders = infoData.value("open_orders").toInt();

      // Parse new data
      parseInfoData(&fundsData);

      // // Clear activeOrders if necessary // TODO: check why this doesnt work
      // if(!openOrders > 0)
      //   activeOrders.clear();

      // Send signal to GUI to update
      sendBalances(balances);
    }
    else {

      qDebug() << "getInfo error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "getInfo Packet error";

    // qDebug() << "downloadManagers size before: " << downloadManagers.size();

    reply->deleteLater();
    // if(infoDownloadManager != NULL)
        // infoDownloadManager->deleteLater();

    disconnect(infoDownloadManager, 0, this, 0);

    // qDebug() << "downloadManagers size after: " << downloadManagers.size();
}

void ExchangeBot_btce::createTradeDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject tradeData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      tradeData = jsonObj.value("return").toObject();

      // Parse new data
      // TODO: parse trade data

      // Send signal to GUI to update
      qDebug() << "Trade created successfully";

    }
    else {

      qDebug() << "Trade error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "Trade Packet error";

    reply->deleteLater();
    createTradeDownloadManager->deleteLater();
}

void ExchangeBot_btce::activeOrdersDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject activeOrdersData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      activeOrdersData = jsonObj.value("return").toObject();

      // Parse new data
      parseActiveOrdersData(&activeOrdersData);

      // Send signal to GUI to update
      sendActiveOrders(activeOrders);
    }
    else {

      qDebug() << "ActiveOrders error: " << getRequestErrorMessage(&jsonObj);

      // Parse empty data
      parseActiveOrdersData(&activeOrdersData);

      // Send signal to GUI to update
      sendActiveOrders(activeOrders);
    }

  } else
    qDebug() << "ActiveOrders Packet error";

    reply->deleteLater();
    activeOrdersDownloadManager->deleteLater();
}

void ExchangeBot_btce::orderInfoDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject orderInfoData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      orderInfoData = jsonObj.value("return").toObject();

      // Parse new data

      // Send signal to GUI to update

    }
    else {

      qDebug() << "OrderInfo error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "OrderInfo Packet error";

    reply->deleteLater();
    orderInfoDownloadManager->deleteLater();
}

void ExchangeBot_btce::cancelOrderDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject cancelOrderData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      cancelOrderData = jsonObj.value("return").toObject();
      uint orderID    = jsonObj.value("order_id").toInt();

      // Parse new data
      // TODO: update new balances

      // Send signal to GUI to update

      qDebug() << "Order: " << orderID << " was cancelled successfully";

    }
    else {

      qDebug() << "CancelOrder error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "CancelOrder Packet error";

    reply->deleteLater();
    cancelOrderDownloadManager->deleteLater();
}

void ExchangeBot_btce::orderHistoryDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject orderHistoryData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      orderHistoryData = jsonObj.value("return").toObject();

      // Parse new data

      // Send signal to GUI to update
      sendOrderHistory(orderHistory);
    }
    else {

      qDebug() << "TradeHistory error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "TradeHistory Packet error";

    reply->deleteLater();
    tradeHistoryDownloadManager->deleteLater();
}

void ExchangeBot_btce::transHistoryDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject transHistoryData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      transHistoryData = jsonObj.value("return").toObject();

      // Parse new data

      // Send signal to GUI to update
      sendTransactionHistory();
    }
    else {

      qDebug() << "TransHistory error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "TransHistory Packet error";

    reply->deleteLater();
    transHistoryDownloadManager->deleteLater();
}

void ExchangeBot_btce::depthDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject depthData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Extract the market data we want
    depthData = jsonObj.value("btc_usd").toObject();

    // Send the data to be parsed
    m.parseRawDepthData(&depthData);

    // Send signal to GUI to update
    sendTradeDepth(m.getTradeDepth());
  }
  else
    qDebug() << "Depth Packet error";

  // Disconnect the signal and release
  // disconnect(depthDownloadManager, 0, this, 0);

  reply->deleteLater();

  if(depthDownloadManager != NULL)
      depthDownloadManager->deleteLater();
}

void ExchangeBot_btce::tickerDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject tickerData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Extract the market data we want
    tickerData = jsonObj.value("btc_usd").toObject();

    // Send the data to be parsed
    m.parseRawTickerData(&tickerData);

    // Send signal to GUI to update
    sendTicker(m.getTicker());
  }
  else
    qDebug() << "Ticker Packet error";

  // Disconnect the signal and release
  // disconnect(tickerDownloadManager, 0, this, 0);

  reply->deleteLater();
  tickerDownloadManager->deleteLater();
}

void ExchangeBot_btce::tradeDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonArray tradeData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Extract the market data we want
    tradeData = jsonObj.value("btc_usd").toArray();

    // Send the data to be parsed
    m.parseRawTradeData(&tradeData);

    // Send signal to GUI to update
    sendTradeHistory(m.getTradeData(), m.getBinnedTradeData(), m.getMAList());
  }
  else
    qDebug() << "Trade Packet error";

  // Disconnect the signal and release
  // disconnect(tradeDownloadManager, 0, this, 0);

  reply->deleteLater();
  if(tradeDownloadManager != NULL)
    tradeDownloadManager->deleteLater();
}

//----------------------------------//
//           UI Commands            //
//----------------------------------//

void ExchangeBot_btce::receiveCreateOrder(int type, double price, double amount) {

  QString pair = "btc_usd";
  QString typeS, priceS, amountS;
  typeS.setNum(type);
  priceS.setNum(price);
  amountS.setNum(amount);

  QList<QString> attr;
  attr.append(pair);
  attr.append(typeS);
  attr.append(priceS);
  attr.append(amountS);
  addExchangeTask(ExchangeTask(1, attr), true);
}

void ExchangeBot_btce::receiveCancelOrder(uint orderID) {

  QString orderIDS; orderIDS.setNum(orderID);
  QList<QString> attr; attr.append(orderIDS);
  addExchangeTask(ExchangeTask(4, attr), true);
}

//----------------------------------//
//         Getters & Setters        //
//----------------------------------//
