#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T20:52:35
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

TARGET = wow_db_editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    setupdbdialog.cpp \
    editcreaturetemplatewidget.cpp \
    creaturequery.cpp \
    creaturetemplatequerygroupbox.cpp \
    trainer_teach_edit.cpp \
    vendor_edit.cpp \
    dbcwidgets.cpp \
    dbwidgets.cpp \
    itemtemplatequery.cpp \
    itemtemplateedit.cpp \
    dbccache.cpp \
    dbcache.cpp \
    resource_storage.cpp \
    database/creature_template.cpp \
    session_manager.cpp \
    widgets/book_selecter_widget.cpp \
    resource_manager.cpp \
    widgets/item_models.cpp \
    database/page_text.cpp \
    widgets/session_view.cpp \
    widgets/texteditanddisplay.cpp \
    widgets/loot_selecter_widget.cpp \
    widgets/selecter_button.cpp \
    resource_dependency.cpp \
    database/test.cpp

HEADERS  += mainwindow.h \
    config.h \
    database.h \
    setupdbdialog.h \
    creaturetemplatequery.h \
    editcreaturetemplatewidget.h \
    SQL_Functions.h \
    creaturequery.h \
    dbc/dbcfile.h \
    dbc/dbctmp.h \
    dbc/dbcrecorddescriptors.h \
    dbc/dbcparse.h \
    dbcwidgets.h \
    creaturetemplatequerygroupbox.h \
    trainer_teach_edit.h \
    vendor_edit.h \
    dbwidgets.h \
    itemtemplatequery.h \
    itemtemplateedit.h \
    dbccache.h \
    dbcache.h \
    cachetypes.h \
    condition.h \
    database/dbtmp.h \
    database/circularqueue.h \
    database/creature_template.h \
    database/table.h \
    database/dbinterface.h \
    resource_storage.h \
    session_manager.h \
    database/test.h \
    widgets/book_selecter_widget.h \
    resource_manager.h \
    widgets/item_models.h \
    resource_initialization.h \
    database/page_text.h \
    widgets/session_view.h \
    widgets/texteditanddisplay.h \
    widgets/loot_selecter_widget.h \
    widgets/selecter_button.h \
    widgets/selecter_dialog.h \
    resource_dependency.h \
    tmp/tmp.h \
    tmp/tmp_math.h \
    tmp/tmp_predicates.h \
    tmp/tmp_test.h \
    tmp/tmp_type_traits.h \
    tmp/tmp_types.h

FORMS    += mainwindow.ui \
    setupdbdialog.ui \
    editcreaturetemplatewidget.ui \
    creaturequery.ui \
    creaturetemplatequerygroupbox.ui \
    trainer_teach_edit.ui \
    vendor_edit.ui \
    gossip_edit.ui \
    itemtemplatequery.ui \
    itemtemplateedit.ui \
    session_manager.ui \
    widgets/session_view.ui \
    widgets/texteditanddisplay.ui
