#include "dbcwidgets.h"
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QString>
#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include "dbccache.h"
#include <vector>
#include <algorithm>

creature_family_selecter::creature_family_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_creature_family_resource()));
}

creature_family_selecter::~creature_family_selecter()
{
}

void creature_family_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::creature_family)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_creature_family_resource()));
    }
}

QComboBox * creature_family_selecter::getWidget()
{
    return widget;
}

void creature_family_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_creature_family_references(), id);
    if(index != -1)
    {
        index += 2;
        widget->setCurrentIndex(index);
    }
    else
    {
        widget->setCurrentIndex(1);
    }
}

int creature_family_selecter::selectedId()
{
    int index = widget->currentIndex() - 2;
    if(index == -1 || index == -2)
    {
        return 0;
    }
    return m_dbccache->get_creature_family_references()->at(index);
}

creature_type_selecter::creature_type_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_creature_type_resource()));
}

creature_type_selecter::~creature_type_selecter()
{
}

QComboBox * creature_type_selecter::getWidget()
{
    return widget;
}

void creature_type_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::creature_type)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_creature_type_resource()));
    }
}

void creature_type_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_creature_type_references(),id);
    if(index != -1)
    {
        index += 2;
        widget->setCurrentIndex(index);
    }
    else
    {
        widget->setCurrentIndex(1);
    }
}

int creature_type_selecter::selectedId()
{
    int index = widget->currentIndex() - 2;
    if(index == -1 || index == -2)
    {
        return 0;
    }
    return m_dbccache->get_creature_type_references()->at(index);
}

skillline_selecter::skillline_selecter(dbccache *c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_skillline_resource()));
}

skillline_selecter::~skillline_selecter()
{
    delete widget;
}

void skillline_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::skillline)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_skillline_resource()));
    }
}

QComboBox * skillline_selecter::getWidget() { return widget; }

void skillline_selecter::clear() { widget->setCurrentIndex(0); }

void skillline_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_skillline_references(),id);
    if(index != -1)
    {
        widget->setCurrentIndex(index + 1);
    }
    else
    {
        widget->setCurrentIndex(0);
    }
}
int skillline_selecter::selectedId()
{
    if(widget->currentIndex() == 0) // default value
    {
        return 0;
    }
    return m_dbccache->get_skillline_references()->at(widget->currentIndex()-1);
}

faction_dialog::faction_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Faction");
    layout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    widget->setLayout(layout);
    list = new QListWidget{};
    list->addItems(*(m_dbccache->get_faction_resource()));

    layout->addWidget(list);
    layout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setMinimumSize(500,500);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonPushed()));
}

faction_dialog::~faction_dialog()
{
    delete widget;
}

QDialog * faction_dialog::getWidget() { return widget; }

void faction_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::faction)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_faction_resource()));
    }
}

void faction_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void faction_dialog::acceptButtonTriggered()
{
    accept();
}
void faction_dialog::cancelButtonTriggered()
{
    reject();
}

void faction_dialog::clearButtonPushed()
{
    emit clear();
    reject();
}

void faction_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void faction_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

faction_selecter::faction_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    selected = -1;
    widget->setText("Faction...");
    dialog = new faction_dialog{c};
    widget->connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    widget->connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

faction_selecter::~faction_selecter(){ }

QPushButton * faction_selecter::getWidget() { return widget; }

void faction_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::faction)
    {
        dialog->reload(r);
    }
}

void faction_selecter::setIndexFromId(int id)
{
    selected = dbccache::find_ref(m_dbccache->get_faction_references(), id);
    if(selected != -1)
    {
        widget->setText(m_dbccache->get_faction_plain()->at(selected));
    }
    else
    {
        widget->setText("Faction...");
    }
}

int faction_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbccache->get_faction_references()->at(selected);
}

int faction_selecter::currentIndex()
{
    if(selected == -1)  { return 0; }
    else                { return selected; }
}

void faction_selecter::buttonPushed()
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
        if(selected != -1)
        {
            widget->setText(m_dbccache->get_faction_plain()->at(selected));
        }
        else
        {
            widget->setText("Faction...");
        }
    }
}

void faction_selecter::clear()
{
    selected = -1;
    widget->setText("Faction...");
}

reputation_faction_selecter::reputation_faction_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_reputation_faction_resource()));
}

reputation_faction_selecter::~reputation_faction_selecter(){}

void reputation_faction_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::reputation_faction)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_reputation_faction_resource()));
    }
}

QComboBox * reputation_faction_selecter::getWidget(){ return widget; }

void reputation_faction_selecter::clear(){ widget->setCurrentIndex(0); }

int reputation_faction_selecter::selectedId()
{
    if(widget->currentIndex())
    {
        return m_dbccache->get_reputation_faction_references()->at(widget->currentIndex()-1);
    }
    else
    {
        return 0;
    }
}

void reputation_faction_selecter::setIndexFromId(int id)
{
    if(id == 0)
    {
        widget->setCurrentIndex(0);
        return;
    }
    int index = dbccache::find_ref(m_dbccache->get_reputation_faction_references(), id);
    if(index != -1)
    {
        widget->setCurrentIndex(index+1);
    }
    else
    {
        widget->setCurrentIndex(0);
    }
}

reputation_rank_selecter::reputation_rank_selecter()
{
    widget = new QComboBox();
    widget->addItem("Standing...");
    widget->addItem("Hated");
    widget->addItem("Hostile");
    widget->addItem("Unfriendly");
    widget->addItem("Neutral");
    widget->addItem("Friendly");
    widget->addItem("Honored");
    widget->addItem("Revered");
    widget->addItem("Exalted");
}
reputation_rank_selecter::~reputation_rank_selecter(){}

QComboBox * reputation_rank_selecter::getWidget(){ return widget; }

void reputation_rank_selecter::clear(){ widget->setCurrentIndex(0); }

int reputation_rank_selecter::selectedId()
{
    if(widget->currentIndex() == 0){ return 0; }
    return widget->currentIndex()-1;
}

void reputation_rank_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(id+1);
}

map_dialog::map_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Map");
    layout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    widget->setLayout(layout);
    list = new QListWidget{};
    list->addItems(*(m_dbccache->get_map_resource()));
    layout->addWidget(list);
    layout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setMinimumSize(150,400);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
}

map_dialog::~map_dialog(){ delete widget; }

QDialog * map_dialog::getWidget(){ return widget; }

void map_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::map)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_map_resource()));
    }
}

void map_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void map_dialog::acceptButtonTriggered()
{
    accept();
}
void map_dialog::cancelButtonTriggered()
{
    reject();
}

void map_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void map_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void map_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

map_selecter::map_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    selected = -1;
    widget->setText("Map...");
    dialog = new map_dialog{m_dbccache};
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

map_selecter::~map_selecter(){ delete widget; }

void map_selecter::reload(dbc_resource_type r)
{
    if(dbc_resource_type::map == r)
    {
        selected = -1;
        widget->setText("Map...");
        dialog->reload(r);
    }
}

QPushButton * map_selecter::getWidget() { return widget; }

void map_selecter::clear()
{
    selected = -1;
    widget->setText("Map...");
}

void map_selecter::setIndexFromId(int id)
{
    if(id == -1)
    {
        selected = -1;
        widget->setText("Map...");
        return;
    }
    selected = dbccache::find_ref(m_dbccache->get_map_references(), id);
    if(selected != -1 && selected != 0) // 0 is id for eastern kingdom treat is as nothing selected
    {
        widget->setText(m_dbccache->get_map_resource()->at(selected));
    }
    else
    {
        selected = -1;
        widget->setText("Map...");
    }
}

int map_selecter::selectedId()
{
    if(selected != -1)
    {
        return m_dbccache->get_map_references()->at(selected);
    }
    return 0;
}

void map_selecter::buttonPushed()
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
        widget->setText(m_dbccache->get_map_resource()->at(selected));
    }
}

area_dialog::area_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Area");
    layout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    widget->setLayout(layout);
    list = new QListWidget{};
    list->addItems(*(m_dbccache->get_area_resource()));
    layout->addWidget(list);
    layout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setMinimumSize(200,400);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
}

area_dialog::~area_dialog(){ delete widget; }

QDialog * area_dialog::getWidget(){ return widget; }

void area_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::area)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_area_resource()));
    }
}

void area_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void area_dialog::acceptButtonTriggered()
{
    accept();
}
void area_dialog::cancelButtonTriggered()
{
    reject();
}
void area_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void area_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void area_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

area_selecter::area_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    selected = -1;
    widget->setText("Area...");
    dialog = new area_dialog{m_dbccache};
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

area_selecter::~area_selecter(){ delete widget; }

void area_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::area)
    {
        dialog->reload(r);
        selected = -1;
        widget->setText("Area...");
    }
}

QPushButton * area_selecter::getWidget() { return widget; }

void area_selecter::setIndexFromId(int id)
{
    if(id == 0)
    {
        selected = -1;
        widget->setText("Area...");
        return;
    }
    selected = dbccache::find_ref(m_dbccache->get_area_references(), id);
    if(selected != -1)
    {
        widget->setText(m_dbccache->get_area_resource()->at(selected));
    }
}

int area_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbccache->get_area_references()->at(selected);
}

void area_selecter::clear()
{
    selected = -1;
    widget->setText("Area...");
}

void area_selecter::buttonPushed()
{
    dialog->setIndex(selected);
    int i = dialog->getWidget()->exec();
    if(i == -1)
    {
    }
    else
    {
        selected = i-1;
        widget->setText(m_dbccache->get_area_resource()->at(selected));
    }
}

creature_rank_selecter::creature_rank_selecter()
{
    widget = new QComboBox();
    widget->addItem("Rank...");
    widget->addItem("Normal");
    widget->addItem("Rare");
    widget->addItem("Elite");
    widget->addItem("Rare-Elite");
    widget->addItem("Boss");
}

creature_rank_selecter::~creature_rank_selecter(){ delete widget; }

QComboBox * creature_rank_selecter::getWidget() { return widget; }

void creature_rank_selecter::clear(){ widget->setCurrentIndex(0); }

int creature_rank_selecter::selectedId()
{
    return indexToRank(widget->currentIndex());
}

void creature_rank_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(rankToIndex(id));
}

inhabit_type_selecter::inhabit_type_selecter()
{
    widget = new QWidget();
    widget->setLayout((layout = new QHBoxLayout{}));
    layout->addWidget((ground = new QCheckBox("Ground:")));
    ground->setLayoutDirection(Qt::RightToLeft);
    layout->addWidget((water = new QCheckBox("Water:")));
    water->setLayoutDirection(Qt::RightToLeft);
    layout->addWidget((air = new QCheckBox("Air:")));
    air->setLayoutDirection(Qt::RightToLeft);
    clear();
}

inhabit_type_selecter::~inhabit_type_selecter(){ delete widget; }

QWidget * inhabit_type_selecter::getWidget() { return widget; }

int inhabit_type_selecter::selectedId()
{
    return (INHABIT_GROUND*ground->isChecked()) |
           (INHABIT_WATER*water->isChecked())   |
           (INHABIT_AIR*air->isChecked());
}

void inhabit_type_selecter::setIndexFromId(int id)
{
    ground->setChecked(INHABIT_GROUND & id);
    water->setChecked(INHABIT_WATER & id);
    air->setChecked(INHABIT_AIR & id);
}

void inhabit_type_selecter::clear()
{
    ground->setChecked(true);
    water->setChecked(true);
    air->setChecked(true);
}

