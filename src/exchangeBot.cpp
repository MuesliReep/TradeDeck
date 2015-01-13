// The exchange bot gathers data from its specific exchange
// To use multiple exchanges at once, more bots will need to be made

#include "exchangeBot.h"

ExchangeBot::ExchangeBot(QObject *parent) :
QObject(parent)
{
}

ExchangeBot::~ExchangeBot() {

}

// Sets the configuration for this Exchange Bot
void ExchangeBot::setConfig(Config *C) {

  c=C;
  MarketData m = MarketData(c);
}

// Updates the market trades data
void ExchangeBot::updateMarketTrades(uint limit) {

  // Make sure the cool down timer has expired
  if(!checkCoolDownExpiration(true))
    return;

  // Create the request to download new data
  QNetworkRequest request = d.generateGetRequest(QUrl("https://btc-e.com/api/3/trades/btc_usd"));

  // Execute the download
  d.doDownload(request, this, SLOT(tradeDataReply(QNetworkReply*)));
}

// Updates the market depth data
void ExchangeBot::updateMarketDepth() {

  // Make sure the cool down timer has expired
  if(!checkCoolDownExpiration(true))
    return;

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/depth/btc_usd"));

  // Execute the download
  d.doDownload(request, this, SLOT(depthDataReply(QNetworkReply*)));
}

// Updates the market ticker data
void ExchangeBot::updateMarketTicker() {

  // Make sure the cool down timer has expired
  if(!checkCoolDownExpiration(true))
    return;

  // Create the request to download new data
  QNetworkRequest request = d.generateRequest(QUrl("https://btc-e.com/api/3/ticker/btc_usd"));

  // Execute the download
  d.doDownload(request, this, SLOT(depthDataReply(tickerDataReply*)));
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

// Slots

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
  }

  reply->deleteLater();
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
  }

  reply->deleteLater();
}

void ExchangeBot::tradeDataReply(QNetworkReply *reply) {

  if(!reply->error()) {

    QJsonObject jsonObj;
    QJsonObject tradeData;

    // Extract JSON object from network reply
    getObjectFromDocument(reply, &jsonObj);

    // Extract the market data we want
    tradeData = jsonObj.value("btc_usd").toObject();

    // Send the data to be parsed
    m.parseRawTradeData(&tradeData);
  }

  reply->deleteLater();
}
