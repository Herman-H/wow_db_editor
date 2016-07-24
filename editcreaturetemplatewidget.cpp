#include "editcreaturetemplatewidget.h"
#include "ui_editcreaturetemplatewidget.h"
#include "creaturetemplatequery.h"
#include "SQL_Functions.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <math.h>
#include <limits>

#include "dbc/dbcrecorddescriptors.h"
#include "dbccache.h"
#include "dbcache.h"
#include "widgets/item_models.h"
#include "widgets/selecter_button.h"
#include "widgets/loot_selecter_widget.h"

EditCreatureTemplateWidget::EditCreatureTemplateWidget(QSqlDatabase &db,
                                                       burning_crusade_records::entityviews::dbcs_module_type &m,
                                                       dbccache * dbcC,
                                                       dbcache * dbC,
                                                       item_models * ims,
                                                       QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditCreatureTemplateWidget),
    db(db),
    dbc(m),
    m_dbccache(dbcC),
    m_dbcache(dbC)
{
    ui->setupUi(this);
    // set line edits constraints
    ui->MinLevelEdit->setValidator(new QIntValidator{});
    ui->MaxLevelEdit->setValidator(new QIntValidator{});
    ui->ScaleEdit->setValidator(new QDoubleValidator{});
    ui->MeleeAttackPower->setValidator(new QIntValidator{});
    ui->RangedAttackPower->setValidator(new QIntValidator{});
    ui->DmgVariance->setValidator(new QDoubleValidator{});
    ui->DamageMultiplier->setValidator(new QDoubleValidator{});
    ui->MeleeAtkSpeed->setValidator(new QIntValidator{});
    ui->RangedAtkSpeed->setValidator(new QIntValidator{});
    ui->HolyResEdit->setValidator(new QIntValidator{});
    ui->FireResEdit->setValidator(new QIntValidator{});
    ui->NatureResEdit->setValidator(new QIntValidator{});
    ui->ShadowResEdit->setValidator(new QIntValidator{});
    ui->FrostResEdit->setValidator(new QIntValidator{});
    ui->ArcaneResEdit->setValidator(new QIntValidator{});
    ui->MinGoldEdit->setValidator(new QIntValidator{});
    ui->MaxGoldEdit->setValidator(new QIntValidator{});
    ui->HealthMultiplier->setValidator(new QDoubleValidator{});
    ui->ManaMultiplier->setValidator(new QDoubleValidator{});
    ui->XpMultiplier->setValidator(new QDoubleValidator{});
    ui->ArmorMultiplier->setValidator(new QDoubleValidator{});
    ui->WalkSpeed->setValidator(new QDoubleValidator{});
    ui->RunSpeed->setValidator(new QDoubleValidator{});

    creatureclassselecter = new creature_class_selecter();
    ui->unitclassLayout->addWidget(creatureclassselecter->getWidget());
    typeselecter = new creature_type_selecter(m_dbccache);
    ui->creaturetypeLayout->addWidget(typeselecter->getWidget());
    creaturefamilyselecter = new creature_family_selecter(m_dbccache);
    ui->creaturefamilyLayout->addWidget(creaturefamilyselecter->getWidget());
    expansionselecter = new expansion_selecter();
    ui->expansionLayout->addWidget(expansionselecter->getWidget());
    ui->rankLayout->addWidget((rankselecter = new creature_rank_selecter())->getWidget());
    emoteselecter = new standing_animations_selecter{m_dbccache};
    killcredit1selecter = new creature_template_selecter_button{db,m_dbccache,true};
    killcredit2selecter = new creature_template_selecter_button{db,m_dbccache,true};
    ui->heroicentryLayout->addWidget((heroicentryselecter = new creature_template_selecter_button{db,m_dbccache,true})->getWidget());
    ui->killCredit1Layout->addWidget(killcredit1selecter->getWidget());
    ui->killCredit2Layout->addWidget(killcredit2selecter->getWidget());
    ui->animationSelLayout->addWidget(emoteselecter->getWidget());
    modelselecter1 = new model_selecter{m_dbccache};
    modelselecter2 = new model_selecter{m_dbccache};
    modelselecter3 = new model_selecter{m_dbccache};
    modelselecter4 = new model_selecter{m_dbccache};
    ui->model1Layout->addWidget(modelselecter1->getWidget());
    ui->model2Layout->addWidget(modelselecter2->getWidget());
    ui->model3Layout->addWidget(modelselecter3->getWidget());
    ui->model4Layout->addWidget(modelselecter4->getWidget());
    ui->creaturelootLayout->addWidget((creaturelootselecter = new selecter_button{new creature_loot_group_dialog{ims->get_creature_loot_group_model(),ims->get_resources()}})->getButton());
    unitflagsselecter = new unit_flags_selecter();
    ui->UnitFlagsGB->layout()->addWidget(unitflagsselecter->getWidget());
    dynflagsselecter = new unit_dynamic_flags_selecter();
    ui->DynFlagsGB->layout()->addWidget(dynflagsselecter->getWidget());
    creaturetypeflagsselecter = new creature_type_flags_selecter();
    ui->CreatureTypeFlagsGB->layout()->addWidget(creaturetypeflagsselecter->getWidget());
    extraflagsselecter = new flags_extra_selecter(false);
    ui->ExtraFlagsGB->layout()->addWidget(extraflagsselecter->getWidget());
    npcflagsselecter = new npc_flags_selecter();
    ui->NPCFlagsGB->layout()->addWidget(npcflagsselecter->getWidget());
    inhabitselecter = new inhabit_type_selecter();
    ui->inhabitGB->layout()->addWidget(inhabitselecter->getWidget());

    trainerclassselecter = new trainer_class_selecter();
    ui->trainerClassLayout->addWidget(trainerclassselecter->getWidget());
    trainerraceselecter = new trainer_race_selecter();
    ui->trainerRaceLayout->addWidget(trainerraceselecter->getWidget());
    trainerspellselecter = new spell_selecter{m_dbccache};
    ui->trainerSpellLayout->addWidget(trainerspellselecter->getWidget());
    trainertypeselecter = new trainer_type_selecter();
    ui->trainerTypeLayout->addWidget(trainertypeselecter->getWidget());

    factionAselecter = new faction_selecter(m_dbccache);
    factionHselecter = new faction_selecter(m_dbccache);
    ui->factionALayout->addWidget(factionAselecter->getWidget());
    ui->factionHLayout->addWidget(factionHselecter ->getWidget());
    movementtypeselecter = new movement_type_selecter();
    ui->movementLayout->addWidget(movementtypeselecter->getWidget());
    mainhandselecter = new item_model_selecter{m_dbccache,item_model_selecter::SLOT_MAINHAND};
    offhandselecter = new item_model_selecter{m_dbccache,item_model_selecter::SLOT_OFFHAND};
    rangedselecter = new item_model_selecter{m_dbccache,item_model_selecter::SLOT_RANGED};
    ui->mainhandLayout->addWidget(mainhandselecter->getWidget());
    ui->offhandLayout->addWidget(offhandselecter->getWidget());
    ui->rangedLayout->addWidget(rangedselecter->getWidget());

    mechanicsimmuneselecter = new mechanics_mask_selecter(false);
    ui->mechanicimmuneLayout->addWidget(mechanicsimmuneselecter->getWidget());

    petspelldataidselecter = new pet_spell_selecter{m_dbccache};
    ui->petspellLayout->addWidget(petspelldataidselecter->getWidget());
    ui->trainerteachGB->layout()->addWidget((trainerteaches = new trainer_teach_edit{db,m,m_dbccache}));
    ui->vendorLayout->addWidget((vendoredit = new vendor_edit{dbC}));

    ui->gossipGB->layout()->addWidget((gossipselecter = new gossip_menu_selecter(m_dbcache))->getWidget());
}

