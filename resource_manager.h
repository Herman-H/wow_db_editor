#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <QString>
#include <QSqlDatabase>
#include "config.h"
#include "resource_initialization.h"
#include "session_manager.h"

class strings_resource_storage;
class books_resource_storage;
class loot_group_resource_storage;


class resource_manager
{
    QSqlDatabase &db;
    strings_resource_storage        * srs;
    books_resource_storage          * brs;
    loot_group_resource_storage     * lgrs;

    resource_initialization_state ris;

    session_manager sess_mgr;

public:
    resource_manager(QSqlDatabase &db, const config &cfg);
    ~resource_manager();

    resource_initialization_state get_initialization_state() const
    {
        return ris;
    }

    const session_manager * get_session_manager() const { return &sess_mgr; }

    // Strings
    strings_resource_storage * get_strings() const;

    // Books
    books_resource_storage * get_books() const;
    void swap_pages(int bookid, int page); // returns the new firstPage
    int add_book(QString text, int page, int nextPage); // returns bookId
    int insert_page(int bookid, int page, QString text);
    void setText(int bookid, int page, QString text);
    void remove_book(int bookid);
    void remove_page(int bookid, int page);
    void restore_book(int bookid, QString text, int page, int nextPage);
    QString book_title(int id, int page, int maxlen, bool showid) const;
    QString book_title_(int pageid, int maxlen, bool showid) const;
    QString page_text_summary(int id, int page, int maxlen) const;

    // Loot
    loot_group_resource_storage * get_loot_group() const;
};

#endif // RESOURCE_MANAGER_H
