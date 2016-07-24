#ifndef PAGE_TEXT
#define PAGE_TEXT

#include <array>
#include "dbtmp.h"

template <size_t N>
using dbstring = std::array<char,N>;

/* Description of page_text table  */
struct page_text
{
    enum class field_index
    {
        entry,
        text,
        next_page,

        SIZE
    };

    typedef dbtmp::tuple_v<static_cast<size_t>(field_index::entry)> primary_key_fields;

    typedef dbtmp::tuple_t<int,std::string,int> field_types;

    static constexpr const dbtmp::sized_cstring field_name[] = { {"entry"},{"text"},{"next_page"} };

    static constexpr const dbtmp::sized_cstring table_name = {"page_text"};

};

#endif // PAGE_TEXT