EditCreatureTemplateWidget::~EditCreatureTemplateWidget()
{
    delete ui;
}

void EditCreatureTemplateWidget::loadEquip(int id)
{
    QSqlQuery q = db.exec("SELECT equipentry1,equipentry2,equipentry3 FROM creature_equip_template WHERE entry = " +
            QString::number(id) + ";");
    if(q.next())
    {
        oldmh = q.record().value(0).toInt();
        oldoh = q.record().value(1).toInt();
        oldr = q.record().value(2).toInt();
        if(oldmh != 0)
        {
            mainhandselecter->setIndexFromId(oldmh);
        }
        else
        {
            mainhandselecter->setIndexFromId(0);
        }
        if(oldoh != 0)
        {
            offhandselecter->setIndexFromId(oldoh);
        }
        else
        {
            offhandselecter->setIndexFromId(0);
        }
        if(oldr != 0)
        {
            rangedselecter->setIndexFromId(oldr);
        }
        else
        {
            rangedselecter->setIndexFromId(0);
        }
    }
    else
    {
        mainhandselecter->setIndexFromId(0);
        offhandselecter->setIndexFromId(0);
        rangedselecter->setIndexFromId(0);
        oldmh = 0;
        oldoh = 0;
        oldr = 0;
    }
}

int EditCreatureTemplateWidget::saveEquip()
{
    oldmh = mainhandselecter->selectedId();
    oldoh = offhandselecter->selectedId();
    oldr = rangedselecter->selectedId();
    if(oldmh != 0 || oldoh != 0 || oldr != 0)
    {
        // First check creature_equip_template if there is an entry that exactly matches the specified values
        QSqlQuery q = db.exec("SELECT entry FROM creature_equip_template WHERE "
                              "equipentry1 = " + QString::number(oldmh) + " AND "
                              "equipentry2 = " + QString::number(oldoh) + " AND "
                              "equipentry3 = " + QString::number(oldr) + ";");
        if(q.next())
        {
            return q.record().value(0).toInt();
        }
        else
        {
            int entry = SQLFunctions::get_first_free_id(db,"creature_equip_template","entry");
            db.exec("INSERT INTO creature_equip_template (entry,equipentry1,equipentry2,equipentry3) VALUES(" +
                    QString::number(entry) + "," +
                    QString::number(oldmh) + "," +
                    QString::number(oldoh) + "," +
                    QString::number(oldr) + ");");
            return entry;
        }
    }
    else
    {
        return 0;
    }
}

