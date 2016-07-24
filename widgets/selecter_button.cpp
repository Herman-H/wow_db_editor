#include "selecter_button.h"
#include "selecter_dialog.h"
#include <QPushButton>
#include <QIcon>
#include <QLayout>
#include <QDialog>
#include <QApplication>
#include <QStyle>

selecter_button::selecter_button(selecter_dialog * dialog, int unselectedId) :
    QObject(),
    dialog(dialog),
    invalidIdDefault(unselectedId)
{
    widget = new QWidget{};
    widget->setLayout(layout = new QHBoxLayout{});
    layout->addWidget(button = new QPushButton{});
    layout->addWidget(clr = new QPushButton{});
    clr->setMaximumWidth(25);
    clr->setIcon(QIcon{QApplication::style()->standardIcon(QStyle::SP_LineEditClearButton)});
    clear();

    QObject::connect(clr,SIGNAL(clicked(bool)),this,SLOT(clear()));
    QObject::connect(button,SIGNAL(clicked(bool)),this,SLOT(buttonPushed()));
}

selecter_button::~selecter_button()
{
    delete dialog;
}

QWidget * selecter_button::getButton()
{
    return widget;
}

int selecter_button::selectedId()
{
    if(isSelectingValidId)
    {
        return selected;
    }
    else
    {
        return invalidIdDefault;
    }
}

void selecter_button::setId(int id)
{
    selected = id;
    dialog->setId(id);
    if(dialog->hasSelection())
    {
        isSelectingValidId = true;
        button->setText(dialog->getString(selected));
    }
    else
    {
        isSelectingValidId = false;
        button->setText(dialog->getString(invalidIdDefault));
    }
}

void selecter_button::buttonPushed()
{
    int i = dialog->getDialog()->exec();
    if(i == -1)
    {
        if(dialog->hasSelection())
        {
            dialog->setId(selected);
        }
    }
    else
    {
        selected = i;
        if(dialog->hasSelection())
        {
            isSelectingValidId = true;
            button->setText(dialog->getString(selected));
        }
        else
        {
            isSelectingValidId = false;
            button->setText(dialog->getString(invalidIdDefault));
        }
    }
}

void selecter_button::clear()
{
    isSelectingValidId = false;
    button->setText(dialog->getString(invalidIdDefault));
    dialog->reset();
}

