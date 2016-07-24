#include "dbwidgets.h"
#include "creaturetemplatequerygroupbox.h"
#include "itemtemplatequery.h"
#include "SQL_Functions.h"

#include <QButtonGroup>
#include <QPushButton>
#include <QDialog>
#include <QLayout>
#include <QFormLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QListWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QScrollArea>
#include <QtConcurrent/QtConcurrent>
#include "dbccache.h"
#include "dbcache.h"

creature_template_selecter_widget::creature_template_selecter_widget(QSqlDatabase &db, dbccache * c) :
    db(db),
    m_dbccache(c)
{
    widget = new CreatureTemplateQueryGroupBox(db,m_dbccache);
}

creature_template_selecter_widget::~creature_template_selecter_widget(){}

CreatureTemplateQueryGroupBox * creature_template_selecter_widget::getWidget(){ return widget; }

void creature_template_selecter_widget::setIndexFromId(int id)
{
    // It is not interesting to do anything here.
    id = id;
}

int creature_template_selecter_widget::selectedId()
{
    return widget->selectedEntry();
}

creature_template_selecter_dialog::creature_template_selecter_dialog(QSqlDatabase &db, dbccache * c) :
    QObject(),
    db(db)
{
    widget = new QDialog();
    QHBoxLayout * wlayout = new QHBoxLayout();
    widget->setLayout(wlayout);
    widget->layout()->addWidget((ct = new CreatureTemplateQueryGroupBox(db,c)));
    ct->getCancelButtonLayout()->addWidget((cancelButton = new QPushButton()));
    cancelButton->setText("Cancel");

    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(ct,SIGNAL(selected()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}

creature_template_selecter_dialog::~creature_template_selecter_dialog(){}

void creature_template_selecter_dialog::setIndex(int id)
{
    // Maybe fill in some other default values?
    QSqlQuery q = db.exec(QString("SELECT Name FROM creature_template WHERE Entry = ") + QString::number(id) + QString(";"));
    if(q.next())
    {
        ct->setNameField(q.record().value(0).toString());
    }
}

void creature_template_selecter_dialog::acceptButtonTriggered()
{
    accept();
}
void creature_template_selecter_dialog::cancelButtonTriggered()
{
    reject();
}

void creature_template_selecter_dialog::accept()
{
    widget->setResult(ct->selectedEntry());
    widget->done(widget->result());
}
void creature_template_selecter_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

creature_template_selecter_button::creature_template_selecter_button(QSqlDatabase &db, dbccache * c, bool clearBtn) :
    QObject(),
    db(db)
{
    widget = new QWidget{};
    widget->setLayout(new QHBoxLayout{});
    widget->layout()->setContentsMargins(0,0,0,0);
    button = new QPushButton{};
    widget->layout()->addWidget(button);
    if(clearBtn)
    {
        clr = new QPushButton{".."};
        clr->setMaximumWidth(20);
        widget->layout()->addWidget(clr);
        QObject::connect(clr,SIGNAL(clicked()),this,SLOT(clear()));
    }
    button->setText("Creature Template...");
    selected = -1;
    QObject::connect(button,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    // initialize the dialog
    dialog = new creature_template_selecter_dialog{db, c};
}

creature_template_selecter_button::~creature_template_selecter_button(){}

QWidget * creature_template_selecter_button::getWidget(){ return widget; }

int creature_template_selecter_button::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return selected;
}

void creature_template_selecter_button::setIndexFromId(int id)
{
    selected = id;
    if(id == 0)
    {
        clear();
        return;
    }
    QSqlQuery q = db.exec(QString("SELECT Name FROM creature_template WHERE Entry = ") + QString::number(id) + QString(";"));
    if(q.next())
    {
        button->setText(q.record().value(0).toString());
    }
    else
    {
        button->setText(QString("DB Entry ") + QString::number(id));
    }
}

void creature_template_selecter_button::clear()
{
    selected = -1;
    button->setText("Creature Template...");
}

void creature_template_selecter_button::buttonPushed()
{
    dialog->setIndex(selected);
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        setIndexFromId(i);
    }
}

item_template_selecter_dialog::item_template_selecter_dialog(QSqlDatabase &db, dbccache * c) :
    QObject(),
    db(db),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setLayout(new QVBoxLayout());
    widget->layout()->addWidget((it = new itemtemplatequery(db,m_dbccache)));
    widget->setModal(true);
    QObject::connect(it->cancelButton(),SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(it,SIGNAL(itemselected()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}

item_template_selecter_dialog::~item_template_selecter_dialog(){}

void item_template_selecter_dialog::setIndex(int id)
{
    // Maybe fill in some other default values?
    QSqlQuery q = db.exec(QString("SELECT name FROM item_template WHERE entry = ") + QString::number(id) + QString(";"));
    if(q.next())
    {
        //it->setNameField(q.record().value(0).toString()); // Skip this, only annoying
    }
}

void item_template_selecter_dialog::acceptButtonTriggered()
{
    accept();
}
void item_template_selecter_dialog::cancelButtonTriggered()
{
    reject();
}

void item_template_selecter_dialog::accept()
{
    widget->setResult(it->selectedEntry());
    widget->done(widget->result());
}
void item_template_selecter_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

item_template_selecter_button::item_template_selecter_button(QSqlDatabase &db, dbccache * c) :
    QObject(),
    db(db)
{
    widget = new QPushButton();
    widget->setText("Item Template...");
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    dialog = new item_template_selecter_dialog{db,c};
}

item_template_selecter_button::~item_template_selecter_button(){}

QPushButton * item_template_selecter_button::getWidget(){ return widget; }

void item_template_selecter_button::setIndexFromId(int id)
{
    QString query = QString("SELECT name FROM item_template WHERE entry = ") + QString::number(id) + QString(";");
    QSqlQuery q = db.exec(query);
    selected = id;
    if(q.next())
    {
        widget->setText(q.value(0).toString() + QString(" (") + QString::number(id) + QString(")"));
        emit selection();
    }
    else
    {
        widget->setText(QString("DB Entry ") + QString::number(id));
    }
}

int item_template_selecter_button::selectedId()
{
    return selected;
}

void item_template_selecter_button::buttonPushed()
{
    dialog->setIndex(selected);
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        setIndexFromId(i);
    }
}

creature_loot_dialog::creature_loot_dialog(dbcache *c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QDialog();
    widget->setLayout((layout = new QVBoxLayout()));
    layout->addWidget((list = new QTreeWidget()));
    layout->addLayout((blayout = new QHBoxLayout()));
    blayout->addWidget((acceptButton = new QPushButton()));
    blayout->addWidget((clearButton = new QPushButton()));
    blayout->addWidget((cancelButton = new QPushButton()));
    acceptButton->setText("Accept");
    clearButton->setText("Clear");
    cancelButton->setText("Cancel");
    list->setColumnCount(1);
    list->setHeaderHidden(true);

    // Resource upon which this widget is dependant is loaded asynchonously
    // It is initialized using "void reload" instead through signals/slots

    widget->setModal(true);
    list->setMinimumSize(500,600);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    QObject::connect(acceptButton,SIGNAL(clicked()),this,SLOT(accept()));
    QObject::connect(clearButton,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}

creature_loot_dialog::~creature_loot_dialog(){}

void creature_loot_dialog::reload(db_resource_type r)
{
    if(r == db_resource_type::creature_loot)
    {
        list->clear();
    }
}

void creature_loot_dialog::setIndex(int index)
{
    list->setCurrentItem(list->topLevelItem(index));
}

void creature_loot_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void creature_loot_dialog::acceptButtonTriggered()
{
    accept();
}

void creature_loot_dialog::cancelButtonTriggered()
{
    reject();
}

void creature_loot_dialog::accept()
{
    int index = 0;
    if(index != -1)
    {
        widget->setResult(index+1);
        widget->done(widget->result());
    }
    else
    {
        reject();
    }
}

void creature_loot_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

creature_loot_selecter::creature_loot_selecter(dbcache * c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QPushButton();
    widget->setText("Creature Loot...");
    widget->setDisabled(true);
    dialog = new creature_loot_dialog(m_dbcache);
    selected = -1;
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(initialized()),this,SLOT(enable()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

creature_loot_selecter::~creature_loot_selecter(){}

void creature_loot_selecter::reload(db_resource_type r)
{
    if(r == db_resource_type::creature_loot)
    {
        selected = -1;
        widget->setText("Creature Loot...");
        dialog->reload(r);
    }
}

void creature_loot_selecter::disable()
{
    dialog->getWidget()->reject();
    if(selected != -1)
    {
    }
    else
    {
        pending_id = 0;
    }
    selected = -1;
    widget->setText("Creature Loot...");
    widget->setDisabled(true);
}

void creature_loot_selecter::enable()
{
    dialog->reload(db_resource_type::creature_loot);
    widget->setEnabled(true);
    if(pending_id != 0)
    {
        setIndexFromId(pending_id);
    }
}

QPushButton * creature_loot_selecter::getWidget(){ return widget; }

void creature_loot_selecter::setIndexFromId(int id)
{
    pending_id = 0;
    if(!(widget->isEnabled()))
    {
        pending_id = id;
        return;
    }
    if(selected == -1)
    {
        widget->setText("Creature Loot...");
        return;
    }
}

int creature_loot_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}

void creature_loot_selecter::clear()
{
    selected = -1;
    widget->setText("Creature Loot...");
}

void creature_loot_selecter::buttonPushed()
{
    if(selected != -1)
    {
        dialog->setIndex(selected);
    }
    else
    {
        dialog->setIndex(0);
    }
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        selected = i-1;
    }
}

disenchant_loot_dialog::disenchant_loot_dialog(dbcache * c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QDialog();
    widget->setLayout((layout = new QVBoxLayout()));
    layout->addWidget((list = new QTreeWidget()));
    layout->addLayout((blayout = new QHBoxLayout()));
    blayout->addWidget((acceptButton = new QPushButton()));
    blayout->addWidget((cancelButton = new QPushButton()));
    blayout->addWidget((clearButton = new QPushButton()));
    acceptButton->setText("Accept");
    cancelButton->setText("Cancel");
    clearButton->setText("Clear");
    list->setColumnCount(1);
    list->setHeaderHidden(true);

    list->addTopLevelItems(*(m_dbcache->get_disenchant_loot_resource()));

    widget->setModal(true);
    list->setMinimumSize(400,600);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    QObject::connect(acceptButton,SIGNAL(clicked()),this,SLOT(accept()));
    QObject::connect(clearButton,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}

disenchant_loot_dialog::~disenchant_loot_dialog(){}

void disenchant_loot_dialog::reload(db_resource_type r)
{
    if(r == db_resource_type::disenchant_loot)
    {
        list->clear();
        list->addTopLevelItems(*(m_dbcache->get_disenchant_loot_resource()));
    }
}

void disenchant_loot_dialog::setIndex(int index)
{
    list->setCurrentItem(list->topLevelItem(index));
}

void disenchant_loot_dialog::acceptButtonTriggered()
{
    accept();
}

void disenchant_loot_dialog::cancelButtonTriggered()
{
    reject();
}
void disenchant_loot_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void disenchant_loot_dialog::accept()
{
    int index = list->currentItem()->data(1,Qt::UserRole).toInt();
    widget->setResult(index+1);
    widget->done(widget->result());
}

void disenchant_loot_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

disenchant_loot_selecter::disenchant_loot_selecter(dbcache * c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QPushButton();
    widget->setText("Disenchant Loot...");
    dialog = new disenchant_loot_dialog{m_dbcache};
    selected = -1;
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

disenchant_loot_selecter::~disenchant_loot_selecter(){}

void disenchant_loot_selecter::reload(db_resource_type r)
{
    if(r == db_resource_type::disenchant_loot)
    {
        selected = -1;
        widget->setText("Disenchant Loot...");
        dialog->reload(r);
    }
}

QPushButton * disenchant_loot_selecter::getWidget(){ return widget; }

void disenchant_loot_selecter::setIndexFromId(int id)
{
    selected = dbcache::find_ref(m_dbcache->get_disenchant_loot_references(),id);
    if(selected == -1)
    {
        widget->setText("Disenchant Loot...");
        return;
    }
    widget->setText(m_dbcache->get_disenchant_loot_resource()->at(selected)->text(0));
}

int disenchant_loot_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbcache->get_disenchant_loot_references()->at(selected);
}

void disenchant_loot_selecter::clear()
{
    selected = -1;
    widget->setText("Disenchant Loot...");
}

void disenchant_loot_selecter::buttonPushed()
{
    if(selected != -1)
    {
        dialog->setIndex(selected);
    }
    else
    {
        dialog->setIndex(0);
    }
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        selected = i-1;
        widget->setText(m_dbcache->get_disenchant_loot_resource()->at(selected)->text(0));
    }
}

item_random_property_dialog::item_random_property_dialog(dbcache * c, bool suffix) :
    QObject(),
    m_dbcache(c),
    m_suffix(suffix)
{
    widget = new QDialog();
    widget->setLayout((layout = new QVBoxLayout()));
    layout->addWidget((list = new QTreeWidget()));
    layout->addLayout((blayout = new QHBoxLayout()));
    blayout->addWidget((acceptButton = new QPushButton()));
    blayout->addWidget((cancelButton = new QPushButton()));
    blayout->addWidget((clearButton = new QPushButton()));
    acceptButton->setText("Accept");
    cancelButton->setText("Cancel");
    clearButton->setText("Clear");
    list->setColumnCount(1);
    list->setHeaderHidden(true);
    if(suffix)
    {
        list->addTopLevelItems(*(m_dbcache->get_item_random_suffix_resource()));
    }
    else
    {
        list->addTopLevelItems(*(m_dbcache->get_item_random_property_resource()));
    }

    widget->setModal(true);
    list->setMinimumSize(400,600);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    QObject::connect(acceptButton,SIGNAL(clicked()),this,SLOT(accept()));
    QObject::connect(clearButton,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}

item_random_property_dialog::~item_random_property_dialog(){}

void item_random_property_dialog::reload(db_resource_type r)
{
    if(r == db_resource_type::item_random_property && !m_suffix)
    {
        list->clear();
        list->addTopLevelItems(*(m_dbcache->get_item_random_property_resource()));
    }
    if(r == db_resource_type::item_random_suffix && m_suffix)
    {
        list->clear();
        list->addTopLevelItems(*(m_dbcache->get_item_random_suffix_resource()));
    }
}

void item_random_property_dialog::setIndex(int index)
{
    list->setCurrentItem(list->topLevelItem(index));
}

void item_random_property_dialog::acceptButtonTriggered()
{
    accept();
}

void item_random_property_dialog::cancelButtonTriggered()
{
    reject();
}

void item_random_property_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}


void item_random_property_dialog::accept()
{
    QTreeWidgetItem * w = list->currentItem();
    widget->setResult(w->data(1,Qt::UserRole).toInt()+1);
    widget->done(widget->result());
}

void item_random_property_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

item_random_property_selecter::item_random_property_selecter(dbcache * c, bool suffix) :
    QObject(),
    m_dbcache(c),
    m_suffix(suffix)
{
    widget = new QPushButton();
    if(!suffix)
    {
        widget->setText("Random Property...");
    }
    else
    {
        widget->setText("Random Suffix...");
    }
    dialog = new item_random_property_dialog(m_dbcache,m_suffix);
    selected = -1;
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

item_random_property_selecter::~item_random_property_selecter(){}

void item_random_property_selecter::reload(db_resource_type r)
{
    if(r == db_resource_type::item_random_suffix && m_suffix)
    {
        dialog->reload(r);
        clear();
    }
    if(r == db_resource_type::item_random_property && !m_suffix)
    {
        dialog->reload(r);
        clear();
    }
}

QPushButton * item_random_property_selecter::getWidget(){ return widget; }

void item_random_property_selecter::setIndexFromId(int id)
{
    // this first check isn't needed, but still kept since most entries have id = 0
    if(id == 0)
    {
        selected = -1;
        if(m_suffix)
        {
            widget->setText("Random Suffix...");
        }
        else
        {
            widget->setText("Random Property...");
        }
    }
    else
    {
        if(m_suffix)
        {
            selected = dbcache::find_ref(m_dbcache->get_item_random_suffix_references(),id);
            if(selected != -1)
            {
                widget->setText(m_dbcache->get_item_random_suffix_resource()->at(selected)->text(0));
            }
            else
            {
                selected = -1;
                widget->setText("Random Suffix...");
            }
        }
        else
        {
            selected = dbcache::find_ref(m_dbcache->get_item_random_property_references(),id);
            if(selected != -1)
            {
                widget->setText(m_dbcache->get_item_random_property_resource()->at(selected)->text(0));
            }
            else
            {
                selected = -1;
                widget->setText("Random Property...");
            }
        }
    }
}

int item_random_property_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    if(m_suffix)
    {
        return m_dbcache->get_item_random_suffix_references()->at(selected);
    }
    else
    {
        return m_dbcache->get_item_random_property_references()->at(selected);
    }
}

void item_random_property_selecter::clear()
{
    setIndexFromId(0);
}

void item_random_property_selecter::buttonPushed()
{
    if(selected == -1)
    {
        dialog->setIndex(0);
    }
    else
    {
        dialog->setIndex(selected);
    }
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        selected = i-1;
        if(m_suffix)
        {
            widget->setText(m_dbcache->get_item_random_suffix_resource()->at(selected)->text(0));
        }
        else
        {
            widget->setText(m_dbcache->get_item_random_property_resource()->at(selected)->text(0));
        }
        emit changed();
    }
}

lock_dialog::lock_dialog(dbcache *c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QDialog();
    widget->setLayout((layout = new QVBoxLayout()));
    layout->addWidget((list = new QListWidget()));
    layout->addLayout((blayout = new QHBoxLayout()));
    blayout->addWidget((acceptButton = new QPushButton()));
    blayout->addWidget((cancelButton = new QPushButton()));
    blayout->addWidget((clearButton = new QPushButton()));
    cancelButton->setText("Cancel");
    acceptButton->setText("Accept");
    clearButton->setText("Clear");

    list->addItems(*(m_dbcache->get_lock_type_resource()));

    widget->setModal(true);
    list->setMinimumSize(400,600);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    QObject::connect(acceptButton,SIGNAL(clicked()),this,SLOT(accept()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(clearButton,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
}

lock_dialog::~lock_dialog(){}

void lock_dialog::reload(db_resource_type r)
{
    if(r == db_resource_type::lock_type)
    {
        list->clear();
        list->addItems(*(m_dbcache->get_lock_type_resource()));
    }
}

void lock_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void lock_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void lock_dialog::acceptButtonTriggered()
{
    accept();
}

void lock_dialog::cancelButtonTriggered()
{
    reject();
}


void lock_dialog::accept()
{
    widget->setResult(list->currentRow()+1);
    widget->done(widget->result());
}

void lock_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}


lock_selecter::lock_selecter(dbcache * c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QPushButton();
    widget->setText("Lock...");
    dialog = new lock_dialog{m_dbcache};
    selected = -1;
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

lock_selecter::~lock_selecter(){}

void lock_selecter::reload(db_resource_type r)
{
    if(r == db_resource_type::lock_type)
    {
        clear();
        dialog->reload(r);
    }
}

QPushButton * lock_selecter::getWidget(){ return widget; }

void lock_selecter::setIndexFromId(int id)
{
    selected = dbcache::find_ref(m_dbcache->get_lock_type_references(),id);
    if(selected == -1)
    {
        widget->setText("Lock...");
    }
    else
    {
        widget->setText(m_dbcache->get_lock_type_resource()->at(selected));
    }
}

int lock_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbcache->get_lock_type_references()->at(selected);
}

void lock_selecter::clear()
{
    setIndexFromId(0);
}

void lock_selecter::buttonPushed()
{
    if(selected == -1)
    {
        dialog->setIndex(0);
    }
    else
    {
        dialog->setIndex(selected);
    }
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        selected = i-1;
        widget->setText(m_dbcache->get_lock_type_resource()->at(selected));
    }
}

itemextendedcost_dialog::itemextendedcost_dialog(dbcache * c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Item Extended Cost");
    layout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    widget->setLayout(layout);
    list = new QListWidget{};
    list->addItems(*(m_dbcache->get_item_extended_cost_resource()));
    layout->addWidget(list);
    layout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setMinimumSize(400,500);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
}

itemextendedcost_dialog::~itemextendedcost_dialog(){ delete widget; }

QDialog * itemextendedcost_dialog::getWidget(){ return widget; }

void itemextendedcost_dialog::reload(db_resource_type r)
{
    if(r == db_resource_type::item_extended_cost)
    {
        list->clear();
        list->addItems(*(m_dbcache->get_item_extended_cost_resource()));
    }
}

void itemextendedcost_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void itemextendedcost_dialog::acceptButtonTriggered()
{
    accept();
}
void itemextendedcost_dialog::cancelButtonTriggered()
{
    reject();
}
void itemextendedcost_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void itemextendedcost_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void itemextendedcost_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

itemextendedcost_selecter::itemextendedcost_selecter(dbcache * c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QPushButton();
    widget->setText("Extended Cost...");
    selected = -1;
    dialog = new itemextendedcost_dialog{m_dbcache};
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

itemextendedcost_selecter::~itemextendedcost_selecter(){ delete widget; }

QPushButton * itemextendedcost_selecter::getWidget(){ return widget; }

void itemextendedcost_selecter::reload(db_resource_type r)
{
    if(r == db_resource_type::item_extended_cost)
    {
        selected = -1;
        widget->setText("Extended Cost...");
        dialog->reload(r);
    }
}

void itemextendedcost_selecter::setIndexFromId(int id)
{
    selected = dbcache::find_ref(m_dbcache->get_item_extended_cost_references(),id);
    if(selected == -1)
    {
        widget->setText("Extended Cost...");
        return;
    }
    widget->setText(m_dbcache->get_item_extended_cost_resource()->at(selected));
}

int itemextendedcost_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbcache->get_item_extended_cost_references()->at(selected);
}

void itemextendedcost_selecter::clear()
{
    selected = -1;
    widget->setText("Extended Cost...");
}

void itemextendedcost_selecter::buttonPushed()
{
    if(selected != -1)
    {
        dialog->setIndex(selected);
    }
    else
    {
        dialog->setIndex(0);
    }
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        selected = i-1;
        widget->setText(m_dbcache->get_item_extended_cost_resource()->at(selected));
    }
}

npc_text_view::npc_text_view(dbcache *c, int id) :
    QWidget(),
    m_dbcache(c)
{
    setView(id);
}

npc_text_view::~npc_text_view()
{
}

void npc_text_view::setView(int id)
{
    setLayout(new QVBoxLayout{});
    std::multimap<int,std::tuple<float,QString,QString>> * l_map = m_dbcache->get_npc_text_resource();
    // loop through all the texts and present with probability
    for(auto it = l_map->lower_bound(id); it != l_map->upper_bound(id); ++it)
    {
        float   p = std::get<0>((*it).second);
        QString t0 = std::get<1>((*it).second);
        QString t1 = std::get<2>((*it).second);
        int a = (t0.length()!=0) + (t1.length()!=0)*2;
        int c = (t0.length()!=0) + (t1.length()!=0);
        QGroupBox * box = new QGroupBox(QString::number(p*100) + QString("%"));
        box->setLayout(new QHBoxLayout{});
        layout()->addWidget(box);
        switch(c)
        {
        case 1: // Present only one of the texts
        {
            QLabel * label = new QLabel{};
            label->setMaximumWidth(400);
            label->setWordWrap(true);
            label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            box->layout()->addWidget(label);
            if(a==1) // t0 isnt empty
            {
                label->setText(t0);
            }
            if(a==2) // t1 isnt empty
            {
                label->setText(t1);
            }
            label->setAlignment(Qt::AlignCenter);
            break;
        }
        case 2: // Present both of the texts
        {
            QLabel * label0 = new QLabel{t0};
            label0->setMaximumWidth(400);
            label0->setWordWrap(true);
            QLabel * labelOR = new QLabel{" or "};
            labelOR->setMaximumWidth(20);
            QLabel * label1 = new QLabel{t1};
            label1->setMaximumWidth(400);
            label1->setWordWrap(true);
            label0->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            label1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            box->layout()->addWidget(label0);
            box->layout()->addWidget(labelOR);
            box->layout()->addWidget(label1);
            break;
        }
        default:
            break;
        }
    }
}

gossip_menu_dialog::gossip_menu_dialog(dbcache *c) :
    QObject(),
    m_dbcache(c),
    optionTriggerData{0},
    pressedOptionIndex(-1)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Gossip Menu");
    layout = new QHBoxLayout{};
    rscroll = new QScrollArea{};
    rscroll->setWidget(new QWidget{});
    llayout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    widget->setLayout(layout);
    layout->addLayout(llayout);
    layout->addWidget(rscroll);
    list = new QTreeWidget{};
    list->setColumnCount(1);
    list->addTopLevelItems(*(m_dbcache->get_gossip_resource()));
    llayout->addWidget(list);
    llayout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setColumnWidth(0, 2000);
    list->setMinimumSize(500,500);
    list->setHeaderHidden(true);
    rscroll->setMinimumSize(500,500);
    llayout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);

    optionButtonGroup = new QButtonGroup{};


    QObject::connect(list,SIGNAL(itemSelectionChanged()),this,SLOT(refreshScrollArea()));
    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
    QObject::connect(optionButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(optionButtonPressed(int)));
}

