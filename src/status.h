#ifndef STATUS_H
#define STATUS_H

#include <QObject>
#include <QMap>

class Status : public QObject
{
    Q_OBJECT
public:
    enum class Subject{
        ShowRealtimeGraph,
        AudioSource,
        GraphType,
        Smoothing,
        Algorithm,
        Decoder,
        PlayerReady,
        Freeze1,
        Freeze2,
    };
    Q_ENUM(Subject)

    enum class State{
        Primary,
        Secondary,
        Undefined,
    };
    Q_ENUM(State)

    static Status &getInstance();

    void setState(Subject subject, State state);
    void setState(Subject subject, bool state);
    State getState(Subject subject);

private:
    explicit Status(QObject *parent = nullptr);

    QMap<Subject, State> status;

signals:
    void statusUpdated();

};

#endif // STATUS_H
