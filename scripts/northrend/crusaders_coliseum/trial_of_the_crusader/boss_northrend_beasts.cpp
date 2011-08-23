/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName:
SD%Complete: 0
SDComment:
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/*######
## npc_beast_combat_stalker
######*/

enum
{
    SAY_TIRION_BEAST_2                  = -1649005,
    SAY_TIRION_BEAST_3                  = -1649006,

    SPELL_BERSERK                       = 26662,

    PHASE_GORMOK                        = 0,
    PHASE_WORMS                         = 1,
    PHASE_ICEHOWL                       = 2,
};

struct MANGOS_DLL_DECL npc_beast_combat_stalkerAI : public Scripted_NoMovementAI
{
    npc_beast_combat_stalkerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
        m_creature->SetInCombatWithZone();
    }

    ScriptedInstance* m_pInstance;
    ObjectGuid m_aSummonedBossGuid[4];
    bool m_bFirstWormDied;
    uint32 m_uiBerserkTimer;
    uint32 m_uiAttackDelayTimer;
    uint32 m_uiNextBeastTimer;
    uint32 m_uiPhase;

    void Reset() override
    {
        m_uiAttackDelayTimer = 0;
        m_uiNextBeastTimer = 0;
        m_bFirstWormDied = false;
        m_uiPhase = PHASE_GORMOK;

        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiBerserkTimer    = 15 * MINUTE * IN_MILLISECONDS;
        else
            m_uiBerserkTimer    = 9 * MINUTE * IN_MILLISECONDS;
    }

    void MoveInLineOfSight(Unit* /*pWho*/) override {}

    void DamageTaken(Unit* /*pDealer*/, uint32& uiDamage) override
    {
        uiDamage = 0;
    }

    void EnterEvadeMode() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);

        for (uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pBoss = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[i]))
                pBoss->ForcedDespawn();
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_GORMOK:     m_uiPhase = PHASE_GORMOK;  break;
            case NPC_DREADSCALE: m_uiPhase = PHASE_WORMS;   break;
            case NPC_ICEHOWL:    m_uiPhase = PHASE_ICEHOWL; break;
            case NPC_ACIDMAW:
                // Cast emerge and delayed set in combat?
                pSummoned->SetInCombatWithZone();
                m_aSummonedBossGuid[3] = pSummoned->GetObjectGuid();
                return;
        }

        m_aSummonedBossGuid[m_uiPhase] = pSummoned->GetObjectGuid();

        pSummoned->GetMotionMaster()->MovePoint(m_uiPhase, aMovePositions[m_uiPhase][0], aMovePositions[m_uiPhase][1], aMovePositions[m_uiPhase][2]);

        // Next beasts are summoned only for heroic modes
        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
            m_uiNextBeastTimer = 150 * IN_MILLISECONDS;     // 2 min 30

        m_uiAttackDelayTimer = 15000;                       // TODO, must be checked.
    }

    // Only for Dreadscale and Icehowl
    void DoSummonNextBeast(uint32 uiBeastEntry)
    {
        if (uiBeastEntry == NPC_DREADSCALE)
        {
            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                DoScriptText(SAY_TIRION_BEAST_2, pTirion);

            m_creature->SummonCreature(NPC_DREADSCALE, aSpawnPositions[2][0], aSpawnPositions[2][1], aSpawnPositions[2][2], aSpawnPositions[2][3], TEMPSUMMON_DEAD_DESPAWN, 0);
        }
        else
        {
            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                DoScriptText(SAY_TIRION_BEAST_3, pTirion);

            m_creature->SummonCreature(NPC_ICEHOWL, aSpawnPositions[4][0], aSpawnPositions[4][1], aSpawnPositions[4][2], aSpawnPositions[4][3], TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        if (!m_pInstance)
            return;

        switch (pSummoned->GetEntry())
        {
            case NPC_GORMOK:
                if (m_uiPhase == PHASE_GORMOK)
                    DoSummonNextBeast(NPC_DREADSCALE);
                break;

            case NPC_DREADSCALE:
            case NPC_ACIDMAW:
                if (m_bFirstWormDied && m_uiPhase == PHASE_WORMS)
                    DoSummonNextBeast(NPC_ICEHOWL);
                else
                    m_bFirstWormDied = true;
                break;

            case NPC_ICEHOWL:
                m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DONE);
                m_creature->ForcedDespawn();
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiNextBeastTimer)
        {
            if (m_uiNextBeastTimer <= uiDiff)
            {
                if (m_uiPhase == PHASE_GORMOK)
                    DoSummonNextBeast(NPC_DREADSCALE);
                else if (m_uiPhase == PHASE_WORMS)
                    DoSummonNextBeast(NPC_ICEHOWL);

                m_uiNextBeastTimer = 0;
            }
            else
                m_uiNextBeastTimer -= uiDiff;
        }

        if (m_uiAttackDelayTimer)
        {
            if (m_uiAttackDelayTimer <= uiDiff)
            {
                if (m_uiPhase == PHASE_WORMS)
                    m_creature->SummonCreature(NPC_ACIDMAW, aSpawnPositions[3][0], aSpawnPositions[3][1], aSpawnPositions[3][2], aSpawnPositions[3][3], TEMPSUMMON_DEAD_DESPAWN, 0);

                if (Creature* pBeast = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[m_uiPhase]))
                    pBeast->SetInCombatWithZone();

                m_uiAttackDelayTimer = 0;
            }
            else
                m_uiAttackDelayTimer -= uiDiff;
        }

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                for (uint8 i = 0; i < 4; ++i)
                {
                    Creature* pBoss = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[i]);
                    if (pBoss && pBoss->isAlive())
                        pBoss->CastSpell(pBoss, SPELL_BERSERK, true);
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        m_creature->SelectHostileTarget();
    }
};

