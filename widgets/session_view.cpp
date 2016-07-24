#include "session_view.h"
#include "ui_session_view.h"
#include "session_manager.h"

session_view::session_view(const session_manager *sess_mgr, QWidget *parent) :
    QDialog(parent),
    sess_mgr(sess_mgr),
    ui(new Ui::session_view)
{
    ui->setupUi(this);
}

void session_view::update()
{
    ui->patchText->setPlainText(QString::fromStdString(sess_mgr->get_patches().first));
    ui->rollbackText->setPlainText(QString::fromStdString(sess_mgr->get_patches().second));
}

session_view::~session_view()
{
    delete ui;
}
