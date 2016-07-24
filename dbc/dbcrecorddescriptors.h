#ifndef DBCRECORDDESCRIPTORS_H
#define DBCRECORDDESCRIPTORS_H

#include "dbcfile.h"
#include "dbcparse.h"

/* Describe all used/known files here. */

typedef dbcfield<4, fieldtype::INTPTR> intptr;
typedef dbcfield<4, fieldtype::INT> id;
typedef dbcfield<4, fieldtype::INT> int_;
typedef dbcfield<8, fieldtype::INT> int2;
typedef dbcfield<16, fieldtype::INT> int4;
typedef dbcfield<28, fieldtype::INT> int7;
typedef dbcfield<32, fieldtype::INT> int8;
typedef dbcfield<48, fieldtype::INT> int12;
typedef dbcfield<4*21, fieldtype::INT> int21;
typedef dbcfield<4*35, fieldtype::INT> int35;
typedef dbcfield<4*59, fieldtype::INT> int59;
typedef dbcfield<4*63, fieldtype::INT> int63;
typedef dbcfield<4*126, fieldtype::INT> int126;
typedef dbcfield<4, fieldtype::INT> bool_;
typedef dbcfield<4, fieldtype::FLOAT> float_;
typedef dbcfield<8, fieldtype::FLOAT> float2;
typedef dbcfield<12,fieldtype::FLOAT> float3;
typedef dbcfield<4, fieldtype::STRING> str;
typedef dbcfield<4*17, fieldtype::STRING> localization;
typedef dbcfield<4, fieldtype::INT>  noXp;