CreatureAI* GetAI_npc_beast_combat_stalker(Creature* pCreature)
{
    return new npc_beast_combat_stalkerAI(pCreature);
}

/*######
## boss_gormok, vehicle driven by 34800 (multiple times)
######*/

enum
{
    // gormok spells
    SPELL_IMPALE                = 66331,
    SPELL_STOMP                 = 66330,
    SPELL_RISING_ANGER          = 66636,
    SPELL_SNOBOLLED             = 66406,    // throw snobold on players head

    // these should be adde in vehicle seats on gormok's back
    NPC_SNOBOLD_VASSAL          = 34800,

    // snobold spells
    // ToDo: move in eventAI
    SPELL_BATTER                = 66408,
    SPELL_FIREBOMB              = 66313,
    SPELL_HEADCRACK             = 66407,
    NPC_FIREBOMB                = 34854,
    SPELL_FIREBOMB_DOT          = 66318,
};

struct MANGOS_DLL_DECL boss_gormokAI : public ScriptedAI
{
    boss_gormokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint8 m_uiMaxSnobold;
    uint8 m_uiSnoboldNo;
    uint8 m_uiHealthPoint;

    uint32 m_uiStompTimer;
    uint32 m_uiImpaleTimer;

    GuidList m_lSummonedSnowbolds;

    void Reset() override
    {
        m_uiStompTimer      = urand(20000, 25000);
        m_uiImpaleTimer     = 10000;
        m_uiSnoboldNo           = 1;

        // this is a workaround for handling snobolds
        // should be using vehicles here
        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL)
        {
            m_uiMaxSnobold  = 5;
            m_uiHealthPoint = 20;
        }
        else if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
        {
            m_uiMaxSnobold  = 6;
            m_uiHealthPoint = 17;
        }
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_SNOBOLD_VASSAL)
            m_lSummonedSnowbolds.push_back(pSummoned->GetObjectGuid());
    }

    void JustReachedHome() override
    {
        for (GuidList::const_iterator itr = m_lSummonedSnowbolds.begin(); itr != m_lSummonedSnowbolds.end(); ++itr)
            if (Creature* pSnowbold = m_creature->GetMap()->GetCreature(*itr))
                pSnowbold->ForcedDespawn();
        m_lSummonedSnowbolds.clear();
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        JustReachedHome();                                  // despawn snowbolds
    }

    void DamageTaken(Unit* /*pDealer*/, uint32& /*uiDamage*/) override
    {
        if (m_uiSnoboldNo > m_uiMaxSnobold)
            return;

        if (m_creature->GetHealthPercent() <= (100 - m_uiHealthPoint * m_uiSnoboldNo))
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                // throw snobold
                if (DoCastSpellIfCan(pTarget, SPELL_SNOBOLLED, true) == CAST_OK)
                {
                    // buff boss
                    DoCastSpellIfCan(m_creature, SPELL_RISING_ANGER);
                    // workaround for summoning snobolds
                    if (Creature* pSnobold = m_creature->SummonCreature(NPC_SNOBOLD_VASSAL, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_DEAD_DESPAWN, 0))
                    {
                        pSnobold->AddThreat(pTarget, 100.0f);
                        m_uiSnoboldNo += 1;
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_STOMP) == CAST_OK)
                m_uiStompTimer = 20000;
        }
        else
            m_uiStompTimer -= uiDiff;

        if (m_uiImpaleTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_IMPALE) == CAST_OK)
                    m_uiImpaleTimer = 10000;
            }
        }
        else
            m_uiImpaleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

