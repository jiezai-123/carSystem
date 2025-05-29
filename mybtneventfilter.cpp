#include "mybtneventfilter.h"
#include <QDebug>
Mybtneventfilter::Mybtneventfilter(QObject *parent) : QObject(parent)
{

}

bool Mybtneventfilter::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress){
        emit goPage(watched);
    }
    return QObject::eventFilter(watched,event);
}
