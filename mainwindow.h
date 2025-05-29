#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <weather.h>
#include <music.h>
#include <video.h>
#include <location.h>
#include <QTimer>
#include <QString>
#include <QPixmap>
#include <QPushButton>
#include "mybtneventfilter.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setBtnIcon(const QString &img, QPushButton* btn);
public slots:
    void showDate();
    void goPage(QObject *watched);
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    Mybtneventfilter *filter;
    Weather *weather;
    Music *music;
    Video * video;
    Location *location;
};
#endif // MAINWINDOW_H
