#include "nodepermdialog.h"
#include "ui_nodepermdialog.h"

#include "verse_client.h"
#include "verse_node.h"

#include <QString>

NodePermDialog::NodePermDialog(QWidget *parent, VerseNode *n) :
    QDialog(parent),
    ui(new Ui::NodePermDialog)
{
    VerseClient *client = (VerseClient*)parent;
    VerseUser *user;
    ui->setupUi(this);

    this->node = n;
    this->client = client;

    ui->idLabel->setText(QString::number(n->getID()));

    foreach(user, client->getUserList()) {
        /* It is useless to try to add/change permission for super user and owner */
        if(user->getID() != client->getUserID() && user->getID() != VRS_SUPER_USER_UID) {
            if(user->getID() == VRS_OTHER_USERS_UID) {
                ui->userComboBox->addItem("Others", user->getID());
            } else {
                ui->userComboBox->addItem(QString::number(user->getID()), user->getID());
            }
        }
    }
}

NodePermDialog::~NodePermDialog()
{
    this->node->remDialog(this);
    delete ui;
}

void NodePermDialog::on_userComboBox_currentIndexChanged(int index)
{
    VerseNodePerm *perm;
    uint16_t user_id = ui->userComboBox->itemData(index).toUInt();

    qDebug("index: %d -> user_id: %d\n", index, user_id);

    ui->readCheckBox->setChecked(false);
    ui->writeCheckBox->setChecked(false);

    foreach(perm, *node->getPermissions()) {
        if(perm->getUserID() == user_id) {
            if(perm->getPerm() & VRS_PERM_NODE_READ) {
                ui->readCheckBox->setChecked(true);
            }
            if(perm->getPerm() & VRS_PERM_NODE_WRITE) {
                ui->writeCheckBox->setChecked(true);
            }
        }
    }
}

void NodePermDialog::on_buttonBox_accepted()
{
    uint16_t user_id = ui->userComboBox->itemData(ui->userComboBox->currentIndex()).toUInt();
    uint8_t perm = 0;

    if(ui->readCheckBox->isChecked() == true) {
        perm |= VRS_PERM_NODE_READ;
    }

    if(ui->writeCheckBox->isChecked() == true) {
        perm |= VRS_PERM_NODE_WRITE;
    }

    vrs_send_node_perm(this->client->getSessionID(), this->node->getPrio(), this->node->getID(), user_id, perm);
}
