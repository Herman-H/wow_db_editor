#include <QPushButton>
#include <QIcon>
#include <QLayout>
#include <QDialog>
#include <QHeaderView>
#include "book_selecter_widget.h"
#include "texteditanddisplay.h"

#include <QApplication>

book_selecter_dialog::book_selecter_dialog(book_item_model * model, const resource_manager * res) :
    QObject(),
    m_model(model),
    m_res(res),
    isSelectingValidId(false)
{
    widget = new QDialog{};
    widget->setLayout(layout = new QHBoxLayout{});
    layout->addLayout(llayout = new QVBoxLayout{});
    layout->addLayout(rlayout = new QVBoxLayout{});
    llayout->addWidget(treeview = new QTreeView{});
    llayout->addLayout(lblayout = new QHBoxLayout{});
    lblayout->addWidget(selectButton = new QPushButton{QString{"Select"}});
    lblayout->addWidget(cancelButton = new QPushButton{QString{"Cancel"}});
    rlayout->addWidget(showtext = new TextEditAndDisplay{});
    rlayout->addLayout(rmlayout = new QHBoxLayout{});
    rlayout->addLayout(rblayout = new QHBoxLayout{});
    rmlayout->addWidget(addBookButton = new QPushButton{QString{"Add book"}});
    rmlayout->addWidget(insertPageButton = new QPushButton{QString{"Insert Page"}});
    rmlayout->addWidget(saveChangesButton = new QPushButton{QString{"Save Changes"}});
    rmlayout->addWidget(deleteSelectedButton = new QPushButton{QString{"Delete"}});
    rblayout->addWidget(movePgUpButton = new QPushButton{QString{"Move up"}});
    rblayout->addWidget(movePgDownButton = new QPushButton{QString{"Move down"}});
    rblayout->addWidget(undoButton = new QPushButton{QString{"Undo"}});
    rblayout->addWidget(redoButton = new QPushButton{QString{"Redo"}});


    widget->setWindowTitle(QString{"Book Select and Edit"});

    treeview->setModel(model);
    treeview->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    treeview->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    treeview->setMinimumSize(QSize{420,500});

    showtext->setMinimumSize(QSize{400,500});
    saveChangesButton->setEnabled(false);
    undoButton->setEnabled(m_model->canUndo());
    redoButton->setEnabled(m_model->canRedo());

    QObject::connect(treeview->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(onBookOrPageSelect(QModelIndex,QModelIndex)));
    QObject::connect(showtext,SIGNAL(textChanged()),this,SLOT(onTextChange()));
    QObject::connect(movePgDownButton,SIGNAL(pressed()),this,SLOT(moveSelectedPageDown()));
    QObject::connect(movePgUpButton,SIGNAL(pressed()),this,SLOT(moveSelectedPageUp()));
    QObject::connect(addBookButton,SIGNAL(pressed()),this,SLOT(addBook()));
    QObject::connect(deleteSelectedButton,SIGNAL(pressed()),this,SLOT(onDeletePageOrBook()));
    QObject::connect(insertPageButton,SIGNAL(pressed()),this,SLOT(insertPage()));
    QObject::connect(saveChangesButton,SIGNAL(pressed()),this,SLOT(saveTextChanges()));
    QObject::connect(m_model,SIGNAL(undoStateChanged()),this,SLOT(undoEnableOrDisable()));
    QObject::connect(m_model,SIGNAL(redoStateChanged()),this,SLOT(redoEnableOrDisable()));
    QObject::connect(undoButton,SIGNAL(pressed()),this,SLOT(undo()));
    QObject::connect(redoButton,SIGNAL(pressed()),this,SLOT(redo()));

    QObject::connect(treeview,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    QObject::connect(selectButton,SIGNAL(pressed()),this,SLOT(accept()));
    QObject::connect(cancelButton,SIGNAL(pressed()),widget,SLOT(reject()));
    QObject::connect(widget,SIGNAL(rejected()),this,SLOT(reject()));
}



void book_selecter_dialog::moveSelectedPageDown()
{
    m_model->movePageDown(treeview->selectionModel()->selectedRows(0).at(0));
    QModelIndex m = treeview->selectionModel()->selectedRows().at(0);
    treeview->selectionModel()->setCurrentIndex(m.sibling(m.row()+1,0),QItemSelectionModel::ClearAndSelect);
}

void book_selecter_dialog::moveSelectedPageUp()
{
    m_model->movePageUp(treeview->selectionModel()->selectedRows(0).at(0));
    QModelIndex m = treeview->selectionModel()->selectedRows().at(0);
    treeview->selectionModel()->setCurrentIndex(m.sibling(m.row()-1,0),QItemSelectionModel::ClearAndSelect);
}

void book_selecter_dialog::undo()
{
    m_model->undo();
}

void book_selecter_dialog::redo()
{
    m_model->redo();
}

void book_selecter_dialog::onBookOrPageSelect(const QModelIndex &mi, const QModelIndex&)
{
    saveChangesButton->setEnabled(false);
    showtext->setText(m_model->data(mi,Qt::UserRole).toString());

    if(!m_model->isParent(mi))
    {
        if(!m_model->isFirstPage(mi))
        {
            movePgUpButton->setEnabled(true);
        }
        else
        {
            movePgUpButton->setEnabled(false);
        }
        if(!m_model->isLastPage(mi))
        {
            movePgDownButton->setEnabled(true);
        }
        else
        {
            movePgDownButton->setEnabled(false);
        }
    }
    else
    {
        movePgDownButton->setEnabled(false);
        movePgUpButton->setEnabled(false);
    }
}

void book_selecter_dialog::onTextChange()
{
    saveChangesButton->setEnabled(showtext->isTextChanged());
}

void book_selecter_dialog::undoEnableOrDisable()
{
    undoButton->setEnabled(m_model->canUndo());
}

void book_selecter_dialog::redoEnableOrDisable()
{
    redoButton->setEnabled(m_model->canRedo());
}

void book_selecter_dialog::setId(int id)
{
    /* the id has to be a front page of a book, so look for it among all books */
    int resultingrow = m_res->get_books()->first_page_index(id,isSelectingValidId);
    QModelIndex selectAndScrollTo{m_model->index(resultingrow,0)};
    treeview->setCurrentIndex(selectAndScrollTo);
    treeview->scrollTo(selectAndScrollTo);
}

void book_selecter_dialog::reset()
{
    treeview->scrollTo(m_model->index(0,0));
    treeview->setCurrentIndex(m_model->index(0,0));
    isSelectingValidId = false;
}

void book_selecter_dialog::addBook()
{
    m_model->onAddBookButtonPressed();
    treeview->setCurrentIndex(m_model->index(m_model->rowCount()-1,0));
}

void book_selecter_dialog::onDeletePageOrBook()
{
    QModelIndex removeIndex = treeview->selectionModel()->selectedRows(0).at(0);
    bool isTopLevel = !removeIndex.parent().isValid() || m_model->rowCount(removeIndex.parent()) == 1;
    int row = !removeIndex.parent().isValid() ? removeIndex.row() : removeIndex.parent().row();
    if(isTopLevel && row >= m_model->rowCount()-1 && m_model->rowCount() > 1)
    {
        // Do this to prevent crashing
        treeview->setCurrentIndex(m_model->index(row-1,0));
    }
    m_model->removePageOrBook(removeIndex);
    // Since we are deleting current selection, the selection will get changed, invoke onBookOrPageSelect
    if(m_model->rowCount() > 0)
    {
        onBookOrPageSelect(treeview->selectionModel()->selectedRows(0).at(0),QModelIndex{});
    }
    else
    {
        // This is when all items are removed
        showtext->setText("");
        saveChangesButton->setEnabled(false);
        movePgDownButton->setEnabled(false);
        movePgUpButton->setEnabled(false);
        deleteSelectedButton->setEnabled(false);
        insertPageButton->setEnabled(false);
    }
    showtext->clearFocus();
}

void book_selecter_dialog::insertPage()
{
    m_model->onInsertPageButtonPressed(treeview->selectionModel()->selectedRows(0).at(0));
}

void book_selecter_dialog::saveTextChanges()
{
    showtext->saveChanges();
    m_model->onSaveChangesButtonPressed(treeview->selectionModel()->selectedRows(0).at(0),showtext->text());
    saveChangesButton->setEnabled(false);
}

QString book_selecter_dialog::getString(int id) const
{
    if(id != 0)
        return m_res->book_title_(id,30,true);
    else
        return QString{"Book..."};
}

void book_selecter_dialog::accept()
{
    isSelectingValidId = true;
    widget->done(m_model->getFirstPageId(treeview->selectionModel()->selectedIndexes().at(0)));
}

void book_selecter_dialog::reject()
{
    widget->setResult(-1);
}