void EditCreatureTemplateWidget::LoadEntry(int entry)
{
    if(SQLFunctions::entry_exists(db,"creature_template","entry",QString::number(entry)))
    {
    QString query = "SELECT Entry,Name,SubName,IconName,MinLevel,MaxLevel,HeroicEntry,ModelId1,"
            "ModelId2,ModelId3,ModelId4,FactionAlliance,FactionHorde,Scale,Family,"
            "CreatureType,InhabitType,RegenerateStats,RacialLeader,NpcFlags,"
            "UnitFlags,DynamicFlags,ExtraFlags,CreatureTypeFlags,SpeedWalk,SpeedRun,"
            "UnitClass,Rank,Expansion,HealthMultiplier,PowerMultiplier,"
            "DamageMultiplier,DamageVariance,ArmorMultiplier,ExperienceMultiplier,"
            "MinLevelHealth,MaxLevelHealth,MinLevelMana,MaxLevelMana,"
            "MinMeleeDmg,MaxMeleeDmg,MinRangedDmg,MaxRangedDmg,Armor,MeleeAttackPower,"
            "RangedAttackPower,MeleeBaseAttackTime,RangedBaseAttackTime,DamageSchool,"
            "MinLootGold,MaxLootGold,LootId,PickpocketLootId,SkinningLootId,KillCredit1,KillCredit2,"
            "MechanicImmuneMask,ResistanceHoly,ResistanceFire,ResistanceNature,ResistanceFrost,"
            "ResistanceShadow,ResistanceArcane,PetSpellDataId,MovementType,TrainerType,TrainerSpell,"
            "TrainerClass,TrainerRace,TrainerTemplateId,VendorTemplateId,EquipmentTemplateId,"
            "GossipMenuId,AIName,ScriptName FROM creature_template WHERE entry = " + QString::number(entry) +";";
    QSqlQuery s{query,db};// = db.exec(query);
    s.next();

    ui->EntryLabel->setText(QString::number(s.value(0).toInt()));
    ui->NameEdit->setText(s.value(1).toString());
    // IconName selecter not implemented
    ui->subnameEdit->setText(s.value(2).toString());
    ui->MinLevelEdit->setText(QString::number(s.value(4).toInt()));
    ui->MaxLevelEdit->setText(QString::number(s.value(5).toInt()));
    if(s.value(6).toInt() != 0)
    {
        heroicentryselecter->setIndexFromId(s.value(6).toInt());
    }
    else{ heroicentryselecter->clear(); }
    modelselecter1->setIndexFromId(s.value(7).toInt());
    modelselecter2->setIndexFromId(s.value(8).toInt());
    modelselecter3->setIndexFromId(s.value(9).toInt());
    modelselecter4->setIndexFromId(s.value(10).toInt());
    factionAselecter->setIndexFromId(s.value(11).toInt());
    factionHselecter->setIndexFromId(s.value(12).toInt());
    ui->ScaleEdit->setText(QString::number(s.value(13).toFloat()));
    creaturefamilyselecter->setIndexFromId(s.value(14).toInt());
    typeselecter->setIndexFromId(s.value(15).toInt());
    inhabitselecter->setIndexFromId(s.value(16).toInt());
    ui->doesRegenHealth->setChecked(s.value(17).toInt() != 0);
    ui->isRacialLeader->setChecked(s.value(18).toInt() != 0);
    npcflagsselecter->setIndexFromId(s.value(19).toInt());
    unitflagsselecter->setIndexFromId(s.value(20).toInt());
    dynflagsselecter->setIndexFromId(s.value(21).toInt());
    extraflagsselecter->setIndexFromId(s.value(22).toInt());
    creaturetypeflagsselecter->setIndexFromId(s.value(23).toInt());
    ui->WalkSpeed->setText(QString::number(s.value(24).toFloat()));
    ui->RunSpeed->setText(QString::number(s.value(25).toFloat()));
    creatureclassselecter->setIndexFromId(s.value(26).toInt());
    rankselecter->setIndexFromId(s.value(27).toInt());
    expansionselecter->setIndexFromId(s.value(28).toInt());
    ui->HealthMultiplier->setText(QString::number(s.value(29).toFloat()));
    ui->ManaMultiplier->setText(QString::number(s.value(30).toFloat()));
    ui->DamageMultiplier->setText(QString::number(s.value(31).toFloat()));
    ui->DmgVariance->setText(QString::number(s.value(32).toFloat()));
    ui->ArmorMultiplier->setText(QString::number(s.value(33).toFloat()));
    ui->XpMultiplier->setText(QString::number(s.value(34).toFloat()));
    /*
     *  Fields 35-43 are obsolete, no need to load (save to them still)
     */
    ui->MeleeAttackPower->setText(QString::number(s.value(44).toInt()));
    ui->RangedAttackPower->setText(QString::number(s.value(45).toInt()));
    ui->MeleeAtkSpeed->setText(QString::number(s.value(46).toInt()));
    ui->RangedAtkSpeed->setText(QString::number(s.value(47).toInt()));
    ui->DmgSchoolCB->setCurrentIndex(s.value(48).toInt()+1);
    ui->MinGoldEdit->setText(QString::number(s.value(49).toInt()));
    ui->MaxGoldEdit->setText(QString::number(s.value(50).toInt()));
    creaturelootselecter->setId(s.value(51).toInt());
    // Pickpocketlootid NYI
    // Skinlootid NYI
    if(s.value(54).toInt() != 0)
    {
        killcredit1selecter->setIndexFromId(s.value(54).toInt());
    }
    else { killcredit1selecter->clear(); }
    if(s.value(55).toInt() != 0)
    {
        killcredit2selecter->setIndexFromId(s.value(55).toInt());
    }
    else { killcredit2selecter->clear(); }
    mechanicsimmuneselecter->setIndexFromId(s.value(56).toInt());
    ui->HolyResEdit->setText(QString::number(s.value(57).toInt()));
    ui->FireResEdit->setText(QString::number(s.value(58).toInt()));
    ui->NatureResEdit->setText(QString::number(s.value(59).toInt()));
    ui->FrostResEdit->setText(QString::number(s.value(60).toInt()));
    ui->ShadowResEdit->setText(QString::number(s.value(61).toInt()));
    ui->ArcaneResEdit->setText(QString::number(s.value(62).toInt()));
    petspelldataidselecter->setIndexFromId(s.value(63).toInt());
    movementtypeselecter->setIndexFromId(s.value(64).toInt());
    trainertypeselecter->setIndexFromId(s.value(65).toInt());
    trainerspellselecter->setIndexFromId(s.value(66).toInt());
    trainerclassselecter->setIndexFromId(s.value(67).toInt());
    trainerraceselecter->setIndexFromId(s.value(68).toInt());
    // trainertemplateid NYI
    // vendortemplateid NYI
    loadEquip(s.value(71).toInt());
    gossipselecter->setIndexFromId(s.value(72).toInt());
    ui->ainameEdit->setText(s.value(73).toString());
    ui->scriptnameEdit->setText(s.value(74).toString());

    updateMobDps();
    updateMobRDps();
    updateMobScore();
    }
}

