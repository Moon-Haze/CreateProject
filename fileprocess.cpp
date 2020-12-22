#include "fileprocess.h"
bool copyDir(const QString &source, const QString &destination, bool override)
{
    QDir directory(source);
    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();
    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i){
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink()) {
            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        }
        else if (fileInfo.isDir()) {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override)){
                error = true;
            }
        }
    }
    return !error;
}

bool writeString(const QString& path,const QString& context){
    QFile des(path);
    if((!des.exists())||(!des.isOpen())){
        des.open(QIODevice::ReadWrite|QIODevice::Text);
    }
    if(des.isOpen()){
        QTextStream out(&des);
#if(QT_VERSION <= QT_VERSION_CHECK(5,15,2))
        out.setCodec("utf-8");
#else
        out.setEncoding(QStringConverter::Utf8);
#endif
        out<<context;
        out.flush();
        return true;
    }
    return false;
}

bool copyFile(const QString &source, const QString &destination, const QString &replacestr){
    QFile src(source);
    if(src.open(QFile::ReadOnly)){
        QTextStream in(&src);
        in.setCodec("utf-8");
        QString context=in.readAll();
        context.replace("TEMPLATE",replacestr.toUpper());
        context.replace("Template",replacestr);
        context.replace("\n","");
        return writeString(destination,context);
    }
    return true;
}

FileProcess::FileProcess(QObject *object):QObject(object){}



bool FileProcess::copyUVprojx() const
{
    QVector<TaskConfig> taskc=task->getFinishTask();
    QFile src("template/Template.uvprojx");
    if(src.open(QFile::ReadOnly)){
        QTextStream in(&src);
        in.setCodec("utf-8");
        QString context=in.readAll();
        context.replace("TEMPLATE",config->getName().toUpper());
        context.replace("Template",config->getName());
        QString hdw{"<Group>\n"
                    "\t<GroupName>HARDWARE</GroupName>\n"
                    "<Files>"};
        for(auto &item :taskc){
            switch (item.taskclass) {
            case COPY_MODULE:
            case ADD_HCPP:
                hdw.append("<File>\n"
                        "\t\t<FileName>"+item.name.toLower()+".c</FileName>\n"
                        "\t\t<FileType>1</FileType>\n"
                        "\t\t<FilePath>..\\HARDWARE\\"+item.name.toLower()+
                           ".c</FilePath>\n\t</File>\n");
                break;
            case SET_PROJECT:
                break;
            }
        }
        hdw.append("\t</Files>\n\t\t</Group>");
        context.replace("<Group><GroupName>HARDWARE</GroupName></Group>",hdw);
        return  writeString(config->getPath()+"/"+config->getName()+"/USER/"+config->getName()+".uvprojx",context);
    }
    return true;
}

bool FileProcess::copyMain() const
{
    QVector<TaskConfig> taskc=task->getFinishTask();
    QString context{"#include \"stm32f10x.h\"\n"
                    "#include \"delay.h\"\n"
                    "#include \"sys.h\"\n"};
    for(auto &item :taskc){
        switch (item.taskclass) {
        case COPY_MODULE:
        case ADD_HCPP:
            context.append("#include \""+item.name.toLower()+".h\"\n");
            break;
        case SET_PROJECT:
            break;
        }
    }
    context.append("\nint main(void){\n\n\twhile(1){\n\n\t}\n}\n");
    return writeString(config->getPath()+"/"+config->getName()+"/USER/main.c",context);
}

void FileProcess::createProject()
{
    QVector<TaskConfig> taskc=task->getFinishTask();
    QString source="template",module="module";
    QDir file(source);
    bool is=false;
    QString path=config->getPath();
    QString name=config->getName();
    QDir projects(path+"/"+name);
    static int status=0;
    if(projects.exists()){
        if(status==PROJECT_EXIST){
            projects.mkpath(projects.currentPath());
        }else{
            emit config->createOver(PROJECT_EXIST);
            status=PROJECT_EXIST;
            return;
        }
    }
    if(file.exists()){
        is = copyDir(source+"/file",path+"/"+name,true) &&
                copyFile(source+"/main.c",path+"/"+name+"/USER/main.c",name)&&
                copyFile(source+"/Template.uvguix.Moon",path+"/"+name+"/USER/"+name+".uvguix.Moon",name) &&
                copyFile(source+"/Template.map",path+"/"+name+"/USER/Listings/"+name+".map",name) &&
                copyFile(source+"/Template.uvoptx",path+"/"+name+"/USER/"+name+".uvoptx",name) &&
                copyUVprojx()&&copyMain();
        for(auto &item : taskc){
            switch (item.taskclass) {
            case ADD_HCPP:
                is=is&&writeString(path+"/"+name+"/HARDWARE/"+name+".c","#include \""+name+".h\"\n")&&
                        writeString(path+"/"+name+"/HARDWARE/"+name+".h","#ifndef __"+name.toUpper()
                                    +"_H\n#define __"+name.toUpper()+"_H\n#include \"delay.h\"\n#include \"sys.h\"\n#endif\n");
                break;
            case COPY_MODULE:
                is=is&& copyDir(module+"/"+item.name,path+"/"+name+"/HARDWARE",true);
                break;
            case SET_PROJECT:
                break;
            }
        }
        if(is){
            emit config->createOver(PROJECT_FINISH);
            return;
        }
    }
    emit config->createOver(PROJECT_ERROR);
}

bool FileProcess::openProject()
{
    QString path=task->getConfig()->getPath();
    QString name=task->getConfig()->getName();
    bool is_open=true;
#ifdef Q_OS_WIN32
    is_open = QDesktopServices::openUrl(QUrl("file:///"+path+"/"+name+"/USER/"+name+".uvprojx", QUrl::TolerantMode));
#else
    QString  cmd= QString("xdg-open ")+ "file:///"+path+"/"+name+"/USER/"+name+".uvprojx";
    system(cmd.toStdString().c_str());
#endif
    return is_open;
}

Task *FileProcess::getTask() const
{
    return task;
}

void FileProcess::setTask(Task *value)
{
    task = value;
    config=value->getConfig();
}

