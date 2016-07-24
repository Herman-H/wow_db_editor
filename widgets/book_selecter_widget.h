#ifndef BOOK_SELECTER_WIDGET
#define BOOK_SELECTER_WIDGET

class QHBoxLayout;
class QVBoxLayout;
class TextEditAndDisplay;
class QPushButton;

#include <QTreeView>
#include <QAbstractItemModel>
#include "./resource_manager.h"
#include "./resource_storage.h"
#include "selecter_dialog.h"

#include <algorithm>

namespace book_item_modification
{
    enum class type
    {
        addBook,
        deleteBook,
        swapPages,
        insertPage,
        deletePage,
        changeText
    };

    struct action
    {
        action(){}
        ~action(){}
        type        modType;
        int         bookId;
        int         numPages;
        int         page;
        int         pageId;
        int         nextPageId;
        QString     text;
    };

    static action getSwapPagesAction(int bookId, int page)
    {
        action a;
        a.modType = type::swapPages;
        a.bookId = bookId;
        a.page = page;
        return a;
    }

    static action getAddBookAction(int bookId, int numPages)
    {
        action a;
        a.modType = type::addBook;
        a.bookId = bookId;
        a.numPages = numPages;
        return a;
    }

    static action getInsertPageAction(int bookId, int page)
    {
        action a;
        a.modType = type::insertPage;
        a.bookId = bookId;
        a.page = page;
        return a;
    }
}


class book_item_model : public QAbstractItemModel
{
    Q_OBJECT
private:
    resource_manager * res;
    typedef std::pair<int,bool> uniqueIndexType;
    std::set<uniqueIndexType> indexData;

    std::stack<book_item_modification::action> undos;
    std::stack<book_item_modification::action> redos;

    void clearRedos()
    {
        while(redos.size())
            redos.pop();
    }

    void insertParentData(int bookId, bool isParent)
    {
        uniqueIndexType p{bookId,isParent};
        auto it = indexData.find(p);
        if(it == indexData.end())
        {
            indexData.insert(uniqueIndexType{bookId,isParent});
        }
    }

    void removeParentData(int bookId)
    {
        uniqueIndexType p{bookId,true};
        uniqueIndexType c{bookId,false};

        auto itp = indexData.find(p);
        if(itp != indexData.end())
        {
            indexData.erase(itp);
        }
        auto itc = indexData.find(c);
        if(itc != indexData.end())
        {
            indexData.erase(itc);
        }
    }

    uniqueIndexType* getIndexData(int bookId, bool isParent) const
    {
        auto it = indexData.find(uniqueIndexType{bookId,isParent});
        if(it == indexData.end())
        {
            return nullptr;
        }
        else
        {
            return const_cast<uniqueIndexType*>(&(*it));
        }
    }

    bool isParent_(void * internalPtr) const
    {
       return static_cast<uniqueIndexType*>(internalPtr)->second;
    }

    int getId_(void * internalPtr) const
    {
        return static_cast<uniqueIndexType*>(internalPtr)->first;
    }

public:
    book_item_model(resource_manager * res, QObject * parent = 0) :
        QAbstractItemModel(parent),
        res(res),
        indexData()
    {
        for(int i = 0; i < res->get_books()->size(); ++i)
        {
            insertParentData(res->get_books()->get_book_at(i),true);
            insertParentData(res->get_books()->get_book_at(i),false);
        }
        setHeaderData(0,Qt::Horizontal,QString{"Book"},Qt::DisplayRole);
        setHeaderData(1,Qt::Horizontal,QString{"Pages"},Qt::DisplayRole);
    }

    ~book_item_model(){}

    int rowCount(const QModelIndex &parent = QModelIndex{}) const
    {
        if(!parent.isValid())
        {
            return res->get_books()->size();
        }
        else if(parent.isValid() && parent.column() == 0)
        {
            if(isParent_(parent.internalPointer()))
            {
                return res->get_books()->number_of_pages(getId_(parent.internalPointer()));
            }
        }
        return 0;
    }

    bool isParent(const QModelIndex &m) const
    {
        return isParent_(m.internalPointer());
    }

    int getId(const QModelIndex &m) const
    {
        return getId_(m.internalPointer());
    }
    int getFirstPageId(const QModelIndex &m) const
    {
        return res->get_books()->get_page(getId(m),0);
    }

    bool isFirstPage(const QModelIndex &m) const
    {
        return !isParent_(m.internalPointer()) && m.row() == 0;
    }

