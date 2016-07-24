#include <QStringList>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <algorithm>
#include "dbcache.h"
#include "condition.h"
#include <QList>
#include <QTreeWidgetItem>
#include <algorithm>
#include <QtConcurrent/QtConcurrent>
#include <QSqlError>

#include "database.h"


dbcache::dbcache(dbccache * c, QSqlDatabase &db, entityviews::dbcs_module_type &dbc) :
    QObject(),
    m_dbccache(c)
{
    item_extended_cost_resource             = new QStringList();
    disenchant_loot_resource                = new QList<QTreeWidgetItem*>();
    disenchant_loot_references              = new std::vector<int>();
    item_random_property_resource           = new QList<QTreeWidgetItem*>();
    item_random_property_references         = new std::vector<int>();
    item_random_suffix_resource             = new QList<QTreeWidgetItem*>();
    item_random_suffix_references           = new std::vector<int>();
    lock_type_resource                      = new QStringList();
    condition_resource                      = new QStringList();
    condition_references                    = new std::vector<int>();
    npc_text_resource                       = new std::multimap<int,npc_text_data>();
    gossip_menu_resource                    = new std::multimap<int,gossip_menu_data>();
    gossip_option_resource                  = new std::multimap<int,gossip_option_data>();
    gossip_resource                         = new QList<QTreeWidgetItem*>();
    gossip_references                       = new std::vector<int>();
    gossip_tree_data                        = new std::vector<int>();

    loadDB(db,dbc);
}

dbcache::~dbcache()
{
    delete item_extended_cost_resource;
    delete disenchant_loot_resource;
    delete disenchant_loot_references;
    delete item_random_property_resource;
    delete item_random_property_references;
    delete item_random_suffix_resource;
    delete item_random_suffix_references;
    delete lock_type_resource;
    delete condition_resource;
    delete condition_references;
    delete npc_text_resource;
    delete gossip_menu_resource;
    delete gossip_option_resource;
    delete gossip_resource;
    delete gossip_references;
    delete gossip_tree_data;
}

void dbcache::item_extended_cost_init(QSqlDatabase &db, entityviews::dbcs_module_type &dbc)
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
    item_extended_cost_resource->clear();
    std::map<int,QString> l_map{};
    int args = m_dbccache->get_item_extended_cost_item_entries()->size();
    if(args)
    {
        bool isNotFirst = false;
        QString query = "SELECT entry,name FROM item_template WHERE";
        for(int i : *(m_dbccache->get_item_extended_cost_item_entries()))
        {
            if(isNotFirst){ query += " OR"; }
            query += " entry=" + QString::number(i);
            isNotFirst = true;
        }
        query += ";";
        QSqlQuery result = db.exec(query);

        while(result.next())
        {
            int entry = result.record().value(0).toInt();
            QString n = result.record().value(1).toString();

            l_map.insert({entry,n});
        }
        // ensure all entries have a name in the map
        for(int i : *(m_dbccache->get_item_extended_cost_item_entries()))
        {
            if(l_map.find(i) == l_map.end())
            {
                l_map.insert({i,QString("DB Entry ") + QString::number(i)});
            }
        }
    }

    item_extended_cost tmp;

    for(int i = 0; i < static_cast<honorcost*>(&dbc)->get_size(); i++)
    {
        item_extended_cost_resource->push_back(tmp.getDisplay(dbc,l_map,i));
    }
}

