#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QVariant>

class Util : public QObject
{
    Q_OBJECT
public:
    static inline QString toStr(bool x){
        if(x){
            return u8"有効";
        }else{
            return u8"無効";
        }
    };

    template<typename QEnum>
    static inline QString toStrEnum(QEnum value){
        return QVariant::fromValue(value).toString();
    }

private:
    explicit Util(QObject *parent = nullptr) : QObject(parent){};

signals:

};

#endif // UTIL_H
