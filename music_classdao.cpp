
#include "music_classdao.h"

Music_classDao::Music_classDao()
{
    //加载数据库驱动
        this->db=QSqlDatabase::addDatabase("QSQLITE");

        //设置数据库文件路径
        db.setDatabaseName("D:/QTproject/carSystem/DB/carSystem.db");

}

void Music_classDao::add(Music_Class &locmusic)
{

        //打开数据库
        if (!db.open()) {
            qDebug()<<"打开数据库失败！"<<db.lastError().text();
            return;
        }
        //执行sql语句
        QString sql="insert into t_music(name,music_path,irc_path) values('%1','%2','%3')";
        sql=sql.arg(locmusic.getName()).arg(locmusic.getMusic_path()).arg(locmusic.getIrc_path());
        QSqlQuery query;
        if (!query.exec(sql)) {
            qDebug()<<"新增视频失败！"<<query.lastError().text();
        }
        else {
//            qDebug()<<"新增视频成功！";
        }
        //关闭数据库
        db.close();

}

void Music_classDao::update(Music_Class &locmusic)
{
        //打开数据库
        if (!db.open()) {
            qDebug()<<"打开数据库失败！"<<db.lastError().text();
            return;
        }
        //执行sql语句
        QString sql="update t_music set name='%1',music_path='%2'irc_path='%3', where id=%5";
        sql=sql.arg(locmusic.getName()).arg(locmusic.getMusic_path().arg(locmusic.getIrc_path()));
        QSqlQuery query;
        if (!query.exec(sql)) {
            qDebug()<<"修改失败！"<<query.lastError().text();
        }
        else {
            qDebug()<<"修改成功！";
        }
        //关闭数据库
        db.close();
}

void Music_classDao::deById(int id)
{
       //打开数据库
       if (!db.open()) {
           qDebug()<<"打开数据库失败！"<<db.lastError().text();
           return;
       }
       //执行sql语句
       QString sql="delete from t_music where id="+QString::number(id);
       QSqlQuery query;
       if (!query.exec(sql)) {
           qDebug()<<"删除失败！"<<query.lastError().text();
       }
       else {
           qDebug()<<"删除成功！";
       }
       //关闭数据库
       db.close();
}

Music_Class *Music_classDao::findById(int id)
{
    Music_Class *locmusic=nullptr;

        //打开数据库
        if (!db.open()) {
            qDebug()<<"打开数据库失败！"<<db.lastError().text();
            return locmusic;
        }

        //执行sql语句
        QString sql="select * from t_music where id="+QString::number(id);
        QSqlQuery query;
        if (!query.exec(sql)) {
            qDebug()<<"查询失败！"<<query.lastError().text();
        }
        else {
            qDebug()<<"查询成功！";
            locmusic=new Music_Class;
            while(query.next()){
                int id=query.value("id").toInt();
                QString name=query.value("name").toString();
                QString music_path=query.value("music_path").toString();
                QString irc_path=query.value("irc_path").toString();
                locmusic->setId(id);
                locmusic->setName(name);
                locmusic->setMusic_path(music_path);
                locmusic->setIrc_path(irc_path);


            }

        }
        //关闭数据库
        db.close();
        return locmusic;
}

QVector<Music_Class> Music_classDao::findAll()
{
    QVector<Music_Class> localmusics;

        //打开数据库
        if (!db.open()) {
            qDebug()<<"打开数据库失败！"<<db.lastError().text();
            return localmusics;
        }

        //执行sql语句
        QString sql="select * from t_music";
        QSqlQuery query;
        if (!query.exec(sql)) {
            qDebug()<<"查询失败！"<<query.lastError().text();
        }
        else {
//            qDebug()<<"查询成功！";
            while(query.next()){
                int id=query.value("id").toInt();
                QString name=query.value("name").toString();
                QString music_path=query.value("music_path").toString();
                QString irc_path=query.value("irc_path").toString();
                Music_Class locmusic(id,name,music_path,irc_path);
                localmusics.push_back(locmusic);

            }

        }
        //关闭数据库
        db.close();
        return localmusics;
}