/*######
## boss_acidmaw
######*/

enum
{
    // mobile
    SPELL_ACID_SPEW             = 66818,
    SPELL_PARALYTIC_BITE        = 66824,
    // stationary
    SPELL_ACID_SPIT             = 66880,
    SPELL_PARALYTIC_SPRAY       = 66901,
    SPELL_PARALYTIC_TOXIN       = 66823,

    // common spells
    SPELL_SLIME_POOL            = 66883,
    SPELL_SWEEP                 = 66794,
    SPELL_ENRAGE                = 68335,

    // slime pool
    SPELL_SLIME_POOL_AURA       = 66882,    // casted by the slime pool; need to add visual as dummy
    NPC_SLIME_POOL              = 35176,

    // display ids
    DISPLAY_ID_ACID_FIXED       = 29815,
    DISPLAY_ID_ACID_MOBILE      = 29816,

    // phases
    PHASE_NON_ATTACK            = 3,
    PHASE_STATIONARY            = 0,
    PHASE_SUBMERGED             = 1,
    PHASE_MOBILE                = 2
};

struct MANGOS_DLL_DECL boss_acidmawAI : public ScriptedAI
{
    boss_acidmawAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    // mobile
    uint32 m_uiAcidSpewTimer;
    uint32 m_uiParaliticBiteTimer;
    uint32 m_uiSlimePoolTimer;

    // stationary
    uint32 m_uiAcidSpitTimer;
    uint32 m_uiParaliticSprayTimer;
    uint32 m_uiSweepTimer;

    // phase change
    uint32 m_uiPhaseChangeTimer;
    uint32 m_uiSubmergeTimer;
    uint32 m_uiMoveTimer;
    uint8 m_uiPhase;

    bool hasEnraged;

    void Reset() override
    {
        // mobile
        m_uiAcidSpewTimer       = 5000;
        m_uiParaliticBiteTimer  = urand(5000, 10000);
        m_uiSlimePoolTimer      = urand(12000, 15000);

        // stationary
        m_uiAcidSpitTimer       = 3000;
        m_uiParaliticSprayTimer = urand(7000, 13000);
        m_uiSweepTimer          = urand(12000, 15000);

        m_uiPhase               = PHASE_MOBILE;
        m_uiPhaseChangeTimer    = 45000;
        m_uiSubmergeTimer   = 60000;
        m_uiMoveTimer       = 60000;

        hasEnraged          = false;
    }

    void JustReachedHome() override
    {
    }

    void Aggro(Unit* /*pWho*/) override
    {
    }

    void JustSummoned(Creature* pSummned) override
    {
        if (pSummned->GetEntry() == NPC_SLIME_POOL)
            pSummned->CastSpell(pSummned, SPELL_SLIME_POOL_AURA, false);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // stationary
        if (m_uiPhase == PHASE_STATIONARY)
        {
            /*if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiMoveTimer       = 2000;
                m_uiSubmergeTimer   = 4000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_ACID_MOBILE);
                phase = 2;
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                SetCombatMovement(true);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;*/

            if (m_uiAcidSpitTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_ACID_SPIT) == CAST_OK)
                        m_uiAcidSpitTimer = 3000;
                }
            }
            else
                m_uiAcidSpitTimer -= uiDiff;

            if (m_uiParaliticSprayTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_PARALYTIC_SPRAY) == CAST_OK)
                        m_uiParaliticSprayTimer = 21000;
                }
            }
            else
                m_uiParaliticSprayTimer -= uiDiff;

            if (m_uiSweepTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SWEEP) == CAST_OK)
                    m_uiSweepTimer = urand(10000, 15000);
            }
            else
                m_uiSweepTimer -= uiDiff;
        }
        else if (m_uiPhase == PHASE_SUBMERGED)
        {
        }
        else if (m_uiPhase == PHASE_MOBILE)
        {
            /*if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                SetCombatMovement(false);
                m_uiMoveTimer       = 2000;
                m_uiSubmergeTimer   = 4000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_ACID_FIXED);
                phase = 1;
                m_creature->StopMoving();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                SetCombatMovement(false);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;*/

            if (m_uiParaliticBiteTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_PARALYTIC_BITE) == CAST_OK)
                        m_uiParaliticBiteTimer = urand(5000, 7000);
                }
            }
            else
                m_uiParaliticBiteTimer -= uiDiff;

            if (m_uiSlimePoolTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SLIME_POOL) == CAST_OK)
                    m_uiSlimePoolTimer = urand(17000, 23000);
            }
            else
                m_uiSlimePoolTimer -= uiDiff;

            if (m_uiAcidSpewTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ACID_SPEW) == CAST_OK)
                    m_uiAcidSpewTimer = 21000;
            }
            else
                m_uiAcidSpewTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_acidmaw(Creature* pCreature)
{
    return new boss_acidmawAI(pCreature);
}

