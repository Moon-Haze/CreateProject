#include "task.h"

Task::Task(QObject *object):
    QObject(object),setList(new ListDataList()),
    moduleList(new ListDataList()),config(new Config){

    addTask("添加相应的.h 和.cpp 文件",TaskClass::ADD_HCPP);
    QDir module{"module"};
    if(module.exists()){
        for(QString &item:module.entryList(QDir::NoDotAndDotDot|QDir::AllDirs)){
            moduleList->append(item);
        }
    }
}

Task::~Task()
{
    delete setList;
    delete moduleList;
    delete config;
}

void Task::addTask(const QString &task, const TaskClass &tsc)

{
    tasks.insert(task,tsc);
    switch (tsc) {
    case SET_PROJECT:
    case ADD_HCPP:
        setList->append(task);
        break;
    case COPY_MODULE:
        moduleList->append(task);
        break;
    }
}


ListDataList *Task::getSetList() const
{
    return setList;
}

ListDataList *Task::getModuleList() const
{
    return moduleList;
}

QVector<TaskConfig> Task::getFinishTask() const
{
    QString name=config->getName();
    QVector<TaskConfig> fTasks;
    for(auto &item : moduleList->items()){
        if(item.selected){
            fTasks.append(TaskConfig{item.module,TaskClass::COPY_MODULE});
        }
    }
    for(auto &item :setList->items()){
        switch (tasks[item.module]) {
        case ADD_HCPP:
            if(item.selected&&(!moduleList->getSelected(config->getName().toUpper()))){
                fTasks.append(TaskConfig{name,TaskClass::ADD_HCPP});
            }
            break;
        case SET_PROJECT:
            break;
        case COPY_MODULE:
            break;
        }

    }
    return fTasks;
}

Config *Task::getConfig() const
{
    return config;
}

void Task::setConfig(Config *value)
{
    config = value;
}