void dbcache::disenchant_loot_init(QSqlDatabase &db)
{
    disenchant_loot_resource->clear();
    disenchant_loot_references->clear();

    int lastid = 0;
    int index = 0;
    int id;
    float weightedIlvl;
    float weightedQuality;
    int totCount;
    float weightedCount;
    QTreeWidgetItem * h;
    QSqlQuery q;/* = db.exec("SELECT de.entry,de.ChanceOrQuestChance,it.name,it.ItemLevel,it.Quality,de.mincountOrRef,de.maxcount "
                          "FROM disenchant_loot_template AS de,"
                          "item_template AS it WHERE de.item = it.entry ORDER BY de.entry ASC;");*/
    dbcache_util::loot d;
    while(q.next())
    {
        id = q.record().value(0).toInt();
        if(id == lastid)
        {
            float avgCount = (q.record().value(5).toInt()+q.record().value(6).toInt())*0.5;
            weightedQuality += q.record().value(4).toInt()*q.record().value(1).toFloat()*0.01;
            weightedIlvl += q.record().value(3).toInt()*q.record().value(1).toFloat()*0.01;
            totCount += q.record().value(5).toInt();
            weightedCount += (avgCount*q.record().value(1).toFloat()*0.01);
            // Add new childwidget to h here
            QTreeWidgetItem * child = new QTreeWidgetItem(h);
            QString t = QString::number(q.record().value(1).toFloat()) + "%";
            t += QString(" ").repeated(8-t.length()) + "[";
            t += q.record().value(2).toString();
            t += "]x" + QString::number(avgCount,'f',2);
            child->setText(0,t);
            child->setData(1,Qt::UserRole,QVariant(index));
        }
        else
        {
            if(lastid != 0)
            {
                // Add header here
                QString headerText = QString::number(weightedCount,'f',2) + "x[" + d.getQuality(weightedQuality);
                headerText += ", Level: " + QString::number(weightedIlvl) + "]";
                h->setText(0, headerText);
                disenchant_loot_resource->push_back(h);
                disenchant_loot_references->push_back(lastid);
                index++;
            }
            float avgCount = (q.record().value(5).toInt()+q.record().value(6).toInt())*0.5;
            weightedQuality = q.record().value(4).toInt()*q.record().value(1).toFloat()*0.01;
            weightedIlvl = q.record().value(3).toInt()*q.record().value(1).toFloat()*0.01;
            totCount = q.record().value(5).toInt();
            weightedCount = (avgCount*q.record().value(1).toFloat()*0.01);
            // Make new header widget
            h = new QTreeWidgetItem();
            h->setData(1,Qt::UserRole,QVariant(index));
            lastid = id;
            // Add new childwidget to h here
            QTreeWidgetItem * child = new QTreeWidgetItem(h);
            QString t = QString::number(q.record().value(1).toFloat()) + "%";
            t += QString(" ").repeated(8-t.length()) + "[";
            t += q.record().value(2).toString();
            t += "]x" + QString::number(avgCount,'f',2);
            child->setText(0,t);
            child->setData(1,Qt::UserRole,QVariant(index));
        }
    }
    QString headerText = QString::number(weightedCount,'f',2) + "x[" + d.getQuality(weightedQuality);
    headerText += ", Level: " + QString::number(weightedIlvl) + "]";
    h->setText(0, headerText);
    disenchant_loot_resource->push_back(h);
    disenchant_loot_references->push_back(id);
}

