#include "verse_data.h"

VerseData::VerseData()
{
    this->type = RESERVED;
    this->parent_data = NULL;
    this->model_index = QModelIndex();
}

VerseData::VerseData(VerseData *parent)
{
    this->type = RESERVED;
    this->parent_data = parent;
    this->model_index = QModelIndex();
}

QString VerseData::ValueDataType(uint8_t _data_type)
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

void VerseData::addData(VerseData *data)
{
    this->child_data.append(data);
    data->setParent(this);
}

void VerseData::removeData(VerseData *data)
{
    this->child_data.removeAt(data->getRow());
}

void VerseData::removeData(void)
{
    this->child_data.clear();
}

int VerseData::getRow(void)
{
    if(this->parent_data != NULL) {
        return this->parent_data->child_data.indexOf(const_cast<VerseData*>(this));
    }

    return 0;
}

int VerseData::childCount()
{
    return this->child_data.count();
}
