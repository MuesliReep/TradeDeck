#include "exchangeBot.h"


ExchangeBot::ExchangeBot(QObject *parent) : QObject(parent) {

  // Start timers
  timer = new QTimer(this);
  timer2 = new QTimer(this);

  // connect(timer, SIGNAL(timeout()), this, SLOT(marketUpdateTick()));
  // connect(timer2, SIGNAL(timeout()), this, SLOT(privateUpdateTick()));
}

// Sets the configuration for this Exchange Bot
void ExchangeBot::setConfig(Config *C) {

  c = C;
  m = MarketData(c, true);
}

//----------------------------------//
//          Bot functions           //
//----------------------------------//

// Creates a nonce from the current timestamp in secondes
void ExchangeBot::createNonce(QByteArray *nonce) {

  uint now = QDateTime::currentDateTime().toMSecsSinceEpoch() / 250;

  if(lastNonce == now) {
    lastNonce+=2;
  }
  else
    lastNonce = now;

  nonce->setNum(lastNonce);

}

// Creates a nonce from the current timestamp in millsecondes
void ExchangeBot::createMilliNonce(QByteArray *nonce) {

  nonce->setNum(QDateTime::currentDateTime().toMSecsSinceEpoch());
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
//         UI Signal Slots          //
//----------------------------------//

void checkBalance(int currency, double amount, bool *result) {

  *result = false;

  if(amount <= balances[currency].getAmount())
    *result = true;
}