void dbcache::item_random_property_and_suffix_init(QSqlDatabase &db, entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemrandompropertyname     name;
    typedef entityviews::itemrandompropertyenchants enchants;
    typedef entityviews::itemrandomsuffixname       suffixname;
    typedef entityviews::itemrandomsuffixenchants   suffixenchants;
    typedef entityviews::itemenchant                enchant;
    item_random_property_resource->clear();
    item_random_property_references->clear();
    item_random_suffix_resource->clear();
    item_random_suffix_references->clear();
    QTreeWidgetItem * cur_p;
    QTreeWidgetItem * cur_s;
    int index_p = -1;
    int index_s = -1;
    QString power;
    // initialization below is dependant on that query result is ordered
    QSqlQuery q = db.exec("SELECT entry,ench,chance FROM item_enchantment_template ORDER BY entry ASC;");
    int currentEntry = 0;
    int lastEntry = -1;
    bool entryvalidforproperty = true; /* this is used because the DB table is shared between two DBC files, and
                                          some entries have invalid ench values for either of them in some cases */
    bool entryvalidforsuffix = true;
    while(q.next())
    {
        // while currentEntry == lastEntry continue to build on the previous iterations' string
        // How to display? Relevant is the enchant and the power of the enchant. Name alone does not convey that.
        // Use QTreeView, like browsing a file system, Power will be Head
        // For now, display the first enchant as the power
        currentEntry = q.value(0).toInt();
        int ench_p = q.value(1).toInt();
        int ench_s = ench_p;
        int ench = ench_p;
        int chance = q.value(2).toFloat();
        int ench1,ench2,ench3,ench4,ench5;
        int enchs1,enchs2,enchs3;
        if(entryvalidforproperty)
        {
            ench_p = static_cast<enchants*>(&dbc)->find_ref_binary(ench_p); // ench is now the index in the DBC, or -1
            entryvalidforproperty = (ench_p != -1);
            if(!entryvalidforproperty && (currentEntry == lastEntry))
            {
                // discard currently worked on 'cur_p'
                index_p--;
                delete item_random_property_resource->last();
                item_random_property_resource->removeLast();
                item_random_property_references->pop_back();
            }
        }
        if(entryvalidforsuffix)
        {
            ench_s = static_cast<suffixenchants*>(&dbc)->find_ref_binary(ench_s);
            entryvalidforsuffix = (ench_s != -1);
            if(!entryvalidforsuffix && (currentEntry == lastEntry))
            {
                // discard currently worked on 'cur_s'
                index_s--;
                delete item_random_suffix_resource->last();
                item_random_suffix_resource->removeLast();
                item_random_suffix_references->pop_back();
            }
        }
        // output of "get_label" is an int pointer here
        if(entryvalidforproperty)
        {
            ench1 = *(static_cast<enchants*>(&dbc)->get_label(ench_p));
            ench2 = *(static_cast<enchants*>(&dbc)->get_label(ench_p) + 1);
            ench3 = *(static_cast<enchants*>(&dbc)->get_label(ench_p) + 2);
            ench4 = *(static_cast<enchants*>(&dbc)->get_label(ench_p) + 3);
            ench5 = *(static_cast<enchants*>(&dbc)->get_label(ench_p) + 4);
        }
        if(entryvalidforsuffix)
        {
            enchs1 = *(static_cast<suffixenchants*>(&dbc)->get_label(ench_s));
            enchs2 = *(static_cast<suffixenchants*>(&dbc)->get_label(ench_s) + 1);
            enchs3 = *(static_cast<suffixenchants*>(&dbc)->get_label(ench_s) + 2);
        }
        if(currentEntry != lastEntry)
        {
            lastEntry = currentEntry;
            ench_p = static_cast<enchants*>(&dbc)->find_ref_binary(ench);
            ench_s = static_cast<suffixenchants*>(&dbc)->find_ref_binary(ench);
            entryvalidforproperty = (ench_p!=-1);
            entryvalidforsuffix = (ench_s!=-1);
            if(entryvalidforproperty)
            {
                index_p++;
                power = "Power like: ";
                if(ench1!=0){ power += QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench1))); }
                if(ench2!=0){ power += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench2))); }
                if(ench3!=0){ power += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench3))); }
                if(ench4!=0){ power += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench4))); }
                if(ench5!=0){ power += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench5))); }
                cur_p = new QTreeWidgetItem();
                cur_p->setText(0,power);
                cur_p->setData(1,Qt::UserRole, QVariant(index_p));
                item_random_property_resource->push_back(cur_p);
                item_random_property_references->push_back(currentEntry);
            }
            if(entryvalidforsuffix)
            {
                index_s++;
                power = "Power like: ";
                if(enchs1!=0){ power += QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(enchs1))); }
                if(enchs2!=0){ power += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(enchs2))); }
                if(enchs3!=0){ power += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(enchs3))); }
                cur_s = new QTreeWidgetItem();
                cur_s->setText(0,power);
                cur_s->setData(1,Qt::UserRole, QVariant(index_s));
                item_random_suffix_resource->push_back(cur_s);
                item_random_suffix_references->push_back(currentEntry);
            }
        }
        if(entryvalidforproperty)
        {
            QString text = QString::number(chance);
            text += ("%" + QString(" ").repeated(7-text.length()));
            text += static_cast<name*>(&dbc)->get_label(ench_p);
            if(ench1!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench1))); }
            if(ench2!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench2))); }
            if(ench3!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench3))); }
            if(ench4!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench4))); }
            if(ench5!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(ench5))); }

            QTreeWidgetItem * newitem = new QTreeWidgetItem(cur_p);
            newitem->setText(0, text);
            newitem->setData(1,Qt::UserRole,QVariant(index_p));
        }
        if(entryvalidforsuffix)
        {
            QString text = QString::number(chance);
            text += ("%" + QString(" ").repeated(7-text.length()));
            text += static_cast<suffixname*>(&dbc)->get_label(ench_s);
            if(enchs1!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(enchs1))); }
            if(enchs2!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(enchs2))); }
            if(enchs3!=0){ text += ", " + QString(static_cast<enchant*>(&dbc)->get_label(static_cast<enchant*>(&dbc)->find_ref(enchs3))); }

            QTreeWidgetItem * newitem = new QTreeWidgetItem(cur_s);
            newitem->setText(0, text);
            newitem->setData(1,Qt::UserRole,QVariant(index_s));
        }
    }
}

