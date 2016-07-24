#ifndef RESOURCE_STORAGE_H
#define RESOURCE_STORAGE_H

#include <vector>
#include <stack>
#include <array>
#include <set>
#include <map>

#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "resource_initialization.h"


class strings_resource_storage
{
private:
    QSqlDatabase                                * m_db;
    std::map<int,int>                             creature_template_names; //<creature_id,name_string_start_index>
    std::map<int,int>                             item_template_names;     //<item_id,name_string_start_index>
    std::map<int,int>                             page_texts;              //<page_id,page_text_start_index>
    std::vector<char>                             strings_block;    // all text strings are allocated in this vector
    std::map<int,int>                             unused_fragments; // keep track of unused fragments here
    int allocate_string(const char *string, int &size);
    int insert_string(const char * str, int size);
    void remove_string(int index);

    int cstrlen(const char * c)
    {
        const char * c_ = c;
        int len = 0;
        while(*c_ != '\0')
        {
            ++c_;
            ++len;
        }
        return len;
    }

    enum class stringtype
    {
        creature_template,
        item_template,
        page_text
    };

    template <stringtype nt>
    struct identity{};

    template <stringtype nt>
    std::map<int,int>& get_id_index_map(identity<nt>){ return creature_template_names; }
    std::map<int,int>& get_id_index_map(identity<stringtype::creature_template>){ return creature_template_names; }
    std::map<int,int>& get_id_index_map(identity<stringtype::item_template>){ return item_template_names; }
    std::map<int,int>& get_id_index_map(identity<stringtype::page_text>){ return page_texts; }
    template <stringtype nt>
    std::map<int,int>& get_id_index_map(){ return get_id_index_map(identity<nt>()); }


    template <stringtype nt>
    QString get_name_spec(identity<nt>){ return QString(""); }
    QString get_name_spec(identity<stringtype::creature_template>){ return QString("Name"); }
    QString get_name_spec(identity<stringtype::item_template>){ return QString("name"); }
    QString get_name_spec(identity<stringtype::page_text>){ return QString("text"); }
    template <stringtype nt>
    QString get_name_spec(){ return get_name_spec(identity<nt>()); }


    template <stringtype nt>
    QString get_table_spec(identity<nt>){ return ""; }
    QString get_table_spec(identity<stringtype::creature_template>){ return QString("creature_template"); }
    QString get_table_spec(identity<stringtype::item_template>){ return QString("item_template"); }
    QString get_table_spec(identity<stringtype::page_text>){ return QString("page_text"); }
    template <stringtype nt>
    QString get_table_spec(){ return get_table_spec(identity<nt>()); }

    template <stringtype nt>
    QString get_id_spec(identity<nt>){ return ""; }
    QString get_id_spec(identity<stringtype::creature_template>){ return QString("Entry"); }
    QString get_id_spec(identity<stringtype::item_template>){ return QString("entry"); }
    QString get_id_spec(identity<stringtype::page_text>){ return QString("entry"); }
    template <stringtype nt>
    QString get_id_spec(){ return get_id_spec(identity<nt>()); }

    template <stringtype nt>
    char * get_string_impl(int id)
    {
        // First check if name exists in cache
        auto it = get_id_index_map<nt>().find(id);
        if(it != get_id_index_map<nt>().end())
        {
            int index = (*it).second;
            return (strings_block.data()+index);
        }
        // If not found, then query database for it and put it in the cache, then return the string
        else
        {
            QString queryStr = QString("SELECT ") + get_name_spec<nt>() + QString(" FROM ") + get_table_spec<nt>() +
                    QString(" WHERE ") + get_id_spec<nt>() + QString(" = ") + QString::number(id) + QString(";");
            QSqlQuery q = m_db->exec(queryStr);
            if(q.next())
            {
                QString s = q.record().value(0).toString();
                int index = insert_string(s.toStdString().c_str(),s.length());
                get_id_index_map<nt>().insert({id,index});
                return (strings_block.data()+index);
            }
        }
        return (strings_block.data()+1);
    }