namespace burning_crusade_records
{
    typedef dbcrecord<recordid::creature_type, id, localization, noXp>  creature_type;
    typedef dbcrecord<recordid::map,
                        id,             /*(0) ID*///1
                        str,            /*(1) InternalName*///2
                        int_,           /*(2) AreaType*///3
                        bool_,          /*(3) IsBattleground*///4
                        localization,   /*(4) Name*///21
                        int_,           /*(5) BGMinLvl*/
                        int_,           /*(6) BGMaxLvl*/
                        int_,           /*(7) BGMaxPlayers*/
                        int_,           /*(8) Unknown*/
                        float_,         /*(9) Unknown*/
                        float_,         /*(10)Unknown*/
                        id,             /*(11) AreaTableID*///28
                        localization,   /*(12) AllianceText*///45
                        localization,   /*(13) HordeText*///62
                        id,             /*(14) LoadingScreenID*///63
                        int_,           /*(15) Unknown*///64
                        int_,           /*(16) Unknown*///65
                        float_,         /*(17) BattlefFieldMapIconScale*///66
                        localization,   /*(18)Requirement*///83
                        localization,   /*(19)HeroicRequirement*///100
                        localization,   /*(20)OtherRequirements*///117
                        id,             /*(21)ParentArea*///118
                        float_,         /*(22)X-InstanceEntrance*///119
                        float_,         /*(23)Y-InstanceEntrance*///120
                        int_,           /*(24)RaidResetTime*///121
                        int_,           /*(25)HeroicRaidResetTime*///122
                        int_,           /*(26)Unknown*///123
                        int_,           /*(27)TimeOfDayOverride*///124
                        int_            /*(28)ExpansionID*///125
            > map;
    typedef dbcrecord<recordid::areatable,
                        id,             /*(0) ID*/
                        id,             /*(1) MapID*/
                        id,             /*(2) ParentArea*/
                        int_,           /*(3) AreaBit*/
                        int_,           /*(4) Flags*/
                        id,             /*(5) SoundPreferences*/
                        id,             /*(6) SoundPreferencesUnderwater*/
                        id,             /*(7) SoundAmbiance*/
                        id,             /*(8) ZoneMusic*/
                        id,             /*(9) ZoneIntroMusic*/
                        int_,           /*(10)ExplorationLevel*/
                        localization,   /*(11)Name*/
                        id,             /*(12)FactionGroup*/
                        id,             /*(13)WaterType*/
                        id,             /*(14)OceanType*/
                        id,             /*(15)MagmaType*/
                        id,             /*(16)SlimeType*/
                        float_,         /*(17)MinElevation*/
                        float_          /*(18)AmbientMultiplier*/
            > areatable;
    typedef dbcrecord<recordid::creature_family,
                        id,             /*(0) ID*/
                        float_,         /*(1) MinScale*/
                        int_,           /*(2) MinScaleLevel*/
                        float_,         /*(3) MaxScale*/
                        int_,           /*(4) MaxScaleLevel*/
                        id,             /*(5) SkillLine*/
                        id,             /*(6) ItemPetFoot*/
                        int_,           /*(7) PetTalentType*/
                        localization,   /*(8)Name*/
                        str             /*(9)IconFile*/
            > creature_family;
    typedef dbcrecord<recordid::faction,
                        id,             /*(0) ID*/
                        int_,           /*(1) ReputationIndex*/
                        int_,           /*(2) ReputationAtWar*/
                        int_,           /*(3) ReputationAllied*/
                        int_,           /*(4) RepRaceMask*/
                        int_,           /*(5) RepRaceMask*/
                        int_,           /*(6) RepRaceMask*/
                        int_,           /*(7) RepRaceMask*/
                        int_,           /*(8) RepRaceMask*/
                        int_,           /*(9) RepRaceMask*/
                        int_,           /*(10)ReputationBase*/
                        int_,           /*(11)ReputationFlags*/
                        id,             /*(12)ParentFactionID*/
                        int_,           /*(13)Unknown*/
                        int_,           /*(14)Unknown*/
                        float2,         /*(15)ParentFactionMod*/
                        int2,           /*(16)ParentFactionCap*/
                        localization,   /*(17)Name*/
                        localization    /*(18)Description*/
            > faction;
    typedef dbcrecord<recordid::factiontemplate,
                        id,             /*(0) ID*/
                        id,             /*(1) FactionID*/
                        int_,           /*(2) Flags*/
                        int_,           /*(3) OurMask*/
                        int_,           /*(4) FriendlyMask */
                        int_,           /*(5) HostileMask*/
                        int8            /*(6) Rest*/
            > factiontemplate;
    typedef dbcrecord<recordid::modelinfo,
                        id,             /*(0) ID*/
                        id,             /*(1) Model*/
                        id,             /*(2) Sound*/
                        id,             /*(3) ExtraDisplayId*/
                        float_,         /*(4) Scale*/
                        int_,           /*(5) Opacity*/
                        str,            /*(6) Skin1*/
                        str,            /*(7) Skin2*/
                        str,            /*(8) Skin3*/
                        str,            /*(9) PortraitTexture*/
                        int_,           /*(10)BloodLevel*/
                        int_,           /*(11)Blood*/
                        int_,           /*(12)NPCSounds*/
                        int_            /*(13)Particles*/
            > modelinfo;
    typedef dbcrecord<recordid::modeldata,
                        id,             /*(0) ID*/
                        int_,           /*(1) Flags*/
                        str,            /*(2) ModelPath*/
                        str,            /*(3) AlternateModel*/
                        float_,         /*(4) ModelScale*/
                        id,             /*(5) BloodLevel*/
                        id,             /*(6) Footprint*/
                        float_,         /*(7) ftprntTextureLength*/
                        float_,         /*(8) ftprntTextureWidth*/
                        float_,         /*(9) ftprntParticleScale*/
                        int_,           /*(10)FoleyMaterial*/
                        id,             /*(11)FootStepShakeSize*/
                        int_,           /*(12)deathThudShakeSize*/
                        id,             /*(13)SoundData*/
                        float_,         /*(14)CollisionWidth*/
                        float_,         /*(15)CollisionHeight*/
                        float_,         /*(16)MountHeight*/
                        float3,         /*(17)GeoBoxMin*/
                        float3,         /*(18)GeoBoxMax*/
                        float_          /*(19)WorldEffectScale*/
            > modeldata;
    typedef dbcrecord<recordid::spell,
                        id,             /*(0) ID*/
                        int_,           /*(1) Category*/
                        int63,          /*(1) DontCare1*/
                        int_,           /*(2) SpellEffect1*/
                        int_,           /*(3) SpellEffect2*/
                        int_,           /*(4) SpellEffect3*/
                        int59,          /*(5) DontCare2*/
                        localization,   /*(6) Name*/
                        localization,   /*(7) Rank*/
                        localization,   /*(8) Explanation*/
                        localization,   /*(9) Tooltip*/
                        int21           /*(10)DontCare3*/
            > spells;
    typedef dbcrecord<recordid::spellitemenchant,
                        id,             /*(0) ID*/
                        int12,          /*(1) BLOB*/
                        localization,   /*(2) Label*/
                        int4            /*(3) BLOB1*/
            > spellitemenchant;
    typedef dbcrecord<recordid::emote,
                        id,             /*(0) ID*/
                        str,            /*(1) Name*/
                        int4,           /*(2) Data*/
                        int_            /*(3) MoreData*/
            > emotes;
    typedef dbcrecord<recordid::creaturespells,
                        id,             /*(0) ID*/
                        int_,           /*(1) Spell1*/
                        int_,           /*(2) Spell2*/
                        int_,           /*(3) Spell3*/
                        int4,           /*(4) Data*/
                        int_            /*(5) MoreData*/
            > creaturespells;
    typedef dbcrecord<recordid::skillline,
                        id,             /*(0) ID*/
                        id,             /*(1) Category*/
                        id,             /*(2) CostID*/
                        localization,   /*(3) Name*/
                        localization,   /*(4) Description*/
                        int_            /*(5) CanLink*/
            > skillline;
    typedef dbcrecord<recordid::itemextendedcost,
                        id,             /*(0) ID*/
                        int_,           /*(1) HonorCost*/
                        int_,           /*(2) ArenaCost*/
                        int_,           /*(3) Item1*/
                        int_,           /*(4) Item2*/
                        int_,           /*(5) Item3*/
                        int_,           /*(6) Item4*/
                        int_,           /*(7) Item5*/
                        int_,           /*(8) Item1Count*/
                        int_,           /*(9) Item2Count*/
                        int_,           /*(10)Item3Count*/
                        int_,           /*(11)Item4Count*/
                        int_,           /*(12)Item5Count*/
                        int_            /*(13)RatingRequired*/
            > itemextendedcost;
    typedef dbcrecord<recordid::item,
                        id,             /*(0) ID*/
                        id,             /*(1) ItemDisplayID*/
                        int_,           /*(2) InventorySlotID*/
                        int_            /*(3) ItemClass*/
            > items;
    typedef dbcrecord<recordid::itemset,
                        id,             /*(0) ID*/
                        localization,   /*(1) Name*/
                        int35           /*(2) Rest*/
            > itemsets;
    typedef dbcrecord<recordid::itemdisplay,
                        id,             /*(0) ID*/
                        str,            /*(1) LeftModel*/
                        str,            /*(2) RightModel*/
                        str,            /*(3) LeftModelTexture*/
                        str,            /*(4) RightModelTexture*/
                        str,            /*(5) Icon*/
                        int8,           /*(6) DontCare1*/
                        int_,           /*(7) DontCare2*/
                        str,            /*(8) UpperArmTexture*/
                        str,            /*(9) LowerArmTexture*/
                        str,            /*(10)HandsTexture*/
                        str,            /*(11)UpperTorsoTexture*/
                        str,            /*(12)LowerTorsoTexture*/
                        str,            /*(13)UpperLegTexture*/
                        str,            /*(14)LowerLegTexture*/
                        str,            /*(15)FootTexture*/
                        id,             /*(16)VisualEnchantID*/
                        int_            /*(17)ParticleColor*/
            > itemdisplays;
    typedef dbcrecord<recordid::itemclass,
                        id,             /*(0) ID*/
                        int_,           /*(1) SubClass*/
                        int_,           /*(2) IsWpn*/
                        localization    /*(3) Label*/
            > itemclasses;
    typedef dbcrecord<recordid::itemsubclass,
                        id,             /*(0) ItemClassID*/
                        id,             /*(1) SubClassID*/
                        int_,           /*(2) PreReqProf*/
                        int_,           /*(3) PostReqProf*/
                        int_,           /*(4) Flags*/
                        int_,           /*(5) DisplayFlags*/
                        int_,           /*(6) WpnParrySeq*/
                        int_,           /*(7) WpnRdySeq*/
                        int_,           /*(8) WpnAtkSeq*/
                        int_,           /*(9) WpnSwingSize*/
                        localization,   /*(10)DisplayName*/
                        localization    /*(11)VerboseName*/
            > itemsubclasses;
    typedef dbcrecord<recordid::itemrandomproperties,
                        id,             /*(0) ID*/
                        str,            /*(1) Label*/
                        intptr,         /*(2) ItemEnchant1*/
                        int_,           /*(3) ItemEnchant2*/
                        int_,           /*(4) ItemEnchant3*/
                        int_,           /*(5) ItemEnchant4*/
                        int_,           /*(6) ItemEnchant5*/
                        localization    /*(7) ProperLabel*/
            >  itemrandomproperties;
    typedef dbcrecord<recordid::itemrandomsuffix,
                        id,             /*(0) ID*/
                        localization,   /*(1) ProperLabel*/
                        str,            /*(2) Name*/
                        intptr,         /*(3) ItemEnchant1*/
                        int_,           /*(4) ItemEnchant2*/
                        int_,           /*(5) ItemEnchant3*/
                        int_,           /*(6) Alloc1?*/
                        int_,           /*(7) Alloc2?*/
                        int_            /*(8) Alloc3?*/
            > itemrandomsuffix;
    typedef dbcrecord<recordid::gemproperties,
                        id,             /*(0) ID*/
                        int_,           /*(1) Enchant*/
                        int_,           /*(2) MaxCountInv*/
                        int_,           /*(3) MaxCountItem*/
                        int_            /*(4) ColorMask*/
            > gemproperties;
    typedef dbcrecord<recordid::title,
                        id,             /*(0) ID*/
                        int_,           /*(1) Cond*/
                        localization,   /*(2) MaleTitle*/
                        localization,   /*(3) FemaleTitle*/
                        int_            /*(4) TitleMaskId*/
            > titles;
    typedef dbcrecord<recordid::language,
                        id,             /*(0) ID*/
                        localization    /*(1) Label*/
            > languages;
    typedef dbcrecord<recordid::totemcategory,
                        id,             /*(0) ID*/
                        localization,   /*(1) Name*/
                        int_,           /*(2) Category*/
                        int_            /*(3) Mask*/
            > totemcategories;
    typedef dbcrecord<recordid::lock,
                        id,             /*(0) ID*/
                        intptr,         /*(1) isItemOrTypeBEGIN*/
                        int7,           /*(2) isItemOrType*/
                        intptr,         /*(3) ItemOrTypeBEGIN*/
                        int7,           /*(4) ItemOrType*/
                        intptr,         /*(5) CorrespondingSkillBEGIN*/
                        int7,           /*(6) CorrespondingSkill*/
                        intptr,         /*(7) Action?BEGIN*/
                        int7            /*(8) Action?*/
            > locks;
    typedef dbcrecord<recordid::locktype,
                        id,             /*(0) ID*/
                        localization,   /*(1) Label*/
                        localization,   /*(2) SubjectLabel*/
                        localization,   /*(3) ActionLabel*/
                        str             /*(4) InternalName*/
            > locktypes;
    typedef dbcentity<triple_tvv<locks,1,0>,triple_tvv<locks,0,0>> lockskind_entity;
    typedef dbcentity<triple_tvv<locks,3,0>,triple_tvv<locks,0,0>> locksid_entity;
    typedef dbcentity<triple_tvv<locks,5,0>,triple_tvv<locks,0,0>> lockskill_entity;
    typedef dbcentity<triple_tvv<locktypes,1,0>,triple_tvv<locktypes,0,0>> locktype_entity;
    typedef dbcentity<triple_tvv<itemrandomproperties,7,0>,triple_tvv<itemrandomproperties,0,0>> itemrandompropertyname_entity;
    typedef dbcentity<triple_tvv<itemrandomproperties,2,0>,triple_tvv<itemrandomproperties,0,0>> itemrandompropertyencants_entity;
    typedef dbcentity<triple_tvv<itemrandomsuffix,1,0>,triple_tvv<itemrandomsuffix,0,0>> itemrandomsuffixname_entity;
    typedef dbcentity<triple_tvv<itemrandomsuffix,3,0>,triple_tvv<itemrandomsuffix,0,0>> itemrandomsuffixenchants_entity;
    typedef dbcentity<triple_tvv<gemproperties,1,0>,triple_tvv<gemproperties,0,0>> gempropertyenchant_entity;
    typedef dbcentity<triple_tvv<gemproperties,4,0>,triple_tvv<gemproperties,0,0>> gempropertycolor_entity;
    typedef dbcentity<triple_tvv<totemcategories,1,0>,triple_tvv<totemcategories,0,0>> totemcategory_entity;
    typedef dbcentity<triple_tvv<languages,1,0>,triple_tvv<languages,0,0>> language_entity;
    typedef dbcentity<triple_tvv<spellitemenchant,2,0>,triple_tvv<spellitemenchant,0,0>> spellitemenchant_entity;
    typedef dbcentity<triple_tvv<titles,1,0>,triple_tvv<titles,0,0>> titlecondition_entity;
    typedef dbcentity<triple_tvv<titles,2,0>,triple_tvv<titles,0,0>> title_entity;
    typedef dbcentity<triple_tvv<itemclasses,3,0>,triple_tvv<itemclasses,0,0>> itemclassname_entity;
    typedef dbcentity<triple_tvv<itemsubclasses,10,0>,triple_tvv<itemsubclasses,0,0>> itemsubclassname_entity;
    typedef dbcentity<triple_tvv<itemsubclasses,11,0>,triple_tvv<itemsubclasses,0,0>> itemsubclassnameext_entity;
    typedef dbcentity<triple_tvv<itemsubclasses,1,0>,triple_tvv<itemsubclasses,0,0>> itemsubclassid_entity;
    typedef dbcentity<triple_tvv<items,1,0>, triple_tvv<items,0,0>> itemdisplay_entity;
    typedef dbcentity<triple_tvv<items,2,0>, triple_tvv<items,0,0>> itemslot_entity;
    typedef dbcentity<triple_tvv<itemsets,1,0>, triple_tvv<itemsets,0,0>> itemset_entity;
    typedef dbcentity<triple_tvv<itemdisplays,1,0>,triple_tvv<itemdisplays,0,0>> itemleftmodel_entity;
    typedef dbcentity<triple_tvv<itemdisplays,2,0>,triple_tvv<itemdisplays,0,0>> itemrightmodel_entity;
    typedef dbcentity<triple_tvv<itemdisplays,3,0>,triple_tvv<itemdisplays,0,0>> itemleftmodeltexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,4,0>,triple_tvv<itemdisplays,0,0>> itemrightmodeltexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,8,0>,triple_tvv<itemdisplays,0,0>> upperarmtexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,9,0>,triple_tvv<itemdisplays,0,0>> lowerarmtexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,10,0>,triple_tvv<itemdisplays,0,0>> handtexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,11,0>,triple_tvv<itemdisplays,0,0>> uppertorsotexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,12,0>,triple_tvv<itemdisplays,0,0>> lowertorsotexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,13,0>,triple_tvv<itemdisplays,0,0>> upperlegtexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,14,0>,triple_tvv<itemdisplays,0,0>> lowerlegtexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,15,0>,triple_tvv<itemdisplays,0,0>> foottexture_entity;
    typedef dbcentity<triple_tvv<itemdisplays,5,0>,triple_tvv<itemdisplays,0,0>> itemicon_entity;

