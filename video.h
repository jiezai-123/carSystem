#ifndef VIDEO_H
#define VIDEO_H
#include "video_class.h"
#include "vedio_classdao.h"
#include <QWidget>
#include <QFile>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QListWidgetItem>
#include<QStyle>
#include<QTime>
#include <QFileDialog>
#include <QVideoWidget>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "screeneventfilter.h"
namespace Ui {
class Video;
}

class Video : public QWidget
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = nullptr);
    ~Video();
    void loadLocalVideosFromDatabase();
private:
    void setActivePlayer(int playerType);
signals:
    void backIndex();
    void showonline();
private slots:
    void on_backBtn_clicked();

    void handleDurationChanged(qint64 duration);

    void handlePositionChanged(qint64 position);

    void on_on_stopBtn_clicked();

    void on_openVideo_clicked();

    void on_preBtn_clicked();

    void on_nextBtn_clicked();

    void on_voiceBtn_clicked();

    void on_voiceslider_valueChanged(int value);

    void on_bigBtn_clicked();

    void on_localList_itemDoubleClicked(QListWidgetItem *item);

    void showSmallScreen();

    void on_onlineList_itemDoubleClicked(QListWidgetItem *item);

     void showline(QNetworkReply *reply);

     void on_tabWidget_tabBarClicked(int index);

     void reloadonline();

     void on_videoslider_sliderReleased();

private:
    Ui::Video *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QMediaPlayer *onlineplayer;
    QMediaPlaylist *onlineplaylist;
    int currentPlayer;
    int total;
    QVideoWidget *videoWidget;
    ScreeneventFilter *filter;
    QNetworkAccessManager *manager;


};

#endif // VIDEO_H
