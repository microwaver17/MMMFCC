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

    translator.moveToThread(&translatorThread);
    connect(&translatorTimer, &QTimer::timeout, this, &MmMfcc::dispatchTransrator);
    connect(this, &MmMfcc::timeoutTranslator, &translator, &Translator::doTranslate);
    connect(&translator, &Translator::updated, this, &MmMfcc::paintGraph);

    translatorThread.start();
    translatorTimer.start(1000.0 / SETTINGS.fps);
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

void MmMfcc::dispatchTransrator()
{
    if (isTranslating){
        return; //フレームスキップ
    }
    emit timeoutTranslator();
    isTranslating = true;
}

void MmMfcc::paintGraph(QVector<double> data)
{
    isTranslating = false;
    graph.plotData(data);
}
