#include "config.h"

Config::Config() {

  fileName = "config.json";
  lastLoadedTimeStamp = 0;
  coolDownTime = 2;
}

Config::~Config() {

}

// Loads a configuration from a given file, overwriting the running config
bool Config::loadConfigFromFile() {

  bool result = false;

  QFile *file = new QFile(fileName);
  QJsonDocument json;

  if(file->open(QFile::ReadOnly)) {
    QJsonParseError error;

    json = QJsonDocument().fromJson(file->readAll(), &error);

    // TODO:
    //Check if JSON was correctly parsed
    // if (error.ParseError == QJsonParseError::NoError)
    //   result = true;

    // Read JSON values
    QJsonObject object = json.object();
    lastLoadedTimeStamp = (uint)object.value("lastLoadedTimeStamp").toInt();
    coolDownTime        = (uint)object.value("coolDownTime").toInt();
  }

  delete file;

  return result;
}

// Saves the running config to file
void Config::saveConfigToFile() {

  // Create JSON object from current configuration
  QJsonObject object;
  object.insert("lastLoadedTimeStamp", QJsonValue((int)lastLoadedTimeStamp));
  object.insert("coolDownTime", QJsonValue((int)coolDownTime));


  // Create a JSON document from the newly created JSON object
  QJsonDocument document(object);

  // Save the JSON document to file
  QFile file(fileName);
  file.open(QFile::WriteOnly);
  file.write(document.toJson(QJsonDocument::Indented));
}

void Config::setCoolDownTime(uint CoolDownTime) { coolDownTime = CoolDownTime; }
uint Config::getCoolDownTime() { return coolDownTime; }

uint Config::getLastLoadedTimeStamp() { return lastLoadedTimeStamp; }
void Config::setLastLoadedTimeStamp(uint LastLoadedTimeStamp) { lastLoadedTimeStamp = LastLoadedTimeStamp; }

// Sets the current time as the timeStamp for the same overloaded function
void Config::setLastLoadedTimeStamp() {

  QDateTime time(QDateTime::currentDateTime());
  setLastLoadedTimeStamp(time.toTime_t());
}
