#include "verse_client.h"
#include "include/layerdialog.h"
#include "ui_layerdialog.h"

LayerDialog::LayerDialog(QWidget *parent, VerseNode *_node) :
    QDialog(parent),
    ui(new Ui::LayerDialog)
{
    ui->setupUi(this);

    if(_node != NULL) {
        VerseLayer *layer = NULL;

        ui->nodeIDLabel->setText(QString::number(_node->getID()));

        ui->parentLayerComboBox->addItem(QString("None"), QVariant(-1));
        // Add all existing layers from the node to the comboBox
        for(std::map<uint16_t, VerseLayer*>::iterator layer_iter = _node->layers.begin();
            layer_iter != _node->layers.end();
            ++layer_iter)
        {
            layer = layer_iter->second;
            ui->parentLayerComboBox->addItem(QString::number(layer->getID()), QVariant(layer->getID()));
        }

        for(uint8_t type = VRS_VALUE_TYPE_UINT8; type<=VRS_VALUE_TYPE_STRING8; type++) {
            ui->dataTypeComboBox->addItem(VerseTag::TagType(type), type);
        }

        this->session_id = ((VerseClient*)parent)->getSessionID();

        this->node = _node;
    }
}

LayerDialog::~LayerDialog()
{
    delete ui;
}

void LayerDialog::on_buttonBox_accepted()
{
    // TODO: get selected parent layer
    vrs_send_layer_create(this->session_id,
                          this->node->getPrio(),
                          this->node->getID(),
                          ui->parentLayerComboBox->itemData(ui->parentLayerComboBox->currentIndex()).toUInt(),
                          ui->dataTypeComboBox->itemData(ui->dataTypeComboBox->currentIndex()).toUInt(),
                          ui->countSpinBox->value(),
                          ui->customTypeSpinBox->value());
}
