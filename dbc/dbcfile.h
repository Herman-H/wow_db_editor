#ifndef DBCFILE_H
#define DBCFILE_H

#include <string>
#include <vector>
#include <inttypes.h>
#include <cstdint>
#include <cstring>

#include "dbctmp.h"

enum class fieldtype
{
    STRING,
    INT,
    UINT,
    FLOAT,
    LOCALE,
    INTPTR
};

template <fieldtype F>
struct fieldreturntype
{
    typedef uint32_t type;
};

template <>
struct fieldreturntype<fieldtype::STRING>
{
    typedef char * type;
};

template <>
struct fieldreturntype<fieldtype::LOCALE>
{
    typedef char * type;
};

template <>
struct fieldreturntype<fieldtype::INT>
{
    typedef int32_t type;
};

template <>
struct fieldreturntype<fieldtype::FLOAT>
{
    typedef float type;
};

template <>
struct fieldreturntype<fieldtype::INTPTR>
{
    typedef int* type;
};

enum class recordid
{
    creature_type,
    creature_family,
    map,
    areatable,
    faction,
    factiontemplate,
    modelinfo,
    modeldata,
    spell,
    spellitemenchant,
    emote,
    creaturespells,
    skillline,
    itemextendedcost,
    item,
    itemset,
    itemdisplay,
    itemclass,
    itemsubclass,
    itemsubclassmask,
    itemrandomproperties,
    itemrandomsuffix,
    gemproperties,
    title,
    language,
    totemcategory,
    lock,
    locktype
};

