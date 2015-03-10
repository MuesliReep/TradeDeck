#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ConfirmDialog(double Price, double Amount, double Total, int Type, QWidget *parent = 0);
  ~ConfirmDialog();

private:
  Ui::ConfirmDialog *ui;

  double price;
  double amount;
  int type;

private slots:
  void closeDialog();
  void orderConfirmed();

signals:
  void confirmOrderRequest(double Price, double Amount, int type);

};

#endif // CONFIRMDIALOG_H
