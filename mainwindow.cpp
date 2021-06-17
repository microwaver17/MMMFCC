#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioDeviceInfo>
#include <QFileDialog>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(
                this,
                u8"音声ファイルを選択してください",
                u8"C:\\Users\\yuya\\Documents\\devel\\mmmfcc\\test_compute-mfcc",
                u8"音声ファイル (*.wav *.mp3 *.m4a *.oga *.opus)"
);
    if (filepath == nullptr){
        return;
    }

    mmmfcc.setAudioFilePath(filepath);
}

