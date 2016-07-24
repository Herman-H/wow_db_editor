#ifndef LOOT_SELECTER_WIDGET_H
#define LOOT_SELECTER_WIDGET_H

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QDialog;
class QTableView;
class QScrollArea;

#include <QString>
#include <QAbstractItemModel>
#include "./resource_manager.h"
#include "./resource_storage.h"
#include "selecter_dialog.h"
#include <map>


class creature_loot_group_item_model : public QAbstractItemModel
{
    Q_OBJECT
private:
    resource_manager * m_res;
    /* reference_loot -> loot_group that refers to it */
    std::multimap<int,int> reference_loot_relation;
    std::set<int>          invalidated_loot_groups;
public:
    creature_loot_group_item_model(resource_manager * res, QObject * parent = 0);
    ~creature_loot_group_item_model(){}

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        return createIndex(row,column,nullptr);
    }
    QModelIndex parent(const QModelIndex &parent) const
    {
        return QModelIndex{};
    }
    int rowCount(const QModelIndex &parent) const
    {
        return m_res->get_loot_group()->creature_loot_groups_size();
    }
    int columnCount(const QModelIndex &parent) const
    {
        return 7;
    }
    QVariant data(const QModelIndex &index, int role) const
    {
        return QVariant{};
    }
};

class creature_loot_group_dialog : public QObject, public selecter_dialog
{
    Q_OBJECT
private:
    QDialog *                           widget;
    QTableView *                        overview;
    QScrollArea *                       scrollArea;
    QHBoxLayout *                       layout;
    QVBoxLayout *                       llayout;
    QVBoxLayout *                       rlayout;
    QHBoxLayout *                       mlayout;
    QHBoxLayout *                       lblayout;
    QHBoxLayout *                       rmlayout;
    QHBoxLayout *                       rblayout;

    creature_loot_group_item_model *    m_model;
    const resource_manager * const      m_res;

    const int                           unselectedId = 0;
    bool                                isSelectingValidId;

public:
    creature_loot_group_dialog(creature_loot_group_item_model * model, const resource_manager * res);
    ~creature_loot_group_dialog();
    QDialog * getDialog() const { return widget; }
    void setId(int id);
    bool hasSelection() const { return isSelectingValidId; }
    void reset();
    QString getString(int id) const;
private slots:
public slots:
    void accept();
    void reject();
};

#endif // LOOT_SELECTER_WIDGET_H
