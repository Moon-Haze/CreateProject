#ifndef CONFIG_H
#define CONFIG_H
#include<QString>
#include<QObjectUserData>
#include<QStringList>
#include<QSettings>

class Config
{
public:
    Config();

    QString getProjectPath() const;
    void setProjectPath(const QString &value);
    QStringList getStringlist() const;
    void setStringlist(const QStringList &value);
    void addString(const QString &value);
    bool indexOf(const QString &value);

private:
    QString projectPath;
    QStringList stringlist;
};

#endif // CONFIG_H
