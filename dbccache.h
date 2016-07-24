#ifndef DBCCACHE_H
#define DBCCACHE_H

#include <vector>
#include <array>
#include <set>
#include <algorithm>
#include <QString>
#include "dbc/dbcrecorddescriptors.h"

using namespace burning_crusade_records;

class QStringList;

/* Handle all resources derived from DBC-files here */


class dbccache
{
private:
    // Creature Family
    QStringList                     * creature_family_resource;
    std::vector<int>                * creature_family_references;
    void creature_family_init(entityviews::dbcs_module_type & dbc);
    bool creature_family_valid(entityviews::dbcs_module_type & dbc);
    // Creature Type
    QStringList                     * creature_type_resource;
    std::vector<int>                * creature_type_references;
    void creature_type_init(entityviews::dbcs_module_type & dbc);
    bool creature_type_valid(entityviews::dbcs_module_type & dbc);
    // Skill Line
    QStringList                     * skillline_resource;
    std::vector<int>                * skillline_references;
    void skillline_init(entityviews::dbcs_module_type & dbc);
    bool skillline_valid(entityviews::dbcs_module_type & dbc);
    // Faction
    QStringList                     * faction_resource;
    QStringList                     * faction_plain;
    std::vector<int>                * faction_references;
    void faction_init(entityviews::dbcs_module_type & dbc);
    bool faction_valid(entityviews::dbcs_module_type & dbc);
    // Reputation Faction
    QStringList                     * reputation_faction_resource;
    std::vector<int>                * reputation_faction_references;
    void reputation_faction_init(entityviews::dbcs_module_type & dbc);
    bool reputation_faction_valid(entityviews::dbcs_module_type & dbc);
    // Item Class
    QStringList                     * item_class_resource;
    std::vector<int>                * item_class_references;
    void item_class_init(entityviews::dbcs_module_type & dbc);
    bool item_class_valid(entityviews::dbcs_module_type & dbc);
    // Item Subclass
    std::vector<QStringList*>       * item_subclass_resource;
    std::vector<std::vector<int>*>  * item_subclass_references;
    void item_subclass_init(entityviews::dbcs_module_type & dbc);
    bool item_subclass_valid(entityviews::dbcs_module_type & dbc);
    // Map
    QStringList                     * map_resource;
    std::vector<int>                * map_references;
    void map_init(entityviews::dbcs_module_type & dbc);
    bool map_valid(entityviews::dbcs_module_type & dbc);
    // Area
    QStringList                     * area_resource;
    std::vector<int>                * area_references;
    void area_init(entityviews::dbcs_module_type & dbc);
    bool area_valid(entityviews::dbcs_module_type & dbc);
    // Model (creature/unit)
    QStringList                     * model_resource;
    QStringList                     * model_plain;
    std::vector<int>                * model_references;
    void model_init(entityviews::dbcs_module_type & dbc);
    bool model_valid(entityviews::dbcs_module_type & dbc);
    // Spell
    QStringList                     * spell_resource;
    std::vector<int>                * spell_references;
    void spell_init(entityviews::dbcs_module_type & dbc);
    bool spell_valid(entityviews::dbcs_module_type & dbc);
    // Standing Animation
    QStringList                     * standing_animation_resource;
    std::vector<int>                * standing_animation_references;
    void standing_animation_init(entityviews::dbcs_module_type & dbc);
    bool standing_animation_valid(entityviews::dbcs_module_type & dbc);
    // Pet Spell
    QStringList                     * pet_spell_resource;
    QStringList                     * pet_spell_plain;
    std::vector<int>                * pet_spell_references;
    void pet_spell_init(entityviews::dbcs_module_type & dbc);
    bool pet_spell_valid(entityviews::dbcs_module_type & dbc);
    // Item Extended Cost
    std::vector<int>                * item_extended_cost_references;
    std::set<int>                   * item_extended_cost_item_entries;
    void item_extended_cost_init(entityviews::dbcs_module_type & dbc);
    bool item_extended_cost_valid(entityviews::dbcs_module_type & dbc);
    // Item Model
    std::array<QStringList*,18>       item_model_resource;    // 18 is the number of slots (see item_model struct)
    std::array<QStringList*,18>       item_model_plain;
    std::array<std::vector<int>*,18>  item_model_item_references;
    std::array<std::vector<int>*,18>  item_model_display_references;
    void item_model_init(entityviews::dbcs_module_type & dbc);
    bool item_model_valid(entityviews::dbcs_module_type  &dbc);
    // Item Set
    QStringList                     * item_set_resource;
    std::vector<int>                * item_set_references;
    void item_set_init(entityviews::dbcs_module_type & dbc);
    bool item_set_valid(entityviews::dbcs_module_type  & dbc);
    // Title
    QStringList                     * title_resource;
    std::vector<int>                * title_references;
    void title_init(entityviews::dbcs_module_type & dbc);
    bool title_valid(entityviews::dbcs_module_type & dbc);
    // Item Enchant
    QStringList                     * item_enchant_resource;
    std::vector<int>                * item_enchant_references;
    void item_enchant_init(entityviews::dbcs_module_type & dbc);
    bool item_enchant_valid(entityviews::dbcs_module_type & dbc);
    // Language
    QStringList                     * language_resource;
    std::vector<int>                * language_references;
    void language_init(entityviews::dbcs_module_type & dbc);
    bool language_valid(entityviews::dbcs_module_type & dbc);
    // Totem Category
    QStringList                     * totem_category_resource;
    std::vector<int>                * totem_category_references;
    void totem_category_init(entityviews::dbcs_module_type & dbc);
    bool totem_category_valid(entityviews::dbcs_module_type & dbc);
    // Gem
    QStringList                     * gem_resource;
    std::vector<int>                * gem_references;
    void gem_init(entityviews::dbcs_module_type & dbc);
    bool gem_valid(entityviews::dbcs_module_type & dbc);
    // Lock Type
    std::set<int>                   * lock_type_item_entries;
    std::vector<int>                * lock_type_references;
    void lock_type_init(entityviews::dbcs_module_type & dbc);
    bool lock_type_valid(entityviews::dbcs_module_type & dbc);


