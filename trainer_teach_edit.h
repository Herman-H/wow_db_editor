#ifndef TRAINER_TEACH_EDIT_H
#define TRAINER_TEACH_EDIT_H

#include <QWidget>
#include <QSqlDatabase>
#include "dbcwidgets.h"

class dbccache;

namespace Ui {
class trainer_teach_edit;
}

class trainer_teach_edit : public QWidget
{
    Q_OBJECT

public:
    explicit trainer_teach_edit(QSqlDatabase& db, entityviews::dbcs_module_type & dbc, dbccache * c);
    ~trainer_teach_edit();

private:
    QSqlDatabase& db;
    entityviews::dbcs_module_type & dbc;
    dbccache    * m_dbccache;

    Ui::trainer_teach_edit *ui;
    spell_selecter * spellselect;
    skillline_selecter * skilllineselect;
};

#endif // TRAINER_TEACH_EDIT_H
