#ifndef NODEOWNERDIALOG_H
#define NODEOWNERDIALOG_H

#include <QDialog>

#include "verse_client.h"
#include "verse_node.h"

namespace Ui {
class NodeOwnerDialog;
}

class NodeOwnerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NodeOwnerDialog(QWidget *parent = 0, VerseNode *node = NULL);
    ~NodeOwnerDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::NodeOwnerDialog *ui;

    VerseClient *client;

    VerseNode *node;
};

#endif // NODEOWNERDIALOG_H
