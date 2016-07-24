#include "dbccache.h"
#include <QList>
#include <QListWidgetItem>
#include <QString>
#include <QStringList>

dbccache::dbccache(entityviews::dbcs_module_type &dbc)
{
    creature_family_resource        = new QStringList();
    creature_family_references      = new std::vector<int>();
    creature_type_resource          = new QStringList();
    creature_type_references        = new std::vector<int>();
    skillline_resource              = new QStringList();
    skillline_references            = new std::vector<int>();
    faction_resource                = new QStringList();
    faction_plain                   = new QStringList();
    faction_references              = new std::vector<int>();
    reputation_faction_resource     = new QStringList();
    reputation_faction_references   = new std::vector<int>();
    item_class_resource             = new QStringList();
    item_class_references           = new std::vector<int>();
    item_subclass_resource          = new std::vector<QStringList*>();
    item_subclass_references        = new std::vector<std::vector<int>*>();
    map_resource                    = new QStringList();
    map_references                  = new std::vector<int>();
    area_resource                   = new QStringList();
    area_references                 = new std::vector<int>();
    model_resource                  = new QStringList();
    model_plain                     = new QStringList();
    model_references                = new std::vector<int>();
    spell_resource                  = new QStringList();
    spell_references                = new std::vector<int>();
    standing_animation_resource     = new QStringList();
    standing_animation_references   = new std::vector<int>();
    pet_spell_resource              = new QStringList();
    pet_spell_plain                 = new QStringList();
    pet_spell_references            = new std::vector<int>();
    item_extended_cost_item_entries = new std::set<int>();
    item_extended_cost_references   = new std::vector<int>();
    for(int i = 0; i < 18; i++)
    {
        item_model_resource[i]          = new QStringList();
        item_model_plain[i]             = new QStringList();
        item_model_item_references[i]   = new std::vector<int>();
        item_model_display_references[i]= new std::vector<int>();
    }
    item_set_resource               = new QStringList();
    item_set_references             = new std::vector<int>();
    title_resource                  = new QStringList();
    title_references                = new std::vector<int>();
    item_enchant_resource           = new QStringList();
    item_enchant_references         = new std::vector<int>();
    language_resource               = new QStringList();
    language_references             = new std::vector<int>();
    totem_category_resource         = new QStringList();
    totem_category_references       = new std::vector<int>();
    gem_resource                    = new QStringList();
    gem_references                  = new std::vector<int>();
    lock_type_item_entries          = new std::set<int>();
    lock_type_references            = new std::vector<int>();
    loadDBC(dbc);
}

dbccache::~dbccache()
{
    delete creature_family_resource;
    delete creature_family_references;
    delete creature_type_resource;
    delete creature_type_references;
    delete skillline_resource;
    delete skillline_references;
    delete faction_resource;
    delete faction_plain;
    delete faction_references;
    delete reputation_faction_resource;
    delete reputation_faction_references;
    delete item_class_resource;
    delete item_class_references;
    free(item_subclass_resource);
    delete item_subclass_resource;
    free(item_subclass_references);
    delete item_subclass_references;
    delete map_resource;
    delete map_references;
    delete area_resource;
    delete area_references;
    delete model_resource;
    delete model_plain;
    delete model_references;
    delete spell_resource;
    delete spell_references;
    delete standing_animation_references;
    delete standing_animation_resource;
    delete pet_spell_resource;
    delete pet_spell_plain;
    delete pet_spell_references;
    delete item_extended_cost_item_entries;
    delete item_extended_cost_references;
    for(int i = 0; i < 18; i++)
    {
        delete item_model_resource[i];
        delete item_model_plain[i];
        delete item_model_item_references[i];
        delete item_model_display_references[i];
    }
    delete item_set_resource;
    delete item_set_references;
    delete title_resource;
    delete title_references;
    delete item_enchant_resource;
    delete item_enchant_references;
    delete language_resource;
    delete language_references;
    delete totem_category_resource;
    delete totem_category_references;
    delete gem_resource;
    delete gem_references;
    delete lock_type_item_entries;
    delete lock_type_references;
}