    template <stringtype nt>
    void remove_string_impl(int id)
    {
        auto it = get_id_index_map<nt>().find(id);
        if(it != get_id_index_map<nt>().end())
        {
            int index = (*it).second;
            remove_string(index);
            get_id_index_map<nt>().erase(it);
        }
    }

    template <stringtype nt>
    void change_string_impl(int id, const char * newstring)
    {
        int oldsize = 0;
        int newsize = cstrlen(newstring);
        auto it = get_id_index_map<nt>().find(id);
        if(it != get_id_index_map<nt>().end())
        {
            int index = (*it).second;
            oldsize = cstrlen(strings_block.data()+index);
            if(oldsize == newsize) // simply overwrite the name on the spot here
            {
                const char * c = newstring;
                for(int i = index;  i < index+newsize; ++i)
                {
                    strings_block[i] = *c;
                    ++c;
                }
            }
            else // Otherwise delete the name and insert the new one where it fits
            {
                remove_string(index);
                get_id_index_map<nt>().erase(it);
                int newindex = insert_string(newstring,newsize);
                get_id_index_map<nt>().insert({id,newindex});
            }
        }
        else // The name didnt exist in the cache to begin with, just insert it
        {
            int newindex = insert_string(newstring,newsize);
            get_id_index_map<nt>().insert({id,newindex});
        }
    }

    template <stringtype nt>
    void add_string_impl(int id, const char *string)
    {
        // Assumes that id does not already exist in cache, otherwise cache may be erronous
        int size = 0;
        int index = allocate_string(string,size);
        get_id_index_map<nt>().insert({id,index});
    }

    void init_creature_names();
    void init_item_names();
    void init_page_texts();


public:
    strings_resource_storage(QSqlDatabase * db);
    ~strings_resource_storage();

    void initialize(resource_initialization_state & ris);

    char * get_creature_template_name(int id) { return get_string_impl<stringtype::creature_template>(id); }
    void add_creature_template_name(int id, const char * name){ add_string_impl<stringtype::creature_template>(id,name); }
    void remove_creature_template_name(int id){ remove_string_impl<stringtype::creature_template>(id); }
    void change_creature_template_name(int id, const char * newname) { change_string_impl<stringtype::creature_template>(id,newname); }

    char * get_item_template_name(int id) { return get_string_impl<stringtype::item_template>(id); }
    void add_item_template_name(int id, const char * name){ add_string_impl<stringtype::item_template>(id,name); }
    void remove_item_template_name(int id) { remove_string_impl<stringtype::item_template>(id); }
    void change_item_template_name(int id, const char * newname) { change_string_impl<stringtype::item_template>(id,newname); }

    char * get_page_text(int id) { return get_string_impl<stringtype::page_text>(id); }
    void add_page_text(int id, const char * text) { add_string_impl<stringtype::page_text>(id,text); }
    void remove_page_text(int id){ remove_string_impl<stringtype::page_text>(id); }
    void change_page_text(int id, const char * newtext){ change_string_impl<stringtype::page_text>(id,newtext); }

    void printAll()
    {
        char * it = strings_block.data();
        while(static_cast<unsigned int>(it-strings_block.data())+1 < strings_block.size())
        {
            qDebug(it);
            qDebug(QString::number(it-strings_block.data()).toLocal8Bit());
            int size = cstrlen(it);
            it+=size+1;
            auto fragit = unused_fragments.find(static_cast<int>(it-strings_block.data()));
            if(fragit != unused_fragments.end())
            {
                it += (*fragit).second+1;
            }
        }
    }
};