    void loadDBC(entityviews::dbcs_module_type & dbc);
public:
    dbccache(entityviews::dbcs_module_type & dbc);
    ~dbccache();

    void reloadDBC(entityviews::dbcs_module_type & dbc);

public:
    QStringList                     * get_creature_family_resource(){ return creature_family_resource; }
    std::vector<int>                * get_creature_family_references(){ return creature_family_references; }
    QStringList                     * get_creature_type_resource(){ return creature_type_resource; }
    std::vector<int>                * get_creature_type_references(){ return creature_type_references; }
    QStringList                     * get_skillline_resource(){ return skillline_resource; }
    std::vector<int>                * get_skillline_references(){ return skillline_references; }
    QStringList                     * get_faction_resource(){ return faction_resource; }
    QStringList                     * get_faction_plain(){ return faction_plain; }
    std::vector<int>                * get_faction_references(){ return faction_references; }
    QStringList                     * get_reputation_faction_resource(){ return reputation_faction_resource; }
    std::vector<int>                * get_reputation_faction_references(){ return reputation_faction_references; }
    QStringList                     * get_item_class_resource(){ return item_class_resource; }
    std::vector<int>                * get_item_class_references(){ return item_class_references; }
    std::vector<QStringList*>       * get_item_subclass_resource(){ return item_subclass_resource; }
    std::vector<std::vector<int>*>  * get_item_subclass_references(){ return item_subclass_references; }
    QStringList                     * get_map_resource(){ return map_resource; }
    std::vector<int>                * get_map_references(){ return map_references; }
    QStringList                     * get_area_resource(){ return area_resource; }
    std::vector<int>                * get_area_references(){ return area_references; }
    QStringList                     * get_model_resource(){ return model_resource; }
    QStringList                     * get_model_plain(){ return model_plain; }
    std::vector<int>                * get_model_references(){ return model_references; }
    QStringList                     * get_spell_resource(){ return spell_resource; }
    std::vector<int>                * get_spell_references(){ return spell_references; }
    QStringList                     * get_standing_animation_resource(){ return standing_animation_resource; }
    std::vector<int>                * get_standing_animation_references(){ return standing_animation_references; }
    QStringList                     * get_pet_spell_resource(){ return pet_spell_resource; }
    QStringList                     * get_pet_spell_plain(){ return pet_spell_plain; }
    std::vector<int>                * get_pet_spell_references(){ return pet_spell_references; }
    std::vector<int>                * get_item_extended_cost_references(){ return item_extended_cost_references; }
    std::set<int>                   * get_item_extended_cost_item_entries(){ return item_extended_cost_item_entries; }
    QStringList                     * get_item_model_resource(int slot){ return item_model_resource[slot]; }
    QStringList                     * get_item_model_plain(int slot){ return item_model_plain[slot]; }
    std::vector<int>                * get_item_model_item_references(int slot){ return item_model_item_references[slot]; }
    std::vector<int>                * get_item_model_display_references(int slot){ return item_model_display_references[slot]; }
    QStringList                     * get_item_set_resource(){ return item_set_resource; }
    std::vector<int>                * get_item_set_references(){ return item_set_references; }
    QStringList                     * get_title_resource(){ return title_resource; }
    std::vector<int>                * get_title_references(){ return title_references; }
    QStringList                     * get_item_enchant_resource(){ return item_enchant_resource; }
    std::vector<int>                * get_item_enchant_references(){ return item_enchant_references; }
    QStringList                     * get_language_resource(){ return language_resource; }
    std::vector<int>                * get_language_references(){ return language_references; }
    QStringList                     * get_totem_category_resource(){ return totem_category_resource; }
    std::vector<int>                * get_totem_category_references(){ return totem_category_references; }
    QStringList                     * get_gem_resource(){ return gem_resource; }
    std::vector<int>                * get_gem_references(){ return gem_references; }
    std::vector<int>                * get_lock_type_references(){ return lock_type_references; }
    std::set<int>                   * get_lock_type_item_entries(){ return lock_type_item_entries; }

public:
    static int find_ref(std::vector<int> * v, int ref)
    {
        int index;
        auto it =  std::find(v->begin(),
                             v->end(), ref);
        if(it == v->end())
        {
            index = -1;
        }
        else
        {
            index = std::distance(v->begin(),
                                  it);
        }
        return index;
    }

private:

