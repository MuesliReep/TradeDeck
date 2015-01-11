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

    //Check if JSON was correctly parsed
    // if (error.ParseError == QJsonParseError::NoError)
    //   result = true;

    // Read JSON values
    QJsonObject object = json.object();
    lastLoadedTimeStamp = (uint)object.value("lastLoadedTimeStamp").toInt();
    coolDownTime        = (uint)object.value("coolDownTime").toInt();

    qDebug() << "lastLoadedTimeStamp";
    qDebug() << lastLoadedTimeStamp;
    qDebug() << "coolDownTime";
    qDebug() << coolDownTime;
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

  qDebug() << "Running config saved";
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

// // Set the given timeStamp as the last time data was collected
// void Config::setLastLoaded(uint timeStamp){
//
//   QFile *file = new QFile("C:/Dummy/config.txt");
//   if(file->open(QFile::WriteOnly))
//   {
//     char buffer[50]; // TODO: 32/64bit length?
//     sprintf(buffer, "%d",timeStamp);
//
//     file->write(buffer);
//     file->flush();
//     file->close();
//   }
//
//   delete file;
// }
//
// uint Config::getLastLoaded(){
//
//   QFile *file = new QFile("C:/Dummy/config.txt");
//
//   // Check if config file exists else return
//   if( !file->exists() ) {
//     qDebug() << "Config file does not exist";
//     return 0;
//   }
//
//   // Read the timestamp from file
//   QString line;
//   QTextStream in(file);
//   while (!in.atEnd()) {
//     line = in.readLine();
//   }
//
//   delete file;
//
//   // Convert QString to uint
//   bool ok;
//   uint result = line.toUInt(&ok,10);
//
//   if(!ok) {
//     qDebug() << "invalid timeStamp read";
//     return 0;
//   }
//
//   return result;
//
// }
