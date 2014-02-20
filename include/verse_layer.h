#ifndef VERSE_LAYER_H
#define VERSE_LAYER_H

#include <verse.h>

#include "verse_data.h"

class VerseNode;

class VerseLayer:public VerseData
{
public:
    VerseLayer(VerseNode *_node,
               VerseLayer *parent_layer,
               const uint32_t _id,
               const uint8_t _data_type,
               const uint8_t _count,
               const uint16_t _custom_type);

    ~VerseLayer();

    /**
     * \brief getID
     * \return This metho returns ID of the Layer
     */
    uint16_t getID(void) const;

    /**
     * \brief getter of verse node
     * \return pointer at VerseNode
     */
    VerseNode *getNode(void) { return this->node; }

    /**
      * \brief The getter of custom type
      */
    uint8_t getClientType(void) { return this->custom_type; }

    /**
      * \brief The getter of subscription flag
      */
    bool isSubscribed(void) { return this->subscribed; }

    /**
     * \brief getter of data type
     * @return data type of layer
     */
    uint8_t getDataType(void) { return this->data_type; }

private:

    /**
     * \brief subscribed
     */
    bool subscribed;

    /**
      * \brief The Layer bellong to this node
      */
    VerseNode *node;

    /**
      * \brief The parent layer of this layer
      */
    VerseLayer *parent_layer;

    /**
      * \brief The ID of this TagGroup
      */
    uint16_t id;

    /**
     * \brief data_type stored in the layer
     */
    uint8_t data_type;

    /**
     * \brief count of values in each item
     */
    uint8_t count;

    /**
      * \brief The type of this TagGroup
      */
    uint16_t custom_type;
};

#endif // VERSE_LAYER_H