    template <typename T>
    void free(std::vector<T*>* v)
    {
        std::for_each(v->begin(),v->end(),[](T * t){ delete t;});
    }

    struct faction
    {
        enum FactionTemplateFlags // copied from mangos
        {
            FACTION_TEMPLATE_FLAG_PVP               = 0x00000800,   // flagged for PvP
            FACTION_TEMPLATE_FLAG_CONTESTED_GUARD   = 0x00001000,   // faction will attack players that were involved in PvP combats
        };
        enum FactionMasks         // copied from mangos
        {
            FACTION_MASK_PLAYER   = 1,                              // any player
            FACTION_MASK_ALLIANCE = 2,                              // player or creature from alliance team
            FACTION_MASK_HORDE    = 4,                              // player or creature from horde team
            FACTION_MASK_MONSTER  = 8                               // aggressive creature from monster team
            // if none flags set then non-aggressive creature
        };
        QString displayMask(int mask);
        QString displayFlags(int flags);
    };

    struct pet_spell
    {
        typedef entityviews::creaturespell1 spell1;
        typedef entityviews::creaturespell2 spell2;
        typedef entityviews::creaturespell3 spell3;
        typedef entityviews::spellname spellname;
        QString getDisplay(int index, entityviews::dbcs_module_type &dbc)
        {
            QString tmp("");
            int a;
            int numberofspells = 0;
            if((a = static_cast<spell1*>(&dbc)->get_label(index)) != 0)
            {
                numberofspells++;
                a = static_cast<spellname*>(&dbc)->find_ref(a);
                tmp += QString(static_cast<spellname*>(&dbc)->get_label(a));
            }
            if((a = static_cast<spell2*>(&dbc)->get_label(index)) != 0)
            {
                if(numberofspells == 1)
                {
                    tmp += ", ";
                }
                numberofspells++;
                a = static_cast<spellname*>(&dbc)->find_ref(a);
                tmp += QString(static_cast<spellname*>(&dbc)->get_label(a));
            }
            if((a = static_cast<spell3*>(&dbc)->get_label(index)) != 0)
            {
                tmp += ", ";
                a = static_cast<spellname*>(&dbc)->find_ref(a);
                tmp += QString(static_cast<spellname*>(&dbc)->get_label(a));
            }
            if(tmp.length() == 0)
            {
                tmp = "None";
            }
            return tmp;
        }

