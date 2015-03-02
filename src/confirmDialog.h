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
  ConfirmDialog(double Price, double Amount, double Total, int type, QWidget *parent = 0);
  ~ConfirmDialog();

private:
  // QHBoxLayout *footerLayout;
  // QVBoxLayout *bodyLayout;
  // QHBoxLayout *headerLayout;

private slots:
  void closeDialog();
  void tradeConfirmed();

signals:
  void confirmTradeRequest(double Price, double Amount, double Total, int type);

};

#endif // CONFIRMDIALOG_H