static void test_strings_resource_storage(QSqlDatabase * db)
{
    const char * c[] = {"Molusk", "Troll", "Giant", "Harpy", "Orc", "Undead", "Human", "Dreadlord", "Boar"};
    const char * i[] = {"Sword", "Item", "Key", "Shield", "Wand", "Robe", "Green Robe", "Red Shiny Robe"};
    strings_resource_storage rm{db};
    rm.add_creature_template_name(0,c[0]);
    rm.add_creature_template_name(1,c[1]);
    rm.add_creature_template_name(2,c[2]);
    rm.add_creature_template_name(3,c[3]);
    rm.add_creature_template_name(4,c[4]);
    rm.add_creature_template_name(5,c[5]);
    rm.add_item_template_name(0,i[0]);
    rm.add_item_template_name(1,i[1]);
    rm.add_item_template_name(2,i[2]);
    rm.add_item_template_name(3,i[3]);
    rm.add_item_template_name(4,i[4]);
    rm.add_item_template_name(5,i[5]);
    rm.printAll();
    rm.remove_creature_template_name(1);
    rm.remove_creature_template_name(3);
    rm.remove_creature_template_name(4);
    rm.remove_item_template_name(1);
    rm.remove_item_template_name(4);
    rm.remove_item_template_name(5);
    rm.printAll();
    rm.change_creature_template_name(1,c[1]);
    rm.change_creature_template_name(3,c[3]);
    rm.change_creature_template_name(4,c[4]);
    rm.change_item_template_name(1,i[1]);
    rm.change_item_template_name(4,i[4]);
    rm.change_item_template_name(5,i[5]);
    rm.printAll();
}

template <typename T>
class resource_vector
{
private:
    std::vector<T>                      vec;
    std::map<unsigned int,unsigned int> unused_fragments;
public:
    resource_vector(int reserve) :
        vec(std::vector<T>{}),
        unused_fragments(std::map<unsigned int,unsigned int>{})
    {
        vec.reserve(reserve);
    }

    T at(unsigned int i) const
    {
        return vec[i];
    }

    int allocate(const std::vector<T> & entries)
    {
        unsigned int index = vec.size();
        for(unsigned int i = 0; i < entries.size(); ++i)
        {
            vec.push_back(entries.at(i));
        }
        return index;
    }

    unsigned int insert(const std::vector<T> & entries)
    {
        unsigned int index = 0;
        unsigned int worstfitindex = 0;
        unsigned int worstfitsize = 0;
        const unsigned int size = entries.size();

        // Allocation now searches all fragments for a perfect fit. If not found, use worst fit
        for(auto it = unused_fragments.begin();
                 it != unused_fragments.end();
                 ++it)
        {
            std::pair<const unsigned int, unsigned int> & p = *it;
            if(p.second == size) // Perfect fit found
            {
                index = p.first;
                break;
            }
            else if(p.second > size && p.second > worstfitsize) // a 'worse' fit is found
            {
                worstfitindex = p.first;
                worstfitsize = p.second;
            }
        }
        if(index != 0)
        {
            // Allocate to perfect fit window at index
            unsigned int it = 0;
            for(unsigned int i = index; i < index+size; ++i)
            {
                vec[i] = entries.at(it);
                ++it;
            }
            unused_fragments.erase(index);
        }
        else if(worstfitindex != 0)
        {
            // Allocate to worst fit at index
            unsigned int it = 0;
            for(unsigned int i = worstfitindex; i < worstfitindex+size; ++i)
            {
                vec[i] = entries.at(it);
                ++it;
            }

            // Remove the fragment
            unused_fragments.erase(worstfitindex);

            // Insert a new fragment that is the rest at worstfitindex+size,
            unused_fragments.insert({worstfitindex+size,worstfitsize-size});
            index = worstfitindex;
        }
        else // No suitable fragment found, then allocate on the end, like allocate_group() function
        {
            index = vec.size();
            unsigned int it = 0;
            for(unsigned int i = 0; i < size; ++i)
            {
                vec.push_back(entries.at(it));
                ++it;
            }
        }
        return index;
    }

