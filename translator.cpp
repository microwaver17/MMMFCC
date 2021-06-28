#include "translator.h"
#include "settings.h"
#include "log.h"
#include "util.h"
#include "status.h"

#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QVariant>
#include <QtDebug>

Translator::Translator(AudioSourceFile &asf, AudioSourceDevice &asd, QObject *parent) : QObject(parent)
  , mfccTranslator(new MFCC(SETTINGS.sampleRate, SETTINGS.cepstramNumber, SETTINGS.windowLength, 10, SETTINGS.filterNumber, SETTINGS.minFreq, SETTINGS.maxFreq))
  , audioSourceFile(asf)
  , audioSourceDevice(asd)
  , currentSource(Source::Device)
  , currentAlgorithm(Algorithm::MFCC)
{
    Status::getInstance().setState(Status::Subject::Algorithm, Status::State::Primary);
}

Translator::~Translator()
{
    delete mfccTranslator;
}

QVector<double> Translator::translate(QVector<qint16> &samples, quint64 fromSamples = 0)
{
    std::vector<qint16> samples_vec = samples.toStdVector();

    if (samples.size() == 0){
        return QVector<double>();
    }
    mfccTranslator->processFrame(samples_vec, fromSamples);

    std::vector<double> data;
    if (currentAlgorithm == Algorithm::MFCC){
        data = mfccTranslator->getMfcc();
        data.assign(data.begin(), data.begin() + SETTINGS.filterNumber);

    }else if (currentAlgorithm == Algorithm::FFT){
        data = mfccTranslator->getpowerSpectralCoef();

        // FFTの最大周波数をMFCCにかける最大周波数と合わせる
        int maxcoef = (double)SETTINGS.maxFreq / (SETTINGS.sampleRate / 2) * mfccTranslator->getNumFFTBins();
        data.assign(data.begin(), data.begin() + maxcoef);
        for (auto coef = data.begin(); coef != data.end(); coef++){
            *coef = *coef / ((SETTINGS.windowLength / 1000.0) * SETTINGS.sampleRate) / pow(2, SETTINGS.sampleSize);
            *coef = sqrt(*coef);
        }
    }
    QVector<double> data_qvec = QVector<double>::fromStdVector(data);
    return data_qvec;
}

void Translator::doTranslate()
{
        QVector<qint16> rawSamples;
        if (currentSource == Source::Device){
            rawSamples = audioSourceDevice.getRawSamples(0);
        }else if (currentSource == Source::File){
            if (!(audioSourceFile.getPlayer().state() == QMediaPlayer::PlayingState)){
                emit updated(QVector<double>());
                return;
            }
            int from = audioSourceFile.getPlayer().position() / 1000.0 * SETTINGS.sampleRate;
            rawSamples = audioSourceFile.getRawSamples(from);
        }
        QVector<double> data = translate(rawSamples, 0);
        emit updated(data);
}

void Translator::setSource(Source source)
{
    QString source_str = Util::toStrEnum<Source>(source);
    LOG.addLog(u8"音声ソースを変更 " + source_str, this);
    currentSource = source;

    if (source == Source::Device){
        Status::getInstance().setState(Status::Subject::AudioSource, Status::State::Primary);
    }else if (source == Source::File){
        Status::getInstance().setState(Status::Subject::AudioSource, Status::State::Secondary);
    }
}

void Translator::setAlgorithm(Algorithm algorithm)
{
    QString algorithm_str = Util::toStrEnum<Algorithm>(algorithm);
    LOG.addLog(u8"アルゴリズムを変更 " + algorithm_str, this);
    currentAlgorithm = algorithm;

    if (algorithm == Algorithm::MFCC){
        Status::getInstance().setState(Status::Subject::Algorithm, Status::State::Primary);
    }else if (algorithm == Algorithm::FFT){
        Status::getInstance().setState(Status::Subject::Algorithm, Status::State::Secondary);
    }
}
