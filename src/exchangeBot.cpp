// The exchange bot gathers data from its specific exchange
// To use multiple exchanges at once, more bots will need to be made

#include "exchangeBot.h"

ExchangeBot::ExchangeBot(QObject *parent) :
QObject(parent) {

  // Start timers
  timer = new QTimer(this);
  timer2 = new QTimer(this);

  connect(timer, SIGNAL(timeout()), this, SLOT(marketUpdateTick()));
  connect(timer2, SIGNAL(timeout()), this, SLOT(privateUpdateTick()));
}

ExchangeBot::~ExchangeBot() {

  delete timer;
}

//
void ExchangeBot::marketUpdateTick() {

  // Make sure the cool down timer has expired
  if(!checkCoolDownExpiration(true))
    return;

  // Update market data
  updateMarketDepth();
  updateMarketTrades(1000);

}

//
void ExchangeBot::privateUpdateTick() {

  pApiQueue++;

  switch(pApiQueue) {

    case 0:
      getInfo();
      break;
    case 1:
      updateTradeHistory();
      break;
    default:
      pApiQueue = -1;
      break;
  }
}

// Starts the Exchange Bot
void ExchangeBot::startBot() {

  // TODO: gather historical data
  // Check last trade timestamp to fill in the gap between last run and now
  // Also check the oldest trade timestamp to get full data set
  m.getOldestTrade();

  // Set the private API queue
  pApiQueue = -1;

  // Start the interval timer
  timer->start(c->getCoolDownTime()*1000);
  timer2->start(5*1000); // TODO: determine correct amount
}

// Sets the configuration for this Exchange Bot
void ExchangeBot::setConfig(Config *C) {

  c = C;
  MarketData m = MarketData(c);
}

//----------------------------------//
//      Private API functions       //
//----------------------------------//

//
void ExchangeBot::getInfo() {

  // Create POST data from method and nonce
  QByteArray method("method=getInfo");
  QByteArray nonce;
  nonce.setNum(QDateTime::currentDateTime().toTime_t()); //TODO: better nonce creation
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
  infoDownloadManager = d.doPostDownload(request, data, this, SLOT(infoDataReply(QNetworkReply*)));
}

//
void ExchangeBot::createTrade(QString pair, int type, double price, double amount) {

  // Create POST data from method and nonce
  QByteArray method("method=Trade");
  QByteArray nonce;
  nonce.setNum(QDateTime::currentDateTime().toTime_t()); //TODO: better nonce creation
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
  createTradeDownloadManager = d.doPostDownload(request, data, this, SLOT(createTradeDataReply(QNetworkReply*)));
}

//
void ExchangeBot::getActiveOrders(QString pair) {

  // Create POST data from method and nonce
  QByteArray method("method=ActiveOrders");
  QByteArray nonce;
  nonce.setNum(QDateTime::currentDateTime().toTime_t()); //TODO: better nonce creation
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
  activeOrdersDownloadManager = d.doPostDownload(request, data, this, SLOT(activeOrdersDataReply(QNetworkReply*)));
}

