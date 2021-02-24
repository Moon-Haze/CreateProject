//
// Created by Moon on 2021/2/1.
//

#ifndef PROJECTCONFIG_H
#define PROJECTCONFIG_H

#include <QObject>
#include <QString>
#include <QStringList>

class ProjectConfig : public QObject {
Q_OBJECT
public:
    explicit ProjectConfig(QObject* object = nullptr);

    explicit ProjectConfig(QString  value);

    void addFile(const QString &string, bool isOpen= true);

    void removeFile(const QString &string);

    QString mainToQString() const;

    QString uvprojxToQString(const QString &source) const;

    QString uvoptxToQString(const QString &source) const;

    QString moonToQString(const QString &source) const;

private:
    QString name;
public:
    const QString &getName() const;

    void setName(const QString &name);

private:
    QStringList openFiles;
    QStringList files;

    static QString readFile(const QString &source);
};


#endif //PROJECTCONFIG_H
