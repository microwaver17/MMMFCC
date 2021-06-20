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
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;

    MmMfcc mmmfcc;
    QMediaPlayer &player;

private slots:
    void updateSeekbar();
    void on_openAudioFile_clicked();
    void on_playPauseButton_clicked();
    void on_seekbarSlider_sliderReleased();
    void on_seekBackButton_clicked();
    void on_seekForwardButton_clicked();
    void on_freeze1Button_clicked();
    void on_freeze2Button_clicked();
    void on_selectSourceMicButton_clicked();
    void on_selectSourceFileButton_clicked();
    void on_toggleGraphButton_clicked();
    void on_clearFreezeButton_clicked();
    void on_seekbarSlider_sliderPressed();
};
#endif // MAINWINDOW_H
