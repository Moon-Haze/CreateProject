#include "fileprocess.h"

bool FileProcess::copyDir(const QString &source, const QString &destination, bool override) {
    QDir directory(source);
    QDir des(destination);
    if (!des.exists()) {
        des.mkpath(destination);
    }
    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();
    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i = 0; i != fileNames.size(); ++i) {
        const QString &fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink()) {
            if (override) {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        } else if (fileInfo.isDir()) {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override)) {
                error = true;
            }
        }
    }
    return !error;
}

bool FileProcess::writeString(const QString &path, const QString &context) {
    qDebug() << path;
    QFile des(path);
    if ((!des.exists()) || (!des.isOpen())) {
        QDir qDir;
        qDir.mkpath(path.left(path.lastIndexOf("/")));
        des.open(QIODevice::ReadWrite | QIODevice::Text);
    }
    if (des.isOpen()) {
        QTextStream out(&des);
#if(QT_VERSION <= QT_VERSION_CHECK(5, 15, 2))
        out.setCodec("utf-8");
#else
        out.setEncoding(QStringConverter::Utf8);
#endif
        out << context;
        out.flush();
        return true;
    }
    return false;
}

bool FileProcess::copyFile(const QString &source, const QString &destination, const QString &replaceStr) {
    QFile src(source);
    if (src.open(QFile::ReadOnly)) {
        QTextStream in(&src);
        in.setCodec("utf-8");
        QString context = in.readAll();
        context.replace("TEMPLATE", replaceStr.toUpper());
        context.replace("Template", replaceStr);
        context.replace("\n", "");
        return writeString(destination, context);
    }
    return true;
}

FileProcess::FileProcess(QObject *object) : QObject(object) {}

void FileProcess::createProject() {
    QVector<TaskConfig> taskClass = task->getFinishTask();
    QString source = "template", module = "module";
    QDir file(source);
    bool is = true;
    QString path = config->getPath();
    QString name = config->getName();
    QDir projects(path + "/" + name);
    projectConfig->setName(name);
    static int status = 0;
    if (projects.exists()) {
        if (status == PROJECT_EXIST) {
            projects.mkpath(QDir::currentPath());
        } else {
            emit config->createOver(PROJECT_EXIST);
            status = PROJECT_EXIST;
            return;
        }
    }
    for (TaskConfig &item : taskClass) {
        switch (item.taskClass) {
            case ADD_H_AND_CPP:
                if (item.name != "main") {
                    is = is && writeString(path + "/" + name + "/HARDWARE/" + name.toLower() + ".c",
                                           "#include \"" + name.toLower() + ".h\"\n") &&
                         writeString(path + "/" + name + "/HARDWARE/" + name.toLower() + ".h",
                                     "#ifndef __" + name.toUpper()
                                     + "_H\n#define __" +
                                     name.toUpper() +
                                     "_H\n#include \"delay.h\"\n#include \"sys.h\"\n#endif\n");
                    
                    projectConfig->addFile(item.name);
                }
                break;
            case COPY_MODULE:
                is = is && copyDir(module + "/" + item.name, path + "/" +
                                                             name + "/HARDWARE", true);
                projectConfig->addFile(item.name);
                break;
            case SET_PROJECT:
                break;
        }
    }
    if (file.exists()) {
        is = copyDir(source + "/file", path + "/" + name, true) &&
             writeString(path + "/" + name + "/USER/main.c", projectConfig->mainToQString()) &&
             writeString(path + "/" + name + "/USER/" + name + ".uvguix.Moon",
                         projectConfig->moonToQString(source + "/Template.uvguix.Moon")) &&
             writeString(path + "/" + name + "/USER/" + name + ".uvoptx",
                         projectConfig->uvoptxToQString("template/Template.uvoptx")) &&
             writeString(path + "/" + name + "/USER/" + name + ".uvprojx",
                         projectConfig->uvprojxToQString("template/Template.uvprojx"));
        if (is) {
            emit config->createOver(PROJECT_FINISH);
            return;
        }
    }
    emit config->createOver(PROJECT_ERROR);
}

bool FileProcess::openProject() {
    QString path = task->getConfig()->getPath();
    QString name = task->getConfig()->getName();
    bool is_open = true;
#ifdef Q_OS_WIN32
    is_open = QDesktopServices::openUrl(
            QUrl("file:///" + path + "/" + name + "/USER/" + name + ".uvprojx", QUrl::TolerantMode));
#else
    QString  cmd= QString("xdg-open ")+ "file:///"+path+"/"+name+"/USER/"+name+".uvprojx";
    system(cmd.toStdString().c_str());
#endif
    return is_open;
}

Task *FileProcess::getTask() const {
    return task;
}

void FileProcess::setTask(Task *value) {
    task = value;
    config = value->getConfig();
}
