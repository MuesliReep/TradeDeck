#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QTextStream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

class Config
{

public:
  Config();
  ~Config();

  bool loadConfigFromFile();
  void saveConfigToFile();

  uint getLastLoadedTimeStamp();
  void setLastLoadedTimeStamp();
  void setLastLoadedTimeStamp(uint LastLoadedTimeStamp);
  
  void setLastLoaded();
  void setLastLoaded(uint timeStamp);
  uint getLastLoaded();

  void setCoolDownTime(uint CoolDownTime);
  uint getCoolDownTime();

private:
  QString fileName;
  uint lastLoadedTimeStamp;
  uint coolDownTime;
};

#endif // CONFIG_H
