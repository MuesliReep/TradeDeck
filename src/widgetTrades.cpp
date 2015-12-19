#include "widgetTrades.h"
#include "ui_widgetTrades.h"

WidgetTrades::WidgetTrades(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTrades)
{
    ui->setupUi(this);
}

WidgetTrades::~WidgetTrades()
{
    delete ui;
}