/*######
## boss_dreadscale
######*/

enum
{
    // mobile
    SPELL_BURNING_BITE          = 66879,
    SPELL_MOLTEN_SPEW           = 66821,
    // stationary
    SPELL_FIRE_SPIT             = 66796,
    SPELL_BURNING_SPRAY         = 66902,
    SPELL_BURNING_BILE          = 66869,

    // display ids
    DISPLAY_ID_DREAD_FIXED      = 26935,
    DISPLAY_ID_DREAD_MOBILE     = 24564,
};

struct MANGOS_DLL_DECL boss_dreadscaleAI : public ScriptedAI
{
    boss_dreadscaleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    // mobile
    uint32 m_uiBurningBiteTimer;
    uint32 m_uiMoltenSpewTimer;
    uint32 m_uiSlimePoolTimer;

    // stationary
    uint32 m_uiFireSpitTimer;
    uint32 m_uiBurningSprayTimer;
    uint32 m_uiSweepTimer;

    uint32 m_uiPhaseChangeTimer;
    uint32 m_uiSubmergeTimer;
    uint32 m_uiMoveTimer;
    uint8 m_uiPhase;

    bool hasEnraged;

    void Reset() override
    {
        // mobile
        m_uiBurningBiteTimer    = urand(3000, 5000);
        m_uiMoltenSpewTimer     = urand(7000, 13000);
        m_uiSlimePoolTimer      = urand(13000, 15000);

        // stationary
        m_uiFireSpitTimer       = 3000;
        m_uiBurningSprayTimer   = urand(5000, 7000);
        m_uiSweepTimer          = urand(13000, 15000);

        m_uiPhase               = PHASE_NON_ATTACK;
        m_uiPhaseChangeTimer    = 45000;
        m_uiSubmergeTimer   = 60000;
        m_uiMoveTimer       = 60000;
    }

    void JustReachedHome() override
    {
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (m_uiPhase == PHASE_NON_ATTACK)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData) override
    {
        if (uiMovementType == POINT_MOTION_TYPE && uiData == PHASE_WORMS)
            m_uiPhase = PHASE_STATIONARY;
    }

    void JustSummoned(Creature* pSummned) override
    {
        if (pSummned->GetEntry() == NPC_SLIME_POOL)
            pSummned->CastSpell(pSummned, SPELL_SLIME_POOL_AURA, false);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // stationary
        if (m_uiPhase == PHASE_STATIONARY)
        {
            /*if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiSubmergeTimer   = 4000;
                m_uiMoveTimer       = 2000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_DREAD_MOBILE);
                phase = 2;
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                SetCombatMovement(true);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;*/

            if (m_uiFireSpitTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_FIRE_SPIT) == CAST_OK)
                        m_uiFireSpitTimer = urand(3000, 5000);
                }
            }
            else
                m_uiFireSpitTimer -= uiDiff;

            if (m_uiBurningSprayTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_BURNING_SPRAY) == CAST_OK)
                        m_uiBurningSprayTimer = 21000;
                }
            }
            else
                m_uiBurningSprayTimer -= uiDiff;

            if (m_uiSweepTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SWEEP) == CAST_OK)
                    m_uiSweepTimer = urand(10000, 15000);
            }
            else
                m_uiSweepTimer -= uiDiff;
        }
        else if (m_uiPhase == PHASE_SUBMERGED)
        {
        }
        else if (m_uiPhase == PHASE_MOBILE)
        {
            /*if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                SetCombatMovement(false);
                m_uiSubmergeTimer   = 4000;
                m_uiMoveTimer       = 2000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_DREAD_FIXED);
                phase = 1;
                m_creature->StopMoving();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                SetCombatMovement(false);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;*/

            if (m_uiBurningBiteTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_BURNING_BITE) == CAST_OK)
                        m_uiBurningBiteTimer = urand(5000, 7000);
                }
            }
            else
                m_uiBurningBiteTimer -= uiDiff;

            if (m_uiSlimePoolTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SLIME_POOL) == CAST_OK)
                    m_uiSlimePoolTimer = urand(17000, 23000);
            }
            else
                m_uiSlimePoolTimer -= uiDiff;

            if (m_uiMoltenSpewTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_MOLTEN_SPEW) == CAST_OK)
                    m_uiMoltenSpewTimer = 21000;
            }
            else
                m_uiMoltenSpewTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dreadscale(Creature* pCreature)
{
    return new boss_dreadscaleAI(pCreature);
}