void dbccache::creature_family_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::creature_family view;

    creature_family_resource->clear();
    creature_family_references->clear();

    creature_family_resource->push_back("Creature Family...");
    creature_family_resource->push_back("None");
    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        creature_family_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        creature_family_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
    }
}

bool dbccache::creature_family_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::creature_family view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::creature_type_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::creature_type view;

    creature_type_resource->clear();
    creature_type_references->clear();

    creature_type_resource->push_back("Creature Type...");
    creature_type_resource->push_back("None");

    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        creature_type_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        creature_type_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
    }
}

bool dbccache::creature_type_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::creature_type view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::skillline_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::skilltype_name view;
    skillline_resource->clear();
    skillline_references->clear();
    skillline_resource->push_back("Skill Type...");
    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        skillline_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        skillline_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
    }
}

bool dbccache::skillline_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::skilltype_name view;
    return static_cast<view*>(&dbc)->is_valid();
}

QString dbccache::faction::displayMask(int mask)
{

    QString tmp("");
    bool hasFlags = false;
    if((mask & FACTION_MASK_PLAYER) != 0)
    {
        tmp += "Player"; hasFlags = true;
    }
    if((mask & FACTION_MASK_ALLIANCE) != 0)
    {
        if(hasFlags){ tmp += ","; }
        tmp += "Alliance"; hasFlags = true;
    }
    if((mask & FACTION_MASK_HORDE) != 0)
    {
        if(hasFlags){ tmp += ","; }
        tmp += "Horde"; hasFlags = true;
    }
    if((mask & FACTION_MASK_MONSTER) != 0)
    {
        if(hasFlags){ tmp += ","; }
        tmp += "Monster"; hasFlags = true;
    }
    if(hasFlags)
    {
        tmp = "{" + tmp + "}";
    }
    return tmp;
}

QString dbccache::faction::displayFlags(int flags)
{
    bool hasFlags = false;
    QString tmp("");
    if((flags & FACTION_TEMPLATE_FLAG_PVP) != 0)
    {
        tmp += "PvP-Flagged"; hasFlags = true;
    }
    if((flags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0)
    {
        if(hasFlags){ tmp += ","; }
        tmp += "Anti-PvP Guard"; hasFlags = true;
    }
    if(hasFlags)
    {
        tmp = "Flags={" + tmp + "}";
    }
    return tmp;
}

void dbccache::faction_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::faction      view1;
    typedef entityviews::faction_name view2;
    typedef entityviews::faction_flags flags;
    typedef entityviews::faction_our_mask ourmask;
    typedef entityviews::faction_friendly_mask friendlymask;
    typedef entityviews::faction_hostile_mask hostilemask;

    dbccache::faction f;

    faction_resource->clear();
    faction_plain->clear();
    faction_references->clear();

    for(int i = 0; i < static_cast<view1*>(&dbc)->get_size(); i++)
    {
        int r = static_cast<view1*>(&dbc)->get_ref(i);
        faction_references->push_back(r);
        QString tmp(QString::number(r));
        tmp += QString(" ").repeated(7-tmp.length());

        int nameindex = static_cast<view1*>(&dbc)->get_label(i);
        nameindex = static_cast<view2*>(&dbc)->find_ref(nameindex);
        if(nameindex != -1)
        {
            tmp += QString(static_cast<view2*>(&dbc)->get_label(nameindex));
        }
        else
        {
            tmp += "Unspecified";
        }

        faction_plain->push_back(tmp);

        if((nameindex = static_cast<flags*>(&dbc)->get_label(i)))
        {
            tmp += "  " + f.displayFlags(nameindex);
        }
        if((nameindex = static_cast<ourmask*>(&dbc)->get_label(i)) != 0)
        {
            tmp += "  Our=" + f.displayMask(nameindex);
        }
        if((nameindex = static_cast<friendlymask*>(&dbc)->get_label(i)) != 0)
        {
            tmp += "  Friendly=" + f.displayMask(nameindex);
        }
        if((nameindex = static_cast<hostilemask*>(&dbc)->get_label(i)) != 0)
        {
            tmp += "  Hostile=" + f.displayMask(nameindex);
        }
        faction_resource->push_back(tmp);
    }
}

