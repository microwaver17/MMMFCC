﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "settings.h"

#include <QAudioDeviceInfo>
#include <QFileDialog>
#include <QtDebug>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mmmfcc(this)
{
    ui->setupUi(this);

    Settings &stgs = Settings::getInstance();

    Graph &graph = mmmfcc.getGraph();
    ui->graphGraphicsView->setScene(&graph.getScene());
    graph.setSceneSize(ui->graphGraphicsView->width(), ui->graphGraphicsView->height());
    ui->graphScalelSlider->setValue(stgs.default_scale * 1000);
    ui->autoScalecheckBox->setChecked(stgs.isAutoScale);

    connect(&Log::getInstance(), &Log::logAdded, this, &MainWindow::updateLog);
    connect(&mmmfcc.getPlayer(), &QMediaPlayer::positionChanged, this, &MainWindow::updateSeekbar);

    updateLog();

    devicesInfo = QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioInput);
    for (int i = 0; i < devicesInfo.size(); i++){
        ui->inputDeviceComboBox->addItem(devicesInfo.at(i).deviceName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    mmmfcc.getGraph().setSceneSize(ui->graphGraphicsView->width(), ui->graphGraphicsView->height());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);

    int key = event->key();
    Qt::KeyboardModifiers modif = event->modifiers();

    if (key == Qt::Key::Key_0 && modif == Qt::KeyboardModifier::KeypadModifier){
        on_playPauseButton_clicked();
    }else if(key == Qt::Key_Home){
        on_seekTopButton_clicked();
    }else if(key == Qt::Key_Minus && modif == Qt::KeyboardModifier::KeypadModifier){
        on_seekBackButton_clicked();
    }else if(key == Qt::Key_Plus && modif == Qt::KeyboardModifier::KeypadModifier){
        on_seekForwardButton_clicked();
    }
}

void MainWindow::updateSeekbar(){
    QMediaPlayer &player = mmmfcc.getPlayer();
    int position = (float)player.position() / player.duration() * ui->seekbarSlider->maximum();
    ui->seekbarSlider->setValue(position);
}

void MainWindow::updateLog()
{
    auto logs = Log::getInstance().getLogs();
    QString logtext = "";
    for (int i = 0; i < logs.size(); i++){
        auto log = logs.at(i);
        logtext += log["message"] + " (" + log["class"] + ")";
        if (i != logs.size() - 1){
            logtext += '\n';
        }
    }
    ui->logTextArea->setPlainText(logtext);
    int max = ui->logTextArea->verticalScrollBar()->maximum();
    ui->logTextArea->verticalScrollBar()->setValue(max);
    qDebug() << "Log updated";
}

void MainWindow::on_openAudioFile_clicked()
{    QString filepath = QFileDialog::getOpenFileName(
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
    QMediaPlayer &player = mmmfcc.getPlayer();
    if (player.state() == QMediaPlayer::PlayingState){
        player.pause();
    }else{
        player.play();
    }
}

void MainWindow::on_seekbarSlider_sliderReleased()
{
    QMediaPlayer &player = mmmfcc.getPlayer();
    int position = (float)ui->seekbarSlider->value() / ui->seekbarSlider->maximum() * player.duration();
    player.setPosition(position);
}

void MainWindow::on_seekbarSlider_sliderPressed()
{
    QMediaPlayer &player = mmmfcc.getPlayer();
    int position = (float)ui->seekbarSlider->value() / ui->seekbarSlider->maximum() * player.duration();
    player.setPosition(position);
}

void MainWindow::on_seekBackButton_clicked()
{
    QMediaPlayer &player = mmmfcc.getPlayer();
    int position = player.position() - 100;
    player.pause();
    player.setPosition(position);
}


void MainWindow::on_seekForwardButton_clicked()
{
    QMediaPlayer &player = mmmfcc.getPlayer();
    int position = player.position() + 100;
    player.pause();
    player.setPosition(position);
}


void MainWindow::on_seekTopButton_clicked()
{
    mmmfcc.getPlayer().setPosition(0);
}


void MainWindow::on_freeze1Button_clicked()
{
    mmmfcc.getGraph().freeze1Graph();
}

void MainWindow::on_freeze2Button_clicked()
{
    mmmfcc.getGraph().freeze2Graph();
}

void MainWindow::on_selectSourceMicButton_clicked()
{
    mmmfcc.getTranslator().setSource(Translator::Source::Device);
}

void MainWindow::on_selectSourceFileButton_clicked()
{
    mmmfcc.getTranslator().setSource(Translator::Source::File);
}


void MainWindow::on_toggleGraphButton_clicked()
{
    Graph &graph = mmmfcc.getGraph();
    graph.isHideCurrentGraph = (graph.isHideCurrentGraph == false);
}


void MainWindow::on_inputDeviceComboBox_currentIndexChanged(int index)
{
    QAudioDeviceInfo info = devicesInfo.at(index);
    mmmfcc.setAudioDevice(info);
}


void MainWindow::on_graphScalelSlider_valueChanged(int value)
{
    double scale = value / 1000.0;
    mmmfcc.getGraph().setScale(scale);
}


void MainWindow::on_autoScalecheckBox_stateChanged(int state)
{
    if (state == Qt::CheckState::Checked){
        mmmfcc.getGraph().isAutoScele = true;
    }else{
        mmmfcc.getGraph().isAutoScele = false;
    }
}


void MainWindow::on_graphTypeLineButton_clicked()
{
    mmmfcc.getGraph().graphType = Graph::GraphType::Line;
}


void MainWindow::on_graphTypeBarButton_clicked()
{
    mmmfcc.getGraph().graphType = Graph::GraphType::Bar;
}

