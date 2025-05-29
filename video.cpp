#include "video.h"
#include "ui_video.h"

Video::Video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Video),
  player(nullptr),
   playlist(nullptr),
   onlineplayer(nullptr),
   onlineplaylist(nullptr),
   currentPlayer(0),
   total(0),
   videoWidget(nullptr)

{
    ui->setupUi(this);
    QFile styleFile(":/qcss/music_bar.qcss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    ui->videoslider->setStyleSheet(style);
    ui->voiceslider->setStyleSheet(style);
    styleFile.close();

    QFile styleFile1(":/qcss/tab.qcss");
    styleFile1.open(QFile::ReadOnly);
    QString style1 = QLatin1String(styleFile1.readAll());
    ui->tabWidget->setStyleSheet(style1);
    styleFile1.close();

    ui->on_stopBtn->setProperty("mode","pause");
    //初始化本地播放器
    this->player=new QMediaPlayer;
    this->playlist=new QMediaPlaylist;
    this->player->setPlaylist(this->playlist);

    //初始化在线播放器
    this->onlineplayer=new QMediaPlayer;
    this->onlineplaylist=new QMediaPlaylist;
    this->onlineplayer->setPlaylist(this->onlineplaylist);
    //初始化声音
    ui->tabWidget->setCurrentIndex(0);
    ui->voiceslider->setRange(0, 100);  // 设置范围为0-100
    ui->voiceslider->setValue(50);
    ui->voiceLab->setText("50%");
    this->player->setVolume(ui->voiceslider->value());
     this->onlineplayer->setVolume(ui->voiceslider->value());

    this->videoWidget=new QVideoWidget();
    this->videoWidget->setStyleSheet("background-color: transparent;");
    ui->horizontalLayout->addWidget(this->videoWidget);
    setActivePlayer(0);
     player->setVideoOutput(this->videoWidget);
//    onlineplayer->setVideoOutput(this->videoWidget);
    this->filter=new ScreeneventFilter;
    this->videoWidget->installEventFilter(this->filter);

    //信号与槽
    connect(player,&QMediaPlayer::positionChanged,this,&Video::handlePositionChanged);
    connect(player,&QMediaPlayer::durationChanged,this,&Video::handleDurationChanged);
    connect(filter,&ScreeneventFilter::smallScreen,this,&Video::showSmallScreen);
    connect(onlineplayer,&QMediaPlayer::positionChanged,this,&Video::handlePositionChanged);
    connect(onlineplayer,&QMediaPlayer::durationChanged,this,&Video::handleDurationChanged);
    connect(this,&Video::showonline,this,&Video::reloadonline);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &Video::on_tabWidget_tabBarClicked);

    //加载数据库列表
    loadLocalVideosFromDatabase();
}

Video::~Video()
{if (player) player->stop();
 if (onlineplayer) onlineplayer->stop();
    delete ui;
}

void Video::loadLocalVideosFromDatabase()
{
    Vedio_classDao vedioDao;
        QVector<Video_class> videoList = vedioDao.findAll();

        // 清空现有列表
        ui->localList->clear();
        playlist->clear();

        // 添加数据库中的视频到列表
        for (const Video_class& video : videoList) {
            QString filePath = video.getVideo_path();
            QFileInfo fileInfo(filePath);

            // 检查文件是否存在
            if (fileInfo.exists()) {
                // 添加到播放列表
                playlist->addMedia(QMediaContent(QUrl::fromLocalFile(filePath)));

                // 添加到UI列表
                QListWidgetItem *item = new QListWidgetItem(fileInfo.baseName());
                ui->localList->addItem(item);
            } else {
                qDebug() << "没有找到:" << filePath;
            }
        }
}

void Video::setActivePlayer(int playerType)
{

        if (playerType == currentPlayer)
               return;
    // 根据选择的播放器类型，设置视频输出
    if (playerType == 0 && player) {
         player->setPlaylist(playlist);
         player->setVideoOutput(this->videoWidget);
    } else if (playerType == 1 && onlineplayer) {
         onlineplayer->setPlaylist(onlineplaylist);
        onlineplayer->setVideoOutput(this->videoWidget);
    }
    // 停止当前活动的播放器
    if (currentPlayer == 0 && player) {
            player->stop();
             player->setMedia(QMediaContent());
             playlist->clear();
             ui->localList->clear();
        } else if (currentPlayer == 1 && onlineplayer) {
            onlineplayer->stop();
            onlineplayer->setMedia(QMediaContent());
            onlineplaylist->clear();
            ui->onlineList->clear();
        }
        currentPlayer = playerType;



}


