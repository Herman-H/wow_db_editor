#ifndef DBWIDGETS_H
#define DBWIDGETS_H

#include "dbc/dbcrecorddescriptors.h"
#include <QObject>
#include <vector>
#include "cachetypes.h"
#include <QWidget>

using namespace burning_crusade_records;

class QSqlDatabase;
class CreatureTemplateQueryGroupBox;
class itemtemplatequery;
class QPushButton;
class QDialog;
class QListWidget;
class QTreeWidget;
class QTreeWidgetItem;
class QVBoxLayout;
class QHBoxLayout;
class QTextEdit;
class QCheckBox;
class QString;
class QLabel;
class QScrollArea;
class dbccache;
class dbcache;
class QButtonGroup;

class creature_template_selecter_widget
{
private:
    QSqlDatabase &                  db;
    CreatureTemplateQueryGroupBox * widget;
    dbccache *                      m_dbccache;
public:

    creature_template_selecter_widget(QSqlDatabase & db, dbccache * c);
    ~creature_template_selecter_widget();
    CreatureTemplateQueryGroupBox * getWidget();
    void setIndexFromId(int id);    // entry in creature_template
    int selectedId();               // get entry which is selected in the menu
};

class creature_template_selecter_dialog : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase &                      db;
    QDialog *                           widget;
    CreatureTemplateQueryGroupBox *     ct;
    QPushButton *                       cancelButton;
public:
    creature_template_selecter_dialog(QSqlDatabase & db, dbccache * c);
    ~creature_template_selecter_dialog();
    QDialog * getWidget(){ return widget; }
    void setIndex(int id);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void accept();
    void reject();
};

class creature_template_selecter_button : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase &                      db;
    QWidget *                           widget;
    QPushButton *                       button;
    QPushButton *                       clr;
    creature_template_selecter_dialog * dialog;
    int selected;
public:
    creature_template_selecter_button(QSqlDatabase & db, dbccache * c, bool clearBtn);
    ~creature_template_selecter_button();

    QWidget *getWidget();
    int selectedId();
    void setIndexFromId(int id);
public slots:
    void buttonPushed();
    void clear();
};

class item_template_selecter_dialog : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase &                      db;
    dbccache *                          m_dbccache;
    QDialog *                           widget;
    itemtemplatequery *                 it;
    QPushButton *                       cancelButton;
public:
    item_template_selecter_dialog(QSqlDatabase & db, dbccache * c);
    ~item_template_selecter_dialog();
    QDialog * getWidget(){ return widget; }
    void setIndex(int id);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void accept();
    void reject();
};

class item_template_selecter_button : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase &                      db;
    QPushButton *                       widget;
    item_template_selecter_dialog *     dialog;
    int selected;
public:
    item_template_selecter_button(QSqlDatabase & db, dbccache * c);
    ~item_template_selecter_button();
    QPushButton * getWidget();
    int selectedId();
    void setIndexFromId(int id);
public slots:
    void buttonPushed();
signals:
    void selection();
};

class creature_loot_dialog : public QObject
{
    Q_OBJECT
private:
    dbcache *                           m_dbcache;
    QDialog *                           widget;
    QTreeWidget *                       list;
    QVBoxLayout *                       layout;
    QHBoxLayout *                       blayout;
    QPushButton *                       acceptButton;
    QPushButton *                       clearButton;
    QPushButton *                       cancelButton;
public:
    creature_loot_dialog(dbcache * c);
    ~creature_loot_dialog();
    QDialog * getWidget(){ return widget; }
    void setIndex(int index);
    void reload(db_resource_type r);
public slots:
    void acceptButtonTriggered();
    void clearButtonTriggered();
    void cancelButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
    void initialized();
};

class creature_loot_selecter : public QObject
{
    Q_OBJECT
private:
    dbcache *                           m_dbcache;
    QPushButton *                       widget;
    creature_loot_dialog *              dialog;
    int selected;
    int pending_id;
public:
    creature_loot_selecter(dbcache * c);
    ~creature_loot_selecter();
    QPushButton * getWidget();
    void reload(db_resource_type r);
    int selectedId();
    void setIndexFromId(int id);
public slots:
    void buttonPushed();
    void clear();
    void enable();
    void disable();
};

