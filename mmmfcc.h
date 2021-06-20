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
    void startTranslatorThread();

    QMediaPlayer &getPlayer();
    Graph &getMfccGraph();
    Translator &getTranslator();

private:
    AudioSourceFile audioSourceFile;
    AudioSourceDevice audioSourceDevice;
    QString audioFilePath;
    Translator translator;
    QThread translatorThread;
    QTimer translatorTimer;
    bool isTranslating;
    Graph mfccGraph;

signals:
    void updatePosition();
    void timeoutTranslator();

private slots:
    void dispatchTransrator();
    void paintGraph(QVector<double> data);
    void test();

};

#endif // MMMFCC_H
