#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QString>
#include <QFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setBtnIcon(":/imgs/weather.png",ui->weatherBtn);
    setBtnIcon(":/imgs/music.png",ui->musicBtn);
    setBtnIcon(":/imgs/video.png",ui->videoBtn);
    setBtnIcon(":/imgs/location.png",ui->locationBtn);
    this->timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::showDate);
    timer->start(1000);
    this->filter=new Mybtneventfilter;
    ui->weatherBtn->installEventFilter(filter);
    ui->musicBtn->installEventFilter(filter);
    ui->videoBtn->installEventFilter(filter);
    ui->locationBtn->installEventFilter(filter);
    this->weather=new Weather(this);
    this->music=new Music(this);
    this->video=new Video(this);
    this->location=new Location(this);
    ui->stack->addWidget(weather);
    ui->stack->addWidget(music);
     ui->stack->addWidget(video);
     ui->stack->addWidget(location);
    connect(filter,&Mybtneventfilter::goPage,this,&MainWindow::goPage);
    connect(weather,&Weather::backIndex,this,[=]{
        ui->stack->setCurrentWidget(ui->index);
    });
    connect(music,&Music::backIndex,this,[=]{
        ui->stack->setCurrentWidget(ui->index);
    });
    connect(video,&Video::backIndex,this,[=]{
        ui->stack->setCurrentWidget(ui->index);
    });
    connect(location,&Location::backIndex,this,[=]{
        ui->stack->setCurrentWidget(ui->index);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setBtnIcon(const QString &img, QPushButton *btn)
{
    QPixmap map(img);
    int bsize=150;
    int width=bsize;
    int height=bsize;
    QPixmap scaledmap = map.scaled(width,height, Qt::KeepAspectRatio,Qt::SmoothTransformation);
       btn->setIconSize(scaledmap.size());
       btn->setFixedSize(200, 200);
        btn->setIcon(map);
        QFile styleFile(":/qcss/index.qcss");
        styleFile.open(QFile::ReadOnly);
        QString style = QLatin1String(styleFile.readAll());
        btn->setStyleSheet(style);

}

void MainWindow::showDate()
{
 QDateTime date= QDateTime::currentDateTime();
 QString time=date.toString("hh:mm:ss");
 ui->timeLab->setText(time);
 QString datetime=date.toString("yyyy-MM-dd ddd");
 ui->dateLab->setText(datetime);
}

void MainWindow::goPage(QObject *watched)
{
    QString name=watched->objectName();
    if(name=="weatherBtn"){
        ui->stack->setCurrentWidget(weather);
    }
    else if(name=="musicBtn"){
        ui->stack->setCurrentWidget(music);
    }
    else if(name=="videoBtn"){
        ui->stack->setCurrentWidget(video);
    }
    else if(name=="locationBtn"){
         location->getip();
        ui->stack->setCurrentWidget(location);
    }

}


