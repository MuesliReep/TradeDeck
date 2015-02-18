#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // Setup UI widgets
    setupPlot();
    setupOrdersTable();
    setupTradesTable();
    setupAsksTable();
    setupBidsTable();

    // Setup UI signals & slots
    setupUISignals();

    // Set colours
    QColor headerColour(47, 61, 69);
    QColor bodyColour(30, 43, 52);
    QColor backgroundColour(21, 35, 44);
    QColor textColour(183,190,195);

    QString widgetStyle("QWidget {background-color:rgb(30, 43, 52);  color: rgb(183,190,195);} QLineEdit{border:1px solid gray; padding: 0 5px; font:12px} QPushButton{background-color: rgb(137,145,152); border-style: none; font: 16px} QPushButton:pressed{ background-color: rgb(30, 43, 52);}");
    QString headerStyle("QWidget {background-color:rgb(47, 61, 69);} QLabel {color:rgb(183,190,195);}");
    QString tableStyle("QTableWidget {gridline-color: rgb(52, 64, 73); background-color: rgb(30, 43, 52); color: rgb(183,190,195);} QHeaderView {background-color:rgb(30, 43, 52); color: rgb(183,190,195);} QHeaderView::section {background-color:rgb(30, 43, 52);}");
    QString tabStyle("QWidget {background-color: rgb(47, 61, 69); color: rgb(183,190,195);} QLineEdit{border:1px solid gray; padding: 0 5px; font:12px} QPushButton{background-color: rgb(137,145,152); border-style: none; font: 16px} QPushButton:pressed{ background-color: rgb(30, 43, 52);}  QTabWidget::tab{background-color: rgb(47, 61, 69);} QTabWidget::pane{border:0px;} QTabBar::tab {min-width: 125px; min-height: 40px; font: 16px} QTabBar::tab:selected {background-color: rgb(47, 61, 69);} QTabBar::tab:!selected {background-color: rgb(30, 43, 52);} QTabWidget::tab-bar {color: rgb(183,190,195);}");

    QPalette palette;

    // Set widget body colours

    // ui->tradeDepthWidget->setStyleSheet(widgetStyle);
    // ui->tradesWidget->setStyleSheet(widgetStyle);
    // ui->balancesWidget->setStyleSheet(widgetStyle);
    // ui->ordersWidget->setStyleSheet(widgetStyle);
    // ui->priceChartWidget->setStyleSheet(widgetStyle);
    // ui->buySellWidget->setStyleSheet(widgetStyle);
    ui->calcWidget->setStyleSheet(widgetStyle);

    palette = ui->tradeDepthWidget->palette();
    palette.setColor(QPalette::Background, bodyColour);
    ui->tradeDepthWidget->setPalette(palette);

    palette = ui->tradesWidget->palette();
    palette.setColor(QPalette::Background, bodyColour);
    ui->tradesWidget->setPalette(palette);

    palette = ui->balancesWidget->palette();
    palette.setColor(QPalette::Background, bodyColour);
    ui->balancesWidget->setPalette(palette);

    palette = ui->ordersWidget->palette();
    palette.setColor(QPalette::Background, bodyColour);
    ui->ordersWidget->setPalette(palette);

    palette = ui->priceChartWidget->palette();
    palette.setColor(QPalette::Background, bodyColour);
    ui->priceChartWidget->setPalette(palette);

    palette = ui->buySellWidget->palette();
    palette.setColor(QPalette::Background, bodyColour);
    ui->buySellWidget->setPalette(palette);

    palette = ui->calcWidget->palette();
    palette.setColor(QPalette::Background, bodyColour);
    ui->calcWidget->setPalette(palette);

    // Set the header styles

    ui->headerWidget->setStyleSheet(headerStyle);
    ui->tradesWidgetHeader->setStyleSheet(headerStyle);
    ui->tradeDepthWidgetHeader->setStyleSheet(headerStyle);
    ui->balancesWidgetHeader->setStyleSheet(headerStyle);
    ui->ordersWidgetHeader->setStyleSheet(headerStyle);
    ui->priceChartWidgetHeader->setStyleSheet(headerStyle);
    ui->calcWidgetHeader->setStyleSheet(headerStyle);

    // Set table styles
    ui->tableWidgetBalances->setStyleSheet(tableStyle);
    ui->tableWidgetOrders->setStyleSheet(tableStyle);
    ui->tableWidgetTrades->setStyleSheet(tableStyle);
    ui->tableWidgetAsks->setStyleSheet(tableStyle);
    ui->tableWidgetBids->setStyleSheet(tableStyle);

    // ui->tableWidgetBalances->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    ui->tableWidgetBalances->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetOrders->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetTrades->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetAsks->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetBids->horizontalHeader()->setStretchLastSection(true);

    // Set tab styles
    ui->tabWidget->setStyleSheet(tabStyle);
    ui->tabWidget->widget(0)->setStyleSheet(tabStyle);
    ui->tabWidget->widget(1)->setStyleSheet(tabStyle);

    // TODO: set QTabBar style
    //geometry

  //  ui->tabWidget->tabBar()->
}

