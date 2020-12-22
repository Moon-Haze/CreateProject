#ifndef LISTDATALIST_H
#define LISTDATALIST_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include<QDebug>

struct ListDataItem{
    QString module;
    bool selected;
};

class ListDataList : public QObject
{
    Q_OBJECT
public:
    explicit ListDataList(QObject *parent = nullptr);

    ListDataList(bool readfile);
    QVector<ListDataItem> items() const;

    void readModules();

    bool setItemAt(int index,const ListDataItem &item);

    bool getSelected(const QString& module) const;
private:
    QVector<ListDataItem> mItems;
signals:
    void preItemAppend();
    void postItemAppend();
    void preItemRemove(int index);
    void postItemRemove();
public slots:
    void appendItem();
    void removeCompleteItems();
    void append(const QString& module,bool selected=false);
    bool remove(int index);
    QString getModule(int index) const;
    QStringList getModules() const;
};

#endif // LISTDATALIST_H
