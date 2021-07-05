#include "status.h"

Status::Status(QObject *parent) : QObject(parent)
{

}

Status &Status::getInstance()
{
    static Status instance;
    return instance;
}

void Status::setState(Subject subject, State state)
{
    status[subject] = state;
    emit statusUpdated();
}

void Status::setState(Subject subject, bool state)
{
    if (state){
        setState(subject, State::Primary);
    }else{
        setState(subject, State::Secondary);
    }
}

Status::State Status::getState(Subject subject)
{
    if (!status.contains(subject)){
        return State::Undefined;
    }
    return status[subject];
}