MainWindow::~MainWindow() {

    delete ui;
}

//
void MainWindow::setExchangeBots(ExchangeBot *E) {

  e = E;

  // Connect bot signals to gui
  connect(e,SIGNAL(sendNewMarketData(int)),this,SLOT(receiveNewMarketData(int)));
}

//
bool MainWindow::checkBalance(int pair, double amount) {

  switch(pair) {
    case 0:
      if(amount < e->getBTCBalance())
        return true;
    case 1:
      if(amount < e->getUSDBalance())
        return true;
  }

  return false;
}

//
double MainWindow::calculateMinimumBuyTrade(double sellPrice, double sellAmount, double fee) {

  double sellTotal  = sellPrice * sellAmount;
  double sellFee    = sellTotal * (fee / 100.0);
  double nettoSell  = sellTotal - sellFee;

  double x = 0.000001;
  int i = 1;
  double buyPrice = 0;
  double buyTotal = 0;
  double buyFee   = -1;
  double nettoBuy = 0;

  while(nettoBuy < nettoSell) {

    buyPrice  = sellPrice - (i * x);
    buyTotal  = nettoSell / buyPrice;
    buyFee    = buyTotal * (fee / 100.0);
    nettoBuy  = buyTotal - buyFee;
    i++;
  }

  qDebug() << "Minimum buy back price: " << buyPrice << "USD";

  return nettoBuy;
}

//
double calculateMinimumSellTrade(double buyPrice, double buyAmount, double fee) {

  return 0.0;
}

//----------------------------------//
//         Setup functions          //
//----------------------------------//

//
void MainWindow::setupPlot() {

  // Set colours

  QColor bodyColour(30, 43, 52);
  QColor lineColour(52, 64, 73);
  QColor gridColour(52, 64, 73);
  QColor textLightColour(183,190,195);
  QColor textMediumColour(137,145,152);

  // Turn on the legend
  // ui->tradePlot->legend->setVisible(true);

  candlesticks = new QCPFinancial(ui->tradePlot->xAxis, ui->tradePlot->yAxis);
  ui->tradePlot->addPlottable(candlesticks);

  candlesticks->setName("Candlestick");
  candlesticks->setChartStyle(QCPFinancial::csCandlestick);
  candlesticks->setWidth(0.5);
  candlesticks->setTwoColored(true);
  candlesticks->setBrushPositive(QColor(76, 175, 80));
  candlesticks->setBrushNegative(QColor(244, 67, 54));
  candlesticks->setPenPositive(QPen(QColor(76, 175, 80)));
  candlesticks->setPenNegative(QPen(QColor(244, 67, 54)));

  ui->tradePlot->xAxis->setRange(0, 250);
  ui->tradePlot->yAxis->setRange(0, 500);

  ui->tradePlot->setBackground(bodyColour);

  ui->tradePlot->xAxis->setBasePen(QPen(lineColour, 1));
  ui->tradePlot->yAxis->setBasePen(QPen(lineColour, 1));
  ui->tradePlot->xAxis->setTickPen(QPen(lineColour, 1));
  ui->tradePlot->yAxis->setTickPen(QPen(lineColour, 1));
  ui->tradePlot->xAxis->setSubTickPen(QPen(lineColour, 1));
  ui->tradePlot->yAxis->setSubTickPen(QPen(lineColour, 1));
  ui->tradePlot->xAxis->setTickLabelColor(textMediumColour);
  ui->tradePlot->yAxis->setTickLabelColor(textMediumColour);
  ui->tradePlot->xAxis->grid()->setPen(QPen(gridColour, 1, Qt::DotLine));
  ui->tradePlot->yAxis->grid()->setPen(QPen(gridColour, 1, Qt::DotLine));

  // Setup moving averages

  MA1 =  ui->tradePlot->addGraph();
  MA2 =  ui->tradePlot->addGraph();

  MA1->setPen(QPen(textLightColour, 1));
  MA2->setPen(QPen(textMediumColour, 1));

  ui->tradePlot->replot();

  // Setup volume graph

}

