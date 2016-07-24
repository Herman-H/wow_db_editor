#include "itemtemplatequery.h"
#include "ui_itemtemplatequery.h"
#include "dbcwidgets.h"
#include <QLayout>
#include <QComboBox>
#include <QValidator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QTableView>
#include <QPushButton>
#include <QSqlError>
#include "dbccache.h"

itemtemplatequery::itemtemplatequery(QSqlDatabase &db, dbccache * c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemtemplatequery),
    db(db),
    m_dbccache(c),
    queryModel(*(new QSqlQueryModel()))
{
    ui->setupUi(this);
    ui->itemclassLayout->addWidget((classselecter = new item_class_selecter(m_dbccache))->getWidget());
    ui->itemsubclassLayout->addWidget((subclassselecter = new item_subclass_selecter(m_dbccache))->getWidget());
    subclassselecter->getWidget()->setEnabled(false);
    ui->qualityLayout->addWidget((qualityselecter = new item_quality_selecter())->getWidget());
    ui->slotselecterLayout->addWidget((slotselecter = new item_slot_selecter())->getWidget());
    ui->classmaskLayout->layout()->addWidget((classmask = new class_mask_selecter())->getWidget());
    ui->racemaskLayout->layout()->addWidget((racemask = new race_mask_selecter())->getWidget());
    QObject::connect(classselecter->getWidget(),SIGNAL(currentIndexChanged(int)),this,SLOT(onitemclass_selection()));

    ui->tableView->setModel(&queryModel);

    ui->minILvLEdit->setValidator(new QIntValidator);
    ui->maxILvLEdit->setValidator(new QIntValidator);
    ui->minReqLvlEdit->setValidator(new QIntValidator);
    ui->maxReqLvlEdit->setValidator(new QIntValidator);
}

itemtemplatequery::~itemtemplatequery()
{
    delete ui;
}

void itemtemplatequery::setNameField(QString name)
{
    ui->itemnameEdit->setText(name);
}

QString itemtemplatequery::currentQuerySpecifiers(QString alias, bool & hasSpecifiers)
{
    QString q = "";
    if(ui->itemnameEdit->text().length() != 0)
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".name LIKE '%" + ui->itemnameEdit->text() + "%'"; hasSpecifiers=true;
    }
    if(!slotselecter->isDefaultSelected())
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".InventoryType = " + QString::number(slotselecter->selectedId()); hasSpecifiers=true;
    }
    if(!classselecter->isDefaultSelected())
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".class = " + QString::number(classselecter->selectedId()); hasSpecifiers=true;
    }
    if(!subclassselecter->isDefaultSelected())
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".subclass = " + QString::number(subclassselecter->selectedId()); hasSpecifiers=true;
    }
    if(!qualityselecter->isDefaultSelected())
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".Quality = " + QString::number(qualityselecter->selectedId()); hasSpecifiers=true;
    }
    if(ui->minILvLEdit->text().length() != 0)
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".ItemLevel >= " + ui->minILvLEdit->text(); hasSpecifiers=true;
    }
    if(ui->maxILvLEdit->text().length() != 0)
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".ItemLevel <= " + ui->maxILvLEdit->text(); hasSpecifiers=true;
    }
    if(ui->minReqLvlEdit->text().length() != 0)
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".RequiredLevel >= " + ui->minReqLvlEdit->text(); hasSpecifiers=true;
    }
    if(ui->maxReqLvlEdit->text().length() != 0)
    {
        if(hasSpecifiers){ q += " AND "; }
        q += alias + ".RequiredLevel <= " + ui->maxReqLvlEdit->text(); hasSpecifiers=true;
    }
    if(classmask->selectedId()!=-1)
    {
        if(hasSpecifiers){ q += " AND "; }

        q += "(" + alias + ".AllowableClass & " + QString::number(classmask->selectedId()) + ") != 0"; hasSpecifiers=true;
    }
    if(racemask->selectedId()!=-1)
    {
        if(hasSpecifiers){ q += " AND "; }
        q += "(" + alias + ".AllowableRace & " + QString::number(racemask->selectedId()) + ") != 0"; hasSpecifiers=true;
    }
    return q;
}

void itemtemplatequery::onitemclass_selection()
{
    if(classselecter->isDefaultSelected())
    {
        subclassselecter->setCategory(-1);
        subclassselecter->getWidget()->setEnabled(false);
        return;
    }
    subclassselecter->setCategory(classselecter->selectedId());
    subclassselecter->getWidget()->setEnabled(true);
}

void itemtemplatequery::on_clearButton_clicked()
{
    subclassselecter->clear();
    slotselecter->clear();
    classselecter->clear();
    qualityselecter->clear();
    ui->itemnameEdit->clear();
    ui->minILvLEdit->clear();
    ui->maxILvLEdit->clear();
    ui->minReqLvlEdit->clear();
    ui->maxReqLvlEdit->clear();
    classmask->clear();
    racemask->clear();
}

void itemtemplatequery::on_queryButton_clicked()
{
    bool hasSpecifiers;
    QString sp = currentQuerySpecifiers(QString("it"), hasSpecifiers);
    QString query = QString(
    "SELECT it.name,it.Quality,it.class,it.ItemLevel,it.RequiredLevel,it.InventoryType,it.entry FROM item_template AS it");
    if(hasSpecifiers)
    {
        query += QString(" WHERE ") + sp;
    }
    query+=";";
    queryModel.setQuery(db.exec(query));
    queryModel.setHeaderData(0, Qt::Horizontal, "Name");
    queryModel.setHeaderData(1, Qt::Horizontal, "Quality");
    queryModel.setHeaderData(2, Qt::Horizontal, "Type");
    queryModel.setHeaderData(3, Qt::Horizontal, "ILvl");
    queryModel.setHeaderData(4, Qt::Horizontal, "ReqLvl");
    ui->tableView->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,120);
    ui->tableView->setColumnWidth(2,120);
    ui->tableView->setColumnWidth(3,60);
    ui->tableView->setColumnWidth(4,60);
    ui->tableView->setColumnWidth(5,120);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setItemDelegateForColumn(1, new ItemQualityWidgetDelegate());
    ui->tableView->setItemDelegateForColumn(2, new ItemClassWidgetDelegate{m_dbccache});
    ui->tableView->setItemDelegateForColumn(5, new ItemSlotWidgetDelegate());
}

QPushButton * itemtemplatequery::cancelButton(){ return ui->cancelButton; }

void itemtemplatequery::on_selectButton_clicked()
{
    if(ui->tableView->selectionModel()->selectedRows().length())
    {
        on_tableView_doubleClicked(ui->tableView->selectionModel()->selectedRows().at(0));
    }
}

void itemtemplatequery::on_tableView_doubleClicked(const QModelIndex &index)
{
    selected = queryModel.record(index.row()).value(6).toInt();
    emit itemselected();
}
