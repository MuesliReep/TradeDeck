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

  ui->listWidgetAsks->clear();
  ui->listWidgetBids->clear();

  QList<Order> asks = e->getMarketData()->getAsks();
  QList<Order> bids = e->getMarketData()->getBids();

  if(asks.size()>0) {

    QString pair1;
    QString pair2;

    for(int i=0;i<asks.size();i++) {

      pair1.setNum(asks[i].getPair1());
      pair2.setNum(asks[i].getPair2());

      new QListWidgetItem(pair1+" / "+pair2, ui->listWidgetAsks);
    }
  }

  if(bids.size()>0) {

    QString pair1;
    QString pair2;

    for(int i=0;i<asks.size();i++) {

      pair1.setNum(bids[i].getPair1());
      pair2.setNum(bids[i].getPair2());

      new QListWidgetItem(pair1+" / "+pair2, ui->listWidgetBids);
    }
  }

}