//
void MainWindow::setupOrdersTable() {

  QStringList labels;
  labels  << "Time" << "Type" << "Amount" << "Price" << "Remaining" << "Status" ;

  ui->tableWidgetOrders->setHorizontalHeaderLabels(labels);
}

//
void MainWindow::setupBalancesTable() {

  QColor textLightColour(183,190,195);
  QColor textMediumColour(137,145,152);

  ui->tableWidgetBalances->itemAt(0,0)->setTextColor(textMediumColour);
  ui->tableWidgetBalances->itemAt(1,0)->setTextColor(textMediumColour);

  ui->tableWidgetBalances->itemAt(0,1)->setTextColor(textLightColour);
  ui->tableWidgetBalances->itemAt(1,1)->setTextColor(textLightColour);

}

//
void MainWindow::setupTradesTable() {

  // Set column header titels
  QStringList labels;
  labels  << "Time" << "Price" << "Amount";

  ui->tableWidgetTrades->setHorizontalHeaderLabels(labels);
}

//
void MainWindow::setupAsksTable() {

  // Set column header titels
  QStringList labels;
  labels << "Price" << "Amount";

  ui->tableWidgetAsks->setHorizontalHeaderLabels(labels);
}

//
void MainWindow::setupBidsTable() {

}

//
void MainWindow::setupUISignals() {

  // Buy/Sell widget
  connect(ui->lineEditBuyAmount,SIGNAL(textChanged(const QString)),this,SLOT(buyTotalChanged(const QString)));
  connect(ui->lineEditBuyPrice,SIGNAL(textChanged(const QString)),this,SLOT(buyTotalChanged(const QString)));
  connect(ui->pushButtonBuy,SIGNAL(clicked()),this,SLOT(buyButtonPressed()));

  connect(ui->lineEditSellAmount,SIGNAL(textChanged(const QString)),this,SLOT(sellTotalChanged(const QString)));
  connect(ui->lineEditSellPrice,SIGNAL(textChanged(const QString)),this,SLOT(sellTotalChanged(const QString)));
  connect(ui->pushButtonSell,SIGNAL(clicked()),this,SLOT(sellButtonPressed()));

  // textChanged(const QString & text)
}

//----------------------------------//
//         Update functions         //
//----------------------------------//

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
void MainWindow::updateTradeList() {

  // Clear the current trades
  ui->tableWidgetTrades->clear();

  // Get the latest trade list
  QList<Trade> trades = e->getMarketData()->getTrades();

  // Set the max. number of trades to be shown
  int listSize = 100;

  // Update the trade table
  if(trades.size()>0) {

    // Set the number of rows
    if(trades.size()<listSize)
      ui->tableWidgetTrades->setRowCount(trades.size());
    else
      ui->tableWidgetTrades->setRowCount(listSize);

    // Iterate through the trade list and add them to the table
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
      amount.setNum(trades[i].getAmount()); //TODO: pad with zeros

      ui->tableWidgetTrades->setItem(i, 0, new QTableWidgetItem(time));
      ui->tableWidgetTrades->setItem(i, 1, new QTableWidgetItem(value));
      ui->tableWidgetTrades->setItem(i, 2, new QTableWidgetItem(amount));

      ui->tableWidgetTrades->item(i,1)->setTextColor(QColor(76, 175, 80));

      if(trades[i].getPrice() < trades[i+1].getPrice() && (i+1) < trades.size())
        ui->tableWidgetTrades->item(i,1)->setTextColor(QColor(244, 67, 54));
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

    // Update the current price in the buy sell tabs if amount is zero
    double buyAmount  = ui->lineEditBuyAmount->text().toDouble();
    double sellAmount = ui->lineEditSellAmount->text().toDouble();

    if(buyAmount == 0)
      ui->lineEditBuyPrice->setText(price);
    if(sellAmount == 0)
      ui->lineEditSellPrice->setText(price);
  }

  QStringList labels;
  labels  << "Time" << "Price" << "Amount";

  ui->tableWidgetTrades->setHorizontalHeaderLabels(labels);
}

