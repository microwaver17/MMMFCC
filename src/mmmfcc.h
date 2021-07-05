#ifndef MMMFCC_H
#define MMMFCC_H

#include <QAudioDecoder>
#include <QMediaPlayer>
#include <QObject>
#include <QThread>
#include <QTimer>

#include "audiosourcefile.h"
#include "translator.h"
#include "graph.h"
#include "audiosourcedevice.h"

Q_DECLARE_METATYPE(QVector<double>)

class MmMfcc : public QObject
{
    Q_OBJECT
public:
    explicit MmMfcc(QObject *parent = nullptr);
    ~MmMfcc();

    void setAudioFilePath(QString path);
    void setAudioDevice(QAudioDeviceInfo info);
    void startTranslatorThread();

    QMediaPlayer &getPlayer();
    Graph &getGraph();
    Translator &getTranslator();

private:
    AudioSourceFile audioSourceFile;
    AudioSourceDevice audioSourceDevice;
    Translator translator;
    QThread translatorThread;
    QTimer translatorTimer;
    bool isTranslating;
    Graph graph;

signals:
    void dispatchTransrator();

private slots:
    void timeoutTranslator();
    void paintGraph(QVector<double> data);

};

#endif // MMMFCC_H
