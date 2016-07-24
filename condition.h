#ifndef CONDITION_H
#define CONDITION_H

#include <map>
#include <tuple>
#include <QString>
#include "dbccache.h"
#include <QStringList>


enum class condition_type
{
    NOT                 = -3,
    OR                  = -2,
    AND                 = -1,
    NONE                = 0,
    aura                = 1,
    item                = 2,
    item_equipped       = 3,
    areaid              = 4,
    reputation_rank_min = 5,
    team                = 6,
    skill               = 7,
    quest_rewarded      = 8,
    quest_taken         = 9,
    AD_commission_aura  = 10,
    no_aura             = 11,
    active_event        = 12,
    area_flag           = 13,
    race_and_class      = 14,
    level               = 15,
    no_item             = 16,
    spell               = 17,
    instance_script     = 18,
    quest_available     = 19,
    //achievement         = 20,
    //achievement_realm   = 21,
    quest_none          = 22,
    item_with_bank      = 23,
    no_item_with_bank   = 24,
    not_active_event    = 25,
    active_holiday      = 26,
    not_active_holiday  = 27,
    learnable_ability   = 28,
    skill_below         = 29,
    reputation_rank_max = 30,
    //completed_encounter = 31,
    source_aura         = 32,
    last_waypoint       = 33,
    //xp_user             = 34,
    gender              = 35,
    dead_or_away        = 36
};

struct condition_data
{
    condition_type  type;
    int             value1;
    int             value2;
};

