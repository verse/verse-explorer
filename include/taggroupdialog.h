#ifndef TAGGROUPDIALOG_H
#define TAGGROUPDIALOG_H

#include <QDialog>

#include <verse.h>

#include "verse_node.h"

namespace Ui {
    class TagGroupDialog;
}

class TagGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagGroupDialog(QWidget *parent = 0,
                            VerseNode *node = NULL);
    ~TagGroupDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::TagGroupDialog *ui;

    uint8_t session_id;

    VerseNode *node;
};

#endif // TAGGROUPDIALOG_H