    typedef dbcentity<triple_tvv<creature_type,1,0>, triple_tvv<creature_type,0,0>> creature_type_entity;
    typedef dbcentity<triple_tvv<creature_family,8,0>, triple_tvv<creature_family,0,0>> creature_family_entity;
    typedef dbcentity<triple_tvv<map,4,0>,triple_tvv<map,0,0>>                      map_name_entity;
    typedef dbcentity<triple_tvv<areatable,11,0>,triple_tvv<areatable,0,0>>         area_name_entity;
    typedef dbcentity<triple_tvv<faction,17,0>,triple_tvv<faction,0,0>>             faction_name_entity;
    typedef dbcentity<triple_tvv<faction,1,0>,triple_tvv<faction,0,0>>              repfaction_entity;
    typedef dbcentity<triple_tvv<factiontemplate,1,0>,triple_tvv<factiontemplate,0,0>>      faction_of_template_entity;
    typedef dbcentity<triple_tvv<factiontemplate,2,0>,triple_tvv<factiontemplate,0,0>>      faction_flags_entity;
    typedef dbcentity<triple_tvv<factiontemplate,3,0>,triple_tvv<factiontemplate,0,0>>      faction_our_mask_entity;
    typedef dbcentity<triple_tvv<factiontemplate,4,0>,triple_tvv<factiontemplate,0,0>>      faction_friendly_mask_entity;
    typedef dbcentity<triple_tvv<factiontemplate,5,0>,triple_tvv<factiontemplate,0,0>>      faction_hostile_mask_entity;

