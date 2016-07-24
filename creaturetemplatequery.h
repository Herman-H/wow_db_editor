#ifndef CREATURETEMPLATEQUERY_H
#define CREATURETEMPLATEQUERY_H

#include <QSql>
#include <QString>
#include <QSqlQuery>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QApplication>

class RankWidgetDelegate : public QStyledItemDelegate
{
public:
    static QString displayRank(int rank)
    {
        switch(rank)
        {
        case 0: return "Normal";
        case 1: return "Elite";
        case 2: return "Rare-Elite";
        case 3: return "Boss";
        case 4: return "Rare";
        default: return "Unspecified";
        }
        return "Unspecified";
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.column() == 3)
        {
            int rank = index.data().toInt();
            QStyleOptionViewItem opt = option;
            opt.text = displayRank(rank);
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem,&opt,painter);
        }
        else
        {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

class TypeWidgetDelegate : public QStyledItemDelegate
{
public:
    static QString displayType(int type)
    {
        switch(type)
        {
        case 0: return "Unspecified";
        case 1: return "Beast";
        case 2: return "Dragon";
        case 3: return "Demon";
        case 4: return "Elemental";
        case 5: return "Giant";
        case 6: return "Undead";
        case 7: return "Humanoid";
        case 8: return "Critter";
        case 9: return "Mechanical";
        case 10: return "Uncategorized";
        case 11: return "Totem";
        case 12: return "Companion-Pet";
        default:
            return "Unspecified";
        }
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.column() == 4)
        {
            int type = index.data().toInt();
            QStyleOptionViewItem opt = option;
            opt.text = displayType(type);
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem,&opt,painter);
        }
        else
        {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

class CreatureTemplateQuery
{
public:
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

    enum class Type
    {
        Beast = 1,
        Dragon = 2,
        Demon = 3,
        Elemental = 4,
        Giant = 5,
        Undead = 6,
        Humanoid = 7,
        Critter = 8,
        Mechanical = 9,
        Uncategorized = 10,
        Totem = 11,
        CompanionPet = 12,
        Unspecified = 13
    };

    static int indexToType(int index)
    {
        switch(index)
        {
        case 0:
            return 13;
        default:
            return index;
        }
    }


private:
    QString name;bool bname;
    int minlevel,maxlevel;bool bminlevel,bmaxlevel;
    int minhp,maxhp;bool bminhp,bmaxhp;
    int minmana,maxmana;bool bminmana,bmaxmana;
    int minarmor,maxarmor;bool bminarmor,bmaxarmor;
    int minatp,maxatp;bool bminatp,bmaxatp;
    int minatt,maxatt;bool bminatt,bmaxatt;
    int minatm,maxatm;bool bminatm,bmaxatm;
    int minrap,maxrap;bool bminrap,bmaxrap;
    int minratt,maxratt;bool bminratt,bmaxratt;
    int minratm,maxratm;bool bminratm,bmaxratm;
    int rank,type;bool brank,btype;

    QString queryString;
public:
    CreatureTemplateQuery(){}

    void setName(QString s){ name = s; bname = (s.length() != 0); }
    void setMinLvl(QString s){ minlevel = s.toInt(); bminlevel = (s.length() != 0); }
    void setMaxLvl(QString s){ maxlevel = s.toInt(); bmaxlevel = (s.length() != 0); }
    void setMinHP(QString s){ minhp = s.toInt(); bminhp = (s.length() != 0); }
    void setMaxHP(QString s){ maxhp = s.toInt(); bmaxhp = (s.length() != 0); }
    void setMinMana(QString s){ minmana = s.toInt(); bminmana = (s.length() != 0); }
    void setMaxMana(QString s){ maxmana = s.toInt(); bmaxmana = (s.length() != 0); }
    void setMinArmor(QString s){ minarmor = s.toInt(); bminarmor = (s.length() != 0); }
    void setMaxArmor(QString s){ maxarmor = s.toInt(); bmaxarmor = (s.length() != 0); }
    void setMinAttackPower(QString s){ minatp = s.toInt(); bminatp = (s.length() != 0); }
    void setMaxAttackPower(QString s){ maxatp = s.toInt(); bmaxatp = (s.length() != 0); }
    void setMinAttackTime(QString s){ minatt = s.toInt(); bminatt = (s.length() != 0); }
    void setMaxAttackTime(QString s){ maxatt = s.toInt(); bmaxatt = (s.length() != 0); }
    void setMinAttackDamage(QString s){ minatm = s.toInt(); bminatm = (s.length() != 0); }
    void setMaxAttackDamage(QString s){ maxatm = s.toInt(); bmaxatm = (s.length() != 0); }
    void setMinRangedAttackPower(QString s){ minrap = s.toInt(); bminrap = (s.length() != 0); }
    void setMaxRangedAttackPower(QString s){ maxrap = s.toInt(); bmaxrap = (s.length() != 0); }
    void setMinRangedAttackTime(QString s){ minratt = s.toInt(); bminratt = (s.length() != 0); }
    void setMaxRangedAttackTime(QString s){ maxratt = s.toInt(); bmaxratt = (s.length() != 0); }
    void setMinRangedAttackDamage(QString s){ minratm = s.toInt(); bminratm = (s.length() != 0); }
    void setMaxRangedAttackDamage(QString s){ maxratm = s.toInt(); bmaxratm = (s.length() != 0); }
    void setRank(int i){ brank = (i != 0); rank = indexToRank(i); }
    void setType(int i){ btype = (i != 0); type = indexToType(i); }

    void clear(){name="";bname=false; bminlevel = false;bmaxlevel=false;bminhp=false;
                bmaxhp=false;bmaxmana=false;bminmana=false;bminarmor=false;bmaxarmor=false;
                bminatp=false;bmaxatp=false;bminatt=false;bmaxatt=false;bminatm=false;
                bmaxatm=false;bminrap=false;bmaxrap=false;bminratt=false;bmaxratt=false;
                bminratm=false;bmaxratm=false;brank=false;btype=false; }

    QString make_query()
    {
        queryString = "SELECT name,minlevel,maxlevel,rank,type,entry FROM creature_template";
        QString querySpecifiers;
        bool hasSpecifiers = false;
        if(bname)
        {
            querySpecifiers += "name LIKE \"%" + name + "%\"";
            hasSpecifiers = true;
        }
        if(bminlevel)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "minlevel >= " + QString::number(minlevel);
            hasSpecifiers = true;
        }
        if(bmaxlevel)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "maxlevel <= " + QString::number(maxlevel);
            hasSpecifiers = true;
        }
        if(bminhp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "minhealth >= " + QString::number(minhp);
            hasSpecifiers = true;
        }
        if(bmaxhp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers = "maxhealth <= " + QString::number(maxhp);
            hasSpecifiers = true;
        }
        if(bminmana)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "minmana >= " + QString::number(minmana);
            hasSpecifiers = true;
        }
        if(bmaxmana)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "maxmana <= " + QString::number(maxmana);
            hasSpecifiers = true;
        }
        if(bminatm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "mindmg >= " + QString::number(minatm);
            hasSpecifiers = true;
        }
        if(bmaxatm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "maxdmg <= " + QString::number(bmaxatm);
            hasSpecifiers = true;
        }
        if(bminatp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "attackpower >= " + QString::number(minatp);
            hasSpecifiers = true;
        }
        if(bmaxatp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "attackpower <= " + QString::number(maxatp);
            hasSpecifiers = true;
        }
        if(bminatt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "baseattacktime >= " + QString::number(minatt);
            hasSpecifiers = true;
        }
        if(bmaxatt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "baseattacktime <= " + QString::number(maxatt);
            hasSpecifiers = true;
        }
        if(bminrap)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "rangedattackpower >= " + QString::number(minrap);
            hasSpecifiers = true;
        }
        if(bmaxrap)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "rangedattackpower <= " + QString::number(maxrap);
            hasSpecifiers = true;
        }
        if(bminratm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "minrangedmg >= " + QString::number(minratm);
            hasSpecifiers = true;
        }
        if(bmaxratm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "maxrangedmg <= " + QString::number(maxratm);
            hasSpecifiers = true;
        }
        if(bminratt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "rangeattacktime >= " + QString::number(minratt);
            hasSpecifiers = true;
        }
        if(bmaxratt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "rangeattacktime <= " + QString::number(maxratt);
            hasSpecifiers = true;
        }
        if(brank)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "rank = " + QString::number(rank);
            hasSpecifiers = true;
        }
        if(btype)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += "type = " + QString::number(type);
            hasSpecifiers = true;
        }
        if(hasSpecifiers)
        {
            queryString += " WHERE ";
            queryString += querySpecifiers;
        }
        //printf(queryString.toLocal8Bit());
        return queryString;
    }

    QString make_query(QString alias)
    {
        queryString = "SELECT name,minlevel,maxlevel,rank,type,entry FROM creature_template";
        QString querySpecifiers;
        bool hasSpecifiers = false;
        if(bname)
        {
            querySpecifiers += alias + ".name LIKE \"%" + name + "%\"";
            hasSpecifiers = true;
        }
        if(bminlevel)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minlevel >= " + QString::number(minlevel);
            hasSpecifiers = true;
        }
        if(bmaxlevel)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxlevel <= " + QString::number(maxlevel);
            hasSpecifiers = true;
        }
        if(bminhp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minhealth >= " + QString::number(minhp);
            hasSpecifiers = true;
        }
        if(bmaxhp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers = alias + ".maxhealth <= " + QString::number(maxhp);
            hasSpecifiers = true;
        }
        if(bminmana)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minmana >= " + QString::number(minmana);
            hasSpecifiers = true;
        }
        if(bmaxmana)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxmana <= " + QString::number(maxmana);
            hasSpecifiers = true;
        }
        if(bminatm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".mindmg >= " + QString::number(minatm);
            hasSpecifiers = true;
        }
        if(bmaxatm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxdmg <= " + QString::number(bmaxatm);
            hasSpecifiers = true;
        }
        if(bminatp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".attackpower >= " + QString::number(minatp);
            hasSpecifiers = true;
        }
        if(bmaxatp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".attackpower <= " + QString::number(maxatp);
            hasSpecifiers = true;
        }
        if(bminatt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".baseattacktime >= " + QString::number(minatt);
            hasSpecifiers = true;
        }
        if(bmaxatt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".baseattacktime <= " + QString::number(maxatt);
            hasSpecifiers = true;
        }
        if(bminrap)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangedattackpower >= " + QString::number(minrap);
            hasSpecifiers = true;
        }
        if(bmaxrap)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangedattackpower <= " + QString::number(maxrap);
            hasSpecifiers = true;
        }
        if(bminratm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minrangedmg >= " + QString::number(minratm);
            hasSpecifiers = true;
        }
        if(bmaxratm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxrangedmg <= " + QString::number(maxratm);
            hasSpecifiers = true;
        }
        if(bminratt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangeattacktime >= " + QString::number(minratt);
            hasSpecifiers = true;
        }
        if(bmaxratt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangeattacktime <= " + QString::number(maxratt);
            hasSpecifiers = true;
        }
        if(brank)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rank = " + QString::number(rank);
            hasSpecifiers = true;
        }
        if(btype)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".type = " + QString::number(type);
            hasSpecifiers = true;
        }
        if(hasSpecifiers)
        {
            queryString += " WHERE ";
            queryString += querySpecifiers;
        }
        //printf(queryString.toLocal8Bit());
        return queryString;
    }

    QString make_query_specifiers(QString alias, bool hasSp)
    {
        QString querySpecifiers;
        bool hasSpecifiers = hasSp;
        if(bname)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".name LIKE \"%" + name + "%\"";
            hasSpecifiers = true;
        }
        if(bminlevel)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minlevel >= " + QString::number(minlevel);
            hasSpecifiers = true;
        }
        if(bmaxlevel)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxlevel <= " + QString::number(maxlevel);
            hasSpecifiers = true;
        }
        if(bminhp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minhealth >= " + QString::number(minhp);
            hasSpecifiers = true;
        }
        if(bmaxhp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers = alias + ".maxhealth <= " + QString::number(maxhp);
            hasSpecifiers = true;
        }
        if(bminmana)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minmana >= " + QString::number(minmana);
            hasSpecifiers = true;
        }
        if(bmaxmana)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxmana <= " + QString::number(maxmana);
            hasSpecifiers = true;
        }
        if(bminatm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".mindmg >= " + QString::number(minatm);
            hasSpecifiers = true;
        }
        if(bmaxatm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxdmg <= " + QString::number(bmaxatm);
            hasSpecifiers = true;
        }
        if(bminatp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".attackpower >= " + QString::number(minatp);
            hasSpecifiers = true;
        }
        if(bmaxatp)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".attackpower <= " + QString::number(maxatp);
            hasSpecifiers = true;
        }
        if(bminatt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".baseattacktime >= " + QString::number(minatt);
            hasSpecifiers = true;
        }
        if(bmaxatt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".baseattacktime <= " + QString::number(maxatt);
            hasSpecifiers = true;
        }
        if(bminrap)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangedattackpower >= " + QString::number(minrap);
            hasSpecifiers = true;
        }
        if(bmaxrap)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangedattackpower <= " + QString::number(maxrap);
            hasSpecifiers = true;
        }
        if(bminratm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".minrangedmg >= " + QString::number(minratm);
            hasSpecifiers = true;
        }
        if(bmaxratm)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".maxrangedmg <= " + QString::number(maxratm);
            hasSpecifiers = true;
        }
        if(bminratt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangeattacktime >= " + QString::number(minratt);
            hasSpecifiers = true;
        }
        if(bmaxratt)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rangeattacktime <= " + QString::number(maxratt);
            hasSpecifiers = true;
        }
        if(brank)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".rank = " + QString::number(rank);
            hasSpecifiers = true;
        }
        if(btype)
        {
            if(hasSpecifiers)
                querySpecifiers += " AND ";
            querySpecifiers += alias + ".type = " + QString::number(type);
            hasSpecifiers = true;
        }
        return querySpecifiers;
    }

    QString getShortDescription()
    {
        QString s;
        bool n = false;
        if(bname)                       { s = "%" + name + "%"; n = true; }
        if(n && (bminlevel || bmaxlevel)) { s += ", "; }
        if(bminlevel || bmaxlevel)      { s += "lvl "; n = true; }
        if(bminlevel)                   { s += QString::number(minlevel) + QString("-"); } else if(bmaxlevel) { s += QString("\?\?-"); }
        if(bmaxlevel)                   { s += QString::number(maxlevel); } else if(bminlevel) { s += QString("\?\?"); }
        if(n && (brank))                { s += ", "; }
        if(brank)                       { s += RankWidgetDelegate::displayRank(rank); n = true; }
        if(n && btype)                  { s += ", "; }
        if(btype)                       { s += TypeWidgetDelegate::displayType(type); n = true; }
        return s;
    }
};

#endif // CREATURETEMPLATEQUERY_H
