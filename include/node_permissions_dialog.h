#ifndef NODE_PERMISSIONS_DIALOG_H
#define NODE_PERMISSIONS_DIALOG_H

#include <QDialog>

namespace Ui {
    class NodePermissionsDialog;
}

class NodePermissionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodePermissionsDialog(QWidget *parent = 0);
    ~NodePermissionsDialog();

private:
    Ui::NodePermissionsDialog *ui;
};

#endif // NODE_PERMISSIONS_DIALOG_H