flags_extra_selecter::flags_extra_selecter(bool hasEnableOption) :
    QObject(),
    hasEnableOption(hasEnableOption)
{
    widget = new QWidget();
    widget->setLayout((outer_layout = new QHBoxLayout{}));
    outer_layout->addLayout((inner_left_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_right_layout = new QVBoxLayout{}));

    if(hasEnableOption)
    {
        inner_left_layout->addWidget((enable = new QCheckBox{}));
        enable->setText("Enable:");
        enable->setLayoutDirection(Qt::RightToLeft);

        QObject::connect(enable, SIGNAL(stateChanged(int)), this, SLOT(toggleEnabled(int)));
    }

    inner_left_layout->addWidget((bindsToInstance = new QCheckBox{}));
    bindsToInstance->setText("Binds to instance on kill:");
    bindsToInstance->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((noAggroRadius = new QCheckBox{}));
    noAggroRadius->setText("No aggro radius:");
    noAggroRadius->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((noParryHaste = new QCheckBox{}));
    noParryHaste->setText("Parry haste disabled:");
    noParryHaste->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((cantParry = new QCheckBox{}));
    cantParry->setText("Parry disabled:");
    cantParry->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((cantCrush = new QCheckBox{}));
    cantCrush->setText("Crushing blows disabled:");
    cantCrush->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((cantBlock = new QCheckBox{}));
    cantBlock->setText("Blocking disabled:");
    cantBlock->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((noXpReward = new QCheckBox{}));
    noXpReward->setText("Doesn't award XP:");
    noXpReward->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isInvisible = new QCheckBox{}));
    isInvisible->setText("Is invisible:");
    isInvisible->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((zoneAggro = new QCheckBox{}));
    zoneAggro->setText("Engages zone in combat:");
    zoneAggro->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isGuard = new QCheckBox{}));
    isGuard->setText("Is guard:");
    isGuard->setLayoutDirection(Qt::RightToLeft);
}

flags_extra_selecter::~flags_extra_selecter(){ delete widget; }

QWidget * flags_extra_selecter::getWidget(){ return widget; }

void flags_extra_selecter::clear()
{
    enable->setChecked(false);
    toggleEnabled(0);
}

bool flags_extra_selecter::selectedIsUsed() { return enable->isChecked(); }

int flags_extra_selecter::selectedId()
{
    return (bindsToInstance->isChecked() *CREATURE_FLAG_EXTRA_INSTANCE_BIND) |
           (noAggroRadius->isChecked()   *CREATURE_FLAG_EXTRA_CIVILIAN)      |
           (cantParry->isChecked()       *CREATURE_FLAG_EXTRA_NO_PARRY)      |
           (noParryHaste->isChecked()    *CREATURE_FLAG_EXTRA_NO_PARRY_HASTEN)|
           (cantBlock->isChecked()       *CREATURE_FLAG_EXTRA_NO_BLOCK)      |
           (cantCrush->isChecked()       *CREATURE_FLAG_EXTRA_NO_CRUSH)      |
           (noXpReward->isChecked()      *CREATURE_FLAG_EXTRA_NO_XP_AT_KILL) |
           (isInvisible->isChecked()     *CREATURE_FLAG_EXTRA_INVISIBLE)     |
           (zoneAggro->isChecked()       *CREATURE_FLAG_EXTRA_AGGRO_ZONE)    |
           (isGuard->isChecked()         *CREATURE_FLAG_EXTRA_GUARD);
}

void flags_extra_selecter::setIndexFromId(int id)
{
    bindsToInstance->setChecked(CREATURE_FLAG_EXTRA_INSTANCE_BIND & id);
    noAggroRadius->setChecked(CREATURE_FLAG_EXTRA_CIVILIAN & id);
    cantParry->setChecked(CREATURE_FLAG_EXTRA_NO_PARRY & id);
    noParryHaste->setChecked(CREATURE_FLAG_EXTRA_NO_PARRY_HASTEN & id);
    cantBlock->setChecked(CREATURE_FLAG_EXTRA_NO_BLOCK & id);
    cantCrush->setChecked(CREATURE_FLAG_EXTRA_NO_CRUSH & id);
    noXpReward->setChecked(CREATURE_FLAG_EXTRA_NO_XP_AT_KILL & id);
    isInvisible->setChecked(CREATURE_FLAG_EXTRA_INVISIBLE & id);
    zoneAggro->setChecked(CREATURE_FLAG_EXTRA_AGGRO_ZONE & id);
    isGuard->setChecked(CREATURE_FLAG_EXTRA_GUARD & id);
}

void flags_extra_selecter::toggleEnabled(int e)
{
    bindsToInstance->setEnabled(e != 0);
    noAggroRadius->setEnabled(e != 0);
    cantParry->setEnabled(e != 0);
    noParryHaste->setEnabled(e != 0);
    cantBlock->setEnabled(e != 0);
    cantCrush->setEnabled(e != 0);
    noXpReward->setEnabled(e != 0);
    isInvisible->setEnabled(e != 0);
    zoneAggro->setEnabled(e != 0);
    isGuard->setEnabled(e != 0);
}

npc_flags_selecter::npc_flags_selecter()
{
    widget = new QWidget();
    widget->setLayout((outer_layout = new QHBoxLayout{}));
    outer_layout->addLayout((inner_left_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_right_layout = new QVBoxLayout{}));

    inner_left_layout->addWidget((hasGossip = new QCheckBox{}));
    hasGossip->setText("Has Gossip:");
    hasGossip->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isQuestgiver = new QCheckBox{}));
    isQuestgiver->setText("Questgiver:");
    isQuestgiver->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isTrainer = new QCheckBox{}));
    isTrainer->setText("Trainer:");
    isTrainer->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isClassTrainer = new QCheckBox{}));
    isClassTrainer->setText("Class Trainer:");
    isClassTrainer->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isProfessionTrainer = new QCheckBox{}));
    isProfessionTrainer->setText("Profession Trainer:");
    isProfessionTrainer->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isVendor = new QCheckBox{}));
    isVendor->setText("Vendor:");
    isVendor->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isAmmoVendor = new QCheckBox{}));
    isAmmoVendor->setText("Ammo Vendor:");
    isAmmoVendor->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isFoodVendor = new QCheckBox{}));
    isFoodVendor->setText("Food Vendor:");
    isFoodVendor->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isPoisonVendor = new QCheckBox{}));
    isPoisonVendor->setText("Poison Vendor:");
    isPoisonVendor->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isReagentVendor = new QCheckBox{}));
    isReagentVendor->setText("Reagent Vendor:");
    isReagentVendor->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isRepairer = new QCheckBox{}));
    isRepairer->setText("Can Repair:");
    isRepairer->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isFlightmaster = new QCheckBox{}));
    isFlightmaster->setText("Flightmaster:");
    isFlightmaster->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isSpiritHealer = new QCheckBox{}));
    isSpiritHealer->setText("Spirit Healer:");
    isSpiritHealer->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isSpiritGuide = new QCheckBox{}));
    isSpiritGuide->setText("Spirit Guide:");
    isSpiritGuide->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isInnkeeper = new QCheckBox{}));
    isInnkeeper->setText("Innkeeper:");
    isInnkeeper->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isBanker = new QCheckBox{}));
    isBanker->setText("Banker:");
    isBanker->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isTabardDesigner = new QCheckBox{}));
    isTabardDesigner->setText("Tabard Designer:");
    isTabardDesigner->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isBattlemaster = new QCheckBox{}));
    isBattlemaster->setText("Battlemaster:");
    isBattlemaster->setLayoutDirection(Qt::RightToLeft);

    inner_left_layout->addWidget((isAuctioneer = new QCheckBox{}));
    isAuctioneer->setText("Auctioneer:");
    isAuctioneer->setLayoutDirection(Qt::RightToLeft);

    inner_right_layout->addWidget((isStablemaster = new QCheckBox{}));
    isStablemaster->setText("Stablemaster:");
    isStablemaster->setLayoutDirection(Qt::RightToLeft);
}

npc_flags_selecter::~npc_flags_selecter(){ delete widget; }

QWidget * npc_flags_selecter::getWidget(){ return widget; }

void npc_flags_selecter::clear()
{
    hasGossip->setChecked(false);
    isQuestgiver->setChecked(false);
    isTrainer->setChecked(false);
    isClassTrainer->setChecked(false);
    isProfessionTrainer->setChecked(false);
    isVendor->setChecked(false);
    isAmmoVendor->setChecked(false);
    isFoodVendor->setChecked(false);
    isPoisonVendor->setChecked(false);
    isReagentVendor->setChecked(false);
    isRepairer->setChecked(false);
    isFlightmaster->setChecked(false);
    isSpiritHealer->setChecked(false);
    isSpiritGuide->setChecked(false);
    isInnkeeper->setChecked(false);
    isBanker->setChecked(false);
    isTabardDesigner->setChecked(false);
    isBattlemaster->setChecked(false);
    isAuctioneer->setChecked(false);
    isStablemaster->setChecked(false);
}

int npc_flags_selecter::selectedId()
{
    return  (hasGossip->isChecked()                 *UNIT_NPC_FLAG_GOSSIP)             |
            (isQuestgiver->isChecked()              *UNIT_NPC_FLAG_QUESTGIVER)         |
            (isTrainer->isChecked()                 *UNIT_NPC_FLAG_TRAINER)            |
            (isClassTrainer->isChecked()            *UNIT_NPC_FLAG_TRAINER_CLASS)      |
            (isProfessionTrainer->isChecked()       *UNIT_NPC_FLAG_TRAINER_PROFESSION) |
            (isVendor->isChecked()                  *UNIT_NPC_FLAG_VENDOR)             |
            (isAmmoVendor->isChecked()              *UNIT_NPC_FLAG_VENDOR_AMMO)        |
            (isFoodVendor->isChecked()              *UNIT_NPC_FLAG_VENDOR_FOOD)        |
            (isPoisonVendor->isChecked()            *UNIT_NPC_FLAG_VENDOR_POISON)      |
            (isReagentVendor->isChecked()           *UNIT_NPC_FLAG_VENDOR_REAGENT)     |
            (isRepairer->isChecked()                *UNIT_NPC_FLAG_REPAIR)             |
            (isFlightmaster->isChecked()            *UNIT_NPC_FLAG_FLIGHTMASTER)       |
            (isSpiritHealer->isChecked()            *UNIT_NPC_FLAG_SPIRITHEALER)       |
            (isSpiritGuide->isChecked()             *UNIT_NPC_FLAG_SPIRITGUIDE)        |
            (isInnkeeper->isChecked()               *UNIT_NPC_FLAG_INNKEEPER)          |
            (isBanker->isChecked()                  *UNIT_NPC_FLAG_BANKER)             |
            (isTabardDesigner->isChecked()          *UNIT_NPC_FLAG_TABARDDESIGNER)     |
            (isBattlemaster->isChecked()            *UNIT_NPC_FLAG_BATTLEMASTER)       |
            (isAuctioneer->isChecked()              *UNIT_NPC_FLAG_AUCTIONEER)         |
            (isStablemaster->isChecked()            *UNIT_NPC_FLAG_STABLEMASTER);
}

