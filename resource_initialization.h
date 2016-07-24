#ifndef RESOURCE_INITIALIZATION
#define RESOURCE_INITIALIZATION

#include <QString>

struct resource_initialization_state
{
    bool        initialization_complete;
    QString     resource_name;
    int         resource_size;
    int         resource_progress;
    bool        has_part;
    QString     resource_part_name;
    int         resource_part_size;
    int         resource_part_progress;
};


#endif // RESOURCE_INITIALIZATION

