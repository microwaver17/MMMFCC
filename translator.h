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

private:
    MFCC *mfccTranslator;
    AudioSourceFile &audioSourceFile;
    AudioSourceDevice &audioSourceDevice;
    Source currentSource;

    QVector<double> translate(QVector<qint16> &samples, quint64 from);

public slots:
    void doTranslate();
    void setSource(Source source);

signals:
    void updated(QVector<double> mfcc);

};

#endif // TRANSLATOR_H
