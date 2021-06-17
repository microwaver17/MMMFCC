#include "mmmfcc.h"

#include <QFile>
#include <QtDebug>

MmMfcc::MmMfcc(QObject *parent) : QObject(parent)
{

}

MmMfcc::~MmMfcc()
{
}

void MmMfcc::setAudioFilePath(QString path)
{
    audioSourceFile.startDecode(path);
}

void MmMfcc::test()
{

}
