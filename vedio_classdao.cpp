#include "vedio_classdao.h"

Vedio_classDao::Vedio_classDao()
{
    //加载数据库驱动
        this->db=QSqlDatabase::addDatabase("QSQLITE");

        //设置数据库文件路径
        db.setDatabaseName("D:/QTproject/carSystem/DB/carSystem.db");
}

void Vedio_classDao::add(Video_class &locvideo)

{

        //打开数据库
        if (!db.open()) {
            qDebug()<<"打开数据库失败！"<<db.lastError().text();
            return;
        }
        //执行sql语句
        QString sql="insert into t_video(name,video_path) values('%1','%2')";
        sql=sql.arg(locvideo.getName()).arg(locvideo.getVideo_path());
        QSqlQuery query;
        if (!query.exec(sql)) {
            qDebug()<<"新增视频失败！"<<query.lastError().text();
        }
        else {
//            qDebug()<<"新增视频成功！";
        }
        db.close();
}
void Vedio_classDao::update(Video_class &locvideo)
{

        //打开数据库
        if (!db.open()) {
            qDebug()<<"打开数据库失败！"<<db.lastError().text();
            return;
        }
        //执行sql语句
        QString sql="update t_video set name='%1',path='%2' where id=%5";
        sql=sql.arg(locvideo.getName()).arg(locvideo.getVideo_path());
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

void Vedio_classDao::deById(int id)
{

       //打开数据库
       if (!db.open()) {
           qDebug()<<"打开数据库失败！"<<db.lastError().text();
           return;
       }
       //执行sql语句
       QString sql="delete from t_video where id="+QString::number(id);
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

Video_class *Vedio_classDao::findById(int id)
{
    Video_class *locvideo=nullptr;

        //打开数据库
        if (!db.open()) {
            qDebug()<<"打开数据库失败！"<<db.lastError().text();
            return locvideo;
        }

        //执行sql语句
        QString sql="select * from t_video where id="+QString::number(id);
        QSqlQuery query;
        if (!query.exec(sql)) {
            qDebug()<<"查询失败！"<<query.lastError().text();
        }
        else {
            qDebug()<<"查询成功！";
            locvideo=new Video_class;
            while(query.next()){
                int id=query.value("id").toInt();
                QString name=query.value("name").toString();
                QString path=query.value("video_path").toString();
                locvideo->setId(id);
                locvideo->setName(name);
                locvideo->setVideo_path(path);


            }

        }
        //关闭数据库
        db.close();
        return locvideo;

}

QVector<Video_class> Vedio_classDao::findAll()
{
    QVector<Video_class> localvideos;

            //打开数据库
            if (!db.open()) {
                qDebug()<<"打开数据库失败！"<<db.lastError().text();
                return localvideos;
            }

            //执行sql语句
            QString sql="select * from t_video";
            QSqlQuery query;
            if (!query.exec(sql)) {
                qDebug()<<"查询失败！"<<query.lastError().text();
            }
            else {
//                qDebug()<<"查询成功！";
                while(query.next()){
                    int id=query.value("id").toInt();
                    QString name=query.value("name").toString();
                    QString video_path=query.value("video_path").toString();
                    Video_class locvideo(id,name,video_path);
                    localvideos.push_back(locvideo);

                }

            }
            //关闭数据库
            db.close();
            return localvideos;
}

