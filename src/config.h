#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QTextStream>

class Config
{

public:
  Config();
  ~Config();

  void setLastLoaded();
  void setLastLoaded(uint timeStamp);
  uint getLastLoaded();

private:

};

#endif // CONFIG_H
