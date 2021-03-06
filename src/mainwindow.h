#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mmmfcc.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    MmMfcc *mmmfcc;
    QList<QAudioDeviceInfo> devicesInfo;

    void initMmmfcc();

public slots:
    void updateScene();
    void updateStatus();

private slots:
    void updateSeekbar();
    void updateLog();
    void on_openAudioFile_clicked();
    void on_playPauseButton_clicked();
    void on_seekBackButton_clicked();
    void on_seekForwardButton_clicked();
    void on_freeze1Button_clicked();
    void on_freeze2Button_clicked();
    void on_selectSourceMicButton_clicked();
    void on_selectSourceFileButton_clicked();
    void on_toggleGraphButton_clicked();
    void on_seekTopButton_clicked();
    void on_inputDeviceComboBox_currentIndexChanged(int index);
    void on_graphScalelSlider_valueChanged(int value);
    void on_autoScalecheckBox_stateChanged(int state);
    void on_graphTypeLineButton_clicked();
    void on_graphTypeBarButton_clicked();
    void on_preProcessNoneButton_clicked();
    void on_preProcessMovingAverageButton_clicked();
    void on_algorithmMfccButton_clicked();
    void on_algorithmFftButton_clicked();
    void on_seekbarSlider_valueChanged(int value);
    void on_settingButton_clicked();
};
#endif // MAINWINDOW_H