void npc_flags_selecter::setIndexFromId(int id)
{
    hasGossip->setChecked(UNIT_NPC_FLAG_GOSSIP & id);
    isQuestgiver->setChecked(UNIT_NPC_FLAG_QUESTGIVER & id);
    isTrainer->setChecked(UNIT_NPC_FLAG_TRAINER & id);
    isClassTrainer->setChecked(UNIT_NPC_FLAG_TRAINER_CLASS & id);
    isProfessionTrainer->setChecked(UNIT_NPC_FLAG_TRAINER_PROFESSION & id);
    isVendor->setChecked(UNIT_NPC_FLAG_VENDOR & id);
    isAmmoVendor->setChecked(UNIT_NPC_FLAG_VENDOR_AMMO & id);
    isFoodVendor->setChecked(UNIT_NPC_FLAG_VENDOR_FOOD & id);
    isPoisonVendor->setChecked(UNIT_NPC_FLAG_VENDOR_POISON & id);
    isReagentVendor->setChecked(UNIT_NPC_FLAG_VENDOR_REAGENT & id);
    isRepairer->setChecked(UNIT_NPC_FLAG_REPAIR & id);
    isFlightmaster->setChecked(UNIT_NPC_FLAG_FLIGHTMASTER & id);
    isSpiritHealer->setChecked(UNIT_NPC_FLAG_SPIRITHEALER & id);
    isSpiritGuide->setChecked(UNIT_NPC_FLAG_SPIRITGUIDE & id);
    isInnkeeper->setChecked(UNIT_NPC_FLAG_INNKEEPER & id);
    isBanker->setChecked(UNIT_NPC_FLAG_BANKER & id);
    isTabardDesigner->setChecked(UNIT_NPC_FLAG_TABARDDESIGNER & id);
    isBattlemaster->setChecked(UNIT_NPC_FLAG_BATTLEMASTER & id);
    isAuctioneer->setChecked(UNIT_NPC_FLAG_AUCTIONEER & id);
    isStablemaster->setChecked(UNIT_NPC_FLAG_STABLEMASTER & id);
}

int trainer_type_selecter::indexToId(int index)
{
    if(index == 0){ return 0; }
    return index-1;
}
int trainer_type_selecter::idToIndex(int id)
{
    return id+1;
}

trainer_type_selecter::trainer_type_selecter()
{
    widget = new QComboBox();
    widget->addItem("Type...");
    widget->addItem("Class Trainer");
    widget->addItem("Mounts Trainer");
    widget->addItem("Profession Trainer");
    widget->addItem("Pet Trainer");
}

trainer_type_selecter::~trainer_type_selecter(){ delete widget; }

QComboBox * trainer_type_selecter::getWidget(){ return widget; }

int trainer_type_selecter::selectedId()
{
    return indexToId(widget->currentIndex());
}

void trainer_type_selecter::clear()
{
    widget->setCurrentIndex(0);
}

void trainer_type_selecter::setIndexFromId(int id)
{
    idToIndex(id);
}

trainer_race_selecter::trainer_race_selecter()
{
    widget = new QComboBox();
    widget->addItem("Trainer Race...");
    widget->addItem("Human");
    widget->addItem("Orc");
    widget->addItem("Dwarf");
    widget->addItem("Night Elf");
    widget->addItem("Forsaken");
    widget->addItem("Tauren");
    widget->addItem("Gnome");
    widget->addItem("Troll");
    widget->addItem("Blood Elf");
    widget->addItem("Draenei");
}

trainer_race_selecter::~trainer_race_selecter(){ delete widget; }

QComboBox * trainer_race_selecter::getWidget(){ return widget; }

void trainer_race_selecter::clear() { widget->setCurrentIndex(0); }

int trainer_race_selecter::selectedId()
{
    switch(widget->currentIndex())
    {
    case 0:return 0;
    case 1:return 1;
    case 2:return 2;
    case 3:return 3;
    case 4:return 4;
    case 5:return 5;
    case 6:return 6;
    case 7:return 7;
    case 8:return 8;
    case 9:return 10;
    case 10:return 11;
    }
    return 0;
}

void trainer_race_selecter::setIndexFromId(int id)
{
    switch(id)
    {
    case 1: widget->setCurrentIndex(1); break;
    case 2: widget->setCurrentIndex(2); break;
    case 3: widget->setCurrentIndex(3); break;
    case 4: widget->setCurrentIndex(4); break;
    case 5: widget->setCurrentIndex(5); break;
    case 6: widget->setCurrentIndex(6); break;
    case 7: widget->setCurrentIndex(7); break;
    case 8: widget->setCurrentIndex(8); break;
    case 10: widget->setCurrentIndex(9); break;
    case 11: widget->setCurrentIndex(10); break;
    default: widget->setCurrentIndex(0); break;
    }
    widget->setCurrentIndex(0);
}

trainer_class_selecter::trainer_class_selecter()
{
    widget = new QComboBox();
    widget->addItem("Trainer Class...");
    widget->addItem("Warrior");
    widget->addItem("Paladin");
    widget->addItem("Hunter");
    widget->addItem("Rogue");
    widget->addItem("Priest");
    widget->addItem("Shaman");
    widget->addItem("Mage");
    widget->addItem("Warlock");
    widget->addItem("Druid");
}

trainer_class_selecter::~trainer_class_selecter(){ delete widget; }

QComboBox * trainer_class_selecter::getWidget(){ return widget; }

void trainer_class_selecter::clear() { widget->setCurrentIndex(0); }

int trainer_class_selecter::selectedId()
{
    switch(widget->currentIndex())
    {
    case 0:return 0;
    case 1:return 1;
    case 2:return 2;
    case 3:return 3;
    case 4:return 4;
    case 5:return 5;
    case 6:return 7;
    case 7:return 8;
    case 8:return 9;
    case 9:return 11;
    }
    return 0;
}

void trainer_class_selecter::setIndexFromId(int id)
{
    switch(id)
    {
    case 1: widget->setCurrentIndex(1); break;
    case 2: widget->setCurrentIndex(2); break;
    case 3: widget->setCurrentIndex(3); break;
    case 4: widget->setCurrentIndex(4); break;
    case 5: widget->setCurrentIndex(5); break;
    case 7: widget->setCurrentIndex(6); break;
    case 8: widget->setCurrentIndex(7); break;
    case 9: widget->setCurrentIndex(8); break;
    case 11: widget->setCurrentIndex(9); break;
    default: widget->setCurrentIndex(0); break;
    }
}

expansion_selecter::expansion_selecter()
{
    widget = new QComboBox();
    widget->addItem("Expansion...");
    widget->addItem("None");
    widget->addItem("Azeroth");
    widget->addItem("Outland");
}
expansion_selecter::~expansion_selecter(){ delete widget; }
QComboBox * expansion_selecter::getWidget(){ return widget; }
void expansion_selecter::clear() { widget->setCurrentIndex(0); }
void expansion_selecter::setIndexFromId(int id)
{
    switch (id)
    {
    case -1: widget->setCurrentIndex(1); return;
    case 0: widget->setCurrentIndex(2); return;
    case 1: widget->setCurrentIndex(3); return;
    }
    widget->setCurrentIndex(1);
}
int expansion_selecter::selectedId()
{
    switch(widget->currentIndex())
    {
    case 0: return -1;
    case 1: return -1;
    case 2: return 0;
    case 3: return 1;
    }
    return -1;
}

movement_type_selecter::movement_type_selecter()
{
    widget = new QComboBox();
    widget->addItem("Movement Type...");
    widget->addItem("Idle Movement");
    widget->addItem("Random Movement");
    widget->addItem("Waypoint Movement");
}

movement_type_selecter::~movement_type_selecter(){ delete widget; }

QComboBox * movement_type_selecter::getWidget(){ return widget; }

void movement_type_selecter::clear() { widget->setCurrentIndex(0); }
void movement_type_selecter::setIndexFromId(int id)
{
    switch(id)
    {
    case 0: widget->setCurrentIndex(1); break;
    case 1: widget->setCurrentIndex(2); break;
    case 2: widget->setCurrentIndex(3); break;
    default: widget->setCurrentIndex(0); break;
    }
}

int movement_type_selecter::selectedId()
{
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    return widget->currentIndex()-1;
}

mechanics_mask_selecter::mechanics_mask_selecter(bool hasEnableOption) :
    QObject(),
    hasEnableOption(hasEnableOption)
{
    widget = new QWidget();
    widget->setLayout((outer_layout = new QHBoxLayout{}));
    outer_layout->addLayout((inner_left_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_middle_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_right_layout = new QVBoxLayout{}));

    if(hasEnableOption)
    {
        inner_left_layout->addWidget((enable = new QCheckBox{}));
        enable->setLayoutDirection(Qt::RightToLeft);
        enable->setText("Enable:");

        QObject::connect(enable, SIGNAL(stateChanged(int)), this, SLOT(toggleEnabled(int)));
    }

    for(int i = 0; i < 30; i++)
    {
        if(i%3 == 0){ inner_left_layout->addWidget((box[i] = new QCheckBox{})); }
        if(i%3 == 1){ inner_middle_layout->addWidget((box[i] = new QCheckBox{})); }
        if(i%3 == 2){ inner_right_layout->addWidget((box[i] = new QCheckBox{})); }
        box[i]->setLayoutDirection(Qt::RightToLeft);
    }
    box[0]->setText("Charm:");
    box[1]->setText("Disorientation:");
    box[2]->setText("Disarm:");
    box[3]->setText("Distract:");
    box[4]->setText("Fear:");
    box[5]->setText("Fumble:");
    box[6]->setText("Root:");
    box[7]->setText("Pacify:");
    box[8]->setText("Silence:");
    box[9]->setText("Sleep:");
    box[10]->setText("Snare:");
    box[11]->setText("Stun:");
    box[12]->setText("Freeze:");
    box[13]->setText("Knockout:");
    box[14]->setText("Bleed:");
    box[15]->setText("Bandage:");
    box[16]->setText("Polymorph:");
    box[17]->setText("Banish:");
    box[18]->setText("Shield:");
    box[19]->setText("Shackle:");
    box[20]->setText("Mount:");
    box[21]->setText("Persuade:");
    box[22]->setText("Turn:");
    box[23]->setText("Horror:");
    box[24]->setText("Invulnerability:");
    box[25]->setText("Interupt:");
    box[26]->setText("Daze:");
    box[27]->setText("Discovery:");
    box[28]->setText("Immune Shield:");
    box[29]->setText("Sap:");
}

mechanics_mask_selecter::~mechanics_mask_selecter(){ delete widget; }

QWidget * mechanics_mask_selecter::getWidget(){ return widget; }

bool mechanics_mask_selecter::selectedIsUsed(){ return enable->isChecked(); }

int mechanics_mask_selecter::selectedId()
{
    uint32_t mask = 0;
    for(int i = 0; i < 30; i++)
    {
        mask |= (box[i]->isChecked()) * (1 << i);
    }
    return mask;
}

void mechanics_mask_selecter::clear()
{
    enable->setChecked(false);
    toggleEnabled(0);
}

void mechanics_mask_selecter::setIndexFromId(int id)
{
    for(int i = 0; i < 30; i++)
    {
        box[i]->setChecked(((1 << i) & id) != 0);
    }
}

void mechanics_mask_selecter::toggleEnabled(int e)
{
    for(int i = 0; i < 30; i++)
    {
        box[i]->setEnabled(e != 0);
    }
}

creature_class_selecter::creature_class_selecter()
{
    widget = new QComboBox();
    widget->addItem("Class...");
    widget->addItem("Unspecified");
    widget->addItem("Warrior");
    widget->addItem("Rogue");
    widget->addItem("Paladin");
    widget->addItem("Mage");
}

creature_class_selecter::~creature_class_selecter(){ delete widget; }

QComboBox * creature_class_selecter::getWidget(){ return widget; }

int creature_class_selecter::selectedId()
{
    return indexToId(widget->currentIndex());
}
void creature_class_selecter::clear()
{
    widget->setCurrentIndex(0);
}
void creature_class_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(idToIndex(id));
}