gossip_menu_dialog::~gossip_menu_dialog(){}

QDialog * gossip_menu_dialog::getWidget(){ return widget; }

void gossip_menu_dialog::reload(db_resource_type r)
{
    if(r == db_resource_type::gossip)
    {
        list->insertTopLevelItems(list->topLevelItemCount(),*(m_dbcache->get_gossip_resource()));
    }
}

void gossip_menu_dialog::setIndex(int index)
{
    list->setCurrentItem(list->topLevelItem(index));
}

void gossip_menu_dialog::refreshScrollArea()
{
    QTreeWidgetItem * newitem = list->selectedItems().at(0);
    bool isMenu = newitem->data(1,Qt::UserRole).toBool();
    if(!isMenu)
    {
        return;
    }
    // First of all handle the button group that will be removed
    for(int i = 0; i < optionButtonGroup->buttons().length(); ++i)
    {
        // Note that the button itself is child of the widget that is replaced, it will get deleted that way
        optionButtonGroup->removeButton(optionButtonGroup->button(i));
    }

    QWidget * newwidget = new QWidget{};
    QVBoxLayout * newlayout = new QVBoxLayout{};
    newwidget->setLayout(newlayout);
    int menuid = newitem->data(2,Qt::UserRole).toInt();
    for(auto it = m_dbcache->get_gossip_menu_resource()->lower_bound(menuid); it != m_dbcache->get_gossip_menu_resource()->upper_bound(menuid); ++it)
    {
        // For every menu text, show the text and its condition
        int textid = std::get<0>((*it).second);
        int conditionid = std::get<2>((*it).second);
        conditionid = dbcache::find_ref(m_dbcache->get_condition_references(),conditionid);
        QString conditiontext;
        if(conditionid != -1)
        {
            conditiontext = m_dbcache->get_condition_resource()->at(conditionid);
        }
        else
        {
            conditiontext = "Always true";
        }
        QGroupBox * newgroupbox = new QGroupBox{conditiontext};
        newlayout->addWidget(newgroupbox);
        newgroupbox->setLayout(new QHBoxLayout());
        newgroupbox->layout()->addWidget(new npc_text_view{m_dbcache,textid});
    }

    for(int i = 0; i < newitem->childCount(); ++i)
    {
        QTreeWidgetItem * optionitem = newitem->child(i);
        QString text = optionitem->text(0);
        int conditionid = optionitem->data(4,Qt::UserRole).toInt();
        conditionid = dbcache::find_ref(m_dbcache->get_condition_references(),conditionid);
        QString conditiontext;
        if(conditionid != -1)
        {
            conditiontext = m_dbcache->get_condition_resource()->at(conditionid);
        }
        else
        {
            conditiontext = "None";
        }
        int action = optionitem->data(5,Qt::UserRole).toInt();
        QString actiontext;
        if(action==-1){actiontext = "Close";}
        else if(action==0){actiontext = "";}
        else if(action==1){actiontext = "Open submenu"; }
        int treedataindex = optionitem->data(6,Qt::UserRole).toInt();
        if(action==1)
        {
            optionTriggerData[i] = treedataindex;
        }
        else
        {
            optionTriggerData[i] = -1;
        }
        QPushButton * optionButton = new QPushButton{text};
        optionButtonGroup->addButton(optionButton,i);
        QFormLayout * buttonDisplayLayout = new QFormLayout{};
        buttonDisplayLayout->addRow(QString("Condition:"),new QLabel{conditiontext});
        buttonDisplayLayout->addRow(QString("Action:"),new QLabel{actiontext});
        buttonDisplayLayout->addRow(optionButton);
        newlayout->addLayout(buttonDisplayLayout);
    }

    rscroll->setWidget(newwidget);
}

