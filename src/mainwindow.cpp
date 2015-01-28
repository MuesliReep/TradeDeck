#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setupPlot();
    setupOrdersTable();

    // Set colours
    QColor headerColour(47, 61, 69);
    QColor bodyColour(30, 43, 52);
    QColor backgroundColour(21, 35, 44);
    QColor textColour(255, 255, 255);

    QString widgetStyle("QWidget {background-color:rgb(47, 61, 69);} QListWidget::item { color: rgb(183,190,195); background-color:transparent; }");
    QString headerStyle("QWidget {background-color:rgb(47, 61, 69);} QLabel {color:rgb(255, 255, 255);}");
    QString tableStyle("QTableWidget {gridline-color: rgb(52, 64, 73); background-color: rgb(30, 43, 52);} QHeaderView::section {background-color:rgb(30, 43, 52);}");

    // ui->labelExchangeMarket->setFont();

    QPalette palette;

    // Set widget body colours

    // ui->tradeDepthWidget->setStyleSheet(widgetStyle);
    // ui->tradesWidget->setStyleSheet(widgetStyle);
    // ui->balancesWidget->setStyleSheet(widgetStyle);
    // ui->ordersWidget->setStyleSheet(widgetStyle);
    // ui->priceChartWidget->setStyleSheet(widgetStyle);
    // ui->buySellWidget->setStyleSheet(widgetStyle);

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

    // Set the header styles

    ui->headerWidget->setStyleSheet(headerStyle);
    ui->tradesWidgetHeader->setStyleSheet(headerStyle);
    ui->tradeDepthWidgetHeader->setStyleSheet(headerStyle);
    ui->balancesWidgetHeader->setStyleSheet(headerStyle);
    ui->ordersWidgetHeader->setStyleSheet(headerStyle);
    ui->priceChartWidgetHeader->setStyleSheet(headerStyle);
    ui->buySellWidgetHeader->setStyleSheet(headerStyle);

    // Set table styles
    ui->tableWidgetBalances->setStyleSheet(tableStyle);
    ui->tableWidgetOrders->setStyleSheet(tableStyle);
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

  ui->tradePlot->replot();

  // Setup volume graph

}

void MainWindow::setupOrdersTable() {

  QColor bodyColour(30, 43, 52);

  // QPalette palette = ui->tableWidgetOrders->palette();
  // palette.setColor(QPalette::Background, bodyColour);
  // ui->tableWidgetOrders->setPalette(palette);



//  background-color: QMainWindow

  QStringList labels;
  labels  << "Time" << "Type" << "Amount" << "Price" << "Remaining" << "Status" ;

  ui->tableWidgetOrders->setHorizontalHeaderLabels(labels);

  for(int i=0;i<6;i++) {

    QTableWidgetItem item;
    item.setText("Test");

    ui->tableWidgetOrders->setItem(1,i,&item);
  }
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

    // TODO: Add volume information

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
    default:
      qDebug() << "UI received unknown data";
      break;
  }
}
