#include "downloader.h"
#include "Config.h"

Downloader::Downloader(QObject *parent) :
QObject(parent)
{
}

Downloader::~Downloader() {

  delete manager;
}

void Downloader::doDownload()
{

  QDateTime time(QDateTime::currentDateTime());
  Config c; // TODO: config should be static

  // Check if we are allowed to gather new data
  if(!time.toTime_t() > c.getLastLoaded()+2) // TODO: expiry time should be defined somewhere not hardcoded
    return;

  manager = new QNetworkAccessManager(this);

  connect(manager, SIGNAL(finished(QNetworkReply*)),
  this, SLOT(replyFinished(QNetworkReply*)));

  manager->get(QNetworkRequest(QUrl("https://btc-e.com/api/3/depth/btc_usd")));
  // manager->get(QNetworkRequest(QUrl("http://google.nl")));
}

void Downloader::replyFinished (QNetworkReply *reply)
{
  if(reply->error())
  {
    qDebug() << "ERROR!";
    qDebug() << reply->errorString();
  }
  else
  {
    qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
    qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
    qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    QFile *file = new QFile("C:/Dummy/downloaded.txt");
    if(file->open(QFile::WriteOnly))
    {
      file->write(reply->readAll());
      file->flush();
      file->close();
    }
    delete file;
  }

  // reply->deleteLater();
  delete reply;
}
