#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setupPlot();



   // ui->labelExchangeMarket->setFont();

    QPalette palette = ui->labelExchangeMarket->palette();
    palette.setColor(ui->labelExchangeMarket->foregroundRole(), QColor(227, 242, 253));
    ui->labelExchangeMarket->setPalette(palette);

    palette = ui->tradeDepthWidget->palette();
    palette.setColor(QPalette::Background, QColor(187, 222, 251));
    ui->tradeDepthWidget->setPalette(palette);

    palette = ui->tradesWidget->palette();
    palette.setColor(QPalette::Background, QColor(187, 222, 251));
    ui->tradesWidget->setPalette(palette);

    // Colour headers
    QColor headerColour(33, 150, 243);

    palette = ui->headerWidget->palette();
    palette.setColor(QPalette::Background, headerColour);
    ui->headerWidget->setPalette(palette);

    palette = ui->tradesWidgetHeader->palette();
    palette.setColor(QPalette::Background, headerColour);
    ui->tradesWidgetHeader->setPalette(palette);

    palette = ui->tradeDepthWidgetHeader->palette();
    palette.setColor(QPalette::Background, headerColour);
    ui->tradeDepthWidgetHeader->setPalette(palette);

    palette = ui->balancesWidgetHeader->palette();
    palette.setColor(QPalette::Background, headerColour);
    ui->balancesWidgetHeader->setPalette(palette);

    palette = ui->ordersWidgetHeader->palette();
    palette.setColor(QPalette::Background, headerColour);
    ui->ordersWidgetHeader->setPalette(palette);
}

MainWindow::~MainWindow() {

    // delete candlesticks;
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

    // Update the current price and set the correct colour
    QString oldPrice = ui->labelLastTradeValue->text();
    QString price;

    price.setNum(trades[0].getPrice());
    ui->labelLastTradeValue->setText(price);

    QPalette palette = ui->labelLastTradeValue->palette();
    if(oldPrice.toDouble() > price.toDouble())
        palette.setColor(ui->labelLastTradeValue->foregroundRole(), QColor(244, 67, 54));
    else
        palette.setColor(ui->labelLastTradeValue->foregroundRole(), QColor(76, 175, 80));

    ui->labelLastTradeValue->setPalette(palette);
  }
}

//
void MainWindow::setupPlot() {

  ui->tradePlot->legend->setVisible(true);

  candlesticks = new QCPFinancial(ui->tradePlot->xAxis, ui->tradePlot->yAxis);
  ui->tradePlot->addPlottable(candlesticks);

  candlesticks->setName("Candlestick");
  candlesticks->setChartStyle(QCPFinancial::csCandlestick);
  candlesticks->setWidth(0.9);
  candlesticks->setTwoColored(true);
  candlesticks->setBrushPositive(QColor(76, 175, 80));
  candlesticks->setBrushNegative(QColor(244, 67, 54));
  candlesticks->setPenPositive(QPen(QColor(0, 0, 0)));
  candlesticks->setPenNegative(QPen(QColor(0, 0, 0)));

  ui->tradePlot->xAxis->setRange(0, 270);
  ui->tradePlot->yAxis->setRange(210, 230);

  ui->tradePlot->replot();

  // Setup volume graph

}

//
void MainWindow::scaleTradePlot() {

  QList<DataPoint> dataPoints = e->getMarketData()->getPriceList();

  double high = dataPoints[0].getHigh();
  double low  = dataPoints[0].getLow();

  for(int i = 0; i < dataPoints.size(); i++) {

    if(dataPoints[i].getHigh() > high)
      high  = dataPoints[i].getHigh();
    if(dataPoints[i].getLow() < low && dataPoints[i].getLow() != 0)
      low   = dataPoints[i].getLow();
  }

  ui->tradePlot->yAxis->setRange((int)high+3, (int)low-3);
}

//
void MainWindow::updateTradePlot() {

  // Retrieve the latest data
  QList<DataPoint> dataPoints = e->getMarketData()->getPriceList();

  candlesticks->clearData();

  double key = 1;
  for(int i = dataPoints.size()-1; i >= 0; i--) {

    candlesticks->addData(key, dataPoints[i].getOpen(), dataPoints[i].getHigh(), dataPoints[i].getLow(), dataPoints[i].getClose());

    // Add volume information

    key++;
  }

  scaleTradePlot();

  ui->tradePlot->replot();
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

    int j = asks.size();

    if(asks.size() >= 15)
      j = 15;

    for(int i=j-1;i>=0;i--) {

      pair1.setNum(asks[i].getPair1());
      pair2.setNum(asks[i].getPair2());

      // new QListWidgetItem(pair1+" \t "+pair2, ui->listWidgetAsks);

      QListWidgetItem *newItem = new QListWidgetItem;
      newItem->setText(pair1+" \t "+pair2);
      ui->listWidgetAsks->addItem(newItem);
    }
  } else
    new QListWidgetItem("No orders", ui->listWidgetAsks);

  if(bids.size()>0) {

    QString pair1;
    QString pair2;

    for(int i=0;i<asks.size()&&i<15;i++) {

      pair1.setNum(bids[i].getPair1());
      pair2.setNum(bids[i].getPair2());

      new QListWidgetItem(pair1+" \t "+pair2, ui->listWidgetBids);
    }
  } else
    new QListWidgetItem("No orders", ui->listWidgetBids);

  // listWidget sizes
  ui->listWidgetAsks->setMinimumHeight(ui->listWidgetAsks->sizeHintForRow(0)*15+5);
  ui->listWidgetAsks->setMaximumHeight(ui->listWidgetAsks->sizeHintForRow(0)*15+5);

  ui->listWidgetBids->setMinimumHeight(ui->listWidgetAsks->sizeHintForRow(0)*15+5);
  ui->listWidgetBids->setMaximumHeight(ui->listWidgetAsks->sizeHintForRow(0)*15+5);
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
      updateTradePlot();
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
