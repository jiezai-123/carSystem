#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "common.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
namespace Ui {
class Weather;
}

class Weather : public QWidget
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    void init();
    void initweather();
    void initVector();
    ~Weather();
signals:
    void backIndex();
private slots:
    void on_backBtn_clicked();
    void showDate();
    void showWeather(QNetworkReply *reply);


private:
    Ui::Weather *ui;
    QTimer *timer;
    QNetworkAccessManager *manager;
    QVector<QLabel*> *datetimes;
    QVector<QLabel*> *temperatures;
    QVector<QLabel*> *icons;
    QVector<QLabel*> *days;
};

#endif // WEATHER_H
