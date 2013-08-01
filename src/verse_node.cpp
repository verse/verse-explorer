#include <QLinkedList>

#include "verse_node.h"

#include <stdio.h>

VerseNode::VerseNode(VerseNode *parent, uint32_t node_id, uint16_t user_id, uint16_t _type)
{
    qDebug("New Node: parent: %p, id: %d, owner: %d\n", parent, node_id, user_id);

    this->id = node_id;
    this->owner = user_id;
    this->subscribed = false;
    this->can_read = false;
    this->can_write = false;
    this->priority = VRS_DEFAULT_PRIORITY;
    this->locker = -1;
    this->type = VERSE_NODE;
    this->last_tg_id = 0;
    this->client_type = _type;

    printf("client_type: %d\n", _type);

    if(parent != NULL) {
        parent->addChild(this);
        this->nodes = parent->nodes;
    } else {
        this->parentNode = NULL;
        // Create map of nodes for fake root node
        this->nodes = new std::map<uint32_t, VerseNode*>;
    }

    this->data_model = new VerseNodeDataTreeModel(this);

    std::map<uint32_t, VerseNode*> &map = *(this->nodes);
    map[node_id] = (VerseNode*)this;
}

VerseNode::~VerseNode()
{
    /* Remove child nodes */
    qDeleteAll(this->childNodes);
    this->childNodes.clear();

    /* Remove list of permissions */
    qDeleteAll(this->permissions);
    this->permissions.clear();

    // Remove node from map of nodes
    std::map<uint32_t, VerseNode*>::iterator node_iter;
    if(this->nodes->count(this->id)==1) {
        node_iter = this->nodes->find(this->id);
        this->nodes->erase(node_iter);
    }

    // Clear map of taggroups
    this->taggroups.clear();

    // Clear all data
    this->removeData();

    // Delete data model (tag groups, tags and layers)
    delete this->data_model;

    VerseNode *parent_node = this->getNodeParent();
    if(parent_node != NULL) {
        parent_node->childNodes.removeAt(this->row());
    } else {
        // When this is fake root node, then delete map of all nodes
        delete this->nodes;
    }

    this->parentNode = NULL;
}

void VerseNode::addChild(VerseNode *child)
{
    child->parentNode = this;
    this->childNodes.append(child);
}

VerseNode* VerseNode::getChildNode(int row)
{
    return this->childNodes.value(row);
}

int VerseNode::childNodeCount() const
{
    return this->childNodes.count();
}

int VerseNode::columnCount() const
{
    return 5;
}

int VerseNode::row() const
{
    if(this->parentNode != NULL) {
        return this->parentNode->childNodes.indexOf(const_cast<VerseNode*>(this));
    }

    return 0;
}

QVariant VerseNode::data(int column) const
{
    if(column == 0) {
        return QVariant(this->id);
    } else if(column == 1) {
        return QVariant();
    } else if(column == 2) {
        return QVariant(this->owner);
    } else if(column == 3) {
        return QVariant(this->priority);
    } else if(column == 4) {
        return QVariant(this->client_type);
    }

    return QVariant();
}

VerseNode* VerseNode::getNodeParent()
{
    return this->parentNode;
}

void VerseNode::setNodeParent(VerseNode *parent_node)
{
    VerseNode *old_parent_node = this->getNodeParent();

    if(old_parent_node != NULL) {
        old_parent_node->childNodes.removeAt(this->row());
    }

    parent_node->addChild(this);
}

void VerseNode::setPermissions(uint16_t user_id, uint8_t permission)
{
    VerseNodePerm *perm;
    int perm_changed = 0;

    foreach(perm, this->permissions) {
        if(perm->getUserID() == user_id) {
            perm->setPerm(permission);
            perm_changed = 1;
            break;
        }
    }

    if(perm_changed == 0) {
        perm = new VerseNodePerm(user_id, permission);
        this->permissions.append(perm);
    }
}

void VerseNode::setCanRead(uint16_t user_id)
{
    VerseNodePerm *perm;

    // Owner of node can ver write to the node
    if(this->owner == user_id) {
        this->can_read = true;
        return;
    }

    foreach(perm, this->permissions) {
        if(perm->getUserID() == user_id || perm->getUserID() == 65535) {
            if( perm->getPerm() & VRS_PERM_NODE_READ ) {
                this->can_read = true;
                return;
            }
        }
    }

    this->can_read = false;

    return;
}

void VerseNode::setCanWrite(uint16_t user_id, uint32_t avatar_id)
{
    VerseNodePerm *perm;

    // Is node locked by some other client?
    if(this->locker != (uint32_t)-1 && this->locker != avatar_id) {
        this->can_write = false;
        return;
    }

    // Owner of node can ver write to the node
    if(this->owner == user_id) {
        this->can_write = true;
        return;
    }

    foreach(perm, this->permissions) {
        if(perm->getUserID() == user_id || perm->getUserID() == 65535) {
            if( perm->getPerm() & VRS_PERM_NODE_WRITE ) {
                this->can_write = true;
                return;
            }
        }
    }

    this->can_write = false;

    return;
}

void VerseNode::setPrio(uint8_t prio)
{
    VerseNode *child_node;
    this->priority = prio;

    foreach(child_node, this->childNodes) {
        child_node->setPrio(prio);
    }
}

void VerseNode::removeNodeData(void)
{
    this->taggroups.clear();
    this->removeData();
    this->getDataModel()->update();
}

void VerseNode::addTagGroup(VerseTagGroup *tag_group)
{
    this->taggroups[tag_group->getID()] = tag_group;
}

void VerseNode::removeTagGroup(VerseTagGroup *tag_group)
{
    std::map<uint16_t, VerseTagGroup*>::iterator tg_iter;

    tg_iter = this->taggroups.find(tag_group->getID());

    if(tg_iter != this->taggroups.end()) {
        this->taggroups.erase(tg_iter);
    }
}

VerseTagGroup* VerseNode::getTagGroup(uint16_t tag_group_id)
{
    std::map<uint16_t, VerseTagGroup*>::iterator tg_iter;

    tg_iter = this->taggroups.find(tag_group_id);

    if(tg_iter != this->taggroups.end()) {
        return tg_iter->second;
    } else {
        return NULL;
    }
}

void VerseNode::addDialog(QDialog *dialog)
{
    this->dialogs.append(dialog);
}

void VerseNode::remDialog(QDialog *dialog)
{
    int pos = this->dialogs.indexOf(dialog);
    this->dialogs.removeAt(pos);
}

void VerseNode::remAllDialogs()
{
    QList<QDialog*>::iterator iter;
    QDialog *dialog;

    for(iter = this->dialogs.begin();
        iter != this->dialogs.end();
        ++iter)
    {
        dialog = *iter;
        dialog->close();
    }

    qDeleteAll(this->dialogs.begin(), this->dialogs.end());
}
