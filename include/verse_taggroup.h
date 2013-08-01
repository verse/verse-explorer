#ifndef VERSE_TAGGROUP_H
#define VERSE_TAGGROUP_H

#include <QVariant>
#include <QString>
#include <QList>

#include <verse.h>

#include "verse_data.h"
#include "verse_tag.h"

class VerseNode;

class VerseTagGroup:public VerseData
{

public:

    /**
      * \brief The constructor of TagGroup
      */
    VerseTagGroup(VerseNode *_node, uint16_t _id, uint16_t _type);

    /**
      * \brief The destructor of TagGroup
      */
    ~VerseTagGroup();

    /**
      * \brief The getter of ID
      */
    uint16_t getID(void) { return this->id; }

    /**
      * \brief The getter of name
      */
    uint16_t getClientType(void) { return this->client_type; }

    /**
      * \brief The getter of node
      */
    VerseNode* getNode(void) { return this->node; }

    /**
      * \brief The getter of subscription flag
      */
    bool isSubscribed(void) { return this->subscribed; }

    /**
      * \brief The setter of subscription flag
      */
    void setSubscribed(bool s) { this->subscribed = s; }

    /**
      * \brief The getter of Tag
      */
    VerseTag* getTag(uint16_t tag_id);

    /**
      * \brief This method add tag to the map of tags
      */
    void addTag(VerseTag *tag);

    /**
      * \brief This method remove tag from the map of tags
      */
    void removeTag(VerseTag *tag);

    /**
      * \brief This method remove all tags from the map of tags
      */
    void removeTags(void);

    /**
     * \brief The getter for last tag id
     */
    uint16_t getLastTagID(void) { return this->last_tg_id; }

    /**
     * \brief The setter of last last tag id
     */
    void setLastTagID(uint16_t id) { this->last_tg_id = id; }

protected:

    VerseTagGroup() {}

private:

    /**
      * \brief The TagGroup bellong to this node
      */
    VerseNode *node;

    /**
      * \brief The ID of this TagGroup
      */
    uint16_t id;

    /**
      * \brief The type of this TagGroup
      */
    uint16_t client_type;

    /**
      * \brief The flag of subscription
      */
    bool subscribed;

    /**
      * \brief The map of Tag stored in this TagGroup
      */
    std::map<uint16_t, VerseTag*> tags;

    /**
     * \brief The last tag id in this tag group
     */
    uint16_t last_tg_id;

};

#endif // VERSE_TAGGROUP_H