namespace condition
{

namespace detail
{
    QString getFactionRank(int rank)
    {
        switch(rank)
        {
        case 0:
            return QString("hated");
        case 1:
            return QString("hostile");
        case 2:
            return QString("unfriendly");
        case 3:
            return QString("neutral");
        case 4:
            return QString("friendly");
        case 5:
            return QString("honored");
        case 6:
            return QString("revered");
        case 7:
            return QString("exalted");
        }
    }
    QString getClassMaskClasses(int flags)
    {
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
        QString s = "";
        bool atLeastOne = false;
        if(flags & (1 << (CLASS_WARRIOR-1))){ if(atLeastOne){s+=", ";} s+="Warrior";atLeastOne=true; }
        if(flags & (1 << (CLASS_PALADIN-1))){ if(atLeastOne){s+=", ";} s+="Paladin";atLeastOne=true; }
        if(flags & (1 << (CLASS_HUNTER-1))){ if(atLeastOne){s+=", ";} s+="Hunter";atLeastOne=true; }
        if(flags & (1 << (CLASS_ROGUE-1))){ if(atLeastOne){s+=", ";} s+="Rogue";atLeastOne=true; }
        if(flags & (1 << (CLASS_PRIEST-1))){ if(atLeastOne){s+=", ";} s+="Priest";atLeastOne=true; }
        if(flags & (1 << (CLASS_SHAMAN-1))){ if(atLeastOne){s+=", ";} s+="Shaman";atLeastOne=true; }
        if(flags & (1 << (CLASS_MAGE-1))){ if(atLeastOne){s+=", ";} s+="Mage";atLeastOne=true; }
        if(flags & (1 << (CLASS_WARLOCK-1))){ if(atLeastOne){s+=", ";} s+="Warlock";atLeastOne=true; }
        if(flags & (1 << (CLASS_DRUID-1))){ if(atLeastOne){s+=", ";} s+="Druid";atLeastOne=true; }
        return s;
    }
    QString getRaceMaskRaces(int flags)
    {
        QString s = "";
        bool atLeastOne = false;
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
            RACE_DRAENEI            = 11
        };
        if(flags & (1 << (RACE_HUMAN-1))){ if(atLeastOne){s+=", ";} s+="Human";atLeastOne=true; }
        if(flags & (1 << (RACE_ORC-1))){ if(atLeastOne){s+=", ";} s+="Orc";atLeastOne=true; }
        if(flags & (1 << (RACE_DWARF-1))){ if(atLeastOne){s+=", ";} s+="Dwarf";atLeastOne=true; }
        if(flags & (1 << (RACE_NIGHTELF-1))){ if(atLeastOne){s+=", ";} s+="Night Elf";atLeastOne=true; }
        if(flags & (1 << (RACE_UNDEAD-1))){ if(atLeastOne){s+=", ";} s+="Undead";atLeastOne=true; }
        if(flags & (1 << (RACE_TAUREN-1))){ if(atLeastOne){s+=", ";} s+="Tauren";atLeastOne=true; }
        if(flags & (1 << (RACE_GNOME-1))){ if(atLeastOne){s+=", ";} s+="Gnome";atLeastOne=true; }
        if(flags & (1 << (RACE_TROLL-1))){ if(atLeastOne){s+=", ";} s+="Troll";atLeastOne=true; }
        if(flags & (1 << (RACE_BLOODELF-1))){ if(atLeastOne){s+=", ";} s+="Blood Elf";atLeastOne=true; }
        if(flags & (1 << (RACE_DRAENEI-1))){ if(atLeastOne){s+=", ";} s+="Dreanei";atLeastOne=true; }
        return s;
    }
    QString removeTabPrefix(QString s)
    {
        return s.remove(0,s.indexOf('\t')+1);
    }
}
    QString label(int entry,std::map<int,std::tuple<int,int,int>>& cs, dbccache & ch)
    {
        QString s = "";
        std::tuple<int,int,int> c = cs[entry];
        condition_type type =   condition_type(std::get<0>(c));
        int value1 =            std::get<1>(c);
        int value2 =            std::get<2>(c);

        switch(type)
        {
        case condition_type::NOT:
            s = QString("!(") + label(value1,cs,ch) + QString(")");
            break;
        case condition_type::OR:
            s = QString("(") + label(value1,cs,ch) + QString(" OR ") + label(value2,cs,ch) + QString(")");
            break;
        case condition_type::AND:
            s = QString("(") + label(value1,cs,ch) + QString(" AND ") + label(value2,cs,ch) + ")";
            break;
        case condition_type::NONE:
            s = QString("True");
            break;
        case condition_type::aura:
            s = QString("Player has aura (Spell ID:") + QString::number(value1) + QString(", Effect Index:") + QString::number(value2) + ")";
            break;
        case condition_type::item:
            s = QString("Player has at least ") + QString::number(value2) + QString(" of item entry ") + QString::number(value1);
            break;
        case condition_type::item_equipped:
            s = QString("Player has item entry ") + QString::number(value1) + QString(" equipped");
            break;
        case condition_type::areaid:
        {
            int i = dbccache::find_ref(ch.get_area_references(),value1);
            s = QString("Player ") + (value2 ? QString("is not") : QString("is")) + QString(" in area ") +
                    (i!=-1?ch.get_area_resource()->at(i):QString("{Error: Area not found in DBC}"));
            break;
        }
        case condition_type::reputation_rank_min:
        {
            int i = dbccache::find_ref(ch.get_reputation_faction_references(),value1);
            s = QString("Player is at least ") + detail::getFactionRank(value2) + QString(" with faction ") +
                    (i!=-1?ch.get_reputation_faction_resource()->at(i+1):QString("{Error: Faction not found in DBC}"));
            break;
        }
        case condition_type::team:
            s = QString("Player belongs to the ") + (value1 == 469 ? QString("Alliance") : QString("Horde"));
            break;
        case condition_type::skill:
        {
            int i = dbccache::find_ref(ch.get_skillline_references(),value1);
            s = QString("Player has at least ") + QString::number(value2) + QString(" in ") +
                    (i!=-1?ch.get_skillline_resource()->at(i+1):QString("{Error: Skill-line not found in DBC}"));
            break;
        }
        case condition_type::quest_rewarded:
            s = QString("Player has completed quest ") + QString::number(value1);
            break;
        case condition_type::quest_taken:
            s = QString("Player has taken quest ") + QString::number(value1);
            break;
        case condition_type::AD_commission_aura:
            s = QString("Player has Argent Dawn commision aura");
            break;
        case condition_type::no_aura:
            s = QString("Player does not have aura (Spell ID:") + QString::number(value1) + QString(", Effect Index:") + QString::number(value2) + ")";
            break;
        case condition_type::active_event:
            s = QString("Event ") + QString::number(value1) + QString(" is active");
            break;
        case condition_type::area_flag:
            s = QString("Current area ") + (value2 ? QString("does not have"): QString("has")) + QString(" flag ") + QString::number(value1);
            break;
        case condition_type::race_and_class:
            if(value1 && value2)
            {
                s = QString("Player is a class among \"") + detail::getClassMaskClasses(value2) + QString("\" and a race among \"")
                        + detail::getRaceMaskRaces(value1) +QString("\"");
            }
            else if(value1 && !value2)
            {
                s = QString("Player is a race among \"") + detail::getRaceMaskRaces(value1) + QString("\"");
            }
            else if(!value1 && value2)
            {
                s = QString("Player is a class among \"") + detail::getClassMaskClasses(value2) + QString("\"");
            }
            else
            {
                s = QString("Player is of any class and race");
            }
            break;
        case condition_type::level:
            s = QString("Player level is");
            if(value2==1)       { s += QString(" higher than or"); }
            else if(value2==2)  { s += QString(" lower than or"); }
            s += QString(" equal to ") + QString::number(value1);
            break;
        case condition_type::no_item:
            s = QString("Player does not have at least ") + QString::number(value2) + QString(" of item entry ") + QString::number(value1);
            break;
        case condition_type::spell:
        {
            int i = dbccache::find_ref(ch.get_spell_references(),value1);
            s = QString("Player ") + (!value2?QString("has"):QString("has not")) + QString(" learned spell ") +
                    (i!=-1?detail::removeTabPrefix(ch.get_spell_resource()->at(i)):QString("{Error: Spell not found in DBC}")) + QString(" (") + QString::number(value1) + QString(")");
            break;
        }
        case condition_type::instance_script:
            s = QString("Result of current instance's script'");
            break;
        case condition_type::quest_available:
            s = QString("Player can pick up quest ") + QString::number(value1);
            break;
        case condition_type::quest_none:
            s = QString("Player has never seen quest ") + QString::number(value1);
            break;
        case condition_type::item_with_bank:
            s = QString("Player has, including from bank, at least ") + QString::number(value2) + QString(" of item entry ") + QString::number(value1);
            break;
        case condition_type::no_item_with_bank:
            s = QString("Player does not have, including from bank, at least ") + QString::number(value2) + QString(" of item entry ") + QString::number(value1);
            break;
        case condition_type::not_active_event:
            s = QString("Event ") + QString::number(value1) + QString(" is not active");
            break;
        case condition_type::active_holiday:
            s = QString("Holiday ") + QString::number(value1) + QString(" is active");
            break;
        case condition_type::not_active_holiday:
            s = QString("Holiday ") + QString::number(value1) + QString(" is not active");
            break;
        case condition_type::learnable_ability:
        {
            int i = dbccache::find_ref(ch.get_skillline_references(),value1);
            s = QString("Player can learn ability ") + (i!=-1?detail::removeTabPrefix(ch.get_spell_resource()->at(i)):QString("{Error: Spell not found in DBC}"))
                    + (value2!=0?QString(" and player has item with entry ") + QString::number(value2) + QString(" in inventory or bank"):QString(""));
            break;
        }
        case condition_type::skill_below:
        {
            int i = dbccache::find_ref(ch.get_skillline_references(),value1);
            s = (value2==1?QString("Player does not have skill "):QString("Player has skill below ") + QString::number(value2) + QString(" in ")) +
                    (i!=-1?ch.get_skillline_resource()->at(i+1):QString("{Error: Skill-line not found in DBC}"));
            break;
        }
        case condition_type::reputation_rank_max:
        {
            int i = dbccache::find_ref(ch.get_reputation_faction_references(),value1);
            s = QString("Player does not have higher standing than ") + detail::getFactionRank(value2) + QString(" with faction ") +
                    (i!=-1?ch.get_reputation_faction_resource()->at(i+1):QString("{Error: Faction not found in DBC}"));
            break;
        }
        case condition_type::source_aura:
            s = QString("Source has aura (Spell ID:") + QString::number(value1) + QString(", Effect Index:") + QString::number(value2) + ")";
            break;
        case condition_type::last_waypoint:
            if(value2==0)
            {
                s = QString("Source is on waypoint of id ") + QString::number(value1);
            }
            else if(value2==1)
            {
                s = QString("Source has not passed waypoint of id ") + QString::number(value1);
            }
            else if(value2==2)
            {
                s = QString("Source has passed the waypoint of id ") + QString::number(value1);
            }
            break;
        case condition_type::gender:
            s = QString("Player is ") + (value1==2?QString("not gendered"):(value1==0?QString("male"):QString("female")));
            break;
        case condition_type::dead_or_away:
            if(value1==0)
            {
                s = QString("Player is dead") +
                        (value2?QString(" or out of range (") + QString::number(value2) + QString(") of source"):QString(""));
            }
            else if(value1==1)
            {
                s = QString("Everyone in the player's group are dead") +
                        (value2?QString(" or out of range (") + QString::number(value2) + QString(") of source"):QString(""));
            }
            else if(value1==2)
            {
                s = QString("All players in the instance are dead") +
                        (value2?QString(" or out of range (") + QString::number(value2) + QString(") of source"):QString(""));
            }
            else if(value1==3)
            {
                s = QString("Source is dead");
            }
           break;
        default:
            break;
        }

        return s;
    }
}


#endif // CONDITION_H
