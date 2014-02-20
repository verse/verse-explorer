#ifndef VERSE_TAG_H
#define VERSE_TAG_H

#include <QString>

#include <verse.h>

#include "verse_data.h"
#include "verse_data_model.h"

class VerseNode;
class VerseTagGroup;
class VerseTag;

class VerseTagValue:public VerseData
{
private:
    uint8_t index;

public:
    VerseTagValue(VerseTag *_tag, uint8_t _index);

    ~VerseTagValue();

    VerseTag *tag;

    /**
      * \brief The pointer at value
      */
    void *value;

    /**
     * @brief The method getIndex is getter of index
     * @return
     */
    uint8_t getIndex(void) { return this->index; }

    /**
      * \brief It is not posssible to add more child data to tag value
      */
    void addData(VerseData* /*data*/) {}

    /**
      * \brief The tag value has no children
      */
    int childCount(void) { return 0; }

    /**
      * \brief Blind method, because a tag value has no children
      */
    void removeData(VerseData* /*data*/) {}

    /**
      * \brief Blind method, because a tag value has no children
      */
    void removeData(void) {}

    /**
     * \brief The method getValue returns value of VerseTagValue
     * \return
     */
    QVariant getValue(void);
};

class VerseTag:public VerseData
{    
protected:
    VerseTag() {}

private:

    /**
      * \brief
      */
    VerseNode *node;

    /**
      * \brief
      */
    VerseTagGroup *tag_group;

    /**
      * \brief The ID of tag
      */
    uint16_t id;

    /**
      * \brief The type of the tag
      */
    uint8_t client_type;

    /**
      * \brief The type of tag
      */
    uint8_t data_type;

    /**
     * \brief This flag save information if this flag is initialized
     */
    bool initialized;

public:

    /**
     * \brief The array of pointers at values
     */
    VerseTagValue **values;

    /**
      * \brief The size of value in memory
      */
    uint8_t value_size;

    /**
      * \brief The count of values (usualy: 1, 2, 3, 4)
      */
    uint8_t value_count;

    /**
      * \brief The pointer at  model
      */
    VerseDataModel *model;

    /**
      * \brief The constructor with usefull values
      */
    VerseTag(VerseNode *_node, VerseTagGroup *_tag_group,
             uint16_t _id, uint8_t _type, uint8_t count, uint16_t _client_type);

    /**
      * \brief The destructor
      */
    ~VerseTag();

    /**
     * \brief The getter of VerseDataModel
     *
     * \return This method return pointer at VerseDataModel of the Tag
     */
    VerseDataModel *getDataModel(void) { return this->model; }

    /**
      * \brief The getter of ID
      */
    uint16_t getID(void) { return this->id; }

    /**
      * \brief The getter of data type
      */
    uint8_t getDataType(void) { return this->data_type; }

    /**
      * \brief This method set value of tag
      */
    void setTagValue(const void *_value, const uint8_t _data_type, const uint8_t _count);

    /**
      * \brief The getter of tag value
      */
    void* getTagValue(uint8_t index);

    /**
     * \brief The method getTagValueSize is getter of tag value size
     *
     * \return This method return size in bytes of one value
     */
    uint8_t getTagValueSize(void) { return this->value_size; }

    /**
      * \brief The getter of custom type
      */
    uint8_t getClientType(void) { return this->client_type; }

    /**
     * \brief The getter of TagGroup
     */
    VerseTagGroup* getTagGroup(void) { return this->tag_group; }

    /**
     * \brief The getter of Node
     */
    VerseNode* getNode(void) { return this->node; }

    /**
     * @brief The getter of initialized flag
     *
     * @return This getter return true, if tag vas initialized. Otherwise
     * it returns false.
     */
    bool isInitialized(void) { return this->initialized; }

    /**
     * \brief The method sendValue send it's value to the server
     * \param tag_value
     * \param value
     */
    bool sendValue(uint8_t session_id, VerseTagValue *tag_value, const QVariant &value);
};

#endif // VERSE_TAG_H
