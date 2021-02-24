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
#include "ProjectConfig.h"

class FileProcess : public QObject {
Q_OBJECT
    enum TaskStatus{
        PROJECT_ERROR = 0,
        PROJECT_FINISH = 1,
        PROJECT_EXIST = 2,
    };
public:
    explicit FileProcess(QObject *object = nullptr);

    Task *getTask() const;

    void setTask(Task *value);

public slots:

    void createProject();

    bool openProject();

private:
    Task *task{};
    Config *config{};
    ProjectConfig *projectConfig{new ProjectConfig()};

    static bool copyDir(const QString &source, const QString &destination, bool override);

    static bool writeString(const QString &path, const QString &context);

    static bool copyFile(const QString &source, const QString &destination, const QString &replaceStr);
};

#endif // FILEPROCESS_H