    typedef dbcentity<triple_tvv<modelinfo,1,0>,triple_tvv<modelinfo,0,0>>          modelid_of_modelinfo_entity;
    typedef dbcentity<triple_tvv<modeldata,2,0>,triple_tvv<modeldata,0,0>>          modelpath_of_modeldata_entity;
    typedef dbcentity<triple_tvv<modelinfo,6,0>,triple_tvv<modelinfo,0,0>>          modelskin1_entity;
    typedef dbcentity<triple_tvv<modelinfo,7,0>,triple_tvv<modelinfo,0,0>>          modelskin2_entity;
    typedef dbcentity<triple_tvv<modelinfo,8,0>,triple_tvv<modelinfo,0,0>>          modelskin3_entity;

    typedef dbcentity<triple_tvv<spells,7,0>,triple_tvv<spells,0,0>>                spellname_entity;
    typedef dbcentity<triple_tvv<spells,8,0>,triple_tvv<spells,0,0>>                spellrank_entity;
    typedef dbcentity<triple_tvv<spells,3,0>,triple_tvv<spells,0,0>>                spelleffect1_entity;
    typedef dbcentity<triple_tvv<spells,1,0>,triple_tvv<spells,0,0>>                spellcategory_entity;


    typedef dbcentity<triple_tvv<creaturespells,1,0>,triple_tvv<creaturespells,0,0>>        creaturespell1_entity;
    typedef dbcentity<triple_tvv<creaturespells,2,0>,triple_tvv<creaturespells,0,0>>        creaturespell2_entity;
    typedef dbcentity<triple_tvv<creaturespells,3,0>,triple_tvv<creaturespells,0,0>>        creaturespell3_entity;

