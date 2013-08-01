#include <verse.h>

#include "login_dialog.h"
#include "ui_logindialog.h"

// Constructor
LoginDialog::LoginDialog(QWidget *parent,
                         QString hostname,
                         uint8_t session_id,
                         const char *username,
                         const uint8_t auth_methods_count,
                         const uint8_t *methods) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(hostname);

    this->session_id = session_id;

    if(auth_methods_count != 0) {
        // Add supported methods to combo box
        for(int i=0; i<auth_methods_count; i++) {
            switch(methods[i]) {
            case VRS_UA_METHOD_PASSWORD:
                ui->authMethodComboBox->addItem("Password", QVariant(VRS_UA_METHOD_PASSWORD));
                break;
            default:
                ui->authMethodComboBox->addItem("None", QVariant(VRS_UA_METHOD_NONE));
                break;
            }
        }

        // When there is more than one supported method, then allow selecting of methods
        if(auth_methods_count > 1) {
            ui->authMethodComboBox->setEnabled(true);
        }

    } else {
        // Default method is NONE
        ui->authMethodComboBox->addItem("None", QVariant(VRS_UA_METHOD_NONE));
    }

    if(username == NULL) {
        ui->usernameEdit->setEnabled(true);
        ui->passwordEdit->setEnabled(false);
    } else {
        QString *name = new QString(username);
        ui->usernameEdit->setText(*name);
        delete name;

        ui->usernameEdit->setEnabled(false);
        ui->passwordEdit->setEnabled(true);
    }
}

// Destructor
LoginDialog::~LoginDialog()
{
    delete ui;
}

// When accepted is clicked/entered
void LoginDialog::on_buttonBox_accepted()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    QByteArray ba_username = username.toLocal8Bit();
    QByteArray ba_password = password.toLocal8Bit();

    uint8_t ua_method = ui->authMethodComboBox->itemData(ui->authMethodComboBox->currentIndex()).toUInt();

    vrs_send_user_authenticate(session_id, ba_username.data(), ua_method, ba_password.data());
}

// When reject is clicked
void LoginDialog::on_buttonBox_rejected()
{
    vrs_send_connect_terminate(session_id);
}
