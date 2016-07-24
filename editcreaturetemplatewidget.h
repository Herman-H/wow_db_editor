#ifndef EDITCREATURETEMPLATEWIDGET_H
#define EDITCREATURETEMPLATEWIDGET_H

#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include <QSqlRecord>

#include "dbcwidgets.h"
#include "dbwidgets.h"
#include "creaturetemplatequerygroupbox.h"
#include "trainer_teach_edit.h"
#include "vendor_edit.h"

class item_models;
class selecter_button;

class dbccache;
class dbcache;
namespace Ui {
class EditCreatureTemplateWidget;
}

class EditCreatureTemplateWidget : public QWidget
{
    Q_OBJECT

public:

    static int indexToDmgSchool(int index)
    {
        //* 0 = Normal
        //* 1 = Holy
        //* 2 = Fire
        //* 3 = Nature
        //* 4 = Frost
        //* 5 = Shadow
        //* 6 = Arcane
        if(index == 0)
        {
            return 0;
        }
        else
        {
            return index-1;
        }
    }

    static int dmgSchoolIndex(int dmgschool)
    {
        return dmgschool + 1;
    }
    static int indexToClass(int index)
    {
        // 0 -> 0
        // 1 -> 0
        // 2 -> 1
        // 3 -> 2
        // 4 -> 4
        // 5 -> 8
        switch(index)
        {
        case 0: return 0;
        case 1: return 0;
        case 2: return 1;
        case 3: return 2;
        case 4: return 4;
        case 5: return 8;
        }
        return 0;
    }
    static int unitClassIndex(int unitClass)
    {
        switch(unitClass)
        {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 4: return 4;
        case 8: return 5;
        }
        return 0;
    }
    static int indexToFamily(int index)
    {
        if(index == 0){ return 0; }
        if(index == 1){ return 0; }
        if(index > 1 && index < 38)
        {
            return index - 1;
        }
        return 0;
    }
    static int familyIndex(int family)
    {
        return family+1;
    }

public:
    explicit EditCreatureTemplateWidget(QSqlDatabase &db, entityviews::dbcs_module_type &m,
                                        dbccache *dbcC, dbcache * dbC, item_models *ims, QWidget *parent = 0);
    ~EditCreatureTemplateWidget();

    void LoadEntry(int entry);
    void SaveEntry(bool overwrite);

private slots:
    void on_OverwriteToDBButton_clicked();

    void on_AddToDBButton_clicked();

    void on_Delete_clicked();

private:
    Ui::EditCreatureTemplateWidget *ui;
    QSqlRecord record;
    QSqlDatabase& db;
    entityviews::dbcs_module_type& dbc;
    dbccache *                      m_dbccache;
    dbcache *                       m_dbcache;
    standing_animations_selecter *  emoteselecter;
    model_selecter *                modelselecter1;
    model_selecter *                modelselecter2;
    model_selecter *                modelselecter3;
    model_selecter *                modelselecter4;
    creature_type_selecter *        typeselecter;
    creature_family_selecter *      creaturefamilyselecter;
    expansion_selecter *            expansionselecter;
    faction_selecter *              factionAselecter;
    faction_selecter *              factionHselecter;
    inhabit_type_selecter *         inhabitselecter;
    trainer_type_selecter *         trainertypeselecter;
    trainer_race_selecter *         trainerraceselecter;
    trainer_class_selecter *        trainerclassselecter;
    spell_selecter *                trainerspellselecter;
    movement_type_selecter *        movementtypeselecter;
    mechanics_mask_selecter *       mechanicsimmuneselecter;
    creature_class_selecter *       creatureclassselecter;
    unit_dynamic_flags_selecter *   dynflagsselecter;
    unit_flags_selecter *           unitflagsselecter;
    creature_type_flags_selecter *  creaturetypeflagsselecter;
    flags_extra_selecter *          extraflagsselecter;
    npc_flags_selecter *            npcflagsselecter;
    pet_spell_selecter *            petspelldataidselecter;
    trainer_teach_edit *            trainerteaches;
    vendor_edit *                   vendoredit;
    item_model_selecter *           mainhandselecter;
    item_model_selecter *           offhandselecter;
    item_model_selecter *           rangedselecter;
    gossip_menu_selecter *          gossipselecter;
    creature_rank_selecter *        rankselecter;
    creature_template_selecter_button * heroicentryselecter;
    creature_template_selecter_button * killcredit1selecter;
    creature_template_selecter_button * killcredit2selecter;
    selecter_button *               creaturelootselecter;
    int oldmh,oldoh,oldr;
    void loadEquip(int id);
    int saveEquip();
    int m_tempi;
    QString makeOverwriteQuery();
    QString makeAddQuery();
    QString commonBody();
    void setEditDefaults();
    void updateMobDps();
    void updateMobRDps();
    void updateMobScore();
};

#endif // EDITCREATURETEMPLATEWIDGET_H
