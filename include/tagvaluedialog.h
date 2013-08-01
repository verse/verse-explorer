#ifndef TAGVALUEDIALOG_H
#define TAGVALUEDIALOG_H

#include <QDialog>

#include <verse.h>

#include "verse_node.h"
#include "verse_taggroup.h"
#include "verse_tag.h"

namespace Ui {
class TagValueDialog;
}

class TagValueDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TagValueDialog(QWidget *parent = 0,
                            uint8_t _session_id = 0,
                            VerseTag *_tag = NULL);
    ~TagValueDialog();

    void sendValue(uint8_t index, double arg);
    
private slots:
    void on_tagValueSpinBox_0_valueChanged(double arg1);

    void on_tagValueSpinBox_1_valueChanged(double arg1);

    void on_tagValueSpinBox_2_valueChanged(double arg1);

    void on_tagValueSpinBox_3_valueChanged(double arg1);

private:
    Ui::TagValueDialog *ui;

    uint8_t session_id;
    VerseNode *node;
    VerseTagGroup *tg;
    VerseTag *tag;
};

#endif // TAGVALUEDIALOG_H
