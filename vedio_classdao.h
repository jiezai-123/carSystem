#ifndef VEDIO_CLASSDAO_H
#define VEDIO_CLASSDAO_H
#include "video_class.h"
#include<QVector>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include <QDebug>
class Vedio_classDao
{
public:
    Vedio_classDao();
    //数据库的新增
           void add(Video_class &locvideo);
           //数据库的修改
           void update(Video_class &locvideo);
           //数据库的删除
           void deById(int id);
           //数据库的查询
           Video_class *findById(int id);
           QVector<Video_class> findAll();
};

#endif // VEDIO_CLASSDAO_H
