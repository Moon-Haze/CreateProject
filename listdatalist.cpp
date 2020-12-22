#include "listdatalist.h"

ListDataList::ListDataList(QObject *parent) : QObject(parent){}

QVector<ListDataItem> ListDataList::items() const
{
    return mItems;
}

bool ListDataList::setItemAt(int index, const ListDataItem &item)
{
    if(index < 0 || index >=mItems.size())
        return false;
    const ListDataItem &oldItem=mItems.at(index);
    if(item.module ==oldItem.module&&item.selected==oldItem.selected)
        return false;
    mItems[index]=item;
    return true;
}

bool ListDataList::getSelected(const QString &module) const
{
    for(auto item :mItems){
        if(item.module==module){
            return item.selected;
        }
    }
    return false;
}

void ListDataList::append(const QString &module,bool selected)
{
    emit preItemAppend();
    mItems.append(ListDataItem {module,selected});
    emit postItemAppend();
}

bool ListDataList::remove(int index)
{
    if(index<0 || index>=mItems.length())
        return false;
    emit preItemRemove(index);
    mItems.removeAt(index);
    emit postItemRemove();
    return true;
}

QString ListDataList::getModule(int index) const
{
    return mItems.at(index).module;
}

QStringList ListDataList::getModules() const
{
    QStringList list;
    for(auto item:mItems){
        list<<item.module;
    }
    return list;
}

void ListDataList::appendItem()
{
    emit preItemAppend();
    ListDataItem item;
    item.module="";
    item.selected=false;
    mItems.append(item);
    emit postItemAppend();
}

void ListDataList::removeCompleteItems()
{
    for(int i=0; i<mItems.length();i++){
        emit preItemRemove(i);
        mItems.removeAt(i);
        emit postItemRemove();
    }
}
