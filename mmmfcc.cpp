#include "mmmfcc.h"
#include "translator.h"
#include "consts.h"

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
    connect(&audioSourceFile.getPlayer(), &QMediaPlayer::positionChanged, this, &MmMfcc::updatePosition);

    translatorThread.start();
    translatorTimer.start(1000.0 / Consts::fps);
}

MmMfcc::~MmMfcc()
{
    translatorThread.quit();
    translatorThread.wait();
}

void MmMfcc::setAudioFilePath(QString path)
{
    audioFilePath = path;
    audioSourceFile.setSource(path);
    translator.setSource(Translator::Source::File);
}

QMediaPlayer &MmMfcc::getPlayer()
{
    return audioSourceFile.getPlayer();
}

Graph &MmMfcc::getMfccGraph()
{
    return mfccGraph;
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
    if (data.size() == 0){
        return;
    }
    mfccGraph.paintCurrent(data);
}

void MmMfcc::test()
{

}
