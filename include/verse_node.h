#ifndef VERSENODE_H
#define VERSENODE_H

#include <QDialog>
#include <QVariant>

#include <verse.h>

#include "verse_data.h"
#include "verse_node_data_model.h"
#include "verse_taggroup.h"
#include "verse_tag.h"

/**
  * \brief The class for node permission of one user
  */
class VerseNodePerm
{
private:
    uint16_t      user_id;
    uint8_t       permission;
public:
    VerseNodePerm(uint16_t user_id, uint8_t perm) { this->user_id = user_id; this->permission = perm; }
    ~VerseNodePerm() {}
    uint16_t getUserID(void) { return this->user_id; }
    uint8_t getPerm(void) { return this->permission; }
    void setPerm(uint8_t perm) { this->permission = perm; }
};

enum ReadWrite {
    NODE_PERM_UNKNOWN = 0,
    NODE_PERM_UNREADABLE = 1,
    NODE_PERM_READONLY = 2,
    NODE_PERM_READWRITE = 3,
    NODE_PERM_LOCKED = 4
};

class VerseNode: public VerseData
{
private:
    /**
      * \brief The priority of this node (client specific property)
      */
    uint8_t             priority;

    /**
      * \brief The pointer at parent node.
      *
      * The fake root node doesn't have any parent. This pointer is null then.
      */
    struct VerseNode    *parentNode;

    /**
      * \brief The unique ID of node.
      */
    uint32_t            id;

    /**
      * \brief The ID of owner
      */
    uint16_t            owner;

    /**
      * \brief The ID of locker
      */
    uint32_t            locker;

    /**
      * \brief The client defined type of node
      */
    uint16_t            client_type;

    /**
      * \brief This flag store information about subscription of the node.
      */
    bool                subscribed;

    /**
      * \brief The list of child nodes
      */
    QList<VerseNode*>   childNodes;

    /**
      * \brief The list of node perimissions
      */
    QList<VerseNodePerm*>   permissions;

    /**
      * \brief The last tg ID
      */
    uint16_t last_tg_id;

    /**
      * \brief The model of data stored in this node
      */
    VerseNodeDataTreeModel  *data_model;

    /**
     * \brief The list of pointers at open dialogs
     */
    QList<QDialog*>     dialogs;

public:
    /**
      * \brief
      */
    bool    can_read;

    /**
      * \brief
      */
    bool    can_write;

    /**
      * \brief The pointer at map of all verse nodes
      *
      * This is created/destroyed, when fake root node is created/destroyed.
      */
    std::map<uint32_t, VerseNode*> *nodes;

    /**
      * \brief The map of TagGroups
      */
    std::map<uint16_t, VerseTagGroup*> taggroups;

    /**
      * \brief Constructor of Verse node
      */
    VerseNode(VerseNode *parent_node,
              uint32_t node_id,
              uint16_t user_id,
              uint16_t _type);
    /**
      * \brief Destructor of Verse node
      */
    ~VerseNode();

    /**
      * \brief Add child to the node
      */
    void addChild(VerseNode *child_node);

    /**
      * \brief This method return n-th child
      */
    VerseNode *getChildNode(int row);

    /**
      * \brief This method returns number of children
      */
    int childNodeCount() const;

    /**
      * \brief This method returns number of column
      */
    int columnCount() const;

    /**
      * \brief This method retuns data stored in this node
      */
    QVariant data(int column) const;

    /**
      * \brief This method returns row of parent's branche
      */
    int row() const;

    /**
      * \brief This method returns pointer at parent
      */
    VerseNode *getNodeParent();

    /**
      * \brief This is getter for id
      */
    uint32_t getID(void) { return this->id; }

    /**
      * \brief This method change parent of node
      */
    void setNodeParent(VerseNode *parent_node);

    /**
      * \brief Setter fo subscribed
      */
    void setSubscribed(bool s) {this->subscribed = s;}

    /**
      * \brief Getter for subscribed
      */
    bool getSubscribed(void) {return this->subscribed;}

    /**
      * \brief Getter for owner
      */
    uint16_t getOwner(void) {return this->owner;}

    /**
      * \brief Getter for child nodes
      */
    QList<VerseNode*>* getChildNodes(void) {return &(this->childNodes);}

    /**
      * \brief Getter for permissions
      */
    QList<VerseNodePerm*>* getPermissions(void) {return &(this->permissions);}

    /**
      * \brief Setter for user permission
      */
    void setPermissions(uint16_t user_id, uint8_t perm);

    /**
      * \brief Setter for owner
      */
    void setOwner(uint16_t user_id) { this->owner = user_id; }

    /**
      * \brief Getter for node priority
      */
    uint8_t getPrio() { return this->priority;}

    /**
      * \brief Setter for node priority
      */
    void setPrio(uint8_t prio);

    /**
      * \brief This method set if client can read this node
      */
    void setCanRead(uint16_t user_id);

    /**
      * \brief This method se if client can write to this node
      */
    void setCanWrite(uint16_t user_id, uint32_t avatar_id);

    /**
      * \brief Getter of locker
      */
    uint32_t getLocker() { return this->locker; }

    /**
      * \brief Setter of locker
      */
    void setLocker(uint32_t l) { this->locker = l; }

    /**
      * \brief Getter for data model of node
      */
    VerseNodeDataTreeModel* getDataModel(void) { return this->data_model; }

    /**
      * \brief This method removes all data in node
      */
    void removeNodeData(void);

    /**
      * \brief This method adds new taggroup to the node
      */
    void addTagGroup(VerseTagGroup *tag_group);

    /**
      * \brief This method removes existing taggroup from the node
      */
    void removeTagGroup(VerseTagGroup *tag_group);

    /**
      * \brief This method try to find TagGroup in the node
      */
    VerseTagGroup* getTagGroup(uint16_t tag_group_id);

    /**
      * \brief The getter for last_tg_id
      */
    uint16_t getLastTgID(void) { return this->last_tg_id; }

    /**
      * \brief The setter for last_tg_id
      */
    void setLastTgID(uint16_t last_id) { this->last_tg_id = last_id; }

    /**
     * \brief addDialog
     * \param dialog
     */
    void addDialog(QDialog *dialog);

    /**
     * @brief remDialog
     * @param dialog
     */
    void remDialog(QDialog *dialog);

    /**
     * @brief remAllDialogs
     */
    void remAllDialogs(void);
};

#endif // VERSENODE_H
