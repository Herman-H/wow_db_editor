#include "trainer_teach_edit.h"
#include "ui_trainer_teach_edit.h"
#include <QComboBox>
#include <dbccache.h>

trainer_teach_edit::trainer_teach_edit(QSqlDatabase & db,
                                       burning_crusade_records::entityviews::dbcs_module_type& dbc,
                                       dbccache * c) :
    db(db),
    dbc(dbc),
    m_dbccache(c),
    ui(new Ui::trainer_teach_edit)
{
    ui->setupUi(this);
    spellselect = new spell_selecter{m_dbccache};
    ui->spellSelectLayout->addWidget(spellselect->getWidget());
    skilllineselect = new skillline_selecter{m_dbccache};
    ui->skillLineLayout->addWidget(skilllineselect->getWidget());
}

trainer_teach_edit::~trainer_teach_edit()
{
    delete ui;
}