/*######
## boss_icehowl
######*/

enum
{
    EMOTE_MASSIVE_CRASH          = -1649039,

    // icehowl spells
    SPELL_FEROCIOUS_BUTT        = 66770,
    SPELL_ARCTIC_BREATH         = 66689,
    SPELL_WHIRL                 = 67345,
    SPELL_MASSIVE_CRASH         = 66683,
    SPELL_SURGE_OF_ADRENALINE   = 68667,    // used only in non heroic
    SPELL_TRAMPLE               = 66734,
    SPELL_STAGGERED_DAZE        = 66758,
    SPELL_FROTHING_RAGE         = 66759,

    POINT_ID_TRAMPLE            = 10,
};

struct MANGOS_DLL_DECL boss_icehowlAI : public ScriptedAI
{
    boss_icehowlAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;

    uint32 m_uiFerociousButtTimer;
    uint32 m_uiArticBreathTimer;
    uint32 m_uiWhirlTimer;
    uint32 m_uiMassiveCrashTimer;
    uint32 m_uiTrampleTimer;
    uint32 m_uiFrothingRageTimer;

    uint8 m_uiTrampleStage;

    bool m_bIsTrample;
    bool m_bTrampleCasted;
    bool m_bMovementStarted;
    bool m_bAdrenalineCasted;

    float fPosX, fPosY, fPosZ;

    void Reset() override
    {
        m_uiFerociousButtTimer  = urand(20000, 30000);
        m_uiArticBreathTimer    = urand(25000, 30000);
        m_uiWhirlTimer          = urand(20000, 25000);
        m_uiMassiveCrashTimer   = 45000;
        m_uiTrampleTimer        = 50000;
        m_uiFrothingRageTimer   = 30000;

        m_uiTrampleStage    = 0;

        m_bMovementStarted  = false;
        m_bTrampleCasted    = false;
        m_bIsTrample        = false;
        m_bAdrenalineCasted = false;
        fPosX = 0;
        fPosY = 0;
        fPosZ = 0;
    }

