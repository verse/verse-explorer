#ifndef VERSE_NODE_DATA_MODEL_H
#define VERSE_NODE_DATA_MODEL_H

#include <QLinkedList>
#include <QAbstractTableModel>

#include <verse.h>

#include "verse_data.h"
#include "verse_taggroup.h"
#include "verse_tag.h"

class VerseNode;

/**
  * \brief This class is model for TagGroups, Tags and Layers
  */
class VerseNodeDataTreeModel:public QAbstractTableModel
{
    Q_OBJECT
public:

    /**
      * \brief The constructor
      */
    VerseNodeDataTreeModel(QObject *parent);

    /**
      * \brief The constructor
      */
    VerseNodeDataTreeModel(VerseNode *_node);

    /**
      * \brief The destructor
      */
    ~VerseNodeDataTreeModel();

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
      * \brief  This method set strings in header of QTreeView
      */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
      * \brief This method should be called, when new tag group was created
      */
    void update(void);

    /**
      * \brief This method should be called, when tag group was destroyed
      */
    void destroyTagGroup(VerseTagGroup *tg);

    /**
      * \brief This method should be called, when tag was destroyed
      */
    void destroyTag(VerseTag *tag);

private:

    /**
      * \brief The pointer at node containing data
      */
    VerseNode *node;
};

#endif // VERSE_NODE_DATA_MODEL_H
