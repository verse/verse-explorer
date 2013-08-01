#include <QDataWidgetMapper>

#include "tagvaluedialog.h"
#include "ui_tagvaluedialog.h"

#include "verse_tag.h"

TagValueDialog::TagValueDialog(QWidget *parent,
                               uint8_t _session_id,
                               VerseTag *_tag) :
    QDialog(parent),
    ui(new Ui::TagValueDialog)
{
    ui->setupUi(this);

    if(_tag != NULL) {
        VerseTagValue *tag_value;
        QString value_string;
        int decimals, min = -999999999;

        this->session_id = _session_id;
        this->node = _tag->getNode();
        this->tg = _tag->getTagGroup();
        this->tag = _tag;

        ui->nodeIDLabel->setText(QString::number(node->getID()));
        ui->tagGroupIDLabel->setText(QString::number(tg->getID()));
        ui->tagGroupNameLabel->setText(QString::number(tg->getClientType()));
        ui->tagIDLabel->setText(QString::number(tag->getID()));
        ui->tagNameLabel->setText(QString::number(tag->getClientType()));

        switch(tag->getDataType()) {
        case VRS_VALUE_TYPE_UINT8:
        case VRS_VALUE_TYPE_UINT16:
        case VRS_VALUE_TYPE_UINT32:
        case VRS_VALUE_TYPE_UINT64:
            min = 0;
            decimals = 0;
            break;
        case VRS_VALUE_TYPE_REAL16:
        case VRS_VALUE_TYPE_REAL32:
        case VRS_VALUE_TYPE_REAL64:
            decimals = 2;
            break;
        case VRS_VALUE_TYPE_STRING8:
            // TODO
            break;
        default:
            break;
        }

        for(int i=0; i<4; i++) {
            if(i<tag->value_count) {
                tag_value = tag->values[i];
            }

            if(i==0) {
                if(i<tag->value_count) {
                    ui->tagValueSpinBox_0->setValue(tag_value->getValue().toDouble());
                } else {
                    ui->tagValueSpinBox_0->setEnabled(false);
                }
                ui->tagValueSpinBox_0->setDecimals(decimals);
                ui->tagValueSpinBox_0->setMinimum(min);
            }
            if(i==1) {
                if(i<tag->value_count) {
                    ui->tagValueSpinBox_1->setValue(tag_value->getValue().toDouble());
                } else {
                    ui->tagValueSpinBox_1->setEnabled(false);
                }
                ui->tagValueSpinBox_1->setDecimals(decimals);
                ui->tagValueSpinBox_1->setMinimum(min);
            }
            if(i==2) {
                if(i<tag->value_count) {
                    ui->tagValueSpinBox_2->setValue(tag_value->getValue().toDouble());
                } else {
                    ui->tagValueSpinBox_2->setEnabled(false);
                }
                ui->tagValueSpinBox_2->setDecimals(decimals);
                ui->tagValueSpinBox_2->setMinimum(min);
            }
            if(i==3) {
                if(i<tag->value_count) {
                    ui->tagValueSpinBox_3->setValue(tag_value->getValue().toDouble());
                } else {
                    ui->tagValueSpinBox_3->setEnabled(false);
                }
                ui->tagValueSpinBox_3->setDecimals(decimals);
                ui->tagValueSpinBox_3->setMinimum(min);
            }
        }

        ui->incrementSpinBox->setDecimals(decimals);
    }
}

TagValueDialog::~TagValueDialog()
{
    this->node->remDialog(this);
    delete ui;
}

void TagValueDialog::sendValue(uint8_t index, double arg)
{
    VerseTagValue *tag_value;

    tag_value = this->tag->values[index];

    tag->sendValue(this->session_id, tag_value, arg);
}

void TagValueDialog::on_tagValueSpinBox_0_valueChanged(double arg1)
{
    this->sendValue(0, arg1);
}

void TagValueDialog::on_tagValueSpinBox_1_valueChanged(double arg1)
{
    this->sendValue(1, arg1);
}

void TagValueDialog::on_tagValueSpinBox_2_valueChanged(double arg1)
{
    this->sendValue(2, arg1);
}

void TagValueDialog::on_tagValueSpinBox_3_valueChanged(double arg1)
{
    this->sendValue(3, arg1);
}