bool dbccache::faction_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::faction      view1;
    typedef entityviews::faction_name view2;
    return static_cast<view1*>(&dbc)->is_valid() && static_cast<view2*>(&dbc)->is_valid();
}

void dbccache::reputation_faction_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::repfaction     rep;
    typedef entityviews::faction_name   name;

    reputation_faction_resource->clear();
    reputation_faction_references->clear();

    reputation_faction_resource->push_back("Faction...");
    for(int i = 0; i < static_cast<rep*>(&dbc)->get_size(); i++)
    {
        if(static_cast<rep*>(&dbc)->get_label(i) != -1)
        {
            reputation_faction_references->push_back(static_cast<rep*>(&dbc)->get_ref(i));
            reputation_faction_resource->push_back(static_cast<name*>(&dbc)->get_label(i));
        }
    }
}

bool dbccache::reputation_faction_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::repfaction   view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::item_class_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemclassname view;

    item_class_resource->clear();
    item_class_references->clear();
    free(item_subclass_references);
    free(item_subclass_resource);
    item_subclass_resource->clear();
    item_subclass_references->clear();

    item_class_resource->push_back("Item Class...");
    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        item_class_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        item_class_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
        // For each itemclass add a subclass QStringList
        item_subclass_resource->push_back(new QStringList());
        item_subclass_references->push_back(new std::vector<int>());
    }
}

bool dbccache::item_class_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemclassname view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::item_subclass_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemsubclassname name;
    typedef entityviews::itemsubclassnameext nameext;
    typedef entityviews::itemsubclassid subclassid;

    for(int i = 0; i < item_subclass_resource->size(); i++)
    {
        item_subclass_resource->at(i)->clear();
        item_subclass_resource->at(i)->push_back("Item Subclass...");
    }
    for(int i = 0; i < item_subclass_resource->size(); i++)
    {
        item_subclass_references->at(i)->clear();
    }

    for(int i = 0; i < static_cast<name*>(&dbc)->get_size(); i++)
    {
        int a = static_cast<name*>(&dbc)->get_ref(i);
        int b = static_cast<subclassid*>(&dbc)->get_label(i);
        QString s = static_cast<name*>(&dbc)->get_label(i);
        QString t = static_cast<nameext*>(&dbc)->get_label(i);
        if(t.length() > 0)
        {
            s += ", " + t;
        }
        item_subclass_references->at(a)->push_back(b);
        item_subclass_resource->at(a)->push_back(s);
    }
}

bool dbccache::item_subclass_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemsubclassname view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::map_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::map_name view;
    map_resource->clear();
    map_references->clear();
    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        map_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        map_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
    }
}

bool dbccache::map_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::map_name view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::area_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::area_name view;

    area_resource->clear();
    area_references->clear();
    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        area_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        area_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
    }
}

bool dbccache::area_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::area_name view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::model_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::model_info view1;
    typedef entityviews::model_path view2;
    typedef entityviews::modelskin1 skin1;
    typedef entityviews::modelskin2 skin2;
    typedef entityviews::modelskin3 skin3;

    model_references->clear();
    model_plain->clear();
    model_resource->clear();

    for(int i = 0; i < static_cast<view1*>(&dbc)->get_size(); i++)
    {
        model_references->push_back(static_cast<view1*>(&dbc)->get_ref(i));
        int selectedmodel = static_cast<view1*>(&dbc)->get_label(i);
        selectedmodel = static_cast<view2*>(&dbc)->find_ref(selectedmodel);
        if(selectedmodel != -1)
        {
            bool b = false;
            QString skins = " skins={";
            if(QString(static_cast<skin1*>(&dbc)->get_label(i)).length() != 0)
            { skins += static_cast<skin1*>(&dbc)->get_label(i); b=true;}
            if(QString(static_cast<skin2*>(&dbc)->get_label(i)).length() != 0)
            {
                if(b==true){ skins+=", "; }
                skins += static_cast<skin2*>(&dbc)->get_label(i); b=true;
            }
            if(QString(static_cast<skin3*>(&dbc)->get_label(i)).length() != 0)
            {
                if(b==true){ skins+=", "; }
                skins += static_cast<skin3*>(&dbc)->get_label(i); b=true;
            }
            skins += "}";
            QString num = QString::number(static_cast<view1*>(&dbc)->get_ref(i)) + "\t";
            if(b){model_resource->push_back(num + QString(static_cast<view2*>(&dbc)->get_label(selectedmodel)) + skins);}
            else {model_resource->push_back(num + QString(static_cast<view2*>(&dbc)->get_label(selectedmodel)));}
            model_plain->push_back(static_cast<view2*>(&dbc)->get_label(selectedmodel));
        }
        else
        {
            model_resource->push_back("NO MODEL PATH");
            model_plain->push_back("NO MODEL PATH");
        }
    }
}

