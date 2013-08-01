#ifndef VERSEMODEL_H
#define VERSEMODEL_H

#include <QLinkedList>
#include <QAbstractTableModel>

#include <verse.h>

#include "verse_node.h"

/**
  * \brief This class is model for tree of verse nodes
  */
class VerseNodeTreeModel:public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
      * \brief The constructor
      */
    VerseNodeTreeModel(QObject *parent);
    /**
      * \brief The destructor
      */
    ~VerseNodeTreeModel();

    /**
      * \brief This method return index of item
      */
    QModelIndex index(int row, int col, const QModelIndex & parent = QModelIndex() ) const;

    /**
      * \brief This method return index for parent item
      */
    QModelIndex parent(const QModelIndex &index) const;

    /**
      * \brief This method return number of rows for item
      */
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    /**
      * \brief This method returns number of columns
      */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
      * \brief This method return data for current index and role
      */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
      * \brief This method specify, if current index could be selected, modified
      * draged, droped, etc
      */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
      * \brief This method sets data directly
      *
      * This model enable to set directly only priorities fo nodes
      */
    bool setData(const QModelIndex & index, const QVariant & value, int role);

    /**
      * \brief This method set strings in header of QTreeView
      */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
      * \brief This method returns list of supported drop actions
      *
      * Only move action is enabled in this model
      */
    Qt::DropActions supportedDropActions() const;

    /**
      * \brief This method is called, when selected nodes are draged
      *
      * IDs of selected nodes are serialized to data stream
      */
    QMimeData* mimeData(const QModelIndexList &indexes) const;

    /**
      * \brief This method is called, when selected nodes are droped
      *
      * The stream (*data) is converted to list of node IDs
      */
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);

    /**
      * \brief This method returns list of supported mime types
      */
    QStringList mimeTypes() const;

    /**
      * \brief This method remove node from model
      */
    void delNode(VerseNode *node);

    /**
      * \brief This method add new Node to the model
      */
    void addNode(void);

    /**
      * \ brief This method change parent
      */
    void changeParent(VerseNode *node, VerseNode *parent_node);

    /**
      * \brief This method get mode from the model
      */
    VerseNode* getNode(uint32_t id);

    /**
      * \brief This method return pointer at fake root node
      */
    VerseNode* getFakeRootNode(void) {return fake_root_node; }

    /**
      * \brie This method subscribe to node
      */
    void subscribe(VerseNode *node);

    /**
      * \brief Unsubscribe from node. The child nodes should be deleted in model.
      */
    void unsubscribe(VerseNode *node);

    /**
      * \brief This method is called from callback function of node_perm
      */
    void update_view(VerseNode *node);

private:
    /**
      * \brief The pointer at root node
      */
    VerseNode *fake_root_node;

    /**
      * \brief The session ID
      */
    uint8_t session_id;

    /**
      * \brief The avatar ID
      */
    uint32_t avatar_id;

    /**
      * \brief The user ID
      */
    uint16_t user_id;
};

#endif // VERSEMODEL_H
