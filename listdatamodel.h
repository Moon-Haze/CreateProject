#ifndef LISTDATAMODEL_H
#define LISTDATAMODEL_H

#include<QDebug>
#include <QAbstractListModel>
#include "listdatalist.h"

class ListDataModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ListDataList *list READ list WRITE setList)

public:
    explicit ListDataModel(QObject *parent = nullptr);

    enum {
        ModuleRole=Qt::UserRole,
        SelectedRole
    };
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int,QByteArray> roleNames() const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    ListDataList *list() const;

    void setList(ListDataList *value);
public slots:
    void append(QString item);

    QString getModule(int index) const;
    bool remove(int index);
    QStringList getModules() const;
private:
    ListDataList *mlist;
};

#endif // LISTDATAMODEL_H