    typedef dbcentity<triple_tvv<emotes,1,0>,triple_tvv<emotes,0,0>>                emotes_entity;
    typedef dbcentity<triple_tvv<skillline,3,0>,triple_tvv<skillline,0,0>>          skillline_entity;

    typedef dbcentity<triple_tvv<itemextendedcost,1,0>,triple_tvv<itemextendedcost,0,0>>     honorcost_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,2,0>,triple_tvv<itemextendedcost,0,0>>     arenacost_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,3,0>,triple_tvv<itemextendedcost,0,0>>     item1cost_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,4,0>,triple_tvv<itemextendedcost,0,0>>     item2cost_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,5,0>,triple_tvv<itemextendedcost,0,0>>     item3cost_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,6,0>,triple_tvv<itemextendedcost,0,0>>     item4cost_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,7,0>,triple_tvv<itemextendedcost,0,0>>     item5cost_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,8,0>,triple_tvv<itemextendedcost,0,0>>     item1count_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,9,0>,triple_tvv<itemextendedcost,0,0>>     item2count_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,10,0>,triple_tvv<itemextendedcost,0,0>>    item3count_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,11,0>,triple_tvv<itemextendedcost,0,0>>    item4count_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,12,0>,triple_tvv<itemextendedcost,0,0>>    item5count_entity;
    typedef dbcentity<triple_tvv<itemextendedcost,13,0>,triple_tvv<itemextendedcost,0,0>>    ratingreq_entity;

