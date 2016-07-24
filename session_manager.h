#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <string>
#include <ctime>
#include <QSqlDatabase>
#include "database/dbinterface.h"
#include "database/table.h"
#include "database/creature_template.h"
#include "database/page_text.h"
#include <QDate>

class session_manager
{
    std::time_t session_begin;
    std::time_t session_last_update;
    QDateTime   session_begin_ph;
    QDateTime   session_last_update_ph;
    std::string session_name;
    std::string previous_session_name;
    bool session_name_set_explicitly;
    bool is_modified;

    db_interface<DB_LIBRARY::QT> dbi;
    dbutil::table<page_text> page_text_table;
    void page_text_table_init();

public:
    //dbutil::table<creature_template_tbc> creature_template;

    session_manager(QSqlDatabase &db,std::string previous_session) :
        session_begin(std::time(nullptr)),
        session_begin_ph(QDateTime::currentDateTime()),
        previous_session_name(previous_session),
        session_name_set_explicitly(false),
        is_modified(false),
        dbi(db)
    {
    }

    ~session_manager(){}

    void update_session_time()
    {
        session_last_update = std::time(nullptr);
        session_last_update_ph = QDateTime::currentDateTime();
    }

    void set_session_name_default();
    void set_session_name(std::string nm);

    std::string get_session_name()
    {
        return session_name;
    }

    std::string get_patch_header();
    std::pair<std::string,std::string> get_patches() const;

    bool save_patches_to_file();

    // page_text
    void page_text_insert(int entry, std::string text, int next_page);
    void page_text_delete(int entry);
    int page_text_free_page();
};

#endif // SESSION_MANAGER_H
