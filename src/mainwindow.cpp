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
    QString headerStyle("QWidget {background-color:rgb(47, 61, 69);} QLabel {color:rgb(183,190,195);} QPushButton{background-color: rgb(244, 67, 54); border-style: none; color: rgb(255,255,255); font: 16px;} QPushButton:pressed{ background-color: rgb(47, 61, 69);}");
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

  // Connect bot signals to UI
  connect(e,SIGNAL(sendActiveOrders(QList<Order>)),   this,SLOT(receiveActiveOrders(QList<Order>)));
  connect(e,SIGNAL(sendOrderHistory(QList<Order>)),   this,SLOT(receiveOrderHistory(QList<Order>)));
  connect(e,SIGNAL(sendBalances(QList<Balance>)),     this,SLOT(receiveBalances(QList<Balance>)));
  connect(e,SIGNAL(sendTicker(Ticker)),               this,SLOT(receiveTicker(Ticker)));
  connect(e,SIGNAL(sendTradeDepth(TradeDepth)),       this,SLOT(receiveTradeDepth(TradeDepth)));
  connect(e,SIGNAL(sendTradeHistory(QList<Trade>,QList<DataPoint>,QList<QList<double> >)),  this,SLOT(receiveTradeHistory(QList<Trade>,QList<DataPoint>,QList<QList<double> >)));
  connect(e,SIGNAL(sendTransactionHistory()),         this,SLOT(receiveTransactionHistory()));
  connect(e,SIGNAL(sendMessage(int, QString)),        this,SLOT(receiveMessage(int, QString)));

  // Connect UI signals to bot
  connect(this,SIGNAL(sendCancelOrder(uint)),                e,SLOT(receiveCancelOrder(uint)));
  connect(this,SIGNAL(sendCreateOrder(int, double, double)), e,SLOT(receiveCreateOrder(int, double, double)));
  connect(this,SIGNAL(checkBalance(int, double, bool*)),     e,SLOT(checkBalance(int, double, bool*)));
}

//
void MainWindow::calculateMinimumBuyTrade(double sellPrice, double sellAmount, double fee, double *buyPrice, double *buyAmount, double *buyTotal, double profit) {

  double x = 0.00000001;
  double i = 0;

  i = profit + sellAmount;
  i = i / (1.0-fee/100.0);
  i = ((sellPrice * sellAmount) * (1.0-fee/100.0)) / i;
  i = (sellPrice - i) / x;
  i++; // +1 to go over break even point

  profit = ((((sellPrice * sellAmount) * (1.0-fee/100.0)) / (sellPrice - i*x)) * (1.0-fee/100.0)) - sellAmount;

  qDebug() << "i= " << i << "\tprofit= " << profit;

  *buyAmount  = (((sellPrice * sellAmount) * (1.0-fee/100.0)) / (sellPrice - i*x));
  *buyPrice   = sellPrice - i * x;
  *buyTotal   = *buyAmount * *buyPrice;

  qDebug() << "\t Buy Amount: \t" << *buyAmount << "\t BTC";
  qDebug() << "\t Buy Price: \t" << *buyPrice << "\t USD";
  qDebug() << "\t Buy Total: \t" << *buyTotal << "\t USD";
}

