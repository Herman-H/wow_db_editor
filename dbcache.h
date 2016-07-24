#ifndef DBCACHE_H
#define DBCACHE_H

#include <vector>
#include <stack>
#include <array>
#include <set>
#include <map>
#include <tuple>
#include "dbccache.h"
#include "dbc/dbcrecorddescriptors.h"
#include <QObject>
#include <QString>

template <typename>
class QList;
template <typename>
class QFuture;
template <typename>
class QFutureWatcher;
class QTreeWidgetItem;
class QStringList;
class QSqlDatabase;


class dbcache : public QObject
{
    Q_OBJECT
private:
    dbccache                                    * m_dbccache;
    typedef std::pair<int,int>                  doubleint;
    typedef std::tuple<int,int,int>             tripleint;
private:
    // Item Extended Cost
    QStringList                                 * item_extended_cost_resource;
    void item_extended_cost_init(QSqlDatabase &db, entityviews::dbcs_module_type &dbc);
    void page_text_init(QSqlDatabase &db);
    // Disenchant Loot
    QList<QTreeWidgetItem*>                     * disenchant_loot_resource;
    std::vector<int>                            * disenchant_loot_references;
    void disenchant_loot_init(QSqlDatabase &db);
    // Item Random Property/Suffix
    QList<QTreeWidgetItem*>                     * item_random_property_resource;
    QList<QTreeWidgetItem*>                     * item_random_suffix_resource;
    std::vector<int>                            * item_random_property_references;
    std::vector<int>                            * item_random_suffix_references;
    void item_random_property_and_suffix_init(QSqlDatabase &db, entityviews::dbcs_module_type &dbc);
    // Lock Type
    QStringList                                 * lock_type_resource;
    void lock_type_init(QSqlDatabase &db, entityviews::dbcs_module_type &dbc);
    // Condition
    QStringList                                 * condition_resource;
    std::vector<int>                            * condition_references;
    void condition_init(QSqlDatabase &db);
    // Npc Text
    typedef std::tuple<float,QString,QString> npc_text_data;
    std::multimap<int,npc_text_data>            * npc_text_resource;
    void npc_text_init(QSqlDatabase &db);
    // Gossip
    std::set<int>                                 all_menus;
    // <text_id,script_id,condition_id>
    typedef std::tuple<int,int,int> gossip_menu_data;
    // <index,text,option_id,npc_flag,action_menu,condition_id>
    typedef std::tuple<int,QString,int,int,int,int> gossip_option_data;
    std::multimap<int,gossip_menu_data>         * gossip_menu_resource;
    void gossip_menu_init(QSqlDatabase &db);
    std::multimap<int,gossip_option_data>       * gossip_option_resource;
    void gossip_option_init(QSqlDatabase &db);
    QList<QTreeWidgetItem*>                     * gossip_resource;
    std::vector<int>                            * gossip_tree_data;
    std::vector<int>                            * gossip_references;
    void gossip_init(QSqlDatabase &db);

    void loadDB(QSqlDatabase &db, entityviews::dbcs_module_type &dbc);
public:

    dbcache(dbccache *c, QSqlDatabase &db, entityviews::dbcs_module_type &dbc);
    ~dbcache();

    void reloadDB(QSqlDatabase &db, entityviews::dbcs_module_type &dbc);

    int addNewGossipMenu(int menuid);

