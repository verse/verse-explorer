#include "tagdialog.h"
#include "ui_tagdialog.h"

#include "verse.h"

#include "verse_tag.h"

TagDialog::TagDialog(QWidget *parent,
                     uint8_t session_id,
                     VerseNode *node,
                     VerseTagGroup *tg) :
    QDialog(parent),
    ui(new Ui::TagDialog)
{
    ui->setupUi(this);

    this->session_id = session_id;
    this->node = node;
    this->tg = tg;

    ui->nodeIDLabel->setText(QString::number(node->getID()));

    ui->tagGroupIDLabel->setText(QString::number(tg->getID()));

    ui->tagGroupName->setText(QString::number(tg->getClientType()));

    for(uint8_t type = VRS_VALUE_TYPE_UINT8; type<=VRS_VALUE_TYPE_STRING8; type++) {
        ui->typeComboBox->addItem(VerseTag::TagType(type), type);
    }

    ui->typeComboBox->setCurrentIndex(0);
}

TagDialog::~TagDialog()
{
    this->node->remDialog(this);
    delete ui;
}

void TagDialog::on_buttonBox_accepted()
{
    uint8_t data_type = ui->typeComboBox->itemData(ui->typeComboBox->currentIndex()).toUInt();
    uint8_t count = ui->countSpinBox->value();
    uint16_t client_type = ui->clientTypeSpinBox->value();

    vrs_send_tag_create(this->session_id, this->node->getPrio(), this->node->getID(), this->tg->getID(),
                        data_type, count, client_type);
}
