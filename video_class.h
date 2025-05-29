#ifndef VIDEO_CLASS_H
#define VIDEO_CLASS_H
#include <QString>

class Video_class
{
public:
    Video_class();
     Video_class(int id,QString name,QString video_path);
    Video_class(QString name,QString video_path);
    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

    QString getVideo_path() const;
    void setVideo_path(const QString &value);

private:
    int id;
    QString name;
    QString video_path;
};

#endif // VIDEO_CLASS_H
