#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QThread>

#include "compute-mfcc/mfcc.h"
#include "audiosourcedevice.h"
#include "audiosourcefile.h"

class Translator : public QObject
{
    Q_OBJECT
public:
    explicit Translator(AudioSourceFile &asf, AudioSourceDevice &asd, QObject *parent = nullptr);
    ~Translator();

    enum class Source{
        File,
        Device
    };
    Q_ENUM(Source)

    enum class Algorithm{
        MFCC,
        FFT,
    };
    Q_ENUM(Algorithm)

    void setSource(Source source);
    void setAlgorithm(Algorithm algorithm);

private:
    MFCC *mfccTranslator;
    AudioSourceFile &audioSourceFile;
    AudioSourceDevice &audioSourceDevice;
    Source currentSource;
    Algorithm currentAlgorithm;

    QVector<double> translate(QVector<qint16> &samples, quint64 from);

public slots:
    void doTranslate();

signals:
    void updated(QVector<double> mfcc);

};

#endif // TRANSLATOR_H
