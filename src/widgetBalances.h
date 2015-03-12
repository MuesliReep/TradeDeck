#ifndef WIDGETBALANCES_H
#define WIDGETBALANCES_H

#include <QWidget>

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
};

#endif // WIDGETBALANCES_H