//
void MainWindow::updateTradePlot() {

  // Retrieve the latest data
  QList<DataPoint> dataPoints = e->getMarketData()->getPriceList();

  candlesticks->clearData();
  MA1->clearData();
  MA2->clearData();

  QList<double> MA1List = e->getMarketData()->getMAList()[0];
  QList<double> MA2List = e->getMarketData()->getMAList()[1];

  double key = 1;
  for(int i = dataPoints.size()-1; i >= 0; i--) {

    // Update candlesticks
    candlesticks->addData(key, dataPoints[i].getOpen(), dataPoints[i].getHigh(), dataPoints[i].getLow(), dataPoints[i].getClose());

    // TODO: Add volume information

    // Update moving averages
    MA1->addData(key, MA1List[i]);
    MA2->addData(key, MA2List[i]);

    key++;
  }

  scaleTradePlot();

  ui->tradePlot->replot();
}

//
void MainWindow::updateTradeDepth() {

  // Clear the current data
  ui->tableWidgetAsks->clear();
  ui->tableWidgetBids->clear();

  // Set the header titels
  QStringList labels;
  labels << "Price" << "Amount";

  ui->tableWidgetAsks->setHorizontalHeaderLabels(labels);

  // Retrieve new data from the exchange bot
  QList<Order> asks = e->getMarketData()->getAsks();
  QList<Order> bids = e->getMarketData()->getBids();

  // Set the number of rows for both Asks and Bids
  int maxRowSize = 15;

  // Update the asks
  if(asks.size()>0) {

    QString pair1;
    QString pair2;

    int j = asks.size();

    if(asks.size() > maxRowSize)
      j = maxRowSize;

    ui->tableWidgetAsks->setRowCount(j);

    int k = 0 ; // Counter

    for(int i = j - 1; i >= 0; i--) {

      pair1.setNum(asks[i].getPair1());
      pair2.setNum(asks[i].getPair2());

      ui->tableWidgetAsks->setItem(k, 0, new QTableWidgetItem(pair1));
      ui->tableWidgetAsks->setItem(k, 1, new QTableWidgetItem(pair2));

      ui->tableWidgetAsks->item(k,1)->setTextColor(QColor(76, 175, 80));
      k++;
    }
  } else { // No ask orders available

    ui->tableWidgetAsks->setRowCount(1);
    ui->tableWidgetAsks->setItem(0, 0, new QTableWidgetItem("No orders"));
  }

  // Update the bids
  if(bids.size()>0) {

    QString pair1;
    QString pair2;

    int j = asks.size();

    if(asks.size() > maxRowSize)
      j = maxRowSize;

    ui->tableWidgetBids->setRowCount(j);

    for(int i = 0; i < j; i++) {

      pair1.setNum(bids[i].getPair1());
      pair2.setNum(bids[i].getPair2());

      ui->tableWidgetBids->setItem(i, 0, new QTableWidgetItem(pair1));
      ui->tableWidgetBids->setItem(i, 1, new QTableWidgetItem(pair2));

      ui->tableWidgetBids->item(i,1)->setTextColor(QColor(244, 67, 54));
    }
  } else { // No bid orders available

    ui->tableWidgetBids->setRowCount(1);
    ui->tableWidgetBids->setItem(0, 0, new QTableWidgetItem("No orders"));
  }

  // Set tableWidget sizes
  int rowSize     = 15; //ui->tableWidgetAsks->item(0,0)->sizeHint().rheight();
  int headerSize  = 15; //ui->tableWidgetAsks->horizontalHeaderItem(0)->sizeHint().rheight();
  int padding     = 5;

  ui->tableWidgetAsks->setMinimumHeight(headerSize + rowSize * maxRowSize + padding);
  ui->tableWidgetAsks->setMaximumHeight(headerSize + rowSize * maxRowSize + padding);

  ui->tableWidgetBids->setMinimumHeight(rowSize * maxRowSize + padding);
  ui->tableWidgetBids->setMaximumHeight(rowSize * maxRowSize + padding);
}

//
void MainWindow::updateBalances() {

  QString usd;
  QString btc;

  usd.setNum(e->getUSDBalance(),'g',8);
  btc.setNum(e->getBTCBalance(),'g',8);

  ui->tableWidgetBalances->item(0,1)->setText(usd);
  ui->tableWidgetBalances->item(1,1)->setText(btc);
}

