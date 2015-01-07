#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QtNetwork/qnetworkrequest.h>

#include "Downloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