bool dbccache::model_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::model_info view1;
    typedef entityviews::model_path view2;
    return static_cast<view1*>(&dbc)->is_valid() && static_cast<view2*>(&dbc)->is_valid();
}

void dbccache::spell_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::spellname view1;
    typedef entityviews::spellrank view2;
    typedef entityviews::spelleffect1 view3;

    spell_resource->clear();
    spell_references->clear();

    for(int i = 0; i < static_cast<view1*>(&dbc)->get_size(); i++)
    {
        spell_references->push_back(static_cast<view1*>(&dbc)->get_ref(i));
        bool b = false;
        QString rank = " {";
        QString prefix(QString("(")+QString::number(static_cast<view1*>(&dbc)->get_ref(i))+QString(")\t"));
        if(QString(static_cast<view2*>(&dbc)->get_label(i)).length() != 0)
        {rank += static_cast<view2*>(&dbc)->get_label(i); b=true;}
        rank += "}";
        bool c = false;
        QString effect("");
        if(c = (static_cast<view3*>(&dbc)->get_label(i) == 36))    // 36 is LearnSpell effect
        {
            effect = QString(" {Learn Spell}");
        }
        QString tmp = prefix + QString(static_cast<view1*>(&dbc)->get_label(i));
        if(b){tmp += rank;}
        if(c){tmp += effect;}
        spell_resource->push_back(tmp);
    }
}

bool dbccache::spell_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::spellname view1;
    return static_cast<view1*>(&dbc)->is_valid();
}

void dbccache::standing_animation_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::emotes view;
    standing_animation_resource->clear();
    standing_animation_references->clear();
    standing_animation_resource->push_back("Standing Animation...");
    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        standing_animation_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        standing_animation_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
    }
}

bool dbccache::standing_animation_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::emotes view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::pet_spell_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::creaturespell1 spell1;
    typedef entityviews::creaturespell2 spell2;
    typedef entityviews::creaturespell3 spell3;
    typedef entityviews::spellname spellname;
    pet_spell p;

    pet_spell_resource->clear();
    pet_spell_plain->clear();
    pet_spell_references->clear();
    for(int i = 0; i < static_cast<spell1*>(&dbc)->get_size(); i++)
    {
        pet_spell_references->push_back(static_cast<spell1*>(&dbc)->get_ref(i));
        pet_spell_resource->push_back(QString::number(static_cast<spell1*>(&dbc)->get_ref(i)) + "\t" + p.getDisplay(i,dbc));
        pet_spell_plain->push_back(p.getDisplayShort(i,dbc));
    }
}

bool dbccache::pet_spell_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::creaturespell1 spell1;
    typedef entityviews::spellname spellname;
    return static_cast<spell1*>(&dbc)->is_valid() && static_cast<spellname*>(&dbc)->is_valid();
}

