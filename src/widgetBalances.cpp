#include "widgetBalances.h"
#include "ui_widgetBalances.h"

WidgetBalances::WidgetBalances(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBalances)
{
    ui->setupUi(this);
}

WidgetBalances::~WidgetBalances()
{
    delete ui;
}
