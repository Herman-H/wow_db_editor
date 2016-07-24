#include "creaturequery.h"
#include "ui_creaturequery.h"
#include <QSqlError>
#include <QScrollBar>

CreatureQuery::CreatureQuery(QSqlDatabase &db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreatureQuery),
    db(db),
    sqlmodel()
{
    ctq = nullptr;
    hasCreatureTemplateSpecifiers = false;
    ui->setupUi(this);
    ui->CreatureTemplateLabel->setText(getCtqText());
    ui->pXMin->setValidator(new QDoubleValidator{});
    ui->pXMax->setValidator(new QDoubleValidator{});
    ui->pYMin->setValidator(new QDoubleValidator{});
    ui->pYMax->setValidator(new QDoubleValidator{});
    ui->pZMin->setValidator(new QDoubleValidator{});
    ui->pZMax->setValidator(new QDoubleValidator{});
    ui->orientationMin->setValidator(new QDoubleValidator{});
    ui->orientationMax->setValidator(new QDoubleValidator{});
    ui->curMinHealth->setValidator(new QIntValidator{});
    ui->curMaxHealth->setValidator(new QIntValidator{});
    ui->curMinMana->setValidator(new QIntValidator{});
    ui->curMaxMana->setValidator(new QIntValidator{});
}

CreatureQuery::~CreatureQuery()
{
    delete ui;
}

void CreatureQuery::setCreatureTemplateQuery(CreatureTemplateQuery *ctq)
{
    if(ctq)
    {
        this->ctq = ctq;
        hasCreatureTemplateSpecifiers = true;
    }
    else
    {
        hasCreatureTemplateSpecifiers = false;
    }
    ui->CreatureTemplateLabel->setText(getCtqText());
}

QString CreatureQuery::getCtqText()
{
    if(ctq)
    {
        return ctq->getShortDescription();
    }
    else
    {
        return "No specified Creature Template";
    }
}

void CreatureQuery::registerUI()
{
    setMap(ui->mapCB->currentIndex());
    setMinXCoord(ui->pXMin->text());
    setMaxXCoord(ui->pXMax->text());
    setMinYCoord(ui->pYMin->text());
    setMaxYCoord(ui->pYMax->text());
    setMinZCoord(ui->pZMin->text());
    setMaxZCoord(ui->pZMax->text());
    setMinOrientation(ui->orientationMin->text());
    setMaxOrientation(ui->orientationMax->text());
    setMinHP(ui->curMinHealth->text());
    setMaxHP(ui->curMaxHealth->text());
    setMinMana(ui->curMinMana->text());
    setMaxMana(ui->curMaxMana->text());
}

void CreatureQuery::on_QueryButton_clicked()
{
    registerUI();
    QSqlQuery sqq = db.exec(makeQuery());
    qDebug(sqq.lastError().text().toLocal8Bit());
    sqlmodel.setQuery(sqq);
    sqlmodel.setHeaderData(0, Qt::Horizontal, "Name");
    sqlmodel.setHeaderData(1, Qt::Horizontal, "Map");
    sqlmodel.setHeaderData(2, Qt::Horizontal, "X");
    sqlmodel.setHeaderData(3, Qt::Horizontal, "Y");
    sqlmodel.setHeaderData(4, Qt::Horizontal, "Z");
    ui->tableView->setModel(&sqlmodel);
    ui->tableView->setColumnWidth(0, 180);
    ui->tableView->setColumnWidth(1, 180);
    ui->tableView->setColumnWidth(2, 70);
    ui->tableView->setColumnWidth(3, 70);
    ui->tableView->setColumnWidth(4, 70);
    ui->tableView->verticalHeader()->hide();
    //ui->tableView->verticalScrollBar()->show();
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setItemDelegateForColumn(1, new MapWidgetDelegate());
}

void CreatureQuery::on_ClearButton_clicked()
{
    ctq = nullptr;
    this->hasCreatureTemplateSpecifiers = false;
    ui->CreatureTemplateLabel->setText(getCtqText());
    ui->mapCB->setCurrentIndex(0);
    ui->pXMin->clear();
    ui->pXMax->clear();
    ui->pYMin->clear();
    ui->pYMax->clear();
    ui->pZMin->clear();
    ui->pZMax->clear();
    ui->orientationMin->clear();
    ui->orientationMax->clear();
    ui->curMinHealth->clear();
    ui->curMaxHealth->clear();
    ui->curMinMana->clear();
    ui->curMaxMana->clear();
}

void CreatureQuery::on_EditCreatureTemplateButton_clicked()
{
    emit pushedQueryCreatureTemplate();
}
