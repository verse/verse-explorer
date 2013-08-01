#ifndef VERSE_DATA_H
#define VERSE_DATA_H

#include <QVariant>
#include <QModelIndex>

class VerseData
{
private:

    /**
      * \brief The pointer at parent data
      */
    VerseData*          parent_data;

    /**
      * \brief The list of data (taggroups, tags, layers)
      */
    QList<VerseData*>   child_data;

    /**
     * \brief The index of this data in model
     */
    QModelIndex model_index;

protected:

    /**
      * \brief Constructor without parameters
      */
    VerseData();

public:

    /**
      * \brief This method adds new data to this data
      */
    void addData(VerseData *data);

    /**
      * \brief This method removes child data from this data
      */
    void removeData(VerseData *data);

    /**
      * \brief This method removes all child data from this data
      */
    void removeData(void);

    /**
      * \brief Enum of data type
      */
    typedef enum {
        RESERVED,
        VERSE_NODE,
        VERSE_TAG_GROUP,
        VERSE_TAG,
        VERSE_TAG_VALUE,
        VERSE_LAYER,
        VERSE_LAYER_VALUE
    } DataType;

    /**
      * \brief The type of data
      */
    DataType type;

    /**
      * \brief The constructor
      */
    VerseData(VerseData *parent);

    /**
      * \brief The getter of child data
      */
    VerseData* getChildData(int row) { return this->child_data.value(row); }

    /**
      * \brief This method returns row in list of parent data
      */
    int getRow(void);

    /**
      * \brief The getter for parent data
      */
    VerseData* getParent(void) { return this->parent_data; }

    /**
      * \brief The getter for type
      */
    DataType getType(void) { return this->type; }

    /**
      * \brief The count of data
      */
    int childCount(void);

    /**
      * \brief The setter for parent_data
      */
    void setParent(VerseData *parent) { this->parent_data = parent; }

    /**
     * \brief The getModelIndex is getter of ModelIndex of this data
     * \return
     */
    QModelIndex getModelIndex(void) { return this->model_index; }

    /**
     * \brief The setModelIndex is setter of ModelIndex for this data
     * \param _index
     */
    void setModelIndex(QModelIndex _index) { this->model_index = _index; }
};

#endif // VERSE_DATA_H
