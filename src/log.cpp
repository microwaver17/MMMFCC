#include "log.h"
#include <QMap>

Log::Log(QObject *parent) : QObject(parent)
{

}

Log &Log::getInstance()
{
    static Log instance;

    return instance;
}

void Log::addLog(QString msg, QObject *sender)
{
    QString cls = "(none)";
    if (sender != nullptr){
        cls = sender->metaObject()->className();
    }

    QMap<QString, QString> log;
    log.insert("class", cls);
    log.insert("message", msg);
    logList.append(log);

    emit logAdded();
}

QVector<QMap<QString, QString>> &Log::getLogs()
{
    return logList;
}