void dbcache::lock_type_init(QSqlDatabase &db, entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::lockskind      kinds;
    typedef entityviews::locksid        ids;
    typedef entityviews::lockskill      skills;
    typedef entityviews::locktype       types;

    lock_type_resource->clear();

    std::map<int,QString> entrynames;

    // First fetch all item names from the database
    bool hasEntries = false;
    QString query = "";
    for(int i : *(m_dbccache->get_lock_type_item_entries()))
    {
        if(hasEntries){ query += " OR "; }
        query += QString::number(i);
        hasEntries = true;
    }
    if(hasEntries)
    {
        query = "SELECT entry,name FROM item_template WHERE " + query + ";";
        QSqlQuery q = db.exec(query);
        while(q.next())
        {
            entrynames.insert({q.record().value(0).toInt(),q.record().value(1).toString()});
        }
    }

    bool dirty;
    QString tmp;
    for(int i = 0; i < static_cast<kinds*>(&dbc)->get_size(); i++)
    {
        dirty = false;
        tmp = "(" + QString::number(static_cast<kinds*>(&dbc)->get_ref(i)) + ")\t";

        for(int j = 0; j < 8; j++)
        {
            switch(*(static_cast<kinds*>(&dbc)->get_label(i) + j))
            {
            case 0: break;
            case 1:
            {
                if(dirty)   { tmp += ", "; }
                // lockid point to an item in the entrynames map
                int id = *(static_cast<ids*>(&dbc)->get_label(i) + j);
                if(entrynames.find(id) != entrynames.end())
                {
                    tmp += entrynames[id];
                }
                else
                {
                    tmp += "DB Entry " + QString::number(id);
                }

                dirty = true;
                break;
            }
            case 2:
            {
                // lockid point to LockType.dbc
                int id = *(static_cast<ids*>(&dbc)->get_label(i) + j);
                if(id == 0){ break; }
                int skill = *(static_cast<skills*>(&dbc)->get_label(i) + j);
                if(dirty)   { tmp += ", "; }

                tmp += QString(static_cast<types*>(&dbc)->get_label(id-1));
                if(skill != 0)
                {
                    tmp += "(" + QString::number(skill) + ")";
                }

                dirty = true;
                break;
            }
            default:
                break;
            }
        }
        lock_type_resource->push_back(tmp);
    }

}

void dbcache::condition_init(QSqlDatabase &db)
{
    condition_resource->clear();
    condition_references->clear();

    // First load all entries into a temporary array
    QString query = "SELECT condition_entry,type,value1,value2 FROM conditions;";
    QSqlQuery q = db.exec(query);
    std::map<int,tripleint> tmp;
    int size = 0;
    while(q.next())
    {
        size++;
        int entry = q.record().value(0).toInt();
        int type = q.record().value(1).toInt();
        int value1 = q.record().value(2).toInt();
        int value2 = q.record().value(3).toInt();
        tmp.insert({entry,tripleint{type,value1,value2}});
    }
    for(std::pair<int,tripleint> v : tmp)
    {
        condition_resource->push_back(condition::label(v.first,tmp,*m_dbccache));
        condition_references->push_back(v.first);
    }
}

