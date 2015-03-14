#include "widgetBalances.h"
#include "ui_widgetBalances.h"

WidgetBalances::WidgetBalances(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBalances)
{
    ui->setupUi(this);

    setup();
}

WidgetBalances::~WidgetBalances()
{
    delete ui;
}

void WidgetBalances::setup() {

  QColor textLightColour(183,190,195);
  QColor textMediumColour(137,145,152);

  ui->tableWidgetBalances->itemAt(0,0)->setTextColor(textMediumColour);
  ui->tableWidgetBalances->itemAt(1,0)->setTextColor(textMediumColour);

  ui->tableWidgetBalances->itemAt(0,1)->setTextColor(textLightColour);
  ui->tableWidgetBalances->itemAt(1,1)->setTextColor(textLightColour);
}

void WidgetBalances::updateBalances(QList<Balance> *balances) {

  QString usd;
  QString btc;

  usd.setNum((*balances)[0].getAmount(),'f',8);
  btc.setNum((*balances)[1].getAmount(),'f',8);

  ui->tableWidgetBalances->item(0,1)->setText(usd);
  ui->tableWidgetBalances->item(1,1)->setText(btc);
}