void EditCreatureTemplateWidget::setEditDefaults()
{
    // If some lineedits are unspecified, set them to a fair default value
    if(ui->MinLevelEdit->text().length() == 0)      { ui->MinLevelEdit->setText("1"); }
    if(ui->MaxLevelEdit->text().length() == 0)      { ui->MaxLevelEdit->setText("1"); }
    if(ui->ScaleEdit->text().length() == 0)         { ui->ScaleEdit->setText("1"); }
    if(ui->WalkSpeed->text().length() == 0)         { ui->WalkSpeed->setText("0.91"); }
    if(ui->RunSpeed->text().length() == 0)          { ui->RunSpeed->setText("1.14286"); }
    if(ui->HealthMultiplier->text().length() == 0)  { ui->HealthMultiplier->setText("1"); }
    if(ui->ManaMultiplier->text().length() == 0)    { ui->ManaMultiplier->setText("1"); }
    if(ui->DamageMultiplier->text().length() == 0)  { ui->DamageMultiplier->setText("1"); }
    if(ui->DmgVariance->text().length() == 0)       { ui->DmgVariance->setText("1"); }
    if(ui->ArmorMultiplier->text().length() == 0)   { ui->ArmorMultiplier->setText("1"); }
    if(ui->XpMultiplier->text().length() == 0)      { ui->XpMultiplier->setText("1"); }
    if(ui->MeleeAttackPower->text().length() == 0)  { ui->MeleeAttackPower->setText("10"); }
    if(ui->RangedAttackPower->text().length() == 0) { ui->RangedAttackPower->setText("10"); }
    if(ui->MeleeAtkSpeed->text().length() == 0)     { ui->MeleeAtkSpeed->setText("2000"); }
    if(ui->RangedAtkSpeed->text().length() == 0)    { ui->RangedAtkSpeed->setText("2200"); }
    if(ui->MinGoldEdit->text().length() == 0)       { ui->MinGoldEdit->setText("0"); }
    if(ui->MaxGoldEdit->text().length() == 0)       { ui->MaxGoldEdit->setText("0"); }
    if(ui->HolyResEdit->text().length() == 0)       { ui->HolyResEdit->setText("0"); }
    if(ui->FireResEdit->text().length() == 0)       { ui->FireResEdit->setText("0"); }
    if(ui->NatureResEdit->text().length() == 0)     { ui->NatureResEdit->setText("0"); }
    if(ui->FrostResEdit->text().length() == 0)      { ui->FrostResEdit->setText("0"); }
    if(ui->ShadowResEdit->text().length() == 0)     { ui->ShadowResEdit->setText("0"); }
    if(ui->ArcaneResEdit->text().length() == 0)     { ui->ArcaneResEdit->setText("0"); }
    if(inhabitselecter->selectedId() == 0)          { inhabitselecter->setIndexFromId(1); }
}