void dbcache::npc_text_init(QSqlDatabase &db)
{
    npc_text_resource->clear();

    QString query = "SELECT ID,prob0,text0_0,text0_1,prob1,text1_0,text1_1,prob2,text2_0,text2_1"
            ",prob3,text3_0,text3_1,prob4,text4_0,text4_1,prob5,text5_0,text5_1"
            ",prob6,text6_0,text6_1,prob7,text7_0,text7_1 FROM npc_text;";
    QSqlQuery q = db.exec(query);
    while(q.next())
    {
        int ID = q.record().value(0).toInt();
        for(int i = 0; i < 8; i++)
        {
            float p = q.record().value(1+i*3).toFloat();
            QString t0 = q.record().value(2+i*3).toString();
            QString t1 = q.record().value(3+i*3).toString();
            // if p != 0, then save p, t0 and t1
            if(p != 0)
            {
                npc_text_resource->insert({ID,std::tuple<float,QString,QString>{p,t0,t1}});
            }
        }
    }
}

void dbcache::gossip_menu_init(QSqlDatabase &db)
{
    gossip_menu_resource->clear();
    all_menus.clear();

    QString query = "SELECT entry,text_id,script_id,condition_id FROM gossip_menu;";
    QSqlQuery q = db.exec(query);

    while(q.next())
    {
        int entry = q.record().value(0).toInt();
        int text = q.record().value(1).toInt();
        int script = q.record().value(2).toInt();
        int cond = q.record().value(3).toInt();
        gossip_menu_resource->insert({entry,gossip_menu_data{text,script,cond}});
        all_menus.insert(entry);
    }
}

void dbcache::gossip_option_init(QSqlDatabase &db)
{
    gossip_option_resource->clear();

    QString query = "SELECT menu_id,id,option_text,option_id,npc_option_npcflag,action_menu_id,condition_id"
            " FROM gossip_menu_option";
    QSqlQuery q = db.exec(query);

    while(q.next())
    {
        int menu_id = q.record().value(0).toInt();
        int id      = q.record().value(1).toInt();
        QString txt = q.record().value(2).toString();
        int option  = q.record().value(3).toInt();
        int noption = q.record().value(4).toInt();
        int actionm = q.record().value(5).toInt();
        int cond    = q.record().value(6).toInt();
        gossip_option_resource->insert({menu_id,gossip_option_data{id,txt,option,noption,actionm,cond}});
    }
}