    void remove(unsigned int index, const unsigned int size)
    {
        // It is assumed that unused_fragments does not have index as an entry already
        unsigned int c = index + size;
        unsigned int s = size;
        // If removed data block is the last one in the block simply decrement the size of the block
        if(c-1 >= vec.size())
        {
            for(unsigned int i = 0; i < size; ++i)
            {
                vec.pop_back();
            }
            return;
        }
        // Merge fragments forward from index?
        auto it1 = unused_fragments.find(c);
        if(it1 != unused_fragments.end()) // Then we shall merge forward
        {
            s += (*it1).second;
            unused_fragments.erase(it1);
        }
        auto it2 = unused_fragments.insert({index,s}).first;
        // Merge fragments backward from index?
        if(it2 != unused_fragments.begin()) // To merge backwards, there has to exist a previous fragment
        {
            --it2;
            if((*it2).first + (*it2).second == index)
            {
                s += (*it2).second;
                unused_fragments.erase(index);
                index = (*it2).first;
                unused_fragments.erase(index);
                unused_fragments.insert({index,size});
            }
        }
    }

    void fill(const int index, const int size, std::vector<T> & vec) const
    {
        for(int i = 0; i < size; ++i)
        {
            vec.push_back(this->vec[index+i]);
        }
    }
};

struct loot_entry
{
    int itemEntry;
    int group;
    float dropRate;
    int mincountOrRef;
    int maxcount;
    int conditionid;
};

class loot_group_resource_storage
{
private:
    QSqlDatabase * m_db;
    resource_vector<loot_entry> data_block;
    std::map<int, std::pair<int,int> > creature_loot_groups;        // The key is loot group id. Value is <index in block, size>
    std::map<int, std::pair<int,int> > disenchant_loot_groups;
    std::map<int, std::pair<int,int> > pickpocketing_loot_groups;
    std::map<int, std::pair<int,int> > skinning_loot_groups;
    std::map<int, std::pair<int,int> > reference_loot_groups;

    int allocate_group(const std::vector<loot_entry> & entries)
    {
        data_block.allocate(entries);
    }
    int insert_group(const std::vector<loot_entry> & entries)
    {
        data_block.insert(entries);
    }
    void remove_group(int index, const int size)
    {
        data_block.remove(index, size);
    }

    enum class loot_type{
        creature,
        disenchant,
        pickpocket,
        skinning,
        reference
    };

    template <loot_type lt>
    struct identity{};


    template <loot_type lt>
    std::map<int, std::pair<int,int> >& get_id_index_map(){ return get_id_index_map(identity<lt>()); }
    template <loot_type lt>
    std::map<int, std::pair<int,int> >& get_id_index_map(identity<lt>){ return creature_loot_groups; }
    std::map<int, std::pair<int,int> >& get_id_index_map(identity<loot_type::creature>){ return creature_loot_groups; }
    std::map<int, std::pair<int,int> >& get_id_index_map(identity<loot_type::disenchant>){ return disenchant_loot_groups; }
    std::map<int, std::pair<int,int> >& get_id_index_map(identity<loot_type::pickpocket>){ return pickpocketing_loot_groups; }
    std::map<int, std::pair<int,int> >& get_id_index_map(identity<loot_type::skinning>){ return skinning_loot_groups; }
    std::map<int, std::pair<int,int> >& get_id_index_map(identity<loot_type::reference>){ return reference_loot_groups; }

    template <loot_type lt>
    QString get_table_spec(){ return get_table_spec(identity<lt>()); }
    template <loot_type lt>
    QString get_table_spec(identity<lt>){ return ""; }
    QString get_table_spec(identity<loot_type::creature>){ return QString("creature_loot_template"); }
    QString get_table_spec(identity<loot_type::disenchant>){ return QString("disenchant_loot_template"); }
    QString get_table_spec(identity<loot_type::pickpocket>){ return QString("pickpocketing_loot_template"); }
    QString get_table_spec(identity<loot_type::skinning>){ return QString("skinning_loot_template"); }
    QString get_table_spec(identity<loot_type::reference>){ return QString("reference_loot_template"); }

