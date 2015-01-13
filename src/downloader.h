#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QDateTime>


class Downloader : public QObject
{
  Q_OBJECT
  public:
    explicit Downloader(QObject *parent = 0);
    ~Downloader();


    QNetworkRequest generateRequest(QUrl url);
    QNetworkRequest generateGetRequest(QUrl url);
    QNetworkRequest generatePostRequest(QUrl url);

    void doDownload(QNetworkRequest request);
    void doDownload(QNetworkRequest request, QObject* receiver, const char * method);


  signals:

  public slots:
    void replyFinished (QNetworkReply *reply);

  private:
    QNetworkAccessManager *manager;

};

#endif // DOWNLOADER_H
