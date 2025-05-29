#ifndef SCREENEVENTFILTER_H
#define SCREENEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
class ScreeneventFilter : public QObject
{
    Q_OBJECT
public:
    explicit ScreeneventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void smallScreen();
};

#endif // SCREENEVENTFILTER_H
