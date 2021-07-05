#include "settingdialog.h"
#include "ui_settingdialog.h"

#include<QTableWidgetItem>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    auto keys = Settings::getInstance().getKeys();
    for (auto key = keys.begin(); key != keys.end(); key++ ){
        itemList.append(Settings::getInstance().getSettingItem(*key));
    }
    qSort(itemList);

    ui->settingsTable->setRowCount(itemList.size());
    ui->settingsTable->horizontalHeader()->setVisible(true);
    for (int i = 0; i < itemList.size(); i++){

        SettingItem item = itemList.at(i);

        // キー
        QTableWidgetItem *keyItem = new QTableWidgetItem();
        keyItem->setText(item.key);
        keyItem->setFlags(Qt::ItemIsEnabled);
        keyItem->setBackgroundColor(QColor(245, 245, 245));
        ui->settingsTable->setItem(i, 0, keyItem);

        // 表示名
        QTableWidgetItem *nameItem = new QTableWidgetItem();
        nameItem->setText(item.displayName);
        nameItem->setFlags(Qt::ItemIsEnabled);
        nameItem->setBackgroundColor(QColor(245, 245, 245));
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
    for (int i = 0; i < itemList.size(); i++){

        // 値
        QTableWidgetItem *valueItem = new QTableWidgetItem();
        if (itemList[i].type == "bool"){
            Qt::CheckState checkstate = itemList[i].value.toBool() ? Qt::Checked : Qt::Unchecked;
            valueItem->setCheckState(checkstate);
            valueItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        }else{
            valueItem->setText(itemList[i].defaultValue.toString());
            valueItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        }
        ui->settingsTable->setItem(i, 2, valueItem);
    }
}


void SettingDialog::on_buttonBox_accepted()
{
     for (int i = 0; i < itemList.size(); i++){

         // 値
         QTableWidgetItem *valueItem = ui->settingsTable->item(i, 2);

         // 型チェック

         if (itemList[i].type == "bool"){
             SETTINGS_SET(itemList[i].key, (valueItem->checkState() == Qt::Checked));
         }else{
             SETTINGS_SET(itemList[i].key, valueItem->text());
         }
     }

     accept();
}


void SettingDialog::on_buttonBox_rejected()
{
    reject();
}