void dbccache::item_extended_cost_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::item1cost item1;
    typedef entityviews::item2cost item2;
    typedef entityviews::item3cost item3;
    typedef entityviews::item4cost item4;
    typedef entityviews::item5cost item5;
    item_extended_cost_item_entries->clear();
    item_extended_cost_references->clear();
    for(int i = 0; i < static_cast<item1*>(&dbc)->get_size(); i++)
    {
        item_extended_cost_references->push_back(static_cast<item1*>(&dbc)->get_ref(i));
        if(static_cast<item1*>(&dbc)->get_label(i) != 0)
        {
            item_extended_cost_item_entries->insert(static_cast<item1*>(&dbc)->get_label(i));
        }
        if(static_cast<item2*>(&dbc)->get_label(i) != 0)
        {
            item_extended_cost_item_entries->insert(static_cast<item2*>(&dbc)->get_label(i));
        }
        if(static_cast<item3*>(&dbc)->get_label(i) != 0)
        {
            item_extended_cost_item_entries->insert(static_cast<item3*>(&dbc)->get_label(i));
        }
        if(static_cast<item4*>(&dbc)->get_label(i) != 0)
        {
            item_extended_cost_item_entries->insert(static_cast<item4*>(&dbc)->get_label(i));
        }
        if(static_cast<item5*>(&dbc)->get_label(i) != 0)
        {
            item_extended_cost_item_entries->insert(static_cast<item5*>(&dbc)->get_label(i));
        }
    }
}

bool dbccache::item_extended_cost_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::honorcost honorcost;
    return static_cast<honorcost*>(&dbc)->is_valid();
}

bool dbccache::item_model::isMainHandSlot(int slot)
{
    return (slot == DBC_SLOT_MAINHAND) ||
           (slot == DBC_SLOT_ONEHAND)  ||
           (slot == DBC_SLOT_TWOHAND);
}

bool dbccache::item_model::isOffHandSlot(int slot)
{
    return (slot == DBC_SLOT_OFFHAND) ||
           (slot == DBC_SLOT_ONEHAND) ||
           (slot == DBC_SLOT_HELD)    ||
           (slot == DBC_SLOT_SHIELD);
}

bool dbccache::item_model::isRangedSlot(int slot)
{
    return (slot == DBC_SLOT_RANGED) ||
           (slot == DBC_SLOT_RANGED_)||
           (slot == DBC_SLOT_BOW)    ||
           (slot == DBC_SLOT_RELIC)  ||
           (slot == DBC_SLOT_THROWN);
}

bool dbccache::item_model::isChestSlot(int slot)
{
    return (slot == DBC_SLOT_ROBE) ||
           (slot == DBC_SLOT_VEST);
}

