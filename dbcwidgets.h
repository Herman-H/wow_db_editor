#ifndef DBCWIDGETS_H
#define DBCWIDGETS_H

#include <QObject>
#include <QString>
#include "dbc/dbcrecorddescriptors.h"
#include <tuple>
#include <vector>
#include "cachetypes.h"

class QWidget;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QDialog;
class QListWidget;
class QLabel;
class QListWidgetItem;
class QSqlDatabase;
class QSqlRecord;
class QSqlField;
class QSqlError;
class QLineEdit;
class dbccache;

using namespace burning_crusade_records;

class creature_family_selecter
{
private:
    QComboBox                       * widget;
    dbccache                        * m_dbccache;
public:
    creature_family_selecter(dbccache *c);
    ~creature_family_selecter();
    QComboBox * getWidget();
    void setIndexFromId(int id);
    void reload(dbc_resource_type r);
    int selectedId();
};

class creature_type_selecter
{
private:
    QComboBox                       * widget;
    dbccache                        * m_dbccache;
public:
    creature_type_selecter(dbccache *c);
    ~creature_type_selecter();
    QComboBox * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
};

class skillline_selecter
{
private:
    dbccache  * m_dbccache;
    QComboBox * widget;
public:
    skillline_selecter(dbccache * c);
    ~skillline_selecter();
    QComboBox * getWidget();
    void clear();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
};