void Video::reloadonline()
{
    this->manager=new QNetworkAccessManager;
     QString url="http://localhost/video/video.json";
        manager->get(QNetworkRequest(QUrl(url)));
        connect(manager,&QNetworkAccessManager::finished,this,&Video::showline);
}

void Video::on_backBtn_clicked()
{
    emit backIndex();
}

void Video::handleDurationChanged(qint64 duration)
{
    this->total=duration;
    QTime totalTime(0,0);
//    在当前时间对象上加多少毫秒
        totalTime = totalTime.addMSecs(duration);
        QString totalTimeStr = totalTime.toString("mm:ss");
        ui->totalLab->setText(totalTimeStr);
        ui->videoslider->setValue(0);
}

void Video::handlePositionChanged(qint64 position)
{
    if(this->total>0){
        int val=position*100/this->total;
        ui->videoslider->setValue(val);
        QTime currentTime(0,0);
        currentTime = currentTime.addMSecs(position);
        QString currentTimeStr = currentTime.toString("mm:ss");
        ui->currentLab->setText(currentTimeStr);
    }
}




void Video::on_on_stopBtn_clicked()
{
    QString mode=ui->on_stopBtn->property("mode").toString();
        if(mode=="pause"){
               ui->on_stopBtn->setProperty("mode","play");
               if (currentPlayer == 0) {
               this->player->play();
               }
               else if (currentPlayer == 1){
               this->onlineplayer->play();
               }
           }else{
               ui->on_stopBtn->setProperty("mode","pause");
               if (currentPlayer == 0){
               this->player->pause();
                }
                 else if (currentPlayer == 1){
               this->onlineplayer->pause();
                 }
           }
           ui->on_stopBtn->style()->unpolish(ui->on_stopBtn);
                   ui->on_stopBtn->style()->polish(ui->on_stopBtn);
                   ui->on_stopBtn->update();
}

void Video::on_openVideo_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "选择视频",
                            "D:/QTproject/carSystem/video",
                            "Musics(*.mp4 *.avi)");
    for(QString file:files){
        this->playlist->addMedia(QMediaContent(QUrl::fromLocalFile(file)));
        QFileInfo info(file);
        ui->localList->addItem(info.baseName());
        Vedio_classDao vedioDao;
        Video_class video_class(info.baseName(),file);
        vedioDao.add(video_class);
    }
    loadLocalVideosFromDatabase();
}

void Video::on_preBtn_clicked()
{
     if (currentPlayer == 0 && playlist->mediaCount() > 0){
    int row=ui->localList->currentRow();
    if(row==0){
        row=ui->localList->count()-1;
    }
    else{
        row--;
    }
    ui->localList->setCurrentRow(row);
    ui->localList->currentItem()->setSelected(true);
    this->playlist->setCurrentIndex(row);
    this->player->play();
    }
    else if (currentPlayer == 1 && onlineplaylist->mediaCount() > 0){
    int row1=ui->onlineList->currentRow();
    if(row1==0){
        row1=ui->onlineList->count()-1;
    }
    else{
        row1--;
    }
    ui->onlineList->setCurrentRow(row1);
    ui->onlineList->currentItem()->setSelected(true);
    this->onlineplaylist->setCurrentIndex(row1);
    this->onlineplayer->play();
    }
    ui->on_stopBtn->setProperty("mode","play");
    ui->on_stopBtn->style()->unpolish(ui->on_stopBtn);
    ui->on_stopBtn->style()->polish(ui->on_stopBtn);
    ui->on_stopBtn->update();
}

