#ifndef DBCPARSE_H
#define DBCPARSE_H

#include <cstdio>
#include <cstdint>
#include <string>
#include <fstream>

#include "dbcfile.h"

template <typename RECORD>
class dbc
{
private:
    dbcheader header;
    std::vector<RECORD>   table;
    bool                  is_init;
    std::vector<char>     string_block;
public:
    dbc(std::string s)
        : table()
    {
        std::ifstream file(s, std::ios::in|std::ios::binary|std::ios::ate);
        std::streampos size;
        char * memblock;
        table.clear();
        string_block.clear();
        if((is_init = file.is_open()))
        {
            size = file.tellg();
            memblock = new char [size];
            file.seekg(0, std::ios::beg);
            file.read(memblock,size);
            file.close();
            // Load header first
            header.wdbc = *reinterpret_cast<uint32_t*>(memblock);
            memblock+=4;
            header.record_count = *reinterpret_cast<uint32_t*>(memblock);
            memblock+=4;
            header.field_count = *reinterpret_cast<uint32_t*>(memblock);
            memblock+=4;
            header.record_size = *reinterpret_cast<uint32_t*>(memblock);
            memblock+=4;
            header.string_block_size = *reinterpret_cast<uint32_t*>(memblock);
            memblock+=4;
            table.reserve(header.record_count);
            string_block.reserve(header.string_block_size);
            for(uint32_t i = 0; i < header.record_count; i++)
            {
                table.push_back(RECORD(memblock));
                memblock += RECORD::record_size;
            }
            for(uint32_t i = 0; i < header.string_block_size; i++)
            {
                string_block.push_back(*memblock);
                memblock++;
            }
        }
    }
    ~dbc(){}
    uint32_t             record_count() { return header.record_count; }
    std::vector<RECORD> &get_table()    { return table; }
    char *               get_strings()  { return string_block.data(); }
    dbcheader &          get_header()   { return header; }
    bool                 is_valid()     { return is_init; }


    void printstrings()
    {
        printf("Header.wdbc = %s\n", reinterpret_cast<char*>(&header.wdbc));
        printf("Header.record_count = %d\n", header.record_count);
        printf("Header.field_count = %d\n", header.field_count);
        printf("Header.record_size = %d\n", header.record_size);
        printf("Header.string_block_size = %d\n", header.string_block_size);
        for(int i = 0; i < header.string_block_size; i++)
        {
            printf("%c",string_block[i]);
        }
        printf("\n");
    }
};


template <fieldtype FT>
struct retrievefield;
template <>
struct retrievefield<fieldtype::INTPTR>
{
    typedef fieldreturntype<fieldtype::INTPTR>::type return_type;
    static fieldreturntype<fieldtype::INTPTR>::type
    field(uint32_t * data, char *)
    {
        return reinterpret_cast<int*>(data);
    }
};
template <>
struct retrievefield<fieldtype::UINT>
{
    typedef fieldreturntype<fieldtype::UINT>::type return_type;
    static fieldreturntype<fieldtype::UINT>::type
    field(uint32_t * data, char *)
    {
        return *data;
    }
};
template <>
struct retrievefield<fieldtype::INT>
{
    typedef fieldreturntype<fieldtype::INT>::type return_type;
    static fieldreturntype<fieldtype::INT>::type
    field(uint32_t * data, char *)
    {
        return *reinterpret_cast<int32_t*>(data);
    }
};
template <>
struct retrievefield<fieldtype::FLOAT>
{
    typedef fieldreturntype<fieldtype::FLOAT>::type return_type;
    static fieldreturntype<fieldtype::FLOAT>::type
    field(uint32_t * data, char *)
    {
        return *reinterpret_cast<float*>(data);
    }
};
template <>
struct retrievefield<fieldtype::STRING>
{
    typedef fieldreturntype<fieldtype::STRING>::type return_type;
    static fieldreturntype<fieldtype::STRING>::type
    field(uint32_t * data, char * string_block)
    {
        return string_block + *data;
    }
};
template <>
struct retrievefield<fieldtype::LOCALE>
{
    typedef fieldreturntype<fieldtype::LOCALE>::type return_type;
    static fieldreturntype<fieldtype::LOCALE>::type
    field(uint32_t * data, char * string_block)
    {
        return string_block + *data;
    }
};

template <fieldtype FT>
struct type_comparator
{
    static bool is_equal(typename fieldreturntype<FT>::type r1, typename fieldreturntype<FT>::type r2)
    {
        return r1 == r2;
    }
    static bool is_greater_than(typename fieldreturntype<FT>::type r1, typename fieldreturntype<FT>::type r2)
    {
        return r1 > r2;
    }
    static bool is_less_than(typename fieldreturntype<FT>::type r1, typename fieldreturntype<FT>::type r2)
    {
        return r1 < r2;
    }
};
template <>
struct type_comparator<fieldtype::STRING>
{
    static bool is_equal(char * r1, char * r2)
    {
        return strcmp(r1,r2) == 0;
    }
    static bool is_greater_than(char * r1, char * r2)
    {
        return strcmp(r1,r2) >= 0;
    }
    static bool is_less_than(char * r1, char * r2)
    {
        return strcmp(r1,r2) <= 0;
    }
};
template <>
struct type_comparator<fieldtype::LOCALE>
{
    static bool is_equal(char * r1, char * r2)
    {
        return strcmp(r1,r2) == 0;
    }
    static bool is_greater_than(char * r1, char * r2)
    {
        return strcmp(r1,r2) >= 0;
    }
    static bool is_less_than(char * r1, char * r2)
    {
        return strcmp(r1,r2) <= 0;
    }
};