//
double calculateMinimumSellTrade(double buyPrice, double buyAmount, double fee, double profit) {

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

  ui->tradePlot->yAxis2->setVisible(true);
  ui->tradePlot->yAxis->setVisible(false); // TODO: set visible for volume plot

  candlesticks = new QCPFinancial(ui->tradePlot->xAxis, ui->tradePlot->yAxis2);
  ui->tradePlot->addPlottable(candlesticks);

  candlesticks->setName("Candlestick");
  candlesticks->setChartStyle(QCPFinancial::csCandlestick);
  candlesticks->setWidth(32); // 0.5
  candlesticks->setTwoColored(true);
  candlesticks->setBrushPositive(QColor(76, 175, 80));
  candlesticks->setBrushNegative(QColor(244, 67, 54));
  candlesticks->setPenPositive(QPen(QColor(76, 175, 80)));
  candlesticks->setPenNegative(QPen(QColor(244, 67, 54)));

  // Set initial ranges, these are updated when new data arrives
  ui->tradePlot->xAxis->setRange(0, 250);
  ui->tradePlot->yAxis->setRange(0, 500);
  ui->tradePlot->yAxis2->setRange(0, 500);

  ui->tradePlot->setBackground(bodyColour);

  ui->tradePlot->xAxis->setBasePen(QPen(lineColour, 1));
  ui->tradePlot->yAxis->setBasePen(QPen(lineColour, 1));
  ui->tradePlot->yAxis2->setBasePen(QPen(lineColour, 1));
  ui->tradePlot->xAxis->setTickPen(QPen(lineColour, 1));
  ui->tradePlot->yAxis->setTickPen(QPen(lineColour, 1));
  ui->tradePlot->yAxis2->setTickPen(QPen(lineColour, 1));
  ui->tradePlot->xAxis->setSubTickPen(QPen(lineColour, 1));
  ui->tradePlot->yAxis->setSubTickPen(QPen(lineColour, 1));
  ui->tradePlot->yAxis2->setSubTickPen(QPen(lineColour, 1));
  ui->tradePlot->xAxis->setTickLabelColor(textMediumColour);
  ui->tradePlot->yAxis->setTickLabelColor(textMediumColour);
  ui->tradePlot->yAxis2->setTickLabelColor(textMediumColour);
  ui->tradePlot->xAxis->grid()->setPen(QPen(gridColour, 1, Qt::DotLine));
  ui->tradePlot->yAxis->grid()->setPen(QPen(gridColour, 1, Qt::DotLine));
  ui->tradePlot->yAxis2->grid()->setPen(QPen(gridColour, 1, Qt::DotLine));


  ui->tradePlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->tradePlot->xAxis->setDateTimeFormat("hh:mm");



  // Setup moving averages

  MA1 =  ui->tradePlot->addGraph(ui->tradePlot->xAxis, ui->tradePlot->yAxis2);
  MA2 =  ui->tradePlot->addGraph(ui->tradePlot->xAxis, ui->tradePlot->yAxis2);

  MA1->setPen(QPen(textLightColour, 1));
  MA2->setPen(QPen(textMediumColour, 1));

  ui->tradePlot->replot();

  // Setup volume graph

  // <<<<<<<<<<<<<<<<<<<< Sub ticks moeten binsize worden???
  ui->tradePlot->xAxis->setAutoTickStep(false);
  ui->tradePlot->xAxis->setTickStep(60*30); // One hour
  ui->tradePlot->xAxis->setSubTickCount(2);

}

//
void MainWindow::setupOrdersTable() {

  QStringList labels;
  labels  << "Time" << "Type" << "Amount" << "Price" << "Cancel";

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

  // Main header
  connect(ui->pushButtonExchangeSettings, SIGNAL(clicked()),this,SLOT(settingsButtonPressed()));


  // Buy/Sell widget
  connect(ui->lineEditBuyAmount,  SIGNAL(textChanged(const QString)),this,SLOT(buyTotalChanged(const QString)));
  connect(ui->lineEditBuyPrice,   SIGNAL(textChanged(const QString)),this,SLOT(buyTotalChanged(const QString)));
  connect(ui->pushButtonBuy,      SIGNAL(clicked()),this,SLOT(buyButtonPressed()));

  connect(ui->lineEditSellAmount, SIGNAL(textChanged(const QString)),this,SLOT(sellTotalChanged(const QString)));
  connect(ui->lineEditSellPrice,  SIGNAL(textChanged(const QString)),this,SLOT(sellTotalChanged(const QString)));
  connect(ui->pushButtonSell,     SIGNAL(clicked()),this,SLOT(sellButtonPressed()));

  // textChanged(const QString & text)

  // Trade calc widget
  connect(ui->lineEditCalcSellAmount, SIGNAL(textEdited(const QString)),this,SLOT(calcSellAmountChanged()));
  connect(ui->lineEditCalcSellPrice,  SIGNAL(textEdited(const QString)),this,SLOT(calcSellPriceChanged()));
  connect(ui->lineEditCalcSellTotal,  SIGNAL(textEdited(const QString)),this,SLOT(calcSellTotalValueChanged()));
  connect(ui->lineEditCalcFee,        SIGNAL(textEdited(const QString)),this,SLOT(calcFeeValueChanged()));
  connect(ui->lineEditCalcBuyAmount,  SIGNAL(textEdited(const QString)),this,SLOT(calcBuyAmountChanged()));
  connect(ui->lineEditCalcBuyPrice,   SIGNAL(textEdited(const QString)),this,SLOT(calcBuyPriceChanged()));
  connect(ui->lineEditCalcBuyTotal,   SIGNAL(textEdited(const QString)),this,SLOT(calcBuyTotalValueChanged()));
  connect(ui->lineEditCalcProfit,     SIGNAL(textEdited(const QString)),this,SLOT(calcProfitValueChanged()));

  // Orders
  connect(ui->pushButtonCancelOrder,  SIGNAL(clicked()),this,SLOT(cancelOrderButtonPressed()));
}

