#ifndef VERSE_DATA_MODEL_H
#define VERSE_DATA_MODEL_H

#include <QAbstractTableModel>
#include <QVariant>

#include <verse.h>

#include "verse_data.h"

class VerseDataModel:public QAbstractTableModel
{
private:
    uint8_t session_id;

public:
    VerseData *verse_data;

    void setSessionID(uint8_t _session_id) { this->session_id = _session_id; }

    /**
     * @brief The constructor of VerseDataModel
     */
    VerseDataModel(VerseData *_data);

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
     * \brief setData
     * \param index
     * \param value
     * \param role
     * \return
     */
    bool setData(const QModelIndex & index, const QVariant & value, int role);

    void update(void);
};

#endif // VERSE_DATA_MODEL_H
