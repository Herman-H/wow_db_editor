#include "vendor_edit.h"
#include "ui_vendor_edit.h"
#include "dbcache.h"

vendor_edit::vendor_edit(dbcache * c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vendor_edit),
    m_dbcache(c)
{
    ui->setupUi(this);
    itemextendedcostselecter = new itemextendedcost_selecter(m_dbcache);
    ui->extendedcostLayout->addWidget(itemextendedcostselecter->getWidget());
}

vendor_edit::~vendor_edit()
{
    delete ui;
}
