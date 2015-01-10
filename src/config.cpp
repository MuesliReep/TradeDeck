#include "config.h"

Config::Config() {

  fileName = "config.txt";
  lastLoadedTimeStamp = 0;
}

Config::~Config() {

}

bool Config::loadConfigFromFile() {

  bool result = false;

  QFile *file = new QFile(fileName);
  QJsonDocument json;

  if(file->open(QFile::ReadOnly)) {
    QJsonParseError error;

    json = QJsonDocument().fromJson(file->readAll(), &error);

    //Check if JSON was correctly parsed
    if (error == 0)
      result = true;
  }

  delete file;

  return result;
}

void Config::saveConfigToFile(QJsonObject *object) {

  QJsonDocument document(object);

  QFile file(fileName);
  file.open(QFile::WriteOnly);
  file.write(document.toJson(QJsonDocument::Indented));
}

uint getLastLoadedTimeStamp() { return timeStamp; }
void setLastLoadedTimeStamp(uint TimeStamp) { timeStamp = TimeStamp; }

// Sets the current time as the timeStamp for the same overloaded function
void Config::setLastLoaded() {

  QDateTime time(QDateTime::currentDateTime());
  uint timeStamp = time.toTime_t();
  setLastLoaded(timeStamp);
}

// Set the given timeStamp as the last time data was collected
void Config::setLastLoaded(uint timeStamp){

  QFile *file = new QFile("C:/Dummy/config.txt");
  if(file->open(QFile::WriteOnly))
  {
    char buffer[50]; // TODO: 32/64bit length?
    sprintf(buffer, "%d",timeStamp);

    file->write(buffer);
    file->flush();
    file->close();
  }

  delete file;
}

uint Config::getLastLoaded(){

  QFile *file = new QFile("C:/Dummy/config.txt");

  // Check if config file exists else return
  if( !file->exists() ) {
    qDebug() << "Config file does not exist";
    return 0;
  }

  // Read the timestamp from file
  QString line;
  QTextStream in(file);
  while (!in.atEnd()) {
    line = in.readLine();
  }

  delete file;

  // Convert QString to uint
  bool ok;
  uint result = line.toUInt(&ok,10);

  if(!ok) {
    qDebug() << "invalid timeStamp read";
    return 0;
  }

  return result;

}
