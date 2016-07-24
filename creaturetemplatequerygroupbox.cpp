#include "creaturetemplatequerygroupbox.h"
#include "creaturetemplatequery.h"
#include "ui_creaturetemplatequerygroupbox.h"
#include <QSpacerItem>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QLayout>
#include <QSqlError>
#include <QTableView>
#include <QItemSelectionModel>
#include "dbccache.h"

CreatureTemplateQueryGroupBox::CreatureTemplateQueryGroupBox(QSqlDatabase & db, dbccache * c, QWidget *parent) :
    QGroupBox(parent),
    db(db),
    ui(new Ui::CreatureTemplateQueryGroupBox),
    m_dbccache(c)
{
    ui->setupUi(this);
    creatureTemplateQueryModel = new QSqlQueryModel();
    ui->resultView->setModel(creatureTemplateQueryModel);
    rankCB = new creature_rank_selecter();
    ui->miscGroupBox->layout()->addWidget(rankCB->getWidget());
    typeCB = new creature_type_selecter(m_dbccache);
    ui->miscGroupBox->layout()->addWidget(typeCB->getWidget());
    familyCB = new creature_family_selecter(m_dbccache);
    ui->miscGroupBox->layout()->addWidget(familyCB->getWidget());
    factionS = new faction_selecter(m_dbccache);
    ui->miscGroupBox->layout()->addWidget(factionS->getWidget());
    inhabitGB = new inhabit_type_selecter();
    ui->InhabitSelectionLayout->insertWidget(0,inhabitGB->getWidget());
    flagsExtraGB = new flags_extra_selecter(true);
    ui->FlagsExtraLayout->addWidget(flagsExtraGB->getWidget());
    mechanics_immunitiesGB = new mechanics_mask_selecter(true);
    ui->mechanicsImmLayout->addWidget(mechanics_immunitiesGB->getWidget());
    ui->mapLayout->addWidget((mapselecter = new map_selecter{m_dbccache})->getWidget());
    initAll();
}

CreatureTemplateQueryGroupBox::~CreatureTemplateQueryGroupBox()
{
    delete ui;
    delete creatureTemplateQueryModel;
}

void CreatureTemplateQueryGroupBox::initAll()
{
    rankCB->getWidget()->setMinimumWidth(95);
    rankCB->getWidget()->setMaximumWidth(95);
    typeCB->getWidget()->setMinimumWidth(140);
    typeCB->getWidget()->setMaximumWidth(140);
    familyCB->getWidget()->setMinimumWidth(140);
    familyCB->getWidget()->setMaximumWidth(140);
    factionS->getWidget()->setMinimumWidth(170);
    factionS->getWidget()->setMaximumWidth(170);
    ui->minLevelEdit->setValidator(new QIntValidator{});
    ui->maxLevelEdit->setValidator(new QIntValidator{});

    inhabitGB->clear();
    flagsExtraGB->clear();
    mechanics_immunitiesGB->clear();
}

void CreatureTemplateQueryGroupBox::clearAll()
{
    bool b;
    ui->nameEdit->clear();
    ui->subNameEdit->clear();
    ui->minLevelEdit->clear();
    ui->maxLevelEdit->clear();

    rankCB->getWidget()->setCurrentIndex(0);
    typeCB->getWidget()->setCurrentIndex(0);
    familyCB->getWidget()->setCurrentIndex(0);
    factionS->clear();
    inhabitGB->clear();
    flagsExtraGB->clear();
    mechanics_immunitiesGB->clear();
    mapselecter->clear();
}

void CreatureTemplateQueryGroupBox::setNameField(QString name)
{
    ui->nameEdit->setText(name);
}

QLayout * CreatureTemplateQueryGroupBox::getCancelButtonLayout()
{
    return ui->cancelButtonLayout;
}

void CreatureTemplateQueryGroupBox::on_clearButton_clicked()
{
    clearAll();
}

