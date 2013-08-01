#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include <verse.h>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0,
                         QString hostname = QString("localhost"),
                         uint8_t session_id = 0,
                         const char *username = NULL,
                         const uint8_t auth_methods_count = 0,
                         const uint8_t *methods = NULL);
    ~LoginDialog();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LoginDialog *ui;

    uint8_t session_id;
};

#endif // LOGINDIALOG_H