        QString getDisplayShort(int index,entityviews::dbcs_module_type &dbc)
        {
            QString tmp("");
            int a;
            int numberofspells = 0;
            if((a = static_cast<spell1*>(&dbc)->get_label(index)) != 0)
            {
                numberofspells++;
                a = static_cast<spellname*>(&dbc)->find_ref(a);
                tmp += QString(static_cast<spellname*>(&dbc)->get_label(a));
            }
            if((a = static_cast<spell2*>(&dbc)->get_label(index)) != 0)
            {
                if(numberofspells == 1)
                {
                    tmp += ", ";
                }
                numberofspells++;
                a = static_cast<spellname*>(&dbc)->find_ref(a);
                tmp += QString(static_cast<spellname*>(&dbc)->get_label(a));
            }
            if((a = static_cast<spell3*>(&dbc)->get_label(index)) != 0)
            {
                numberofspells++;
                tmp += ", ";
                a = static_cast<spellname*>(&dbc)->find_ref(a);
                tmp += QString(static_cast<spellname*>(&dbc)->get_label(a));
            }
            return tmp;
        }
    };

    struct item_model
    {
        typedef entityviews::itemleftmodel      leftmodel;
        typedef entityviews::itemrightmodel     rightmodel;
        typedef entityviews::itemlefttexture    lefttexture;
        typedef entityviews::itemrighttexture   righttexture;
        typedef entityviews::upperarmtexture    uarmtexture;
        typedef entityviews::lowerarmtexture    larmtexture;
        typedef entityviews::handtexture        handtexture;
        typedef entityviews::uppertorsotexture  utorsotexture;
        typedef entityviews::lowertorsotexture  ltorsotexture;
        typedef entityviews::upperlegtexture    ulegtexture;
        typedef entityviews::lowerlegtexture    llegtexture;
        typedef entityviews::foottexture        foottexture;
        typedef entityviews::itemicon           icon;
        bool isMainHandSlot(int dbcslot);
        bool isOffHandSlot(int dbcslot);
        bool isRangedSlot(int dbcslot);
        bool isChestSlot(int dbcslot);
        QString displayItemEntry(int displayid,entityviews::dbcs_module_type &dbc);
        QString displayItemEntryShort(int displayid,entityviews::dbcs_module_type &dbc);
        bool isCorrectSlot(int slot, int dbcslot);

        enum itemslot{
            SLOT_NONE = 1,
            SLOT_MAINHAND = 2,
            SLOT_OFFHAND = 3,
            SLOT_RANGED = 4,
            SLOT_HEAD = 5,
            SLOT_NECK = 6,
            SLOT_SHOULDERS = 7,
            SLOT_BACK = 8,
            SLOT_CHEST = 9,
            SLOT_SHIRT = 10,
            SLOT_TABARD = 11,
            SLOT_WRISTS = 12,
            SLOT_HANDS = 13,
            SLOT_WAIST = 14,
            SLOT_LEGS = 15,
            SLOT_FEET = 16,
            SLOT_RING = 17,
            SLOT_TRINKET = 18
        };
        enum itemslotdbc{
            DBC_SLOT_NONE = 0,
            DBC_SLOT_HEAD = 1,
            DBC_SLOT_NECK = 2,
            DBC_SLOT_SHOULDERS = 3,
            DBC_SLOT_SHIRT = 4,
            DBC_SLOT_VEST = 5,
            DBC_SLOT_WAIST = 6,
            DBC_SLOT_LEGS = 7,
            DBC_SLOT_FEET = 8,
            DBC_SLOT_WRISTS = 9,
            DBC_SLOT_HANDS = 10,
            DBC_SLOT_RING = 11,
            DBC_SLOT_TRINKET = 12,
            DBC_SLOT_ONEHAND = 13,
            DBC_SLOT_SHIELD = 14,
            DBC_SLOT_BOW = 15,
            DBC_SLOT_BACK = 16,
            DBC_SLOT_TWOHAND = 17,
            DBC_SLOT_BAG = 18,
            DBC_SLOT_TABARD = 19,
            DBC_SLOT_ROBE = 20,
            DBC_SLOT_MAINHAND = 21,
            DBC_SLOT_OFFHAND = 22,
            DBC_SLOT_HELD = 23,
            DBC_SLOT_AMMO = 24,
            DBC_SLOT_THROWN = 25,
            DBC_SLOT_RANGED = 26,
            DBC_SLOT_RANGED_ = 27,
            DBC_SLOT_RELIC = 28
        };
    };
};



#endif // DBCCACHE_H