// Required to run after gossip_menu_init and gossip_option_init
// Seems to load in a fraction of a second, good enough considering its complexity
void dbcache::gossip_init(QSqlDatabase &db)
{
    gossip_resource->clear();
    gossip_references->clear();
    gossip_tree_data->clear();
    gossip_tree_data->push_back(-1);

    /* In case there are entries with the same id, then that means selection of text will be done as follows:
         - If theres no conditions on any of them, the first will be selected
         - If theres a condition on at least one of them, the one with highest condition id will be selected
    */
    // <gossip_menu_id, creature_template_entry>
    std::set<int> creature_menus;
    std::set<int> unique_creature_menus; // a menu is considered unique if it is not considered an entrance point by default
    QString query = "SELECT DISTINCT GossipMenuId FROM creature_template WHERE GossipMenuId != 0;";
    QSqlQuery q = db.exec(query);
    while(q.next())
    {
        creature_menus.insert(q.record().value(0).toInt());
    }
    std::map<int,gossip_menu_data> not_referenced_menus_by_option;
    std::set<int> referenced_menus_by_option;
    std::set<int> circular_references;
    std::map<int,int> completed_menus;

    for(auto it = gossip_option_resource->begin(); it != gossip_option_resource->end(); it++)
    {
        // Index 4 is action_menu_id
        gossip_option_data d = (*it).second;
        int action_menu_id = std::get<4>(d);
        if(action_menu_id != -1 && action_menu_id != 0)
        {
            referenced_menus_by_option.insert(action_menu_id);
        }
    }
    std::set_difference(gossip_menu_resource->begin(),gossip_menu_resource->end(),
                        referenced_menus_by_option.begin(),referenced_menus_by_option.end(),
                        std::inserter(not_referenced_menus_by_option,not_referenced_menus_by_option.end()),
                        dbcache_util::gossip_menu_cmp());
    std::set_difference(creature_menus.begin(),creature_menus.end(),
                        not_referenced_menus_by_option.begin(),not_referenced_menus_by_option.end(),
                        std::inserter(unique_creature_menus,unique_creature_menus.end()),
                        dbcache_util::gossip_menu_cmp());
    int localindex = 0;
    std::vector<int> localindices;
    std::map<int,int> indexmap;
    // Start with all menus from "not_referenced_menus_by_option"
    for(auto it = not_referenced_menus_by_option.begin(); it != not_referenced_menus_by_option.end(); ++it)
    {
        QTreeWidgetItem* newmenu = new QTreeWidgetItem();
        int menuid = (*it).first;
        dbcache_util::build_gossip_menu(newmenu,menuid,npc_text_resource,gossip_menu_resource);
        gossip_resource->push_back(newmenu);
        gossip_references->push_back(menuid);

        indexmap.clear();
        localindices.push_back(localindex);
        dbcache_util::build_gossip_menu_tree(menuid,menuid,indexmap,localindices,
                                             gossip_tree_data,newmenu,
                                             gossip_menu_resource,gossip_option_resource,npc_text_resource);
        localindices.pop_back();
        ++localindex;
        // All keys in indexmap should be unioned with completed_menus set (its a map but used entirely as a set)
        completed_menus.insert(indexmap.begin(),indexmap.end());
        completed_menus.insert({menuid,0});
    }
    // Now continue with all menus that are directly refered to by NPCs
    for(auto it = unique_creature_menus.begin(); it != unique_creature_menus.end(); ++it)
    {
        QTreeWidgetItem* newmenu = new QTreeWidgetItem();
        int menuid = *it;
        dbcache_util::build_gossip_menu(newmenu,menuid,npc_text_resource,gossip_menu_resource);
        gossip_resource->push_back(newmenu);
        gossip_references->push_back(menuid);

        indexmap.clear();
        localindices.push_back(localindex);
        dbcache_util::build_gossip_menu_tree(menuid,menuid,indexmap,localindices,
                                             gossip_tree_data,newmenu,
                                             gossip_menu_resource,gossip_option_resource,npc_text_resource);
        localindices.pop_back();
        ++localindex;
        // All keys in indexmap should be unioned with completed_menus set (its a map but used entirely as a set)
        completed_menus.insert(indexmap.begin(),indexmap.end());
        completed_menus.insert({menuid,0});
    }
    std::set_difference(all_menus.begin(),all_menus.end(),
                        completed_menus.begin(),completed_menus.end(),
                        std::inserter(circular_references,circular_references.end()),
                        dbcache_util::gossip_menu_cmp());
    // All gossip menus in circular_references are refered to by each other. Pick the first and do above routines on them
    while(circular_references.size())
    {
        QTreeWidgetItem* newmenu = new QTreeWidgetItem();
        int menuid = *(circular_references.begin());
        dbcache_util::build_gossip_menu(newmenu,menuid,npc_text_resource,gossip_menu_resource);
        gossip_resource->push_back(newmenu);
        gossip_references->push_back(menuid);

        indexmap.clear();
        localindices.push_back(localindex);
        dbcache_util::build_gossip_menu_tree(menuid,menuid,indexmap,localindices,
                                             gossip_tree_data,newmenu,
                                             gossip_menu_resource,gossip_option_resource,npc_text_resource);
        localindices.pop_back();
        ++localindex;
        // All keys in indexmap should be removed from circular_references if they exist there
        for(auto it = indexmap.begin(); it != indexmap.end(); ++it)
        {
            circular_references.erase((*it).first);
        }
    }
}

int dbcache::addNewGossipMenu(int menuid)
{

    std::vector<int> localindices;
    std::map<int,int> indexmap;
    QTreeWidgetItem* newmenu = new QTreeWidgetItem();
    dbcache_util::build_gossip_menu(newmenu,menuid,npc_text_resource,gossip_menu_resource);
    localindices.push_back(gossip_references->size());
    dbcache_util::build_gossip_menu_tree(menuid,menuid,indexmap,localindices,
                                         gossip_tree_data,newmenu,
                                         gossip_menu_resource,gossip_option_resource,npc_text_resource);
    gossip_references->push_back(menuid);
    gossip_resource->push_back(newmenu);
    return localindices.at(0);
}