//----------------------------------//
//         Update functions         //
//----------------------------------//

// TODO: this function should be in a shared class
uint MainWindow::findClosestBin(uint desiredTime, uint binSize) {

  return desiredTime - (desiredTime % binSize) + binSize;
}

//
void MainWindow::scaleTradePlot(QList<DataPoint> *binnedTradeData) {

  // QList<DataPoint> dataPoints = e->getMarketData()->getPriceList();

  double high = (*binnedTradeData)[0].getHigh();
  double low  = (*binnedTradeData)[0].getLow();

  for(int i = 0; i < binnedTradeData->size(); i++) {

    if((*binnedTradeData)[i].getHigh() > high)
      high  = (*binnedTradeData)[i].getHigh();
    if((*binnedTradeData)[i].getLow() < low && (*binnedTradeData)[i].getLow() != 0)
      low   = (*binnedTradeData)[i].getLow();
  }

  // ui->tradePlot->yAxis->setRange((int)high+2, (int)low-2);
  ui->tradePlot->yAxis2->setRange((int)high+2, (int)low-2);

  // Next update the yAxis
  uint binSize = 60;

  uint now  = findClosestBin((uint)QDateTime::currentDateTime().toTime_t(), binSize);
  uint then = findClosestBin(now-250*60, binSize);
  ui->tradePlot->xAxis->setRange(then, now); // TODO: num bins needs to be defined in settings
}