    bool isLastPage(const QModelIndex &m) const
    {
        return !isParent_(m.internalPointer()) && m.row() == (res->get_books()->number_of_pages(getId_(m.internalPointer())) - 1);
    }

    int columnCount(const QModelIndex &parent = QModelIndex{}) const
    {
        if(!parent.isValid())
        {
            return 2;
        }
        else if(parent.isValid() && parent.column() == 0 && isParent_(parent.internalPointer()))
        {
            return 1;
        }
        return 0;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex{}) const
    {
        if(!parent.isValid())
        {
            int bookId = res->get_books()->get_book_at(row);
            uniqueIndexType * p = getIndexData(bookId,true);
            return createIndex(row,column,static_cast<void*>(p));
        }
        else if(parent.isValid() && column == 0)
        {
            int bookId = res->get_books()->get_book_at(parent.row());
            uniqueIndexType * p = getIndexData(bookId,false);
            return createIndex(row,column,static_cast<void*>(p));
        }
        return QModelIndex{};
    }

    QModelIndex idToIndex(int id) const
    {
        return index(res->get_books()->get_index(id),0);
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if(!index.isValid())
        {
            return QVariant{};
        }
        int book_id = getId_(index.internalPointer());
        bool is_parent = isParent_(index.internalPointer());
        int page_number = index.row();
        if(role == Qt::DisplayRole)
        {
            if(is_parent && index.column() == 1)
            {
                return QString::number(res->get_books()->number_of_pages(book_id));
            }
            else if(is_parent && !parent(index).isValid())
            {
                return res->book_title(book_id,0,40,true);
            }
            else if(!is_parent && index.column() == 0)
            {
                //return QString{QString{"BookId: "} + QString::number(book_id) + QString{" Page: "} + QString::number(page_number)};
                return res->page_text_summary(book_id,page_number,40);
            }
        }
        else if(role == Qt::UserRole)
        {
            return QString{res->get_strings()->get_page_text(res->get_books()->get_page(book_id,is_parent ? 0 : page_number))};
        }
        return QVariant();
    }

    QModelIndex parent(const QModelIndex &index) const
    {
        if(!index.isValid())
            return QModelIndex{};
        if(!isParent_(index.internalPointer()))
        {
            // Not toplevel
            int bookId = getId_(index.internalPointer());
            uniqueIndexType * q = getIndexData(bookId,true);
            return createIndex(res->get_books()->get_index(bookId),0,static_cast<void*>(q));
        }
        else
        {
            // Toplevel
            return QModelIndex{};
        }
    }

    QVariant headerData(int section, Qt::Orientation, int role) const
    {
        if(role == Qt::DisplayRole)
        {
            if(section == 0)
            {
                return QString{"Book"};
            }
            else if(section == 1)
            {
                return QString{"Number of Pages"};
            }
        }
        return QVariant{};
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if(role == Qt::EditRole)
        {
            switch(value.toInt())
            {
            case (int)book_item_modification::type::swapPages:
                emit dataChanged(index,index.sibling(index.row()+1,0));
                emit dataChanged(index.parent(),index.parent());
                break;
            case (int)book_item_modification::type::changeText:
                emit dataChanged(index,index);
                emit dataChanged(index.parent(),index.parent());
                break;
            }
        }
        return true;
    }

    Qt::ItemFlags flags(const QModelIndex &) const
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    bool canUndo() const
    {
        return undos.size() != 0;
    }

    bool canRedo() const
    {
        return redos.size() != 0;
    }

    void undo()
    {
        if(canUndo())
        {
            redoundo(undos,redos);
        }
    }

    void redo()
    {
        if(canRedo())
        {
            redoundo(redos,undos);
        }
    }

    void movePageUp(const QModelIndex &index)
    {
        if(!index.isValid())
            return;
        if(index.row() == 0)
            return;

        int bookId = getId_(index.internalPointer());
        int pagenum = index.row()-1;
        swapPages(bookId, pagenum);
        undos.push(book_item_modification::getSwapPagesAction(bookId,pagenum));
        clearRedos();
        emit undoStateChanged();
        emit redoStateChanged();
    }

    void movePageDown(const QModelIndex &index)
    {
        if(!index.isValid())
            return;
        if(isLastPage(index))
            return;

        int bookId = getId_(index.internalPointer());
        int pagenum = index.row();
        swapPages(bookId, pagenum);
        undos.push(book_item_modification::getSwapPagesAction(bookId,pagenum));
        clearRedos();
        emit undoStateChanged();
        emit redoStateChanged();
    }