QString dbccache::item_model::displayItemEntry(int displayid, entityviews::dbcs_module_type &dbc)
{
    QString tmp("");
    bool hasModel = false;
    int index = static_cast<leftmodel*>(&dbc)->find_ref_binary(displayid);
    if(index == -1){ return tmp; }
    if(QString(static_cast<leftmodel*>(&dbc)->get_label(index)).length() != 0)
    {
        if(!hasModel){ tmp += "Model={"; }
        else{ tmp += ", "; }
        hasModel = true;
        tmp += QString(static_cast<leftmodel*>(&dbc)->get_label(index));
    }
    if(QString(static_cast<rightmodel*>(&dbc)->get_label(index)).length() != 0)
    {
        if(!hasModel){ tmp += "Model={"; }
        else{ tmp += ", "; }
        hasModel = true;
        tmp += QString(static_cast<rightmodel*>(&dbc)->get_label(index));
    }
    if(hasModel){ tmp += "} "; }
    bool hasTexture = false;
    if(QString(static_cast<lefttexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(!hasTexture){ tmp += "ModelTexture={"; }
        else{ tmp += ", "; }
        hasTexture = true;
        tmp += QString(static_cast<lefttexture*>(&dbc)->get_label(index));
    }
    if(QString(static_cast<righttexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(!hasTexture){ tmp += "ModelTexture={"; }
        else{ tmp += ", "; }
        hasTexture = true;
        tmp += QString(static_cast<righttexture*>(&dbc)->get_label(index));
    }
    if(hasTexture){ tmp += "} "; }
    bool hasBodyTexture = false;
    if(QString(static_cast<larmtexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<larmtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<uarmtexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<uarmtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<handtexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<handtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<ltorsotexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<ltorsotexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<utorsotexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<utorsotexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<llegtexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<llegtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<ulegtexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<ulegtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<foottexture*>(&dbc)->get_label(index)).length() != 0)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<foottexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(hasBodyTexture){ tmp += "} "; }
    //
    if(QString(static_cast<icon*>(&dbc)->get_label(index)).length() != 0)
    {
        tmp += "Icon={";
        tmp += QString(static_cast<icon*>(&dbc)->get_label(index));
        tmp += "}";
    }
    return tmp;
}

QString dbccache::item_model::displayItemEntryShort(int displayid, entityviews::dbcs_module_type &dbc)
{
    /* Order: Model->BodyTexture->Icon */
    QString tmp("");
    bool hasBodyTexture = false;
    int index = static_cast<leftmodel*>(&dbc)->find_ref_binary(displayid);
    if(QString(static_cast<leftmodel*>(&dbc)->get_label(index)).length() != 0)
    {
        tmp += "Model={";
        tmp += QString(static_cast<leftmodel*>(&dbc)->get_label(index));
        return tmp + "}";
    }
    if(QString(static_cast<rightmodel*>(&dbc)->get_label(index)).length() != 0)
    {
        tmp += "Model={";
        tmp += QString(static_cast<rightmodel*>(&dbc)->get_label(index));
        return tmp + "}";
    }
    if(QString(static_cast<larmtexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<larmtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<uarmtexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<uarmtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<handtexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<handtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<ltorsotexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<ltorsotexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<utorsotexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<utorsotexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<llegtexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<llegtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<ulegtexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<ulegtexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(QString(static_cast<foottexture*>(&dbc)->get_label(index)).length() != 0 && !hasBodyTexture)
    {
        if(hasBodyTexture){ tmp += ", "; }
        else              { tmp += "Texture={"; }
        tmp+=QString(static_cast<foottexture*>(&dbc)->get_label(index));
        hasBodyTexture = true;
    }
    if(hasBodyTexture)
    {
        return tmp + "}";
    }
    if(QString(static_cast<icon*>(&dbc)->get_label(index)).length() != 0)
    {
        tmp += "Icon={";
        tmp += QString(static_cast<icon*>(&dbc)->get_label(index));
        return tmp + "}";
    }
    return "";
}


bool dbccache::item_model::isCorrectSlot(int slot, int dbcslot)
{
    switch(slot)
    {
    case SLOT_NONE:         return dbcslot==DBC_SLOT_NONE;
    case SLOT_MAINHAND:     return isMainHandSlot(dbcslot);
    case SLOT_OFFHAND:      return isOffHandSlot(dbcslot);
    case SLOT_RANGED:       return isRangedSlot(dbcslot);
    case SLOT_HEAD:         return dbcslot==DBC_SLOT_HEAD;
    case SLOT_NECK:         return dbcslot==DBC_SLOT_NECK;
    case SLOT_SHOULDERS:    return dbcslot==DBC_SLOT_SHOULDERS;
    case SLOT_BACK:         return dbcslot==DBC_SLOT_BACK;
    case SLOT_CHEST:        return isChestSlot(dbcslot);
    case SLOT_SHIRT:        return dbcslot==DBC_SLOT_SHIRT;
    case SLOT_TABARD:       return dbcslot==DBC_SLOT_TABARD;
    case SLOT_WRISTS:       return dbcslot==DBC_SLOT_WRISTS;
    case SLOT_HANDS:        return dbcslot==DBC_SLOT_HANDS;
    case SLOT_WAIST:        return dbcslot==DBC_SLOT_WAIST;
    case SLOT_LEGS:         return dbcslot==DBC_SLOT_LEGS;
    case SLOT_FEET:         return dbcslot==DBC_SLOT_FEET;
    case SLOT_RING:         return dbcslot==DBC_SLOT_RING;
    case SLOT_TRINKET:      return dbcslot==DBC_SLOT_TRINKET;
    }
    return false;
}

void dbccache::item_model_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemleftmodel leftmodel;
    typedef entityviews::itemslot itemslot;
    typedef entityviews::itemdisplayid displayid;

    for(int i = 0; i < 18; i++)
    {
        item_model_resource[i]->clear();
        item_model_plain[i]->clear();
        item_model_item_references[i]->clear();
        item_model_display_references[i]->clear();
    }
    item_model m;
    for(int i = 0; i < static_cast<displayid*>(&dbc)->get_size(); i++)
    {
        for(int j = 0; j < 18; j++)
        {
            if(m.isCorrectSlot(j+1,static_cast<itemslot*>(&dbc)->get_label(i)))
            {
                item_model_resource[j]->push_back(m.displayItemEntry(static_cast<displayid*>(&dbc)->get_label(i),dbc));
                item_model_plain[j]->push_back(m.displayItemEntryShort(static_cast<displayid*>(&dbc)->get_label(i),dbc));
                item_model_item_references[j]->push_back(static_cast<displayid*>(&dbc)->get_ref(i));
                item_model_display_references[j]->push_back(static_cast<displayid*>(&dbc)->get_label(i));
            }
        }
    }
}

bool dbccache::item_model_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemleftmodel leftmodel;
    typedef entityviews::itemdisplayid displayid;
    return static_cast<leftmodel*>(&dbc)->is_valid() && static_cast<displayid*>(&dbc)->is_valid();
}

void dbccache::item_set_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemset itemset;

    item_set_resource->clear();
    item_set_references->clear();
    item_set_resource->push_back("Item Set...");
    for(int i = 0; i < static_cast<itemset*>(&dbc)->get_size(); i++)
    {
        item_set_references->push_back(static_cast<itemset*>(&dbc)->get_ref(i));
        item_set_resource->push_back(static_cast<itemset*>(&dbc)->get_label(i));
    }
}

bool dbccache::item_set_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemset itemset;
    return static_cast<itemset*>(&dbc)->is_valid();
}