    namespace entityviews
    {
        typedef tuple_t<    dbc<creature_type>,
                            dbc<map>,
                            dbc<areatable>,
                            dbc<creature_family>,
                            dbc<faction>,
                            dbc<factiontemplate>,
                            dbc<modelinfo>,
                            dbc<modeldata>,
                            dbc<spells>,
                            dbc<emotes>,
                            dbc<creaturespells>,
                            dbc<skillline>,
                            dbc<itemextendedcost>,
                            dbc<items>,
                            dbc<itemdisplays>,
                            dbc<itemclasses>,
                            dbc<itemsubclasses>,
                            dbc<itemrandomproperties>,
                            dbc<itemrandomsuffix>,
                            dbc<gemproperties>,
                            dbc<titles>,
                            dbc<spellitemenchant>,
                            dbc<languages>,
                            dbc<itemsets>,
                            dbc<totemcategories>,
                            dbc<locks>,
                            dbc<locktypes>
        > all_dbcs;
        typedef dbcs_map<all_dbcs> dbcs_module_type;
        typedef dbcentityview<creature_type_entity, all_dbcs> creature_type;
        typedef dbcentityview<map_name_entity, all_dbcs> map_name;
        typedef dbcentityview<area_name_entity, all_dbcs> area_name;
        typedef dbcentityview<creature_family_entity, all_dbcs> creature_family;
        typedef dbcentityview<faction_name_entity, all_dbcs> faction_name;
        typedef dbcentityview<faction_flags_entity, all_dbcs> faction_flags;
        typedef dbcentityview<faction_our_mask_entity, all_dbcs> faction_our_mask;
        typedef dbcentityview<faction_friendly_mask_entity, all_dbcs> faction_friendly_mask;
        typedef dbcentityview<faction_hostile_mask_entity, all_dbcs> faction_hostile_mask;
        typedef dbcentityview<faction_of_template_entity, all_dbcs> faction;
        typedef dbcentityview<repfaction_entity, all_dbcs> repfaction;
        typedef dbcentityview<modelid_of_modelinfo_entity, all_dbcs> model_info;
        typedef dbcentityview<modelpath_of_modeldata_entity, all_dbcs> model_path;
        typedef dbcentityview<modelskin1_entity, all_dbcs> modelskin1;
        typedef dbcentityview<modelskin2_entity, all_dbcs> modelskin2;
        typedef dbcentityview<modelskin3_entity, all_dbcs> modelskin3;
        typedef dbcentityview<spellname_entity, all_dbcs> spellname;
        typedef dbcentityview<spellrank_entity, all_dbcs> spellrank;
        typedef dbcentityview<spelleffect1_entity, all_dbcs> spelleffect1;
        typedef dbcentityview<spellcategory_entity, all_dbcs> spellcategory;
        typedef dbcentityview<emotes_entity, all_dbcs> emotes;
        typedef dbcentityview<creaturespell1_entity, all_dbcs> creaturespell1;
        typedef dbcentityview<creaturespell2_entity, all_dbcs> creaturespell2;
        typedef dbcentityview<creaturespell3_entity, all_dbcs> creaturespell3;
        typedef dbcentityview<skillline_entity, all_dbcs> skilltype_name;
        typedef dbcentityview<item1cost_entity, all_dbcs> item1cost;
        typedef dbcentityview<item2cost_entity, all_dbcs> item2cost;
        typedef dbcentityview<item3cost_entity, all_dbcs> item3cost;
        typedef dbcentityview<item4cost_entity, all_dbcs> item4cost;
        typedef dbcentityview<item5cost_entity, all_dbcs> item5cost;
        typedef dbcentityview<item1count_entity, all_dbcs> item1count;
        typedef dbcentityview<item2count_entity, all_dbcs> item2count;
        typedef dbcentityview<item3count_entity, all_dbcs> item3count;
        typedef dbcentityview<item4count_entity, all_dbcs> item4count;
        typedef dbcentityview<item5count_entity, all_dbcs> item5count;
        typedef dbcentityview<honorcost_entity, all_dbcs> honorcost;
        typedef dbcentityview<arenacost_entity, all_dbcs> arenacost;
        typedef dbcentityview<ratingreq_entity, all_dbcs> ratingrequired;
        typedef dbcentityview<itemdisplay_entity, all_dbcs> itemdisplayid;
        typedef dbcentityview<itemslot_entity, all_dbcs> itemslot;
        typedef dbcentityview<itemset_entity, all_dbcs> itemset;
        typedef dbcentityview<itemleftmodel_entity, all_dbcs> itemleftmodel;
        typedef dbcentityview<itemrightmodel_entity, all_dbcs> itemrightmodel;
        typedef dbcentityview<itemleftmodeltexture_entity, all_dbcs> itemlefttexture;
        typedef dbcentityview<itemrightmodeltexture_entity, all_dbcs> itemrighttexture;
        typedef dbcentityview<upperarmtexture_entity, all_dbcs> upperarmtexture;
        typedef dbcentityview<lowerarmtexture_entity, all_dbcs> lowerarmtexture;
        typedef dbcentityview<handtexture_entity, all_dbcs> handtexture;
        typedef dbcentityview<uppertorsotexture_entity, all_dbcs> uppertorsotexture;
        typedef dbcentityview<lowertorsotexture_entity, all_dbcs> lowertorsotexture;
        typedef dbcentityview<upperlegtexture_entity, all_dbcs> upperlegtexture;
        typedef dbcentityview<lowerlegtexture_entity, all_dbcs> lowerlegtexture;
        typedef dbcentityview<foottexture_entity, all_dbcs> foottexture;
        typedef dbcentityview<itemicon_entity, all_dbcs> itemicon;
        typedef dbcentityview<itemclassname_entity, all_dbcs> itemclassname;
        typedef dbcentityview<itemsubclassname_entity, all_dbcs> itemsubclassname;
        typedef dbcentityview<itemsubclassnameext_entity, all_dbcs> itemsubclassnameext;
        typedef dbcentityview<itemsubclassid_entity, all_dbcs> itemsubclassid;
        typedef dbcentityview<title_entity, all_dbcs> title;
        typedef dbcentityview<titlecondition_entity, all_dbcs> titlecondition;
        typedef dbcentityview<spellitemenchant_entity, all_dbcs> itemenchant;
        typedef dbcentityview<language_entity, all_dbcs> language;
        typedef dbcentityview<totemcategory_entity, all_dbcs> totemcategory;
        typedef dbcentityview<gempropertyenchant_entity, all_dbcs> gemenchant;
        typedef dbcentityview<gempropertycolor_entity, all_dbcs> gemcolor;
        typedef dbcentityview<itemrandompropertyname_entity, all_dbcs> itemrandompropertyname;
        typedef dbcentityview<itemrandompropertyencants_entity, all_dbcs> itemrandompropertyenchants;
        typedef dbcentityview<itemrandomsuffixname_entity, all_dbcs> itemrandomsuffixname;
        typedef dbcentityview<itemrandomsuffixenchants_entity, all_dbcs> itemrandomsuffixenchants;
        typedef dbcentityview<lockskind_entity, all_dbcs> lockskind;
        typedef dbcentityview<locksid_entity, all_dbcs> locksid;
        typedef dbcentityview<lockskill_entity, all_dbcs> lockskill;
        typedef dbcentityview<locktype_entity, all_dbcs> locktype;
    }
}

#endif // DBCFILEDESCRIPTORS_H
