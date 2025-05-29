#include "music.h"
#include "ui_music.h"
Music::Music(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Music),
  player(nullptr),
   playlist(nullptr),
   onlineplayer(nullptr),
   onlineplaylist(nullptr),
   currentPlayer(0),
   total(0),
   videoWidget(nullptr)
{
    manager = new QNetworkAccessManager(this);
    ui->setupUi(this);
    QFile styleFile(":/qcss/music_bar.qcss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    ui->musicslider->setStyleSheet(style);
    ui->voiceslider->setStyleSheet(style);
    styleFile.close();

    QFile styleFile1(":/qcss/tab.qcss");
    styleFile1.open(QFile::ReadOnly);
    QString style1 = QLatin1String(styleFile1.readAll());
    ui->tabWidget->setStyleSheet(style1);
    styleFile1.close();


    QFile styleFile2(":/qcss/lrcitem.qcss");
    styleFile2.open(QFile::ReadOnly);
    QString style2 = QLatin1String(styleFile2.readAll());
    ui->ircList->setStyleSheet(style2);
    styleFile2.close();


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


    //信号与槽
    connect(player,&QMediaPlayer::positionChanged,this,&Music::handlePositionChanged);
    connect(player,&QMediaPlayer::durationChanged,this,&Music::handleDurationChanged);
    connect(onlineplayer,&QMediaPlayer::positionChanged,this,&Music::handlePositionChanged);
    connect(onlineplayer,&QMediaPlayer::durationChanged,this,&Music::handleDurationChanged);
    connect(this,&Music::showonline,this,&Music::reloadonline);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &Music::on_tabWidget_tabBarClicked);

    //加载数据库列表
    loadLocalMusicFromDatabase();



}

Music::~Music()
{
    delete ui;
}
void Music::on_tabWidget_tabBarClicked(int index)
{
    setActivePlayer(index);
        if(index==0){
            loadLocalMusicFromDatabase();
        }
        else if (index == 1) {

            emit showonline();
        }
}

void Music::loadLocalMusicFromDatabase()
{
    Music_classDao musicDao;
        QVector<Music_Class> musicList = musicDao.findAll();

        // 清空现有列表
        ui->localList->clear();
        playlist->clear();

        // 添加数据库中的音乐到列表
        for (const Music_Class& music : musicList) {
            QString music_filePath = music.getMusic_path();
            QString irc_filepath=music.getIrc_path();
            QFileInfo fileInfo(music_filePath);

            // 检查文件是否存在
            if (fileInfo.exists()) {
                // 添加到播放列表
                playlist->addMedia(QMediaContent(QUrl::fromLocalFile(music_filePath)));

                // 添加到UI列表
                QListWidgetItem *item = new QListWidgetItem(fileInfo.baseName());
                item->setData(Qt::UserRole, irc_filepath);
                ui->localList->addItem(item);
            } else {
                qDebug() << "没有找到:" << music_filePath;
            }
        }
}

QList<Lyric> Music::parseLyricFile(const QString &filePath)
{
    QList<Lyric> lyrics;
       QFile file(filePath);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           return lyrics;
       }

       QRegularExpression re("\\[(\\d{2}):(\\d{2})\\.(\\d{2})\\](.*)");
       while (!file.atEnd()) {
           QString line = file.readLine().trimmed();
           QRegularExpressionMatch match = re.match(line);
           if (match.hasMatch()) {
               int minutes = match.captured(1).toInt();
               int seconds = match.captured(2).toInt();
               int milliseconds = match.captured(3).toInt();
               int time = (minutes * 60 + seconds) * 1000 + milliseconds;
               QString text = match.captured(4);
               lyrics.append({time, text});
           }
       }
       file.close();
       return lyrics;
}

void Music::updateLyrics(const QList<Lyric> &lyrics, qint64 position)
{
       ui->ircList->clear();
       for (const Lyric& lyric : lyrics) {
           if (lyric.time <= position) {

               ui->ircList->addItem(lyric.text);
           }
       }
}

QList<Lyric> Music::parseLyricData(const QByteArray &data)
{
    QList<Lyric> lyrics;

        // 尝试自动检测编码
        QTextCodec::ConverterState state;
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QString text = codec->toUnicode(data.constData(), data.size(), &state);

        // 如果UTF-8检测失败，尝试GBK（常见中文编码）
        if (state.invalidChars > 0) {
            codec = QTextCodec::codecForName("GBK");
            text = codec->toUnicode(data);
        }

        QTextStream stream(&text);
        QRegularExpression re("\\[(\\d{2}):(\\d{2})\\.(\\d{2})\\](.*)");

        while (!stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                int minutes = match.captured(1).toInt();
                int seconds = match.captured(2).toInt();
                int milliseconds = match.captured(3).toInt();
                int time = (minutes * 60 + seconds) * 1000 + milliseconds;
                QString text = match.captured(4);
                lyrics.append({time, text});
            }
        }
        return lyrics;
}


