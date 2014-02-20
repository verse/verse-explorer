#ifndef LAYERDIALOG_H
#define LAYERDIALOG_H

#include <QDialog>

#include <verse.h>

#include "verse_node.h"

namespace Ui {
class LayerDialog;
}

class LayerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LayerDialog(QWidget *parent = 0, VerseNode *_node = NULL);
    ~LayerDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::LayerDialog *ui;

    uint8_t session_id;

    VerseNode *node;
};

#endif // LAYERDIALOG_H
