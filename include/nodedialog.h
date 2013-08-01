#ifndef NODEDIALOG_H
#define NODEDIALOG_H

#include <QDialog>

#include <verse.h>

namespace Ui {
class NodeDialog;
}

class NodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NodeDialog(QWidget *parent = 0, const uint8_t _session_id = 0);
    ~NodeDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::NodeDialog *ui;

    uint8_t session_id;
};

#endif // NODEDIALOG_H