void gossip_menu_dialog::optionButtonPressed(int id)
{
    int index = id;
    int treedataindex = optionTriggerData[index];
    if(treedataindex == -1)
        return;
    std::vector<int> * vs = m_dbcache->get_gossip_treedata();
    index = vs->at(treedataindex);
    QTreeWidgetItem * w = list->topLevelItem(index);
    ++treedataindex;
    while(vs->at(treedataindex) != -1)
    {
        index = vs->at(treedataindex);
        w = w->child(index)->child(0); // first child is the optionwidget
        ++treedataindex;
    }
    list->setCurrentItem(w,0);
}

void gossip_menu_dialog::acceptButtonTriggered()
{
    accept();
}

void gossip_menu_dialog::cancelButtonTriggered()
{
    reject();
}

void gossip_menu_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void gossip_menu_dialog::accept()
{
    if(list->currentItem() == nullptr)
        return;
    // Find out which is the selected items menu
    bool widgetitemIsMenu = list->currentItem()->data(1,Qt::UserRole).toBool();
    int selectedmenuId = 0;
    if(widgetitemIsMenu)
    {
        selectedmenuId = list->currentItem()->data(2,Qt::UserRole).toInt();
    }
    else
    {
        // an option should always have a parent that is a menu
        // note that menu and option data are different
        selectedmenuId = list->currentItem()->parent()->data(2,Qt::UserRole).toInt();
    }

    int index = dbccache::find_ref(m_dbcache->get_gossip_references(),selectedmenuId);


    // if index is found amongst the current top level items in the tree, then return this index
    if(index != -1)
    {
        widget->setResult(index+1);
        widget->done(widget->result());
    }
    // otherwise create a new toplevelitem with entrance as the new menuid, then reload this widget, find the menuid index and return it
    else
    {
        index = m_dbcache->addNewGossipMenu(selectedmenuId);
        widget->setResult(index+1);
        widget->done(widget->result());
        reload(db_resource_type::gossip);
    }
}