void Music::setActivePlayer(int playerType)
{

    if (playerType == currentPlayer)
           return;
    // 停止当前活动的播放器
    if (currentPlayer == 0 && player) {
            player->stop();
             player->setMedia(QMediaContent());
             playlist->clear();
             ui->localList->clear();
             ui->ircList->clear();
        } else if (currentPlayer == 1 && onlineplayer) {
            onlineplayer->stop();
            onlineplayer->setMedia(QMediaContent());
            onlineplaylist->clear();
            ui->onlineList->clear();
            ui->ircList->clear();
        }
        currentPlayer = playerType;

        // 根据选择的播放器类型，设置视频输出
        if (playerType == 0 && player) {
             player->setPlaylist(playlist);
        } else if (playerType == 1 && onlineplayer) {
             onlineplayer->setPlaylist(onlineplaylist);
        }
}

void Music::on_backBtn_clicked()
{
    emit backIndex();
}

void Music::on_on_stopBtn_clicked()
{    QString mode=ui->on_stopBtn->property("mode").toString();
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

void Music::on_openMusic_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "选择音乐",
                            "D:/QTproject/carSystem/music",
                            "Musics(*.mp3)");
    for(QString file:files){
        this->playlist->addMedia(QMediaContent(QUrl::fromLocalFile(file)));
        QFileInfo info(file);
        ui->localList->addItem(info.baseName());
        QString baseName = info.completeBaseName();
        QString irc_path=info.absolutePath()+"/"+baseName+".lrc";
        Music_classDao musicDao;
        Music_Class music_class(info.baseName(),file,irc_path);
        musicDao.add(music_class);
    }
      loadLocalMusicFromDatabase();
}

void Music::on_preBtn_clicked()
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

void Music::on_nextBtn_clicked()
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

void Music::on_voiceBtn_clicked()
{
    ui->voiceBtn->setStyleSheet("border-image: url(:/imgs/voiceclose.png);");
    ui->voiceLab->setText("0");
    ui->voiceslider->setValue(0);

}

void Music::on_localList_itemDoubleClicked(QListWidgetItem *item)
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
//总长度
void Music::handleDurationChanged(qint64 duration)
{
    this->total=duration;
    QTime totalTime(0,0);
//    在当前时间对象上加多少毫秒
        totalTime = totalTime.addMSecs(duration);
        QString totalTimeStr = totalTime.toString("mm:ss");
        ui->totalLab->setText(totalTimeStr);
        ui->musicslider->setValue(0);
}

void Music::handlePositionChanged(qint64 position)
{
    if(this->total>0){
        int val=position*100/this->total;
        ui->musicslider->setValue(val);
        QTime currentTime(0,0);
        currentTime = currentTime.addMSecs(position);
        QString currentTimeStr = currentTime.toString("mm:ss");
        ui->currentLab->setText(currentTimeStr);
        // 根据播放进度显示歌词

        QString lyricFilePath;
        if (currentPlayer == 0) {
            lyricFilePath = ui->localList->currentItem()->data(Qt::UserRole).toString();
            QList<Lyric> lyrics = parseLyricFile(lyricFilePath);
            updateLyrics(lyrics, position);
        } else if(currentPlayer == 1){
            lyricFilePath = ui->onlineList->currentItem()->data(Qt::UserRole).toString();
            if (lyricFilePath.startsWith("http")) {
                this->lrcReply=manager->get(QNetworkRequest(QUrl(lyricFilePath)));
                connect(this->lrcReply, &QNetworkReply::finished,
                    [this, position]() {
                        if (this->lrcReply->error() == QNetworkReply::NoError) {
                            QByteArray data = this->lrcReply->readAll();
                            QList<Lyric> lyrics = parseLyricData(data);
                            updateLyrics(lyrics, position);
                        }
                        this->lrcReply->deleteLater();

                    });

            }
            else {
                QList<Lyric> lyrics = parseLyricFile(lyricFilePath);
                updateLyrics(lyrics, position);

            }

        }

    }
}

void Music::on_musicslider_sliderReleased()
{
    int val=ui->musicslider->value();
    int postion=val*this->total/100;
    this->player->setPosition(postion);
    this->onlineplayer->setPosition(postion);
}


void Music::on_voiceslider_valueChanged(int value)
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

void Music::reloadonline()
{
     QString url="http://localhost/music/music.json";
     this->onlineReply=manager->get(QNetworkRequest(QUrl(url)));
     connect( this->onlineReply,&QNetworkReply::finished,this,&Music::showline);
}

void Music::showline()
{
    QByteArray arr=this->onlineReply->readAll();
    QJsonDocument doc=QJsonDocument::fromJson(arr);
    if(doc.isArray()){
        QJsonArray dataArr=doc.array();
        ui->onlineList->clear();
        onlineplaylist->clear();
        for(int i=0;i<dataArr.size();i++){
            QJsonObject everymusic=dataArr.at(i).toObject();
            QString music_path=everymusic.value("music_path").toString();
            QString irc_path=everymusic.value("irc_path").toString();
            QFileInfo info(music_path);
            this->onlineplaylist->addMedia(QMediaContent(QUrl(music_path)));
            QListWidgetItem *item = new QListWidgetItem(info.baseName());
            item->setData(Qt::UserRole, irc_path);
            ui->onlineList->addItem(item);
        }

    }
}

void Music::on_onlineList_itemDoubleClicked(QListWidgetItem *item)
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
