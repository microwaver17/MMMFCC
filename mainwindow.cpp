#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "settings.h"
#include "status.h"
#include "consts.h"
#include "settingdialog.h"

#include <QAudioDeviceInfo>
#include <QFileDialog>
#include <QtDebug>
#include <QScrollBar>

void MainWindow::initMmmfcc(){
    mmmfcc = new MmMfcc(this);
    connect(&mmmfcc->getPlayer(), &QMediaPlayer::positionChanged, this, &MainWindow::updateSeekbar);
    connect(&mmmfcc->getGraph(), &Graph::updated, this, &MainWindow::updateScene);

    ui->graphScalelSlider->setValue(SETTINGS_DOUBLE(SettingKeys::default_scale) * ui->graphScalelSlider->maximum());
    ui->autoScalecheckBox->setChecked(SETTINGS_BOOL(SettingKeys::isAutoScale));

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMmmfcc();

    connect(&LOG, &Log::logAdded, this, &MainWindow::updateLog);
    connect(&Status::getInstance(), &Status::statusUpdated, this, &MainWindow::updateStatus);

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);

    int key = event->key();
    Qt::KeyboardModifiers modif = event->modifiers();

    if (key == Qt::Key::Key_0){
        on_playPauseButton_clicked();
    }else if(key == Qt::Key_Home){
        on_seekTopButton_clicked();
    }else if(key == Qt::Key_Minus){
        on_seekBackButton_clicked();
    }else if(key == Qt::Key_Plus){
        on_seekForwardButton_clicked();
    }
}

void MainWindow::updateScene()
{
    int width = ui->graphGraphicsView->width();
    int height = ui->graphGraphicsView->height();
    ui->graphGraphicsView->setScene(mmmfcc->getGraph().getScene());
    ui->graphGraphicsView->setSceneRect(3, 3, width-3, height-3);
    mmmfcc->getGraph().setSceneSize(width, height);
}

inline void setHighlightButtonOne(QPushButton *button, bool state){
    if (button == nullptr){
        return;
    }

    QString color = QColor(193, 206, 232).name();

    if (state){
        button->setStyleSheet(QString("QWidget {background-color: %1; font-weight: bold;}").arg(color));
        button->setDown(true);
    }else{
        button->setStyleSheet("");
        button->setDown(false);
    }
}

inline void setHighlightButton(QPushButton *buttonPrimary, QPushButton *buttonSecondary, Status::Subject subject){
    Status::State state = Status::getInstance().getState(subject);
    if (state == Status::State::Primary){
        setHighlightButtonOne(buttonPrimary, true);
        setHighlightButtonOne(buttonSecondary, false);
    }else if (state == Status::State::Secondary){
        setHighlightButtonOne(buttonPrimary, false);
        setHighlightButtonOne(buttonSecondary, true);
    }else{
        setHighlightButtonOne(buttonPrimary, false);
        setHighlightButtonOne(buttonSecondary, false);
    }
}

void MainWindow::updateStatus(){
    setHighlightButton(ui->selectSourceMicButton, ui->selectSourceFileButton, Status::Subject::AudioSource);
    setHighlightButton(ui->toggleGraphButton, nullptr, Status::Subject::ShowRealtimeGraph);
    setHighlightButton(ui->algorithmMfccButton, ui->algorithmFftButton, Status::Subject::Algorithm);
    setHighlightButton(ui->graphTypeLineButton, ui->graphTypeBarButton, Status::Subject::GraphType);
    setHighlightButton(ui->preProcessMovingAverageButton, ui->preProcessNoneButton, Status::Subject::Smoothing);
    setHighlightButton(ui->playPauseButton, nullptr, Status::Subject::Decoder);
    setHighlightButton(ui->freeze1Button, nullptr, Status::Subject::Freeze1);
    setHighlightButton(ui->freeze2Button, nullptr, Status::Subject::Freeze2);

    if (Status::getInstance().getState(Status::Subject::PlayerReady) == Status::State::Primary){
        ui->seekbarSlider->setEnabled(true);
    }else{
        ui->seekbarSlider->setEnabled(false);
    }
}

void MainWindow::updateSeekbar(){
    QMediaPlayer &player = mmmfcc->getPlayer();
    int position = (float)player.position() / player.duration() * ui->seekbarSlider->maximum();
    ui->seekbarSlider->setValue(position);
}

