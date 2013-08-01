#include "verse_client.h"
#include "taggroupdialog.h"
#include "ui_taggroupdialog.h"

TagGroupDialog::TagGroupDialog(QWidget *parent, VerseNode *_node) :
    QDialog(parent),
    ui(new Ui::TagGroupDialog)
{
    ui->setupUi(this);

    ui->nodeIDLabel->setText(QString::number(_node->getID()));

    this->session_id = ((VerseClient*)parent)->getSessionID();

    this->node = _node;
}

TagGroupDialog::~TagGroupDialog()
{
    this->node->remDialog(this);
    delete ui;
}

void TagGroupDialog::on_buttonBox_accepted()
{
    int type = ui->tgTypeSpinBox->value();

    vrs_send_taggroup_create(this->session_id, this->node->getPrio(), this->node->getID(), type);
}
