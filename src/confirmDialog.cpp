#include "confirmDialog.h"

ConfirmDialog::ConfirmDialog(double Price, double Amount, double Total, int type, QWidget *parent)
  : QDialog(parent) {

    int frameStyle = QFrame::Sunken | QFrame::Panel;

    setWindowFlags( Qt::CustomizeWindowHint );


    // Create widgets

    //----------------------------------//
    //          Header widgets          //
    //----------------------------------//
    QWidget *headerWidget     = new QWidget;
    QHBoxLayout *headerLayout = new QHBoxLayout;
    QLabel *headerLabel       = new QLabel(tr("Please confirm order"));
    QPushButton *cancelButton = new QPushButton(tr("X"));

    headerLayout->addWidget(headerLabel);
    headerLayout->addWidget(cancelButton);

    // Customize widgets
    headerWidget->setMinimumSize(0, 40);
    headerWidget->setMaximumHeight(40);

    cancelButton->setMinimumSize(40, 40);
    cancelButton->setMaximumSize(40, 40);

    headerLayout->setContentsMargins(0,0,0,0);

    // Set the layout
    headerWidget->setLayout(headerLayout);

    //----------------------------------//
    //           Body widgets           //
    //----------------------------------//
    QWidget *bodyWidget       = new QWidget;
    QWidget *amountWidget     = new QWidget;
    QWidget *priceWidget      = new QWidget;
    QWidget *totalWidget      = new QWidget;
    QVBoxLayout *bodyLayout   = new QVBoxLayout;
    QHBoxLayout *amountLayout = new QHBoxLayout;
    QHBoxLayout *priceLayout  = new QHBoxLayout;
    QHBoxLayout *totalLayout  = new QHBoxLayout;
    QLabel *amountLabel       = new QLabel("Amount: ");
    QLabel *priceLabel        = new QLabel("Price: ");
    QLabel *totalLabel        = new QLabel("Total: ");
    QLabel *amountValueLabel  = new QLabel;
    QLabel *priceValueLabel   = new QLabel;
    QLabel *totalValueLabel   = new QLabel;
    QLabel *orderTypeLabel    = new QLabel("Buy");
    if(type == 1)
      orderTypeLabel->setText("Sell");
    // Line *topLine            = new Line;
    // Line *botLine            = new Line;

    amountLayout->addWidget(amountLabel);
    amountLayout->addWidget(amountValueLabel);
    priceLayout->addWidget(priceLabel);
    priceLayout->addWidget(priceValueLabel);
    totalLayout->addWidget(totalLabel);
    totalLayout->addWidget(totalValueLabel);

    amountWidget->setLayout(amountLayout);
    priceWidget->setLayout(priceLayout);
    totalWidget->setLayout(totalLayout);

    bodyLayout->addWidget(orderTypeLabel);
    // bodyLayout->addWidget(topLine);
    bodyLayout->addWidget(amountWidget);
    bodyLayout->addWidget(priceWidget);
    bodyLayout->addWidget(totalWidget);
    // bodyLayout->addWidget(botLine);

    // Customize widgets
    amountLabel->setAlignment(Qt::AlignRight);
    priceLabel->setAlignment(Qt::AlignRight);
    totalLabel->setAlignment(Qt::AlignRight);

    amountLabel->setMaximumWidth(150);
    priceLabel->setMaximumWidth(150);
    totalLabel->setMaximumWidth(150);

    amountValueLabel->setAlignment(Qt::AlignLeft);
    priceValueLabel->setAlignment(Qt::AlignLeft);
    totalValueLabel->setAlignment(Qt::AlignLeft);
    orderTypeLabel->setAlignment(Qt::AlignLeft);

    orderTypeLabel->setStyleSheet("font: 20pt;");


    // Set the layout
    bodyWidget->setLayout(bodyLayout);

    //----------------------------------//
    //          Footer widgets          //
    //----------------------------------//
    QWidget *footerWidget       = new QWidget;
    QHBoxLayout *footerLayout   = new QHBoxLayout;
    QPushButton *confirmButton  = new QPushButton(tr("CONFIRM"));

    footerLayout->addWidget(confirmButton);

    // Customize footer widgets
    confirmButton->setMinimumSize(0, 40);
    confirmButton->setMaximumHeight(40);

    // footerLayout->setMaximumHeight(40);
    headerLayout->setContentsMargins(9,0,9,0);

    footerWidget->setLayout(footerLayout);

    // Set style sheets
    QString headerStyle("QWidget {background-color:rgb(47, 61, 69); } QLabel {color:rgb(183,190,195); padding: 0 5px; font: 16;} QPushButton{background-color: rgb(244, 67, 54); border-style: none; color: rgb(255,255,255); font: 16px;} QPushButton:pressed{ background-color: rgb(47, 61, 69);}");
    QString bodyStyle("QWidget {background-color:rgb(30, 43, 52);  color: rgb(183,190,195);} QLineEdit{border:1px solid gray; padding: 0 5px; font:16px;} QPushButton{background-color: rgb(137,145,152); border-style: none; font: 16px;} QPushButton:pressed{ background-color: rgb(30, 43, 52);}");
    QString widgetStyle("QWidget {background-color:rgb(30, 43, 52); }");

    this->setStyleSheet(widgetStyle);
    headerWidget->setStyleSheet(headerStyle);
    bodyWidget->setStyleSheet(bodyStyle);
    footerWidget->setStyleSheet(bodyStyle);

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);

    // Add widgets to layout
    layout->addWidget(headerWidget);
    layout->addWidget(bodyWidget);
    layout->addWidget(footerWidget);

    // Set the dialog layout
    setLayout(layout);
    setMinimumSize(300, 300);

    // Set the content
    QString price;
    QString amount;
    QString total;

    price.setNum(Price,'f',3);
    amount.setNum(Amount,'f',8);
    total.setNum(Total,'f',3);

    price.append(" USD");
    amount.append(" BTC");
    total.append(" USD");

    priceValueLabel->setText(price);
    amountValueLabel->setText(amount);
    totalValueLabel->setText(total);

    // Connect button signals
    connect(cancelButton,  SIGNAL(clicked()),this,SLOT(closeDialog()));
    connect(confirmButton, SIGNAL(clicked()),this,SLOT(tradeConfirmed()));

    // Connect confirmation signal
    connect(this, SIGNAL(confirmTradeRequest(double, double, double, int)), parent, SLOT());
}

ConfirmDialog::~ConfirmDialog() {
  // delete headerLayout;
  // delete bodyLayout;
  // delete footerLayout;
}

// Closes the dialog box and cancels the trade
void ConfirmDialog::closeDialog() {
  close();
}

// Confirmes the requested trade
void ConfirmDialog::tradeConfirmed() {

}
