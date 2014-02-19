#include <QPixmap>
#include <QIcon>
#include <QFont>

#include "verse_node.h"
#include "verse_node_data_model.h"

VerseNodeDataTreeModel::VerseNodeDataTreeModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    this->node = NULL;
}

VerseNodeDataTreeModel::VerseNodeDataTreeModel(VerseNode *_node)
{
    if(_node != NULL) {
        this->node = _node;
    }
}

VerseNodeDataTreeModel::~VerseNodeDataTreeModel()
{
    this->node = NULL;
}

QModelIndex VerseNodeDataTreeModel::index(int row, int col, const QModelIndex &parent) const
{
    VerseData *parent_data, *child_data;

    if(!parent.isValid()) {
        parent_data = (VerseData*)this->node;
    } else {
        parent_data = static_cast<VerseData*>(parent.internalPointer());
    }

    child_data = parent_data->getChildData(row);
    if(child_data) {
        return createIndex(row, col, child_data);
    } else {
        return QModelIndex();
    }

    return QModelIndex();
}

QModelIndex VerseNodeDataTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return QModelIndex();
    } else {
        VerseData *child_data = static_cast<VerseData*>(index.internalPointer());

        if(child_data != NULL) {
            VerseData *parent_data = child_data->getParent();

            if(parent_data == (VerseData*)this->node) {
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

int VerseNodeDataTreeModel::rowCount(const QModelIndex &parent) const
{
    VerseData *parent_data;

    if(!parent.isValid()) {
        parent_data = (VerseData*)this->node;
    } else {
        parent_data = static_cast<VerseData*>(parent.internalPointer());
    }

    if(parent_data->getType() == VerseData::VERSE_NODE ||
            parent_data->getType() == VerseData::VERSE_TAG_GROUP)
    {
        return parent_data->childCount();
    } else {
        return 0;
    }
}

int VerseNodeDataTreeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

QVariant VerseNodeDataTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    } else {
        VerseData *data = static_cast<VerseData*>(index.internalPointer());
        VerseTagGroup *tg;
        VerseTag *tag;
        VerseLayer *layer;

        switch(role) {
        // Icons
        case Qt::DecorationRole:
            if(index.column() == 0) {
                QIcon icon;

                switch(data->getType()) {
                case VerseData::VERSE_TAG_GROUP:
                    tg = (VerseTagGroup*)data;
                    icon.addFile(":/images/icons/tag-group.png", QSize(16,16));

                    if(tg->isSubscribed()==true) {
                        return icon.pixmap(QSize(16,16), QIcon::Normal, QIcon::On);
                    } else {
                        return icon.pixmap(QSize(16,16), QIcon::Disabled, QIcon::Off);
                    }

                    break;
                case VerseData::VERSE_TAG:
                    icon.addFile(":/images/icons/emblem-system.png", QSize(16,16));
                    return icon.pixmap(QSize(16,16), QIcon::Normal, QIcon::On);
                case VerseData::VERSE_LAYER:
                    layer = (VerseLayer*)data;
                    icon.addFile(":/images/icons/tag-group.png", QSize(16,16));

                    if(layer->isSubscribed()==true) {
                        return icon.pixmap(QSize(16,16), QIcon::Normal, QIcon::On);
                    } else {
                        return icon.pixmap(QSize(16,16), QIcon::Disabled, QIcon::Off);
                    }
                    break;
                case VerseData::RESERVED:
                case VerseData::VERSE_NODE:
                    return QVariant();
                    break;
                default:
                    return QVariant();
                    break;
                }
            } else {
                return QVariant();
            }
            break;
        // Text
        case Qt::DisplayRole:
            switch(data->getType()) {
            case VerseData::RESERVED:
                break;
            case VerseData::VERSE_NODE:
                if(index.column()==0) {
                    return ((VerseNode*)data)->getID();
                } else {
                    return QVariant();
                }
                break;
            case VerseData::VERSE_TAG_GROUP:
                if(index.column()==0) {
                    return ((VerseTagGroup*)data)->getID();
                } else if(index.column()==1) {
                    return ((VerseTagGroup*)data)->getClientType();
                } else if(index.column()==2) {
                    return QVariant("TagGroup");
                } else {
                    return QVariant();
                }
                break;
            case VerseData::VERSE_TAG:
                if(index.column()==0) {
                    return ((VerseTag*)data)->getID();
                } else if(index.column()==1) {
                    return ((VerseTag*)data)->getClientType();
                } else if(index.column()==2) {
                    return VerseTag::TagType(((VerseTag*)data)->getDataType());
                }
                break;
            case VerseData::VERSE_LAYER:
                if(index.column()==0) {
                    return ((VerseLayer*)data)->getID();
                } else if(index.column()==1) {
                    return ((VerseLayer*)data)->getClientType();
                } else if(index.column()==2) {
                    return QVariant("Layer");
                } else {
                    return QVariant();
                }
                break;
            default:
                break;
            }
            break;
        case Qt::FontRole:
            switch(data->getType()) {
            case VerseData::VERSE_TAG_GROUP:
                tg = (VerseTagGroup*)data;
                if(tg->isSubscribed() == true) {
                    QFont boldFont;
                    boldFont.setBold(true);
                    return boldFont;
                }
                break;
            case VerseData::VERSE_TAG:
                tag = (VerseTag*)data;
                if(tag->isInitialized() == true) {
                    QFont boldFont;
                    boldFont.setBold(true);
                    return boldFont;
                }
                break;
            case VerseData::RESERVED:
            case VerseData::VERSE_NODE:
            case VerseData::VERSE_LAYER:
                return QVariant();
                break;
            default:
                return QVariant();
                break;
            }
            break;
        }
    }

    return QVariant();
}

Qt::ItemFlags VerseNodeDataTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    Qt::ItemFlags flags = 0;
    VerseData *data = static_cast<VerseData*>(index.internalPointer());
    VerseData *parent_data = data->getParent();

    if(parent_data != NULL) {
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    return flags;
}

QVariant VerseNodeDataTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QVariant("ID");
            case 1:
                return QVariant("Type");
            case 2:
                return QVariant("DataType");
            }
        }
    }

    return QVariant();
}

void VerseNodeDataTreeModel::update(void)
{
    // Update DataTreeView
    emit layoutChanged();
}

void VerseNodeDataTreeModel::destroyTag(VerseTag *tag)
{
    // DataTreeView will be changed
    emit layoutAboutToBeChanged();

    delete tag;

    // Update DataTreeView
    emit layoutChanged();
}

void VerseNodeDataTreeModel::destroyTagGroup(VerseTagGroup *tg)
{
    // DataTreeView will be changed
    emit layoutAboutToBeChanged();

    // Delete TagGroup itself
    delete tg;

    // Update DataTreeView
    emit layoutChanged();
}
