#include "precompiled.h"

/*#####
# item_wow4you_tabard
#####*/

enum
{
    SPELL_TABARD_OF_THE_LIGHTBRINGER = 73326,
    SPELL_PURPLE_FIREWORK = 30161,
    SPELL_TEST_TICKLE = 5025,
    SPELL_MEAT_SLAP = 37597,
    SPELL_I_CANT_DRIVE_55 = 31565,
    SPELL_BLOATED = 49822,
    SPELL_ZOMBIE_FORM = 7293,
    SPELL_LICH_SLAP = 60924,
    SPELL_GOBLIN_BINARY = 50246,
    SPELL_GNOMISH_BINARY = 50242,
    SPELL_GROUND_SLAM = 33525,
    SPELL_BLACKWHELP_TRANSFORMATION = 38318,
    SPELL_SUICIDE = 27965,
    SPELL_FROG_FORM = 3329,
    SPELL_RABBIT_FORM = 61716,
    SPELL_DIRE_WOLF_FORM = 22660,
};

#define MAX_SPELLS 16

enum
{
    DAY_SUNDAY = 1,
    DAY_MONDAY = 2,
    DAY_TUESDAY = 4,
    DAY_WEDNESDAY = 8,
    DAY_THURSDAY = 16,
    DAY_FRIDAY = 32,
    DAY_SATURDAY = 64,

    DAY_EVERY_DAY = DAY_SUNDAY | DAY_MONDAY | DAY_TUESDAY |
                    DAY_WEDNESDAY | DAY_THURSDAY | DAY_FRIDAY | DAY_SATURDAY,
};

enum
{
    HOUR_MIDNIGHT = 1,
    HOUR_1 = 2,
    HOUR_2 = 4,
    HOUR_3 = 8,
    HOUR_4 = 16,
    HOUR_5 = 32,
    HOUR_6 = 64,
    HOUR_7 = 128,
    HOUR_8 = 256,
    HOUR_9 = 512,
    HOUR_10 = 1024,
    HOUR_11 = 2048,
    HOUR_12 = 4096,
    HOUR_13 = 8192,
    HOUR_14 = 16384,
    HOUR_15 = 32768,
    HOUR_16 = 65536,
    HOUR_17 = 131072,
    HOUR_18 = 262144,
    HOUR_19 = 524288,
    HOUR_20 = 1048576,
    HOUR_21 = 2097152,
    HOUR_22 = 4194304,
    HOUR_23 = 8388608,

    HOUR_EVERY_HOUR = HOUR_MIDNIGHT | HOUR_1 | HOUR_2 | HOUR_3 | HOUR_4 | HOUR_5 |
                      HOUR_6 | HOUR_7 | HOUR_8 | HOUR_9 | HOUR_10 | HOUR_11 | HOUR_12 | HOUR_13 | HOUR_14 |
                      HOUR_15 | HOUR_16 | HOUR_17 | HOUR_18 | HOUR_19 | HOUR_20 | HOUR_21 | HOUR_22 | HOUR_23,
};

struct ItemCastData
{
    uint32 uiSpellId;
    uint8 uiDays;
    uint32 uiHours;
    uint8 uiChance;
};

const static ItemCastData aItemCastData[MAX_SPELLS] =
{
    { SPELL_TABARD_OF_THE_LIGHTBRINGER, DAY_FRIDAY | DAY_SATURDAY | DAY_SUNDAY, HOUR_15 | HOUR_16 | HOUR_17 | HOUR_18 | HOUR_19 | HOUR_20 | HOUR_21, 50 },
    { SPELL_MEAT_SLAP, DAY_MONDAY | DAY_TUESDAY | DAY_WEDNESDAY | DAY_THURSDAY | DAY_FRIDAY, HOUR_7 | HOUR_8 | HOUR_9 | HOUR_10 | HOUR_11 | HOUR_12 | HOUR_13, 50 },
    { SPELL_I_CANT_DRIVE_55, DAY_TUESDAY | DAY_WEDNESDAY | DAY_THURSDAY, HOUR_15 | HOUR_16 | HOUR_17 | HOUR_18 | HOUR_19, 50 },
    { SPELL_BLOATED, DAY_SATURDAY | DAY_SUNDAY, HOUR_7 | HOUR_8 | HOUR_9 | HOUR_10 | HOUR_11 | HOUR_12 | HOUR_13, 25 },
    { SPELL_ZOMBIE_FORM, DAY_EVERY_DAY, HOUR_22 | HOUR_23 | HOUR_MIDNIGHT | HOUR_1, 50 },
    { SPELL_LICH_SLAP, DAY_MONDAY | DAY_TUESDAY | DAY_WEDNESDAY | DAY_THURSDAY | DAY_FRIDAY, HOUR_7 | HOUR_8 | HOUR_9 | HOUR_10 | HOUR_11 | HOUR_12 | HOUR_13, 50 },
    { SPELL_GOBLIN_BINARY, DAY_EVERY_DAY, HOUR_22, 25 },
    { SPELL_GNOMISH_BINARY, DAY_EVERY_DAY, HOUR_23, 25 },
    { SPELL_GROUND_SLAM, DAY_EVERY_DAY, HOUR_20 | HOUR_21, 25 },
    { SPELL_BLACKWHELP_TRANSFORMATION, DAY_FRIDAY | DAY_SATURDAY | DAY_SUNDAY, HOUR_15 | HOUR_16 | HOUR_17 | HOUR_18 | HOUR_19 | HOUR_20 | HOUR_21, 50 },
    { SPELL_SUICIDE, DAY_EVERY_DAY, HOUR_MIDNIGHT, 45 },
    { SPELL_FROG_FORM, DAY_EVERY_DAY, HOUR_2 | HOUR_3 | HOUR_4, 50 },
    { SPELL_RABBIT_FORM, DAY_EVERY_DAY, HOUR_5 | HOUR_6, 35 },
    { SPELL_TEST_TICKLE, DAY_EVERY_DAY, HOUR_14 | HOUR_15 | HOUR_16 | HOUR_17 | HOUR_18 | HOUR_19, 75 },
    { SPELL_DIRE_WOLF_FORM, DAY_EVERY_DAY, HOUR_EVERY_HOUR, 15 },
    { SPELL_PURPLE_FIREWORK, DAY_EVERY_DAY, HOUR_EVERY_HOUR, 100 } // Wird immer gecastet wenn nichts gefunden wurde
};

bool ItemUse_item_wow4you_tabard(Player* pPlayer, Item* pItem, const SpellCastTargets& pTargets)
{
    uint32 uiSpellId = 0;
    time_t tTime = time(NULL);
    tm* pLocalTime = localtime(&tTime);
    uint8 uiDay = pow((float)2, (float)pLocalTime->tm_wday);
    uint32 uiHour = pow((float)2, (float)pLocalTime->tm_hour);

    for (uint8 i = 0; i < MAX_SPELLS && !uiSpellId; ++i)
    {
        if ((aItemCastData[i].uiDays & uiDay) &&
                (aItemCastData[i].uiHours & uiHour))
        {
            if (roll_chance_i(aItemCastData[i].uiChance))
                uiSpellId = aItemCastData[i].uiSpellId;
        }
    }

    if (uiSpellId)
        pPlayer->CastSpell(pPlayer, uiSpellId, true);

    return false;
}

void AddSC_wow4you_tabard()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "item_wow4you_tabard";
    pNewScript->pItemUse = &ItemUse_item_wow4you_tabard;
    pNewScript->RegisterSelf();
}
