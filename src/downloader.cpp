#include "downloader.h"

Downloader::Downloader(QObject *parent) :
QObject(parent)
{
}

Downloader::~Downloader() {

  delete manager;
}


// Generates a network request based on the given url
QNetworkRequest Downloader::generateRequest(QUrl url) {

  return QNetworkRequest(url);
}

// Generates a HTTP GET request
QNetworkRequest Downloader::generateGetRequest(QUrl url) {

  return QNetworkRequest(url);
}

// Generates a HTTP POST request
QNetworkRequest Downloader::generatePostRequest(QUrl url) {

  return QNetworkRequest(url);
}

//
void Downloader::doDownload(QNetworkRequest request) {

  manager = new QNetworkAccessManager(this);

  connect(manager, SIGNAL(finished(QNetworkReply*)),
  this, SLOT(replyFinished(QNetworkReply*)));

  // manager->get(QNetworkRequest(QUrl("https://btc-e.com/api/3/depth/btc_usd")));
  manager->get(request);
}

//
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

    QFile *file = new QFile("downloadedData.json");
    if(file->open(QFile::WriteOnly))
    {
      file->write(reply->readAll());
      file->flush();
      file->close();
    }
    delete file;
  }

  reply->deleteLater();
  // delete reply;
}

void Downloader::setConfig(Config *C) { c=C; }
