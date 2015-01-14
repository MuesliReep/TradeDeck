#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
}

MainWindow::~MainWindow() {
  
    delete ui;
}

void MainWindow::setExchangeBots(ExchangeBot *E) {

  e = E;

  // Connect bot signals to gui
  connect(e,SIGNAL(sendNewMarketData()),this,SLOT(receiveNewMarketData()));
}

// Slots

void MainWindow::receiveNewMarketData() {

  qDebug() << "new Market Data received";
}
