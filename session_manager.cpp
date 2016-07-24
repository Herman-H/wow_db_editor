#include "session_manager.h"
#include <iomanip>

void session_manager::set_session_name_default()
{
    if(!session_name_set_explicitly)
    {
        session_name = "session_";
        //session_name.append(std::put_time(std::localtime(&session_begin),"%y%m%d%H%M%S"));
        session_name.append(session_begin_ph.toString(QString{"yy/MM/dd hh:mm:ss"}).toStdString());
        session_name.append("_");
        //session_name.append(std::put_time(std::localtime(&session_last_update),"%y%m%d%H%M%S"));
        session_name.append(session_last_update_ph.toString(QString{"yy/MM/dd hh:mm:ss"}).toStdString());
    }
}

void session_manager::set_session_name(std::string nm)
{
    session_name = nm;
    session_name_set_explicitly = true;
}

std::string session_manager::get_patch_header()
{
    std::string s{""};
    if(previous_session_name.length())
    {
        s.append("/* Previous Session: ");
        s.append(previous_session_name);
        s.append(" */\n");
    }
    s.append("/* Session initiated ");
    //s.append(std::put_time(std::localtime(&session_begin),"%F %T"));
    session_name.append(session_begin_ph.toString(QString{"yy/MM/dd hh:mm:ss"}).toStdString());
    s.append("*/\n");
    s.append("/* Generated ");
    //s.append(std::put_time(std::localtime(&session_last_update),"%F %T"));
    session_name.append(session_last_update_ph.toString(QString{"yy/MM/dd hh:mm:ss"}).toStdString());
    s.append("*/\n");
    return std::string{s};
}


std::pair<std::string,std::string> session_manager::get_patches() const
{
    /* string s is forward patch, string r is rollback patch */
    std::string s{""};
    std::string r{""};

    if(page_text_table.is_modified())
    {
        s.append("/* ");
        s.append(page_text_table.tablename());
        s.append(" */\n");
        s.append(page_text_table.get_table_patch());

        r.append("/* ");
        r.append(page_text_table.tablename());
        r.append(" */\n");
        r.append(page_text_table.get_table_rollback_patch());
    }

    return std::pair<std::string,std::string>{s,r};
}

void session_manager::page_text_table_init()
{
    typedef dbutil::table<page_text> TBL;
    page_text_table.set_default(TBL::field<page_text::field_index::entry>{0},
                                TBL::field<page_text::field_index::text>{std::string{""}},
                                TBL::field<page_text::field_index::next_page>{0});
}

void session_manager::page_text_insert(int entry, std::string text, int next_page)
{
    typedef dbutil::table<page_text> TBL;
    page_text_table.insert_entry(dbi,TBL::field<page_text::field_index::entry>{entry},
                                 TBL::field<page_text::field_index::text>{text},
                                 TBL::field<page_text::field_index::next_page>{next_page});
}

void session_manager::page_text_delete(int entry)
{
    typedef dbutil::table<page_text> TBL;
    page_text_table.delete_entry(dbi, TBL::field<page_text::field_index::entry>{entry});
}

int session_manager::page_text_free_page()
{
    typedef dbutil::table<page_text> TBL;
    return page_text_table.max(dbi, TBL::field<page_text::field_index::entry>{})+1;
}