//
void ExchangeBot::getOrderInfo(uint orderID) {

  // Create POST data from method and nonce
  QByteArray method("method=OrderInfo");
  QByteArray nonce;
  nonce.setNum(QDateTime::currentDateTime().toTime_t()); //TODO: better nonce creation
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
void ExchangeBot::cancelOrder(uint orderID) {

  // Create POST data from method and nonce
  QByteArray method("method=CancelOrder");
  QByteArray nonce;
  nonce.setNum(QDateTime::currentDateTime().toTime_t()); //TODO: better nonce creation
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
  cancelOrderDownloadManager = d.doPostDownload(request, data, this, SLOT(cancelOrderDataReply(QNetworkReply*)));
}

// Updates the users trade history on the exchange
void ExchangeBot::updateTradeHistory() {

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
  nonce.setNum(QDateTime::currentDateTime().toTime_t()); //TODO: better nonce creation
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
  tradeHistoryDownloadManager = d.doPostDownload(request, data, this, SLOT(tradeHistoryDataReply(QNetworkReply*)));
}

//
void ExchangeBot::updateTransactionHistory() {

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
  nonce.setNum(QDateTime::currentDateTime().toTime_t()); //TODO: better nonce creation
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
bool ExchangeBot::checkSuccess(QJsonObject *object) {

  bool result = false;

  if(object->value("success").toInt() == 1)
    result = true;

  return result;
}

QString ExchangeBot::getRequestErrorMessage(QJsonObject *object) {

  return object->value("error").toString();
}

//----------------------------------//
//        Public API functions      //
//----------------------------------//

// Updates the market trades data
void ExchangeBot::updateMarketTrades(uint limit) {

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/trades/btc_usd?limit=2000"));

  // Execute the download
  tradeDownloadManager = d.doDownload(request, this, SLOT(tradeDataReply(QNetworkReply*)));
}

// Updates the market depth data
void ExchangeBot::updateMarketDepth() {

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/depth/btc_usd?limit=20"));

  // Execute the download
  depthDownloadManager = d.doDownload(request, this, SLOT(depthDataReply(QNetworkReply*)));
}

// Updates the market ticker data
void ExchangeBot::updateMarketTicker() {

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/ticker/btc_usd"));

  // Execute the download
  tickerDownloadManager = d.doDownload(request, this, SLOT(depthDataReply(tickerDataReply*)));
}

// Checks if the current cool down timer has expired
// Can additionally reset the timer
bool ExchangeBot::checkCoolDownExpiration(bool reset) {

  // The result to be returned
  bool result = false;

  // Check the current time and compare it to the saved timestamp
  QDateTime time(QDateTime::currentDateTime());
  if(time.toTime_t() > c->getLastLoadedTimeStamp()+c->getCoolDownTime()) {

    // If the timer needs to be reset
    if(reset) {

      c->setLastLoadedTimeStamp();
      c->saveConfigToFile();
    }

    return true;
  }

  return false;
}

// Grabs a JSON object from a Network reply
// Returns true if succesfull
bool ExchangeBot::getObjectFromDocument(QNetworkReply *reply, QJsonObject *object) {

  QJsonDocument   jsonDoc;
  QJsonParseError error;

  jsonDoc = QJsonDocument().fromJson(reply->readAll(), &error);
  *object = jsonDoc.object();

  return true; // TODO: check json validity
}

//----------------------------------//
//            Parsers               //
//----------------------------------//

//
void ExchangeBot::parseInfoData(QJsonObject *object) {

  USDBalance = object->value("usd").toDouble();
  BTCBalance = object->value("btc").toDouble();
}

//----------------------------------//
//              Slots               //
//----------------------------------//

void ExchangeBot::infoDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject infoData;
    QJsonObject fundsData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      infoData = jsonObj.value("return").toObject();
      fundsData = infoData.value("funds").toObject();

      // Parse new data
      parseInfoData(&fundsData);

      // Send signal to GUI to update
      sendNewMarketData(3);
    }
    else {

      qDebug() << "getInfo error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "getInfo Packet error";

    reply->deleteLater();
    infoDownloadManager->deleteLater();
}

void ExchangeBot::createTradeDataReply(QNetworkReply *reply) {

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

      // Send signal to GUI to update

    }
    else {

      qDebug() << "Trade error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "Trade Packet error";

    reply->deleteLater();
    createTradeDownloadManager->deleteLater();
}

void ExchangeBot::activeOrdersDataReply(QNetworkReply *reply) {

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

      // Send signal to GUI to update

    }
    else {

      qDebug() << "ActiveOrders error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "ActiveOrders Packet error";

    reply->deleteLater();
    activeOrdersDownloadManager->deleteLater();
}

void ExchangeBot::orderInfoDataReply(QNetworkReply *reply) {

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

void ExchangeBot::cancelOrderDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject cancelOrderData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      cancelOrderData = jsonObj.value("return").toObject();

      // Parse new data

      // Send signal to GUI to update

    }
    else {

      qDebug() << "CancelOrder error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "CancelOrder Packet error";

    reply->deleteLater();
    cancelOrderDownloadManager->deleteLater();
}

void ExchangeBot::tradeHistoryDataReply(QNetworkReply *reply) {

  qDebug() << "New tradeHistory Packet " << QDateTime::currentDateTime().toTime_t();

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject tradeHistoryData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Check if authentication was a succes
    if(checkSuccess(&jsonObj)) {

      // Extract the info data we want
      tradeHistoryData = jsonObj.value("return").toObject();

      // Parse new data

      // Send signal to GUI to update
      sendNewMarketData(8);
    }
    else {

      qDebug() << "TradeHistory error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "TradeHistory Packet error";

    reply->deleteLater();
    tradeHistoryDownloadManager->deleteLater();
}

void ExchangeBot::transHistoryDataReply(QNetworkReply *reply) {

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

    }
    else {

      qDebug() << "TransHistory error: " << getRequestErrorMessage(&jsonObj);
    }

  } else
    qDebug() << "TransHistory Packet error";

    reply->deleteLater();
    transHistoryDownloadManager->deleteLater();
}

void ExchangeBot::depthDataReply(QNetworkReply *reply) {

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
    sendNewMarketData(1);
  }
  else
    qDebug() << "Depth Packet error";

  // Disconnect the signal and release
  // disconnect(depthDownloadManager, 0, this, 0);

  reply->deleteLater();

  if(depthDownloadManager != NULL)
      depthDownloadManager->deleteLater();
}

void ExchangeBot::tickerDataReply(QNetworkReply *reply) {

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
    sendNewMarketData(2);
  }
  else
    qDebug() << "Ticker Packet error";

  // Disconnect the signal and release
  // disconnect(tickerDownloadManager, 0, this, 0);

  reply->deleteLater();
  tickerDownloadManager->deleteLater();
}

void ExchangeBot::tradeDataReply(QNetworkReply *reply) {

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
    sendNewMarketData(0);
  }
  else
    qDebug() << "Trade Packet error";

  // Disconnect the signal and release
  // disconnect(tradeDownloadManager, 0, this, 0);

  reply->deleteLater();
  tradeDownloadManager->deleteLater();
}

// Getters & Setters

MarketData* ExchangeBot::getMarketData() {

  return &m;
}

double ExchangeBot::getUSDBalance() {
  return USDBalance;
}
double ExchangeBot::getBTCBalance() {
  return BTCBalance;
}
