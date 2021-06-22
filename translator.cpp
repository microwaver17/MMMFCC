#include "translator.h"
#include "consts.h"

#include <QVector>
#include <QDateTime>
#include <QThread>

Translator::Translator(AudioSourceFile &asf, AudioSourceDevice &asd, QObject *parent) : QObject(parent)
  , mfccTranslator(new MFCC(Consts::sampleRate, 12 * 3, Consts::windowLength, 10, 40, 50, 6500))
  , audioSourceFile(asf)
  , audioSourceDevice(asd)
  , currentSource(Source::Device)
{
}

Translator::~Translator()
{
    delete mfccTranslator;
}

QVector<double> Translator::translate(QVector<qint16> &samples, quint64 fromSamples = 0)
{
    std::vector<qint16> samples_vec = samples.toStdVector();

    std::vector<double> mfcc = mfccTranslator->processFrameWithoutShift(samples_vec, fromSamples);

    QVector<double> mfcc_qvec = QVector<double>::fromStdVector(mfcc);
    return mfcc_qvec;
}

void Translator::doTranslate()
{
        QVector<qint16> rawSamples;
        if (currentSource == Source::Device){
            rawSamples = audioSourceDevice.getRawSamples();
        }else if (currentSource == Source::File){
            size_t from = audioSourceFile.getPlayer().position() / 1000.0 * Consts::sampleRate;
            rawSamples = audioSourceFile.getRawSamples(from);
        }
        // qDebug() << mfcc;
        QVector<double> mfcc = translate(rawSamples, 0);
        emit updated(mfcc);
}

void Translator::setSource(Source source)
{
    currentSource = source;
}
