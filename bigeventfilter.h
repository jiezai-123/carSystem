#ifndef BIGEVENTFILTER_H
#define BIGEVENTFILTER_H

#include <QObject>
#include <QEvent>

class BigeventFilter : public QObject
{
    Q_OBJECT
public:
    explicit BigeventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
signals:
     void bigScreen();

};

#endif // BIGEVENTFILTER_H
