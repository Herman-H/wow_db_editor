#include <QDialog>
#include <QTableView>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

#include "loot_selecter_widget.h"

creature_loot_group_item_model::creature_loot_group_item_model(resource_manager *res, QObject *parent) :
    QAbstractItemModel(parent),
    m_res(res)
{
}

QVariant creature_loot_group_item_model::headerData(int section, Qt::Orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return QString{"Dropped By"};
        case 1:
            return QString {"Poor"};
        case 2:
            return QString {"Common"};
        case 3:
            return QString {"Uncommon"};
        case 4:
            return QString {"Rare"};
        case 5:
            return QString {"Epic"};
        case 6:
            return QString {"Legendary"};

        }
    }
    return QVariant{};
}

creature_loot_group_dialog::creature_loot_group_dialog(creature_loot_group_item_model * model, const resource_manager * res) :
    m_model(model),
    m_res(res)
{
    widget = new QDialog{};
    widget->setLayout(layout = new QHBoxLayout{});
    layout->addWidget(scrollArea = new QScrollArea{});
    scrollArea->setLayout(llayout = new QVBoxLayout{});
    llayout->addWidget(overview = new QTableView{});

    widget->setWindowTitle(QString{"Creature Loot Select and Edit"});

    overview->setModel(m_model);
    overview->setFixedSize(350,700);
    overview->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    overview->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    overview->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    overview->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    overview->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    overview->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    overview->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    reset();
}



creature_loot_group_dialog::~creature_loot_group_dialog()
{

}

void creature_loot_group_dialog::setId(int id)
{
    // TODO
}
void creature_loot_group_dialog::reset()
{
    // TODO
    isSelectingValidId = false;
}

QString creature_loot_group_dialog::getString(int id) const
{
    if(id == unselectedId)
    {
        return QString{"Creature Loot..."};
    }
    else
    {
        return QString{"TODO"};
    }
}
void creature_loot_group_dialog::accept()
{
    widget->done(0);
}
void creature_loot_group_dialog::reject()
{
    widget->setResult(-1);
}
