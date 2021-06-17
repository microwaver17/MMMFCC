#ifndef MMMFCC_H
#define MMMFCC_H

#include <QAudioDecoder>
#include <QObject>

#include "audiosourcefile.h"

class MmMfcc : public QObject
{
    Q_OBJECT
public:
    explicit MmMfcc(QObject *parent = nullptr);
    ~MmMfcc();

    void setAudioFilePath(QString path);

private:
    AudioSourceFile audioSourceFile;

private slots:
    void test();

};

#endif // MMMFCC_H
