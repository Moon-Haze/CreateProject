#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include <QObjectList>
#include <QObject>
#include <QThread>
#include "task.h"
#include "listdatamodel.h"
#include "fileprocess.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
    QApplication app(argc, argv);
    qmlRegisterType<ListDataModel> ("ListData",1,0,"ListDataModel");
    qmlRegisterType<ListDataList>("ListData",1,0,"ListDataList");
    app.setWindowIcon(QIcon(":/iamge/myapp.ico"));
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    Task *task=new Task();
    ListDataList *setList=task->getSetList();
    ListDataList *moduleList=task->getModuleList();
    Config *config=task->getConfig();

    QQmlContext *context=engine.rootContext();
    context->setContextProperty(QStringLiteral("setList"),setList);
    context->setContextProperty(QStringLiteral("moduleList"),moduleList);
    context->setContextProperty(QStringLiteral("config"),config);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

//    QObject* root = engine.rootObjects().first();
    QObject* root=engine.rootObjects().at(0);
    FileProcess *file=new FileProcess();
    file->setTask(task);

    QThread *thread=new QThread(&app);
    file->moveToThread(thread);
    thread->start();

    QObject::connect(root,SIGNAL(createProject()),file,SLOT(createProject()));
    QObject::connect(root,SIGNAL(openProject()),file,SLOT(openProject()));
    QObject::connect(&app,&QApplication::destroyed,[=]{
       thread->quit();
       thread->wait();
       delete file;
       delete task;
    });

    return app.exec();
}
