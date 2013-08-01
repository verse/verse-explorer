#include "verse_node.h"
#include "verse_taggroup.h"

VerseTagGroup::VerseTagGroup(VerseNode *_node, uint16_t _id, uint16_t _type)
{
    this->type = VERSE_TAG_GROUP;

    this->node = _node;
    this->id = _id;
    this->client_type = _type;

    this->subscribed = false;

    this->last_tg_id = 0;
}

VerseTagGroup::~VerseTagGroup()
{
    std::map<uint16_t, VerseTagGroup*>::iterator tg_iter;

    // Remove data from list of parent child data
    this->node->removeData(this);

    // Set parent to the NULL
    this->setParent(NULL);

    // Remove tag_group from map of tag_groups
    if(this->node->taggroups.count(this->id)==1) {
        tg_iter = this->node->taggroups.find(this->id);
        this->node->taggroups.erase(tg_iter);
    }
}

VerseTag * VerseTagGroup::getTag(uint16_t tag_id)
{
    std::map<uint16_t, VerseTag*>::iterator tag_iter;

    tag_iter = this->tags.find(tag_id);

    if(tag_iter != this->tags.end()) {
        return tag_iter->second;
    } else {
        return NULL;
    }
}

void VerseTagGroup::addTag(VerseTag *tag)
{
    this->tags[tag->getID()] = tag;
}

void VerseTagGroup::removeTag(VerseTag *tag)
{
    std::map<uint16_t, VerseTag*>::iterator tag_iter;

    if(this->tags.count(tag->getID())==1) {
        tag_iter = this->tags.find(tag->getID());
        this->tags.erase(tag_iter);
    }
}

void VerseTagGroup::removeTags()
{
    this->tags.clear();
}
