#include "mmmfcc.h"
#include "translator.h"
#include "settings.h"
#include "log.h"

#include <QFile>
#include <QtDebug>
#include <QDateTime>

MmMfcc::MmMfcc(QObject *parent) : QObject(parent)
  , audioSourceFile(this)
  , audioSourceDevice(this)
  , translator(audioSourceFile, audioSourceDevice)
  , isTranslating(false)
{
    qRegisterMetaType<QVector<double>>();

    connect(&translatorTimer, &QTimer::timeout, this, &MmMfcc::timeoutTranslator);      // 1/fps 秒ごとにMFCC計算実行を判断
    connect(this, &MmMfcc::dispatchTransrator, &translator, &Translator::doTranslate);  // MFCC計算実行
    connect(&translator, &Translator::updated, this, &MmMfcc::paintGraph);              // 計算終了->グラフ描画

    // 計算は別スレッド
    translator.moveToThread(&translatorThread);
    translatorThread.start();
    translatorTimer.start(1000.0 / SETTINGS_INT(SettingKeys::fps));
    LOG.addLog(u8"スレッド開始", this);
}

MmMfcc::~MmMfcc()
{
    translatorThread.quit();
    translatorThread.wait();
}

void MmMfcc::setAudioFilePath(QString path)
{
    audioSourceFile.setSource(path);
    translator.setSource(Translator::Source::File);
}

void MmMfcc::setAudioDevice(QAudioDeviceInfo info)
{
    audioSourceDevice.setSource(info);
    translator.setSource(Translator::Source::Device);
}

QMediaPlayer &MmMfcc::getPlayer()
{
    return audioSourceFile.getPlayer();
}

Graph &MmMfcc::getGraph()
{
    return graph;
}

Translator &MmMfcc::getTranslator()
{
    return translator;
}

void MmMfcc::timeoutTranslator()
{
    if (isTranslating){
        return; //計算が終わってなければフレームスキップ
    }
    emit dispatchTransrator();
    isTranslating = true;
}

void MmMfcc::paintGraph(QVector<double> data)
{
    isTranslating = false;
    graph.plotData(data);
}
