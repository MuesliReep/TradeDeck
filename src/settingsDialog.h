#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "config.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Config *C, QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;

    Config *c;

    void loadSettings();
    void saveSettings();
};

#endif // SETTINGSDIALOG_H