QString EditCreatureTemplateWidget::commonBody()
{
    setEditDefaults();
    QString query = "";
    query += "\"" + ui->NameEdit->text() + "\",";
    query += "\"" + ui->subnameEdit->text() + "\",";
    query += "'',"; //IconName NYI
    query += ui->MinLevelEdit->text() + ",";
    query += ui->MaxLevelEdit->text() + ",";
    query += QString::number(heroicentryselecter->selectedId()) + ",";
    query += QString::number(modelselecter1->selectedId()) + ",";
    query += QString::number(modelselecter2->selectedId()) + ",";
    query += QString::number(modelselecter3->selectedId()) + ",";
    query += QString::number(modelselecter4->selectedId()) + ",";
    query += QString::number(factionAselecter->selectedId()) + ",";
    query += QString::number(factionHselecter->selectedId()) + ",";
    query += ui->ScaleEdit->text() + ",";
    query += QString::number(creaturefamilyselecter->selectedId()) + ",";
    query += QString::number(typeselecter->selectedId()) + ",";
    query += QString::number(inhabitselecter->selectedId()) + ",";
    query += QString::number(ui->doesRegenHealth->isChecked()) + ",";
    query += QString::number(ui->isRacialLeader->isChecked()) + ",";
    query += QString::number(npcflagsselecter->selectedId()) + ",";
    query += QString::number(unitflagsselecter->selectedId()) + ",";
    query += QString::number(dynflagsselecter->selectedId()) + ",";
    query += QString::number(extraflagsselecter->selectedId()) + ",";
    query += QString::number(creaturetypeflagsselecter->selectedId()) + ",";
    query += ui->WalkSpeed->text() + ",";
    query += ui->RunSpeed->text() + ",";;
    query += QString::number(creatureclassselecter->selectedId()) + ",";
    query += QString::number(rankselecter->selectedId()) + ",";
    query += QString::number(expansionselecter->selectedId()) + ",";
    query += ui->HealthMultiplier->text() + ",";
    query += ui->ManaMultiplier->text() + ",";
    query += ui->DamageMultiplier->text() + ",";
    query += ui->DmgVariance->text() + ",";
    query += ui->ArmorMultiplier->text() + ",";
    query += ui->XpMultiplier->text() + ",";
    query += "100,"; // MinLevelHealth
    query += "100,"; // MaxLevelHealth
    query += "100,"; // MinLevelMana
    query += "100,"; // MaxLevelMana
    query += "100,"; // MinMeleeDmg
    query += "100,"; // MaxMeleeDmg
    query += "100,"; // MinRangedDmg
    query += "100,"; // MaxRangedDmg
    query += "100,"; // Armor
    query += ui->MeleeAttackPower->text() + ",";
    query += ui->RangedAttackPower->text() + ",";
    query += ui->MeleeAtkSpeed->text() + ",";
    query += ui->RangedAtkSpeed->text() + ",";
    query += QString::number(ui->DmgSchoolCB->currentIndex()-1) + ",";
    query += ui->MinGoldEdit->text() + ",";
    query += ui->MaxGoldEdit->text() + ",";
    query += QString::number(creaturelootselecter->selectedId()) + ",";
    query += "0,"; // PickpocketLootId
    query += "0,"; // SkinningLootId
    query += QString::number(killcredit1selecter->selectedId()) + ",";
    query += QString::number(killcredit2selecter->selectedId()) + ",";
    query += QString::number(mechanicsimmuneselecter->selectedId()) + ",";
    query += ui->HolyResEdit->text() + ",";
    query += ui->FireResEdit->text() + ",";
    query += ui->NatureResEdit->text() + ",";
    query += ui->FrostResEdit->text() + ",";
    query += ui->ShadowResEdit->text() + ",";
    query += ui->ArcaneResEdit->text() + ",";
    query += QString::number(petspelldataidselecter->selectedId()) + ",";
    query += QString::number(movementtypeselecter->selectedId()) + ",";
    query += QString::number(trainertypeselecter->selectedId()) + ",";
    query += QString::number(trainerspellselecter->selectedId()) + ",";
    query += QString::number(trainerclassselecter->selectedId()) + ",";
    query += QString::number(trainerraceselecter->selectedId()) + ",";
    query += "0,"; // TrainerTemplateId
    query += "0,"; // VendorTemplateId
    query += QString::number(saveEquip()) + ",";
    query += "0,"; // GossipMenuId
    query += "'" + ui->ainameEdit->text() + "',";
    query += "'" + ui->scriptnameEdit->text() + "')";
    return query;
}