unit_dynamic_flags_selecter::unit_dynamic_flags_selecter()
{
    widget = new QWidget();
    widget->setLayout((outer_layout = new QHBoxLayout{}));
    outer_layout->addLayout((inner_left_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_right_layout = new QVBoxLayout{}));
    for(int i = 0; i < 6; i++)
    {
        if(i%2 == 0){ inner_left_layout->addWidget((box[i] = new QCheckBox{})); }
        if(i%2 == 1){ inner_right_layout->addWidget((box[i] = new QCheckBox{})); }
        box[i]->setLayoutDirection(Qt::RightToLeft);
    }
    box[0]->setText("Lootable:");
    box[1]->setText("Track Unit:");
    box[2]->setText("Tapped:");
    box[3]->setText("Rooted:");
    box[4]->setText("Special Info:");
    box[5]->setText("Dead:");
}

unit_dynamic_flags_selecter::~unit_dynamic_flags_selecter(){ delete widget; }

QWidget * unit_dynamic_flags_selecter::getWidget(){ return widget; }

void unit_dynamic_flags_selecter::clear()
{
    for(int i = 0; i < 6; i++)
    {
        box[i]->setChecked(false);
    }
}

int unit_dynamic_flags_selecter::selectedId()
{
    uint32_t r = 0;
    for(int i = 0; i < 6; i++)
    {
        r |= (box[i]->isChecked() << i);
    }
    return r;
}

void unit_dynamic_flags_selecter::setIndexFromId(int id)
{
    for(int i = 0; i < 6; i++)
    {
        box[i]->setChecked(((1 << i) & id) != 0);
    }
}


unit_flags_selecter::unit_flags_selecter()
{
    widget = new QWidget();
    widget->setLayout((outer_layout = new QHBoxLayout{}));
    outer_layout->addLayout((inner_left_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_middle_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_right_layout = new QVBoxLayout{}));

    for(int i = 0; i < 22; i++)
    {
        if(i%3 == 0){ inner_left_layout->addWidget((box[i] = new QCheckBox{}));}
        if(i%3 == 1){ inner_middle_layout->addWidget((box[i] = new QCheckBox{}));}
        if(i%3 == 2){ inner_right_layout->addWidget((box[i] = new QCheckBox{}));}
        box[i]->setLayoutDirection(Qt::RightToLeft);
    }
    box[0]->setText("Not Attackable:");
    box[1]->setText("Disable Move:");
    box[2]->setText("Rename:");
    box[3]->setText("Preparation:");
    box[4]->setText("Not Attackable OOC:");
    box[5]->setText("Passive:");
    box[6]->setText("Loot Animation:");
    box[7]->setText("Pet in Combat:");
    box[8]->setText("PvP:");
    box[9]->setText("Silenced:");
    box[10]->setText("Pacified:");
    box[11]->setText("Stunned:");
    box[12]->setText("In Combat:");
    box[13]->setText("On Taxi");
    box[14]->setText("Disarmed:");
    box[15]->setText("Confused:");
    box[16]->setText("Fleeing:");
    box[17]->setText("Player Controlled:");
    box[18]->setText("Not Selectable:");
    box[19]->setText("Skinnable:");
    box[20]->setText("Mount:");
    box[21]->setText("Sheathe:");
}

unit_flags_selecter::~unit_flags_selecter(){ delete widget; }

QWidget * unit_flags_selecter::getWidget(){ return widget; }

void unit_flags_selecter::clear()
{
    for(int i = 0; i < 22; i++)
    {
        box[i]->setChecked(false);
    }
}

int unit_flags_selecter::selectedId()
{
    return  (box[0]->isChecked()    *UNIT_FLAG_NON_ATTACKABLE)      |
            (box[1]->isChecked()    *UNIT_FLAG_DISABLE_MOVE)        |
            (box[2]->isChecked()    *UNIT_FLAG_RENAME)              |
            (box[3]->isChecked()    *UNIT_FLAG_PREPARATION)         |
            (box[4]->isChecked()    *UNIT_FLAG_OOC_NOT_ATTACKABLE)  |
            (box[5]->isChecked()    *UNIT_FLAG_PASSIVE)             |
            (box[6]->isChecked()    *UNIT_FLAG_LOOTING)             |
            (box[7]->isChecked()    *UNIT_FLAG_PET_IN_COMBAT)       |
            (box[8]->isChecked()    *UNIT_FLAG_PVP)                 |
            (box[9]->isChecked()    *UNIT_FLAG_SILENCED)            |
            (box[10]->isChecked()   *UNIT_FLAG_PACIFIED)            |
            (box[11]->isChecked()   *UNIT_FLAG_STUNNED)             |
            (box[12]->isChecked()   *UNIT_FLAG_IN_COMBAT)           |
            (box[13]->isChecked()   *UNIT_FLAG_TAXI_FLIGHT)         |
            (box[14]->isChecked()   *UNIT_FLAG_DISARMED)            |
            (box[15]->isChecked()   *UNIT_FLAG_CONFUSED)            |
            (box[16]->isChecked()   *UNIT_FLAG_FLEEING)             |
            (box[17]->isChecked()   *UNIT_FLAG_PLAYER_CONTROLLED)   |
            (box[18]->isChecked()   *UNIT_FLAG_NOT_SELECTABLE)      |
            (box[19]->isChecked()   *UNIT_FLAG_SKINNABLE)           |
            (box[20]->isChecked()   *UNIT_FLAG_MOUNT)               |
            (box[21]->isChecked()   *UNIT_FLAG_SHEATHE);
}

void unit_flags_selecter::setIndexFromId(int id)
{
    box[0]->setChecked((UNIT_FLAG_NON_ATTACKABLE & id) != 0);
    box[1]->setChecked((UNIT_FLAG_DISABLE_MOVE & id) != 0);
    box[2]->setChecked((UNIT_FLAG_RENAME & id) != 0);
    box[3]->setChecked((UNIT_FLAG_PREPARATION & id) != 0);
    box[4]->setChecked((UNIT_FLAG_OOC_NOT_ATTACKABLE & id) != 0);
    box[5]->setChecked((UNIT_FLAG_PASSIVE & id) != 0);
    box[6]->setChecked((UNIT_FLAG_LOOTING & id) != 0);
    box[7]->setChecked((UNIT_FLAG_PET_IN_COMBAT & id) != 0);
    box[8]->setChecked((UNIT_FLAG_PVP & id) != 0);
    box[9]->setChecked((UNIT_FLAG_SILENCED & id) != 0);
    box[10]->setChecked((UNIT_FLAG_PACIFIED & id) != 0);
    box[11]->setChecked((UNIT_FLAG_STUNNED & id) != 0);
    box[12]->setChecked((UNIT_FLAG_IN_COMBAT & id) != 0);
    box[13]->setChecked((UNIT_FLAG_TAXI_FLIGHT & id) != 0);
    box[14]->setChecked((UNIT_FLAG_DISARMED & id) != 0);
    box[15]->setChecked((UNIT_FLAG_CONFUSED & id) != 0);
    box[16]->setChecked((UNIT_FLAG_FLEEING & id) != 0);
    box[17]->setChecked((UNIT_FLAG_PLAYER_CONTROLLED & id) != 0);
    box[18]->setChecked((UNIT_FLAG_NOT_SELECTABLE & id) != 0);
    box[19]->setChecked((UNIT_FLAG_SKINNABLE & id) != 0);
    box[20]->setChecked((UNIT_FLAG_MOUNT & id) != 0);
    box[21]->setChecked((UNIT_FLAG_SHEATHE & id) != 0);
}

model_dialog::model_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Model");
    layout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    widget->setLayout(layout);
    list = new QListWidget{};
    list->addItems(*(m_dbccache->get_model_resource()));
    layout->addWidget(list);
    layout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setMinimumSize(600,500);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
}

model_dialog::~model_dialog(){ delete widget; }

void model_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::model)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_model_resource()));
    }
}

QDialog * model_dialog::getWidget(){ return widget; }

void model_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void model_dialog::acceptButtonTriggered()
{
    accept();
}
void model_dialog::cancelButtonTriggered()
{
    reject();
}

void model_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void model_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void model_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

model_selecter::model_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    selected = -1;
    widget->setText("Model...");
    dialog = new model_dialog{m_dbccache};
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

model_selecter::~model_selecter(){}

void model_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::model)
    {
        dialog->reload(r);
        selected = -1;
        widget->setText("Model...");
    }
}

QPushButton * model_selecter::getWidget(){ return widget; }

void model_selecter::setIndexFromId(int id)
{
    selected = dbccache::find_ref(m_dbccache->get_model_references(),id);
    if(selected == -1)
    {
        widget->setText("Model...");
        return;
    }
    widget->setText(m_dbccache->get_model_plain()->at(selected));
}

int model_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbccache->get_model_references()->at(selected);
}

void model_selecter::clear()
{
    selected = -1;
    widget->setText("Model...");
}

void model_selecter::buttonPushed()
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
        widget->setText(m_dbccache->get_model_plain()->at(selected));
    }
}

spell_dialog::spell_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Spell");
    layout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    tlayout = new QHBoxLayout{};
    widget->setLayout(layout);
    layout->addLayout(tlayout);
    tlayout->addWidget((filter = new QLabel("Filter:")));
    tlayout->addWidget((filterEdit = new QLineEdit()));
    QObject::connect(filterEdit,SIGNAL(textChanged(QString)),this,SLOT(filterTextChanged()));
    list = new QListWidget{};
    list->addItems(*(m_dbccache->get_spell_resource()));
    layout->addWidget(list);
    layout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setMinimumSize(600,500);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}

spell_dialog::~spell_dialog(){ delete widget; }

QDialog * spell_dialog::getWidget(){ return widget; }

void spell_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void spell_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::spell)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_spell_resource()));
        filterTextChanged();
    }
}

void spell_dialog::acceptButtonTriggered()
{
    accept();
}
void spell_dialog::cancelButtonTriggered()
{
    reject();
}

void spell_dialog::clearButtonTriggered()
{
    widget->setResult(-2);
    widget->done(widget->result());
}

void spell_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void spell_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

void spell_dialog::filterTextChanged()
{
    if(filterEdit->text().length() != 0)
    {
        for(int i = 0; i < list->count(); i++)
        {
            list->item(i)->setHidden(true);
        }
        QList<QListWidgetItem*> items = list->findItems(filterEdit->text(),Qt::MatchContains);
        for(int i = 0; i < items.count(); i++)
        {
            items[i]->setHidden(false);
        }
    }
    else
    {
        for(int i = 0; i < list->count(); i++)
        {
            list->item(i)->setHidden(false);
        }
    }
}

spell_selecter::spell_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    selected = -1;
    isclear = true;
    widget->setText("Spell...");
    dialog = new spell_dialog{m_dbccache};
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
}

spell_selecter::~spell_selecter(){}

void spell_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::spell)
    {
        selected = -1;
        widget->setText("Spell...");
        dialog->reload(r);
    }
}

QPushButton * spell_selecter::getWidget(){ return widget; }

void spell_selecter::setIndexFromId(int id)
{
    if(id == 0)
    {
        clear();
        return;
    }
    selected = dbccache::find_ref(m_dbccache->get_spell_references(), id);
    if(selected != -1)
    {
        widget->setText(m_dbccache->get_spell_resource()->at(selected));
    }
}

int spell_selecter::selectedId()
{
    if(!isclear && selected != -1)
    {
        int id = m_dbccache->get_spell_references()->at(selected);
        if(id == -1)
        {
            return 0;
        }
        return id;
    }
    else
    {
        return 0;
    }
}

