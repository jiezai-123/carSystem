#ifndef MUSIC_CLASS_H
#define MUSIC_CLASS_H
#include <QString>

class Music_Class
{
public:
    Music_Class();
      Music_Class(int id,QString name,QString music_path,QString irc_path);
      Music_Class(QString name,QString music_path,QString irc_path);

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

    QString getMusic_path() const;
    void setMusic_path(const QString &value);

    QString getIrc_path() const;
    void setIrc_path(const QString &value);

private:
    int id;
    QString name;
    QString music_path;
    QString irc_path;
};

#endif // MUSIC_CLASS_H
