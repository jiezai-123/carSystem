#ifndef MUSIC_CLASSDAO_H
#define MUSIC_CLASSDAO_H
#include<QVector>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include <QDebug>
#include "music_class.h"

class Music_classDao
{
public:
    Music_classDao();
    //数据库的新增
           void add(Music_Class &locmusic);
           //数据库的修改
           void update(Music_Class &locmusic);
           //数据库的删除
           void deById(int id);
           //数据库的查询
           Music_Class *findById(int id);
           QVector<Music_Class> findAll();
};

#endif // MUSIC_CLASSDAO_H
