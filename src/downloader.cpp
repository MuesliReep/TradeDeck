#include "downloader.h"

Downloader::Downloader(QObject *parent) :
QObject(parent) {

  manager = new QNetworkAccessManager(this);
  inUse   = false;
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
void Downloader::doDownload(QNetworkRequest request, QObject * receiver, const char * method) {

  connect(manager, SIGNAL(finished(QNetworkReply*)),
  receiver, method);

  manager->get(request);
}

// Checks the validity of a network reply
bool Downloader::checkReply(QNetworkReply *reply) {

  if(reply->error()) {

    qDebug() << "ERROR!";
    qDebug() << reply->errorString();

    return false;
  }
  else {

    qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
    qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
    qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    return true;
  }
}

QNetworkAccessManager* Downloader::getManager() { return manager; }

// //
// void Downloader::doDownload(QNetworkRequest request) {
//
//   manager = new QNetworkAccessManager(this);
//
//   connect(manager, SIGNAL(finished(QNetworkReply*)),
//   this, SLOT(replyFinished(QNetworkReply*)));
//
//   manager->get(request);
// }
//
// //
// void Downloader::replyFinished (QNetworkReply *reply) {
//
//   if(reply->error())
//   {
//     qDebug() << "ERROR!";
//     qDebug() << reply->errorString();
//   }
//   else
//   {
//     qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
//     qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
//     qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
//     qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//     qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
//
//     QFile *file = new QFile("downloadedData.json");
//     if(file->open(QFile::WriteOnly))
//     {
//       file->write(reply->readAll());
//       file->flush();
//       file->close();
//     }
//     delete file;
//   }
//
//   reply->deleteLater();
//   // delete reply;
// }