void gossip_menu_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

gossip_menu_selecter::gossip_menu_selecter(dbcache *c) :
    QObject(),
    m_dbcache(c)
{
    widget = new QPushButton();
    widget->setText("Gossip...");
    selected = -1;
    dialog = new gossip_menu_dialog{m_dbcache};

    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
}

gossip_menu_selecter::~gossip_menu_selecter(){}

QPushButton * gossip_menu_selecter::getWidget(){ return widget; }

void gossip_menu_selecter::reload(db_resource_type r)
{
    if(r == db_resource_type::gossip)
    {
        selected = -1;
        widget->setText("Gossip...");
        dialog->reload(r);
    }
}

void gossip_menu_selecter::setIndexFromId(int id)
{
    selected = dbcache::find_ref(m_dbcache->get_gossip_references(),id);
    if(selected != -1)
    {
        widget->setText(m_dbcache->get_gossip_resource()->at(selected)->text(0));
    }
    else
    {
        widget->setText("Gossip Menu...");
    }
}

int gossip_menu_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return selected;
}

void gossip_menu_selecter::clear()
{
    selected = -1;
    widget->setText("Gossip...");
}

void gossip_menu_selecter::buttonPushed()
{
    if(selected != -1)
    {
        dialog->setIndex(selected);
    }
    else
    {
        dialog->setIndex(0);
    }
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        selected = i-1;
        widget->setText(m_dbcache->get_gossip_resource()->at(selected)->text(0));
    }
}