template <recordid ID>
struct get_dbc_filename_
{
    static const char * name(){ return ""; }
};
template <>
struct get_dbc_filename_<recordid::creature_type>
{
    static const char * name(){ return "CreatureType.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::map>
{
    static const char * name(){ return "Map.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::areatable>
{
    static const char * name(){ return "AreaTable.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::creature_family>
{
    static const char * name(){ return "CreatureFamily.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::faction>
{
    static const char * name(){ return "Faction.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::factiontemplate>
{
    static const char * name(){ return "FactionTemplate.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::modelinfo>
{
    static const char * name(){ return "CreatureDisplayInfo.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::modeldata>
{
    static const char * name(){ return "CreatureModelData.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::spell>
{
    static const char * name(){ return "Spell.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::emote>
{
    static const char * name(){ return "Emotes.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::creaturespells>
{
    static const char * name(){ return "CreatureSpellData.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::skillline>
{
    static const char * name(){ return "SkillLine.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemextendedcost>
{
    static const char * name(){ return "ItemExtendedCost.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::item>
{
    static const char * name(){ return "Item.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemset>
{
    static const char * name(){ return "ItemSet.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemdisplay>
{
    static const char * name(){ return "ItemDisplayInfo.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemclass>
{
    static const char * name(){ return "ItemClass.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemsubclass>
{
    static const char * name(){ return "ItemSubClass.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemsubclassmask>
{
    static const char * name(){ return "ItemSubClassMask.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemrandomproperties>
{
    static const char * name(){ return "ItemRandomProperties.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::itemrandomsuffix>
{
    static const char * name(){ return "ItemRandomSuffix.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::gemproperties>
{
    static const char * name(){ return "GemProperties.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::title>
{
    static const char * name(){ return "CharTitles.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::spellitemenchant>
{
    static const char * name(){ return "SpellItemEnchantment.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::language>
{
    static const char * name(){ return "Languages.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::totemcategory>
{
    static const char * name(){ return "TotemCategory.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::lock>
{
    static const char * name(){ return "Lock.dbc"; }
};
template <>
struct get_dbc_filename_<recordid::locktype>
{
    static const char * name(){ return "LockType.dbc"; }
};

template <size_t BYTES, fieldtype T>
struct dbcfield;

template <typename FIELD>
struct fieldsizepredicate;
template <template <size_t,fieldtype> class F, size_t BYTES, fieldtype T>
struct fieldsizepredicate<F<BYTES,T>>
{
    enum { value = BYTES };
};
template <typename FIELD>
struct getfieldtype;
template <template <size_t,fieldtype> class F, size_t BYTES, fieldtype T>
struct getfieldtype<F<BYTES,T>>
{
    static const fieldtype type = T;
};

template <typename FIELD>
struct singlefield;
template <size_t BYTES, fieldtype T>
struct singlefield<dbcfield<BYTES,T>>
{
    enum { size = BYTES };
    uint32_t data[BYTES/4];
};

template <typename ... FIELDS>
struct fieldstore : inherit_all<tuple_t<singlefield<FIELDS>...>>
{
};

template <recordid ID, typename ... FIELDS>
struct dbcrecord
{
public:
    fieldstore<FIELDS...> record;
private:
    typedef fieldstore<FIELDS...> recordtype;

    template <size_t I, typename ... FS>
    struct parse;
    template <size_t I>
    struct parse<I>
    {
        static void field(char * p, recordtype &r)
        {
            p=p;r=r; // lines should be optimised away, but removes warnings that parameters are not in use
        }
    };
    template <size_t I, typename F, typename ... FS>
    struct parse<I, F, FS...> : parse<I+1, FS...>
    {
        static void field(char * p, recordtype &r)
        {
            memcpy(r.template get<I>()->data, p, r.template get<I>()->size);
            parse<I+1,FS...>::field(p + r.template get<I>()->size, r);
        }
    };

public:
    enum { record_size = accumulate<fieldsizepredicate,tuple_t<FIELDS...>>::value };

    template <size_t S>
    struct field
    {
        static const fieldtype type = getfieldtype<typename return_Nth<S, tuple_t<FIELDS...>>::type>::type;
        enum { offset = accumulate<fieldsizepredicate, typename return_N_first<S,FIELDS...>::type>::value };
    };

    dbcrecord(char * p)
    {
        parse<0, FIELDS...>::field(p,record);
        p+=record_size;
    }
};

template <typename P>
struct indexIsntInRecord;
template <recordid RID, typename ... FIELDS, size_t INDEX>
struct indexIsntInRecord<pair_tv<dbcrecord<RID,FIELDS...>, INDEX>>
{
    enum { value = (INDEX >= sizeof...(FIELDS)) };
};

template <typename PAIR>
struct gettypeoffieldpredicate;

template <recordid RID, typename ... FIELDS, size_t INDEX>
struct gettypeoffieldpredicate<pair_tv<dbcrecord<RID,FIELDS...>, INDEX>>
{
    typedef integral_type<
                static_cast<size_t>(getfieldtype<typename return_Nth<INDEX, tuple_t<FIELDS...>>::type>::type)
                        >
    type;
};

template <typename PAIR>
struct getrecordidpredicate;
template <recordid RID, typename ... FIELDS, size_t INDEX>
struct getrecordidpredicate<pair_tv<dbcrecord<RID,FIELDS...>,INDEX>>
{
    typedef integral_type<static_cast<size_t>(RID)> type;
};

/* Each pair should consist of a dbcrecord and a field index
 * (1) Ensure that index is in range of its paired record
 * (2) Ensure that all indices of the records are of the same fieldtype
 * (3) Ensure that all records have unique IDs */
template <typename ... PAIRS>
struct fields_match
{
    /*(1)*/static_assert(accumulate<indexIsntInRecord, tuple_t<PAIRS...>>::value == 0, "Index is out of range!\n");
    /*(2)*/static_assert(count_tuple<
                    typename make_set<
                        typename accumulate_types<gettypeoffieldpredicate, tuple_t<PAIRS...>>::type
                    >::type
                  >::value == 1, "Types of the provided fields do not match!\n");
    /*(3)*/static_assert(count_tuple<
                  typename make_set<
                      typename accumulate_types<getrecordidpredicate, tuple_t<PAIRS...>>::type
                  >::type
                >::value == sizeof...(PAIRS), "Provided records are not distinct!\n");
};

template <recordid RID, typename ... FIELDS, size_t INDEX>
struct fields_match<pair_tv<dbcrecord<RID, FIELDS...>,INDEX>>
{

};

/* Any dbc entity has a label, of type recordfield */
template <  typename L,
/* and a database id, of type dbcfield, associated with it */
            typename ID,
/* all other entries should hold the dbc id which should all match */
            typename ... MATCHING_PAIRS>
struct dbcentity;

template < recordid LRID, typename ... LFIELDS, size_t LINDEX, size_t LMATCH,
           recordid IDID, typename ... IDFIELDS, size_t IDINDEX, size_t IDMATCH,
           typename ... MATCHING_PAIRS>
struct dbcentity<   triple_tvv<dbcrecord<LRID,LFIELDS...>,LINDEX,LMATCH>,
                    triple_tvv<dbcrecord<IDID,IDFIELDS...>,IDINDEX,IDMATCH>,
                    MATCHING_PAIRS...>
{
    static_assert(LINDEX < sizeof...(LFIELDS), "Label index out of range!\n");
    static_assert(LMATCH < sizeof...(LFIELDS), "Label match index out of range!\n");
    static_assert(IDINDEX < sizeof...(IDFIELDS), "Id index out of range!\n");
    static_assert(IDMATCH < sizeof...(IDFIELDS), "Id match index out of range!\n");
    static const recordid label_recordid = LRID;    // The label's DBC-file
    static const recordid ref_recordid = IDID;      // The reference's DBC-file
    static const size_t label_id = LMATCH;          // The label's ID across all DBC-files
    static const size_t ref_id = IDMATCH;           // The reference's ID across all DBC-files
    enum { label_index = LINDEX };                  // The column of the DBC-file where the label-data is stored
    enum { ref_index = IDINDEX };                   // The column of the DBC-file where the references-data is stored
    typedef fields_match<MATCHING_PAIRS...> matches;
};


struct dbcheader
{
    uint32_t wdbc;
    uint32_t record_count;
    uint32_t field_count;
    uint32_t record_size;
    uint32_t string_block_size;
};

#endif // DBCFILE_H
