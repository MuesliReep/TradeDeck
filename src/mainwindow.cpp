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
  connect(e,SIGNAL(sendNewMarketData(int)),this,SLOT(receiveNewMarketData(int)));
}

//
void MainWindow::updateTradeList() {

  ui->listWidgetTrades->clear();

  QList<Trade> trades = e->getMarketData()->getTrades();

  if(trades.size()>0) {

    for(int i=0;i<trades.size()&&i<50;i++) {

      QString time;
      QString value;
      QString amount;

      // Convert timestamp to date time
      QDateTime dateTime;
      dateTime.setTime_t(trades[i].getTimeStamp());
      time = dateTime.toString("hh:mm:ss");

      // Convert value & amount to strings
      value.setNum(trades[i].getPrice());
      amount.setNum(trades[i].getAmount());

      new QListWidgetItem(time+" \t "+value+" \t "+amount, ui->listWidgetTrades);
    }

    QString price;
    price.setNum(trades[0].getPrice());

    ui->labelLastTradeValue->setText(price);
  }
}

//
void MainWindow::updateTradeDepth() {

  ui->listWidgetAsks->clear();
  ui->listWidgetBids->clear();

  QList<Order> asks = e->getMarketData()->getAsks();
  QList<Order> bids = e->getMarketData()->getBids();

  if(asks.size()>0) {

    QString pair1;
    QString pair2;

    for(int i=0;i<asks.size()&&i<15;i++) {

      pair1.setNum(asks[i].getPair1());
      pair2.setNum(asks[i].getPair2());

      new QListWidgetItem(pair1+" / "+pair2, ui->listWidgetAsks);
    }
  }

  if(bids.size()>0) {

    QString pair1;
    QString pair2;

    for(int i=0;i<asks.size()&&i<15;i++) {

      pair1.setNum(bids[i].getPair1());
      pair2.setNum(bids[i].getPair2());

      new QListWidgetItem(pair1+" / "+pair2, ui->listWidgetBids);
    }
  }
}

// Slots

void MainWindow::receiveNewMarketData(int dataType) {

  // Update the individual screen elements according to the data type
  // 0 = Trade data
  // 1 = Depth data
  // 2 = Ticker data
  switch(dataType) {

    case 0:
      updateTradeList();
      qDebug() << "new Trade Data received";
      break;
    case 1:
      updateTradeDepth();
      qDebug() << "new Depth Data received";
      break;
    case 2:
      break;
  }
}
