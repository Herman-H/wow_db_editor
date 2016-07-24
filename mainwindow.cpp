#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QItemSelectionModel>
#include <string>
#include <QFileDialog>
#include "dbwidgets.h"
#include "dbccache.h"
#include "dbcache.h"
#include "widgets/session_view.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cfg("db.cfg"),
    dcfg("dbc.cfg"),
    session_prepend_cfg("session_prepend.cfg"),
    session_append_cfg("session_append.cfg"),
    db(cfg),
    dbc(dcfg.get_path().toStdString()),
    m_dbccache(new dbccache(dbc)),
    m_dbcache(new dbcache(m_dbccache,db.World(),dbc)),
    itemmodels(new item_models{})
{
    itemmodels->initialize(db.World(),cfg);
    ui->setupUi(this);
    editcreature = new EditCreatureTemplateWidget{db.World(),dbc,m_dbccache,m_dbcache,itemmodels,this};
    editcreature->setMinimumSize(QSize{950,700});
    creaturetemplateselecter = new creature_template_selecter_widget{(db.World()),m_dbccache};
    ui->CreatureTemplateQueryLayout->addWidget(creaturetemplateselecter->getWidget());
    ui->itemtemplateeditLayout->addWidget((itemedit = new itemtemplateedit(db.World(),dbc,m_dbccache,m_dbcache,itemmodels)));
    ui->editCreatureScrollArea->setWidget(editcreature);
    ui->editCreatureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->editCreatureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    creatureQuery = new CreatureQuery{db.World(),this};
    QObject::connect(creatureQuery,SIGNAL(pushedQueryCreatureTemplate()), this, SLOT(onCreatureTemplateQueryPushed()));
    QObject::connect(creaturetemplateselecter->getWidget(),SIGNAL(selected()),this,SLOT(onCreatureTemplateSelected()));
    ui->CreatureQueryLayout->addWidget(creatureQuery);
    creatureQuery->show();
    try_connect_all();
    //test_strings_resource_storage(&(db.World()));
    sess_view = new session_view{itemmodels->get_session_manager()};
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionDatabase_Setup_triggered()
{
    db_cfg_dialog = new SetupDBDialog{cfg,this};
    QObject::connect(db_cfg_dialog,SIGNAL(closed(void)),this,SLOT(onSetupDBDialog_Close(void)));
    QObject::connect(db_cfg_dialog,SIGNAL(issued_connect(void)),this, SLOT(onSetupDBDialog_Connect(void)));
    db_cfg_dialog->show();
}

void MainWindow::onSetupDBDialog_Close()
{
    db_cfg_dialog->close();
    QObject::disconnect(db_cfg_dialog,SIGNAL(closed(void)),this,SLOT(onSetupDBDialog_Close()));
    QObject::disconnect(db_cfg_dialog,SIGNAL(issued_connect(void)),this,SLOT(onSetupDBDialog_Connect()));
    delete db_cfg_dialog;
}

void MainWindow::onSetupDBDialog_Connect()
{
    onSetupDBDialog_Close();
    // Try To Connect to database
    db.reconfigure(cfg);
    try_connect_all();
}

QString MainWindow::display_connection_status(database::status s)
{
    QString t;
    if(s.connected)
    {
        t = "connected";
    }
    else if(s.didnttry)
    {
        t = "waiting to connect";
    }
    else if(s.connection_error)
    {
        t = "connection error";
    }
    else if(s.unknown_error)
    {
        t = "unknown error";
    }
    else
    {
        t = "display_connection_status";
    }
    return t;
}

void MainWindow::try_connect_all()
{
    QString w,r,c;
    w = display_connection_status(db.try_open_world());
    r = display_connection_status(db.try_open_realm());
    c = display_connection_status(db.try_open_chars());
    QString statustext = "World DB: " + w + " | Realm DB: " + r + " | Chars DB: " + c;
    ui->DBstatus->setText(statustext);
}

void MainWindow::onCreatureTemplateQueryPushed()
{
    ui->CreatureTabs->setCurrentIndex(0);
}

void MainWindow::onCreatureTemplateSelected()
{
    ui->CreatureTabs->setCurrentIndex(1);
    editcreature->LoadEntry(creaturetemplateselecter->selectedId());
}

void MainWindow::on_actionDBC_Setup_triggered()
{
    QString s = QFileDialog::getExistingDirectory(this, tr("Select Directory"), dcfg.get_path(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(s.length() != 0)
    {
        dcfg.set_path(s+"/");
        dcfg.save_config();
    }
}


void MainWindow::on_actionView_Session_triggered()
{
    sess_view->update();
    sess_view->exec();
}