    int onAddBookButtonPressed(QString text = QString{}, int pageId = 0, int nextPageId = 0)
    {
        int bookId = addBook(text,pageId,nextPageId,undos);
        clearRedos();
        emit undoStateChanged();
        emit redoStateChanged();
        return bookId;
    }


    void removePageOrBook(const QModelIndex &index)
    {
        /* Two cases: either remove just a page, or remove a book */
        const bool isRemovingABook = isParent(index) || rowCount(index.parent()) == 1;
        QModelIndex bookIndex = isParent(index) ? index : index.parent();
        if(isRemovingABook)
        {
            removeBook(bookIndex,undos);
        }
        else
        {
            removePage(index,undos);
        }

        clearRedos();
        emit undoStateChanged();
        emit redoStateChanged();
    }

    void onInsertPageButtonPressed(const QModelIndex &index)
    {
        /* Insert first page if "parent" is selected, else insert after the selected page */
        if(isParent(index))
        {
            insertPage(getId(index),0,QString{},undos,true);
        }
        else
        {
            insertPage(getId(index),index.row()+1,QString{},undos,true);
        }
        clearRedos();
        emit undoStateChanged();
        emit redoStateChanged();
    }

    void onSaveChangesButtonPressed(const QModelIndex &index, QString text)
    {
        /* Simply find out the bookid and page number of the text that is updated */
        if(isParent(index))
        {
            setText(getId(index),0,text,undos);
        }
        else
        {
            setText(getId(index),index.row(),text,undos);
        }
        clearRedos();
        emit undoStateChanged();
        emit redoStateChanged();
    }

private:
    void swapPages(int bookId, int page)
    {
        res->swap_pages(bookId,page);
        setData(idToIndex(bookId).child(page,0),QVariant{(int)book_item_modification::type::swapPages},Qt::EditRole);
    }

    void removeBook(const QModelIndex &index, std::stack<book_item_modification::action> & stack)
    {
        const int bookId = getId(index);
        std::vector<int> book;
        res->get_books()->get_book(bookId,book);
        beginRemoveRows(QModelIndex{},index.row(),index.row());
        book_item_modification::action a;
        a.modType = book_item_modification::type::deleteBook;
        a.numPages = book.size();
        a.bookId = bookId;
        for(int i = a.numPages-1; i >= 0; --i)
        {
            book_item_modification::action p;
            p.modType = book_item_modification::type::deletePage;
            p.text = {res->get_strings()->get_page_text(book[i])};
            p.bookId = bookId;
            p.page = i;
            p.pageId = book[i];
            p.nextPageId = (i == a.numPages-1) ? 0 : book[i+1];

            stack.push(p);
        }
        stack.push(a);
        res->remove_book(bookId);
        removeParentData(bookId);
        endRemoveRows();
    }

    void removePage(const QModelIndex &index, std::stack<book_item_modification::action> & stack)
    {

        const int bookId = getId_(index.internalPointer());
        const int pageRow = index.row();

        std::vector<int> book;
        res->get_books()->get_book(bookId,book);
        if(book.size() == 1)
        {
            removeBook(index.parent(),stack);
            return;
        }
        beginRemoveRows(index.parent(),index.row(),index.row());

        book_item_modification::action a;
        a.modType = book_item_modification::type::deletePage;
        a.bookId = bookId;
        a.text = {res->get_strings()->get_page_text(book[pageRow])};
        a.page = pageRow;
        a.pageId = book[pageRow];
        a.nextPageId = (pageRow == a.numPages-1) ? 0 : book[pageRow+1];

        res->remove_page(bookId, pageRow);

        stack.push(a);
        endRemoveRows();
    }

    int addBook(QString text, int pageId, int nextPageId, std::stack<book_item_modification::action> & stack)
    {
        int idx = rowCount(QModelIndex());
        beginInsertRows(QModelIndex(), idx, idx+1);
        int bookId = res->add_book(text,pageId,nextPageId);
        insertParentData(bookId,true);
        insertParentData(bookId,false);

        book_item_modification::action a;
        a.modType = book_item_modification::type::addBook;
        a.bookId = bookId;
        stack.push(a);

        endInsertRows();
        return bookId;
    }

    void restoreBook(int bookId, QString text, int pageId, int nextPageId, std::stack<book_item_modification::action> & stack)
    {
        int idx = rowCount(QModelIndex());
        beginInsertRows(QModelIndex(), idx, idx+1);
        res->restore_book(bookId,text,pageId,nextPageId);
        insertParentData(bookId,true);
        insertParentData(bookId,false);

        book_item_modification::action a;
        a.modType = book_item_modification::type::addBook;
        a.bookId = bookId;
        stack.push(a);

        endInsertRows();
    }

