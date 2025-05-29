#ifndef MYBTNEVENTFILTER_H
#define MYBTNEVENTFILTER_H

#include <QObject>
#include <QEvent>
class Mybtneventfilter : public QObject
{
    Q_OBJECT
public:
    explicit Mybtneventfilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void goPage(QObject *watched);
};

#endif // MYBTNEVENTFILTER_H
