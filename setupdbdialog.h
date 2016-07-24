#ifndef SETUPDBDIALOG_H
#define SETUPDBDIALOG_H

#include <QDialog>
#include "config.h"

namespace Ui {
class SetupDBDialog;
}

class SetupDBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDBDialog(config &cfg,QWidget *parent = 0);
    ~SetupDBDialog();

private slots:
    void on_ConnectButton_clicked();
    void on_CancelButton_clicked();

signals:
    void closed();
    void issued_connect();

private:
    Ui::SetupDBDialog *ui;
    config &cfg;
};

#endif // SETUPDBDIALOG_H
