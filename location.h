#ifndef LOCATION_H
#define LOCATION_H

#include <QWidget>
#include "common.h"
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPoint>
#include <QMouseEvent>
namespace Ui {
class Location;
}

class Location : public QWidget
{
    Q_OBJECT

public:
    explicit Location(QWidget *parent = nullptr);
    ~Location();
    void initmap();
    void getip();
    void initLngLat();
    bool eventFilter(QObject *obj, QEvent *event)override;
signals:
    void backIndex();
private slots:
    void on_backBtn_clicked();
    void showmap();
    void showip();
    void showLngLat();

    void on_zoom_big_clicked();

    void on_zoom_small_clicked();

private:
    Ui::Location *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *mapreply;
    QNetworkReply *ipreply;
    QNetworkReply *LngLatreply;
    QString lng="104.064328";
    QString lat="30.573457";
    int zoom=10;
    QString ip;
    bool isDragging;
    QPoint lastMousePos;

};

#endif // LOCATION_H