//
void MainWindow::updateTradeList(QList<Trade> *tradeData) {

  // Clear the current trades
  ui->tableWidgetTrades->clear();

  // Get the latest trade list
  // QList<Trade> trades = e->getMarketData()->getTrades();

  // Set the max. number of trades to be shown
  int listSize = 100;

  // Update the trade table
  if(tradeData->size()>0) {

    // Set the number of rows
    if(tradeData->size()<listSize)
      ui->tableWidgetTrades->setRowCount(tradeData->size());
    else
      ui->tableWidgetTrades->setRowCount(listSize);

    // Iterate through the trade list and add them to the table
    for(int i=0;i<tradeData->size()&&i<50;i++) {

      QString time;
      QString value;
      QString amount;

      // Convert timestamp to date time
      QDateTime dateTime;
      dateTime.setTime_t((*tradeData)[i].getTimeStamp());
      time = dateTime.toString("hh:mm:ss");

      // Convert value & amount to strings
      value.setNum((*tradeData)[i].getPrice());
      amount.setNum((*tradeData)[i].getAmount()); //TODO: pad with zeros

      ui->tableWidgetTrades->setItem(i, 0, new QTableWidgetItem(time));
      ui->tableWidgetTrades->setItem(i, 1, new QTableWidgetItem(value));
      ui->tableWidgetTrades->setItem(i, 2, new QTableWidgetItem(amount));

      ui->tableWidgetTrades->item(i,1)->setTextColor(QColor(76, 175, 80));

      if((*tradeData)[i].getPrice() < (*tradeData)[i+1].getPrice() && (i+1) < (*tradeData).size())
        ui->tableWidgetTrades->item(i,1)->setTextColor(QColor(244, 67, 54));
    }

    // Update the current price and set the correct colour
    QString oldPrice = ui->labelLastTradeValue->text();
    QString price;

    price.setNum((*tradeData)[0].getPrice());
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
void MainWindow::updateTradePlot(QList<DataPoint> *binnedTradeData, QList<QList<double> > *MAList) {

  // Retrieve the latest data
  // QList<DataPoint> dataPoints = e->getMarketData()->getPriceList();

  candlesticks->clearData();
  MA1->clearData();
  MA2->clearData();

  QList<double> MA1List = (*MAList)[0];
  QList<double> MA2List = (*MAList)[1];

  double key = 1;
  for(int i = binnedTradeData->size()-1; i >= 0; i--) {

    key = (*binnedTradeData)[i].getTimeStamp();

    // Update candlesticks
    candlesticks->addData(key, (*binnedTradeData)[i].getOpen(), (*binnedTradeData)[i].getHigh(), (*binnedTradeData)[i].getLow(), (*binnedTradeData)[i].getClose());

    // TODO: Add volume information

    // Update moving averages
    MA1->addData(key, MA1List[i]);
    MA2->addData(key, MA2List[i]);

    // key++;
  }

  scaleTradePlot(binnedTradeData);

  ui->tradePlot->replot();
}

//
void MainWindow::updateTradeDepth(TradeDepth *t) {

  // Clear the current data
  ui->tableWidgetAsks->clear();
  ui->tableWidgetBids->clear();

  // Set the header titels
  QStringList labels;
  labels << "Price" << "Amount";

  ui->tableWidgetAsks->setHorizontalHeaderLabels(labels);

  // Retrieve new data from the exchange bot
  QList<Order> asks = t->getAsks();
  QList<Order> bids = t->getBids();

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
void MainWindow::updateBalances(QList<Balance> *balances) {

  QString usd;
  QString btc;

  usd.setNum((*balances)[0].getAmount(),'f',8);
  btc.setNum((*balances)[1].getAmount(),'f',8);

  ui->tableWidgetBalances->item(0,1)->setText(usd);
  ui->tableWidgetBalances->item(1,1)->setText(btc);
}

//
void MainWindow::updateOrders(QList<Order> *activeOrders) {

  // TODO: only clear when no orders
  // Clear old data
  if(ui->tableWidgetOrders->rowCount() == 0 && activeOrders->isEmpty()) {

    ui->tableWidgetOrders->clear();

    // TODO: Check why header is deleted on clear
    QStringList labels;
    labels  << "Time" << "Type" << "Amount" << "Price" << "OrderID";

    ui->tableWidgetOrders->setHorizontalHeaderLabels(labels);
  }

  // QList<Order> orders = e->getActiveOrders();

  // First check if the already present orders are still present in the new list
  for(int i = 0; i < ui->tableWidgetOrders->rowCount(); i++) {

    bool alive = false;

    for(int j = 0; j < activeOrders->size(); j++) {

      if(ui->tableWidgetOrders->item(i, 4)->text().toUInt() == (*activeOrders)[j].getOrderID()) {

        alive = true;
        break;
      }
    }

    // Remove from table
    if(!alive)
      ui->tableWidgetOrders->removeRow(i);
  }

  // TODO: remove orders already present from orders for efficiency
  ui->tableWidgetOrders->setRowCount(activeOrders->size());

  // For each order add it to the table
  for(int i = 0; i < activeOrders->size(); i++) {

    // Check if this order already exists, otherwise add it
    if(ui->tableWidgetOrders->rowCount() == 0) {
      for(int j = 0; j < activeOrders->size(); j++) {

        if(ui->tableWidgetOrders->item(j, 4)->text().toUInt() == (*activeOrders)[i].getOrderID())
          continue;
      }
    }

    QString time; QDateTime dateTime;
    dateTime.setTime_t((*activeOrders)[i].getTimeStamp());
    time = dateTime.toString("dd/MM hh:mm:ss");
    QString type("Buy");
    if((*activeOrders)[i].getType() == 1)
      type = "Sell";
    QString amount;     amount.setNum((*activeOrders)[i].getPair1()); amount.append(" BTC");
    QString price;      price.setNum((*activeOrders)[i].getPair2()); price.append(" USD");
    QString remaining;  remaining.setNum(-1); remaining.append(" BTC");
    QString status;     status.setNum(-1);
    QString orderID;    orderID.setNum((*activeOrders)[i].getOrderID());

    // Add items to row
    ui->tableWidgetOrders->setItem(i, 0, new QTableWidgetItem(time));
    ui->tableWidgetOrders->setItem(i, 1, new QTableWidgetItem(type));
    ui->tableWidgetOrders->setItem(i, 2, new QTableWidgetItem(amount));
    ui->tableWidgetOrders->setItem(i, 3, new QTableWidgetItem(price));
    ui->tableWidgetOrders->setItem(i, 4, new QTableWidgetItem(orderID));

    // ui->tableWidgetOrders->setItem(i, 4, new QTableWidgetItem(remaining)); // TODO: Remaining should be in popup dialog
    // ui->tableWidgetOrders->setItem(i, 5, new QTableWidgetItem(status));
  }

  // Remove cancel button when no orders are present & return it when there are
  if(activeOrders->size() == 0 && ui->pushButtonCancelOrder->isVisible())
    ui->pushButtonCancelOrder->setVisible(false);
  else if(activeOrders->size() > 0 && !ui->pushButtonCancelOrder->isVisible())
    ui->pushButtonCancelOrder->setVisible(true);
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
  uint currency = 0;

  // Check USD balance
  bool result;
  checkBalance(currency, amount, &result);

  if(result && amount != 0.0) {

    qDebug() << "Buy " << amount << "BTC, at " << price << "USD/BTC";

    // Reset the UI input
    ui->lineEditBuyAmount->setText("0");

    // Create dialog window to confirm order
    ConfirmDialog *dialog = new ConfirmDialog(price, amount, total, 0, this);
    dialog->exec();

  } else {

    qDebug() << "Insufficient funds for buy order";

    // TODO: Show error dialog
  }
}

//
void MainWindow::sellButtonPressed() {

  // Get values
  double price  = ui->lineEditSellPrice->text().toDouble();
  double amount = ui->lineEditSellAmount->text().toDouble();
  double total  = price * amount;
  uint currency = 1;

  // Check BTC balance
  bool result;
  checkBalance(currency, amount, &result);

  if(result && amount != 0.0) {

    qDebug() << "Sell " << amount << "BTC, at " << price << "USD/BTC";

    // Reset the UI input
    ui->lineEditSellAmount->setText("0");

    // Create dialog window to confirm order
    ConfirmDialog *dialog = new ConfirmDialog(price, amount, total, 1, this);
    dialog->exec();

  } else {

    qDebug() << "Insufficient funds for sell order";

    // TODO: Show error dialog
  }
}

//
void MainWindow::cancelOrderButtonPressed() {

  // Get selected items
  // QList<QTableWidgetItem *> selectedOrders = ui->tableWidgetOrders->selectedItems();

  QItemSelectionModel *selection = ui->tableWidgetOrders->selectionModel();
  QModelIndexList selectedOrders = selection->selectedIndexes();

  // Process each item
  for(int i = 0; i < selectedOrders.size(); i++) {

    // Get the row ID
    int rowID = selectedOrders[i].row();

    // Get the orderID from table
    uint orderID = ui->tableWidgetOrders->item(rowID, 4)->text().toUInt();

    // Remove items from table
    ui->tableWidgetOrders->removeRow(rowID);

    // Send cancel to exchange
    sendCancelOrder(orderID); //TODO: emitted 4 times?
  }
}

//
void MainWindow::settingsButtonPressed() {

  SettingsDialog *dialog = new SettingsDialog();
  dialog->exec();
}

//
void MainWindow::calcValueChanged(int value) {

  double sellAmount = ui->lineEditCalcSellAmount->text().toDouble();
  double sellPrice  = ui->lineEditCalcSellPrice->text().toDouble();
  double sellTotal  = ui->lineEditCalcSellTotal->text().toDouble();

  double buyAmount  = ui->lineEditCalcBuyAmount->text().toDouble();
  double buyPrice   = ui->lineEditCalcBuyPrice->text().toDouble();
  double buyTotal   = ui->lineEditCalcBuyTotal->text().toDouble();

  double fee = ui->lineEditCalcFee->text().toDouble();
  double sellFee  = 0.0;
  double buyFee   = 0.0;

  double profit = ui->lineEditCalcProfit->text().toDouble();

  switch(value) {

    case 0: // sell amount or price changed
      sellTotal = sellAmount * sellPrice;
    break;
    case 1: // buy amount or price changed
      buyTotal = buyAmount * buyPrice;
    break;
    case 2: // sell total changed
      sellAmount = sellTotal / sellPrice;
    break;
    case 3: // buy total changed
      buyAmount = buyTotal / buyPrice;
    break;
    case 4: // fee percentage changed
    break;
    case 5: // profit changed
    break;
  }

  if(value == 0 || value == 2 || value == 4 || value == 6) { // Calculate minimum buy amount
    calculateMinimumBuyTrade(sellPrice, sellAmount, fee, &buyPrice, &buyAmount, &buyTotal);
  }
  else if (value == 1 || value == 5 || value == 3) { // Calculate minimum sell amount
    // calculateMinimumSellTrade(buyPrice, buyAmount, fee);
  }
  else if (value == 7) {
    calculateMinimumBuyTrade(sellPrice, sellAmount, fee, &buyPrice, &buyAmount, &buyTotal, profit);
  }
  else{
    qDebug() << "bad value";
  }

  sellFee = sellTotal * (fee / 100);
  buyFee  = buyTotal  * (fee / 100);

  QString sSellAmount;
  QString sSellPrice;
  QString sSellTotal;
  QString sBuyAmount;
  QString sBuyPrice;
  QString sBuyTotal;
  QString sFee;
  QString sSellFee;
  QString sBuyFee;

  // Only update fields that were not changed by the user:
  if(value != 5)
    ui->lineEditCalcBuyTotal->setText(sBuyTotal.setNum(buyTotal,'f',3));
  if(value != 3)
    ui->lineEditCalcBuyPrice->setText(sBuyPrice.setNum(buyPrice,'f',3));
  if(value != 1)
    ui->lineEditCalcBuyAmount->setText(sBuyAmount.setNum(buyAmount,'f',8));
  if(value != 6)
    ui->lineEditCalcFee->setText(sFee.setNum(fee,'f',8));
  if(value != 4)
    ui->lineEditCalcSellTotal->setText(sSellTotal.setNum(sellTotal,'f',3));
  if(value != 2)
    ui->lineEditCalcSellPrice->setText(sSellPrice.setNum(sellPrice,'f',3));
  if(value != 0)
    ui->lineEditCalcSellAmount->setText(sSellAmount.setNum(sellAmount,'f',8));
  ui->labelCalcFeeSellAmount->setText(sSellFee.setNum(sellFee,'f',8));
  ui->labelCalcFeeBuyAmount->setText(sBuyFee.setNum(buyFee,'f',8));

}

void MainWindow::calcSellAmountChanged()      { calcValueChanged(0); }
void MainWindow::calcBuyAmountChanged()       { calcValueChanged(1); }
void MainWindow::calcSellPriceChanged()       { calcValueChanged(2); }
void MainWindow::calcBuyPriceChanged()        { calcValueChanged(3); }
void MainWindow::calcSellTotalValueChanged()  { calcValueChanged(4); }
void MainWindow::calcBuyTotalValueChanged()   { calcValueChanged(5); }
void MainWindow::calcFeeValueChanged()        { calcValueChanged(6); }
void MainWindow::calcProfitValueChanged()     { calcValueChanged(7); }
void MainWindow::calcUseButtonPressed()       { }

//----------------------------------//
//           Public Slots           //
//----------------------------------//

// Receives a signal which confirmes a user wishes to make the selected order
// Can be called from a dialog or from any other privileged source
void MainWindow::receiveOrderConfirmed(double price, double amount, int type) {

  qDebug() << "UI Received order was confirmed by user";

  sendCreateOrder(type, price, amount);
}

//
// void MainWindow::receiveNewMarketData(int dataType) {
//
//   // Update the individual screen elements according to the data type
//   // 0 = Trade data
//   // 1 = Depth data
//   // 2 = Ticker data
//   // 3 = info data
//   // 4 = new trade
//   // 5 = active orders
//   // 6 = order info
//   // 7 = cancel order
//   // 8 = trade history
//   // 9 = trans history
//   switch(dataType) {
//
//     case 0:
//       updateTradeList();
//       updateTradePlot();
//       qDebug() << "new Trade Data received";
//       break;
//     case 1:
//       updateTradeDepth();
//       qDebug() << "new Depth Data received";
//       break;
//     case 2:
//       break;
//     case 3:
//       updateBalances();
//       // updateOrders();
//       qDebug() << "new info Data received";
//       break;
//     case 5:
//       updateOrders();
//       qDebug() << "new active order Data received";
//       break;
//     default:
//       qDebug() << "UI received unknown data: " << dataType;
//       break;
//   }
// }

void MainWindow::receiveActiveOrders(QList<Order> activeOrders) { updateOrders(&activeOrders); }
void MainWindow::receiveOrderHistory(QList<Order> orderHistory) { }
void MainWindow::receiveBalances(QList<Balance> balances) { updateBalances(&balances); }
void MainWindow::receiveTicker(Ticker ticker) { }
void MainWindow::receiveTradeDepth(TradeDepth tradeDepth) { updateTradeDepth(&tradeDepth); }
void MainWindow::receiveTradeHistory(QList<Trade> tradeData, QList<DataPoint> binnedTradeData, QList<QList<double> > MAList) { updateTradeList(&tradeData); updateTradePlot(&binnedTradeData, &MAList); }
void MainWindow::receiveTransactionHistory() { }
void MainWindow::receiveMessage(int type, QString message) { }