void dbccache::title_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::title title;
    title_references->clear();
    title_resource->clear();

    title_resource->push_back("Title...");
    for(int i = 0; i < static_cast<title*>(&dbc)->get_size(); i++)
    {
        title_references->push_back(static_cast<title*>(&dbc)->get_ref(i));
        title_resource->push_back(static_cast<title*>(&dbc)->get_label(i));
    }
}

bool dbccache::title_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::title title;
    return static_cast<title*>(&dbc)->is_valid();
}

void dbccache::item_enchant_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemenchant view;
    item_enchant_resource->clear();
    item_enchant_references->clear();

    for(int i = 0; i < static_cast<view*>(&dbc)->get_size(); i++)
    {
        item_enchant_references->push_back(static_cast<view*>(&dbc)->get_ref(i));
        item_enchant_resource->push_back(static_cast<view*>(&dbc)->get_label(i));
    }
}

bool dbccache::item_enchant_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::itemenchant view;
    return static_cast<view*>(&dbc)->is_valid();
}

void dbccache::language_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::language language;
    language_resource->clear();
    language_references->clear();
    language_resource->push_back("Language...");
    for(int i = 0; i < static_cast<language*>(&dbc)->get_size(); i++)
    {
        language_references->push_back(static_cast<language*>(&dbc)->get_ref(i));
        language_resource->push_back(static_cast<language*>(&dbc)->get_label(i));
    }
}

bool dbccache::language_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::language language;
    return static_cast<language*>(&dbc)->is_valid();
}

void dbccache::totem_category_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::totemcategory totem;
    totem_category_resource->clear();
    totem_category_references->clear();
    totem_category_resource->push_back("Totem Category...");
    for(int i = 0; i < static_cast<totem*>(&dbc)->get_size(); i++)
    {
        totem_category_references->push_back(static_cast<totem*>(&dbc)->get_ref(i));
        totem_category_resource->push_back(static_cast<totem*>(&dbc)->get_label(i));
    }
}

bool dbccache::totem_category_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::totemcategory totem;
    return static_cast<totem*>(&dbc)->is_valid();
}

