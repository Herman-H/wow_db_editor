#ifndef CREATURETEMPLATEQUERYGROUPBOX_H
#define CREATURETEMPLATEQUERYGROUPBOX_H

#include <QGroupBox>
#include "dbcwidgets.h"

class QSqlQueryModel;
class QSqlDatabase;
class QLayout;
class dbccache;

namespace Ui {
class CreatureTemplateQueryGroupBox;
}

class CreatureTemplateQueryGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit CreatureTemplateQueryGroupBox(QSqlDatabase& db, dbccache * c, QWidget *parent = 0);
    ~CreatureTemplateQueryGroupBox();

    int selectedEntry(){ return selectedentry; }
    void setNameField(QString name);
    QLayout * getCancelButtonLayout();

public slots:
    QString currentQuery(QString alias, bool & hasSpecifiers);

private slots:
    void on_clearButton_clicked();
    void on_queryButton_clicked();
    void on_selectButton_clicked();

    void on_resultView_doubleClicked(const QModelIndex &index);

signals:

    void queried();
    void selected();

private:
    QSqlDatabase &          db;
    QSqlQueryModel  *       creatureTemplateQueryModel;
    Ui::CreatureTemplateQueryGroupBox *ui;
    dbccache *               m_dbccache;
    creature_rank_selecter * rankCB;
    creature_type_selecter * typeCB;
    creature_family_selecter * familyCB;
    faction_selecter       * factionS;
    inhabit_type_selecter  * inhabitGB;
    flags_extra_selecter   * flagsExtraGB;
    npc_flags_selecter     * npcFlagsGB;
    mechanics_mask_selecter * mechanics_immunitiesGB;
    map_selecter *          mapselecter;
    int selectedentry;
    void initAll();
    void clearAll();

};

#endif // CREATURETEMPLATEQUERYGROUPBOX_H
