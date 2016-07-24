#ifndef CACHETYPES_H
#define CACHETYPES_H

/*
 *  A resource is either a db or dbc dependant. db resources can depend on dbc data. This relationship is implicit in
 *  the structure of class dbcache.
 */

enum class db_resource_type
{
    item_extended_cost,
    page_text,
    creature_loot,
    disenchant_loot,
    item_random_property,
    item_random_suffix,
    lock_type,
    gossip,
    condition
};

enum class dbc_resource_type
{
    creature_family,
    creature_type,
    skillline,
    faction,
    reputation_faction,
    item_class,
    item_subclass,
    map,
    area,
    model,
    spell,
    standing_animation,
    pet_spell,
    item_model,
    item_set,
    title,
    item_enchant,
    language,
    totem_category,
    gem
};

#endif // CACHETYPES_H