    template <loot_type lt>
    void get_loot_group_impl(int id, std::vector<loot_entry> & vec)
    {
        // First check if name exists in cache
        auto it = get_id_index_map<lt>().find(id);
        if(it != get_id_index_map<lt>().end())
        {
            int index = (*it).second.first;
            int size = (*it).second.second;
            data_block.fill(index,size,vec);
        }
        // If not found, then query database for it and put it in the cache, then return the string
        else
        {
            QString queryStr = QString("SELECT item,groupid,ChanceOrQuestChance,mincountOrRef,maxcount,condition_id FROM ") + get_table_spec<lt>() +
                    QString(" WHERE entry = ") + QString::number(id) + QString(";");
            QSqlQuery q = m_db->exec(queryStr);
            vec.clear();
            while(q.next())
            {
                vec.push_back({q.record().value(0).toInt(),
                                        q.record().value(1).toInt(),
                                        q.record().value(2).toFloat(),
                                        q.record().value(3).toInt(),
                                        q.record().value(4).toInt(),
                                        q.record().value(5).toInt()});
            }
            if(vec.size() != 0)
            {
                int index = data_block.insert(vec);
                get_id_index_map<lt>().insert({id,{index,vec.size()}});
            }
        }
    }

    template <loot_type lt>
    void remove_loot_group_impl(const int id)
    {
        auto it = get_id_index_map<lt>().find(id);
        if(it != get_id_index_map<lt>().end())
        {
            int index = (*it).second.first;
            int size = (*it).second.second;
            data_block.remove(index,size);
            get_id_index_map<lt>().erase(it);
        }
    }

    template <loot_type lt>
    void modify_loot_group_impl(const int id, const std::vector<loot_entry> & new_entries)
    {
        auto it = get_id_index_map<lt>().find(id);
        if(it != get_id_index_map<lt>().end())
        {
            int index = (*it).second.first;
            int size = (*it).second.second;
            data_block.remove(index,size);
            get_id_index_map<lt>().erase(it);
        }
        int newindex = data_block.insert(new_entries);
        get_id_index_map<lt>().insert({id,{newindex,new_entries.size()}});
    }

    template <loot_type lt>
    void add_loot_group_impl(int id, const std::vector<loot_entry> & new_entries)
    {
        // Assumes that id does not already exist in cache, otherwise cache may become erronous
        int index = data_block.allocate(new_entries);
        get_id_index_map<lt>().insert({id,{index,new_entries.size()}});
    }

    template <loot_type lt>
    void init_loot_group_impl()
    {
        get_id_index_map<lt>().clear();

        QString queryStr = QString("SELECT entry,item,groupid,ChanceOrQuestChance,mincountOrRef,maxcount,condition_id FROM ")
                         + get_table_spec<lt>() +
                           QString(" ORDER BY entry;");
        QSqlQuery res = m_db->exec(queryStr);
        int current_entry = -1;
        int newentry;
        std::vector<loot_entry> loot_group;
        while(res.next())
        {
            newentry = res.record().value(0).toInt();
            if(newentry != current_entry)
            {
                if(loot_group.size())
                {
                    int index = data_block.allocate(loot_group);
                    get_id_index_map<lt>().insert({current_entry,{index,loot_group.size()}});
                }
                loot_group.clear();
                current_entry = newentry;
            }
            loot_group.push_back({res.record().value(1).toInt(),
                                 res.record().value(2).toInt(),
                                 res.record().value(3).toFloat(),
                                 res.record().value(4).toInt(),
                                 res.record().value(5).toInt(),
                                 res.record().value(6).toInt()});
        }
        if(loot_group.size())
        {
            int index = data_block.allocate(loot_group);
            get_id_index_map<lt>().insert({current_entry,{index,loot_group.size()}});
        }
    }


    void init_creature_loot_groups(){ init_loot_group_impl<loot_type::creature>(); }
    void init_disenchant_loot_groups(){ init_loot_group_impl<loot_type::disenchant>(); }
    void init_pickpocket_loot_groups(){ init_loot_group_impl<loot_type::pickpocket>(); }
    void init_reference_loot_groups(){ init_loot_group_impl<loot_type::reference>(); }
    void init_skinning_loot_groups(){ init_loot_group_impl<loot_type::skinning>(); }


public:

