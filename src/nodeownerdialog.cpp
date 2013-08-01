#include "nodeownerdialog.h"
#include "ui_nodeownerdialog.h"

NodeOwnerDialog::NodeOwnerDialog(QWidget *parent, VerseNode *node) :
    QDialog(parent),
    ui(new Ui::NodeOwnerDialog)
{
    VerseUser *user;
    int index = 0, cur = 0;

    ui->setupUi(this);

    this->client = (VerseClient*)parent;

    this->node = node;

    ui->idLabel->setText(QString::number(node->getID()));

    foreach(user, client->getUserList()) {
        /* It is useless to try to add/change permission for super user and owner */
        if(user->getID() != VRS_OTHER_USERS_UID && user->getID() != VRS_SUPER_USER_UID) {
            if(user->getID() == this->client->getUserID()) {
                ui->ownerComboBox->addItem("Me", user->getID());
                cur = index;
            } else {
                ui->ownerComboBox->addItem(QString::number(user->getID()), user->getID());
            }
            index++;
        }
    }

    ui->ownerComboBox->setCurrentIndex(cur);
}

NodeOwnerDialog::~NodeOwnerDialog()
{
    this->node->remDialog(this);
    delete ui;
}

void NodeOwnerDialog::on_buttonBox_accepted()
{
    uint16_t user_id = ui->ownerComboBox->itemData(ui->ownerComboBox->currentIndex()).toUInt();

    vrs_send_node_owner(this->client->getSessionID(), this->node->getPrio(), this->node->getID(), user_id);
}
