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

    connect(&mmmfcc, &MmMfcc::updatePosition, this, &MainWindow::updateSeekbar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSeekbar(){
    int position = (float)mmmfcc.getPlayPositon() / mmmfcc.getPlayDuration() * ui->seekbarSlider->maximum();
    ui->seekbarSlider->setValue(position);
}

void MainWindow::on_openAudioFile_clicked()
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


void MainWindow::on_playPauseButton_clicked()
{
    mmmfcc.togglePlayPause();
}


void MainWindow::on_seekbarSlider_sliderReleased()
{
    int position = (float)ui->seekbarSlider->value() / ui->seekbarSlider->maximum() * mmmfcc.getPlayDuration();
    mmmfcc.setPlayPosition(position);
}


void MainWindow::on_seekBackButton_clicked()
{
    int position = mmmfcc.getPlayPositon() - 1000;
    mmmfcc.setPlayPosition(position);
}


void MainWindow::on_seekForwardButton_clicked()
{
    int position = mmmfcc.getPlayPositon() + 1000;
    mmmfcc.setPlayPosition(position);
}