void spell_selecter::clear(){ isclear = true; selected = -1; widget->setText("Spell..."); }

void spell_selecter::buttonPushed()
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
    else if( i == -2)
    {
        clear();
        emit wascleared();
    }
    else
    {
        isclear = false;
        selected = i-1;
        widget->setText(m_dbccache->get_spell_resource()->at(selected));
        emit wasselected();
    }
}


creature_type_flags_selecter::creature_type_flags_selecter()
{
    widget = new QWidget();
    widget->setLayout((outer_layout = new QHBoxLayout{}));
    outer_layout->addLayout((inner_left_layout = new QVBoxLayout{}));
    outer_layout->addLayout((inner_right_layout = new QVBoxLayout{}));

    for(int i = 0; i < 6; i++)
    {
        if(i%2 == 0){ inner_left_layout->addWidget((box[i] = new QCheckBox{})); }
        if(i%2 == 1){ inner_right_layout->addWidget((box[i] = new QCheckBox{})); }
        box[i]->setLayoutDirection(Qt::RightToLeft);
    }
    box[0]->setText("Tameable:");
    box[1]->setText("Can be seen by player ghosts:");
    box[2]->setText("Salvageable by herbalists:");
    box[3]->setText("Salvageable by miners:");
    box[4]->setText("Can assist players in combat:");
    box[5]->setText("Salvageable by engineers:");
}

creature_type_flags_selecter::~creature_type_flags_selecter(){ delete widget; }

QWidget * creature_type_flags_selecter::getWidget(){ return widget; }

void creature_type_flags_selecter::clear() { for(int i = 0; i < 6; i++){ box[i]->setChecked(false); } }

void creature_type_flags_selecter::setIndexFromId(int id)
{
    box[0]->setChecked((CREATURE_TYPEFLAGS_TAMEABLE & id) != 0);
    box[1]->setChecked((CREATURE_TYPEFLAGS_GHOST_VISIBLE & id) != 0);
    box[2]->setChecked((CREATURE_TYPEFLAGS_HERBLOOT & id) != 0);
    box[3]->setChecked((CREATURE_TYPEFLAGS_MININGLOOT & id) != 0);
    box[4]->setChecked((CREATURE_TYPEFLAGS_CAN_ASSIST & id) != 0);
    box[5]->setChecked((CREATURE_TYPEFLAGS_ENGINEERLOOT & id) != 0);
}

int creature_type_flags_selecter::selectedId()
{
    return (box[0]->isChecked() * CREATURE_TYPEFLAGS_TAMEABLE)          |
           (box[1]->isChecked() * CREATURE_TYPEFLAGS_GHOST_VISIBLE)     |
           (box[2]->isChecked() * CREATURE_TYPEFLAGS_HERBLOOT)          |
           (box[3]->isChecked() * CREATURE_TYPEFLAGS_MININGLOOT)        |
           (box[4]->isChecked() * CREATURE_TYPEFLAGS_CAN_ASSIST)        |
           (box[5]->isChecked() * CREATURE_TYPEFLAGS_ENGINEERLOOT);
}


standing_animations_selecter::standing_animations_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_standing_animation_resource()));
}

standing_animations_selecter::~standing_animations_selecter(){ }

QComboBox * standing_animations_selecter::getWidget(){ return widget; }

void standing_animations_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::standing_animation)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_standing_animation_resource()));
    }
}

void standing_animations_selecter::clear() { widget->setCurrentIndex(0); }

void standing_animations_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_standing_animation_references(),id);
    if(index == -1)
    {
        widget->setCurrentIndex(0);
        return;
    }
    widget->setCurrentIndex(index+1);
}

int standing_animations_selecter::selectedId()
{
    if(widget->currentIndex() != 0)
    {
        return m_dbccache->get_standing_animation_references()->at(widget->currentIndex()-1);
    }
    else
    {
        return 0;
    }
}

pet_spell_dialog::pet_spell_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setModal(true);
    widget->setWindowTitle("Pet Spell");
    layout = new QVBoxLayout{};
    blayout = new QHBoxLayout{};
    widget->setLayout(layout);
    list = new QListWidget{};
    list->addItems(*(m_dbccache->get_pet_spell_resource()));
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

pet_spell_dialog::~pet_spell_dialog(){}

void pet_spell_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::pet_spell)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_pet_spell_resource()));
    }
}

QDialog * pet_spell_dialog::getWidget(){ return widget; }

void pet_spell_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void pet_spell_dialog::acceptButtonTriggered()
{
    accept();
}
void pet_spell_dialog::cancelButtonTriggered()
{
    reject();
}
void pet_spell_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void pet_spell_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void pet_spell_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

pet_spell_selecter::pet_spell_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    selected = -1;
    widget->setText("Pet Spell...");
    dialog = new pet_spell_dialog{m_dbccache};
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

pet_spell_selecter::~pet_spell_selecter(){}

void pet_spell_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::pet_spell)
    {
        selected = -1;
        widget->setText("Pet Spell...");
        dialog->reload(r);
    }
}

QPushButton * pet_spell_selecter::getWidget(){ return widget; }

void pet_spell_selecter::setIndexFromId(int id)
{
    selected = dbccache::find_ref(m_dbccache->get_pet_spell_references(),id);
    if(selected == -1)
    {
        widget->setText("Pet Spell...");
        return;
    }
    widget->setText(m_dbccache->get_pet_spell_plain()->at(selected));
}

int pet_spell_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbccache->get_pet_spell_references()->at(selected);
}

void pet_spell_selecter::buttonPushed()
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
        widget->setText(m_dbccache->get_pet_spell_plain()->at(selected));
    }
}

void pet_spell_selecter::clear()
{
    selected = -1;
    widget->setText("Pet Spell...");
}

item_model_dialog::item_model_dialog(dbccache * c, int slot) :
    QObject(),
    m_dbccache(c),
    slot(slot)
{
    widget = new QDialog();
    widget->setWindowTitle("Item Model");
    widget->setLayout((layout = new QVBoxLayout()));

    layout->addWidget((list = new QListWidget()));
    if(slot == 0)
    {

    }
    else
    {
        list->addItems(*(m_dbccache->get_item_model_resource(slot-1)));
    }
    blayout = new QHBoxLayout();
    layout->addLayout(blayout);
    blayout->addWidget((acceptB = new QPushButton{"Accept"}));
    blayout->addWidget((cancelB = new QPushButton{"Cancel"}));
    blayout->addWidget((clearB = new QPushButton{"Clear"}));

    list->setMinimumSize(600,500);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(acceptButtonTriggered()));
    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(cancelButtonTriggered()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(clearButtonTriggered()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(acceptButtonTriggered()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}

item_model_dialog::~item_model_dialog(){}

void item_model_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::item_model)
    {
        list->clear();
        if(slot == 0)
        {

        }
        else
        {
            list->addItems(*(m_dbccache->get_item_model_resource(slot-1)));
        }
    }
}

QDialog * item_model_dialog::getWidget(){ return widget; }

void item_model_dialog::changeSlot(int slot)
{
    this->slot = slot;
    list->clear();
    if(slot == 0)
    {

    }
    else
    {
        list->addItems(*(m_dbccache->get_item_model_resource(slot-1)));
    }
}

void item_model_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
    list->item(index)->setSelected(true);
}

void item_model_dialog::acceptButtonTriggered()
{
    accept();
}
void item_model_dialog::cancelButtonTriggered()
{
    reject();
}
void item_model_dialog::clearButtonTriggered()
{
    emit clear();
    reject();
}

void item_model_dialog::accept()
{
    widget->setResult(list->currentIndex().row()+1);
    widget->done(widget->result());
}
void item_model_dialog::reject()
{
    widget->setResult(-1);
    widget->done(widget->result());
}


item_model_selecter::item_model_selecter(dbccache *c, int slot) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    selected = -1;
    widget->setText("Item Model...");
    if(slot == 0)
    {
        widget->setEnabled(false);
    }
    dialog = new item_model_dialog{m_dbccache,slot};
    currentSlot = slot;
    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

item_model_selecter::~item_model_selecter(){ }

QPushButton * item_model_selecter::getWidget(){ return widget; }

void item_model_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::item_model)
    {
        selected = -1;
        widget->setText("Item Model...");
        dialog->reload(r);
    }
}

void item_model_selecter::changeSlot(int slot)
{
    if(currentSlot == slot){ return; }
    else
    {
        if(slot == 0){ widget->setEnabled(false); }
        else         { widget->setEnabled(true); }
        currentSlot = slot;
        selected = -1;
        dialog->changeSlot(slot);
        dialog->setIndex(0);
        widget->setText("Item Model...");
    }
}

void item_model_selecter::setIndexFromId(int id)
{
    selected = dbccache::find_ref(m_dbccache->get_item_model_item_references(currentSlot-1), id);
    if(selected == -1)
    {
        widget->setText("Item Model...");
        return;
    }
    widget->setText(m_dbccache->get_item_model_plain(currentSlot-1)->at(selected));
}

void item_model_selecter::setIndexFromDisplayId(int id)
{
    selected = dbccache::find_ref(m_dbccache->get_item_model_display_references(currentSlot-1), id);
    if(selected == -1)
    {
        widget->setText("Item Model...");
        return;
    }
    widget->setText(m_dbccache->get_item_model_plain(currentSlot-1)->at(selected));
}

int item_model_selecter::selectedId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbccache->get_item_model_item_references(currentSlot-1)->at(selected);
}

int item_model_selecter::selectedDisplayId()
{
    if(selected == -1)
    {
        return 0;
    }
    return m_dbccache->get_item_model_display_references(currentSlot-1)->at(selected);
}

void item_model_selecter::clear()
{
    selected = -1;
    widget->setText("Item Model...");
}

void item_model_selecter::buttonPushed()
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
        widget->setText(m_dbccache->get_item_model_plain(currentSlot-1)->at(selected));
    }
}

item_class_selecter::item_class_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_item_class_resource()));
}
item_class_selecter::~item_class_selecter(){}

QComboBox * item_class_selecter::getWidget(){ return widget; }

void item_class_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::item_class)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_item_class_resource()));
    }
}

void item_class_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_item_class_references(), id);
    if(index == -1)
    {
        widget->setCurrentIndex(0);
        return;
    }
    widget->setCurrentIndex(index+1);
}

int item_class_selecter::selectedId()
{
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    return m_dbccache->get_item_class_references()->at(widget->currentIndex()-1);
}

bool item_class_selecter::isDefaultSelected()
{
    return widget->currentIndex() == 0;
}

void item_class_selecter::clear()
{
    widget->setCurrentIndex(0);
}

item_subclass_selecter::item_subclass_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItem("Item Subclass...");

    selectedCategory = -1;
}

item_subclass_selecter::~item_subclass_selecter(){}

QComboBox * item_subclass_selecter::getWidget(){ return widget; }

void item_subclass_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::item_subclass)
    {
        if(selectedCategory != -1)
        {
            widget->clear();
            widget->addItems(*(m_dbccache->get_item_subclass_resource()->at(selectedCategory)));
        }
    }
}

int item_subclass_selecter::selectedId()
{
    if(widget->currentIndex() <= 0)
    {
        return -1;
    }
    if(selectedCategory != -1 && widget->currentIndex())
    {
        return m_dbccache->get_item_subclass_references()->at(selectedCategory)->at(widget->currentIndex()-1);
    }
    else
    {
        return -1;
    }
}

