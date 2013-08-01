#include "fpsdialog.h"
#include "ui_fpsdialog.h"

#include <verse.h>

#include "verse_client.h"

FPSDialog::FPSDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FPSDialog)
{
    ui->setupUi(this);

    this->session_id = ((VerseClient*)parent)->getSessionID();

    this->client = (VerseClient*)parent;

    ui->fpsDoubleSpinBox->setValue(((VerseClient*)parent)->getFPS());
}

FPSDialog::~FPSDialog()
{
    delete ui;
}

void FPSDialog::on_buttonBox_accepted()
{
    float fps = ui->fpsDoubleSpinBox->value();

    vrs_send_fps(this->session_id, VRS_DEFAULT_PRIORITY, fps);

    this->client->setFPS(fps);
}
