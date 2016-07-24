#ifndef CREATUREQUERY_H
#define CREATUREQUERY_H

#include <QWidget>
#include <QSql>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QApplication>

#include "creaturetemplatequery.h"

class MapWidgetDelegate : public QStyledItemDelegate
{
public:
    static QString displayMap(int map)
    {
        switch(map)
        {
        case 0: return "Eastern Kingdoms";
        case 1: return "Kalimdor";
        case 530: return "Outlands";
        case 369: return "Deeprun Tram";
        case 389: return "Ragefire Chasm";
        case 34: return "The Stockades";
        case 36: return "Deadmines";
        case 43: return "Wailing Caverns";
        case 33: return "Shadowfang Keep";
        case 48: return "Blackfathom Deeps";
        case 47: return "Razorfen Kraul";
        case 129: return "Razorfen Downs";
        case 90: return "Gnomeregan";
        case 189: return "Scarlet Monastery";
        case 70: return "Uldaman";
        case 209: return "Zul'Farrak";
        case 349: return "Mauradon";
        case 109: return "Sunken Temple";
        case 230: return "Blackrock Depths";
        case 229: return "Blackrock Spire";
        case 289: return "Scholomance";
        case 329: return "Stratholme";
        case 429: return "Dire Maul";
        case 309: return "Zul'Gurub";
        case 249: return "Onyxia's Lair";
        case 409: return "Molten Core";
        case 469: return "Blackwing Lair";
        case 509: return "Ahn'Qiraj Ruins";
        case 531: return "Ahn'Qiraj Temple";
        case 533: return "Naxxramas";
        case 558: return "Auchenai Crypts";
        case 557: return "Mana Tombs";
        case 556: return "Sethekk Halls";
        case 555: return "Shadow Labyrinth";
        case 560: return "Old Hillsbrad";
        case 269: return "Black Morass";
        case 534: return "Mount Hyjal";
        case 547: return "Slave Pens";
        case 546: return "Underbog";
        case 545: return "Steamvaults";
        case 548: return "Serpentshrine Cavern";
        case 543: return "Hellfire Ramparts";
        case 542: return "Blood Furnace";
        case 540: return "Shattered Halls";
        case 544: return "Magtheridon's Lair";
        case 554: return "The Mechanar";
        case 553: return "Botanica";
        case 552: return "Arcatraz";
        case 550: return "The Eye";
        case 585: return "Magister's Terrace";
        case 532: return "Karazhan";
        case 568: return "Zul'Aman";
        case 565: return "Gruul's Lair";
        case 564: return "Black Temple";
        case 580: return "Sunwell Plateau";
        case 449: return "Alliance PvP Barracks";
        case 450: return "Horde PvP Barracks";
        case 30: return "Alterac Valley";
        case 529: return "Arathi Basin";
        case 37: return "Azshara Crater";
        case 566: return "The Eye of The Storm";
        case 489: return "Warsong Gulch";
        case 559: return "Nagrand Arena";
        case 562: return "Blade's Edge Arena";
        }
        return "Unspecified";
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.column() == 1)
        {
            int map = index.data().toInt();
            QStyleOptionViewItem opt = option;
            opt.text = displayMap(map);
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem,&opt,painter);
        }
        else
        {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

namespace Ui {
class CreatureQuery;
}

class CreatureQuery : public QWidget
{
    Q_OBJECT

    static int indexToMap(int index)
    {
        switch (index)
        {
        case 0: return -1;
        case 1: return 0;
        case 2: return 1;
        case 3: return 530;
        case 4: return 369;
        case 5: return 389;
        case 6: return 34;
        case 7: return 36;
        case 8: return 43;
        case 9: return 33;
        case 10: return 48;
        case 11: return 47;
        case 12: return 129;
        case 13: return 90;
        case 14: return 189;
        case 15: return 70;
        case 16: return 209;
        case 17: return 349;
        case 18: return 109;
        case 19: return 230;
        case 20: return 229;
        case 21: return 289;
        case 22: return 329;
        case 23: return 429;
        case 24: return 309;
        case 25: return 249;
        case 26: return 409;
        case 27: return 469;
        case 28: return 509;
        case 29: return 531;
        case 30: return 533;
        case 31: return 558;
        case 32: return 557;
        case 33: return 556;
        case 34: return 555;
        case 35: return 560;
        case 36: return 269;
        case 37: return 534;
        case 38: return 547;
        case 39: return 546;
        case 40: return 545;
        case 41: return 548;
        case 42: return 543;
        case 43: return 542;
        case 44: return 540;
        case 45: return 544;
        case 46: return 554;
        case 47: return 553;
        case 48: return 552;
        case 49: return 550;
        case 50: return 585;
        case 51: return 532;
        case 52: return 568;
        case 53: return 565;
        case 54: return 564;
        case 55: return 580;
        case 56: return 449;
        case 57: return 450;
        case 58: return 30;
        case 59: return 529;
        case 60: return 37;
        case 61: return 566;
        case 62: return 489;
        case 63: return 559;
        case 64: return 562;
        }
        return -1;
    }

    static int mapIndex(int map)
    {
        switch(map)
        {
        case 0: return 1;
        case 1: return 2;
        case 530: return 3;
        case 369: return 4;
        case 389: return 5;
        case 34: return 6;
        case 36: return 7;
        case 43: return 8;
        case 33: return 9;
        case 48: return 10;
        case 47: return 11;
        case 129: return 12;
        case 90: return 13;
        case 189: return 14;
        case 70: return 15;
        case 209: return 16;
        case 349: return 17;
        case 109: return 18;
        case 230: return 19;
        case 229: return 20;
        case 289: return 21;
        case 329: return 22;
        case 429: return 23;
        case 309: return 24;
        case 249: return 25;
        case 409: return 26;
        case 469: return 27;
        case 509: return 28;
        case 531: return 29;
        case 533: return 30;
        case 558: return 31;
        case 557: return 32;
        case 556: return 33;
        case 555: return 34;
        case 560: return 35;
        case 269: return 36;
        case 534: return 37;
        case 547: return 38;
        case 546: return 39;
        case 545: return 40;
        case 548: return 41;
        case 543: return 42;
        case 542: return 43;
        case 540: return 44;
        case 544: return 45;
        case 554: return 46;
        case 553: return 47;
        case 552: return 48;
        case 550: return 49;
        case 585: return 50;
        case 532: return 51;
        case 568: return 52;
        case 565: return 53;
        case 564: return 54;
        case 580: return 55;
        case 449: return 56;
        case 450: return 57;
        case 30: return 58;
        case 529: return 59;
        case 37: return 60;
        case 566: return 61;
        case 489: return 62;
        case 559: return 63;
        case 562: return 64;
        }
        return 0;
    }

public:
    explicit CreatureQuery(QSqlDatabase &db,QWidget *parent = 0);
    ~CreatureQuery();
private:
    QSqlDatabase &db;
    Ui::CreatureQuery *ui;
    CreatureTemplateQuery *ctq;
    QSqlQueryModel sqlmodel;

    bool hasCreatureTemplateSpecifiers;
    bool bmap;int map;
    bool bmix;float mix;bool bmax;float max;
    bool bmiy;float miy;bool bmay;float may;
    bool bmiz;float miz;bool bmaz;float maz;
    bool bmio;float mio;bool bmao;float mao;
    bool bmih;int mih;bool bmah;int mah;
    bool bmim;int mim;bool bmam;int mam;

    QString getCtqText();
    void registerUI();
public:
    void setCreatureTemplateQuery(CreatureTemplateQuery *ctq);
    void setMap(int s){ map = indexToMap(s); bmap = (s != 0); }
    void setMinXCoord(QString s){ mix = s.toFloat(); bmix = (s.length() != 0); }
    void setMaxXCoord(QString s){ max = s.toFloat(); bmax = (s.length() != 0); }
    void setMinYCoord(QString s){ miy = s.toFloat(); bmiy = (s.length() != 0); }
    void setMaxYCoord(QString s){ may = s.toFloat(); bmay = (s.length() != 0); }
    void setMinZCoord(QString s){ miz = s.toFloat(); bmiz = (s.length() != 0); }
    void setMaxZCoord(QString s){ maz = s.toFloat(); bmaz = (s.length() != 0); }
    void setMinOrientation(QString s){ mio = s.toFloat(); bmio = (s.length() != 0); }
    void setMaxOrientation(QString s){ mao = s.toFloat(); bmao = (s.length() != 0); }
    void setMinHP(QString s){ mih = s.toInt(); bmih = (s.length() != 0); }
    void setMaxHP(QString s){ mah = s.toInt(); bmah = (s.length() != 0); }
    void setMinMana(QString s){ mim = s.toInt(); bmim = (s.length() != 0); }
    void setMaxMana(QString s){ mam = s.toInt(); bmam = (s.length() != 0); }

    QString makeQuery()
    {
        QString query = "SELECT ct.name, c.map, c.position_x, c.position_y, c.position_z FROM creature AS c"
                ", creature_template AS ct WHERE c.id = ct.entry";
        if(bmap){ query += " AND c.map = " + QString::number(map); }
        if(bmix){ query += " AND c.position_x >= " + QString::number(mix); }
        if(bmax){ query += " AND c.position_x <= " + QString::number(max); }
        if(bmiy){ query += " AND c.position_y >= " + QString::number(miy); }
        if(bmay){ query += " AND c.position_y <= " + QString::number(may); }
        if(bmiz){ query += " AND c.position_z >= " + QString::number(miz); }
        if(bmaz){ query += " AND c.position_z <= " + QString::number(maz); }
        if(bmio){ query += " AND c.orientation >= " + QString::number(mio); }
        if(bmao){ query += " AND c.orientation <= " + QString::number(mao); }
        if(bmih){ query += " AND c.curhealth >= " + QString::number(mih); }
        if(bmah){ query += " AND c.curhealth <= " + QString::number(mah); }
        if(bmim){ query += " AND c.curmana >= " + QString::number(mim); }
        if(bmam){ query += " AND c.curmana <= " + QString::number(mam); }
        if(hasCreatureTemplateSpecifiers)
        {
            query += ctq->make_query_specifiers("ct",true);
        }
        return query;
    }

    QString make_query_specifiers(QString alias, bool hasSp)
    {
        QString query;
        bool hasSpecifiers = hasSp;
        if(hasSpecifiers){ query += " AND "; }
        if(bmap){ query += alias + ".map = " + QString::number(map); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmix){ query += alias + ".position_x >= " + QString::number(mix); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmax){ query += alias + ".position_x <= " + QString::number(max); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmiy){ query += alias + ".position_y >= " + QString::number(miy); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmay){ query += alias + ".position_y <= " + QString::number(may); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmiz){ query += alias + ".position_z >= " + QString::number(miz); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmaz){ query += alias + ".position_z <= " + QString::number(maz); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmio){ query += alias + ".orientation >= " + QString::number(mio); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmao){ query += alias + ".orientation <= " + QString::number(mao); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmih){ query += alias + ".curhealth >= " + QString::number(mih); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmah){ query += alias + ".curhealth <= " + QString::number(mah); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmim){ query += alias + ".curmana >= " + QString::number(mim); hasSpecifiers = true; }
        if(hasSpecifiers){ query += " AND "; }
        if(bmam){ query += alias + ".curmana <= " + QString::number(mam); }
        return query;
    }
private slots:
    void on_QueryButton_clicked();
    void on_ClearButton_clicked();
    void on_EditCreatureTemplateButton_clicked();
signals:
    void pushedQueryCreatureTemplate();
};

#endif // CREATUREQUERY_H
