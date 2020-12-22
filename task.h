#ifndef TASK_H
#define TASK_H

#include<QHash>
#include<QString>
#include<QVector>
#include "listdatalist.h"
#include "config.h"

enum TaskClass{
    ADD_HCPP,
    SET_PROJECT,
    COPY_MODULE,
};

struct TaskConfig{
    QString name;
    TaskClass taskclass;
};

class Task:public QObject
{
    Q_OBJECT

public:
    Task(QObject *object =nullptr);
    ~Task();
    void addTask(const QString& task,const TaskClass& tsc);

    ListDataList *getSetList() const;

    ListDataList *getModuleList() const;

    QVector<TaskConfig> getFinishTask() const;

    Config *getConfig() const;

    void setConfig(Config *value);

private:
    QHash<QString,TaskClass> tasks;
    ListDataList *setList;
    ListDataList *moduleList;
    Config *config;
};
#endif // TASK_H