//
void MainWindow::updateOrders() {

  // Clear old data
  ui->tableWidgetOrders->clear();

  QList<Order> orders = e->getActiveOrders();

  ui->tableWidgetOrders->setRowCount(orders.size());

  // For each order add it to the table
  for(int i = 0; i < orders.size(); i++) {

    QString time; QDateTime dateTime;
    dateTime.setTime_t(orders[i].getTimeStamp());
    time = dateTime.toString("hh:mm:ss");
    QString type;       type.setNum(orders[i].getType());
    QString amount;     amount.setNum(orders[i].getPair1());
    QString price;      price.setNum(orders[i].getPair2());
    QString remaining;  remaining.setNum(-1);
    QString status;     status.setNum(-1);

    ui->tableWidgetOrders->setItem(i, 0, new QTableWidgetItem(time));
    ui->tableWidgetOrders->setItem(i, 1, new QTableWidgetItem(type));
    ui->tableWidgetOrders->setItem(i, 2, new QTableWidgetItem(amount));
    ui->tableWidgetOrders->setItem(i, 3, new QTableWidgetItem(price));
    ui->tableWidgetOrders->setItem(i, 4, new QTableWidgetItem(remaining));
    ui->tableWidgetOrders->setItem(i, 5, new QTableWidgetItem(status));
  }

  // TODO: Check why header is deleted on clear
  QStringList labels;
  labels  << "Time" << "Type" << "Amount" << "Price" << "Remaining" << "Status" ;

  ui->tableWidgetOrders->setHorizontalHeaderLabels(labels);
}

//----------------------------------//
//          Private Slots           //
//----------------------------------//

//
void MainWindow::buyTotalChanged(const QString text) {

  QString totalText;
  totalText.setNum(ui->lineEditBuyAmount->text().toDouble() * ui->lineEditBuyPrice->text().toDouble());

  ui->labelBuyTotalAmount->setText(totalText);
}

//
void MainWindow::sellTotalChanged(const QString text) {

  QString totalText;
  totalText.setNum(ui->lineEditSellAmount->text().toDouble() * ui->lineEditSellPrice->text().toDouble());

  ui->labelSellTotalAmount->setText(totalText);
}

//
void MainWindow::buyButtonPressed() {

  // Get values
  double price  = ui->lineEditBuyPrice->text().toDouble();
  double amount = ui->lineEditBuyAmount->text().toDouble();
  double total  = price * amount;

  // Check USD balance
  if(checkBalance(1, total)) {

    qDebug() << "Buy " << amount << "BTC, at " << price << "USD/BTC";

    // Reset the UI input
    ui->lineEditBuyAmount->setText("0");

  } else {

    qDebug() << "Insufficient funds for buy order";

    //Show error dialog
  }
}

//
void MainWindow::sellButtonPressed() {

  // Get values
  double price  = ui->lineEditSellPrice->text().toDouble();
  double amount = ui->lineEditSellAmount->text().toDouble();
  double total  = price * amount;

  // Check BTC balance
  if(checkBalance(0, amount)) {

    qDebug() << "Sell " << amount << "BTC, at " << price << "USD/BTC";

    // Reset the UI input
    ui->lineEditSellAmount->setText("0");

  } else {

    qDebug() << "Insufficient funds for sell order";

    //Show error dialog
  }
}

//
void MainWindow::calcSellUpdate() {

  // Update total

  // Update sell total

  // Calculate minimum buy

}

//
void MainWindow::calcBuyUpdate() {

}

//
void MainWindow::calcUseButtonPressed() {

}

//----------------------------------//
//           Public Slots           //
//----------------------------------//

//
void MainWindow::receiveNewMarketData(int dataType) {

  // Update the individual screen elements according to the data type
  // 0 = Trade data
  // 1 = Depth data
  // 2 = Ticker data
  // 3 = info data
  // 4 = new trade
  // 5 = active orders
  // 6 = order info
  // 7 = cancel order
  // 8 = trade history
  // 9 = trans history
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
    case 3:
      updateBalances();
      updateOrders();
      break;
    case 5:
      updateOrders();
      qDebug() << "new active order Data received";
      break;
    default:
      qDebug() << "UI received unknown data: " << dataType;
      break;
  }
}
