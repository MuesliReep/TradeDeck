#include "settingsDialog.h"
#include "ui_settingsDialog.h"

SettingsDialog::SettingsDialog(Config *C, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    c = C;

    loadSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::loadSettings() {

  QString apiKey = c->getApiKey();
  QString secret = c->getApiSecret();

  ui->apiKeyLineEdit->setText(apiKey);
  ui->secretLineEdit->setText(secret);
}

void SettingsDialog::saveSettings() {

  // Get settings from each field
  QString apiKey = ui->apiKeyLineEdit->text();
  QString secret = ui->secretLineEdit->text();

  // Write them to config
  c->setApiKey(apiKey);
  c->setApiSecret(secret);

  // Save to file
}
