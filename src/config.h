#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

class Config
{

public:
  Config();
  ~Config();

  bool loadConfigFromFile();
  void saveConfigToFile(QJsonObject *object);

  uint getLastLoadedTimeStamp();
  void setLastLoadedTimeStamp(uint TimeStamp);

  void setLastLoaded();
  void setLastLoaded(uint timeStamp);
  uint getLastLoaded();

private:
  QString fileName;
  uint lastLoadedTimeStamp;
};

#endif // CONFIG_H
