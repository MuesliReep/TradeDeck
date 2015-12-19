#ifndef WIDGETTRADES_H
#define WIDGETTRADES_H

#include <QWidget>

namespace Ui {
class WidgetTrades;
}

class WidgetTrades : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTrades(QWidget *parent = 0);
    ~WidgetTrades();

private:
    Ui::WidgetTrades *ui;
};

#endif // WIDGETTRADES_H
