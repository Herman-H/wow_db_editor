#ifndef ITEM_MODELS_H
#define ITEM_MODELS_H

#include <QObject>
#include <QSqlDatabase>
#include "config.h"

// The resource manager
class resource_manager;

// The session manager
class session_manager;

// Item models
class book_item_model;
class creature_loot_group_item_model;


/*
 * Item models are the widget data that can be viewed/selected and edited. It is the "top layer" of the
 * resource management system.
 *
 * Underlying this class is the resource manager which has to be initialized first. Item models depends on
 * the resources, which can be said to be the "raw data" in memory.
 *
 * The only way to manipulate the resource and persistent data, are through the item models. Which is why item models
 * is the top layer.
 *
 * Below the resource management is the persistance layer, or session manager. The actual difference
 * the session has caused to the persistant data, is compiled to an SQL patch, as well as another SQL backpatch to
 * undo the session. This means that all data-manipulation on the database go through this layer. This is the
 * responsibility of the resource manager.
 *
 * In summary, any data-manipulation changes both the database, handled by the session manager, and the loaded
 * resources in memory, handled by the resource manager.
 */

class item_models
{
private:

    resource_manager * res;
    void init_resource_manager(QSqlDatabase &db);

    book_item_model * book_model;
    creature_loot_group_item_model * creature_loot_group_model;
public:
    item_models();

    void initialize(QSqlDatabase &db, const config & cfg);

    const session_manager * get_session_manager() const;

    resource_manager * get_resources()
    {
        return res;
    }

    book_item_model * get_book_model()
    {
        return book_model;
    }

    creature_loot_group_item_model * get_creature_loot_group_model()
    {
        return creature_loot_group_model;
    }
};

#endif // ITEM_MODELS_H