    int insertPage(int bookId, int page, QString text, std::stack<book_item_modification::action> & stack, bool record = true)
    {
        const auto midx = idToIndex(bookId);
        beginInsertRows(midx, page, page+1);
        int pageId = res->insert_page(bookId,page,text);
        if(record)
        {
            book_item_modification::action a;
            a.modType = book_item_modification::type::insertPage;
            a.bookId = bookId;
            a.page = page;
            stack.push(a);
        }

        endInsertRows();
        return pageId;
    }

    void setText(int bookId, int page, QString text, std::stack<book_item_modification::action> & stack)
    {
        book_item_modification::action a;
        a.modType = book_item_modification::type::changeText;
        a.bookId = bookId;
        a.page = page;
        // save the old text for undo/redo
        a.text = QString{res->get_strings()->get_page_text(res->get_books()->get_page(bookId,page))};
        res->setText(bookId,page,text);
        stack.push(a);
        setData(idToIndex(bookId).child(page,0),QVariant{(int)book_item_modification::type::changeText},Qt::EditRole);
    }

    void redoundo(std::stack<book_item_modification::action> & undo, std::stack<book_item_modification::action> & redo)
    {
        book_item_modification::action a = undo.top();
        switch(a.modType)
        {
        case book_item_modification::type::addBook:
            removeBook(idToIndex(a.bookId),redo);
            undo.pop();
            break;
        case book_item_modification::type::changeText:
            setText(a.bookId,a.page,a.text,redo);
            undo.pop();
            break;
        case book_item_modification::type::deleteBook:
        {
            undo.pop();
            book_item_modification::action b = undo.top();
            restoreBook(a.bookId,b.text,b.pageId,b.nextPageId,redo);
            undo.pop();
            for(int i = 1; i < a.numPages; ++i)
            {
                book_item_modification::action c = undo.top();
                insertPage(a.bookId,i,c.text,redo,false);
                undo.pop();
            }
        }
            break;
        case book_item_modification::type::deletePage:
            insertPage(a.bookId,a.page,a.text,redo);
            undo.pop();
            break;
        case book_item_modification::type::insertPage:
            removePage(idToIndex(a.bookId).child(a.page,0),redo);
            undo.pop();
            break;
        case book_item_modification::type::swapPages:
            swapPages(a.bookId,a.page);
            redo.push(a);
            undo.pop();
            break;
        }
        emit redoStateChanged();
        emit undoStateChanged();
    }


public:
signals:
    void undoStateChanged();
    void redoStateChanged();
};

class book_selecter_dialog : public QObject, public selecter_dialog
{
    Q_OBJECT
private:
    QDialog *                           widget;
    QTreeView *                         treeview;
    TextEditAndDisplay *                showtext;
    QHBoxLayout *                       layout;
    QVBoxLayout *                       llayout;
    QVBoxLayout *                       rlayout;
    QHBoxLayout *                       mlayout;
    QHBoxLayout *                       lblayout;
    QHBoxLayout *                       rmlayout;
    QHBoxLayout *                       rblayout;
    QPushButton *                       selectButton;
    QPushButton *                       cancelButton;
    QPushButton *                       saveChangesButton;
    QPushButton *                       addBookButton;
    QPushButton *                       insertPageButton;
    QPushButton *                       deleteSelectedButton;
    QPushButton *                       movePgUpButton;
    QPushButton *                       movePgDownButton;
    QPushButton *                       undoButton;
    QPushButton *                       redoButton;

    book_item_model *                   m_model;
    const resource_manager * const      m_res;

    bool                                isSelectingValidId;

public:
    book_selecter_dialog(book_item_model * model, const resource_manager * res);
    ~book_selecter_dialog(){}
    QDialog * getDialog() const { return widget; }
    void setId(int id);
    bool hasSelection() const { return isSelectingValidId; }
    void reset();
    QString getString(int id) const;
private slots:
    void onBookOrPageSelect(const QModelIndex & mi, const QModelIndex &);
    void onTextChange();
    void undo();
    void redo();
    void moveSelectedPageUp();
    void moveSelectedPageDown();
    void undoEnableOrDisable();
    void redoEnableOrDisable();
    void addBook();
    void onDeletePageOrBook();
    void insertPage();
    void saveTextChanges();
public slots:
    void accept();
    void reject();
};

#endif // BOOK_SELECTER_WIDGET