QString EditCreatureTemplateWidget::makeOverwriteQuery()
{
    QString query = "INSERT INTO creature_template (Entry,Name,SubName,IconName,MinLevel,MaxLevel,"
            "HeroicEntry,ModelId1,ModelId2,ModelId3,ModelId4,FactionAlliance,FactionHorde,Scale,"
            "Family,CreatureType,InhabitType,RegenerateHealth,RacialLeader,NpcFlags,UnitFlags,"
            "DynamicFlags,ExtraFlags,CreatureTypeFlags,SpeedWalk,SpeedRun,UnitClass,Rank,Expansion,"
            "HealthMultiplier,ManaMultiplier,DamageMultiplier,DamageVariance,ArmorMultiplier,"
            "ExperienceMultiplier,MinLevelHealth,MaxLevelHealth,MinLevelMana,MaxLevelMana,"
            "MinMeleeDmg,MaxMeleeDmg,MinRangedDmg,MaxRangedDmg,Armor,MeleeAttackPower,RangedAttackPower,"
            "MeleeBaseAttackTime,RangedBaseAttackTime,DamageSchool,MinLootGold,MaxLootGold,LootId,"
            "PickpocketLootId,SkinningLootId,KillCredit1,KillCredit2,MechanicImmuneMask,ResistanceHoly,"
            "ResistanceFire,ResistanceNature,ResistanceFrost,ResistanceShadow,ResistanceArcane,"
            "PetSpellDataId,MovementType,TrainerType,TrainerSpell,TrainerClass,TrainerRace,"
            "TrainerTemplateId,VendorTemplateId,EquipmentTemplateId,GossipMenuId,AIName,ScriptName) VALUES(";
    query += ui->EntryLabel->text() + ",";
    query += commonBody();
    return query;
}

