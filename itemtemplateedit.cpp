#include "itemtemplateedit.h"
#include "ui_itemtemplateedit.h"
#include "widgets/book_selecter_widget.h"
#include "dbwidgets.h"
#include "dbcwidgets.h"
#include "SQL_Functions.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QValidator>
#include <QTextEdit>
#include <math.h>
#include "widgets/selecter_button.h"
#include "dbccache.h"
#include "dbcache.h"

itemtemplateedit::itemtemplateedit(QSqlDatabase &db, entityviews::dbcs_module_type &dbc, dbccache * dbcC, dbcache * dbC, item_models* ims, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemtemplateedit),
    db(db),
    m_dbccache(dbcC),
    m_dbcache(dbC),
    module(dbc)
{
    ui->setupUi(this);
    ui->entryselecterLayout->addWidget((itemselecter = new item_template_selecter_button(db,m_dbccache))->getWidget());
    ui->classLayout->addWidget((classselecter = new item_class_selecter{m_dbccache})->getWidget());
    subclassselecter = new item_subclass_selecter{m_dbccache};
    ui->subclassLayout->addWidget(subclassselecter->getWidget());
    subclassselecter->getWidget()->setEnabled(false);
    ui->qualityLayout->addWidget((qualityselecter = new item_quality_selecter())->getWidget());
    ui->inventotytypeLayout->addWidget((slotselecter = new item_slot_selecter())->getWidget());
    ui->itemsetLayout->addWidget((itemsetselecter = new item_set_selecter{m_dbccache})->getWidget());
    ui->reqmapLayout->addWidget((reqmapselecter = new map_selecter{m_dbccache})->getWidget());
    ui->reqareaLayout->addWidget((reqareaselecter = new area_selecter{m_dbccache})->getWidget());
    ui->displayLayout->addWidget((modelselect = new item_model_selecter{m_dbccache,0})->getWidget());
    ui->classmaskLayout->layout()->addWidget(((classmask = new class_mask_selecter())->getWidget()));
    ui->racemaskLayout->layout()->addWidget(((racemask = new race_mask_selecter())->getWidget()));
    ui->reqhonorLayout->addWidget((honorselecter = new honor_rank_selecter())->getWidget());
    ui->reqskillLayout->addWidget((skillselecter = new skillline_selecter{m_dbccache})->getWidget());
    ui->reqspellLayout->addWidget((spellselecter = new spell_selecter{m_dbccache})->getWidget());
    ui->reqfactionLayout->addWidget((repfactionselecter = new reputation_faction_selecter{m_dbccache})->getWidget());
    ui->reqfactionrankLayout->addWidget((reprankselecter = new reputation_rank_selecter())->getWidget());
    QObject::connect(classselecter->getWidget(),SIGNAL(currentIndexChanged(int)),this,SLOT(onItemClassChange()));
    QObject::connect(slotselecter->getWidget(),SIGNAL(currentIndexChanged(int)),this,SLOT(onItemSlotChange()));

    ui->ilvlEdit->setValidator(new QIntValidator());
    ui->reqlvlEdit->setValidator(new QIntValidator());

    /* Stat panel */
    ui->stat1->addWidget((stats[0] = new item_stat_type_selecter())->getWidget());
    ui->stat1->addWidget((statsamount[0] = new QLineEdit()));
    ui->stat2->addWidget((stats[1] = new item_stat_type_selecter())->getWidget());
    ui->stat2->addWidget((statsamount[1] = new QLineEdit()));
    ui->stat3->addWidget((stats[2] = new item_stat_type_selecter())->getWidget());
    ui->stat3->addWidget((statsamount[2] = new QLineEdit()));
    ui->stat4->addWidget((stats[3] = new item_stat_type_selecter())->getWidget());
    ui->stat4->addWidget((statsamount[3] = new QLineEdit()));
    ui->stat5->addWidget((stats[4] = new item_stat_type_selecter())->getWidget());
    ui->stat5->addWidget((statsamount[4] = new QLineEdit()));
    ui->stat6->addWidget((stats[5] = new item_stat_type_selecter())->getWidget());
    ui->stat6->addWidget((statsamount[5] = new QLineEdit()));
    ui->stat7->addWidget((stats[6] = new item_stat_type_selecter())->getWidget());
    ui->stat7->addWidget((statsamount[6] = new QLineEdit()));
    ui->stat8->addWidget((stats[7] = new item_stat_type_selecter())->getWidget());
    ui->stat8->addWidget((statsamount[7] = new QLineEdit()));
    ui->stat9->addWidget((stats[8] = new item_stat_type_selecter())->getWidget());
    ui->stat9->addWidget((statsamount[8] = new QLineEdit()));
    ui->stat10->addWidget((stats[9] = new item_stat_type_selecter())->getWidget());
    ui->stat10->addWidget((statsamount[9] = new QLineEdit()));

    statsamount[0]->setValidator(new QIntValidator());
    statsamount[1]->setValidator(new QIntValidator());
    statsamount[2]->setValidator(new QIntValidator());
    statsamount[3]->setValidator(new QIntValidator());
    statsamount[4]->setValidator(new QIntValidator());
    statsamount[5]->setValidator(new QIntValidator());
    statsamount[6]->setValidator(new QIntValidator());
    statsamount[7]->setValidator(new QIntValidator());
    statsamount[8]->setValidator(new QIntValidator());
    statsamount[9]->setValidator(new QIntValidator());
    statsamount[0]->setPlaceholderText("Amount...");
    statsamount[1]->setPlaceholderText("Amount...");
    statsamount[2]->setPlaceholderText("Amount...");
    statsamount[3]->setPlaceholderText("Amount...");
    statsamount[4]->setPlaceholderText("Amount...");
    statsamount[5]->setPlaceholderText("Amount...");
    statsamount[6]->setPlaceholderText("Amount...");
    statsamount[7]->setPlaceholderText("Amount...");
    statsamount[8]->setPlaceholderText("Amount...");
    statsamount[9]->setPlaceholderText("Amount...");
    statsamount[0]->setMaximumWidth(80);
    statsamount[1]->setMaximumWidth(80);
    statsamount[2]->setMaximumWidth(80);
    statsamount[3]->setMaximumWidth(80);
    statsamount[4]->setMaximumWidth(80);
    statsamount[5]->setMaximumWidth(80);
    statsamount[6]->setMaximumWidth(80);
    statsamount[7]->setMaximumWidth(80);
    statsamount[8]->setMaximumWidth(80);
    statsamount[9]->setMaximumWidth(80);

    ui->randompropLayout->addWidget((randompropertyselecter = new item_random_property_selecter{m_dbcache,false})->getWidget());
    ui->randomsuffLayout->addWidget((randomsuffixselecter = new item_random_property_selecter{m_dbcache,true})->getWidget());

    QObject::connect(randompropertyselecter,SIGNAL(changed()),this,SLOT(onPropertyChange()));
    QObject::connect(randomsuffixselecter,SIGNAL(changed()),this,SLOT(onSuffixChange()));

    ui->armorEdit->setValidator(new QIntValidator());
    ui->bonusarmorEdit->setValidator(new QIntValidator());
    ui->blockEdit->setValidator(new QIntValidator());
    ui->holyresEdit->setValidator(new QIntValidator());
    ui->natureresEdit->setValidator(new QIntValidator());
    ui->fireresEdit->setValidator(new QIntValidator());
    ui->shadowresEdit->setValidator(new QIntValidator());
    ui->frostresEdit->setValidator(new QIntValidator());
    ui->arcaneresEdit->setValidator(new QIntValidator());

    ui->spinBox->setValue(10);
    ui->durabilityEdit->setValidator(new QIntValidator());

    ui->socket1Layout->addWidget((socketselecter[0] = new socket_color_selecter())->getWidget());
    ui->socket2Layout->addWidget((socketselecter[1] = new socket_color_selecter())->getWidget());
    ui->socket3Layout->addWidget((socketselecter[2] = new socket_color_selecter())->getWidget());
    ui->socketbonusLayout->addWidget((socketbonusselecter = new item_enchant_selecter{m_dbccache})->getWidget());

    /* Spell panel */
    ui->spell1selectLayout->addWidget((spell1selecter = new spell_selecter{m_dbccache})->getWidget());
    ui->spell2selectLayout->addWidget((spell2selecter = new spell_selecter{m_dbccache})->getWidget());
    ui->spell3selectLayout->addWidget((spell3selecter = new spell_selecter{m_dbccache})->getWidget());
    ui->spell5selectLayout->addWidget((spell5selecter = new spell_selecter{m_dbccache})->getWidget());
    ui->spell4selectLayout->addWidget((spell4selecter = new spell_selecter{m_dbccache})->getWidget());
    ui->spelltrigger1Layout->addWidget((spelltriggerselecter[0] = new spell_trigger_selecter())->getWidget());
    ui->spelltrigger2Layout->addWidget((spelltriggerselecter[1] = new spell_trigger_selecter())->getWidget());
    ui->spelltrigger3Layout->addWidget((spelltriggerselecter[2] = new spell_trigger_selecter())->getWidget());
    ui->spelltrigger4Layout->addWidget((spelltriggerselecter[3] = new spell_trigger_selecter())->getWidget());
    ui->spelltrigger5Layout->addWidget((spelltriggerselecter[4] = new spell_trigger_selecter())->getWidget());

    QObject::connect(spell1selecter,SIGNAL(wasselected()),this,SLOT(onSpell1Selection()));
    QObject::connect(spell2selecter,SIGNAL(wasselected()),this,SLOT(onSpell2Selection()));
    QObject::connect(spell3selecter,SIGNAL(wasselected()),this,SLOT(onSpell3Selection()));
    QObject::connect(spell4selecter,SIGNAL(wasselected()),this,SLOT(onSpell4Selection()));
    QObject::connect(spell5selecter,SIGNAL(wasselected()),this,SLOT(onSpell5Selection()));

    QObject::connect(spell1selecter,SIGNAL(wascleared()),this,SLOT(on_spell1Clr_clicked()));
    QObject::connect(spell2selecter,SIGNAL(wascleared()),this,SLOT(on_spell2Clr_clicked()));
    QObject::connect(spell3selecter,SIGNAL(wascleared()),this,SLOT(on_spell3Clr_clicked()));
    QObject::connect(spell4selecter,SIGNAL(wascleared()),this,SLOT(on_spell4Clr_clicked()));
    QObject::connect(spell5selecter,SIGNAL(wascleared()),this,SLOT(on_spell5Clr_clicked()));

    /* Weapon panel */
    ui->sheatheLayout->addWidget((sheatheselecter = new item_sheathe_selecter())->getWidget());
    ui->dmg1Layout->addWidget((wpndmgtype[0] = new damage_school_selecter())->getWidget());
    ui->dmg2Layout->addWidget((wpndmgtype[1] = new damage_school_selecter())->getWidget());
    ui->dmg3Layout->addWidget((wpndmgtype[2] = new damage_school_selecter())->getWidget());
    ui->dmg4Layout->addWidget((wpndmgtype[3] = new damage_school_selecter())->getWidget());
    ui->dmg5Layout->addWidget((wpndmgtype[4] = new damage_school_selecter())->getWidget());
    ui->dmgcapEdit->setValidator(new QDoubleValidator());
    ui->desiredILvlEdit->setValidator(new QDoubleValidator());
    armorDamageModifier = 0;
    ui->dmg1Weight->setValidator(new QDoubleValidator());
    ui->dmg2Weight->setValidator(new QDoubleValidator());
    ui->dmg3Weight->setValidator(new QDoubleValidator());
    ui->dmg4Weight->setValidator(new QDoubleValidator());
    ui->dmg5Weight->setValidator(new QDoubleValidator());
    ui->dmg1Spread->setValidator(new QDoubleValidator(0.0f,1.0f,5));
    ui->dmg2Spread->setValidator(new QDoubleValidator(0.0f,1.0f,5));
    ui->dmg3Spread->setValidator(new QDoubleValidator(0.0f,1.0f,5));
    ui->dmg4Spread->setValidator(new QDoubleValidator(0.0f,1.0f,5));
    ui->dmg5Spread->setValidator(new QDoubleValidator(0.0f,1.0f,5));

    QObject::connect(ui->dmg1Weight,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg2Weight,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg3Weight,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg4Weight,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg5Weight,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg1Spread,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg2Spread,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg3Spread,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg4Spread,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->dmg5Spread,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->desiredILvlEdit,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(ui->atkspeedEdit,SIGNAL(textChanged(QString)),this,SLOT(onWeaponModification()));
    QObject::connect(qualityselecter->getWidget(),SIGNAL(currentIndexChanged(int)),this,SLOT(onWeaponModification()));
    QObject::connect(slotselecter->getWidget(),SIGNAL(currentIndexChanged(int)),this,SLOT(onWeaponModification()));
    QObject::connect(classselecter->getWidget(),SIGNAL(currentIndexChanged(int)),this,SLOT(onWeaponModification()));
    QObject::connect(subclassselecter->getWidget(),SIGNAL(currentIndexChanged(int)),this,SLOT(onWeaponModification()));


    /* Vendor panel */
    ui->buycountEdit->setValidator(new QIntValidator());
    ui->sellpriceEdit->setValidator(new QIntValidator());
    ui->buypriceEdit->setValidator(new QIntValidator());

    /* Profession panel */
    ui->totemLayout->addWidget((totemselecter = new totem_category_selecter{m_dbccache})->getWidget());
    ui->delootLayout->addWidget((delootselecter = new disenchant_loot_selecter{m_dbcache})->getWidget());

    /* Bag panel */
    ui->bagslotsEdit->setValidator(new QIntValidator());
    ui->bagfamilyGB->layout()->addWidget((bagfamilyselecter = new bag_family_mask_selecter())->getWidget());
    ui->lockLayout->addWidget((lockselecter = new lock_selecter{m_dbcache})->getWidget());

    /* Misc panel */
    ui->ammotypeLayout->addWidget((ammotypeselecter = new ammo_type_selecter())->getWidget());
    ui->foodtypeLayout->addWidget((foodtypeselecter = new food_type_selecter())->getWidget());
    ui->gemtypeLayout->addWidget((gemselecter = new gem_selecter{m_dbccache})->getWidget());
    ui->materialLayout->addWidget((materialselecter = new material_selecter())->getWidget());
    ui->maxcountEdit->setValidator(new QIntValidator());
    ui->stackableEdit->setValidator(new QIntValidator());
    ui->pagetextGB->layout()->addWidget((bookselecter = new selecter_button{new book_selecter_dialog{ims->get_book_model(),ims->get_resources()},0})->getButton());
    ui->languageLayout->addWidget((languageselecter = new language_selecter{m_dbccache})->getWidget());
    ui->languageLayout->addWidget((pagematerialselecter = new page_material_selecter())->getWidget());
    ui->durationEdit->setValidator(new QIntValidator());
    // Treat field "RangedModRange" as 100 for ranged weapons and 0 otherwise.

    /* Flags panel */
    ui->flagsLayout->addWidget((flagsselecter = new item_flags_selecter())->getWidget());

    QObject::connect(itemselecter,SIGNAL(selection()),this,SLOT(onLoadEntry()));
}

itemtemplateedit::~itemtemplateedit()
{
    delete ui;
}

double itemtemplateedit::ilvlToDps(double x)
{
    double magnitude = 2044.0f;
    double omega = 3.14150265/20000;
    double min = 0.01f;
    double phase = -0.02748893525f - 3.14150265/2;   // shifts such that sin reaches its minumum at x = 175

    if(x <= 0)
    {
        return 0;
    }
    else if(x > 0 && x <= 175)
    {
        return min + magnitude*sin(omega*x);
    }
    else if(x > 175 && x < 5000)
    {
        return min + magnitude*(sin(omega*x) + 10*(sin(omega*x + phase) + 1));
    }
    else if(x >= 5000)
    {
        return 7040 + 3*(x-5000);
    }
    else
    {
        return 0;
    }
}

// dpsToIlvl(ilvlToDps(x)) == x
double itemtemplateedit::dpsToIlvl(double x)
{
    double magnitude = 2044.0f;
    double omega = 3.14150265/20000;
    double min = 0.01f;
    double phase = -0.02748893525f - 3.14150265/2;   // shifts such that sin reaches its minumum at x = 175
    if(x < 0)
    {
        return 0;
    }
    else if(x > 0 && x <= 56.1887)
    {
        return asin((x - min) / magnitude) / omega;
    }
    else if(x > 56.1887 && x < 7040)
    {
        double ilvl = 200;
        double bottom = 100;
        double top = 5000;
        double error = ilvlToDps(ilvl)-x;
        while(error > 0.2f || error < -0.2f)
        {
            if(error > 0)
            {
                top = ilvl;
                ilvl = (bottom+top)/2;
                error = ilvlToDps(ilvl)-x;
            }
            else if(error < 0)
            {
                bottom = ilvl;
                ilvl = (bottom+top)/2;
                error = ilvlToDps(ilvl)-x;
            }
        }
        return ilvl;
    }
    return 0;
}

void itemtemplateedit::initWeaponView()
{
    // Initialize Weights/Spreads and desiredIlvl
    float avg1 = (dmgMin[0] + dmgMax[0])/2;
    float avg2 = (dmgMin[1] + dmgMax[1])/2;
    float avg3 = (dmgMin[2] + dmgMax[2])/2;
    float avg4 = (dmgMin[3] + dmgMax[3])/2;
    float avg5 = (dmgMin[4] + dmgMax[4])/2;
    float atkSpd = 0;
    if(ui->atkspeedEdit->text().length() != 0){ atkSpd = ui->atkspeedEdit->text().toFloat()/1000; }
    float dps = (avg1+avg2+avg3+avg4+avg5)/atkSpd;
    ui->showdpsLabel->setText(QString::number(dps));
    // The average for each attack will work as a weight
    float s1,s2,s3,s4,s5;
    if(avg1 > 0.01){ s1 = (avg1-dmgMin[0])/avg1;}else{s1=0;}
    if(avg2 > 0.01){ s2 = (avg2-dmgMin[1])/avg2;}else{s2=0;}
    if(avg3 > 0.01){ s3 = (avg3-dmgMin[2])/avg3;}else{s3=0;}
    if(avg4 > 0.01){ s4 = (avg4-dmgMin[3])/avg4;}else{s4=0;}
    if(avg5 > 0.01){ s5 = (avg5-dmgMin[4])/avg5;}else{s5=0;}
    float normalizedDps = 0; // Normalize to equivalent of 1h-dmg
    // Twohand
    if(slotselecter->selectedId() == 17)
    {
        normalizedDps = dps/1.3;
    }
    // One-Hand
    else if(slotselecter->selectedId() == 13 || slotselecter->selectedId() == 21 || slotselecter->selectedId() == 22)
    {
        normalizedDps = dps;
    }
    // Ranged
    else if(slotselecter->selectedId() == 15 || slotselecter->selectedId() == 25 ||
            ((classselecter->selectedId() == 2) &&          // weapon
            ((subclassselecter->selectedId() == 2) ||       // bow
            (subclassselecter->selectedId() == 3)  ||       // gun
            (subclassselecter->selectedId() == 16) ||       // thrown
            (subclassselecter->selectedId() == 18))))       // crossbow
    {
        normalizedDps = dps*1.1;
    }
    // Wands
    else if((classselecter->selectedId() == 2) && (subclassselecter->selectedId() == 19))
    {
        normalizedDps = dps*0.544152745f;
    }
    float desiredILvl;
    switch(qualityselecter->selectedId())
    {
    case 0:// Poor
        //dps = ilvlToDps(ilvl), ilvl = dpsToIlvl(dps)
        desiredILvl = dpsToIlvl(normalizedDps);
        break;
    case 1:// Common
        //dps = ilvlToDps(ilvl*1.75-2), ilvl = (dpsToIlvl(dps)+2)/1.75
        desiredILvl = (dpsToIlvl(normalizedDps)+2)/1.75;
        break;
    case 2:// Uncommon
        //dps = ilvlToDps(ilvl*1.75+1), ilvl = (dpsToIlvl(dps)-1)/1.75
        desiredILvl = (dpsToIlvl(normalizedDps)-1)/1.75;
        break;
    case 3: // Rare
        //dps = ilvlToDps(ilvl*1.9+4), ilvl = (dpsToIlvl(dps)-4)/1.9
        desiredILvl = (dpsToIlvl(normalizedDps)-4)/1.9;
        break;
        // treat legendary/artifact/epic as same dps potential (maybe change this later?)
    case 6:
    case 5:
    case 4://Epic,Legendary,Artifact
        //dps = ilvlToDps(ilvl*2.2+12), ilvl = (dpsToIlvl(dps)-12)/2.2
        desiredILvl = (dpsToIlvl(normalizedDps)-12)/2.2;
        break;
    }
    ui->dmg1Weight->setText(QString::number(avg1));
    ui->dmg2Weight->setText(QString::number(avg2));
    ui->dmg3Weight->setText(QString::number(avg3));
    ui->dmg4Weight->setText(QString::number(avg4));
    ui->dmg5Weight->setText(QString::number(avg5));
    ui->dmg1Spread->setText(QString::number(s1));
    ui->dmg2Spread->setText(QString::number(s2));
    ui->dmg3Spread->setText(QString::number(s3));
    ui->dmg4Spread->setText(QString::number(s4));
    ui->dmg5Spread->setText(QString::number(s5));
    ui->desiredILvlEdit->setText(QString::number(desiredILvl));
    ui->showdpsLabel->setText(QString::number(dps));
    updateWeaponView();
}

void itemtemplateedit::updateWeaponView()
{
    float w1,w2,w3,w4,w5,s1,s2,s3,s4,s5;
    if(ui->dmg1Weight->text().length() != 0){w1 = ui->dmg1Weight->text().toFloat();}
    else{w1=0;}
    if(ui->dmg2Weight->text().length() != 0){w2 = ui->dmg2Weight->text().toFloat();}
    else{w2=0;}
    if(ui->dmg3Weight->text().length() != 0){w3 = ui->dmg3Weight->text().toFloat();}
    else{w3=0;}
    if(ui->dmg4Weight->text().length() != 0){w4 = ui->dmg4Weight->text().toFloat();}
    else{w4=0;}
    if(ui->dmg5Weight->text().length() != 0){w5 = ui->dmg5Weight->text().toFloat();}
    else{w5=0;}
    if(ui->dmg1Spread->text().length() != 0){s1 = ui->dmg1Spread->text().toFloat();}
    else{s1=0;}
    if(ui->dmg2Spread->text().length() != 0){s2 = ui->dmg2Spread->text().toFloat();}
    else{s2=0;}
    if(ui->dmg3Spread->text().length() != 0){s3 = ui->dmg3Spread->text().toFloat();}
    else{s3=0;}
    if(ui->dmg4Spread->text().length() != 0){s4 = ui->dmg4Spread->text().toFloat();}
    else{s4=0;}
    if(ui->dmg5Spread->text().length() != 0){s5 = ui->dmg5Spread->text().toFloat();}
    else{s5=0;}
    float totalWeight = w1+w2+w3+w4+w5;
    float desiredILvl;
    float attackPeriod;
    if(ui->desiredILvlEdit->text().length() != 0)
    {
        desiredILvl = ui->desiredILvlEdit->text().toFloat();
    }
    else{ desiredILvl = 0; }
    if(ui->atkspeedEdit->text().length() != 0)
    {
        attackPeriod = ui->atkspeedEdit->text().toFloat()/1000;
    }
    else{ attackPeriod = 0; }
    float iLvlDesiredDps = 0;
    switch(qualityselecter->selectedId())
    {
    case 0: // Poor
        iLvlDesiredDps = ilvlToDps(desiredILvl);
        break;
    case 1: // Uncommon
        iLvlDesiredDps = ilvlToDps(desiredILvl*1.75 - 2);
        break;
    case 2: // Common
        iLvlDesiredDps = ilvlToDps(desiredILvl*1.75 + 1);
        break;
    case 3:
        iLvlDesiredDps = ilvlToDps(desiredILvl*1.9 + 4);
        break;
        // treat legendary/artifact/epic as same dps potential (maybe change this later?)
    case 6:
    case 5:
    case 4:
        iLvlDesiredDps = ilvlToDps(desiredILvl*2.2 + 12);
        break;
    }

    // Twohand
    if(slotselecter->selectedId() == 17)
    {
        iLvlDesiredDps = iLvlDesiredDps * 1.3;
    }
    // One-Hand
    else if(slotselecter->selectedId() == 13 || slotselecter->selectedId() == 21 || slotselecter->selectedId() == 22)
    {
        iLvlDesiredDps = iLvlDesiredDps;
    }
    // Ranged
    else if(slotselecter->selectedId() == 15 || slotselecter->selectedId() == 25 ||
            ((classselecter->selectedId() == 2) &&          // weapon
            ((subclassselecter->selectedId() == 2) ||       // bow
            (subclassselecter->selectedId() == 3)  ||       // gun
            (subclassselecter->selectedId() == 16) ||       // thrown
            (subclassselecter->selectedId() == 18))))       // crossbow
    {
        iLvlDesiredDps = iLvlDesiredDps/1.1;
    }
    else if((classselecter->selectedId() == 2) &&(subclassselecter->selectedId() == 19))
    {
        iLvlDesiredDps = iLvlDesiredDps/0.544152745f;
    }
    else
    {
        // If its not a weapon being edited, dont do anything
        return;
    }
    float armrDmgMod;
    if(ui->dmgcapEdit->text().length() == 0)
    {
        armrDmgMod = 0;
    }
    else
    {
        armrDmgMod = ui->dmgcapEdit->text().toFloat() - iLvlDesiredDps;
    }
    float f1 = w1 / totalWeight;
    float f2 = w2 / totalWeight;
    float f3 = w3 / totalWeight;
    float f4 = w4 / totalWeight;
    float f5 = w5 / totalWeight;
    float dps1 = iLvlDesiredDps*f1;
    float dps2 = iLvlDesiredDps*f2;
    float dps3 = iLvlDesiredDps*f3;
    float dps4 = iLvlDesiredDps*f4;
    float dps5 = iLvlDesiredDps*f5;
    float dmg1 = dps1*attackPeriod;
    float dmg2 = dps2*attackPeriod;
    float dmg3 = dps3*attackPeriod;
    float dmg4 = dps4*attackPeriod;
    float dmg5 = dps5*attackPeriod;
    float min1 = (dmg1 - dmg1*s1);
    float min2 = (dmg2 - dmg2*s2);
    float min3 = (dmg3 - dmg3*s3);
    float min4 = (dmg4 - dmg4*s4);
    float min5 = (dmg5 - dmg5*s5);
    float max1 = (dmg1 + dmg1*s1);
    float max2 = (dmg2 + dmg2*s2);
    float max3 = (dmg3 + dmg3*s3);
    float max4 = (dmg4 + dmg4*s4);
    float max5 = (dmg5 + dmg5*s5);

    float avg1 = (min1 + max1)/2;
    float avg2 = (min2 + max2)/2;
    float avg3 = (min3 + max3)/2;
    float avg4 = (min4 + max4)/2;
    float avg5 = (min5 + max5)/2;
    QString presentation = "";
    bool hasPrev = false;
    if(avg1 > 0.1)
    {
        presentation += QString::number(min1)+"-"+QString::number(max1) + " " + wpndmgtype[0]->selectedString() + " Damage";
    }
    if(avg2 > 0.1)
    {
        presentation += QString::number(min2)+"-"+QString::number(max2) + " " + wpndmgtype[1]->selectedString() + " Damage";
    }
    if(avg3 > 0.1)
    {
        presentation += QString::number(min3)+"-"+QString::number(max3) + " " + wpndmgtype[2]->selectedString() + " Damage";
    }
    if(avg4 > 0.1)
    {
        presentation += QString::number(min4)+"-"+QString::number(max4) + " " + wpndmgtype[3]->selectedString() + " Damage";
    }
    if(avg5 > 0.1)
    {
        presentation += QString::number(min5)+"-"+QString::number(max5) + " " + wpndmgtype[4]->selectedString() + " Damage";
    }

    ui->showdmgLabel->setText(presentation);
    ui->showdpsLabel->setText(QString::number(iLvlDesiredDps));
    dmgMin[0] = min1;
    dmgMin[1] = min2;
    dmgMin[2] = min3;
    dmgMin[3] = min4;
    dmgMin[4] = min5;
    dmgMax[0] = max1;
    dmgMax[1] = max2;
    dmgMax[2] = max3;
    dmgMax[3] = max4;
    dmgMax[4] = max5;
}

void itemtemplateedit::onWeaponModification()
{
    int a = slotselecter->selectedId();
    int b = classselecter->selectedId();
    if((a == 13 || a == 15 || a == 17 || a == 21 || a == 22 || a == 25 || a == 26)
            ||
       (b == 2))
    {
        updateWeaponView();
    }
    else
    {
        ui->showdmgLabel->setText("");
        ui->showdpsLabel->setText("");
    }
}

void itemtemplateedit::onItemClassChange()
{
    if(classselecter->isDefaultSelected())
    {
        subclassselecter->setCategory(-1);
        subclassselecter->getWidget()->setEnabled(false);
        return;
    }
    subclassselecter->getWidget()->clear();
    subclassselecter->setCategory(classselecter->selectedId());
    subclassselecter->getWidget()->setEnabled(true);
}

void itemtemplateedit::on_spell1Clr_clicked()
{
    spell1selecter->clear();
    ui->spell1cag->setText("Category...");
    ui->spell1cagcd->clear();
}

void itemtemplateedit::on_spell2Clr_clicked()
{
    spell2selecter->clear();
    ui->spell2cag->setText("Category...");
    ui->spell2cagcd->clear();
}

void itemtemplateedit::on_spell3Clr_clicked()
{
    spell3selecter->clear();
    ui->spell3cag->setText("Category...");
    ui->spell3cagcd->clear();
}

void itemtemplateedit::on_spell4Clr_clicked()
{
    spell4selecter->clear();
    ui->spell4cag->setText("Category...");
    ui->spell4cagcd->clear();
}

void itemtemplateedit::on_spell5Clr_clicked()
{
    spell5selecter->clear();
    ui->spell5cag->setText("Category...");
    ui->spell5cagcd->clear();
}

void itemtemplateedit::onSpell1Selection()
{
    int a = spell1selecter->selectedId();
    int b = static_cast<entityviews::spellcategory*>(&module)->find_ref(a);
    ui->spell1cag->setText(QString::number(static_cast<entityviews::spellcategory*>(&module)->get_label(b)));
    ui->spell1cagcd->clear();
}

void itemtemplateedit::onSpell2Selection()
{
    int a = spell2selecter->selectedId();
    int b = static_cast<entityviews::spellcategory*>(&module)->find_ref(a);
    ui->spell2cag->setText(QString::number(static_cast<entityviews::spellcategory*>(&module)->get_label(b)));
    ui->spell2cagcd->clear();
}
void itemtemplateedit::onSpell3Selection()
{
    int a = spell3selecter->selectedId();
    int b = static_cast<entityviews::spellcategory*>(&module)->find_ref(a);
    ui->spell3cag->setText(QString::number(static_cast<entityviews::spellcategory*>(&module)->get_label(b)));
    ui->spell3cagcd->clear();
}
void itemtemplateedit::onSpell4Selection()
{
    int a = spell4selecter->selectedId();
    int b = static_cast<entityviews::spellcategory*>(&module)->find_ref(a);
    ui->spell4cag->setText(QString::number(static_cast<entityviews::spellcategory*>(&module)->get_label(b)));
    ui->spell4cagcd->clear();
}
void itemtemplateedit::onSpell5Selection()
{
    int a = spell5selecter->selectedId();
    int b = static_cast<entityviews::spellcategory*>(&module)->find_ref(a);
    ui->spell5cag->setText(QString::number(static_cast<entityviews::spellcategory*>(&module)->get_label(b)));
    ui->spell5cagcd->clear();
}

void itemtemplateedit::on_spinBox_valueChanged(int arg1)
{
    for(int i = 0; i < 10; i++)
    {
        if(i < arg1)
        {
            stats[i]->getWidget()->setEnabled(true);
            statsamount[i]->setEnabled(true);
        }
        else
        {
            stats[i]->getWidget()->setEnabled(false);
            statsamount[i]->setEnabled(false);
        }
    }
}

void itemtemplateedit::onItemSlotChange()
{
    modelselect->changeSlot(item_model_selecter::dbcSlotToSlot(slotselecter->selectedId()));
}

void itemtemplateedit::onPropertyChange()
{
    if(randompropertyselecter->selectedId() == 0)
    {
        return;
    }
    randomsuffixselecter->setIndexFromId(0);
}

void itemtemplateedit::onSuffixChange()
{
    if(randomsuffixselecter->selectedId() == 0)
    {
        return;
    }
    randompropertyselecter->setIndexFromId(0);
}

void itemtemplateedit::onLoadEntry()
{
    int tmp;
    int entry = itemselecter->selectedId();
    QSqlQuery q = db.exec("SELECT entry,class,subclass,unk0,name,displayid,Quality,Flags,BuyCount,BuyPrice,SellPrice"
                          ",InventoryType,AllowableClass,AllowableRace,ItemLevel,RequiredLevel,RequiredSkill,"
                          "RequiredSkillRank,requiredspell,requiredhonorrank,RequiredCityRank,RequiredReputationFaction"
                          ",RequiredReputationRank,maxcount,stackable,ContainerSlots,stat_type1,stat_value1,stat_type2,"
                          "stat_value2,stat_type3,stat_value3,stat_type4,stat_value4,stat_type5,stat_value5"
                          ",stat_type6,stat_value6,stat_type7,stat_value7,stat_type8,stat_value8,stat_type9,stat_value9"
                          ",stat_type10,stat_value10,dmg_min1,dmg_max1,dmg_type1,dmg_min2,dmg_max2,dmg_type2,"
                          "dmg_min3,dmg_max3,dmg_type3,dmg_min4,dmg_max4,dmg_type4,dmg_min5,dmg_max5,dmg_type5,"
                          "armor,holy_res,fire_res,nature_res,frost_res,shadow_res,arcane_res,delay,ammo_type,"
                          "RangedModRange,spellid_1,spelltrigger_1,spellcharges_1,spellppmRate_1,spellcooldown_1,spellcategory_1,spellcategorycooldown_1,"
                          "spellid_2,spelltrigger_2,spellcharges_2,spellppmRate_2,spellcooldown_2,spellcategory_2,spellcategorycooldown_2,"
                          "spellid_3,spelltrigger_3,spellcharges_3,spellppmRate_3,spellcooldown_3,spellcategory_3,spellcategorycooldown_3,"
                          "spellid_4,spelltrigger_4,spellcharges_4,spellppmRate_4,spellcooldown_4,spellcategory_4,spellcategorycooldown_4,"
                          "spellid_5,spelltrigger_5,spellcharges_5,spellppmRate_5,spellcooldown_5,spellcategory_5,spellcategorycooldown_5,"
                          "bonding,description,PageText,LanguageID,PageMaterial,startquest,lockid,Material,sheath,"
                          "RandomProperty,RandomSuffix,block,itemset,MaxDurability,area,Map,BagFamily,TotemCategory,"
                          "socketColor_1,socketContent_1,socketColor_2,socketContent_2,socketColor_3,socketContent_3,"
                          "socketBonus,GemProperties,RequiredDisenchantSkill,ArmorDamageModifier,ScriptName,"
                          "DisenchantID,FoodType,minMoneyLoot,maxMoneyLoot,Duration,ExtraFlags FROM item_template WHERE entry"
                          " = " + QString::number(entry) + ";");
    bool exists = q.next();
    if(exists)
    {
        QSqlRecord r = q.record();
        classselecter->             setIndexFromId(r.value(1).toInt());
        subclassselecter->          setIndexFromId(r.value(2).toInt());
        ui->nameEdit->              setText(r.value(4).toString());
        slotselecter->              setIndexFromId(r.value(11).toInt());
        modelselect->               setIndexFromDisplayId(r.value(5).toInt());
        qualityselecter->           setIndexFromId(r.value(6).toInt());
        flagsselecter->             setIndexFromId(r.value(7).toInt());
        ui->buycountEdit->          setText(QString::number(r.value(8).toInt()));
        ui->buypriceEdit->          setText(QString::number(r.value(9).toInt()));
        ui->sellpriceEdit->         setText(QString::number(r.value(10).toInt()));
        classmask->                 setIndexFromId(r.value(12).toInt());
        racemask->                  setIndexFromId(r.value(13).toInt());
        ui->ilvlEdit->              setText(QString::number(r.value(14).toInt()));
        ui->reqlvlEdit->            setText(QString::number(r.value(15).toInt()));
        skillselecter->             setIndexFromId(r.value(16).toInt());
        ui->reqskillrankEdit->      setText(QString::number(r.value(17).toInt()));
        spellselecter->             setIndexFromId(r.value(18).toInt());
        honorselecter->             setIndexFromId(r.value(19).toInt());
        repfactionselecter->        setIndexFromId(r.value(21).toInt());
        reprankselecter->           setIndexFromId(r.value(22).toInt());
        if(repfactionselecter->selectedId() == 0){ reprankselecter->clear(); }
        ui->maxcountEdit->          setText(QString::number(r.value(23).toInt()));
        ui->stackableEdit->         setText(QString::number(r.value(24).toInt()));
        ui->bagslotsEdit->          setText(QString::number(r.value(25).toInt()));
        stats[0]->                  setIndexFromId(r.value(26).toInt());
        statsamount[0]->            setText(QString::number(r.value(27).toInt()));
        if(r.value(27).toInt()==0){ stats[0]->clear(); }
        stats[1]->                  setIndexFromId(r.value(28).toInt());
        statsamount[1]->            setText(QString::number(r.value(29).toInt()));
        if(r.value(29).toInt()==0){ stats[1]->clear(); }
        stats[2]->                  setIndexFromId(r.value(30).toInt());
        statsamount[2]->            setText(QString::number(r.value(31).toInt()));
        if(r.value(31).toInt()==0){ stats[2]->clear(); }
        stats[3]->                  setIndexFromId(r.value(32).toInt());
        statsamount[3]->            setText(QString::number(r.value(33).toInt()));
        if(r.value(33).toInt()==0){ stats[3]->clear(); }
        stats[4]->                  setIndexFromId(r.value(34).toInt());
        statsamount[4]->            setText(QString::number(r.value(35).toInt()));
        if(r.value(35).toInt()==0){ stats[4]->clear(); }
        stats[5]->                  setIndexFromId(r.value(36).toInt());
        statsamount[5]->            setText(QString::number(r.value(37).toInt()));
        if(r.value(37).toInt()==0){ stats[5]->clear(); }
        stats[6]->                  setIndexFromId(r.value(38).toInt());
        statsamount[6]->            setText(QString::number(r.value(39).toInt()));
        if(r.value(39).toInt()==0){ stats[6]->clear(); }
        stats[7]->                  setIndexFromId(r.value(40).toInt());
        statsamount[7]->            setText(QString::number(r.value(41).toInt()));
        if(r.value(41).toInt()==0){ stats[7]->clear(); }
        stats[8]->                  setIndexFromId(r.value(42).toInt());
        statsamount[8]->            setText(QString::number(r.value(43).toInt()));
        if(r.value(43).toInt()==0){ stats[8]->clear(); }
        stats[9]->                  setIndexFromId(r.value(44).toInt());
        statsamount[9]->            setText(QString::number(r.value(45).toInt()));
        if(r.value(45).toInt()==0){ stats[9]->clear(); }
        dmgMin[0] =                 r.value(46).toFloat();
        dmgMax[0] =                 r.value(47).toFloat();
        wpndmgtype[0]->             setIndexFromId(r.value(48).toInt());
        dmgMin[1] =                 r.value(49).toFloat();
        dmgMax[1] =                 r.value(50).toFloat();
        wpndmgtype[1]->             setIndexFromId(r.value(51).toInt());
        dmgMin[2] =                 r.value(52).toFloat();
        dmgMax[2] =                 r.value(53).toFloat();
        wpndmgtype[2]->             setIndexFromId(r.value(54).toInt());
        dmgMin[3] =                 r.value(55).toFloat();
        dmgMax[3] =                 r.value(56).toFloat();
        wpndmgtype[3]->             setIndexFromId(r.value(57).toInt());
        dmgMin[4] =                 r.value(58).toFloat();
        dmgMax[4] =                 r.value(59).toFloat();
        wpndmgtype[4]->             setIndexFromId(r.value(60).toInt());
        ui->armorEdit->             setText(QString::number(r.value(61).toInt()));
        ui->holyresEdit->           setText(QString::number(r.value(62).toInt()));
        ui->fireresEdit->           setText(QString::number(r.value(63).toInt()));
        ui->natureresEdit->         setText(QString::number(r.value(64).toInt()));
        ui->frostresEdit->          setText(QString::number(r.value(65).toInt()));
        ui->shadowresEdit->         setText(QString::number(r.value(66).toInt()));
        ui->arcaneresEdit->         setText(QString::number(r.value(67).toInt()));
        ui->atkspeedEdit->          setText(QString::number(r.value(68).toInt()));
        ammotypeselecter->          setIndexFromId(r.value(69).toInt());
        tmp = r.value(71).toInt();
        spell1selecter->            setIndexFromId(tmp);
        if(tmp == 0){ spelltriggerselecter[0]->clear(); }
        else        { spelltriggerselecter[0]->   setIndexFromId(r.value(72).toInt()); }
        ui->spell1Charges->         setText(QString::number(r.value(73).toInt()));
        ui->spell1ppm->             setText(QString::number(r.value(74).toInt()));
        ui->spell1cd->              setText(QString::number(r.value(75).toInt()));
        ui->spell1cag->             setText(QString::number(r.value(76).toInt()));
        ui->spell1cagcd->           setText(QString::number(r.value(77).toInt()));
        tmp = r.value(78).toInt();
        spell2selecter->            setIndexFromId(tmp);
        if(tmp == 0){ spelltriggerselecter[1]->clear(); }
        else        { spelltriggerselecter[1]->   setIndexFromId(r.value(79).toInt()); }
        ui->spell2Charges->         setText(QString::number(r.value(80).toInt()));
        ui->spell2ppm->             setText(QString::number(r.value(81).toInt()));
        ui->spell2cd->              setText(QString::number(r.value(82).toInt()));
        ui->spell2cag->             setText(QString::number(r.value(83).toInt()));
        ui->spell2cagcd->           setText(QString::number(r.value(84).toInt()));
        tmp = r.value(85).toInt();
        spell3selecter->            setIndexFromId(tmp);
        if(tmp == 0){ spelltriggerselecter[2]->clear(); }
        else        { spelltriggerselecter[2]->   setIndexFromId(r.value(86).toInt()); }
        ui->spell3Charges->         setText(QString::number(r.value(87).toInt()));
        ui->spell3ppm->             setText(QString::number(r.value(88).toInt()));
        ui->spell3cd->              setText(QString::number(r.value(89).toInt()));
        ui->spell3cag->             setText(QString::number(r.value(90).toInt()));
        ui->spell3cagcd->           setText(QString::number(r.value(91).toInt()));
        tmp = r.value(92).toInt();
        spell4selecter->            setIndexFromId(tmp);
        if(tmp == 0){ spelltriggerselecter[3]->clear(); }
        else        { spelltriggerselecter[3]->   setIndexFromId(r.value(93).toInt()); }
        ui->spell4Charges->         setText(QString::number(r.value(94).toInt()));
        ui->spell4ppm->             setText(QString::number(r.value(95).toInt()));
        ui->spell4cd->              setText(QString::number(r.value(96).toInt()));
        ui->spell4cag->             setText(QString::number(r.value(97).toInt()));
        ui->spell4cagcd->           setText(QString::number(r.value(98).toInt()));
        tmp = r.value(99).toInt();
        spell5selecter->            setIndexFromId(tmp);
        if(tmp == 0){ spelltriggerselecter[4]->clear(); }
        else        { spelltriggerselecter[4]->   setIndexFromId(r.value(100).toInt()); }
        ui->spell5Charges->         setText(QString::number(r.value(101).toInt()));
        ui->spell5ppm->             setText(QString::number(r.value(102).toInt()));
        ui->spell5cd->              setText(QString::number(r.value(103).toInt()));
        ui->spell5cag->             setText(QString::number(r.value(104).toInt()));
        ui->spell5cagcd->           setText(QString::number(r.value(105).toInt()));
        ui->bondingCB->             setCurrentIndex(r.value(106).toInt());
        ui->descEdit->              setText(r.value(107).toString());
        bookselecter->              setId(r.value(108).toInt());
        languageselecter->          setIndexFromId(r.value(109).toInt());
        pagematerialselecter->      setIndexFromId(r.value(110).toInt());
        // implement quest selecter for field 111
        lockselecter->              setIndexFromId(r.value(112).toInt());
        materialselecter->          setIndexFromId(r.value(113).toInt());
        sheatheselecter->           setIndexFromId(r.value(114).toInt());
        randompropertyselecter->    setIndexFromId(r.value(115).toInt());
        randomsuffixselecter->      setIndexFromId(r.value(116).toInt());
        ui->blockEdit->             setText(QString::number(r.value(117).toInt()));
        itemsetselecter->           setIndexFromId(r.value(118).toInt());
        ui->durabilityEdit->        setText(QString::number(r.value(119).toInt()));
        reqareaselecter->           setIndexFromId(r.value(120).toInt());
        reqmapselecter->            setIndexFromId(r.value(121).toInt());
        bagfamilyselecter->         setIndexFromId(r.value(122).toInt());
        totemselecter->             setIndexFromId(r.value(123).toInt());
        socketselecter[0]->         setIndexFromId(r.value(124).toInt());
        socketselecter[1]->         setIndexFromId(r.value(126).toInt());
        socketselecter[2]->         setIndexFromId(r.value(128).toInt());
        socketbonusselecter->       setIndexFromId(r.value(130).toInt());
        gemselecter->               setIndexFromId(r.value(131).toInt());
        ui->deskillEdit->           setText(QString::number(r.value(132).toInt()));
        armorDamageModifier =       r.value(133).toInt();
        delootselecter->            setIndexFromId(r.value(135).toInt());
        foodtypeselecter->          setIndexFromId(r.value(136).toInt());
        ui->minmoneyEdit->          setText(QString::number(r.value(137).toInt()));
        ui->maxmoneyEdit->          setText(QString::number(r.value(138).toInt()));
        ui->durationEdit->          setText(QString::number(r.value(139).toInt()));
        ui->realtimeCheck->         setChecked((r.value(140).toInt() & 2) != 0);
        initWeaponView();
    }
    else
    {

    }
}

QString itemtemplateedit::currentQueryFields()
{
    QString tmp = "";
    tmp += "entry,class,subclass,unk0,name,";
    tmp += "displayid,Quality,Flags,BuyCount,BuyPrice,";
    tmp += "SellPrice,InventoryType,AllowableClass,AllowableRace,";
    tmp += "ItemLevel,RequiredLevel,RequiredSkill,RequiredSkillRank,";
    tmp += "requiredspell,requiredhonorrank,RequiredCityRank,";
    tmp += "RequiredReputationFaction,RequiredReputationRank,maxcount,";
    tmp += "stackable,ContainerSlots,stat_type1,stat_value1,";
    tmp += "stat_type2,stat_value2,stat_type3,stat_value3,";
    tmp += "stat_type4,stat_value4,stat_type5,stat_value5,";
    tmp += "stat_type6,stat_value6,stat_type7,stat_value7,";
    tmp += "stat_type8,stat_value8,stat_type9,stat_value9,";
    tmp += "stat_type10,stat_value10,dmg_min1,dmg_max1,dmg_type1,";
    tmp += "dmg_min2,dmg_max2,dmg_type2,dmg_min3,dmg_max3,dmg_type3,";
    tmp += "dmg_min4,dmg_max4,dmg_type4,dmg_min5,dmg_max5,dmg_type5,";
    tmp += "armor,holy_res,fire_res,nature_res,frost_res,";
    tmp += "shadow_res,arcane_res,delay,ammo_type,";
    tmp += "RangedModRange,spellid_1,spelltrigger_1,spellcharges_1,spellppmRate_1,spellcooldown_1,spellcategory_1,spellcategorycooldown_1,";
    tmp += "spellid_2,spelltrigger_2,spellcharges_2,spellppmRate_2,spellcooldown_2,spellcategory_2,spellcategorycooldown_2,";
    tmp += "spellid_3,spelltrigger_3,spellcharges_3,spellppmRate_3,spellcooldown_3,spellcategory_3,spellcategorycooldown_3,";
    tmp += "spellid_4,spelltrigger_4,spellcharges_4,spellppmRate_4,spellcooldown_4,spellcategory_4,spellcategorycooldown_4,";
    tmp += "spellid_5,spelltrigger_5,spellcharges_5,spellppmRate_5,spellcooldown_5,spellcategory_5,spellcategorycooldown_5,";
    tmp += "bonding,description,PageText,LanguageID,PageMaterial,";
    tmp += "startquest,lockid,Material,sheath,RandomProperty,";
    tmp += "RandomSuffix,block,itemset,MaxDurability,area,";
    tmp += "Map,BagFamily,TotemCategory,socketColor_1,socketContent_1,";
    tmp += "socketColor_2,socketContent_2,socketColor_3,socketContent_3,";
    tmp += "socketBonus,GemProperties,RequiredDisenchantSkill,ArmorDamageModifier,";
    tmp += "ScriptName,DisenchantID,FoodType,minMoneyLoot,";
    tmp += "maxMoneyLoot,Duration,ExtraFlags";
    return tmp;
}

void itemtemplateedit::setEditStatsDefault()
{
    for(int i = 0; i < 10; i++)
    {
        if(statsamount[i]->text().length() == 0)    { statsamount[i]->setText("0"); }
    }
    if(ui->armorEdit->text().length() == 0)         { ui->armorEdit->setText("0"); }
    if(ui->holyresEdit->text().length() == 0)       { ui->holyresEdit->setText("0"); }
    if(ui->fireresEdit->text().length() == 0)       { ui->fireresEdit->setText("0"); }
    if(ui->natureresEdit->text().length() == 0)     { ui->natureresEdit->setText("0"); }
    if(ui->frostresEdit->text().length() == 0)      { ui->frostresEdit->setText("0"); }
    if(ui->shadowresEdit->text().length() == 0)     { ui->shadowresEdit->setText("0"); }
    if(ui->arcaneresEdit->text().length() == 0)     { ui->arcaneresEdit->setText("0"); }
    if(ui->blockEdit->text().length() == 0)         { ui->blockEdit->setText("0"); }
}

// When saving into the database, if some edits are not filled in, fill them with default values
void itemtemplateedit::setEditDefaults()
{
    bool ok;
    setEditStatsDefault();
    if(ui->buycountEdit->text().length() == 0)      { ui->buycountEdit->setText("1"); }
    if(ui->buypriceEdit->text().length() == 0)      { ui->buypriceEdit->setText("0"); }
    if(ui->sellpriceEdit->text().length() == 0)     { ui->sellpriceEdit->setText("0"); }
    if(ui->ilvlEdit->text().length() == 0)          { ui->ilvlEdit->setText("1"); }
    if(ui->reqlvlEdit->text().length() == 0)        { ui->reqlvlEdit->setText("0"); }
    if(ui->reqskillrankEdit->text().length() == 0)  { ui->reqskillrankEdit->setText("0"); }
    if(ui->maxcountEdit->text().length() == 0)      { ui->maxcountEdit->setText("0"); }
    if(ui->stackableEdit->text().length() == 0)     { ui->stackableEdit->setText("1"); }
    if(ui->bagslotsEdit->text().length() == 0)      { ui->bagslotsEdit->setText("0"); }
    if(ui->atkspeedEdit->text().length() == 0)      { ui->atkspeedEdit->setText("1000"); }
    if(ui->spell1Charges->text().length() == 0)     { ui->spell1Charges->setText("0"); }
    if(ui->spell1ppm->text().length() == 0)         { ui->spell1ppm->setText("0"); }
    if(ui->spell1cd->text().length() == 0)          { ui->spell1cd->setText("-1"); }
    ui->spell1cag->text().toInt(&ok);
    if(!ok)                                         { ui->spell1cag->setText("0"); }
    if(ui->spell1cagcd->text().length() == 0)       { ui->spell1cagcd->setText("0"); }
    if(ui->spell2Charges->text().length() == 0)     { ui->spell2Charges->setText("0"); }
    if(ui->spell2ppm->text().length() == 0)         { ui->spell2ppm->setText("0"); }
    if(ui->spell2cd->text().length() == 0)          { ui->spell2cd->setText("-1"); }
    ui->spell2cag->text().toInt(&ok);
    if(!ok)                                         { ui->spell2cag->setText("0"); }
    if(ui->spell2cagcd->text().length() == 0)       { ui->spell2cagcd->setText("0"); }
    if(ui->spell3Charges->text().length() == 0)     { ui->spell3Charges->setText("0"); }
    if(ui->spell3ppm->text().length() == 0)         { ui->spell3ppm->setText("0"); }
    if(ui->spell3cd->text().length() == 0)          { ui->spell3cd->setText("-1"); }
    ui->spell3cag->text().toInt(&ok);
    if(!ok)                                         { ui->spell3cag->setText("0"); }
    if(ui->spell3cagcd->text().length() == 0)       { ui->spell3cagcd->setText("0"); }
    if(ui->spell4Charges->text().length() == 0)     { ui->spell4Charges->setText("0"); }
    if(ui->spell4ppm->text().length() == 0)         { ui->spell4ppm->setText("0"); }
    if(ui->spell4cd->text().length() == 0)          { ui->spell4cd->setText("-1"); }
    ui->spell4cag->text().toInt(&ok);
    if(!ok)                                         { ui->spell4cag->setText("0"); }
    if(ui->spell4cagcd->text().length() == 0)       { ui->spell4cagcd->setText("0"); }
    if(ui->spell5Charges->text().length() == 0)     { ui->spell5Charges->setText("0"); }
    if(ui->spell5ppm->text().length() == 0)         { ui->spell5ppm->setText("0"); }
    if(ui->spell5cd->text().length() == 0)          { ui->spell5cd->setText("-1"); }
    ui->spell5cag->text().toInt(&ok);
    if(!ok)                                         { ui->spell5cag->setText("0"); }
    if(ui->spell5cagcd->text().length() == 0)       { ui->spell5cagcd->setText("0"); }
    if(ui->durabilityEdit->text().length() == 0)    { ui->durabilityEdit->setText("0"); }
    if(ui->deskillEdit->text().length() == 0)       { ui->deskillEdit->setText("-1"); }
    if(ui->minmoneyEdit->text().length() == 0)      { ui->minmoneyEdit->setText("0"); }
    if(ui->maxmoneyEdit->text().length() == 0)      { ui->maxmoneyEdit->setText("0"); }
    if(ui->durationEdit->text().length() == 0)      { ui->durationEdit->setText("0"); }
}

QString itemtemplateedit::currentQueryValues()
{
    // Decide everything but entry value here. Entry value depends on if its overwriting or adding.
    QString tmp = "";
    tmp += QString::number(classselecter->selectedId()) + ",";
    tmp += QString::number(subclassselecter->selectedId()) + ",";
    tmp += "-1,"; // unk0
    tmp += "\"" + ui->nameEdit->text() + "\",";
    tmp += QString::number(modelselect->selectedDisplayId()) + ",";
    tmp += QString::number(qualityselecter->selectedId()) + ",";
    tmp += QString::number(flagsselecter->selectedId()) + ",";
    tmp += ui->buycountEdit->text() + ",";
    tmp += ui->buypriceEdit->text() + ",";
    tmp += ui->sellpriceEdit->text() + ",";
    tmp += QString::number(slotselecter->selectedId()) + ",";
    tmp += QString::number(classmask->selectedId()) + ",";
    tmp += QString::number(racemask->selectedId()) + ",";
    tmp += ui->ilvlEdit->text() + ",";
    tmp += ui->reqlvlEdit->text() + ",";
    tmp += QString::number(skillselecter->selectedId()) + ",";
    tmp += ui->reqskillrankEdit->text() + ",";
    tmp += QString::number(spellselecter->selectedId()) + ",";
    tmp += QString::number(honorselecter->selectedId()) + ",";
    tmp += "0,";  // RequiredCityRank
    tmp += QString::number(repfactionselecter->selectedId()) + ",";
    tmp += QString::number(reprankselecter->selectedId()) + ",";
    tmp += ui->maxcountEdit->text() + ",";
    tmp += ui->stackableEdit->text() + ",";
    tmp += ui->bagslotsEdit->text() + ",";
    tmp += QString::number(stats[0]->selectedId()) + ",";
    tmp += statsamount[0]->text() + ",";
    tmp += QString::number(stats[1]->selectedId()) + ",";
    tmp += statsamount[1]->text() + ",";
    tmp += QString::number(stats[2]->selectedId()) + ",";
    tmp += statsamount[2]->text() + ",";
    tmp += QString::number(stats[3]->selectedId()) + ",";
    tmp += statsamount[3]->text() + ",";
    tmp += QString::number(stats[4]->selectedId()) + ",";
    tmp += statsamount[4]->text() + ",";
    tmp += QString::number(stats[5]->selectedId()) + ",";
    tmp += statsamount[5]->text() + ",";
    tmp += QString::number(stats[6]->selectedId()) + ",";
    tmp += statsamount[6]->text() + ",";
    tmp += QString::number(stats[7]->selectedId()) + ",";
    tmp += statsamount[7]->text() + ",";
    tmp += QString::number(stats[8]->selectedId()) + ",";
    tmp += statsamount[8]->text() + ",";
    tmp += QString::number(stats[9]->selectedId()) + ",";
    tmp += statsamount[9]->text() + ",";
    tmp += QString::number(dmgMin[0]) + ",";
    tmp += QString::number(dmgMax[0]) + ",";
    tmp += QString::number(wpndmgtype[0]->selectedId()) + ",";
    tmp += QString::number(dmgMin[1]) + ",";
    tmp += QString::number(dmgMax[1]) + ",";
    tmp += QString::number(wpndmgtype[1]->selectedId()) + ",";
    tmp += QString::number(dmgMin[2]) + ",";
    tmp += QString::number(dmgMax[2]) + ",";
    tmp += QString::number(wpndmgtype[2]->selectedId()) + ",";
    tmp += QString::number(dmgMin[3]) + ",";
    tmp += QString::number(dmgMax[3]) + ",";
    tmp += QString::number(wpndmgtype[3]->selectedId()) + ",";
    tmp += QString::number(dmgMin[4]) + ",";
    tmp += QString::number(dmgMax[4]) + ",";
    tmp += QString::number(wpndmgtype[4]->selectedId()) + ",";
    tmp += ui->armorEdit->text() + ",";
    tmp += ui->holyresEdit->text() + ",";
    tmp += ui->fireresEdit->text() + ",";
    tmp += ui->natureresEdit->text() + ",";
    tmp += ui->frostresEdit->text() + ",";
    tmp += ui->shadowresEdit->text() + ",";
    tmp += ui->arcaneresEdit->text() + ",";
    tmp += ui->atkspeedEdit->text() + ",";
    tmp += QString::number(ammotypeselecter->selectedId()) + ",";
    // if weapon is ranged set RangedModRange to 100
    bool isRanged = (classselecter->selectedId() == 2) &&   // weapon
            ((subclassselecter->selectedId() == 2) ||       // bow
            (subclassselecter->selectedId() == 3) ||        // gun
            (subclassselecter->selectedId() == 16) ||       // thrown
            (subclassselecter->selectedId() == 18) ||       // crossbow
            (subclassselecter->selectedId() == 19));        // wand
    if(isRanged){ tmp += "100,"; }
    else        { tmp += "0,"; }
    tmp += QString::number(spell1selecter->selectedId()) + ",";
    tmp += QString::number(spelltriggerselecter[0]->selectedId()) + ",";
    tmp += ui->spell1Charges->text() + ",";
    tmp += ui->spell1ppm->text() + ",";
    tmp += ui->spell1cd->text() + ",";
    tmp += ui->spell1cag->text() + ",";
    tmp += ui->spell1cagcd->text() + ",";
    tmp += QString::number(spell2selecter->selectedId()) + ",";
    tmp += QString::number(spelltriggerselecter[1]->selectedId()) + ",";
    tmp += ui->spell2Charges->text() + ",";
    tmp += ui->spell2ppm->text() + ",";
    tmp += ui->spell2cd->text() + ",";
    tmp += ui->spell2cag->text() + ",";
    tmp += ui->spell2cagcd->text() + ",";
    tmp += QString::number(spell3selecter->selectedId()) + ",";
    tmp += QString::number(spelltriggerselecter[2]->selectedId()) + ",";
    tmp += ui->spell3Charges->text() + ",";
    tmp += ui->spell3ppm->text() + ",";
    tmp += ui->spell3cd->text() + ",";
    tmp += ui->spell3cag->text() + ",";
    tmp += ui->spell3cagcd->text() + ",";
    tmp += QString::number(spell4selecter->selectedId()) + ",";
    tmp += QString::number(spelltriggerselecter[3]->selectedId()) + ",";
    tmp += ui->spell4Charges->text() + ",";
    tmp += ui->spell4ppm->text() + ",";
    tmp += ui->spell4cd->text() + ",";
    tmp += ui->spell4cag->text() + ",";
    tmp += ui->spell4cagcd->text() + ",";
    tmp += QString::number(spell5selecter->selectedId()) + ",";
    tmp += QString::number(spelltriggerselecter[4]->selectedId()) + ",";
    tmp += ui->spell5Charges->text() + ",";
    tmp += ui->spell5ppm->text() + ",";
    tmp += ui->spell5cd->text() + ",";
    tmp += ui->spell5cag->text() + ",";
    tmp += ui->spell5cagcd->text() + ",";
    tmp += QString::number(ui->bondingCB->currentIndex()) + ",";
    tmp += "\"" + ui->descEdit->toPlainText() + "\",";
    tmp += QString::number(bookselecter->selectedId()) + ",";
    tmp += QString::number(languageselecter->selectedId()) + ",";
    tmp += QString::number(pagematerialselecter->selectedId()) + ",";
    tmp += "0,";    // startquest NYI
    tmp += QString::number(lockselecter->selectedId()) + ",";
    tmp += QString::number(materialselecter->selectedId()) + ",";
    tmp += QString::number(sheatheselecter->selectedId()) + ",";
    tmp += QString::number(randompropertyselecter->selectedId()) + ",";
    tmp += QString::number(randomsuffixselecter->selectedId()) + ",";
    tmp += ui->blockEdit->text() + ",";
    tmp += QString::number(itemsetselecter->selectedId()) + ",";
    tmp += ui->durabilityEdit->text() + ",";
    tmp += QString::number(reqareaselecter->selectedId()) + ",";
    tmp += QString::number(reqmapselecter->selectedId()) + ",";
    tmp += QString::number(bagfamilyselecter->selectedId()) + ",";
    tmp += QString::number(totemselecter->selectedId()) + ",";
    tmp += QString::number(socketselecter[0]->selectedId()) + ",";
    tmp += "0,";    // socketContent_1
    tmp += QString::number(socketselecter[1]->selectedId()) + ",";
    tmp += "0,";    // socketContent_2
    tmp += QString::number(socketselecter[2]->selectedId()) + ",";
    tmp += "0,";    // socketContent_3
    tmp += QString::number(socketbonusselecter->selectedId()) + ",";
    tmp += QString::number(gemselecter->selectedId()) + ",";
    tmp += ui->deskillEdit->text() + ",";
    tmp += QString::number(armorDamageModifier) + ",";    // ArmorDamageModifier
    tmp += "'',";   // ScriptName
    tmp += QString::number(delootselecter->selectedId()) + ",";
    tmp += QString::number(foodtypeselecter->selectedId()) + ",";
    tmp += ui->minmoneyEdit->text() + ",";
    tmp += ui->maxmoneyEdit->text() + ",";
    tmp += ui->durationEdit->text() + ",";
    tmp += QString::number(ui->realtimeCheck->isChecked() * 2);

    return tmp;
}

void itemtemplateedit::on_addButton_clicked()
{
    setEditDefaults();
    QString query;
    int e = SQLFunctions::get_first_free_id(db,"item_template","entry");
    QString entry = QString::number(e);
    query = "INSERT INTO item_template (" + currentQueryFields() + ")" +
            " VALUES(" + entry + ","  + currentQueryValues() + ");";
    QSqlQuery q = db.exec(query);
    itemselecter->setIndexFromId(e);
    ui->queryStatus->setText("Entry " + entry + " was successfully inserted into item_template.");
}

void itemtemplateedit::on_deleteButton_clicked()
{
    // get current id
    int entry = itemselecter->selectedId();
    bool exists = SQLFunctions::entry_exists(db,"item_template","entry",QString::number(entry));
    if(exists)
    {
        db.exec("DELETE FROM item_template WHERE entry = " + QString::number(entry) + ";");
        ui->queryStatus->setText("Entry " + QString::number(entry) + " was successfully deleted from item_template.");
    }
    else
    {
        ui->queryStatus->setText("No entry was specified for deletion.");
    }
}

void itemtemplateedit::on_overwriteButton_clicked()
{
    int entry = itemselecter->selectedId();
    bool exists = SQLFunctions::entry_exists(db,"item_template","entry",QString::number(entry));
    if(exists)
    {
        // overwrite
        db.exec("DELETE FROM item_template WHERE entry = " + QString::number(entry) + ";");
        setEditDefaults();
        QString query;
        QString e = QString::number(entry);
        query = "INSERT INTO item_template (" + currentQueryFields() + ")" +
                " VALUES(" + e + ","  + currentQueryValues() + ");";
        QSqlQuery q = db.exec(query);
        itemselecter->setIndexFromId(entry);
        ui->queryStatus->setText("Entry " + e + " was successfully overwritten.");
    }
    else
    {
        // trivial add
        on_addButton_clicked();
    }
}
