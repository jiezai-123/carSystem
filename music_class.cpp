#include "music_class.h"

Music_Class::Music_Class()
{

}

Music_Class::Music_Class(int id, QString name, QString music_path, QString irc_path)
{
    this->id=id;
    this->name=name;
    this->music_path=music_path;
    this->irc_path=irc_path;

}

Music_Class::Music_Class(QString name, QString music_path, QString irc_path)
{
    this->name=name;
    this->music_path=music_path;
    this->irc_path=irc_path;

}

int Music_Class::getId() const
{
    return id;
}

void Music_Class::setId(int value)
{
    id = value;
}

QString Music_Class::getName() const
{
    return name;
}

void Music_Class::setName(const QString &value)
{
    name = value;
}

QString Music_Class::getMusic_path() const
{
    return music_path;
}

void Music_Class::setMusic_path(const QString &value)
{
    music_path = value;
}

QString Music_Class::getIrc_path() const
{
    return irc_path;
}

void Music_Class::setIrc_path(const QString &value)
{
    irc_path = value;
}
