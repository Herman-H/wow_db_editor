#ifndef VENDOR_EDIT_H
#define VENDOR_EDIT_H

#include <QWidget>
#include "dbwidgets.h"
#include "dbcwidgets.h"

namespace Ui {
class vendor_edit;
}

class dbcache;

class vendor_edit : public QWidget
{
    Q_OBJECT

public:
    explicit vendor_edit(dbcache *c, QWidget *parent = 0);
    ~vendor_edit();

private:
    Ui::vendor_edit *ui;
    dbcache *                       m_dbcache;
    itemextendedcost_selecter*      itemextendedcostselecter;
};

#endif // VENDOR_EDIT_H
