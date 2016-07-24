#ifndef ITEMTEMPLATEQUERY_H
#define ITEMTEMPLATEQUERY_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QApplication>
#include "dbc/dbcrecorddescriptors.h"
#include "dbccache.h"
using namespace burning_crusade_records;

namespace Ui {
class itemtemplatequery;
}
class QSqlDatabase;
class QSqlQueryModel;
class QString;
class QPushButton;
class item_class_selecter;
class item_subclass_selecter;
class item_quality_selecter;
class item_slot_selecter;
class class_mask_selecter;
class race_mask_selecter;

class ItemClassWidgetDelegate : public QStyledItemDelegate
{
private:
    dbccache * m_dbccache;
public:
    ItemClassWidgetDelegate(dbccache * c) : QStyledItemDelegate(),m_dbccache(c){}

    static QString displayClass(int c, dbccache * C)
    {
        int index = dbccache::find_ref(C->get_item_class_references(),c);
        if(index == -1)
        {
            return "";
        }
        return C->get_item_class_resource()->at(index+1);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.column() == 2)
        {
            int clas = index.data().toInt();
            QStyleOptionViewItem opt = option;
            opt.text = displayClass(clas,m_dbccache);
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem,&opt,painter);
        }
        else
        {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

class ItemQualityWidgetDelegate : public QStyledItemDelegate
{
public:

    static QString displayQuality(int q)
    {
        switch(q)
        {
        case 0: return "Poor";
        case 1: return "Common";
        case 2: return "Uncommon";
        case 3: return "Rare";
        case 4: return "Epic";
        case 5: return "Legendary";
        case 6: return "Artifact";
        }
        return "Common";
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.column() == 1)
        {
            int q = index.data().toInt();
            QStyleOptionViewItem opt = option;
            opt.text = displayQuality(q);
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem,&opt,painter);
        }
        else
        {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

class ItemSlotWidgetDelegate : public QStyledItemDelegate
{
public:

    static QString displaySlot(int s)
    {
        switch(s)
        {
        case 0: return "Not Equipable";
        case 1: return "Head";
        case 2: return "Neck";
        case 3: return "Shoulder";
        case 4: return "Shirt";
        case 5: return "Chest";
        case 6: return "Waist";
        case 7: return "Legs";
        case 8: return "Feet";
        case 9: return "Wrists";
        case 10: return "Hands";
        case 11: return "Finger";
        case 12: return "Trinket";
        case 13: return "Weapon";
        case 14: return "Shield";
        case 15: return "Ranged";
        case 16: return "Back";
        case 17: return "Two-Hand";
        case 18: return "Bag";
        case 19: return "Tabard";
        case 20: return "Robe";
        case 21: return "Main Hand";
        case 22: return "Off Hand";
        case 23: return "Holdable (Tome)";
        case 24: return "Ammo";
        case 25: return "Thrown";
        case 26: return "Ranged Right";
        case 27: return "Quiver";
        case 28: return "Relic";
        }
        return "Common";
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.column() == 5)
        {
            int s = index.data().toInt();
            QStyleOptionViewItem opt = option;
            opt.text = displaySlot(s);
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem,&opt,painter);
        }
        else
        {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

class itemtemplatequery : public QWidget
{
    Q_OBJECT
private:
    QSqlDatabase &                  db;
    dbccache *                      m_dbccache;
    QSqlQueryModel &                queryModel;
    int                             selected;
    item_class_selecter *           classselecter;
    item_subclass_selecter *        subclassselecter;
    item_quality_selecter *         qualityselecter;
    item_slot_selecter *            slotselecter;
    class_mask_selecter *           classmask;
    race_mask_selecter *            racemask;
public:
    explicit itemtemplatequery(QSqlDatabase &db, dbccache * c, QWidget *parent = 0);
    ~itemtemplatequery();

    void setNameField(QString name);
    int selectedEntry(){ return selected; }
    QPushButton * cancelButton();
    QString currentQuerySpecifiers(QString alias, bool & hasSpecifiers);
private:
    Ui::itemtemplatequery *ui;
public slots:
    void onitemclass_selection();
private slots:
    void on_clearButton_clicked();
    void on_queryButton_clicked();
    void on_selectButton_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);

signals:
    void itemselected();
};

#endif // ITEMTEMPLATEQUERY_H