class faction_dialog : public QObject
{
    Q_OBJECT
private:
    QVBoxLayout *        layout;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog *            widget;
    dbccache *           m_dbccache;
public:
    faction_dialog(dbccache * c);
    ~faction_dialog();
    void setIndex(int index);
    void reload(dbc_resource_type r);
    QDialog * getWidget();
public slots:
    void clearButtonPushed();
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class faction_selecter : public QObject
{
    Q_OBJECT
private:
    int                  selected;
    faction_dialog *     dialog;
    QPushButton *        widget;
    dbccache *           m_dbccache;
public:
    faction_selecter(dbccache *c);
    ~faction_selecter();
    QPushButton * getWidget();
    void setIndexFromId(int id);
    void reload(dbc_resource_type r);
    int selectedId();
    int currentIndex();

public slots:

    void buttonPushed();
    void clear();

};

class reputation_faction_selecter
{
private:
    QComboBox * widget;
    dbccache *  m_dbccache;
public:
    reputation_faction_selecter(dbccache * c);
    ~reputation_faction_selecter();
    QComboBox * getWidget();
    int selectedId();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    void clear();
};

class reputation_rank_selecter
{
private:
    QComboBox * widget;
public:
    reputation_rank_selecter();
    ~reputation_rank_selecter();
    QComboBox * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class map_dialog : public QObject
{
    Q_OBJECT
private:
    QVBoxLayout *        layout;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog     *        widget;
    dbccache    *        m_dbccache;
public:
    map_dialog(dbccache * c);
    ~map_dialog();
    QDialog * getWidget();
    void setIndex(int index);
    void reload(dbc_resource_type r);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class map_selecter : public QObject
{
    Q_OBJECT
private:
    int                  selected;
    QPushButton *        widget;
    map_dialog *         dialog;
    dbccache *           m_dbccache;
public:
    map_selecter(dbccache *c);
    ~map_selecter();
    QPushButton * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
public slots:
    void buttonPushed();
    void clear();
};

class area_dialog : public QObject
{
    Q_OBJECT
private:
    dbccache    *        m_dbccache;
    QVBoxLayout *        layout;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog     *        widget;
public:
    area_dialog(dbccache *c);
    ~area_dialog();
    QDialog * getWidget();
    void reload(dbc_resource_type r);
    void setIndex(int index);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class area_selecter : public QObject
{
    Q_OBJECT
private:
    dbccache    *        m_dbccache;
    int                  selected;
    QPushButton *        widget;
    area_dialog *        dialog;
public:
    area_selecter(dbccache * c);
    ~area_selecter();
    QPushButton * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
public slots:
    void buttonPushed();
    void clear();
};


class creature_rank_selecter
{
private:
    enum class Rank
    {
        Normal = 0,
        Rare = 4,
        Elite = 1,
        RareElite = 2,
        Boss = 3,
        Unspecified = 5
    };
    static int indexToRank(int index)
    {
        switch(index)
        {
        case 0:
            return 5;
        case 1:
            return 0;
        case 2:
            return 4;
        case 3:
            return 1;
        case 4:
            return 2;
        case 5:
            return 3;
        }
        return -1;
    }
    static int rankToIndex(int rank)
    {
        switch(rank)
        {
        case 5:
            return 0;
        case 0:
            return 1;
        case 4:
            return 2;
        case 1:
            return 3;
        case 2:
            return 4;
        case 3:
            return 5;
        }
        return 0;
    }
    QComboBox * widget;
public:
    creature_rank_selecter();
    ~creature_rank_selecter();
    QComboBox * getWidget();
    int selectedId();
    void clear();
    void setIndexFromId(int id);
};

class inhabit_type_selecter
{
private:
    enum InhabitTypeValues
    {
        INHABIT_GROUND = 1,
        INHABIT_WATER  = 2,
        INHABIT_AIR    = 4,
        INHABIT_ANYWHERE = INHABIT_GROUND | INHABIT_WATER | INHABIT_AIR
    };
    QWidget * widget;
    QHBoxLayout * layout;
    QCheckBox * ground;
    QCheckBox * water;
    QCheckBox * air;

public:
    inhabit_type_selecter();
    ~inhabit_type_selecter();
    QWidget * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};


class flags_extra_selecter : public QObject
{
    Q_OBJECT
private:
    enum CreatureFlagsExtra
    {
        CREATURE_FLAG_EXTRA_INSTANCE_BIND   = 0x00000001,       // creature kill bind instance with killer and killer's group
        CREATURE_FLAG_EXTRA_CIVILIAN        = 0x00000002,       // not aggro (ignore faction/reputation hostility)
        CREATURE_FLAG_EXTRA_NO_PARRY        = 0x00000004,       // creature can't parry
        CREATURE_FLAG_EXTRA_NO_PARRY_HASTEN = 0x00000008,       // creature can't counter-attack at parry
        CREATURE_FLAG_EXTRA_NO_BLOCK        = 0x00000010,       // creature can't block
        CREATURE_FLAG_EXTRA_NO_CRUSH        = 0x00000020,       // creature can't do crush attacks
        CREATURE_FLAG_EXTRA_NO_XP_AT_KILL   = 0x00000040,       // creature kill not provide XP
        CREATURE_FLAG_EXTRA_INVISIBLE       = 0x00000080,       // creature is always invisible for player (mostly trigger creatures)
        CREATURE_FLAG_EXTRA_NOT_TAUNTABLE   = 0x00000100,       // creature is immune to taunt auras and effect attack me
        CREATURE_FLAG_EXTRA_AGGRO_ZONE      = 0x00000200,       // creature sets itself in combat with zone on aggro
        CREATURE_FLAG_EXTRA_GUARD           = 0x00000400,       // creature is a guard
    };
    QHBoxLayout * outer_layout;
    QVBoxLayout * inner_left_layout;
    QVBoxLayout * inner_right_layout;
    QWidget *     widget;
    QCheckBox   * enable;
    QCheckBox   * bindsToInstance;
    QCheckBox   * noAggroRadius;
    QCheckBox   * cantParry;
    QCheckBox   * noParryHaste;
    QCheckBox   * cantBlock;
    QCheckBox   * cantCrush;
    QCheckBox   * noXpReward;
    QCheckBox   * isInvisible;
    QCheckBox   * zoneAggro;
    QCheckBox   * isGuard;
    const bool hasEnableOption;
public:
    flags_extra_selecter(bool hasEnableOption);
    ~flags_extra_selecter();
    QWidget * getWidget();
    void clear();
    bool selectedIsUsed();
    int selectedId();
    void setIndexFromId(int id);
public slots:
    void toggleEnabled(int e);
};

class npc_flags_selecter
{
private:
    /// Non Player Character flags, copied from MaNGOS, dont use flags here which are not understood
    enum NPCFlags
    {
        UNIT_NPC_FLAG_NONE                  = 0x00000000,
        UNIT_NPC_FLAG_GOSSIP                = 0x00000001,       // 100%
        UNIT_NPC_FLAG_QUESTGIVER            = 0x00000002,       // guessed, probably ok
        UNIT_NPC_FLAG_UNK1                  = 0x00000004,
        UNIT_NPC_FLAG_UNK2                  = 0x00000008,
        UNIT_NPC_FLAG_TRAINER               = 0x00000010,       // 100%
        UNIT_NPC_FLAG_TRAINER_CLASS         = 0x00000020,       // 100%
        UNIT_NPC_FLAG_TRAINER_PROFESSION    = 0x00000040,       // 100%
        UNIT_NPC_FLAG_VENDOR                = 0x00000080,       // 100%
        UNIT_NPC_FLAG_VENDOR_AMMO           = 0x00000100,       // 100%, general goods vendor
        UNIT_NPC_FLAG_VENDOR_FOOD           = 0x00000200,       // 100%
        UNIT_NPC_FLAG_VENDOR_POISON         = 0x00000400,       // guessed
        UNIT_NPC_FLAG_VENDOR_REAGENT        = 0x00000800,       // 100%
        UNIT_NPC_FLAG_REPAIR                = 0x00001000,       // 100%
        UNIT_NPC_FLAG_FLIGHTMASTER          = 0x00002000,       // 100%
        UNIT_NPC_FLAG_SPIRITHEALER          = 0x00004000,       // guessed
        UNIT_NPC_FLAG_SPIRITGUIDE           = 0x00008000,       // guessed
        UNIT_NPC_FLAG_INNKEEPER             = 0x00010000,       // 100%
        UNIT_NPC_FLAG_BANKER                = 0x00020000,       // 100%
        UNIT_NPC_FLAG_PETITIONER            = 0x00040000,       // 100% 0xC0000 = guild petitions, 0x40000 = arena team petitions
        UNIT_NPC_FLAG_TABARDDESIGNER        = 0x00080000,       // 100%
        UNIT_NPC_FLAG_BATTLEMASTER          = 0x00100000,       // 100%
        UNIT_NPC_FLAG_AUCTIONEER            = 0x00200000,       // 100%
        UNIT_NPC_FLAG_STABLEMASTER          = 0x00400000,       // 100%
        UNIT_NPC_FLAG_GUILD_BANKER          = 0x00800000,       // cause client to send 997 opcode
        UNIT_NPC_FLAG_SPELLCLICK            = 0x01000000,       // cause client to send 1015 opcode (spell click), dynamic, set at loading and don't must be set in DB
    };

    QCheckBox * hasGossip;
    QCheckBox * isQuestgiver;
    QCheckBox * isTrainer;
    QCheckBox * isClassTrainer;
    QCheckBox * isProfessionTrainer;
    QCheckBox * isVendor;
    QCheckBox * isAmmoVendor;
    QCheckBox * isFoodVendor;
    QCheckBox * isPoisonVendor;
    QCheckBox * isReagentVendor;
    QCheckBox * isRepairer;
    QCheckBox * isFlightmaster;
    QCheckBox * isSpiritHealer;
    QCheckBox * isSpiritGuide;
    QCheckBox * isInnkeeper;
    QCheckBox * isBanker;
    QCheckBox * isTabardDesigner;
    QCheckBox * isBattlemaster;
    QCheckBox * isAuctioneer;
    QCheckBox * isStablemaster;

    QWidget *   widget;

    QHBoxLayout * outer_layout;
    QVBoxLayout * inner_left_layout;
    QVBoxLayout * inner_right_layout;
public:

    npc_flags_selecter();
    ~npc_flags_selecter();
    QWidget * getWidget();
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};


class trainer_type_selecter
{
private:
    // copied from mangos
    enum TrainerType                                            // this is important type for npcs!
    {
        TRAINER_TYPE_CLASS             = 0,
        TRAINER_TYPE_MOUNTS            = 1,                     // on blizz it's 2
        TRAINER_TYPE_TRADESKILLS       = 2,
        TRAINER_TYPE_PETS              = 3
    };
    QComboBox * widget;
    int indexToId(int index);
    int idToIndex(int id);
public:
    trainer_type_selecter();
    ~trainer_type_selecter();
    QComboBox * getWidget();
    int selectedId();
    void clear();
    void setIndexFromId(int id);
};


class trainer_race_selecter
{
private:
    QComboBox * widget;
public:
    trainer_race_selecter();
    ~trainer_race_selecter();
    QComboBox * getWidget();
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};

class trainer_class_selecter
{
private:
    QComboBox * widget;
public:
    trainer_class_selecter();
    ~trainer_class_selecter();
    QComboBox * getWidget();
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};

class expansion_selecter
{
private:
    QComboBox * widget;
public:
    expansion_selecter();
    ~expansion_selecter();
    QComboBox * getWidget();
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};

class movement_type_selecter
{
private:
    enum MovementGeneratorType// copied from mangos
    {
        IDLE_MOTION_TYPE                = 0,                    // IdleMovementGenerator.h
        RANDOM_MOTION_TYPE              = 1,                    // RandomMovementGenerator.h
        WAYPOINT_MOTION_TYPE            = 2                    // WaypointMovementGenerator.h
    };
    QComboBox * widget;
public:
    movement_type_selecter();
    ~movement_type_selecter();
    QComboBox * getWidget();
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};


class mechanics_mask_selecter : public QObject
{
    Q_OBJECT
private:
    // Spell mechanics, copied from mangos. Seem to cohere with mechanic_immune_mask column in database
    enum Mechanics
    {
        MECHANIC_NONE             = 0,
        MECHANIC_CHARM            = 1,
        MECHANIC_DISORIENTED      = 2,
        MECHANIC_DISARM           = 3,
        MECHANIC_DISTRACT         = 4,
        MECHANIC_FEAR             = 5,
        MECHANIC_FUMBLE           = 6,                          // Decreases chance on hit
        MECHANIC_ROOT             = 7,
        MECHANIC_PACIFY           = 8,                          // No spells use this mechanic
        MECHANIC_SILENCE          = 9,
        MECHANIC_SLEEP            = 10,
        MECHANIC_SNARE            = 11,
        MECHANIC_STUN             = 12,
        MECHANIC_FREEZE           = 13,
        MECHANIC_KNOCKOUT         = 14,
        MECHANIC_BLEED            = 15,
        MECHANIC_BANDAGE          = 16,
        MECHANIC_POLYMORPH        = 17,
        MECHANIC_BANISH           = 18,
        MECHANIC_SHIELD           = 19,
        MECHANIC_SHACKLE          = 20,
        MECHANIC_MOUNT            = 21,
        MECHANIC_PERSUADE         = 22,                         //0 spells use this mechanic
        MECHANIC_TURN             = 23,
        MECHANIC_HORROR           = 24,
        MECHANIC_INVULNERABILITY  = 25,
        MECHANIC_INTERRUPT        = 26,
        MECHANIC_DAZE             = 27,
        MECHANIC_DISCOVERY        = 28,
        MECHANIC_IMMUNE_SHIELD    = 29,                         // Divine (Blessing) Shield/Protection and Ice Block
        MECHANIC_SAPPED           = 30
    };
    QWidget * widget;
    QCheckBox *box[30];
    QCheckBox *enable;
    QHBoxLayout * outer_layout;
    QVBoxLayout * inner_left_layout;
    QVBoxLayout * inner_middle_layout;
    QVBoxLayout * inner_right_layout;
    const bool hasEnableOption;
public:
    mechanics_mask_selecter(bool hasEnableOption);
    ~mechanics_mask_selecter();
    QWidget *getWidget();
    bool selectedIsUsed();
    int selectedId();
    void clear();
    void setIndexFromId(int id);
public slots:
    void toggleEnabled(int e);
};


class creature_class_selecter
{
private:
    static int idToIndex(int id)
    {
        switch(id)
        {
        case 0: return 1;
        case 1: return 2;
        case 4: return 3;
        case 2: return 4;
        case 8: return 5;
        }
        return 0;
    }
    static int indexToId(int index)
    {
        switch(index)
        {
        case 0: return 0;
        case 1: return 0;
        case 2: return 1;
        case 3: return 4;
        case 4: return 2;
        case 5: return 8;
        }
        return 0;
    }
    QComboBox * widget;
public:

    creature_class_selecter();
    ~creature_class_selecter();
    QComboBox * getWidget();
    int selectedId();
    void clear();
    void setIndexFromId(int id);
};


class unit_dynamic_flags_selecter
{
private:
    enum UnitDynFlags
    {
        UNIT_DYNFLAG_NONE                       = 0x0000,
        UNIT_DYNFLAG_LOOTABLE                   = 0x0001,
        UNIT_DYNFLAG_TRACK_UNIT                 = 0x0002,
        UNIT_DYNFLAG_TAPPED                     = 0x0004,       // Lua_UnitIsTapped
        UNIT_DYNFLAG_ROOTED                     = 0x0008,
        UNIT_DYNFLAG_SPECIALINFO                = 0x0010,
        UNIT_DYNFLAG_DEAD                       = 0x0020,
    };
    QWidget * widget;
    QCheckBox *box[6];
    QHBoxLayout * outer_layout;
    QVBoxLayout * inner_right_layout;
    QVBoxLayout * inner_left_layout;
public:

    unit_dynamic_flags_selecter();
    ~unit_dynamic_flags_selecter();
    QWidget * getWidget();
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};


class unit_flags_selecter
{
private:
    enum UnitFlags
    {
        //UNIT_FLAG_UNK_0                 = 0x00000001,
        UNIT_FLAG_NON_ATTACKABLE        = 0x00000002,           // not attackable
        UNIT_FLAG_DISABLE_MOVE          = 0x00000004,
        //UNIT_FLAG_PVP_ATTACKABLE        = 0x00000008,           // allow apply pvp rules to attackable state in addition to faction dependent state
        UNIT_FLAG_RENAME                = 0x00000010,
        UNIT_FLAG_PREPARATION           = 0x00000020,           // don't take reagents for spells with SPELL_ATTR_EX5_NO_REAGENT_WHILE_PREP
        //UNIT_FLAG_UNK_6                 = 0x00000040,
        //UNIT_FLAG_NOT_ATTACKABLE_1      = 0x00000080,           // ?? (UNIT_FLAG_PVP_ATTACKABLE | UNIT_FLAG_NOT_ATTACKABLE_1) is NON_PVP_ATTACKABLE
        UNIT_FLAG_OOC_NOT_ATTACKABLE    = 0x00000100,           // 2.0.8 - (OOC Out Of Combat) Can not be attacked when not in combat. Removed if unit for some reason enter combat (flag probably removed for the attacked and it's party/group only)
        UNIT_FLAG_PASSIVE               = 0x00000200,           // makes you unable to attack everything. Almost identical to our "civilian"-term. Will ignore it's surroundings and not engage in combat unless "called upon" or engaged by another unit.
        UNIT_FLAG_LOOTING               = 0x00000400,           // loot animation
        UNIT_FLAG_PET_IN_COMBAT         = 0x00000800,           // in combat?, 2.0.8
        UNIT_FLAG_PVP                   = 0x00001000,
        UNIT_FLAG_SILENCED              = 0x00002000,           // silenced, 2.1.1
        //UNIT_FLAG_UNK_14                = 0x00004000,           // 2.0.8
        //UNIT_FLAG_UNK_15                = 0x00008000,
        //UNIT_FLAG_UNK_16                = 0x00010000,           // removes attackable icon
        UNIT_FLAG_PACIFIED              = 0x00020000,
        UNIT_FLAG_STUNNED               = 0x00040000,           // stunned, 2.1.1
        UNIT_FLAG_IN_COMBAT             = 0x00080000,
        UNIT_FLAG_TAXI_FLIGHT           = 0x00100000,           // disable casting at client side spell not allowed by taxi flight (mounted?), probably used with 0x4 flag
        UNIT_FLAG_DISARMED              = 0x00200000,           // disable melee spells casting..., "Required melee weapon" added to melee spells tooltip.
        UNIT_FLAG_CONFUSED              = 0x00400000,
        UNIT_FLAG_FLEEING               = 0x00800000,
        UNIT_FLAG_PLAYER_CONTROLLED     = 0x01000000,           // used in spell Eyes of the Beast for pet... let attack by controlled creature
        UNIT_FLAG_NOT_SELECTABLE        = 0x02000000,
        UNIT_FLAG_SKINNABLE             = 0x04000000,
        UNIT_FLAG_MOUNT                 = 0x08000000,
        //UNIT_FLAG_UNK_28                = 0x10000000,
        //UNIT_FLAG_UNK_29                = 0x20000000,           // used in Feing Death spell
        UNIT_FLAG_SHEATHE               = 0x40000000
        // UNIT_FLAG_UNK_31              = 0x80000000           // no affect in 2.4.3
    };
    QHBoxLayout * outer_layout;
    QVBoxLayout * inner_left_layout;
    QVBoxLayout * inner_middle_layout;
    QVBoxLayout * inner_right_layout;

    QCheckBox *box[22];
    QWidget * widget;
public:
    unit_flags_selecter();
    ~unit_flags_selecter();
    QWidget * getWidget();
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};

class model_dialog : public QObject
{
    Q_OBJECT
private:
    dbccache    *        m_dbccache;
    QVBoxLayout *        layout;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog*             widget;

public:
    model_dialog(dbccache * c);
    ~model_dialog();
    QDialog * getWidget();
    void reload(dbc_resource_type r);
    void setIndex(int index);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class model_selecter : public QObject
{
    Q_OBJECT
private:
    dbccache    *        m_dbccache;
    int                  selected;
    model_dialog *       dialog;
    QPushButton *        widget;
public:
    model_selecter(dbccache * c);
    ~model_selecter();
    QPushButton * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
public slots:
    void buttonPushed();
    void clear();
};

class spell_dialog : public QObject
{
    Q_OBJECT
private:
    dbccache    *        m_dbccache;
    QVBoxLayout *        layout;
    QHBoxLayout *        tlayout;
    QLabel      *        filter;
    QLineEdit   *        filterEdit;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog *            widget;

public:
    spell_dialog(dbccache * c);
    ~spell_dialog();
    QDialog * getWidget();
    void reload(dbc_resource_type r);
    void setIndex(int index);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
    void filterTextChanged();
};


class spell_selecter : public QObject
{
    Q_OBJECT
private:
    dbccache *           m_dbccache;
    int                  selected;
    spell_dialog *       dialog;
    QPushButton *        widget;
    bool                 isclear;
public:
    spell_selecter(dbccache * c);
    ~spell_selecter();
    QPushButton * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
    void clear();
public slots:
    void buttonPushed();
signals:
    void wascleared();
    void wasselected();
};


class creature_type_flags_selecter
{
private:
    enum CreatureTypeFlags
    {
        CREATURE_TYPEFLAGS_TAMEABLE         = 0x00000001,       // Tameable by any hunter
        CREATURE_TYPEFLAGS_GHOST_VISIBLE    = 0x00000002,       // Creatures which can _also_ be seen when player is a ghost, used in CanInteract function by client, can't be attacked
        //CREATURE_TYPEFLAGS_UNK3             = 0x00000004,       // "BOSS" flag for tooltips
        //CREATURE_TYPEFLAGS_UNK4             = 0x00000008,
        //CREATURE_TYPEFLAGS_UNK5             = 0x00000010,       // controls something in client tooltip related to creature faction
        //CREATURE_TYPEFLAGS_UNK6             = 0x00000020,       // may be sound related
        //CREATURE_TYPEFLAGS_UNK7             = 0x00000040,       // may be related to attackable / not attackable creatures with spells, used together with lua_IsHelpfulSpell/lua_IsHarmfulSpell
        //CREATURE_TYPEFLAGS_UNK8             = 0x00000080,       // has something to do with unit interaction / quest status requests
        CREATURE_TYPEFLAGS_HERBLOOT         = 0x00000100,       // Can be looted by herbalist
        CREATURE_TYPEFLAGS_MININGLOOT       = 0x00000200,       // Can be looted by miner
        //CREATURE_TYPEFLAGS_UNK11            = 0x00000400,       // no idea, but it used by client
        //CREATURE_TYPEFLAGS_UNK12            = 0x00000800,       // related to possibility to cast spells while mounted
        CREATURE_TYPEFLAGS_CAN_ASSIST       = 0x00001000,       // Can aid any player (and group) in combat. Typically seen for escorting NPC's
        //CREATURE_TYPEFLAGS_UNK14            = 0x00002000,       // checked from calls in Lua_PetHasActionBar
        //CREATURE_TYPEFLAGS_UNK15            = 0x00004000,       // Lua_UnitGUID, client does guid_low &= 0xFF000000 if this flag is set
        CREATURE_TYPEFLAGS_ENGINEERLOOT     = 0x00008000,       // Can be looted by engineer
    };
    QCheckBox *box[6];
    QHBoxLayout * outer_layout;
    QVBoxLayout * inner_left_layout;
    QVBoxLayout * inner_right_layout;
    QWidget * widget;
public:
    creature_type_flags_selecter();
    ~creature_type_flags_selecter();
    QWidget * getWidget();
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};

class standing_animations_selecter
{
private:
    dbccache * m_dbccache;
    QComboBox * widget;
public:
    standing_animations_selecter(dbccache *c);
    ~standing_animations_selecter();
    QComboBox * getWidget();
    void reload(dbc_resource_type r);
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};

class pet_spell_dialog : public QObject
{
    Q_OBJECT
private:
    dbccache    *        m_dbccache;
    QVBoxLayout *        layout;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog *            widget;
public:
    pet_spell_dialog(dbccache * c);
    ~pet_spell_dialog();
    void reload(dbc_resource_type r);
    QDialog * getWidget();
    void setIndex(int index);

public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};
class pet_spell_selecter : public QObject
{
    Q_OBJECT
private:
    dbccache *           m_dbccache;
    int                  selected;
    pet_spell_dialog *   dialog;
    QPushButton *        widget;

    QString getDisplay(int index);

public:
    pet_spell_selecter(dbccache * c);
    ~pet_spell_selecter();
    QPushButton * getWidget();
    void setIndexFromId(int id);
    void reload(dbc_resource_type r);
    int selectedId();
public slots:
    void buttonPushed();
    void clear();
};

class item_model_dialog : public QObject
{
    Q_OBJECT
private:
    dbccache    *        m_dbccache;
    int                  slot;
    QVBoxLayout *        layout;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog *            widget;
public:
    item_model_dialog(dbccache *c, int slot);
    ~item_model_dialog();
    QDialog * getWidget();
    void setIndex(int index);
    void reload(dbc_resource_type r);
    void changeSlot(int slot);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class item_model_selecter : public QObject
{
    Q_OBJECT
private:
    dbccache *           m_dbccache;
    int                  selected;
    item_model_dialog *  dialog;
    QPushButton *        widget;
    int                  currentSlot;
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
public:
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
    item_model_selecter(dbccache * c, int slot);
    ~item_model_selecter();
    QPushButton * getWidget();
    void setIndexFromId(int id);
    void setIndexFromDisplayId(int id);
    void reload(dbc_resource_type r);
    int selectedId();
    int selectedDisplayId();
    void changeSlot(int slot);
    static int dbcSlotToSlot(int dbcslot)
    {
        switch(dbcslot)
        {
        case DBC_SLOT_NONE:     return SLOT_NONE;
        case DBC_SLOT_HEAD:     return SLOT_HEAD;
        case DBC_SLOT_NECK:     return SLOT_NECK;
        case DBC_SLOT_SHOULDERS:return SLOT_SHOULDERS;
        case DBC_SLOT_SHIRT:    return SLOT_SHIRT;
        case DBC_SLOT_VEST:     return SLOT_CHEST;
        case DBC_SLOT_WAIST:    return SLOT_WAIST;
        case DBC_SLOT_LEGS:     return SLOT_LEGS;
        case DBC_SLOT_FEET:     return SLOT_FEET;
        case DBC_SLOT_WRISTS:   return SLOT_WRISTS;
        case DBC_SLOT_HANDS:    return SLOT_HANDS;
        case DBC_SLOT_RING:     return SLOT_RING;
        case DBC_SLOT_TRINKET:  return SLOT_TRINKET;
        case DBC_SLOT_ONEHAND:  return SLOT_MAINHAND;
        case DBC_SLOT_SHIELD:   return SLOT_OFFHAND;
        case DBC_SLOT_BOW:      return SLOT_RANGED;
        case DBC_SLOT_BACK:     return SLOT_BACK;
        case DBC_SLOT_TWOHAND:  return SLOT_MAINHAND;
        case DBC_SLOT_BAG:      return SLOT_NONE;
        case DBC_SLOT_TABARD:   return SLOT_TABARD;
        case DBC_SLOT_ROBE:     return SLOT_CHEST;
        case DBC_SLOT_MAINHAND: return SLOT_MAINHAND;
        case DBC_SLOT_OFFHAND:  return SLOT_OFFHAND;
        case DBC_SLOT_HELD:     return SLOT_OFFHAND;
        case DBC_SLOT_AMMO:     return SLOT_NONE;
        case DBC_SLOT_THROWN:   return SLOT_RANGED;
        case DBC_SLOT_RANGED:   return SLOT_RANGED;
        case DBC_SLOT_RANGED_:  return SLOT_RANGED;
        case DBC_SLOT_RELIC:    return SLOT_RANGED;
        }
        return 0;
    }

public slots:
    void buttonPushed();
    void clear();
};

class item_class_selecter
{
private:
    int selected;
    QComboBox *         widget;
    dbccache *          m_dbccache;
public:
    item_class_selecter(dbccache * c);
    ~item_class_selecter();
    QComboBox * getWidget();
    void setIndexFromId(int id);
    void reload(dbc_resource_type r);
    int selectedId();
    bool isDefaultSelected();
    void clear();
};

class item_subclass_selecter
{
private:
    int selectedCategory;
    QComboBox * widget;
    dbccache * m_dbccache;
public:
    item_subclass_selecter(dbccache *c);
    ~item_subclass_selecter();
    QComboBox * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    void setCategory(int c);
    int selectedId();
    bool isDefaultSelected();
    void clear();
};

class item_quality_selecter
{
private:
    QComboBox * widget;
public:
    item_quality_selecter();
    ~item_quality_selecter();
    void setIndexFromId(int id);
    int selectedId();
    bool isDefaultSelected();
    QComboBox * getWidget();
    void clear();
};

class item_slot_selecter
{
private:
    QComboBox * widget;
public:
    item_slot_selecter();
    ~item_slot_selecter();
    void setIndexFromId(int id);
    int selectedId();
    bool isDefaultSelected();
    QComboBox * getWidget();
    void clear();
};

class item_set_selecter
{
private:
    QComboBox * widget;
    dbccache * m_dbccache;
public:
    item_set_selecter(dbccache * c);
    ~item_set_selecter();
    QComboBox * getWidget();
    void setIndexFromId(int id);
    void reload(dbc_resource_type r);
    void clear();
    int selectedId();
};

class class_mask_selecter
{
    // Hardcode this widget
    //Copied from mangos
    enum Classes
    {
        CLASS_WARRIOR       = 1,
        CLASS_PALADIN       = 2,
        CLASS_HUNTER        = 3,
        CLASS_ROGUE         = 4,
        CLASS_PRIEST        = 5,
        // CLASS_DEATH_KNIGHT  = 6,                             // not listed in DBC, will be in 3.0
        CLASS_SHAMAN        = 7,
        CLASS_MAGE          = 8,
        CLASS_WARLOCK       = 9,
        // CLASS_UNK2       = 10,unused
        CLASS_DRUID         = 11,
    };
private:
    QWidget * widget;
    QCheckBox * box[9];
public:
    class_mask_selecter();
    ~class_mask_selecter();
    QWidget * getWidget();
    int selectedId();
    void setIndexFromId(int);
    void clear();
};

class race_mask_selecter
{
private:
    //Copied from mangos, only using playable races at the moment
    enum Races
    {
        RACE_HUMAN              = 1,
        RACE_ORC                = 2,
        RACE_DWARF              = 3,
        RACE_NIGHTELF           = 4,
        RACE_UNDEAD             = 5,
        RACE_TAUREN             = 6,
        RACE_GNOME              = 7,
        RACE_TROLL              = 8,
        RACE_GOBLIN             = 9,
        RACE_BLOODELF           = 10,
        RACE_DRAENEI            = 11,
        RACE_FEL_ORC            = 12,
        RACE_NAGA               = 13,
        RACE_BROKEN             = 14,
        RACE_SKELETON           = 15,
        RACE_VRYKUL             = 16,
        RACE_TUSKARR            = 17,
        RACE_FOREST_TROLL       = 18,
    };
    QWidget * widget;
    QCheckBox * box[10];
public:
    race_mask_selecter();
    ~race_mask_selecter();
    QWidget * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class title_selecter
{
private:
    dbccache * m_dbccache;
    QComboBox * widget;
public:
    title_selecter(dbccache * c);
    ~title_selecter();
    QComboBox * getWidget();
    void reload(dbc_resource_type r);
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};

class honor_rank_selecter
{
private:
    QComboBox * widget;
public:
    honor_rank_selecter();
    ~honor_rank_selecter();
    QComboBox * getWidget();
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};

class item_stat_type_selecter
{
private:
    // Copied from mangos
    enum ItemModType
    {
        ITEM_MOD_MANA                     = 0,
        ITEM_MOD_HEALTH                   = 1,
        ITEM_MOD_AGILITY                  = 3,
        ITEM_MOD_STRENGTH                 = 4,
        ITEM_MOD_INTELLECT                = 5,
        ITEM_MOD_SPIRIT                   = 6,
        ITEM_MOD_STAMINA                  = 7,
        ITEM_MOD_DEFENSE_SKILL_RATING     = 12,
        ITEM_MOD_DODGE_RATING             = 13,
        ITEM_MOD_PARRY_RATING             = 14,
        ITEM_MOD_BLOCK_RATING             = 15,
        ITEM_MOD_HIT_MELEE_RATING         = 16,
        ITEM_MOD_HIT_RANGED_RATING        = 17,
        ITEM_MOD_HIT_SPELL_RATING         = 18,
        ITEM_MOD_CRIT_MELEE_RATING        = 19,
        ITEM_MOD_CRIT_RANGED_RATING       = 20,
        ITEM_MOD_CRIT_SPELL_RATING        = 21,
        ITEM_MOD_HIT_TAKEN_MELEE_RATING   = 22,
        ITEM_MOD_HIT_TAKEN_RANGED_RATING  = 23,
        ITEM_MOD_HIT_TAKEN_SPELL_RATING   = 24,
        ITEM_MOD_CRIT_TAKEN_MELEE_RATING  = 25,
        ITEM_MOD_CRIT_TAKEN_RANGED_RATING = 26,
        ITEM_MOD_CRIT_TAKEN_SPELL_RATING  = 27,
        ITEM_MOD_HASTE_MELEE_RATING       = 28,
        ITEM_MOD_HASTE_RANGED_RATING      = 29,
        ITEM_MOD_HASTE_SPELL_RATING       = 30,
        ITEM_MOD_HIT_RATING               = 31,
        ITEM_MOD_CRIT_RATING              = 32,
        ITEM_MOD_HIT_TAKEN_RATING         = 33,
        ITEM_MOD_CRIT_TAKEN_RATING        = 34,
        ITEM_MOD_RESILIENCE_RATING        = 35,
        ITEM_MOD_HASTE_RATING             = 36,
        ITEM_MOD_EXPERTISE_RATING         = 37
    };
    QComboBox * widget;
public:
    item_stat_type_selecter();
    ~item_stat_type_selecter();
    QComboBox * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class item_sheathe_selecter
{
private:
    QComboBox * widget;
public:
    item_sheathe_selecter();
    ~item_sheathe_selecter();
    QComboBox * getWidget();
    void clear();
    int selectedId();
    void setIndexFromId(int id);
};

class socket_color_selecter
{
private:
    QComboBox * widget;
public:
    socket_color_selecter();
    ~socket_color_selecter();
    QComboBox * getWidget();
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};

class item_enchant_dialog : public QObject
{
    Q_OBJECT
private:
    dbccache    *                   m_dbccache;
    QDialog *                       widget;
    QVBoxLayout *                   layout;
    QHBoxLayout *                   blayout;
    QListWidget *                   list;
    QPushButton *                   cancelB;
    QPushButton *                   acceptB;
    QPushButton *                   clearB;
public:
    item_enchant_dialog(dbccache * c);
    ~item_enchant_dialog();
    QDialog * getWidget();
    void reload(dbc_resource_type r);
    void setIndex(int index);
    int selectedId();
public slots:
    void accepted();
    void cleared();
    void rejected();
signals:
    void clear();
};

class item_enchant_selecter : public QObject
{
    Q_OBJECT
private:
    dbccache *                      m_dbccache;
    QPushButton *                   widget;
    item_enchant_dialog *           dialog;
    int                             selected;
public:
    item_enchant_selecter(dbccache * c);
    ~item_enchant_selecter();
    QPushButton * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
public slots:
    void onButtonPushed();
    void clear();
};

class damage_school_selecter
{
private:
    QComboBox * widget;
public:
    damage_school_selecter();
    ~damage_school_selecter();
    QComboBox * getWidget();
    QString  selectedString();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class ammo_type_selecter
{
private:
    QComboBox * widget;
public:
    ammo_type_selecter();
    ~ammo_type_selecter();
    QComboBox * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class spell_trigger_selecter
{
private:
    QComboBox * widget;
public:
    spell_trigger_selecter();
    ~spell_trigger_selecter();
    QComboBox * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class language_selecter
{
private:
    dbccache * m_dbccache;
    QComboBox * widget;
public:
    language_selecter(dbccache * c);
    ~language_selecter();
    void reload(dbc_resource_type r);
    QComboBox * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class page_material_selecter
{
private:
    QComboBox * widget;
public:
    page_material_selecter();
    ~page_material_selecter();
    QComboBox * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class material_selecter
{
private:
    QComboBox * widget;
public:
    material_selecter();
    ~material_selecter();
    QComboBox * getWidget();
    int selectedId();
    void setIndexFromId(int id);
    void clear();
};

class bag_family_mask_selecter
{
private:
    QWidget * widget;
    QHBoxLayout * layout;
    QVBoxLayout * tlayout;
    QVBoxLayout * mlayout;
    QVBoxLayout * blayout;
    QCheckBox * box[10];
public:
    bag_family_mask_selecter();
    ~bag_family_mask_selecter();
    QWidget * getWidget();
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};

class totem_category_selecter
{
private:
    dbccache * m_dbccache;
    QComboBox * widget;
public:
    totem_category_selecter(dbccache * c);
    ~totem_category_selecter();
    void reload(dbc_resource_type r);
    QComboBox * getWidget();
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};

class gem_dialog : public QObject
{
    Q_OBJECT
private:
    dbccache    *                       m_dbccache;
    QDialog *                           widget;
    QVBoxLayout *                       layout;
    QHBoxLayout *                       blayout;
    QPushButton *                       cancelB;
    QPushButton *                       acceptB;
    QPushButton *                       clearB;
    QListWidget *                       list;
public:
    gem_dialog(dbccache * c);
    ~gem_dialog();
    QDialog * getWidget();
    void reload(dbc_resource_type r);
    void setIndex(int id);
public slots:
    void accepted();
    void rejected();
    void cleared();
signals:
    void clear();
};

class gem_selecter : public QObject
{
    Q_OBJECT
private:
    dbccache *                          m_dbccache;
    QPushButton *                       widget;
    gem_dialog *                        dialog;
    int                                 selected;
public:
    gem_selecter(dbccache * c);
    ~gem_selecter();
    QPushButton * getWidget();
    void reload(dbc_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
public slots:
    void buttonPushed();
    void clear();
};

class food_type_selecter
{
private:
    QComboBox * widget;
public:
    food_type_selecter();
    ~food_type_selecter();
    QComboBox * getWidget();
    void setIndexFromId(int id);
    int selectedId();
    void clear();
};

class item_flags_selecter
{
private:
    enum ITEM_FLAGS
    {
        IF_SOULBOUND        = 1,
        IF_CONJURED         = 2,
        IF_LOOTABLE         = 4,
        IF_WRAPPED          = 8,
        IF_TOTEM            = 32,
        IF_ACTIVATABLE      = 64,
        IF_WRAPPER          = 256,
        IF_GIFTS            = 1024,
        IF_ALLCANLOOT       = 2048,
        IF_CHARTER          = 8192,
        IF_PVPREWARD        = 32768,
        IF_UNIQUEEQUIPPED   = 524288,
        IF_THROWABLE        = 4194304,
        IF_SPECIALUSE       = 8388608
    };

    QWidget * widget;
    QVBoxLayout * layout;
    QHBoxLayout * tlayout;
    QHBoxLayout * mlayout;
    QHBoxLayout * blayout;
    QCheckBox * box[14];
public:
    item_flags_selecter();
    ~item_flags_selecter();
    QWidget * getWidget() { return widget; }
    void clear();
    void setIndexFromId(int id);
    int selectedId();
};

#endif // DBCWIDGETS_H
