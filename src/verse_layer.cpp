#include "include/verse_layer.h"

VerseLayer::VerseLayer(VerseNode *_node,
                       VerseLayer *parent_layer,
                       const uint32_t _id,
                       const uint8_t _data_type,
                       const uint8_t _count,
                       const uint16_t _custom_type)
{
    this->type = VERSE_LAYER;

    this->subscribed = false;

    this->node = _node;
    this->parent_layer = parent_layer;
    this->id = _id;
    this->data_type = _data_type;
    this->count = _count;
    this->custom_type = _custom_type;
}

uint16_t VerseLayer::getID(void) const
{
    return this->id;
}