void dbcache_util::build_gossip_menu_tree(const int top, int menuid, std::map<int,int> & indexmap,
                                          std::vector<int> & localindices, std::vector<int> *treedata,
                                          QTreeWidgetItem * res,
                                          std::multimap<int,std::tuple<int,int,int>> * menus,
                                          std::multimap<int,std::tuple<int,QString,int,int,int,int>> * options,
                                          std::multimap<int,std::tuple<float,QString,QString>> * texts)
{
    /*
     * Every option is a childwidget to the menu (res)
     *   data to provide is: - index to its submenu in the tree (a localindices stack)
     *                       - type of action (close, submenu, script, etc)
     *                       - its text
     *                       - condition to show it
     */
    // "menus"    < menuid, < text_id, script_id, condition_id > >
    // "options"  < menuid, < index, text, option_id, npc_flag, action_menu, condition_id> >
    int localindex = 0;
    localindices.push_back(localindex);
    for(auto it = options->lower_bound(menuid); it != options->upper_bound(menuid); ++it)
    {
        std::tuple<int,QString,int,int,int,int> optiondata = (*it).second;
        QTreeWidgetItem* newoption = new QTreeWidgetItem(res);
        newoption->setText(0, std::get<1>(optiondata));
        newoption->setData(1,Qt::UserRole,QVariant(bool(false))); // false that this is a menu (its an option)
        newoption->setData(2,Qt::UserRole,QVariant(std::get<2>(optiondata)));
        newoption->setData(3,Qt::UserRole,QVariant(std::get<4>(optiondata)));
        newoption->setData(4,Qt::UserRole,QVariant(std::get<5>(optiondata)));
        // For every item add it and its children to the working_set
        int childmenu = std::get<4>((*it).second);
        if(childmenu == -1)
        {
            newoption->setData(5,Qt::UserRole,QVariant(int(-1))); // close the gossip menu
            newoption->setData(6,Qt::UserRole,QVariant(int(0)));  // so its submenu index is 0 (nothing)
        }
        else if(childmenu == 0)
        {
            newoption->setData(5,Qt::UserRole,QVariant(int(0))); // No action taken
            newoption->setData(6,Qt::UserRole,QVariant(int(0))); // so its submenu index is 0 (nothing)
        }
        else
        {
            // Getting here means that the menu option has a child menu
            newoption->setData(5,Qt::UserRole,QVariant(int(1))); // Open submenu
            auto itc = indexmap.find(childmenu);
            if(itc == indexmap.end())
            {
                // Getting here means the child does not exist in the working set already: proceed to add it
                newoption->setData(6,Qt::UserRole,QVariant(int(treedata->size()))); // this is its submenu location in the tree
                indexmap.insert({childmenu,treedata->size()});
                for(int i = 0; i < localindices.size(); ++i)
                {
                    treedata->push_back(localindices[i]);
                }
                treedata->push_back(-1); // -1 signifies the end of the sequence
                // Continue to build the menu structure deeper down the hierarchy
                QTreeWidgetItem* newmenu = new QTreeWidgetItem(newoption);
                build_gossip_menu(newmenu,childmenu,texts,menus);
                build_gossip_menu_tree(top,childmenu,indexmap,localindices,treedata,newmenu,menus,options,texts);
            }
            else
            {
                // Find the already existing menus position in the tree (this allows "showing" circular gossip_menus in the tree)
                newoption->setData(6,Qt::UserRole,QVariant(int((*itc).second)));
            }
        }
        localindices.pop_back();
        ++localindex;
        localindices.push_back(localindex);
    }
    localindices.pop_back();
}

