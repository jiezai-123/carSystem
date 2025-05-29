#include "weather.h"
#include "ui_weather.h"

Weather::Weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Weather)
{
    ui->setupUi(this);
     this->init();
    this->timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&Weather::showDate);
    timer->start(1000);
}
void Weather::init()
{ this->initVector();
  this->initweather();
}

void Weather::initweather()
{   this->manager=new QNetworkAccessManager;
    QString url="http://gfeljm.tianqiapi.com/api?unescape=1&version=v91&appid=%1&appsecret=%2&ext=&cityid=";
    QString newurl=url.arg(APPID).arg(APPSECRET);
    manager->get(QNetworkRequest(QUrl(newurl)));
    connect(manager,&QNetworkAccessManager::finished,this,&Weather::showWeather);
}

void Weather::initVector()
{
    this->datetimes=new QVector<QLabel*>;
    datetimes->push_back(ui->timeLab1);
    datetimes->push_back(ui->timeLab2);
    datetimes->push_back(ui->timeLab3);
    datetimes->push_back(ui->timeLab4);
    datetimes->push_back(ui->timeLab5);
    datetimes->push_back(ui->timeLab6);
    datetimes->push_back(ui->timeLab7);
    this->temperatures=new QVector<QLabel*>;
    temperatures->push_back(ui->tLab1);
    temperatures->push_back(ui->tLab2);
    temperatures->push_back(ui->tLab3);
    temperatures->push_back(ui->tLab4);
    temperatures->push_back(ui->tLab5);
    temperatures->push_back(ui->tLab6);
    temperatures->push_back(ui->tLab7);
    this->icons=new QVector<QLabel*>;
    icons->push_back(ui->ticon1);
     icons->push_back(ui->ticon2);
     icons->push_back(ui->ticon3);
     icons->push_back(ui->ticon4);
     icons->push_back(ui->ticon5);
     icons->push_back(ui->ticon6);
     icons->push_back(ui->ticon7);
    this->days=new QVector<QLabel*>;
     days->push_back(ui->dayLab1);
     days->push_back(ui->dayLab2);
     days->push_back(ui->dayLab3);
     days->push_back(ui->dayLab4);
     days->push_back(ui->dayLab5);
     days->push_back(ui->dayLab6);
     days->push_back(ui->dayLab7);
}

Weather::~Weather()
{
    delete ui;
}

void Weather::on_backBtn_clicked()
{
    emit backIndex();
}

void Weather::showDate()
{
    QDateTime datetime=QDateTime::currentDateTime();
    QString date=datetime.toString("yyyy-MM-dd");
    ui->dateLab->setText(date);
    QString time=datetime.toString("hh:mm:ss ddd");
    ui->timeLab->setText(time);
}

void Weather::showWeather(QNetworkReply *reply)
{
    QByteArray arr=reply->readAll();
    QJsonDocument document=QJsonDocument::fromJson(arr);
    if(document.isObject()){
        QJsonObject object= document.object();
        QString city=object.value("city").toString();
        ui->cityLab->setText(city);
         QJsonValue data=object.value("data");
         if(data.isArray()){
             QJsonArray dataArr=data.toArray();
             for(int i=0;i<dataArr.size();i++){
                 QJsonObject everyday=dataArr.at(i).toObject();
                QString date=everyday.value("date").toString().append("\n").append(everyday.value("week").toString());
                datetimes->at(i)->setText(date);
                QString tmp=everyday.value("tem2").toString().append("℃~").append(everyday.value("tem1").toString()).append("℃");
                temperatures->at(i)->setText(tmp);
                QString day=everyday.value("wea").toString();
                days->at(i)->setText(day);
                QString icon=everyday.value("wea_img").toString();
                QString path = QString(":/imgs/cherry/")
                             .append(icon).append(".png");
                QPixmap map = QPixmap(path);
                       icons->at(i)->setPixmap(map);
                       icons->at(i)->setScaledContents(true);
                       if(i==0){

                                  ui->tLab->setText(everyday.value("tem").toString().append("℃"));
                                  ui->dayLab->setText(everyday.value("wea").toString());

                                  QString path = QString(":/imgs/cherry/")
                                          .append(everyday.value("wea_img").toString()).append(".png");
                                  QPixmap map = QPixmap(path);
                                  ui->ticon->setPixmap(map);
                                  ui->ticon->setScaledContents(true);
                              }
             }
         }
    }

}