bool item_subclass_selecter::isDefaultSelected()
{
    return widget->currentIndex() == 0;
}

void item_subclass_selecter::setIndexFromId(int id)
{
    if(selectedCategory != -1)
    {
        int index = dbccache::find_ref(m_dbccache->get_item_subclass_references()->at(selectedCategory),id);
        if(index != -1)
        {
            widget->setCurrentIndex(index+1);
        }
        else
        {
            widget->setCurrentIndex(0);
        }
    }
    else
    {
        widget->setCurrentIndex(0);
    }
}

void item_subclass_selecter::setCategory(int c)
{
    if(selectedCategory == c)
    {
        return;
    }
    selectedCategory = c;
    if(selectedCategory == -1)
    {
        widget->clear();
        widget->addItem("Item Subclass...");
    }
    else
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_item_subclass_resource()->at(selectedCategory)));
    }
}

void item_subclass_selecter::clear()
{
    setCategory(-1);
}

item_set_selecter::item_set_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_item_set_resource()));
}

item_set_selecter::~item_set_selecter(){}

void item_set_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::item_set)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_item_set_resource()));
    }
}

QComboBox * item_set_selecter::getWidget(){ return widget; }

void item_set_selecter::clear(){ widget->setCurrentIndex(0); }

void item_set_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_item_set_references(),id);
    if(index == -1)
    {
        widget->setCurrentIndex(0);
        return;
    }
    widget->setCurrentIndex(index+1);
}

int item_set_selecter::selectedId()
{
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    else
    {
        return m_dbccache->get_item_set_references()->at(widget->currentIndex()-1);
    }
}

item_quality_selecter::item_quality_selecter()
{
    widget = new QComboBox();
    widget->addItem("Item Quality...");
    widget->addItem("Poor");
    widget->addItem("Common");
    widget->addItem("Uncommon");
    widget->addItem("Rare");
    widget->addItem("Epic");
    widget->addItem("Legendary");
    widget->addItem("Artifact");
}

item_quality_selecter::~item_quality_selecter(){}

QComboBox * item_quality_selecter::getWidget() { return widget; }

void item_quality_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(id+1);
}

int item_quality_selecter::selectedId()
{
    // If none is selected, return 0 (poor quality)
    if(widget->currentIndex()==0)
    {
        return 0;
    }
    return widget->currentIndex()-1;
}

bool item_quality_selecter::isDefaultSelected()
{
    return widget->currentIndex() == 0;
}

void item_quality_selecter::clear()
{
    widget->setCurrentIndex(0);
}

item_slot_selecter::item_slot_selecter()
{
    widget = new QComboBox();
    widget->addItem("Slot...");
    widget->addItem("Not Equipable");
    widget->addItem("Head");
    widget->addItem("Neck");
    widget->addItem("Shoulder");
    widget->addItem("Shirt");
    widget->addItem("Chest");
    widget->addItem("Waist");
    widget->addItem("Legs");
    widget->addItem("Feet");
    widget->addItem("Wrists");
    widget->addItem("Hands");
    widget->addItem("Finger");
    widget->addItem("Trinket");
    widget->addItem("Weapon");
    widget->addItem("Shield");
    widget->addItem("Ranged");
    widget->addItem("Back");
    widget->addItem("Two-Hand");
    widget->addItem("Bag");
    widget->addItem("Tabard");
    widget->addItem("Robe");
    widget->addItem("Main Hand");
    widget->addItem("Off Hand");
    widget->addItem("Holdable (Tome)");
    widget->addItem("Ammo");
    widget->addItem("Thrown");
    widget->addItem("Ranged Right");
    widget->addItem("Quiver");
    widget->addItem("Relic");
}

item_slot_selecter::~item_slot_selecter(){}

QComboBox * item_slot_selecter::getWidget(){ return widget; }

void item_slot_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(id+1);
}

int item_slot_selecter::selectedId()
{
    // default return 0 (not equipable)
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    return widget->currentIndex()-1;
}

bool item_slot_selecter::isDefaultSelected()
{
    return widget->currentIndex() == 0;
}

void item_slot_selecter::clear()
{
    widget->setCurrentIndex(0);
}

class_mask_selecter::class_mask_selecter()
{
    widget = new QWidget();
    QVBoxLayout * layout = new QVBoxLayout();
    QHBoxLayout * tlayout = new QHBoxLayout();
    QHBoxLayout * mlayout = new QHBoxLayout();
    QHBoxLayout * dlayout = new QHBoxLayout();
    layout->addLayout(tlayout);
    layout->addLayout(mlayout);
    layout->addLayout(dlayout);
    widget->setLayout(layout);

    tlayout->addWidget((box[0] = new QCheckBox()));
    tlayout->addWidget((box[1] = new QCheckBox()));
    tlayout->addWidget((box[2] = new QCheckBox()));
    mlayout->addWidget((box[3] = new QCheckBox()));
    mlayout->addWidget((box[4] = new QCheckBox()));
    mlayout->addWidget((box[5] = new QCheckBox()));
    dlayout->addWidget((box[6] = new QCheckBox()));
    dlayout->addWidget((box[7] = new QCheckBox()));
    dlayout->addWidget((box[8] = new QCheckBox()));

    box[0]->setText("Warrior");
    box[1]->setText("Paladin");
    box[2]->setText("Hunter");
    box[3]->setText("Rogue");
    box[4]->setText("Priest");
    box[5]->setText("Shaman");
    box[6]->setText("Mage");
    box[7]->setText("Warlock");
    box[8]->setText("Druid");

    clear();
}

class_mask_selecter::~class_mask_selecter(){}

QWidget * class_mask_selecter::getWidget(){ return widget; }

void class_mask_selecter::clear(){ for(int i = 0; i < 9; i++){ box[i]->setChecked(true); } }

int class_mask_selecter::selectedId()
{
    int id = -1;
    for(int i = 0; i < 9; i++)
    {
        if(!box[i]->isChecked())
        {
            id = 0;
            id |= box[0]->isChecked() << (CLASS_WARRIOR-1);
            id |= box[1]->isChecked() << (CLASS_PALADIN-1);
            id |= box[2]->isChecked() << (CLASS_HUNTER-1);
            id |= box[3]->isChecked() << (CLASS_ROGUE-1);
            id |= box[4]->isChecked() << (CLASS_PRIEST-1);
            id |= box[5]->isChecked() << (CLASS_SHAMAN-1);
            id |= box[6]->isChecked() << (CLASS_MAGE-1);
            id |= box[7]->isChecked() << (CLASS_WARLOCK-1);
            id |= box[8]->isChecked() << (CLASS_DRUID-1);
            return id;
        }
    }
    return id;
}

void class_mask_selecter::setIndexFromId(int id)
{
    if(id == -1)
    {
        clear();
        return;
    }
    else
    {
        box[0]->setChecked(id & (1 << (CLASS_WARRIOR-1)));
        box[1]->setChecked(id & (1 << (CLASS_PALADIN-1)));
        box[2]->setChecked(id & (1 << (CLASS_HUNTER-1)));
        box[3]->setChecked(id & (1 << (CLASS_ROGUE-1)));
        box[4]->setChecked(id & (1 << (CLASS_PRIEST-1)));
        box[5]->setChecked(id & (1 << (CLASS_SHAMAN-1)));
        box[6]->setChecked(id & (1 << (CLASS_MAGE-1)));
        box[7]->setChecked(id & (1 << (CLASS_WARLOCK-1)));
        box[8]->setChecked(id & (1 << (CLASS_DRUID-1)));
    }
}

race_mask_selecter::race_mask_selecter()
{
    widget = new QWidget();
    QVBoxLayout * layout = new QVBoxLayout();
    QHBoxLayout * l0 = new QHBoxLayout();
    QHBoxLayout * l1 = new QHBoxLayout();
    QHBoxLayout * l2 = new QHBoxLayout();
    QHBoxLayout * l3 = new QHBoxLayout();
    layout->addLayout(l0);
    layout->addLayout(l1);
    layout->addLayout(l2);
    layout->addLayout(l3);
    widget->setLayout(layout);
    for(int i = 0; i < 10; i++)
    {
        box[i] = new QCheckBox();
    }
    l0->addWidget(box[0]);
    l0->addWidget(box[1]);
    l0->addWidget(box[2]);
    l1->addWidget(box[3]);
    l1->addWidget(box[4]);
    l1->addWidget(box[5]);
    l2->addWidget(box[6]);
    l2->addWidget(box[7]);
    l2->addWidget(box[8]);
    l3->addWidget(box[9]);

    box[0]->setText("Human");
    box[1]->setText("Orc");
    box[2]->setText("Dwarf");
    box[3]->setText("Night Elf");
    box[4]->setText("Undead");
    box[5]->setText("Tauren");
    box[6]->setText("Gnome");
    box[7]->setText("Troll");
    box[8]->setText("Blood Elf");
    box[9]->setText("Draenei");

    clear();
}

race_mask_selecter::~race_mask_selecter(){}

QWidget * race_mask_selecter::getWidget(){ return widget; }

void race_mask_selecter::clear(){for(int i = 0;i<10;i++){box[i]->setChecked(true);}}

int race_mask_selecter::selectedId()
{
    int id = -1;
    for(int i = 0; i<10;i++)
    {
        if(!box[i]->isChecked())
        {
            id = 0;
            id |= box[0]->isChecked() << (RACE_HUMAN-1);
            id |= box[1]->isChecked() << (RACE_ORC-1);
            id |= box[2]->isChecked() << (RACE_DWARF-1);
            id |= box[3]->isChecked() << (RACE_NIGHTELF-1);
            id |= box[4]->isChecked() << (RACE_UNDEAD-1);
            id |= box[5]->isChecked() << (RACE_TAUREN-1);
            id |= box[6]->isChecked() << (RACE_GNOME-1);
            id |= box[7]->isChecked() << (RACE_TROLL-1);
            id |= box[8]->isChecked() << (RACE_BLOODELF-1);
            id |= box[9]->isChecked() << (RACE_DRAENEI-1);
            return id;
        }
    }
    return id;
}

void race_mask_selecter::setIndexFromId(int id)
{
    box[0]->setChecked(id & (1 << (RACE_HUMAN-1)));
    box[1]->setChecked(id & (1 << (RACE_ORC-1)));
    box[2]->setChecked(id & (1 << (RACE_DWARF-1)));
    box[3]->setChecked(id & (1 << (RACE_NIGHTELF-1)));
    box[4]->setChecked(id & (1 << (RACE_UNDEAD-1)));
    box[5]->setChecked(id & (1 << (RACE_TAUREN-1)));
    box[6]->setChecked(id & (1 << (RACE_GNOME-1)));
    box[7]->setChecked(id & (1 << (RACE_TROLL-1)));
    box[8]->setChecked(id & (1 << (RACE_BLOODELF-1)));
    box[9]->setChecked(id & (1 << (RACE_DRAENEI-1)));
}

title_selecter::title_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_title_resource()));
}

title_selecter::~title_selecter(){}

void title_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::title)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_title_resource()));
    }
}

QComboBox * title_selecter::getWidget(){ return widget; }

void title_selecter::clear()
{
    widget->setCurrentIndex(0);
}

int title_selecter::selectedId()
{
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    return m_dbccache->get_title_references()->at(widget->currentIndex()-1);
}

void title_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_title_references(),id);
    if(index == -1)
    {
        widget->setCurrentIndex(0);
        return;
    }
    widget->setCurrentIndex(index+1);
}

