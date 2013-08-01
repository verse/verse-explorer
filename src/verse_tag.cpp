#include "verse_tag.h"

#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string.h>

#include <QString>

#include <verse.h>

#include "verse_node.h"
#include "verse_taggroup.h"

VerseTag::VerseTag(VerseNode *_node, VerseTagGroup *_tag_group,
                   uint16_t _id, uint8_t _type, uint8_t count, uint16_t _client_type)
{
    this->type = VerseData::VERSE_TAG;

    this->node = _node;
    this->tag_group = _tag_group;
    this->id = _id;
    this->client_type = _client_type;
    this->data_type = _type;
    this->model = new VerseDataModel(this);
    this->initialized = false;

    this->value_count = count;
    switch(this->data_type) {
    case VRS_VALUE_TYPE_UINT8:
        this->value_size = sizeof(uint8_t);
        break;
    case VRS_VALUE_TYPE_UINT16:
        this->value_size = sizeof(uint16_t);
        break;
    case VRS_VALUE_TYPE_UINT32:
        this->value_size = sizeof(uint32_t);
        break;
    case VRS_VALUE_TYPE_UINT64:
        this->value_size = sizeof(uint64_t);
        break;
    case VRS_VALUE_TYPE_REAL16:
        this->value_size = sizeof(uint16_t);
        break;
    case VRS_VALUE_TYPE_REAL32:
        this->value_size = sizeof(float);
        break;
    case VRS_VALUE_TYPE_REAL64:
        this->value_size = sizeof(double);
        break;
    case VRS_VALUE_TYPE_STRING8:
        this->value_size = 0;
        break;
    default:
        this->value_size = 0;
        this->value_count = 0;
        break;
    }

    if(this->value_size>0 && this->value_count>0) {
        this->values = (VerseTagValue**)calloc(this->value_count, sizeof(VerseTagValue*));
        for(int i=0; i<this->value_count; i++) {
            this->values[i] = new VerseTagValue(this, i);
            this->addData((VerseData*)this->values[i]);
        }
        this->model->update();
    } else {
        this->values = NULL;
    }
}

VerseTag::~VerseTag()
{
    VerseTagGroup *tg = this->getTagGroup();

    // Remove data from list of parent child data
    tg->removeData(this);

    // Set parent to the NULL
    this->setParent(NULL);

    // Remove tag from the map of tags in the tag group
    tg->removeTag(this);

    // Remove tag values
    if(this->values != NULL) {
        for(int i=0; i<this->value_count; i++) {
            delete this->values[i];
        }
        free(this->values);
    }

    delete this->model;
}

QString VerseTag::TagType(uint8_t _data_type)
{
    switch(_data_type) {
    case VRS_VALUE_TYPE_UINT8:
        return "Uint8";
    case VRS_VALUE_TYPE_UINT16:
        return "Uint16";
    case VRS_VALUE_TYPE_UINT32:
        return "Uint32";
    case VRS_VALUE_TYPE_UINT64:
        return "Uint64";
    case VRS_VALUE_TYPE_REAL16:
        return "Real16";
    case VRS_VALUE_TYPE_REAL32:
        return "Real32";
    case VRS_VALUE_TYPE_REAL64:
        return "Real64";
    case VRS_VALUE_TYPE_STRING8:
        return "String8";
    default:
        return "Unknown";
    }
}

void VerseTag::setTagValue(const void *_value, const uint8_t _data_type, const uint8_t _count)
{
    if(_data_type != this->data_type) {
        return;
    }

    if(_count != this->value_count) {
        return;
    }

    switch(this->data_type) {
    case VRS_VALUE_TYPE_UINT8:
    case VRS_VALUE_TYPE_UINT16:
    case VRS_VALUE_TYPE_UINT32:
    case VRS_VALUE_TYPE_UINT64:
    case VRS_VALUE_TYPE_REAL16:
    case VRS_VALUE_TYPE_REAL32:
    case VRS_VALUE_TYPE_REAL64:
        for(int i=0; i<this->value_count; i++) {
            memcpy(this->values[i]->value, ((char*)_value)+i*this->value_size, this->value_size);
        }
        break;
    case VRS_VALUE_TYPE_STRING8:
        // TODO
        break;
    default:
        break;
    }

    this->initialized = true;

    // TODO: fix this (need to update only value)
    this->model->update();
}