QString EditCreatureTemplateWidget::makeAddQuery()
{
    QString query = "INSERT INTO creature_template (Entry,Name,SubName,IconName,MinLevel,MaxLevel,"
            "HeroicEntry,ModelId1,ModelId2,ModelId3,ModelId4,FactionAlliance,FactionHorde,Scale,"
            "Family,CreatureType,InhabitType,RegenerateHealth,RacialLeader,NpcFlags,UnitFlags,"
            "DynamicFlags,ExtraFlags,CreatureTypeFlags,SpeedWalk,SpeedRun,UnitClass,Rank,Expansion,"
            "HealthMultiplier,ManaMultiplier,DamageMultiplier,DamageVariance,ArmorMultiplier,"
            "ExperienceMultiplier,MinLevelHealth,MaxLevelHealth,MinLevelMana,MaxLevelMana,"
            "MinMeleeDmg,MaxMeleeDmg,MinRangedDmg,MaxRangedDmg,Armor,MeleeAttackPower,RangedAttackPower,"
            "MeleeBaseAttackTime,RangedBaseAttackTime,DamageSchool,MinLootGold,MaxLootGold,LootId,"
            "PickpocketLootId,SkinningLootId,KillCredit1,KillCredit2,MechanicImmuneMask,ResistanceHoly,"
            "ResistanceFire,ResistanceNature,ResistanceFrost,ResistanceShadow,ResistanceArcane,"
            "PetSpellDataId,MovementType,TrainerType,TrainerSpell,TrainerClass,TrainerRace,"
            "TrainerTemplateId,VendorTemplateId,EquipmentTemplateId,GossipMenuId,AIName,ScriptName) VALUES(";
    m_tempi = SQLFunctions::get_first_free_id(db, "creature_template", "entry");
    query += QString::number(m_tempi) + ",";
    query += commonBody();
    return query;
}

void EditCreatureTemplateWidget::on_OverwriteToDBButton_clicked()
{
    if(SQLFunctions::entry_exists(db,"creature_template","entry",ui->EntryLabel->text()))
    {
        // Overwrite
        db.exec("DELETE FROM creature_template WHERE entry = " + ui->EntryLabel->text() + ";");
        db.exec(makeOverwriteQuery());
        ui->EditStatusLabel->setText("Creature " + ui->EntryLabel->text() + " overwritten");
    }
    else
    {
        on_AddToDBButton_clicked();
    }
}

void EditCreatureTemplateWidget::on_AddToDBButton_clicked()
{
    db.exec(makeAddQuery());
    ui->EntryLabel->setText(QString::number(m_tempi));
    ui->EditStatusLabel->setText("Creature " + QString::number(m_tempi) + " was added");
}

void EditCreatureTemplateWidget::on_Delete_clicked()
{
    // First check that there exists no creatures with this template
    if(SQLFunctions::entry_exists(db,"creature","id",ui->EntryLabel->text()))
    {
        ui->EditStatusLabel->setText("There exists creatures that use this template!");
    }
    else if(SQLFunctions::entry_exists(db,"creature_template","entry",ui->EntryLabel->text()))
    {
        db.exec("DELETE FROM creature_template WHERE entry = " + ui->EntryLabel->text() + ";");
        ui->EditStatusLabel->setText("Creature " + ui->EntryLabel->text() + " was deleted");
    }
    else
    {
        ui->EditStatusLabel->setText("No entry was specified for deletion");
    }
}

