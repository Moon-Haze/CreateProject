#include "config.h"

Config::Config(QObject *object):QObject(object)
{
    settings.beginGroup("CreateProject");
    for(QString key:settings.allKeys()){
        if(key=="projectPath"){
            path=settings.value("projectPath").toString();
            checked=(path!="");
        }else{
            setPaths(settings.value("list").toStringList());
        }
    }
    connect(this,&Config::createOver,this,[=](int ques){
        if(ques==1&&(!paths.count(path))){
            paths<<path;
        }
    });
}

Config::~Config()
{
    settings.beginGroup("CreateProject");
    settings.setValue("list",paths);
    if(checked){
        settings.setValue("projectPath",path);
    }
    settings.endGroup();
}

QString Config::getPath() const
{
    return path;
}

void Config::setPath(QString value)
{
    path = value;
}

QStringList Config::getPaths() const
{
    return paths;
}

void Config::setPaths(QStringList value)
{
    paths = value;
}

void Config::addPath(QString value)
{
    paths<<value;
}


bool Config::getChecked() const
{
    return checked;
}

void Config::setChecked(bool value)
{
    checked = value;
}

QString Config::getName() const
{
    return name;
}

void Config::setName(QString value)
{
    name = value;
}


