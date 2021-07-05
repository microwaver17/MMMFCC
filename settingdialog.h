#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "settings.h"

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private slots:
    void on_defaultButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SettingDialog *ui;

    QList<SettingItem> itemList;
};

#endif // SETTINGDIALOG_H
