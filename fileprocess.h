#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <QObject>
#include <QDebug>
#include<QDir>
#include<QFileInfo>
#include<QFile>
#include <QDesktopServices>
#include "listdatamodel.h"
#include <QUrl>
#include <QSettings>
#include "task.h"

class FileProcess:public QObject
{
    Q_OBJECT

    enum {
        PROJECT_ERROR=0,
        PROJECT_FINISH=1,
        PROJECT_EXIST=2,
    };
public:
    explicit FileProcess(QObject *object = nullptr);

    bool copyUVprojx() const;
    bool copyMain() const;

    Task *getTask() const;
    void setTask(Task *value);

public slots:
    void createProject();
    bool openProject();
private:
    Task *task;
    Config * config;
};

#endif // FILEPROCESS_H
