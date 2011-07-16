/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_UTG_PINNACLE_H
#define DEF_UTG_PINNACLE_H

enum
{
    MAX_ENCOUNTER                   = 4,
    MAX_SPECIAL_ACHIEV_CRITS        = 2,

    TYPE_SVALA                      = 0,
    TYPE_GORTOK                     = 1,
    TYPE_SKADI                      = 2,
    TYPE_YMIRON                     = 3,

    TYPE_ACHIEV_HULK                = 0,
    TYPE_ACHIEV_LOVE_SKADI          = 1,

    GO_STASIS_GENERATOR             = 188593,
    GO_DOOR_SKADI                   = 192173,
    GO_DOOR_YMIRON                  = 192174,

    NPC_FLAME_BRAZIER               = 27273,            // throw flames at players on Svalna event
    NPC_STATIS_ORB                  = 26086,            // not sure if this is the right one - should it be 26688 ?
    NPC_FURBOLG                     = 26684,
    NPC_WORGEN                      = 26683,
    NPC_JORMUNGAR                   = 26685,
    NPC_RHINO                       = 26686,
    NPC_GORTOK                      = 26687,
    NPC_YMIRON                      = 26861,
    NPC_BJORN                       = 27303,            // front right
    NPC_HALDOR                      = 27307,            // front left
    NPC_RANULF                      = 27308,            // back left
    NPC_TORGYN                      = 27309,            // back right

    ACHIEV_CRIT_INCREDIBLE_HULK     = 7322,             // Svala achiev - 2043
    ACHIEV_CRIT_KINGS_BANE          = 7598,             // Ymiron achiev - 2157
    ACHIEV_CRIT_GIRL_LOVES_SKADI    = 7595,             // Skadi achiev - 2156

    ACHIEV_START_SKADI_ID           = 17726,            // Starts Skadi timed achiev - 1873

    SPELL_BALL_OF_FLAME             = 48246,            // spell used by the flame braziers
    SPELL_AWAKEN_SUBBOSS            = 47669,            // spells for gortok event
    SPELL_AWAKEN_GORTOK             = 47670,
};

class MANGOS_DLL_DECL instance_pinnacle : public ScriptedInstance
{
    public:
        instance_pinnacle(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet);
        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

        void DoProcessCallFlamesEvent();
        void DoProcessKingBaneEvent() { m_bIsKingBane = false; }

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        bool m_abAchievCriteria[MAX_SPECIAL_ACHIEV_CRITS];
        std::string m_strInstData;

        uint8 m_uiGortokBeastsDead;

        bool m_bIsKingBane;

        GUIDList m_lFlameBraziersList;
        GUIDList m_lGortokBeastsList;
};

#endif
