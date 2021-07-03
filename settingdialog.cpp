#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "settings.h"

#include<QTableWidgetItem>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    auto keys = Settings::getInstance().getKeys();
    ui->settingsTable->setRowCount(keys.size());
    ui->settingsTable->horizontalHeader()->setVisible(true);
    for (int i = 0; i < keys.size(); i++){

        SettingItem item = Settings::getInstance().getSettingItem(keys[i]);

        // キー
        QTableWidgetItem *keyItem = new QTableWidgetItem();
        keyItem->setText(keys[i]);
        keyItem->setFlags(Qt::ItemIsEnabled);
        ui->settingsTable->setItem(i, 0, keyItem);

        // 表示名
        QTableWidgetItem *nameItem = new QTableWidgetItem();
        nameItem->setText(item.displayName);
        nameItem->setFlags(Qt::ItemIsEnabled);
        ui->settingsTable->setItem(i, 1, nameItem);

        // 値
        QTableWidgetItem *valueItem = new QTableWidgetItem();
        if (item.type == "bool"){
            Qt::CheckState checkstate = item.value.toBool() ? Qt::Checked : Qt::Unchecked;
            valueItem->setCheckState(checkstate);
            valueItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        }else{
            valueItem->setText(item.value.toString());
            valueItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        }
        ui->settingsTable->setItem(i, 2, valueItem);
    }
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_defaultButton_clicked()
{
    auto keys = Settings::getInstance().getKeys();
    ui->settingsTable->setRowCount(keys.size());
    for (int i = 0; i < keys.size(); i++){

        SettingItem item = Settings::getInstance().getSettingItem(keys[i]);

        // 値
        QTableWidgetItem *valueItem = new QTableWidgetItem();
        if (item.type == "bool"){
            Qt::CheckState checkstate = item.value.toBool() ? Qt::Checked : Qt::Unchecked;
            valueItem->setCheckState(checkstate);
            valueItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        }else{
            valueItem->setText(item.defaultValue.toString());
            valueItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        }
        ui->settingsTable->setItem(i, 2, valueItem);
    }
}


void SettingDialog::on_buttonBox_accepted()
{
    auto keys = Settings::getInstance().getKeys();
     for (int i = 0; i < keys.size(); i++){

         SettingItem item = Settings::getInstance().getSettingItem(keys[i]);

         // 値
         QTableWidgetItem *valueItem = ui->settingsTable->item(i, 2);

         // 型チェック


         if (item.type == "bool"){
             SETTINGS_SET(keys[i], (valueItem->checkState() == Qt::Checked));
         }else{

             SETTINGS_SET(keys[i], valueItem->text());
         }
         ui->settingsTable->setItem(i, 2, valueItem);
     }

     accept();
}


void SettingDialog::on_buttonBox_rejected()
{
    reject();
}

