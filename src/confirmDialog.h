#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QLabel>

class ConfirmDialog : public QDialog
{
  Q_OBJECT

public:
  ConfirmDialog(double Price, double Amount, double Total, int Type, QWidget *parent = 0);
  ~ConfirmDialog();

private:
  // QHBoxLayout *footerLayout;
  // QVBoxLayout *bodyLayout;
  // QHBoxLayout *headerLayout;
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