void Video::on_nextBtn_clicked()
{
    if (currentPlayer == 0 && playlist->mediaCount() > 0){
    int row=ui->localList->currentRow();
    if(row==ui->localList->count()-1){
        row=0;
    }
    else{
        row++;
    }
    ui->localList->setCurrentRow(row);
    ui->localList->currentItem()->setSelected(true);
    this->playlist->setCurrentIndex(row);
    this->player->play();
    }
    else if (currentPlayer == 1 && onlineplaylist->mediaCount() > 0){
        int row1=ui->onlineList->currentRow();
        if(row1==ui->onlineList->count()-1){
            row1=0;
        }
        else{
            row1++;
        }
        ui->onlineList->setCurrentRow(row1);
        ui->onlineList->currentItem()->setSelected(true);
        this->onlineplaylist->setCurrentIndex(row1);
        this->onlineplayer->play();
    }
     ui->on_stopBtn->setProperty("mode","play");
    ui->on_stopBtn->style()->unpolish(ui->on_stopBtn);
    ui->on_stopBtn->style()->polish(ui->on_stopBtn);
    ui->on_stopBtn->update();
}

void Video::on_voiceBtn_clicked()
{
    ui->voiceBtn->setStyleSheet("border-image: url(:/imgs/voiceclose.png);");
    ui->voiceLab->setText("0");
    ui->voiceslider->setValue(0);
}

void Video::on_voiceslider_valueChanged(int value)
{
    if(value>0){
        ui->voiceBtn->setStyleSheet("border-image: url(:/imgs/voiceopen.png);");
    }
       if(ui->voiceslider->value()==0){
           ui->voiceBtn->setStyleSheet("border-image: url(:/imgs/voiceclose.png);");
       }


    QString str="%1%";
    ui->voiceLab->setText(str.arg(QString::number(value)));

    this->player->setVolume(ui->voiceslider->value());
    this->onlineplayer->setVolume(ui->voiceslider->value());

}


void Video::on_bigBtn_clicked()
{   this->videoWidget->setWindowFlags(Qt::Window);
    this->videoWidget->showFullScreen();
}

void Video::on_localList_itemDoubleClicked(QListWidgetItem *item)
{
    int index=ui->localList->row(item);
    this->playlist->setCurrentIndex(index);
    setActivePlayer(0);
    this->player->play();
    ui->localList->setCurrentRow(index);
    ui->on_stopBtn->setProperty("mode","play");
    ui->on_stopBtn->style()->unpolish(ui->on_stopBtn);
    ui->on_stopBtn->style()->polish(ui->on_stopBtn);
    ui->on_stopBtn->update();
}



void Video::showSmallScreen()
{
    this->videoWidget->setWindowFlags(Qt::SubWindow);
    this->videoWidget->showNormal();
}

void Video::on_onlineList_itemDoubleClicked(QListWidgetItem *item)
{
    int index=ui->onlineList->row(item);
    this->onlineplaylist->setCurrentIndex(index);
    setActivePlayer(1);
    this->onlineplayer->play();
    ui->onlineList->setCurrentRow(index);
    ui->on_stopBtn->setProperty("mode","play");
    ui->on_stopBtn->style()->unpolish(ui->on_stopBtn);
    ui->on_stopBtn->style()->polish(ui->on_stopBtn);
    ui->on_stopBtn->update();
}



void Video::showline(QNetworkReply *reply)
{
    QByteArray arr=reply->readAll();
    QJsonDocument doc=QJsonDocument::fromJson(arr);
    if(doc.isArray()){
        QJsonArray dataArr=doc.array();
        ui->onlineList->clear();
        onlineplaylist->clear();
        for(int i=0;i<dataArr.size();i++){
            QJsonObject everyvideo=dataArr.at(i).toObject();
            QString video=everyvideo.value("path").toString();
            QFileInfo info(video);
            this->onlineplaylist->addMedia(QMediaContent(QUrl(video)));
            ui->onlineList->addItem(info.baseName());
        }

    }
}



void Video::on_tabWidget_tabBarClicked(int index)
{
    setActivePlayer(index);


        if (index == 1) {

            emit showonline();
        }
        else if(index==0){
            loadLocalVideosFromDatabase();
        }

}

void Video::on_videoslider_sliderReleased()
{
    int val=ui->videoslider->value();
    int postion=val*this->total/100;
    this->player->setPosition(postion);
    this->onlineplayer->setPosition(postion);
}
