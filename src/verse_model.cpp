#include <QPixmap>
#include <QFont>

#include "verse_client.h"

#include "verse_model.h"

VerseNodeTreeModel::VerseNodeTreeModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    qDebug("Creating fake root node\n");

    this->fake_root_node = new VerseNode(NULL, -1, -1, 0);
    this->fake_root_node->can_read = false;
    this->fake_root_node->can_write = false;

    this->session_id = ((VerseClient*)parent)->getSessionID();

    this->avatar_id = ((VerseClient*)parent)->getAvatarID();

    this->user_id = ((VerseClient*)parent)->getUserID();
}

VerseNodeTreeModel::~VerseNodeTreeModel()
{
    delete fake_root_node;
}

QModelIndex VerseNodeTreeModel::index(int row, int col, const QModelIndex &parent) const
{
    VerseNode *parent_node, *child_node;

    if(!parent.isValid()) {
        parent_node = this->fake_root_node;
    } else {
        parent_node = static_cast<VerseNode*>(parent.internalPointer());
    }

    child_node = parent_node->getChildNode(row);
    if(child_node) {
        QModelIndex _model_index = createIndex(row, col, child_node);
        child_node->setModelIndex(_model_index);
        return _model_index;
    } else {
        return QModelIndex();
    }

    return QModelIndex();
}

