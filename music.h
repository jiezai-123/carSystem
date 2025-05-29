#ifndef MUSIC_H
#define MUSIC_H

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
#include "music_class.h"
#include "music_classdao.h"
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>
#include <QTextCodec>
#include <QColor>
struct Lyric {
    int time; // 时间，单位为毫秒
    QString text; // 歌词内容
};
namespace Ui {
class Music;
}

class Music : public QWidget
{
    Q_OBJECT

public:
    explicit Music(QWidget *parent = nullptr);
    ~Music();
    void loadLocalMusicFromDatabase();
    QList<Lyric> parseLyricFile(const QString& filePath);
    void updateLyrics(const QList<Lyric> &lyrics, qint64 position);
    QList<Lyric> parseLyricData(const QByteArray &data);
private:
    void setActivePlayer(int playerType);
signals:
    void backIndex();
    void showonline();
private slots:
    void on_backBtn_clicked();

    void on_on_stopBtn_clicked();

    void on_openMusic_clicked();

    void on_preBtn_clicked();

    void on_nextBtn_clicked();

    void on_voiceBtn_clicked();

    void on_localList_itemDoubleClicked(QListWidgetItem *item);

    void handleDurationChanged(qint64 duration);

    void handlePositionChanged(qint64 position);

    void on_musicslider_sliderReleased();

    void on_voiceslider_valueChanged(int value);

    void reloadonline();

    void on_tabWidget_tabBarClicked(int index);

   void showline();

   void on_onlineList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::Music *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QMediaPlayer *onlineplayer;
    QMediaPlaylist *onlineplaylist;
    int currentPlayer;
    int total;
    QVideoWidget *videoWidget;
    QNetworkAccessManager *manager=nullptr;
    QNetworkReply *onlineReply=nullptr;
    QNetworkReply *lrcReply=nullptr;
};

#endif // MUSIC_H
