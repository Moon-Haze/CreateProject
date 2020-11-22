#include "config.h"

Config::Config()
{

}

QString Config::getProjectPath() const
{
    return projectPath;
}

void Config::setProjectPath(const QString &value)
{
    projectPath = value;
}

QStringList Config::getStringlist() const
{
    return stringlist;
}

void Config::setStringlist(const QStringList &value)
{
    stringlist = value;
}

void Config::addString(const QString &value)
{
    stringlist<<value;
}

bool Config::indexOf(const QString &value)
{
    if(stringlist.indexOf(value)!=-1){
        return true;
    }
    return false;
}

