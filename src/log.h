#ifndef LOG_H
#define LOG_H

#define LOG (Log::getInstance())

#include <QObject>
#include <QVector>

class Log : public QObject
{
    Q_OBJECT
public:
    static Log &getInstance();
    void addLog(QString msg, QObject *sender);
    QVector<QMap<QString, QString>> &getLogs();

private:
    explicit Log(QObject *parent = nullptr);

    QVector<QMap<QString, QString>> logList;

signals:
    void logAdded();

};

#endif // LOG_H
