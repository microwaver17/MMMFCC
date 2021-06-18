#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mmmfcc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MmMfcc mmmfcc;

private slots:
    void updateSeekbar();
    void on_openAudioFile_clicked();

    void on_playPauseButton_clicked();

    void on_seekbarSlider_sliderReleased();

    void on_seekBackButton_clicked();

    void on_seekForwardButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