void* VerseTag::getTagValue(uint8_t index)
{
    if(index <= this->value_count) {
        return this->values[index]->value;
    } else {
        return NULL;
    }
}

bool VerseTag::sendValue(uint8_t session_id, VerseTagValue *tag_value, const QVariant &value)
{
    VerseTagGroup *tg = this->getTagGroup();
    VerseNode *node = tg->getNode();
    long int v_int;
    double v_dbl;

    (void)tag_value;

    switch(this->getDataType()) {
    case VRS_VALUE_TYPE_UINT8:
        v_int = value.toInt();
        // Limit max and min value
        v_int = (v_int > UCHAR_MAX) ? UCHAR_MAX : v_int;
        v_int = (v_int < 0) ? 0 : v_int;
        vrs_send_tag_set_value(session_id, node->getPrio(), node->getID(), tg->getID(), this->getID(), this->getDataType(), 1, &v_int);
        break;
    case VRS_VALUE_TYPE_UINT16:
        v_int = value.toInt();
        // Limit max and min value
        v_int = (v_int > USHRT_MAX) ? USHRT_MAX : v_int;
        v_int = (v_int < 0) ? 0 : v_int;
        vrs_send_tag_set_value(session_id, node->getPrio(), node->getID(), tg->getID(), this->getID(), this->getDataType(), 1, &v_int);
        break;
    case VRS_VALUE_TYPE_UINT32:
        v_int = value.toInt();
        // Limit max and min value
        v_int = (v_int > UINT_MAX) ? UINT_MAX : v_int;
        v_int = (v_int < 0) ? 0 : v_int;
        vrs_send_tag_set_value(session_id, node->getPrio(), node->getID(), tg->getID(), this->getID(), this->getDataType(), 1, &v_int);
        break;
    case VRS_VALUE_TYPE_REAL32:
        v_dbl = value.toDouble();
        // Limit max and min value
        v_dbl = (v_dbl > FLT_MAX) ? FLT_MAX : v_dbl;
        v_dbl = (v_dbl < -FLT_MAX) ? -FLT_MAX : v_dbl;
        vrs_send_tag_set_value(session_id, node->getPrio(), node->getID(), tg->getID(), this->getID(), this->getDataType(), 1, &v_dbl);
        break;
    case VRS_VALUE_TYPE_REAL64:
        v_dbl = value.toDouble();
        vrs_send_tag_set_value(session_id, node->getPrio(), node->getID(), tg->getID(), this->getID(), this->getDataType(), 1, &v_dbl);
        break;
    case VRS_VALUE_TYPE_STRING8:
        // TODO
        return false;
        break;
    default:
        return false;
        break;
    }

    return true;
}

VerseTagValue::VerseTagValue(VerseTag *_tag, uint8_t _index)
{
    this->type = VerseData::VERSE_TAG_VALUE;
    this->tag = _tag;
    this->index = _index;
    this->value = calloc(1, tag->getTagValueSize());
}

VerseTagValue::~VerseTagValue()
{
    if(this->value != NULL) {
        free(this->value);
    }
}

QVariant VerseTagValue::getValue(void)
{
    switch(this->tag->getDataType()) {
    case VRS_VALUE_TYPE_UINT8:
        return *((uint8_t*)this->value);
        break;
    case VRS_VALUE_TYPE_UINT16:
        return *((uint16_t*)this->value);
        break;
    case VRS_VALUE_TYPE_UINT32:
        return *((uint32_t*)this->value);
        break;
    case VRS_VALUE_TYPE_UINT64:
        return (int)*((uint64_t*)this->value);
        break;
    case VRS_VALUE_TYPE_REAL32:
        return (float)*((float*)this->value);
        break;
    case VRS_VALUE_TYPE_REAL64:
        return (double)*((double*)this->value);
        break;
    case VRS_VALUE_TYPE_STRING8:
        // TODO
        return QVariant();
        break;
    default:
        return QVariant();
        break;
    }
}