    void JustReachedHome() override
    {
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DONE);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE || id != POINT_ID_TRAMPLE)
            return;

        m_bMovementStarted = false;
        SetCombatMovement(true);
        if (m_creature->getVictim())
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
    }

    // every player casts surge of addrenaline on normal difficulty
    void DoCastSurgeOfAdrenaline()
    {
        Map::PlayerList const& PlayerList = m_creature->GetMap()->GetPlayers();

        if (PlayerList.isEmpty())
            return;

        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
        {
            if (i->getSource()->isAlive())
                i->getSource()->CastSpell(i->getSource(), SPELL_SURGE_OF_ADRENALINE, false);
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsTrample)
        {
            if (m_uiTrampleTimer < uiDiff)
            {
                switch (m_uiTrampleStage)
                {
                        // go to center
                    case 0:
                        SetCombatMovement(false);
                        m_creature->GetMotionMaster()->MoveIdle();
                        m_creature->GetMotionMaster()->MoveJump(aSpawnPositions[11][0], aSpawnPositions[11][1], aSpawnPositions[11][2], 10.0f, 10.0f);

                        ++m_uiTrampleStage;
                        m_uiTrampleTimer = 3000;
                        break;
                        // cast massive crash & stop
                    case 1:
                        if (DoCastSpellIfCan(m_creature, SPELL_MASSIVE_CRASH) == CAST_OK)
                        {
                            ++m_uiTrampleStage;
                            m_uiTrampleTimer = 8000;
                        }
                        break;
                        // wait 5 secs -> cast surge of addrenaline on players
                    case 2:
                        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
                        {
                            if (!m_bAdrenalineCasted)
                            {
                                m_bAdrenalineCasted = true;
                                DoCastSurgeOfAdrenaline();
                            }
                        }
                        // pick a target and run for it
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTarget->GetPosition(fPosX, fPosY, fPosZ);
                            DoScriptText(EMOTE_MASSIVE_CRASH, m_creature, pTarget);
                            m_bMovementStarted = true;
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
                            m_creature->SetWalk(false);
                            m_creature->GetMotionMaster()->MovePoint(POINT_ID_TRAMPLE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());
                            ++m_uiTrampleStage;
                            m_uiTrampleTimer = 500;
                        }
                        break;
                        // run to the target; if target hit cast Trample, else go to next phase
                    case 3:
                        if (m_bMovementStarted)
                        {
                            Map::PlayerList const& lPlayers = m_creature->GetMap()->GetPlayers();
                            if (lPlayers.isEmpty())
                                return;

                            for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                            {
                                Player* pPlayer = itr->getSource();
                                if (!pPlayer)
                                    continue;

                                if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 5.0f))
                                {
                                    DoCast(pPlayer, SPELL_TRAMPLE);
                                    m_bTrampleCasted = true;
                                    m_bMovementStarted = false;
                                    m_creature->GetMotionMaster()->MovementExpired();
                                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                                }
                            }
                        }
                        else
                            ++m_uiTrampleStage;

                        if (m_bTrampleCasted)
                            ++m_uiTrampleStage;
                        break;
                        // if trample not casted, cast stun, else continue
                    case 4:
                        if (!m_bTrampleCasted)
                        {
                            // Missing emote!
                            //DoScriptText(EMOTE_STUN, m_creature);
                            DoCast(m_creature, SPELL_STAGGERED_DAZE);
                        }
                        m_bMovementStarted = false;
                        m_bAdrenalineCasted = false;
                        m_bTrampleCasted = false;
                        SetCombatMovement(true);
                        m_creature->GetMotionMaster()->MovementExpired();
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        m_uiMassiveCrashTimer = urand(45000, 50000);
                        m_bIsTrample = false;
                        break;
                    default:
                        m_uiTrampleTimer = 100000;
                }
            }
            else
                m_uiTrampleTimer -= uiDiff;
        }

        // return if doing trample
        if (m_bIsTrample)
            return;

        if (m_uiFrothingRageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FROTHING_RAGE) == CAST_OK)
                m_uiFrothingRageTimer = 40000;
        }
        else
            m_uiFrothingRageTimer -= uiDiff;

        if (m_uiMassiveCrashTimer < uiDiff)
        {
            m_bIsTrample = true;
            m_uiTrampleTimer = 1000;
            m_uiTrampleStage = 0;
            m_uiMassiveCrashTimer = urand(45000, 50000);
        }
        else
            m_uiMassiveCrashTimer -= uiDiff;

        if (m_uiWhirlTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WHIRL) == CAST_OK)
                m_uiWhirlTimer = urand(20000, 25000);
        }
        else
            m_uiWhirlTimer -= uiDiff;

        if (m_uiArticBreathTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ARCTIC_BREATH) == CAST_OK)
                    m_uiArticBreathTimer = urand(25000, 30000);
            }
        }
        else
            m_uiArticBreathTimer -= uiDiff;

        if (m_uiFerociousButtTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_FEROCIOUS_BUTT) == CAST_OK)
                    m_uiFerociousButtTimer = urand(20000, 30000);
            }
        }
        else
            m_uiFerociousButtTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}

void AddSC_northrend_beasts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_beast_combat_stalker";
    pNewScript->GetAI = &GetAI_npc_beast_combat_stalker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_gormok";
    pNewScript->GetAI = &GetAI_boss_gormok;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_acidmaw";
    pNewScript->GetAI = &GetAI_boss_acidmaw;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_dreadscale";
    pNewScript->GetAI = &GetAI_boss_dreadscale;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_icehowl";
    pNewScript->GetAI = &GetAI_boss_icehowl;
    pNewScript->RegisterSelf();
}