void EditCreatureTemplateWidget::updateMobDps()
{
    /*
     *  Boss attack power formula lvl 73
     *
     *  Boss Base Dmg multiplier, bbdm = 1 + AP * 0.0004888
     *
     *  Boss Damage = (bbd * bbdm) * dmg_multiplier = bd
     *
     *  Boss Dps = bd / baseattacktime
     *
     *//*
    float ap = ui->MeleeAtkPowerEdit->text().toDouble();
    float basemin = ui->MinMeleeDmgEdit->text().toDouble();
    float basemax = ui->MaxMeleeDmgEdit->text().toDouble();
    float mult = ui->DmgMultiplierEdit->text().toDouble();
    float at = ui->MeleeAtkSpeedEdit->text().toDouble() / 1000; // attack speed is stored in milliseconds
    ui->DPSLabel->setText(QString::number(( ((basemin+basemax)/2)*(1+ap*0.0004888) * mult) / at ));*/
}

void EditCreatureTemplateWidget::updateMobRDps()
{
    /*
     *  Boss attack power formula lvl 73
     *
     *  Boss Base Dmg multiplier, bbdm = 1 + AP * 0.0004888
     *
     *  Boss Damage = (bbd * bbdm) * dmg_multiplier = bd
     *
     *  Boss Dps = bd / baseattacktime
     *
     *//*
    float ap = ui->RangedAtkPowerEdit->text().toDouble();
    float basemin = ui->MinRangedAtkDmgEdit->text().toDouble();
    float basemax = ui->MaxRangedAtkDmgEdit->text().toDouble();
    float mult = ui->DmgMultiplierEdit->text().toDouble();
    float at = ui->RangedAtkSpeedEdit->text().toDouble() / 1000; // attack speed is stored in milliseconds
    ui->RDPSLabel->setText(QString::number(( ((basemin+basemax)/2)*(1+ap*0.0004888) * mult) / at ));*/
}

void EditCreatureTemplateWidget::updateMobScore()
{
    // Find different factors of difficulty
    /*
     *
     *  HP - more hp makes mob more difficult, but difficulty does not scale linearly with HP, probably logarithmic scaling
     *
     *  DPS - more dps makes mob more difficult, assume linear difficulty scaling linearly with dps
     *
     *  Burst - High burst damage is a factor of difficulty since it limits progress due to tank hp, exponential component
     *
     *  Extend this model later if needed
     *//*
    float hp = (ui->MinHpEdit->text().toDouble() + ui->MaxHpEdit->text().toDouble()) / 2;
    float hp_factor = log(hp);

    // pick highest of dps or range dps
    float dps;
    float matkspd = ui->MeleeAtkSpeedEdit->text().toDouble() / 1000;
    float map = ui->MeleeAtkPowerEdit->text().toDouble();
    float mbasemin = ui->MinMeleeDmgEdit->text().toDouble();
    float mbasemax = ui->MaxMeleeDmgEdit->text().toDouble();
    float mmult = ui->DmgMultiplierEdit->text().toDouble();
    float mdps = (((mbasemin+mbasemax)/2)*(1+map*0.0004888) * mmult) / matkspd;

    float ratkspd = ui->RangedAtkSpeedEdit->text().toDouble() / 1000;
    float rap = ui->RangedAtkPowerEdit->text().toDouble();
    float rbasemin = ui->MinRangedAtkDmgEdit->text().toDouble();
    float rbasemax = ui->MaxRangedAtkDmgEdit->text().toDouble();
    float rmult = ui->DmgMultiplierEdit->text().toDouble();
    float rdps = (((rbasemin+rbasemax)/2)*(1+rap*0.0004888) * rmult) / ratkspd;

    if( ratkspd == 0  || rdps < mdps)
    {
        dps = mdps;
    }
    else
    {
        dps = rdps;
    }

    float dps_factor = dps / 1000;

    float burst = std::max(mbasemax*(1+map*0.0004888),rbasemax*(1+rap*0.0004888));

    // burst_factor should shoot up the closer it gets to tank max hp
    float burst_factor = exp(burst*0.0001);

//  printf("dps factor " + QString::number(dps_factor).toLocal8Bit());
//  printf("hp factor " + QString::number(hp_factor).toLocal8Bit());
//  printf("burst factor " + QString::number(burst_factor).toLocal8Bit());

    float score = hp_factor*dps_factor*burst_factor;

    ui->DifficultyScoreLabel->setText(QString::number(score));*/
}