QModelIndex VerseNodeTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return QModelIndex();
    } else {
        VerseNode *child_node = static_cast<VerseNode*>(index.internalPointer());

        if(child_node != NULL) {
            VerseNode *parent_node = child_node->getNodeParent();

            if(parent_node == this->fake_root_node) {
                return QModelIndex();
            } else {
                if(parent_node != NULL) {
                    return createIndex(parent_node->row(), 0, parent_node);
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

int VerseNodeTreeModel::rowCount(const QModelIndex &parent) const
{
    VerseNode *parent_node;

    if(!parent.isValid()) {
        parent_node = this->fake_root_node;
    } else {
        parent_node = static_cast<VerseNode*>(parent.internalPointer());
    }

    return parent_node->childNodeCount();
}

int VerseNodeTreeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) {
        VerseNode *node = static_cast<VerseNode*>(parent.internalPointer());
        return node->columnCount();
    } else {
        return this->fake_root_node->columnCount();
    }
}

QVariant VerseNodeTreeModel::data(const QModelIndex &index, int role) const
{
    QPixmap icon;

    if(!index.isValid()) {
        return QVariant();
    } else {
        VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
        switch(role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            if(index.column()==2) {
                if(node->getOwner()==100) {
                    return QString("Server");
                } else if(node->getOwner()==this->user_id) {
                    return QString("Me");
                } else {
                    return node->data(index.column());
                }
            } else {
                return node->data(index.column());
            }
            break;
        case Qt::DecorationRole:
            if(index.column() == 0) {

                switch(node->getID()) {
                case 0:
                    icon.load(":/images/icons/folder-visiting.png", 0, Qt::AutoColor);
                    break;
                case 1:
                    if(node->getSubscribed() == true) {
                        icon.load(":/images/icons/user-home.png", 0, Qt::AutoColor);
                    } else {
                        icon.load(":/images/icons/user-home_unsubscribed.png", 0, Qt::AutoColor);
                    }
                    break;
                case 2:
                    if(node->getSubscribed() == true) {
                        icon.load(":/images/icons/system-users.png", 0, Qt::AutoColor);
                    } else {
                        icon.load(":/images/icons/system-users_unsubscribed.png", 0, Qt::AutoColor);
                    }
                    break;
                case 100:
                    if(node->getSubscribed() == true) {
                        icon.load(":/images/icons/network-server.png", 0, Qt::AutoColor);
                    } else {
                        icon.load(":/images/icons/network-server_unsubscribed.png", 0, Qt::AutoColor);
                    }
                    break;
                default:
                    if(node->getID() == (uint32_t)this->avatar_id) {
                        if(node->getSubscribed() == true) {
                            icon.load(":/images/icons/user-desktop.png", 0, Qt::AutoColor);
                        } else {
                            icon.load(":/images/icons/user-desktop_unsubscribed.png", 0, Qt::AutoColor);
                        }
                    } else if(node->getID() == (uint32_t)this->user_id) {
                        if(node->getSubscribed() == true) {
                            icon.load(":/images/icons/face-monkey.png", 0, Qt::AutoColor);
                        } else {
                            icon.load(":/images/icons/face-monkey_unsubscribed.png", 0, Qt::AutoColor);
                        }
                    } else if(node->getNodeParent() != NULL && node->getNodeParent()->getID() == 2) {
                        if(node->getSubscribed() == true) {
                            icon.load(":/images/icons/face-smile.png", 0, Qt::AutoColor);
                        } else {
                            icon.load(":/images/icons/face-smile_unsubscribed.png", 0, Qt::AutoColor);
                        }
                    } else if(node->getSubscribed() == true) {
                        icon.load(":/images/icons/folder.png", 0, Qt::AutoColor);
                    } else {
                        icon.load(":/images/icons/folder_unsubscribed.png", 0, Qt::AutoColor);
                    }
                    break;
                }
                return icon;
            } else if(index.column() == 1) {
                if(node->getSubscribed() == true) {
                    if(node->can_read == true && node->can_write == true) {
                        if(node->getLocker() == this->avatar_id) {
                            icon.load(":/images/icons/emblem-locked-me.png", 0, Qt::AutoColor);
                        } else {
                            icon.load(":/images/icons/emblem-default.png", 0, Qt::AutoColor);
                        }
                    } else if((node->can_read == true) && (node->can_write == false)) {
                        if(node->getLocker() != (uint32_t)-1) {
                            icon.load(":/images/icons/emblem-locked.png", 0, Qt::AutoColor);
                        } else {
                            icon.load(":/images/icons/emblem-readonly.png", 0, Qt::AutoColor);
                        }
                    } else if((node->can_read == false) && (node->can_write == false)) {
                        icon.load(":/images/icons/emblem-unreadable.png", 0, Qt::AutoColor);
                    }
                } else {
                    if(node->getOwner()==this->user_id) {
                        icon.load(":/images/icons/emblem-default.png", 0, Qt::AutoColor);
                    } else {
                        icon.load(":/images/icons/emblem-unknown.png", 0, Qt::AutoColor);
                    }
                }
                return icon;
            } else {
                return QVariant();
            }
            break;
        case Qt::FontRole:
            if(node->getSubscribed() == true) {
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
            break;
        default:
            return QVariant();
            break;
        }
    }

    return QVariant();
}

Qt::ItemFlags VerseNodeTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    VerseNode *node = static_cast<VerseNode*>(index.internalPointer());
    VerseNode *parent_node = node->getNodeParent();
    Qt::ItemFlags flags = 0;

    if(parent_node != NULL) {
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    // Can be node parent of draged node?
    if(node->getOwner() == this->user_id || node->can_write == true) {
        flags = flags | Qt::ItemIsDropEnabled;
    }

    // Can this node be draged?
    if(parent_node != NULL) {
        if ((parent_node->getOwner() == this->user_id || parent_node->can_write == true) &&
                (node->getOwner() == this->user_id || node->can_write == true))
        {
            flags = flags | Qt::ItemIsDragEnabled;
        }
    }

    // It is possible to edit only node priority
    if(index.column() == 3 && node->getSubscribed()==true) {
        flags = flags | Qt::ItemIsEditable;
    }

    return flags;
}

bool VerseNodeTreeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        if(index.column() == 3) {

            VerseNode *node = static_cast<VerseNode*>(index.internalPointer());

            int prio = value.toInt();

            // Limit max and min value
            if(prio>255) {
                prio = 255;
            }
            if(prio<0) {
                prio = 0;
            }

            // Save value from editor to priority of node
            node->setPrio(prio);

            // Send this priority to the server
            vrs_send_node_prio(this->session_id, node->getPrio(), node->getID(), node->getPrio());
        }
    }
    return true;
}

QVariant VerseNodeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QVariant("ID");
            case 1:
                return QVariant("R/W");
            case 2:
                return QVariant("Owner");
            case 3:
                return QVariant("Prio");
            case 4:
                return QVariant("Type");
            }
        }
    }
    return QVariant();
}