void dbcache_util::build_gossip_menu(QTreeWidgetItem *menu, int menuid,
                                     std::multimap<int, std::tuple<float, QString, QString> > *texts,
                                     std::multimap<int, std::tuple<int, int, int> > *menus)
{
    bool textfound = false;
    // Show a text if there is one, otherwise display "<No Text>"
    for(auto it_menu = menus->lower_bound(menuid); it_menu != menus->upper_bound(menuid);++it_menu)
    {
        int textid = std::get<0>((*it_menu).second);
        for(auto it_text = texts->lower_bound(textid); it_text != texts->upper_bound(textid); ++it_text)
        {
            std::tuple<float,QString,QString> textdata = (*it_text).second;
            if(std::get<1>(textdata).length() != 0)
            {
                menu->setText(0, std::get<1>(textdata));
                textfound = true;
                break;
            }
            else if(std::get<2>(textdata).length() != 0)
            {
                menu->setText(0, std::get<2>(textdata));
                textfound = true;
                break;
            }
        }
        if(textfound)
            break;
    }
    if(!textfound)
    {
        menu->setText(0,QString("<No Text>"));
    }
    menu->setData(1,Qt::UserRole,QVariant(bool(true))); // its true that this widget represents a menu
    menu->setData(2,Qt::UserRole,QVariant(menuid));
}

void dbcache::loadDB(QSqlDatabase &db, entityviews::dbcs_module_type &dbc)
{
    item_extended_cost_init(db,dbc);
    //disenchant_loot_init(db);
    item_random_property_and_suffix_init(db,dbc);
    lock_type_init(db,dbc);
    condition_init(db);
    npc_text_init(db);
    gossip_menu_init(db);
    gossip_option_init(db);
    gossip_init(db);
}

void dbcache::reloadDB(QSqlDatabase &db, entityviews::dbcs_module_type &dbc)
{
    loadDB(db,dbc);
}

QString dbcache::item_extended_cost::getDisplay(entityviews::dbcs_module_type &dbc, std::map<int,QString> &m, int index)
{
    QString tmp("");
    bool hasAtLeastOne = false;
    int a;
    if((a = static_cast<honorcost*>(&dbc)->get_label(index)) != 0)
    {
        tmp += QString::number(a) + " Honor Points";
        hasAtLeastOne = true;
    }
    if((a = static_cast<arenacost*>(&dbc)->get_label(index)) != 0)
    {
        if(hasAtLeastOne){ tmp += ", "; }
        hasAtLeastOne = true;
        tmp += QString::number(a) + " Arena Points";
    }
    if((a = static_cast<ratingreq*>(&dbc)->get_label(index)) != 0)
    {
        if(hasAtLeastOne){ tmp += ", "; }
        hasAtLeastOne = true;
        tmp += "Requires ";
        tmp += QString::number(a) + " Rating";
    }
    if((a = static_cast<item1*>(&dbc)->get_label(index)) != 0)
    {
        if(hasAtLeastOne){ tmp += ", "; }
        hasAtLeastOne = true;
        tmp += "["+m[a]+"]x";
        tmp += QString::number(static_cast<x1*>(&dbc)->get_label(index));
    }
    if((a = static_cast<item2*>(&dbc)->get_label(index)) != 0)
    {
        if(hasAtLeastOne){ tmp += ", "; }
        hasAtLeastOne = true;
        tmp += "["+m[a]+"]x";
        tmp += QString::number(static_cast<x2*>(&dbc)->get_label(index));
    }
    if((a = static_cast<item3*>(&dbc)->get_label(index)) != 0)
    {
        if(hasAtLeastOne){ tmp += ", "; }
        hasAtLeastOne = true;
        tmp += "["+m[a]+"]x";
        tmp += QString::number(static_cast<x3*>(&dbc)->get_label(index));
    }
    if((a = static_cast<item4*>(&dbc)->get_label(index)) != 0)
    {
        if(hasAtLeastOne){ tmp += ", "; }
        hasAtLeastOne = true;
        tmp += "["+m[a]+"]x";
        tmp += QString::number(static_cast<x4*>(&dbc)->get_label(index));
    }
    if((a = static_cast<item5*>(&dbc)->get_label(index)) != 0)
    {
        if(hasAtLeastOne){ tmp += ", "; }
        hasAtLeastOne = true;
        tmp += "["+m[a]+"]x";
        tmp += QString::number(static_cast<x5*>(&dbc)->get_label(index));
    }
    return tmp;
}
