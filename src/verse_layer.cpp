#include "include/verse_layer.h"
#include "verse_node.h"

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

VerseLayer::~VerseLayer()
{
    std::map<uint16_t, VerseLayer*>::iterator layer_iter;

    // Remove data from list of parent child data
    this->node->removeData(this);

    // Set parent to the NULL
    this->setParent(NULL);

    // Remove tag_group from map of tag_groups
    if(this->node->layers.count(this->id) == 1) {
        layer_iter = this->node->layers.find(this->id);
        this->node->layers.erase(layer_iter);
    }
}

uint16_t VerseLayer::getID(void) const
{
    return this->id;
}