// List of supported drop actions
Qt::DropActions VerseNodeTreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

// This method is called, when data items are draged (items are serialized to data stream)
QMimeData *VerseNodeTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            if(index.column()==0) {
                uint32_t node_id = this->data(index, Qt::DisplayRole).toInt();
                stream << node_id;
            }
        }
    }

    mimeData->setData("text/plain", encodedData);
    return mimeData;
}

// This method is called, when selection is droped (stream is converted to list of items)
bool VerseNodeTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                  int row, int column, const QModelIndex &parent)
{
    VerseNode *parent_node = static_cast<VerseNode*>(parent.internalPointer());

    qDebug("dropMimeData: row: %d, column: %d, parent node: %d\n", row, column, parent_node->getID());

    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("text/plain"))
        return false;

    QByteArray encodedData = data->data("text/plain");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    uint32_t child_node_id;
    while (!stream.atEnd()) {
        stream >> child_node_id;
        qDebug("Dropping node: %d\n", child_node_id);
        vrs_send_node_link(this->session_id, parent_node->getPrio(), parent_node->getID(), child_node_id);
    }

    return true;
}

// This method returns list of supported mime types
QStringList VerseNodeTreeModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

void VerseNodeTreeModel::delNode(VerseNode *node)
{
    emit layoutAboutToBeChanged();

//    this->beginRemoveRows(node->getParent()->getModelIndex(), node->row(), node->row());

    if(this->fake_root_node == node) {
        this->fake_root_node = NULL;
    }

//    this->removeRow(node->row(), node->getParent()->getModelIndex());

    // Delete node itself
    delete node;

//    this->endRemoveRows();

    // Update TreeView
    emit layoutChanged();
}

void VerseNodeTreeModel::addNode(void)
{
    // Update TreeView
    emit layoutChanged();
}

void VerseNodeTreeModel::changeParent(VerseNode *node, VerseNode *parent_node)
{
    node->setNodeParent(parent_node);

    // Update TreeView
    emit layoutChanged();
}

VerseNode* VerseNodeTreeModel::getNode(uint32_t id)
{
    std::map<uint32_t, VerseNode*>::iterator node;

    if(this->fake_root_node->nodes->count(id)==1) {
        node = this->fake_root_node->nodes->find(id);

        return node->second;
    } else {
        return NULL;
    }
}

void VerseNodeTreeModel::subscribe(VerseNode *node)
{
    if(node->getSubscribed() == false) {
        vrs_send_node_subscribe(this->session_id, node->getPrio(), node->getID(), 0, 0);
        node->setSubscribed(true);
    }
}

void VerseNodeTreeModel::unsubscribe(VerseNode *node)
{
    if(node->getSubscribed() == true) {
        vrs_send_node_unsubscribe(this->session_id, node->getPrio(), node->getID(), 0);
        node->setSubscribed(false);

        foreach(VerseNode *child_node, *node->getChildNodes()) {
            delete child_node;
        }

        emit layoutChanged();
    }
}

void VerseNodeTreeModel::update_view(VerseNode *node)
{
    VerseNode *parent_node = node->getNodeParent();
    int rows = parent_node->childNodeCount();

    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomRight = createIndex(rows,node->columnCount());

    emit dataChanged(topLeft, bottomRight);
}
