
#include "video_class.h"

Video_class::Video_class()
{

}

Video_class::Video_class(int id, QString name, QString video_path)
{
    this->id=id;
    this->name=name;
    this->video_path=video_path;

}

Video_class::Video_class(QString name, QString video_path)
{
    this->name=name;
    this->video_path=video_path;

}

int Video_class::getId() const
{
    return id;
}

void Video_class::setId(int value)
{
    this->id = value;
}

QString Video_class::getName() const
{
    return name;
}

void Video_class::setName(const QString &value)
{
    this->name = value;
}

QString Video_class::getVideo_path() const
{
    return video_path;
}

void Video_class::setVideo_path(const QString &value)
{
    this->video_path = value;
}
