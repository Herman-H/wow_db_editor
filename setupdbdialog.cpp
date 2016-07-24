#include "setupdbdialog.h"
#include "ui_setupdbdialog.h"

SetupDBDialog::SetupDBDialog(config &cfg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDBDialog),
    cfg(cfg)
{
    ui->setupUi(this);
    ui->hostnameLineEdit->setText(cfg.get_host().toString());
    ui->portLineEdit->setText(QString::number(cfg.get_port()));
    ui->usernameLineEdit->setText(cfg.get_user());
    ui->passwordLineEdit->setText(cfg.get_pass());
    ui->worldDBLineEdit->setText(cfg.get_world_db());
    ui->realmDBLineEdit->setText(cfg.get_realm_db());
    ui->charactersDBLineEdit->setText(cfg.get_chars_db());
    ui->comboBox->setCurrentIndex(static_cast<int>(cfg.get_dbms()));
}

SetupDBDialog::~SetupDBDialog()
{
    delete ui;
}

void SetupDBDialog::on_ConnectButton_clicked()
{
    // First save all information of the form
    cfg.set_host(ui->hostnameLineEdit->text());
    cfg.set_port(ui->portLineEdit->text());
    cfg.set_user(ui->usernameLineEdit->text());
    cfg.set_pass(ui->passwordLineEdit->text());
    cfg.set_world_db(ui->worldDBLineEdit->text());
    cfg.set_realm_db(ui->realmDBLineEdit->text());
    cfg.set_chars_db(ui->charactersDBLineEdit->text());
    cfg.set_dbms(ui->comboBox->currentIndex());
    cfg.save_config();
    emit issued_connect(); // parent will close this window
}

void SetupDBDialog::on_CancelButton_clicked()
{
    emit closed();
}