class disenchant_loot_dialog : public QObject
{
    Q_OBJECT
private:
    dbcache     *                       m_dbcache;
    QDialog *                           widget;
    QTreeWidget *                       list;
    QVBoxLayout *                       layout;
    QHBoxLayout *                       blayout;
    QPushButton *                       acceptButton;
    QPushButton *                       editButton;
    QPushButton *                       cancelButton;
    QPushButton *                       clearButton;
public:
    disenchant_loot_dialog(dbcache * c);
    ~disenchant_loot_dialog();
    QDialog * getWidget(){ return widget; }
    void setIndex(int index);
    void update();
    void reload(db_resource_type r);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class disenchant_loot_selecter : public QObject
{
    Q_OBJECT
private:
    dbcache *                           m_dbcache;
    QPushButton *                       widget;
    disenchant_loot_dialog *            dialog;
    int selected;
public:
    disenchant_loot_selecter(dbcache * c);
    ~disenchant_loot_selecter();
    QPushButton * getWidget();
    int selectedId();
    void reload(db_resource_type r);
    void setIndexFromId(int id);
public slots:
    void buttonPushed();
    void clear();
};

class item_random_property_dialog : public QObject
{
    Q_OBJECT
private:
    dbcache *                           m_dbcache;
    QDialog *                           widget;
    QTreeWidget *                       list;
    QVBoxLayout *                       layout;
    QHBoxLayout *                       blayout;
    QPushButton *                       acceptButton;
    QPushButton *                       cancelButton;
    QPushButton *                       clearButton;
    bool m_suffix;
public:
    item_random_property_dialog(dbcache * c, bool suffix);
    ~item_random_property_dialog();
    QDialog * getWidget(){ return widget; }
    void setIndex(int index);
    void reload(db_resource_type r);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class item_random_property_selecter : public QObject
{
    Q_OBJECT
private:
    dbcache *                           m_dbcache;
    QPushButton *                       widget;
    item_random_property_dialog *       dialog;
    int selected;
    bool m_suffix;
public:
    item_random_property_selecter(dbcache * c, bool suffix);
    ~item_random_property_selecter();
    QPushButton * getWidget();
    int selectedId();
    void reload(db_resource_type r);
    void setIndexFromId(int id);
public slots:
    void buttonPushed();
    void clear();
signals:
    void changed();
};

class lock_dialog : public QObject
{
    Q_OBJECT
private:
    dbcache *                           m_dbcache;
    QDialog *                           widget;
    QListWidget *                       list;
    QVBoxLayout *                       layout;
    QHBoxLayout *                       blayout;
    QPushButton *                       acceptButton;
    QPushButton *                       cancelButton;
    QPushButton *                       clearButton;
public:
    lock_dialog(dbcache * c);
    ~lock_dialog();
    QDialog * getWidget(){ return widget; }
    void setIndex(int index);
    void reload(db_resource_type r);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class lock_selecter : public QObject
{
    Q_OBJECT
private:
    dbcache *                           m_dbcache;
    QPushButton *                       widget;
    lock_dialog *                       dialog;
    int selected;
public:
    lock_selecter(dbcache * c);
    ~lock_selecter();
    QPushButton * getWidget();
    void reload(db_resource_type r);
    int selectedId();
    void setIndexFromId(int id);
public slots:
    void buttonPushed();
    void clear();
};

class itemextendedcost_dialog : public QObject
{
    Q_OBJECT
private:
    dbcache     *        m_dbcache;
    int                  selected;
    QVBoxLayout *        layout;
    QHBoxLayout *        blayout;
    QListWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog *            widget;
public:
    itemextendedcost_dialog(dbcache * c);
    ~itemextendedcost_dialog();
    QDialog * getWidget();
    void reload(db_resource_type r);
    void setIndex(int index);
public slots:
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
signals:
    void clear();
};

class itemextendedcost_selecter : public QObject
{
    Q_OBJECT
private:
    dbcache *                   m_dbcache;
    int                         selected;
    itemextendedcost_dialog *   dialog;
    QPushButton *               widget;
public:
    itemextendedcost_selecter(dbcache *c);
    ~itemextendedcost_selecter();
    QPushButton * getWidget();
    void reload(db_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
public slots:
    void buttonPushed();
    void clear();
};

class npc_text_view : public QWidget
{
    Q_OBJECT
    // Present the content of an npc_text entry on a widget
private:
    dbcache * m_dbcache;
    void setView(int id);
public:
    npc_text_view(dbcache *c, int id);
    ~npc_text_view();
};

class gossip_menu_dialog : public QObject
{
    Q_OBJECT
private:
    dbcache     *        m_dbcache;
    int                  selected;
    QHBoxLayout *        layout;
    QVBoxLayout *        llayout;
    QScrollArea *        rscroll;
    QHBoxLayout *        blayout;
    QTreeWidget *        list;
    QPushButton *        acceptB;
    QPushButton *        cancelB;
    QPushButton *        clearB;
    QDialog *            widget;
    QButtonGroup *       optionButtonGroup;
    int pressedOptionIndex;
    int optionTriggerData[32]; // There seems to be a limit to 32 options in the client (source: mangos src code)
public:
    gossip_menu_dialog(dbcache * c);
    ~gossip_menu_dialog();
    QDialog * getWidget();
    void reload(db_resource_type r);
    void setIndex(int index);
public slots:
    void refreshScrollArea();
    void acceptButtonTriggered();
    void cancelButtonTriggered();
    void clearButtonTriggered();
    void accept();
    void reject();
    void optionButtonPressed(int id);
signals:
    void clear();
};

class gossip_menu_selecter : public QObject
{
    Q_OBJECT
private:
    dbcache             * m_dbcache;
    gossip_menu_dialog  * dialog;
    int                   selected;
    QPushButton         * widget;
public:
    gossip_menu_selecter(dbcache *c);
    ~gossip_menu_selecter();
    QPushButton * getWidget();
    void reload(db_resource_type r);
    void setIndexFromId(int id);
    int selectedId();
public slots:
    void buttonPushed();
    void clear();
};



#endif // DBWIDGETS_H
