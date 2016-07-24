#include "item_models.h"

// the resources
#include "resource_manager.h"

// individual models
#include "book_selecter_widget.h"
#include "loot_selecter_widget.h"

item_models::item_models()
{

}

const session_manager * item_models::get_session_manager() const
{
    return res->get_session_manager();
}

void item_models::initialize(QSqlDatabase &db, const config &cfg)
{
    // The resource data
    res                         = new resource_manager{db,cfg};
    // The individual models based on the resource data
    book_model                  = new book_item_model{res};
    creature_loot_group_model   = new creature_loot_group_item_model{};
}