    QStringList                             * get_item_extended_cost_resource(){ return item_extended_cost_resource; }
    std::vector<int>                        * get_item_extended_cost_references(){ return m_dbccache->get_item_extended_cost_references(); }
    QList<QTreeWidgetItem*>                 * get_disenchant_loot_resource(){ return disenchant_loot_resource; }
    std::vector<int>                        * get_disenchant_loot_references(){ return disenchant_loot_references; }
    QList<QTreeWidgetItem*>                 * get_item_random_property_resource(){ return item_random_property_resource; }
    std::vector<int>                        * get_item_random_property_references(){ return item_random_property_references; }
    QList<QTreeWidgetItem*>                 * get_item_random_suffix_resource(){ return item_random_suffix_resource; }
    std::vector<int>                        * get_item_random_suffix_references(){ return item_random_suffix_references; }
    QStringList                             * get_lock_type_resource(){ return lock_type_resource; }
    std::vector<int>                        * get_lock_type_references(){ return m_dbccache->get_lock_type_references(); }
    QStringList                             * get_condition_resource(){ return condition_resource; }
    std::vector<int>                        * get_condition_references(){ return condition_references; }
    std::multimap<int,npc_text_data>        * get_npc_text_resource(){ return npc_text_resource; }
    std::multimap<int,gossip_menu_data>     * get_gossip_menu_resource(){ return gossip_menu_resource; }
    std::multimap<int,gossip_option_data>   * get_gossip_option_data(){ return gossip_option_resource; }
    QList<QTreeWidgetItem*>                 * get_gossip_resource(){ return gossip_resource; }
    std::vector<int>                        * get_gossip_references(){ return gossip_references; }
    std::vector<int>                        * get_gossip_treedata(){ return gossip_tree_data; }
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
    struct firstelementmatches
    {
        int k;
        firstelementmatches(int k){ this->k = k; }
        bool operator () (tripleint i)
        {
            if(std::get<0>(i) == k)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };

public:
    static int find_ref(std::vector<tripleint> * v, int ref)
    {
        int index;
        firstelementmatches f(ref);

        auto it =  std::find_if(v->begin(),
                             v->end(), f);
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

    struct item_extended_cost
    {
        typedef entityviews::item1cost item1;
        typedef entityviews::item1count x1;
        typedef entityviews::item2cost item2;
        typedef entityviews::item2count x2;
        typedef entityviews::item3cost item3;
        typedef entityviews::item3count x3;
        typedef entityviews::item4cost item4;
        typedef entityviews::item4count x4;
        typedef entityviews::item5cost item5;
        typedef entityviews::item5count x5;
        typedef entityviews::honorcost honorcost;
        typedef entityviews::arenacost arenacost;
        typedef entityviews::ratingrequired ratingreq;
        QString getDisplay(entityviews::dbcs_module_type &dbc, std::map<int,QString> &m, int index);
    };

};

namespace dbcache_util
{

struct loot
{
    QString getQuality(float avg)
    {
        int quality = round(avg);
        switch(quality)
        {
        case 0: return QString("Poor");
        case 1: return QString("Common");
        case 2: return QString("Uncommon");
        case 3: return QString("Rare");
        case 4: return QString("Epic");
        case 5: return QString("Legendary");
        case 6: return QString("Artifact");
        }
        return "Unknown Quality";
    }
};

struct gossip_menu_cmp
{
public:
    bool operator()(const int & l, std::pair<const int,std::tuple<int,int,int>> & r) const
    {
        return l < r.first;
    }
    bool operator()(std::pair<const int,std::tuple<int,int,int>> & l, const int & r) const
    {
        return l.first < r;
    }

    bool operator()(const int & l, std::pair<const int,int> & r) const
    {
        return l < r.first;
    }
    bool operator()(std::pair<const int,int> & l, const int & r) const
    {
        return l.first < r;
    }
};

void build_gossip_menu_tree(const int top, int menuid, std::map<int, int> &indexmap,
                            std::vector<int> &localindices, std::vector<int> * treedata,
                            QTreeWidgetItem *res,
                            std::multimap<int,std::tuple<int,int,int>> * menus,
                            std::multimap<int,std::tuple<int,QString,int,int,int,int>> * options, std::multimap<int, std::tuple<float, QString, QString> > *texts);

void build_gossip_menu(QTreeWidgetItem * menu, int menuid,
                       std::multimap<int,std::tuple<float,QString,QString>> * texts,
                       std::multimap<int,std::tuple<int,int,int>> * menus);

}

#endif // DBCACHE_H
