#include "confirmDialog.h"
#include "ui_confirmDialog.h"

ConfirmDialog::ConfirmDialog(double Price, double Amount, double Total, int Type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);

    // Set the content
    QString priceS;
    QString amountS;
    QString totalS;

    priceS.setNum(Price,'f',3);
    amountS.setNum(Amount,'f',8);
    totalS.setNum(Total,'f',3);

    priceS.append(" USD");
    amountS.append(" BTC");
    totalS.append(" USD");

    ui->priceValueLabel->setText(priceS);
    ui->amountValueLabel->setText(amountS);
    ui->totalValueLabel->setText(totalS);

    if(type == 1)
      ui->orderTypeLabel->setText("Sell");

    // Connect button signals
    connect(ui->cancelButton,  SIGNAL(clicked()),this,SLOT(closeDialog()));
    connect(ui->confirmButton, SIGNAL(clicked()),this,SLOT(orderConfirmed()));

    // Connect confirmation signal
    connect(this, SIGNAL(confirmOrderRequest(double, double, int)), parent, SLOT(receiveOrderConfirmed(double, double, int)));
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
    // delete headerLayout;
    // delete bodyLayout;
    // delete footerLayout;
}

// Closes the dialog box and cancels the trade
void ConfirmDialog::closeDialog() {

  close();
}

// Confirmes the requested order
void ConfirmDialog::orderConfirmed() {

  // Send confirm signal to UI
  confirmOrderRequest(price, amount, type);

  // Close this dialog
  close();
}
