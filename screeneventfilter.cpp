#include "screeneventfilter.h"

ScreeneventFilter::ScreeneventFilter(QObject *parent) : QObject(parent)
{

}

bool ScreeneventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonDblClick){
        emit smallScreen();
    }
    QKeyEvent *keyEvent=static_cast<QKeyEvent*>(event);
   if(keyEvent->key()==Qt::Key_Escape){
       emit smallScreen();
   }
   return QObject::eventFilter(watched,event);

}