QString CreatureTemplateQueryGroupBox::currentQuery(QString alias, bool & hasSpecifiers)
{
    QString tmp("");

    if(hasSpecifiers && ui->nameEdit->text().length() != 0){ tmp += " AND "; }
    if(ui->nameEdit->text().length() != 0)      { tmp += alias + ".Name LIKE '%" + ui->nameEdit->text() + "%' ";hasSpecifiers=true; }
    if(hasSpecifiers && ui->subNameEdit->text().length() != 0){ tmp += " AND "; }
    if(ui->subNameEdit->text().length() != 0)   { tmp += alias + ".Subname LIKE '%" + ui->subNameEdit->text() + "%' ";hasSpecifiers=true; }
    if(hasSpecifiers && ui->minLevelEdit->text().length() != 0) { tmp += " AND "; }
    if(ui->minLevelEdit->text().length() != 0)  { tmp += alias + ".MinLevel >= " + ui->minLevelEdit->text() + " OR ";
                                                  tmp += alias + ".MaxLevel >= " + ui->minLevelEdit->text();hasSpecifiers=true; }
    if(hasSpecifiers && ui->maxLevelEdit->text().length() != 0) { tmp += " AND "; }
    if(ui->maxLevelEdit->text().length() != 0)  { tmp += alias + ".MaxLevel <= " + ui->maxLevelEdit->text() + " OR ";
                                                  tmp += alias + ".MinLevel <= " + ui->maxLevelEdit->text();hasSpecifiers=true; }

    if(hasSpecifiers && rankCB->getWidget()->currentIndex() != 0) { tmp += " AND "; }
    if(rankCB->getWidget()->currentIndex() != 0)             { tmp += alias + ".Rank = " + QString::number(rankCB->selectedId());hasSpecifiers=true; }
    if(hasSpecifiers && typeCB->getWidget()->currentIndex() != 0) { tmp += " AND "; }
    if(typeCB->getWidget()->currentIndex() != 0)             { tmp += alias + ".CreatureType = " + QString::number(typeCB->selectedId());hasSpecifiers=true; }
    if(hasSpecifiers && familyCB->getWidget()->currentIndex() != 0) { tmp += " AND "; }
    if(familyCB->getWidget()->currentIndex() != 0)           { tmp += alias + ".Family = " + QString::number(familyCB->selectedId());hasSpecifiers=true; }
    if(hasSpecifiers && factionS->currentIndex() != 0) { tmp += " AND "; }
    if(factionS->currentIndex() != 0)           { tmp += alias + ".FactionAlliance = " + QString::number(factionS->selectedId());hasSpecifiers=true; }
    if(hasSpecifiers && mechanics_immunitiesGB->selectedIsUsed()) { tmp += " AND "; }
    if(mechanics_immunitiesGB->selectedIsUsed()){ if(mechanics_immunitiesGB->selectedId() != 0)
        {tmp += alias + ".MechanicImmuneMask & " + QString::number(mechanics_immunitiesGB->selectedId()) + " = " + QString::number(mechanics_immunitiesGB->selectedId());hasSpecifiers=true;}
        else
        {tmp += alias + ".MechanicImmuneMask = 0";hasSpecifiers=true;}
    }
    if(hasSpecifiers && flagsExtraGB->selectedIsUsed()) { tmp += " AND "; }
    if(flagsExtraGB->selectedIsUsed())          { if(flagsExtraGB->selectedId() != 0)
        {tmp += alias + ".ExtraFlags & " + QString::number(flagsExtraGB->selectedId()) + " = " + QString::number(flagsExtraGB->selectedId());hasSpecifiers=true;}
        else
        {tmp += alias + ".ExtraFlags = 0";hasSpecifiers=true;}
    }
    if(hasSpecifiers) { tmp += " AND "; }
    tmp += alias + ".InhabitType & " + QString::number(inhabitGB->selectedId()) + " != 0";
    hasSpecifiers = true;

    return tmp;
}

void CreatureTemplateQueryGroupBox::on_queryButton_clicked()
{
    bool hasSpecifiers = false;
    QString tables = "creature_template AS ct";
    if(mapselecter->selectedId()!=-1 && mapselecter->selectedId() != 0){ tables += ",creature AS c"; }
    QString query = "SELECT DISTINCT ct.Name, ct.MinLevel, ct.MaxLevel, ct.Rank, ct.CreatureType, ct.Entry FROM " + tables + " WHERE ";
    query += currentQuery(QString("ct"), hasSpecifiers);
    if(mapselecter->selectedId() != -1 && mapselecter->selectedId() != 0)
    {
        if(hasSpecifiers){ query += " AND "; }
        query += "c.map = " + QString::number(mapselecter->selectedId());
        query += " AND c.id = ct.entry";
    }
    query += ";";
    creatureTemplateQueryModel->setQuery(db.exec(query));
    // Update the view with query
    creatureTemplateQueryModel->setHeaderData(0, Qt::Horizontal, "Name");
    creatureTemplateQueryModel->setHeaderData(1, Qt::Horizontal, "Min");
    creatureTemplateQueryModel->setHeaderData(2, Qt::Horizontal, "Max");
    creatureTemplateQueryModel->setHeaderData(3, Qt::Horizontal, "Rank");
    creatureTemplateQueryModel->setHeaderData(4, Qt::Horizontal, "Type");
    ui->resultView->setColumnWidth(0,150);
    ui->resultView->setColumnWidth(1,50);
    ui->resultView->setColumnWidth(2,50);
    ui->resultView->setColumnWidth(3,75);
    ui->resultView->setColumnWidth(4,95);
    ui->resultView->verticalHeader()->hide();
    ui->resultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultView->setItemDelegateForColumn(3, new RankWidgetDelegate());
    ui->resultView->setItemDelegateForColumn(4, new TypeWidgetDelegate());
}

void CreatureTemplateQueryGroupBox::on_selectButton_clicked()
{
    if(ui->resultView->selectionModel()->selectedRows().length())
    {
        on_resultView_doubleClicked(ui->resultView->selectionModel()->selectedRows().at(0));
    }
}

void CreatureTemplateQueryGroupBox::on_resultView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    selectedentry = creatureTemplateQueryModel->record(row).value(5).toInt();
    selected();
}
