#include "resource_manager.h"
#include "resource_storage.h"
#include <set>

resource_manager::resource_manager(QSqlDatabase &db, const config &cfg) :
    db(db),
    sess_mgr(db,cfg.get_previous_session().toStdString())
{
    ris.initialization_complete = false;
    ris.resource_name = QString{};
    ris.resource_size = 1;
    ris.resource_progress = 1;
    ris.has_part = false;
    ris.resource_part_name = QString{};
    ris.resource_part_size = 1;
    ris.resource_part_progress = 1;
    srs = new strings_resource_storage{&db};
    srs->initialize(ris);
    brs = new books_resource_storage{};
    brs->initialize(db,ris);
    lgrs = new loot_group_resource_storage{&db};
    lgrs->initialize(ris);
}

resource_manager::~resource_manager()
{
    delete srs;
    delete brs;
}

strings_resource_storage * resource_manager::get_strings() const
{
    return srs;
}

books_resource_storage * resource_manager::get_books() const
{
    return brs;
}

void resource_manager::swap_pages(int id, int page)
{
    std::vector<int> book;
    brs->get_book(id,book);

    if(page == book.size() - 1)
    {
        return;
    }

    if(page != 0)
    {
        sess_mgr.page_text_insert(book[page-1],std::string{srs->get_page_text(book[page-1])},book[page+1]);
    }

    sess_mgr.page_text_insert(book[page+1],std::string{srs->get_page_text(book[page+1])},book[page]);
    sess_mgr.page_text_insert(book[page],std::string{srs->get_page_text(book[page])},(page < book.size()-2 ? book[page+2] : 0));
    std::iter_swap(book.begin()+page, book.begin()+page+1);
    brs->modify_book(id,book);
}

int resource_manager::add_book(QString text, int page, int nextPage)
{
    if(page == 0)
    {
        page = sess_mgr.page_text_free_page();
    }

    sess_mgr.page_text_insert(page,text.toStdString(),nextPage);
    srs->add_page_text(page, text.toStdString().c_str());
    int bookId = brs->add_book(std::vector<int>{page});

    return bookId;
}

void resource_manager::restore_book(int bookid, QString text, int page, int nextPage)
{
    sess_mgr.page_text_insert(page,text.toStdString(),nextPage);
    srs->add_page_text(page, text.toStdString().c_str());
    brs->restore_book(bookid,std::vector<int>{page});
}

QString resource_manager::book_title_(int pageId, int maxlen, bool showid) const
{
    QString r{};
    if(showid)
    {
        r.append("(");
        r.append(QString::number(pageId));
        r.append(")");
        r.append(QString{" "}.repeated(8-r.length()));
    }
    int s = r.length();
    int dif = maxlen-s;

    if(dif > 0)
    {
        QString pagetext{srs->get_page_text(pageId)};
        if(dif < pagetext.length())
        {
            r.append(pagetext.left(dif-3));
            r.append("...");
        }
        else
        {
            r.append(pagetext.left(dif));
        }
    }
    return r.split('\n')[0];
}

QString resource_manager::book_title(int id, int page, int maxlen, bool showid) const
{
    return book_title_(brs->get_page(id, page),maxlen,showid);
}

QString resource_manager::page_text_summary(int id, int page, int maxlen) const
{
    return book_title(id,page,maxlen,false);
}

void resource_manager::remove_page(int bookid, int page)
{
    std::vector<int> b;
    brs->get_book(bookid,b);

    if(page > 0)
    {
        std::string str{srs->get_page_text(b[page-1])};
        int nextPage = page+1 < b.size() ? b[page+1] : 0;
        sess_mgr.page_text_delete(page-1);
        sess_mgr.page_text_insert(page-1, str, nextPage);
    }

    srs->remove_page_text(b[page]);
    sess_mgr.page_text_delete(b[page]);
    b.erase(b.begin()+page);
    brs->modify_book(bookid,b);
}

int resource_manager::insert_page(int bookid, int page, QString text)
{
    int pageId,nextPageId;
    std::vector<int> b;
    brs->get_book(bookid,b);
    nextPageId = brs->get_page(bookid,page);
    pageId = sess_mgr.page_text_free_page();
    srs->add_page_text(pageId,text.toLocal8Bit().data());
    sess_mgr.page_text_insert(pageId,text.toStdString(),nextPageId);
    page = (page > b.size() ? b.size() : page);
    if(page > 0)
    {
        // Fix link from previous page to this one
        sess_mgr.page_text_delete(b[page-1]);
        sess_mgr.page_text_insert(b[page-1],{srs->get_page_text(b[page-1])},pageId);
    }
    b.insert(b.begin()+page,pageId);
    brs->modify_book(bookid,b);

    return pageId;
}

void resource_manager::remove_book(int id)
{
    std::vector<int> b;
    brs->get_book(id,b);

    for(int i = 0; i < b.size(); ++i)
    {
        srs->remove_page_text(b[i]);
        sess_mgr.page_text_delete(b[i]);
    }
    brs->remove_book(id);
}

void resource_manager::setText(int bookid, int page, QString text)
{
    int page_id = brs->get_page(bookid,page);
    int next_page_id = brs->get_page(bookid,page+1);
    srs->change_page_text(page_id,text.toLocal8Bit().data());
    sess_mgr.page_text_delete(page_id);
    sess_mgr.page_text_insert(page_id,text.toStdString(),next_page_id);
}

loot_group_resource_storage * resource_manager::get_loot_group()
{
    return lgrs;
}
