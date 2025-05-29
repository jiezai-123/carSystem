#include "location.h"
#include "ui_location.h"
#include <QString>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>
Location::Location(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Location),
    isDragging(false),
    lastMousePos(0,0)
{
    ui->setupUi(this);
     this->manager=new QNetworkAccessManager(this);
     ui->map_area->installEventFilter(this);
}

Location::~Location()
{
    delete ui;
}

void Location::initmap()
{
    QString url="https://api.map.baidu.com/staticimage/v2?ak=%1&width=400&height=300&zoom=%2&scaler=2&center=%3,%4";
    url=url.arg(API_AKY).arg(this->zoom).arg(this->lng).arg(this->lat);
    this->mapreply=manager->get(QNetworkRequest(QUrl(url)));
    connect(this->mapreply,&QNetworkReply::finished,this,&Location::showmap);

}

void Location::getip()
{
    QString url="https://icanhazip.com/";
    this->ipreply=manager->get(QNetworkRequest(QUrl(url)));
    connect(this->ipreply,&QNetworkReply::finished,this,&Location::showip);

}

void Location::initLngLat()
{
    QString url="https://api.map.baidu.com/location/ip?ip=%1&coor=bd09ll&ak=%2";
    url=url.arg(this->ip).arg(API_AKY);
    this->LngLatreply=manager->get(QNetworkRequest(QUrl(url)));
    connect(this->LngLatreply,&QNetworkReply::finished,this,&Location::showLngLat);

}

bool Location::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->map_area) {
           if (event->type() == QEvent::MouseButtonPress) {
               QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
               if (mouseEvent->button() == Qt::LeftButton) {
                   isDragging = true;
                   lastMousePos = mouseEvent->pos();
               }
           } else if (event->type() == QEvent::MouseMove) {
               QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
               if (isDragging) {
                   QPoint delta = mouseEvent->pos() - lastMousePos;
                   // 根据偏移量更新经纬度
                   double deltaLng = delta.x() * 0.01; // 这里的系数需要根据实际情况调整
                   double deltaLat = delta.y() * 0.01; // 这里的系数需要根据实际情况调整
                   lng = QString::number(lng.toDouble() + deltaLng);
                   lat = QString::number(lat.toDouble() - deltaLat);
                   lastMousePos = mouseEvent->pos();
                   // 重新渲染地图
                   this->initmap();
               }
           } else if (event->type() == QEvent::MouseButtonRelease) {
               QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
               if (mouseEvent->button() == Qt::LeftButton) {
                   isDragging = false;
               }
           }
       }
       return QWidget::eventFilter(obj, event);

}

void Location::on_backBtn_clicked()
{
    emit backIndex();
}

void Location::showmap()
{
  QByteArray arr=this->mapreply->readAll();
  QFile file("./location_map.png");
  if(file.open(QIODevice::ReadWrite)){
     file.write(arr);
     file.close();
      QPixmap pixmap("./location_map.png");
      ui->map_area->setPixmap(pixmap);
  }

}

void Location::showip()
{
    QByteArray arr=this->ipreply->readAll();
    this->ip=QString(arr).replace("\n","");
    this->initLngLat();

}

void Location::showLngLat()
{
    QByteArray arr=this->LngLatreply->readAll();
    QJsonDocument Document=QJsonDocument::fromJson(arr);
    if(Document.isObject()){
        QJsonObject object=Document.object();
        QJsonObject content=object.value("content").toObject();
        QJsonObject point=content.value("point").toObject();
        lng=point.value("x").toString();
        lat=point.value("y").toString();
          this->initmap();
    }


}

void Location::on_zoom_big_clicked()
{
    if(this->zoom>=19){
        QMessageBox::critical(this,"提示","已经放大到最大!");
        return;
    }
    this->zoom++;
    this->initmap();
}

void Location::on_zoom_small_clicked()
{   if(this->zoom<=3){
        QMessageBox::critical(this,"提示","已经缩小到最小!");
        return;
    }
    this->zoom--;
    this->initmap();
}
