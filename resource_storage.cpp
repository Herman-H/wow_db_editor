#include "resource_storage.h"

strings_resource_storage::strings_resource_storage(QSqlDatabase *db) :
    m_db(db),
    creature_template_names(),
    item_template_names(),
    strings_block(),
    unused_fragments()
{
    strings_block.reserve(400000); // assume at least 30000 different names and medium length 12 chars/name, +1 for null termination
    strings_block.push_back('\0');
    strings_block.push_back('<');
    strings_block.push_back('N');
    strings_block.push_back('o');
    strings_block.push_back('t');
    strings_block.push_back(' ');
    strings_block.push_back('F');
    strings_block.push_back('o');
    strings_block.push_back('u');
    strings_block.push_back('n');
    strings_block.push_back('d');
    strings_block.push_back('>');
    strings_block.push_back('\0');
}

strings_resource_storage::~strings_resource_storage()
{

}

int strings_resource_storage::allocate_string(const char *str, int & size)
{
    // Allocation is basically appending the nameblock with the new name
    // Input must be a nullterminated string
    // Writes the size of the string to the variable 'size' because no need to know size here
    int index = strings_block.size();
    const char * c = str;
    size = 0;
    while(*c != '\0')
    {
        strings_block.push_back(*c);
        ++c;
        ++size;
    }
    strings_block.push_back('\0');
    return index;
}

int strings_resource_storage::insert_string(const char *str, int size)
{
    int index = 0;
    int worstfitindex = 0;
    int worstfitsize = 0;
    // Allocation now searches all fragments for a perfect fit. If not found, use worst fit
    for(auto it = unused_fragments.begin();
             it != unused_fragments.end();
             ++it)
    {
        std::pair<const int,int> & p = *it;
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
        const char *c = str;
        for(int i = index; i < index+size; ++i)
        {
            strings_block[i] = *c;
            ++c;
        }
        strings_block[index+size] = '\0';
        unused_fragments.erase(index);
    }
    else if(worstfitindex != 0)
    {
        // Allocate to worst fit at index
        const char *c = str;
        for(int i = worstfitindex; i < worstfitindex+size; ++i)
        {
            strings_block[i] = *c;
            ++c;
        }
        strings_block[worstfitindex+size] = '\0';

        // Remove the fragment
        unused_fragments.erase(worstfitindex);

        // Insert a new fragment that is the rest at worstfitindex+size+1,
        unused_fragments.insert({worstfitindex+size+1,worstfitsize-size-1});
        index = worstfitindex;
    }
    else // No suitable fragment found, then allocate on the end, like allocate_name() function
    {
        index = strings_block.size();
        const char *c = str;
        for(int i = 0; i < size; ++i)
        {
            strings_block.push_back(*c);
            ++c;
        }
        strings_block.push_back('\0');
    }
    return index;
}

void strings_resource_storage::remove_string(int index)
{
    // It is assumed that block_fragmentation_tracker does not have index as an entry already
    // Find size of removed block by searching for next null
    int c = index;
    int size = 0;
    while(strings_block[c] != '\0')
    {
        ++c;
        ++size;
    }
    // If removed string is the last one in the block simply decrement the size of the block
    if(c+1 >= strings_block.size())
    {
        for(int i = 0; i <= size; ++i)
        {
            strings_block.pop_back();
        }
        return;
    }
    // Merge fragments forward from index?
    auto it1 = unused_fragments.find(c+1);
    if(it1 != unused_fragments.end()) // Then we shall merge forwards
    {
        size += (*it1).second+1;
        unused_fragments.erase(it1);
    }
    auto it2 = unused_fragments.insert({index,size}).first;
    // Merge fragments backward from index?
    if(it2 != unused_fragments.begin()) // To merge backwards, there has to exist a previous fragment
    {
        --it2;
        if((*it2).first + (*it2).second + 1 == index)
        {
            size += (*it2).second+1;
            unused_fragments.erase(index);
            index = (*it2).first;
            unused_fragments.erase(index);
            unused_fragments.insert({index,size});
        }
    }
}

void strings_resource_storage::init_creature_names()
{
    creature_template_names.clear();

    QString queryStr = "SELECT Entry,Name FROM creature_template;";
    QSqlQuery res = m_db->exec(queryStr);
    while(res.next())
    {
        int a;
        int index = allocate_string(res.record().value(1).toByteArray().data(),a);
        creature_template_names.insert({res.value(0).toInt(),index});
    }
}

void strings_resource_storage::init_item_names()
{
    item_template_names.clear();

    QString queryStr = "SELECT entry,name FROM item_template;";
    QSqlQuery res = m_db->exec(queryStr);
    while(res.next())
    {
        int a;
        int index = allocate_string(res.record().value(1).toByteArray().data(),a);
        item_template_names.insert({res.value(0).toInt(),index});
    }
}

void strings_resource_storage::init_page_texts()
{
    page_texts.clear();

    QString queryStr = "SELECT entry,text FROM page_text;";
    QSqlQuery res = m_db->exec(queryStr);
    while(res.next())
    {
        int a;
        int index = allocate_string(res.record().value(1).toByteArray().data(),a);
        item_template_names.insert({res.value(0).toInt(),index});
    }
}

void strings_resource_storage::initialize(resource_initialization_state &ris)
{
    unused_fragments.clear();
    strings_block.clear();

    init_creature_names();
    init_item_names();
    init_page_texts();
}


loot_group_resource_storage::loot_group_resource_storage(QSqlDatabase *db) :
    m_db(db),
    data_block(300000),
    creature_loot_groups(),
    disenchant_loot_groups(),
    pickpocketing_loot_groups(),
    skinning_loot_groups(),
    reference_loot_groups()
{
}






















