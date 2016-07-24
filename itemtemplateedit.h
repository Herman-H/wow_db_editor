#ifndef ITEMTEMPLATEEDIT_H
#define ITEMTEMPLATEEDIT_H

#include <QWidget>
#include <QWidgetList>
#include <vector>
#include "widgets/item_models.h"
#include "dbc/dbcrecorddescriptors.h"
using namespace burning_crusade_records;

class QSqlDatabase;
class QHBoxLayout;
class QLineEdit;
class item_template_selecter_button;
class item_class_selecter;
class item_subclass_selecter;
class item_quality_selecter;
class item_slot_selecter;
class class_mask_selecter;
class race_mask_selecter;
class skillline_selecter;
class honor_rank_selecter;
class spell_selecter;
class reputation_faction_selecter;
class reputation_rank_selecter;
class map_selecter;
class area_selecter;
class item_set_selecter;
class item_stat_type_selecter;
class item_sheathe_selecter;
class socket_color_selecter;
class item_enchant_selecter;
class damage_school_selecter;
class ammo_type_selecter;
class spell_trigger_selecter;
class item_model_selecter;
class language_selecter;
class page_material_selecter;
class material_selecter;
class bag_family_mask_selecter;
class totem_category_selecter;
class gem_selecter;
class food_type_selecter;
class disenchant_loot_selecter;
class item_random_property_selecter;
class item_flags_selecter;
class lock_selecter;
class selecter_button;
class dbccache;
class dbcache;

namespace Ui {
class itemtemplateedit;
}

class itemtemplateedit : public QWidget
{
    Q_OBJECT
private:
    QSqlDatabase &                  db;
    entityviews::dbcs_module_type & module;
    item_template_selecter_button * itemselecter;
    item_class_selecter *           classselecter;
    item_subclass_selecter *        subclassselecter;
    item_quality_selecter *         qualityselecter;
    item_slot_selecter *            slotselecter;
    item_set_selecter *             itemsetselecter;
    item_model_selecter *           modelselect;
    class_mask_selecter *           classmask;
    race_mask_selecter *            racemask;
    map_selecter *                  reqmapselecter;
    area_selecter *                 reqareaselecter;
    skillline_selecter *            skillselecter;
    spell_selecter *                spellselecter;
    honor_rank_selecter *           honorselecter;
    reputation_faction_selecter *   repfactionselecter;
    reputation_rank_selecter *      reprankselecter;
    item_sheathe_selecter *         sheatheselecter;
    socket_color_selecter *         socketselecter[3];
    item_enchant_selecter *         socketbonusselecter;

    spell_selecter *                spell1selecter;
    spell_selecter *                spell2selecter;
    spell_selecter *                spell3selecter;
    spell_selecter *                spell4selecter;
    spell_selecter *                spell5selecter;
    spell_trigger_selecter *        spelltriggerselecter[5];

    item_stat_type_selecter *       stats[10];
    QLineEdit *                     statsamount[10];
    item_random_property_selecter * randompropertyselecter;
    item_random_property_selecter * randomsuffixselecter;

    damage_school_selecter *        wpndmgtype[5];
    float                           dmgMin[5];
    float                           dmgMax[5];
    int                             armorDamageModifier;

    ammo_type_selecter *            ammotypeselecter;
    food_type_selecter *            foodtypeselecter;
    gem_selecter *                  gemselecter;

    selecter_button *               bookselecter;
    language_selecter *             languageselecter;
    page_material_selecter *        pagematerialselecter;
    material_selecter *             materialselecter;

    bag_family_mask_selecter *      bagfamilyselecter;
    lock_selecter *                 lockselecter;

    totem_category_selecter *       totemselecter;
    disenchant_loot_selecter *      delootselecter;

    item_flags_selecter *           flagsselecter;

    dbccache *                      m_dbccache;
    dbcache *                       m_dbcache;

public:
    explicit itemtemplateedit(QSqlDatabase &db, entityviews::dbcs_module_type &dbc, dbccache * dbcC, dbcache * dbC, item_models * ims, QWidget *parent = 0);
    ~itemtemplateedit();

private:
    Ui::itemtemplateedit *ui;
    // INSERT INTO item_template AS "alias" ("currentQueryFields("alias")") VALUES("currentQueryValues("alias",b=false)");
    QString currentQueryFields();
    QString currentQueryValues();
    void setEditDefaults();
    void setEditStatsDefault();
    void initWeaponView();
    void updateWeaponView();
    float getItemValue();
    double ilvlToDps(double x);
    double dpsToIlvl(double x);
private slots:
    void onLoadEntry();

    void onItemClassChange();
    void onItemSlotChange();
    void onPropertyChange();
    void onSuffixChange();
    void onWeaponModification();

    void on_spell1Clr_clicked();
    void on_spell2Clr_clicked();
    void on_spell3Clr_clicked();
    void on_spell4Clr_clicked();
    void on_spell5Clr_clicked();

    void onSpell1Selection();
    void onSpell2Selection();
    void onSpell3Selection();
    void onSpell4Selection();
    void onSpell5Selection();
    void on_spinBox_valueChanged(int arg1);
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_overwriteButton_clicked();
};

#endif // ITEMTEMPLATEEDIT_H