    loot_group_resource_storage(QSqlDatabase *db);
    ~loot_group_resource_storage() {}

    void initialize(resource_initialization_state &ris)
    {
        init_creature_loot_groups();
        init_disenchant_loot_groups();
        init_pickpocket_loot_groups();
        init_reference_loot_groups();
        init_skinning_loot_groups();
    }

    void get_creature_loot_group(const int id, std::vector<loot_entry> & r)
    {
        get_loot_group_impl<loot_type::creature>(id,r);
    }
    void add_creature_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        add_loot_group_impl<loot_type::creature>(id,entries);
    }
    void remove_creature_loot_group(const int id)
    {
        remove_loot_group_impl<loot_type::creature>(id);
    }
    void modify_creature_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        modify_loot_group_impl<loot_type::creature>(id,entries);
    }
    void creature_loot_groups_size() const { return get_id_index_map(identity<loot_type::creature>{}).size(); }

    void get_disenchant_loot_group(const int id, std::vector<loot_entry> & r)
    {
        get_loot_group_impl<loot_type::disenchant>(id,r);
    }
    void add_disenchant_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        add_loot_group_impl<loot_type::disenchant>(id,entries);
    }
    void remove_disenchant_loot_group(const int id)
    {
        remove_loot_group_impl<loot_type::disenchant>(id);
    }
    void modify_disenchant_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        modify_loot_group_impl<loot_type::disenchant>(id,entries);
    }
    void disenchant_loot_groups_size() const { return get_id_index_map(identity<loot_type::disenchant>{}).size(); }

    void get_pickpocket_loot_group(const int id, std::vector<loot_entry> & r)
    {
        get_loot_group_impl<loot_type::pickpocket>(id,r);
    }
    void add_pickpocket_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        add_loot_group_impl<loot_type::pickpocket>(id,entries);
    }
    void remove_pickpocket_loot_group(const int id)
    {
        remove_loot_group_impl<loot_type::pickpocket>(id);
    }
    void modify_pickpocket_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        modify_loot_group_impl<loot_type::pickpocket>(id,entries);
    }
    void pickpocket_loot_groups_size() const { return get_id_index_map(identity<loot_type::pickpocket>{}).size(); }

    void get_reference_loot_group(const int id, std::vector<loot_entry> & r)
    {
        get_loot_group_impl<loot_type::reference>(id,r);
    }
    void add_reference_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        add_loot_group_impl<loot_type::reference>(id,entries);
    }
    void remove_reference_loot_group(const int id)
    {
        remove_loot_group_impl<loot_type::reference>(id);
    }
    void modify_reference_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        modify_loot_group_impl<loot_type::reference>(id,entries);
    }
    void reference_loot_groups_size() const { return get_id_index_map(identity<loot_type::reference>{}).size(); }

    void get_skinning_loot_group(const int id, std::vector<loot_entry> & r)
    {
        get_loot_group_impl<loot_type::skinning>(id,r);
    }
    void add_skinning_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        add_loot_group_impl<loot_type::skinning>(id,entries);
    }
    void remove_skinninge_loot_group(const int id)
    {
        remove_loot_group_impl<loot_type::skinning>(id);
    }
    void modify_skinning_loot_group(const int id, const std::vector<loot_entry> & entries)
    {
        modify_loot_group_impl<loot_type::skinning>(id,entries);
    }
    void skinning_loot_groups_size() const { return get_id_index_map(identity<loot_type::skinning>{}).size(); }
};

class loot_source_storage
{
private:
    resource_vector<int> data_block;
    std::map<int, std::pair<int,int> > loot_source_groups;

public:
};

