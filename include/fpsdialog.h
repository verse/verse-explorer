#ifndef FPSDIALOG_H
#define FPSDIALOG_H

#include <QDialog>

#include <verse.h>

#include "verse_client.h"

namespace Ui {
class FPSDialog;
}

class FPSDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FPSDialog(QWidget *parent = 0);
    ~FPSDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::FPSDialog *ui;

    uint8_t session_id;

    VerseClient *client;
};

#endif // FPSDIALOG_H
