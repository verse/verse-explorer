#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>

#include <verse.h>

#include "verse_node.h"
#include "verse_taggroup.h"

namespace Ui {
class TagDialog;
}

class TagDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TagDialog(QWidget *parent = 0,
                       uint8_t session_id = 0,
                       VerseNode *node = NULL,
                       VerseTagGroup *tg = NULL);
    ~TagDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::TagDialog   *ui;

    uint8_t           session_id;

    VerseNode       *node;

    VerseTagGroup   *tg;
};

#endif // TAGDIALOG_H