template <typename DBCS>
struct getrecordtype_t;
template <recordid T, typename ... FIELDS>
struct getrecordtype_t<dbc<dbcrecord<T,FIELDS...>>>
{
    typedef integral_type<static_cast<size_t>(T)> type;
    static const recordid rtype = T;
};

template <typename C, typename R>
struct is_record_of_id;
template <size_t C, recordid ID, typename ... FS>
struct is_record_of_id<integral_type<C>, dbc<dbcrecord<ID,FS...>>>
{
    enum { value = types_equal<integral_type<static_cast<size_t>(ID)>,integral_type<C>>::value };
};

template <typename DBC>
struct get_dbc_filename;
template <recordid ID, typename ... FIELDS>
struct get_dbc_filename<dbc<dbcrecord<ID,FIELDS...>>>
{
    static const char * name(){ return get_dbc_filename_<ID>::name(); }
};

template <typename DBCS>
struct dbcs_map;
template <typename ... DBCS>
struct dbcs_map<tuple_t<DBCS...>> : public inherit_all<tuple_t<DBCS...>>
{
    // Ensure that all DBCS are unique
    /* Makes compiler run out of memory, so disable this for now
     * static_assert(count_tuple<
                  typename make_set<
                      typename accumulate_types<getrecordtype_t, tuple_t<DBCS...>>::type
                  >::type
                >::value == sizeof...(DBCS), "Provided records are not distinct!\n");*/
    template <recordid RID>
    struct get_dbc
    {
        typedef find_first_match<is_record_of_id,integral_type<static_cast<size_t>(RID)>,tuple_t<DBCS...>> t_;
        typedef typename t_::type type;
        enum { index = t_::index };
        enum { found = t_::found };
    };

    dbcs_map(std::string path) :
        inherit_all<tuple_t<DBCS...>>(std::forward<DBCS>(DBCS(path + std::string(get_dbc_filename<DBCS>::name())))...)
    {
    }
};

template <size_t I, typename DBC>
struct getfieldtype_t;
template <size_t I, typename RECORD>
struct getfieldtype_t<I, dbc<RECORD>>
{
    static const fieldtype type = RECORD::template field<I>::type;
};

/*
 * This class wraps up all dbc-files together
 *
 * The entities are interfaces to these files;
 *
 *  Example: A zone has an id which is its db-reference,
 *           however, its "name" or "label" might be described
 *           in another dbc file. Then, an entity will describe
 *           this connection.
 */
template <typename ENTITY, typename DBCS>
class dbcentityview;
template <typename ENTITY, typename ... DBCS>
class dbcentityview<ENTITY, tuple_t<DBCS...>> : public dbcs_map<tuple_t<DBCS...>>
{
    /* A DBC is a dbc<RECORD>
     *      where RECORD is dbcrecord<recordid, FIELDS...>
     */
private:
    /* With an entity you can do 2 things; get its label and get its database id */
    typedef typename dbcs_map<tuple_t<DBCS...>>::template get_dbc<ENTITY::label_recordid> label;
    typedef typename label::type labeldbc;
    enum { labelIndex = ENTITY::label_index };
    static const fieldtype labeltype = getfieldtype_t<labelIndex,labeldbc>::type;
    typedef typename dbcs_map<tuple_t<DBCS...>>::template get_dbc<ENTITY::ref_recordid> ref;
    typedef typename ref::type refdbc;
    enum { refIndex = ENTITY::ref_index };
    static const fieldtype reftype = getfieldtype_t<refIndex,refdbc>::type;
public:

    typedef typename retrievefield<labeltype>::return_type label_return_type;
    typedef typename retrievefield<reftype  >::return_type ref_return_type;

    size_t            get_size(){ return (*this).template get<label::index>()->get_table().size(); }
    bool              is_valid(){ return (*this).template get<label::index>()->is_valid(); }
    label_return_type get_label(size_t i)
    {
        return retrievefield<labeltype>::field(
                    (*this).template get<label::index>()->get_table().at(i).record.template get<labelIndex>()->data,
                    (*this).template get<label::index>()->get_strings());
    }
    ref_return_type get_ref(size_t i)
    {
        return retrievefield<reftype>::field(
                    (*this).template get<ref::index>()->get_table().at(i).record.template get<refIndex>()->data,
                    (*this).template get<ref::index>()->get_strings());
    }

    size_t          find_ref(ref_return_type r)
    {
        for(size_t a = 0; a < get_size(); a++)
        {
            if(type_comparator<reftype>::is_equal(get_ref(a),r))
            {
                return a;
            }
        }
        return -1;
    }
private:
    size_t binary_search(ref_return_type r, size_t l, size_t u)
    {
        if(type_comparator<reftype>::is_equal(get_ref((l+u)/2),r)){ return (l+u)/2; }
        else if((u-l) <= 1)
        {
            if(type_comparator<reftype>::is_equal(get_ref(u),r)){ return u; }
            else if(type_comparator<reftype>::is_equal(get_ref(l),r)){ return l; }
            else{ return -1; }
        }
        else if(type_comparator<reftype>::is_greater_than(get_ref((l+u)/2),r))
        {
            return binary_search(r,l,(l+u)/2);
        }
        else if(type_comparator<reftype>::is_less_than(get_ref((l+u)/2),r))
        {
            return binary_search(r,(l+u)/2,u);
        }
        return -1;
    }

public:
    size_t find_ref_binary(ref_return_type r)
    {
        return binary_search(r, 0, get_size()-1);
    }

};























#endif // DBCPARSE_H
