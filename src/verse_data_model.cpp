#include <QBrush>

#include <verse.h>

#include "verse_data_model.h"
#include "verse_tag.h"
#include "verse_taggroup.h"
#include "verse_node.h"

VerseDataModel::VerseDataModel(VerseData *_data)
{
    this->verse_data = _data;
}

QModelIndex VerseDataModel::index(int row, int col, const QModelIndex &parent) const
{
    VerseData *parent_data, *child_data;

    if(!parent.isValid()) {
        parent_data = (VerseData*)this->verse_data;
    } else {
        parent_data = static_cast<VerseData*>(parent.internalPointer());
    }

    child_data = parent_data->getChildData(row);
    if(child_data) {
        QModelIndex index = createIndex(row, col, child_data);
        child_data->setModelIndex(index);
        return index;
    } else {
        return QModelIndex();
    }

    return QModelIndex();
}

QModelIndex VerseDataModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return QModelIndex();
    } else {
        VerseData *child_data = static_cast<VerseData*>(index.internalPointer());

        if(child_data != NULL) {
            VerseData *parent_data = child_data->getParent();

            if(parent_data == (VerseData*)this->verse_data) {
                return QModelIndex();
            } else {
                if(parent_data != NULL) {
                    return createIndex(parent_data->getRow(), 0, parent_data);
                } else {
                    return QModelIndex();
                }
            }

        } else {
            return QModelIndex();
        }
    }

    return QModelIndex();
}

int VerseDataModel::rowCount(const QModelIndex &parent) const
{
    VerseData *parent_data;

    if(!parent.isValid()) {
        parent_data = (VerseData*)this->verse_data;
    } else {
        parent_data = static_cast<VerseData*>(parent.internalPointer());
    }

    if(parent_data->getType() == VerseData::VERSE_TAG) {
        return parent_data->childCount();
    } else {
        return 0;
    }
}

int VerseDataModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant VerseDataModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    } else {
        VerseData *data = static_cast<VerseData*>(index.internalPointer());
        VerseTagValue *tag_value;
        VerseTag *tag;

        switch(role) {
        case Qt::EditRole:
        case Qt::DisplayRole:

            switch(data->getType()) {
            case VerseData::VERSE_TAG_VALUE:
                tag_value = (VerseTagValue*)data;
                if(index.column()==0) {
                    return tag_value->getIndex();
                } else if(index.column()==1){
                    return tag_value->getValue();
                } else {
                    return QVariant();
                }
                break;
            default:
                return QVariant();
                break;
            }

            break;
        case Qt::ForegroundRole:
            switch(data->getType()) {
            case VerseData::VERSE_TAG_VALUE:
                tag_value = (VerseTagValue*)data;
                tag = tag_value->tag;

                if(tag->isInitialized() == false) {
                    QBrush fontColor(Qt::gray);
                    return fontColor;
                } else {
                    QBrush fontColor(Qt::black);
                    return fontColor;
                }
            default:
                return QVariant();
                break;
            }
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

Qt::ItemFlags VerseDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    VerseData *data = static_cast<VerseData*>(index.internalPointer());
    Qt::ItemFlags flags = 0;

    if(data->getType() == VerseData::VERSE_TAG_VALUE) {
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if(index.column() == 1) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

QVariant VerseDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QVariant("Index");
            case 1:
                return QVariant("Value");
            }
        }
    }

    return QVariant();
}

bool VerseDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if(index.column() == 1) {
            VerseData *data = static_cast<VerseData*>(index.internalPointer());
            VerseTagValue *tag_value;
            VerseTag *tag;

            switch(data->getType()) {
            case VerseData::VERSE_TAG_VALUE:
                tag_value = (VerseTagValue*)data;
                tag = tag_value->tag;

                tag->sendValue(this->session_id, tag_value, value);

                break;
            default:
                return false;
                break;
            }
        }
    }
    return true;
}

void VerseDataModel::update()
{
    emit layoutChanged();
}
