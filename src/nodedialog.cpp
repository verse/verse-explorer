#include <verse.h>

#include "nodedialog.h"
#include "ui_nodedialog.h"

NodeDialog::NodeDialog(QWidget *parent, const uint8_t _session_id) :
    QDialog(parent),
    ui(new Ui::NodeDialog)
{
    ui->setupUi(this);

    this->session_id = _session_id;
}

NodeDialog::~NodeDialog()
{
    delete ui;
}

void NodeDialog::on_buttonBox_accepted()
{
    uint16_t type = ui->spinBox->value();

    vrs_send_node_create(this->session_id, VRS_DEFAULT_PRIORITY, type);
}