void MainWindow::updateLog()
{
    auto logs = LOG.getLogs();
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

    mmmfcc->setAudioFilePath(filepath);
}

void MainWindow::on_playPauseButton_clicked()
{
    QMediaPlayer &player = mmmfcc->getPlayer();
    if (player.state() == QMediaPlayer::PlayingState){
        player.pause();
    }else{
        player.play();
        mmmfcc->getTranslator().setSource(Translator::Source::File);
        mmmfcc->getGraph().setIsHideCurrentGraph(false);
    }
}

void MainWindow::on_seekbarSlider_valueChanged(int value)
{
    QMediaPlayer &player = mmmfcc->getPlayer();
    int position = (float)ui->seekbarSlider->value() / ui->seekbarSlider->maximum() * player.duration();
    if (abs(position - player.position()) <= Consts::playerTimeUnit){
        return;
    }
    player.setPosition(position);
}

void MainWindow::on_seekBackButton_clicked()
{
    QMediaPlayer &player = mmmfcc->getPlayer();
    int position = player.position() - 1000;
    player.pause();
    player.setPosition(position);
}


void MainWindow::on_seekForwardButton_clicked()
{
    QMediaPlayer &player = mmmfcc->getPlayer();
    int position = player.position() + 1000;
    player.pause();
    player.setPosition(position);
}


void MainWindow::on_seekTopButton_clicked()
{
    mmmfcc->getPlayer().setPosition(0);
}

void MainWindow::on_freeze1Button_clicked()
{
    mmmfcc->getGraph().freeze1Graph();
}

void MainWindow::on_freeze2Button_clicked()
{
    mmmfcc->getGraph().freeze2Graph();
}

void MainWindow::on_selectSourceMicButton_clicked()
{
    mmmfcc->getTranslator().setSource(Translator::Source::Device);
    mmmfcc->getGraph().setIsHideCurrentGraph(false);
}

void MainWindow::on_selectSourceFileButton_clicked()
{
    mmmfcc->getTranslator().setSource(Translator::Source::File);
    mmmfcc->getGraph().setIsHideCurrentGraph(false);
}

void MainWindow::on_toggleGraphButton_clicked()
{
    Graph &graph = mmmfcc->getGraph();
    mmmfcc->getGraph().setIsHideCurrentGraph(graph.getIsHideCurrentGraph() == false);
}

void MainWindow::on_inputDeviceComboBox_currentIndexChanged(int index)
{
    QAudioDeviceInfo info = devicesInfo.at(index);
    mmmfcc->setAudioDevice(info);
    mmmfcc->getGraph().setIsHideCurrentGraph(false);
}

void MainWindow::on_graphScalelSlider_valueChanged(int value)
{
    double scale = value / 1000.0;
    mmmfcc->getGraph().setScale(scale);
}

void MainWindow::on_autoScalecheckBox_stateChanged(int state)
{
    mmmfcc->getGraph().setIsAutoScele(state == Qt::CheckState::Checked);
    SETTINGS_SET("isAutoScale", state == Qt::CheckState::Checked);
}

void MainWindow::on_graphTypeLineButton_clicked()
{
    mmmfcc->getGraph().setGraphType(Graph::GraphType::Line);
}


void MainWindow::on_graphTypeBarButton_clicked()
{
    mmmfcc->getGraph().setGraphType(Graph::GraphType::Bar);
}

void MainWindow::on_preProcessNoneButton_clicked()
{
    mmmfcc->getGraph().setIsMovingAvarage(false);
}

void MainWindow::on_preProcessMovingAverageButton_clicked()
{
    mmmfcc->getGraph().setIsMovingAvarage(true);
}


void MainWindow::on_algorithmMfccButton_clicked()
{
    mmmfcc->getTranslator().setAlgorithm(Translator::Algorithm::MFCC);
    mmmfcc->getGraph().setIsZeroLineAtMiddle(true);
}


void MainWindow::on_algorithmFftButton_clicked()
{
    mmmfcc->getTranslator().setAlgorithm(Translator::Algorithm::FFT);
    mmmfcc->getGraph().setIsZeroLineAtMiddle(false);
}

void MainWindow::on_settingButton_clicked()
{
    SettingDialog sd(this);
    if (sd.exec() == QDialog::Rejected){
        return;
    }

    delete mmmfcc;

    initMmmfcc();

    on_inputDeviceComboBox_currentIndexChanged(0);

    Settings::getInstance().save();
}

