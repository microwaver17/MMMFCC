#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"

#include <QAudioDeviceInfo>
#include <QFileDialog>
#include <QtDebug>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mmmfcc(this)
    , player(mmmfcc.getPlayer())
{
    ui->setupUi(this);

    Graph &graph = mmmfcc.getMfccGraph();
    ui->graphGraphicsView->setScene(&graph.getScene());
    graph.setSceneSize(ui->graphGraphicsView->width(), ui->graphGraphicsView->height());
    graph.setMaxValue(6);

    connect(&Log::getInstance(), &Log::logAdded, this, &MainWindow::updateLog);
    connect(&mmmfcc, &MmMfcc::updatePosition, this, &MainWindow::updateSeekbar);

    updateLog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    mmmfcc.getMfccGraph().setSceneSize(ui->graphGraphicsView->width(), ui->graphGraphicsView->height());
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
    if (player.state() == QMediaPlayer::PlayingState){
        player.pause();
    }else{
        player.play();
    }
}

void MainWindow::on_seekbarSlider_sliderReleased()
{
    int position = (float)ui->seekbarSlider->value() / ui->seekbarSlider->maximum() * player.duration();
    player.setPosition(position);
}

void MainWindow::on_seekbarSlider_sliderPressed()
{
    int position = (float)ui->seekbarSlider->value() / ui->seekbarSlider->maximum() * player.duration();
    player.setPosition(position);
}

void MainWindow::on_seekBackButton_clicked()
{
    int position = player.position() - 1000;
    player.setPosition(position);
}


void MainWindow::on_seekForwardButton_clicked()
{
    int position = player.position() + 1000;
    player.setPosition(position);
}


void MainWindow::on_seekTopButton_clicked()
{
    player.setPosition(0);
}


void MainWindow::on_freeze1Button_clicked()
{
    mmmfcc.getMfccGraph().freeze1Graph();
}

void MainWindow::on_freeze2Button_clicked()
{
    mmmfcc.getMfccGraph().freeze2Graph();
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
    Graph &graph = mmmfcc.getMfccGraph();
    graph.isHideCurrentGraph = (graph.isHideCurrentGraph == false);
}


