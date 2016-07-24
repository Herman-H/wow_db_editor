#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <setupdbdialog.h>
#include "config.h"
#include "database.h"
#include "editcreaturetemplatewidget.h"
#include "creaturequery.h"
#include "dbc/dbcrecorddescriptors.h"
#include "creaturetemplatequerygroupbox.h"
#include "itemtemplateedit.h"

#include "widgets/item_models.h"

#include "session_manager.h"
#include "resource_manager.h"

class dbccache;
class dbcache;
class creature_template_selecter_widget;
class session_view;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionDatabase_Setup_triggered();

public slots:
    void onSetupDBDialog_Close();
    void onSetupDBDialog_Connect();
    void onCreatureTemplateQueryPushed();
    void onCreatureTemplateSelected();
private:
    config cfg;
    config_dbc dcfg;
    config_session_add_file session_prepend_cfg;
    config_session_add_file session_append_cfg;
    database db;
    Ui::MainWindow *ui;
    SetupDBDialog *db_cfg_dialog;
    creature_template_selecter_widget * creaturetemplateselecter;
    itemtemplateedit * itemedit;
    entityviews::dbcs_module_type dbc;
    dbccache * m_dbccache;
    dbcache * m_dbcache;
    EditCreatureTemplateWidget *editcreature;
    CreatureQuery *creatureQuery;
    int selected_creature_template_entry;

    item_models * itemmodels;
    session_view * sess_view;

private:
    QString display_connection_status(database::status s);
    void try_connect_all();

private slots:
    void on_actionDBC_Setup_triggered();
    void on_actionView_Session_triggered();
};

#endif // MAINWINDOW_H