honor_rank_selecter::honor_rank_selecter()
{
    widget = new QComboBox();
    widget->addItem("Honor Rank...");
    widget->addItem("Pariah");
    widget->addItem("Outlaw");
    widget->addItem("Exiled");
    widget->addItem("Dishonored");
    widget->addItem("Rank 1");
    widget->addItem("Rank 2");
    widget->addItem("Rank 3");
    widget->addItem("Rank 4");
    widget->addItem("Rank 5");
    widget->addItem("Rank 6");
    widget->addItem("Rank 7");
    widget->addItem("Rank 8");
    widget->addItem("Rank 9");
    widget->addItem("Rank 10");
    widget->addItem("Rank 11");
    widget->addItem("Rank 12");
    widget->addItem("Rank 13");
    widget->addItem("Rank 14");
    widget->addItem("Leader");
}

honor_rank_selecter::~honor_rank_selecter(){}

QComboBox * honor_rank_selecter::getWidget(){ return widget; }

void honor_rank_selecter::clear(){ widget->setCurrentIndex(0); }

int honor_rank_selecter::selectedId(){ return widget->currentIndex(); }

void honor_rank_selecter::setIndexFromId(int id){ widget->setCurrentIndex(id); }

item_stat_type_selecter::item_stat_type_selecter()
{
    widget = new QComboBox();
    widget->addItem("Select Stat...");
    widget->addItem("Mana");
    widget->addItem("Health");
    widget->addItem("Agility");
    widget->addItem("Strength");
    widget->addItem("Intellect");
    widget->addItem("Spirit");
    widget->addItem("Stamina");
    widget->addItem("Defense Rating");
    widget->addItem("Dodge Rating");
    widget->addItem("Parry Rating");
    widget->addItem("Block Rating");
    widget->addItem("Melee Hit Rating");
    widget->addItem("Ranged Hit Rating");
    widget->addItem("Spell Hit Rating");
    widget->addItem("Melee Crit Rating");
    widget->addItem("Ranged Crit Rating");
    widget->addItem("Spell Crit Rating");
    widget->addItem("Melee Hit-Reduce Rating");
    widget->addItem("Ranged Hit-Reduce Rating");
    widget->addItem("Spell Hit-Reduce Rating");
    widget->addItem("Melee Crit-Reduce Rating");
    widget->addItem("Ranged Crit-Reduce Rating");
    widget->addItem("Spell Crit-Reduce Rating");
    widget->addItem("Melee Haste Rating");
    widget->addItem("Ranged Haste Rating");
    widget->addItem("Spell Haste Rating");
    widget->addItem("Hit Rating");
    widget->addItem("Crit Rating");
    widget->addItem("Hit-Reduce Rating");
    widget->addItem("Crit-Reduce Rating");
    widget->addItem("Resilience Rating");
    widget->addItem("Haste Rating");
    widget->addItem("Expertise Rating");
}

item_stat_type_selecter::~item_stat_type_selecter(){}

QComboBox * item_stat_type_selecter::getWidget(){ return widget; }

void item_stat_type_selecter::clear(){ widget->setCurrentIndex(0); }

void item_stat_type_selecter::setIndexFromId(int id)
{
    // Hard code
    if(id >= 0 && id <= 1){ widget->setCurrentIndex(id+1); return; }
    if(id >= 3 && id <= 7){widget->setCurrentIndex(id); return; }
    if(id >= 12 && id <= 37){widget->setCurrentIndex(id - 4); return; }
}

int item_stat_type_selecter::selectedId()
{
    // Hard code, reverse of setIndexFromId
    int a = widget->currentIndex()-1;
    if(a >= 0 && a <= 1){ return a; }
    if(a >= 2 && a <= 6){ return a+1; }
    if(a >= 7 && a <= 32){ return a+5; }
    return 0;
}

item_sheathe_selecter::item_sheathe_selecter()
{
    widget = new QComboBox();
    widget->addItem("Sheathe...");
    widget->addItem("Two-Handed Weapon");
    widget->addItem("Staff");
    widget->addItem("One-Handed");
    widget->addItem("Shield");
    widget->addItem("Enchanter's Rod");
    widget->addItem("Off-Hand");
}

item_sheathe_selecter::~item_sheathe_selecter(){}

QComboBox * item_sheathe_selecter::getWidget(){ return widget; }

void item_sheathe_selecter::clear(){ widget->setCurrentIndex(0); }

int item_sheathe_selecter::selectedId()
{
    return widget->currentIndex();
}

void item_sheathe_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(id);
}

socket_color_selecter::socket_color_selecter()
{
    widget = new QComboBox();
    widget->addItem("Socket Color...");
    widget->addItem("Meta");
    widget->addItem("Red");
    widget->addItem("Yellow");
    widget->addItem("Blue");
}

socket_color_selecter::~socket_color_selecter(){}

QComboBox * socket_color_selecter::getWidget(){ return widget; }

void socket_color_selecter::clear(){ widget->setCurrentIndex(0); }

int socket_color_selecter::selectedId()
{
    if(widget->currentIndex() < 2){ return widget->currentIndex(); }
    if(widget->currentIndex() >= 2){ return 1 << (widget->currentIndex()-1); }
    return 0;
}

void socket_color_selecter::setIndexFromId(int id)
{
    switch(id)
    {
    case 0: widget->setCurrentIndex(0); return;
    case 1: widget->setCurrentIndex(1); return;
    case 2: widget->setCurrentIndex(2); return;
    case 4: widget->setCurrentIndex(3); return;
    case 8: widget->setCurrentIndex(4); return;
    }
    widget->setCurrentIndex(0);
}

item_enchant_dialog::item_enchant_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setLayout((layout = new QVBoxLayout()));
    layout->addWidget((list = new QListWidget()));
    list->addItems(*(m_dbccache->get_item_enchant_resource()));
    layout->addLayout((blayout = new QHBoxLayout()));
    blayout->addWidget((acceptB = new QPushButton()));
    blayout->addWidget((cancelB = new QPushButton()));
    blayout->addWidget((clearB = new QPushButton()));
    cancelB->setText("Cancel");
    acceptB->setText("Accept");
    clearB->setText("Clear");

    list->setMinimumSize(400,600);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);


    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(rejected()));
    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(accepted()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accepted()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(rejected()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(cleared()));
}
item_enchant_dialog::~item_enchant_dialog(){}

void item_enchant_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::item_enchant)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_item_enchant_resource()));
    }
}

QDialog * item_enchant_dialog::getWidget(){ return widget; }

void item_enchant_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}

void item_enchant_dialog::cleared()
{
    emit clear();
    rejected();
}

void item_enchant_dialog::rejected()
{
    widget->setResult(-1);
    widget->done(widget->result());
}

void item_enchant_dialog::accepted()
{
    widget->setResult(list->currentRow()+1);
    widget->done(widget->result());
}


item_enchant_selecter::item_enchant_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    selected = -1;
    widget = new QPushButton();
    widget->setText("Item Enchant...");

    dialog = new item_enchant_dialog{m_dbccache};

    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(onButtonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

item_enchant_selecter::~item_enchant_selecter(){}

void item_enchant_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::item_enchant)
    {
        selected = -1;
        widget->setText("Item Enchant...");
        dialog->reload(r);
    }
}

QPushButton * item_enchant_selecter::getWidget(){ return widget; }

int item_enchant_selecter::selectedId()
{
    if(selected == -1){ return 0; }
    return m_dbccache->get_item_enchant_references()->at(selected);
}

void item_enchant_selecter::setIndexFromId(int id)
{
    selected = dbccache::find_ref(m_dbccache->get_item_enchant_references(),id);
    if(selected != -1)
    {
        widget->setText(m_dbccache->get_item_enchant_resource()->at(selected));
    }
    else
    {
        widget->setText("Item Enchant...");
    }
}

void item_enchant_selecter::clear()
{
    selected = -1;
    widget->setText("Item Enchant...");
}

void item_enchant_selecter::onButtonPushed()
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
        widget->setText(m_dbccache->get_item_enchant_resource()->at(selected));
    }
}

damage_school_selecter::damage_school_selecter()
{
    widget = new QComboBox();
    widget->addItem("Dmg Type...");
    widget->addItem("Physical");
    widget->addItem("Holy");
    widget->addItem("Fire");
    widget->addItem("Nature");
    widget->addItem("Frost");
    widget->addItem("Shadow");
    widget->addItem("Arcane");
}

damage_school_selecter::~damage_school_selecter(){}

QComboBox * damage_school_selecter::getWidget(){ return widget; }

int damage_school_selecter::selectedId()
{
    if(widget->currentIndex() == 0) // default value is 0 (Physical)
    {
        return 0;
    }
    return widget->currentIndex()-1;
}

QString damage_school_selecter::selectedString()
{
    if(widget->currentIndex() == 0)
    {
        return widget->itemText(1);
    }
    return widget->currentText();
}

void damage_school_selecter::setIndexFromId(int id){ widget->setCurrentIndex(id+1); }

void damage_school_selecter::clear(){ widget->setCurrentIndex(0); }


ammo_type_selecter::ammo_type_selecter()
{
    widget = new QComboBox();
    widget->addItem("Ammo Type...");
    widget->addItem("Arrow");
    widget->addItem("Bullet");
}

ammo_type_selecter::~ammo_type_selecter(){}

QComboBox * ammo_type_selecter::getWidget(){ return widget; }

int ammo_type_selecter::selectedId()
{
    switch(widget->currentIndex())
    {
    case 0: return 0;
    case 1: return 2;
    case 2: return 3;
    }
    return 0;
}

void ammo_type_selecter::setIndexFromId(int id)
{
    switch(id)
    {
    case 0: widget->setCurrentIndex(0); return;
    case 2: widget->setCurrentIndex(1); return;
    case 3: widget->setCurrentIndex(2); return;
    }
}

void ammo_type_selecter::clear(){ widget->setCurrentIndex(0); }

spell_trigger_selecter::spell_trigger_selecter()
{
    widget = new QComboBox();
    widget->addItem("Spell Trigger...");
    widget->addItem("Use");
    widget->addItem("On Equip");
    widget->addItem("Chance on Hit");
    widget->addItem("Soulstone");
    widget->addItem("Use with no delay");
}

spell_trigger_selecter::~spell_trigger_selecter(){}

QComboBox * spell_trigger_selecter::getWidget(){ return widget; }

int spell_trigger_selecter::selectedId()
{
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    return widget->currentIndex()-1;
}

void spell_trigger_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(id+1);
}

void spell_trigger_selecter::clear(){ widget->setCurrentIndex(0); }

language_selecter::language_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_language_resource()));
}

language_selecter::~language_selecter() {}

void language_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::language)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_language_resource()));
    }
}

QComboBox * language_selecter::getWidget(){ return widget; }

void language_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_language_references(), id);
    if(index == -1)
    {
        widget->setCurrentIndex(0);
        return;
    }
    widget->setCurrentIndex(index+1);
}

int language_selecter::selectedId()
{
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    return m_dbccache->get_language_references()->at(widget->currentIndex()-1);
}

void language_selecter::clear(){ widget->setCurrentIndex(0); }

page_material_selecter::page_material_selecter()
{
    widget = new QComboBox();
    widget->addItem("Page Material...");
    widget->addItem("Parchment");
    widget->addItem("Stone");
    widget->addItem("Marble");
    widget->addItem("Silver");
    widget->addItem("Bronze");
    widget->addItem("Valentine");
    widget->addItem("Illidan");
}

page_material_selecter::~page_material_selecter(){}

QComboBox * page_material_selecter::getWidget(){ return widget; }

void page_material_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(id);
}

