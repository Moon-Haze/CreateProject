#include "listdatamodel.h"

ListDataModel::ListDataModel(QObject *parent)
    : QAbstractListModel(parent),mlist(nullptr){}

int ListDataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid()|| !mlist)
        return 0;

    // FIXME: Implement me!
    return mlist->items().size();
}

QVariant ListDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()|| !mlist)
        return QVariant();

    const ListDataItem item=mlist->items().at(index.row());
    switch (role) {
    case ModuleRole:
        return QVariant(item.module);
    case SelectedRole:
        return QVariant(item.selected);
    }
    // FIXME: Implement me!
    return QVariant();
}

QHash<int, QByteArray> ListDataModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[ModuleRole]="module";
    names[SelectedRole]="selected";
    return names;
}

bool ListDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!mlist){
        return false;
    }
    ListDataItem item=mlist->items().at(index.row());
    switch (role) {
    case ModuleRole:
        item.module=value.toString();
        break;
    case SelectedRole:
        item.selected=value.toBool();
        break;
    }
    if (mlist->setItemAt(index.row(),item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ListDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable; // FIXME: Implement me!
}

ListDataList *ListDataModel::list() const
{
    return mlist;
}

void ListDataModel::setList(ListDataList *value)
{
    beginResetModel();
    if(mlist){
        mlist->disconnect(this);
    }
    mlist=value;
    if(mlist){
        connect(mlist,&ListDataList::preItemAppend,this,[=]{
            const int index=mlist->items().size();
            beginInsertRows(QModelIndex(),index,index);
        });
        connect(mlist,&ListDataList::postItemAppend,this,[=]{
            endInsertRows();
        });
        connect(mlist,&ListDataList::preItemRemove,this,[=](int index){
            beginRemoveRows(QModelIndex(),index,index);
        });

        connect(mlist,&ListDataList::postItemRemove,this,[=](){
            endRemoveRows();
        });
    }
    endResetModel();
}

void ListDataModel::append(QString item)
{
    if(!mlist){
        setList(new ListDataList);
    }
    mlist->append(item);
}

QString ListDataModel::getModule(int index) const
{
    return mlist->getModule(index);
}

bool ListDataModel::remove(int index)
{
    if(mlist){
        return mlist->remove(index);
    }
    return false;
}

QStringList ListDataModel::getModules() const
{
    if(!mlist){
        return QStringList{};
    }
    return mlist->getModules();
}