class books_resource_storage
{
private:
    resource_vector<int> data_block;
    std::map<int,std::pair<int,int>> books;
    std::vector<int> keys; // redundancy
    int highest_book_id;
public:
    books_resource_storage() :
        data_block(3000),
        books(std::map<int,std::pair<int,int>>{}),
        highest_book_id(0)
    {
    }
    ~books_resource_storage(){}

    void get_book(int id, std::vector<int> & v) const
    {
        auto it = books.find(id);
        if(it != books.end())
        {
            int index = (*it).second.first;
            int size = (*it).second.second;
            data_block.fill(index,size,v);
        }
    }

    int get_index(int id) const
    {
        for(int i = 0; i < keys.size(); ++i)
        {
            if(keys[i] == id)
                return i;
        }
        return -1;
    }

    int first_page_index(int pageId, bool & found) const
    {
        auto it = books.begin();
        int i = 0;
        while(it != books.end())
        {
            auto firstPage = data_block.at((*it).second.first);
            if(firstPage == pageId)
            {
                found = true;
                return i;
            }
            ++i;
            ++it;
        }
        found = false;
        return 0;
    }

    int get_book_at(int index) const
    {
        if(index < size())
            return keys[index];
        else
            return 0;
    }

    int get_page(int book_id, int page_number) const
    {
        auto it = books.find(book_id);
        if(it != books.end())
        {
            int index = it->second.first;
            int size = it->second.second;

            if(page_number < size)
            {
                return data_block.at(index+page_number);
            }
        }
        return 0;
    }

    int number_of_pages(int book_id) const
    {
        auto it = books.find(book_id);
        if(it != books.end())
        {
            return it->second.second;
        }
        else
        {
            return 0;
        }
    }

    int add_book(const std::vector<int> & v)
    {
        int id = highest_book_id+1;
        highest_book_id++;
        int index = data_block.allocate(v);
        books.insert({id,{index,v.size()}});
        keys.push_back(id);
        return id;
    }

    void remove_book(int id)
    {
        auto it = books.find(id);
        if(it != books.end())
        {
            int index = (*it).second.first;
            int size = (*it).second.second;
            data_block.remove(index,size);
            books.erase(it);
            keys.erase(std::find(keys.begin(),keys.end(),id));
        }
    }

    int modify_book(int id, const std::vector<int> & v)
    {
        auto it = books.find(id);
        if(it != books.end())
        {
            int i = (*it).second.first;
            int s = (*it).second.second;
            data_block.remove(i,s);
            books.erase(it);
        }
        int index = data_block.insert(v);
        books.insert({id,{index,v.size()}});
        return id;
    }

    void restore_book(int bookId, const std::vector<int> & v)
    {
        // Only perform this is the bookId is not in use
        if(books.find(bookId) == books.end())
        {
            highest_book_id++;
            int index = data_block.allocate(v);
            books.insert({bookId,{index,v.size()}});
            keys.push_back(bookId);
        }
    }

    int size() const
    {
        return books.size();
    }

    void initialize(QSqlDatabase & db, resource_initialization_state &ris)
    {
        std::map<int,int> data;
        std::set<int> nexts;
        std::set<int> entries;
        std::set<int> firstPages;
        QSqlQuery q = db.exec("SELECT entry,next_page FROM page_text;");
        while(q.next())
        {
            int entry = q.record().value(0).toInt();
            int next = q.record().value(1).toInt();
            nexts.insert(next);
            entries.insert(entry);
            data.insert({entry,next});
        }
        std::set_difference(entries.begin(),entries.end(),nexts.begin(),nexts.end(),
                            std::inserter(firstPages,firstPages.end()));

        std::vector<int> book;
        for(int i : firstPages)
        {
            int nextPage = i;
            while(nextPage != 0)
            {
                auto it = data.find(nextPage);
                if(it == data.end())
                    break; // this means that the data is in a bad form (reference to a next page that does not exist)
                int page = (*it).first;
                nextPage = (*it).second;
                book.push_back(page);
            }
            add_book(book);
            book.clear();
        }
    }
};

#endif // RESOURCE_STORAGE_H