int page_material_selecter::selectedId(){ return widget->currentIndex(); }

void page_material_selecter::clear(){ widget->setCurrentIndex(0); }

material_selecter::material_selecter()
{
    widget = new QComboBox();
    widget->addItem("Material...");
    widget->addItem("Consumable");
    widget->addItem("Metal");
    widget->addItem("Wood");
    widget->addItem("Liquid");
    widget->addItem("Jewelry");
    widget->addItem("Chain");
    widget->addItem("Plate");
    widget->addItem("Cloth");
    widget->addItem("Leather");
}

material_selecter::~material_selecter(){}

QComboBox * material_selecter::getWidget(){ return widget; }

void material_selecter::setIndexFromId(int id)
{
    if(id == -1){ widget->setCurrentIndex(1); }
    else if(id >= 1){ widget->setCurrentIndex(id+1); }
    else{ widget->setCurrentIndex(0); }

}

int material_selecter::selectedId()
{
    if(widget->currentIndex() == 0){ return 0; }
    else if(widget->currentIndex() == 1){ return -1; }
    else if(widget->currentIndex() > 1){ return widget->currentIndex() - 1; }
    else{return -1; }
}

void material_selecter::clear(){ widget->setCurrentIndex(0); }

bag_family_mask_selecter::bag_family_mask_selecter()
{
    widget = new QWidget();
    widget->setLayout((layout = new QHBoxLayout()));
    layout->addLayout((tlayout = new QVBoxLayout()));
    layout->addLayout((mlayout = new QVBoxLayout()));
    layout->addLayout((blayout = new QVBoxLayout()));
    tlayout->addWidget((box[0] = new QCheckBox()));
    tlayout->addWidget((box[1] = new QCheckBox()));
    tlayout->addWidget((box[2] = new QCheckBox()));
    tlayout->addWidget((box[3] = new QCheckBox()));
    mlayout->addWidget((box[4] = new QCheckBox()));
    mlayout->addWidget((box[5] = new QCheckBox()));
    mlayout->addWidget((box[6] = new QCheckBox()));
    blayout->addWidget((box[7] = new QCheckBox()));
    blayout->addWidget((box[8] = new QCheckBox()));
    blayout->addWidget((box[9] = new QCheckBox()));
    box[0]->setText("Arrows");
    box[1]->setText("Bullets");
    box[2]->setText("Soul Shards");
    box[3]->setText("Leatherworking Supplies");
    box[4]->setText("Herbs");
    box[5]->setText("Enchanting Supplies");
    box[6]->setText("Engineering Supplies");
    box[7]->setText("Keys");
    box[8]->setText("Gems");
    box[9]->setText("Mining Supplies");
    clear();
}

bag_family_mask_selecter::~bag_family_mask_selecter(){}

QWidget * bag_family_mask_selecter::getWidget(){ return widget; }

void bag_family_mask_selecter::clear(){ for(int i = 0; i < 10; i++){ box[i]->setChecked(false); } }

void bag_family_mask_selecter::setIndexFromId(int id)
{
    for(int i = 0; i < 10; i++)
    {
        if(id & (1 << (i+1))){ box[i]->setChecked(true); }
        else{ box[i]->setChecked(false); }
    }
}

int bag_family_mask_selecter::selectedId()
{
    int r = 0;
    for(int i = 0; i < 10; i++)
    {
        r |= box[i]->isChecked() << (i+1);
    }
    return r;
}

totem_category_selecter::totem_category_selecter(dbccache * c) :
    m_dbccache(c)
{
    widget = new QComboBox();
    widget->addItems(*(m_dbccache->get_totem_category_resource()));
}

totem_category_selecter::~totem_category_selecter() {}

void totem_category_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::totem_category)
    {
        widget->clear();
        widget->addItems(*(m_dbccache->get_totem_category_resource()));
    }
}

QComboBox * totem_category_selecter::getWidget(){ return widget; }

void totem_category_selecter::clear(){ widget->setCurrentIndex(0); }

void totem_category_selecter::setIndexFromId(int id)
{
    int index = dbccache::find_ref(m_dbccache->get_totem_category_references(),id);
    if(index == -1)
    {
        widget->setCurrentIndex(0);
        return;
    }
    widget->setCurrentIndex(index+1);
}

int totem_category_selecter::selectedId()
{
    if(widget->currentIndex() == 0)
    {
        return 0;
    }
    return m_dbccache->get_totem_category_references()->at(widget->currentIndex()-1);
}

gem_dialog::gem_dialog(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QDialog();
    widget->setLayout((layout = new QVBoxLayout()));
    layout->addWidget((list = new QListWidget()));
    list->addItems(*(m_dbccache->get_gem_resource()));
    layout->addLayout((blayout = new QHBoxLayout()));
    blayout->addWidget((acceptB = new QPushButton()));
    blayout->addWidget((cancelB = new QPushButton()));
    blayout->addWidget((clearB = new QPushButton()));
    acceptB->setText("Accept");
    cancelB->setText("Cancel");
    clearB->setText("Clear");

    list->setMinimumSize(400, 600);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setSizeGripEnabled(false);
    widget->setModal(true);


    QObject::connect(cancelB,SIGNAL(clicked()),this,SLOT(rejected()));
    QObject::connect(acceptB,SIGNAL(clicked()),this,SLOT(accepted()));
    QObject::connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accepted()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(rejected()));
    QObject::connect(clearB,SIGNAL(clicked()),this,SLOT(cleared()));
}

gem_dialog::~gem_dialog(){}

void gem_dialog::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::gem)
    {
        list->clear();
        list->addItems(*(m_dbccache->get_gem_resource()));
    }
}

QDialog * gem_dialog::getWidget(){ return widget; }

void gem_dialog::setIndex(int index)
{
    list->setCurrentRow(index);
}
void gem_dialog::cleared()
{
    emit clear();
    rejected();
}

void gem_dialog::rejected()
{
    widget->setResult(-1);
    widget->done(widget->result());
}
void gem_dialog::accepted()
{
    widget->setResult(list->currentRow()+1);
    widget->done(widget->result());
}

gem_selecter::gem_selecter(dbccache * c) :
    QObject(),
    m_dbccache(c)
{
    widget = new QPushButton();
    widget->setText("Gem...");
    dialog = new gem_dialog{m_dbccache};
    selected = -1;

    QObject::connect(widget,SIGNAL(clicked()),this,SLOT(buttonPushed()));
    QObject::connect(dialog,SIGNAL(clear()),this,SLOT(clear()));
}

gem_selecter::~gem_selecter(){}

void gem_selecter::reload(dbc_resource_type r)
{
    if(r == dbc_resource_type::gem)
    {
        selected = -1;
        widget->setText("Gem...");
        dialog->reload(r);
    }
}

QPushButton * gem_selecter::getWidget(){ return widget; }

void gem_selecter::clear(){ widget->setText("Gem...");selected = -1; }

void gem_selecter::setIndexFromId(int id)
{
    selected = dbccache::find_ref(m_dbccache->get_gem_references(),id);
    if(selected == -1)
    {
        widget->setText("Gem...");
        return;
    }
    widget->setText(m_dbccache->get_gem_resource()->at(selected));
}

int gem_selecter::selectedId()
{
    if(selected != -1)
    {
        return m_dbccache->get_gem_references()->at(selected);
    }
    else
    {
        return 0;
    }
}


void gem_selecter::buttonPushed()
{
    if(selected)
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
        widget->setText(m_dbccache->get_gem_resource()->at(selected));
    }
}

food_type_selecter::food_type_selecter()
{
    widget = new QComboBox();
    widget->addItem("Food Type...");
    widget->addItem("Meat");
    widget->addItem("Fish");
    widget->addItem("Cheese");
    widget->addItem("Bread");
    widget->addItem("Fungus");
    widget->addItem("Fruit");
    widget->addItem("Raw Meat");
    widget->addItem("Raw Fish");
}

food_type_selecter::~food_type_selecter(){}

QComboBox * food_type_selecter::getWidget(){ return widget; }

int food_type_selecter::selectedId()
{
    return widget->currentIndex();
}

void food_type_selecter::clear(){ widget->setCurrentIndex(0); }

void food_type_selecter::setIndexFromId(int id)
{
    widget->setCurrentIndex(id);
}

item_flags_selecter::item_flags_selecter()
{
    widget = new QWidget();
    widget->setLayout((layout = new QVBoxLayout()));
    layout->addLayout((tlayout = new QHBoxLayout()));
    layout->addLayout((mlayout = new QHBoxLayout()));
    layout->addLayout((blayout = new QHBoxLayout()));
    for(int i = 0; i < 14; i++)
    {
        if(i%3 == 0){ tlayout->addWidget((box[i] = new QCheckBox())); }
        if(i%3 == 1){ mlayout->addWidget((box[i] = new QCheckBox())); }
        if(i%3 == 2){ blayout->addWidget((box[i] = new QCheckBox())); }
        box[i]->setLayoutDirection(Qt::RightToLeft);
    }
    box[0]->setText("Soulbound");
    box[1]->setText("Conjured");
    box[2]->setText("Lootable");
    box[3]->setText("Wrapped");
    box[4]->setText("Totem");
    box[5]->setText("Activatable");
    box[6]->setText("Wrapper");
    box[7]->setText("Gifts");
    box[8]->setText("Can be looted by all");
    box[9]->setText("Guild/Arena Charter");
    box[10]->setText("PvP Reward");
    box[11]->setText("Unique Equipped");
    box[12]->setText("Throwable");
    box[13]->setText("Special Use");
}

item_flags_selecter::~item_flags_selecter(){}

void item_flags_selecter::clear()
{
    for(int i = 0; i < 14; i++)
    {
        box[i]->setChecked(false);
    }
}

void item_flags_selecter::setIndexFromId(int id)
{
    box[0]->setChecked(id & IF_SOULBOUND);
    box[1]->setChecked(id & IF_CONJURED);
    box[2]->setChecked(id & IF_LOOTABLE);
    box[3]->setChecked(id & IF_WRAPPED);
    box[4]->setChecked(id & IF_TOTEM);
    box[5]->setChecked(id & IF_ACTIVATABLE);
    box[6]->setChecked(id & IF_WRAPPER);
    box[7]->setChecked(id & IF_GIFTS);
    box[8]->setChecked(id & IF_ALLCANLOOT);
    box[9]->setChecked(id & IF_CHARTER);
    box[10]->setChecked(id & IF_PVPREWARD);
    box[11]->setChecked(id & IF_UNIQUEEQUIPPED);
    box[12]->setChecked(id & IF_THROWABLE);
    box[13]->setChecked(id & IF_SPECIALUSE);
}

int item_flags_selecter::selectedId()
{
    return ((box[0]->isChecked()*IF_SOULBOUND) |
            (box[1]->isChecked()*IF_CONJURED) |
            (box[2]->isChecked()*IF_LOOTABLE) |
            (box[3]->isChecked()*IF_WRAPPED) |
            (box[4]->isChecked()*IF_TOTEM) |
            (box[5]->isChecked()*IF_ACTIVATABLE) |
            (box[6]->isChecked()*IF_WRAPPER) |
            (box[7]->isChecked()*IF_GIFTS) |
            (box[8]->isChecked()*IF_ALLCANLOOT) |
            (box[9]->isChecked()*IF_CHARTER) |
            (box[10]->isChecked()*IF_PVPREWARD) |
            (box[11]->isChecked()*IF_UNIQUEEQUIPPED) |
            (box[12]->isChecked()*IF_THROWABLE) |
            (box[13]->isChecked()*IF_SPECIALUSE));
}













