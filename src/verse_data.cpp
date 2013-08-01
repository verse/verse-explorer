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
