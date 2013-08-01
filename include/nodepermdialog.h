#ifndef NODEPERMDIALOG_H
#define NODEPERMDIALOG_H

#include <QDialog>

#include "verse_node.h"
#include "verse_client.h"

namespace Ui {
    class NodePermDialog;
}

class NodePermDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodePermDialog(QWidget *parent = 0, VerseNode *n = NULL);
    ~NodePermDialog();

private slots:
    void on_userComboBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

private:
    Ui::NodePermDialog *ui;

    VerseNode *node;

    VerseClient *client;
};

#endif // NODEPERMDIALOG_H
