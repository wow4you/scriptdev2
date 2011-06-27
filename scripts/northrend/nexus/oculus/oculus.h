/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_OCULUS_H
#define DEF_OCULUS_H

/* Encounters
 * Drakos           = 0
 * Varos            = 1
 * Urom             = 2
 * Eregos           = 3
 */

enum
{
    MAX_ENCOUNTER               = 5,

    TYPE_DRAKOS                 = 0,
    TYPE_VAROS                  = 1,
    TYPE_UROM                   = 2,
    TYPE_EREGOS                 = 3,
    TYPE_CONSTRUCTS             = 4,

    NPC_DRAKOS                  = 27654,
    NPC_VAROS                   = 27447,
    NPC_UROM                    = 27655,
    NPC_EREGOS                  = 27656,
    NPC_CONSTRUCT               = 27641,
    NPC_AZURE_CAPTAIN           = 28236,
    NPC_AZURE_GUARDIAN          = 27638,    // temp replacement of the azure captain because of the bad DB content

    NPC_ETERNOS                 = 27659,    // bronze
    NPC_VERDISA                 = 27657,    // emerald
    NPC_BELGARISTRASZ           = 27658,    // ruby

    GO_DRAGON_CAGE_DOOR         = 193995,

    // Loot
    GO_CACHE_EREGOS             = 191349,
    GO_CACHE_EREGOS_H           = 193603,

    // intro yells ToDo: fix ids
    SAY_VAROS_INTRO             = -1578000,
    SAY_EREGOS_SPAWN            = -1578011,

    // varos' shield
    SPELL_CENTRIFUGE_SHIELD     = 50053,

    // world states to show how many constructs are still alive
    WORLD_STATE_CON             = 3524,
    WORLD_STATE_CON_COUNT       = 3486,

    ACHIEV_START_EREGOS_ID      = 18153,            // eregos timed kill achiev
    // for Experienced drake rider achiev 1871 - we have 3 criterias 7177, 7178, 7179

    //ACHIEV_CRIT_RUBY_VOID     = 7323,                 // Ruby void achiev 2044
    //ACHIEV_CRIT_EMERALD_VOID  = 7324,                 // Emerald void achiev 2045
    //ACHIEV_CRIT_AMBER_VOID    = 7525,                 // Amber void achiev 2046
};

struct sOculusLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static const sOculusLocations m_sPrisonersMoveLocs[] =
{
    {944.384f, 1058.418f, 359.967f},        // Eternos
    {949.928f, 1034.753f, 359.967f},        // Verdista
    {944.868f, 1044.982f, 359.967f}         // Belgaristrasz
};

// Urom and Eregos are summoned bosses
// This is because it's blizzlike to perserver boss order
// Drakos and Varos are spawned by default
static const sOculusLocations m_sOculusBossSpawnLocs[] =
{
    {1177.47f, 937.722f, 527.405f, 2.21657f},    // Urom
    {1077.04f, 1086.21f, 655.497f, 4.18879f}     // Eregos
};

class MANGOS_DLL_DECL instance_oculus : public ScriptedInstance
{
    public:
        instance_oculus(Map* pMap);
        ~instance_oculus() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);
        void OnPlayerEnter(Player* pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        void DoCallAzureRingCaptain();

    protected:
        void DoSummonNextBossIfCan(bool bByPlayerEnter = false);
        void DoOpenAllCages();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        GUIDList m_lCageDoorGUIDs;
        GUIDSet m_sConstructsGUIDs;
        GUIDSet m_sCaptainsGUIDs;
};

#endif
