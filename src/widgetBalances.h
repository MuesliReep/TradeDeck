#ifndef WIDGETBALANCES_H
#define WIDGETBALANCES_H

#include <QWidget>

#include "exchangeBot.h"

namespace Ui {
class WidgetBalances;
}

class WidgetBalances : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBalances(QWidget *parent = 0);
    ~WidgetBalances();

private:
    Ui::WidgetBalances *ui;
    void setup();

public slots:
  void updateBalances(QList<Balance> *balances);

};

#endif // WIDGETBALANCES_H