void dbccache::gem_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::gemcolor gemcolor;
    typedef entityviews::gemenchant gemenchant;
    typedef entityviews::itemenchant enchant;
    gem_references->clear();
    gem_resource->clear();

    for(int i = 0; i < static_cast<gemenchant*>(&dbc)->get_size(); i++)
    {
        gem_references->push_back(static_cast<gemenchant*>(&dbc)->get_ref(i));
        int spellenchantid = static_cast<gemenchant*>(&dbc)->get_label(i);
        int gemcolorid = static_cast<gemcolor*>(&dbc)->get_label(i);
        QString s;
        if(spellenchantid != 0)
        {
            int a = static_cast<enchant*>(&dbc)->find_ref_binary(spellenchantid);
            s += static_cast<enchant*>(&dbc)->get_label(a);
        }
        else
        {
            s = "No Stats";
        }
        s += " Gemcolor={";
        switch(gemcolorid)
        {
        case 1: s+= "Meta}";break;
        case 2: s+= "Red}";break;
        case 4: s+= "Yellow}";break;
        case 6: s+= "Orange}";break;
        case 8: s+= "Blue}";break;
        case 10: s+= "Purple}";break;
        case 12: s+= "Green}";break;
        default: s+= "No Color}";break;
        }
        gem_resource->push_back(s);
    }
}

bool dbccache::gem_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::gemcolor gemcolor;
    typedef entityviews::itemenchant enchant;
    return static_cast<gemcolor*>(&dbc)->is_valid() && static_cast<enchant*>(&dbc)->is_valid();
}

void dbccache::lock_type_init(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::lockskind      kinds;
    typedef entityviews::locksid        ids;
    lock_type_item_entries->clear();
    lock_type_references->clear();

    for(int i = 0; i < static_cast<kinds*>(&dbc)->get_size(); i++)
    {
        lock_type_references->push_back(static_cast<kinds*>(&dbc)->get_ref(i));

        for(int j = 0; j < 8; j++)
        {
            if(*(static_cast<kinds*>(&dbc)->get_label(i) + j) == 1)
            {
                lock_type_item_entries->insert(*(static_cast<ids*>(&dbc)->get_label(i) + j));
            }
        }
    }
}

bool dbccache::lock_type_valid(entityviews::dbcs_module_type &dbc)
{
    typedef entityviews::lockskind      kinds;
    typedef entityviews::locktype       types;
    return static_cast<kinds*>(&dbc)->is_valid() && static_cast<types*>(&dbc)->is_valid();
}

void dbccache::loadDBC(entityviews::dbcs_module_type &dbc)
{
    if(creature_family_valid(dbc))
        creature_family_init(dbc);
    if(creature_type_valid(dbc))
        creature_type_init(dbc);
    if(skillline_valid(dbc))
        skillline_init(dbc);
    if(faction_valid(dbc))
        faction_init(dbc);
    if(reputation_faction_valid(dbc))
        reputation_faction_init(dbc);
    // item_class must be initialized before item_subclass
    if(item_class_valid(dbc))
    {
        item_class_init(dbc);
        if(item_subclass_valid(dbc))
            item_subclass_init(dbc);
    }
    //
    if(map_valid(dbc))
        map_init(dbc);
    if(area_valid(dbc))
       area_init(dbc);
    if(model_valid(dbc))
        model_init(dbc);
    if(spell_valid(dbc))
        spell_init(dbc);
    if(standing_animation_valid(dbc))
        standing_animation_init(dbc);
    if(pet_spell_valid(dbc))
        pet_spell_init(dbc);
    if(item_extended_cost_valid(dbc))
        item_extended_cost_init(dbc);
    if(item_model_valid(dbc))
        item_model_init(dbc);
    if(item_set_valid(dbc))
        item_set_init(dbc);
    if(title_valid(dbc))
        title_init(dbc);
    if(item_enchant_valid(dbc))
        item_enchant_init(dbc);
    if(language_valid(dbc))
        language_init(dbc);
    if(totem_category_valid(dbc))
        totem_category_init(dbc);
    if(gem_valid(dbc))
        gem_init(dbc);
    if(lock_type_valid(dbc))
        lock_type_init(dbc);
}

void dbccache::reloadDBC(entityviews::dbcs_module_type &dbc)
{
    loadDBC(dbc);
}
