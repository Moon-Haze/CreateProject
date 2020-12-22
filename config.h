#ifndef CONFIG_H
#define CONFIG_H
#include<QString>
#include<QStringList>
#include<QSettings>
#include<QObject>
#include<QDebug>

class Config:public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *object = nullptr);
    ~Config();

    Q_INVOKABLE QString getPath() const;

    Q_INVOKABLE QStringList getPaths() const;

    Q_INVOKABLE bool getChecked() const;

    Q_INVOKABLE QString getName() const;

public slots:
    void setChecked(bool value);
    void setPath(QString value);
    void setPaths(QStringList value);
    void addPath(QString value);
    void setName(QString value);
signals:
    void createOver(int ques);
private:
    QString name;
    QString path;

    QStringList paths;
    bool checked;
    QSettings settings{"Swx","CreateProject"};
};

#endif // CONFIG_H
