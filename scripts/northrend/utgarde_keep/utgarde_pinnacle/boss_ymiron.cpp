/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Ymiron
SD%Complete: 80%
SDComment: Randomize the boats; Spell 48316 can't be interrupted
SDCategory: Utgarde Pinnacle
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    SAY_AGGRO                   = -1575031,
    SAY_SUMMON_BJORN            = -1575032,
    SAY_SUMMON_HALDOR           = -1575033,
    SAY_SUMMON_RANULF           = -1575034,
    SAY_SUMMON_TORGYN           = -1575035,
    SAY_SLAY_1                  = -1575036,
    SAY_SLAY_2                  = -1575037,
    SAY_SLAY_3                  = -1575038,
    SAY_SLAY_4                  = -1575039,
    SAY_DEATH                   = -1575040,

    MAX_BOATS                   = 4,

    SPELL_BANE                  = 48294,    // sends script event 20651 when target is hit - set achiev to false
    SPELL_BANE_H                = 59301,
    SPELL_DARK_SLASH            = 48292,
    SPELL_FETID_ROT             = 48291,
    SPELL_FETID_ROT_H           = 59300,
    SPELL_SCREAMS_OF_THE_DEAD   = 51750,    // knockback players to summon boat
    //SPELL_CHOOSE_SPIRIT         = 48306,    // boss chooses spirit - has script effect

    // blessings
    SPELL_SPIRIT_BURST          = 48529,    // by Ranulf
    SPELL_SPIRIT_BURST_H        = 59305,
    SPELL_SPIRIT_STRIKE         = 48423,    // by Haldor
    SPELL_SPIRIT_STRIKE_H       = 59304,
    SPELL_SUMMON_SPIRIT_FOUNT   = 48386,    // by Bjorn
    SPELL_AVENGING_SPIRITS      = 48590,    // by Torgyn - has script effect - 48592

    // visuals
    SPELL_CHANNEL_SPIRIT_YMIRON = 48316,    // spell need to be casted as triggered, but then it can't be interrupted
    SPELL_CHANNEL_YMIRON_SPIRIT = 48307,

    // by summoned creatures
    SPELL_SPIRIT_FOUNT          = 48380,
    SPELL_SPIRIT_FOUNT_H        = 59320,
    SPELL_SPIRIT_FOUNT_BEAM     = 48385,    // channeled beam on the spirit fount
    SPELL_SPIRIT_VISUAL         = 48593,    // avenging spirit summon visual
    SPELL_WITHER_TRIGG          = 48584,    // aura for avenging spirits - triggers 48584 on melee
    SPELL_SPIRIT_DIES           = 48596,    // cast by a boat spirit

    // spirit transforms
    SPELL_BJORN_TRANSFORM       = 48308,
    SPELL_HALDOR_TRANSFORM      = 48311,
    SPELL_RANULF_TRANSFORM      = 48312,
    SPELL_TORGYN_TRANSFORM      = 48313,

    NPC_SPIRIT_FOUNT            = 27339,
    NPC_AVENGING_SPIRIT         = 27386,

    POINT_ID_BOAT               = 0,

    PHASE_NO_BOAT               = 0,
    PHASE_BJORN                 = 1,
    PHASE_HALDOR                = 2,
    PHASE_RANULF                = 3,
    PHASE_TORGYN                = 4
};

struct BoatSpirits
{
    uint32 uiSpiritSpell, uiSpiritTarget, uiYellId;
    uint8 uiBoatPhase;
    float fMovePosX, fMovePosY, fMovePosZ;
};

static const BoatSpirits aYmironBoatsSpirits[MAX_BOATS] =
{
    {SPELL_BJORN_TRANSFORM,  NPC_BJORN,  SAY_SUMMON_BJORN,  PHASE_BJORN,  381.546f, -314.362f, 104.756f},
    {SPELL_HALDOR_TRANSFORM, NPC_HALDOR, SAY_SUMMON_HALDOR, PHASE_HALDOR, 404.310f, -314.761f, 104.756f},
    {SPELL_RANULF_TRANSFORM, NPC_RANULF, SAY_SUMMON_RANULF, PHASE_RANULF, 404.379f, -335.335f, 104.756f},
    {SPELL_TORGYN_TRANSFORM, NPC_TORGYN, SAY_SUMMON_TORGYN, PHASE_TORGYN, 380.813f, -335.069f, 104.756f}
};

/*######
## boss_ymiron
######*/

struct MANGOS_DLL_DECL boss_ymironAI : public ScriptedAI
{
    boss_ymironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();

        // randomize boats
        for (uint8 i = 0; i < MAX_BOATS; ++i)
            m_vuiBoatPhases.push_back(aYmironBoatsSpirits[i].uiBoatPhase);

        std::random_shuffle(m_vuiBoatPhases.begin(), m_vuiBoatPhases.end());
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFetidRotTimer;
    uint32 m_uiBaneTimer;
    uint32 m_uiDarkSlashTimer;
    uint8 m_uiBoatPhase;
    uint8 m_uiPhase;
    uint8 m_uiHealthCheck;

    uint32 m_uiSpiritBurstTimer;
    uint32 m_uiSpiritStrikeTimer;
    uint32 m_uiSpiritFountTimer;
    uint32 m_uiAvengingSpiritsTimer;

    bool m_bIsChannelingSpirit;

    ObjectGuid m_uiCurrentSpiritGuid;

    GUIDList m_lSummonedSpiritsList;
    std::vector<uint8> m_vuiBoatPhases;

    void Reset()
    {
        m_uiFetidRotTimer   = urand(8000, 13000);
        m_uiBaneTimer       = urand(18000, 23000);
        m_uiDarkSlashTimer  = urand(28000, 33000);
        m_uiPhase           = PHASE_NO_BOAT;
        m_uiHealthCheck     = 20;
        m_uiBoatPhase       = 0;

        m_uiSpiritBurstTimer        = 10000;
        m_uiSpiritStrikeTimer       = 10000;
        m_uiSpiritFountTimer        = 10000;
        m_uiAvengingSpiritsTimer    = 10000;

        m_bIsChannelingSpirit       = false;

        m_uiCurrentSpiritGuid.Clear();

        // door exploit check
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_SKADI) != DONE)
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
            case 3: DoScriptText(SAY_SLAY_4, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, DONE);

        DoDespawnBoats();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, FAIL);

        DoDespawnBoats();
    }

    void DoDespawnBoats()
    {
        if (m_pInstance)
        {
            // remove all auras from the spirits
            for (uint8 i = 0; i < MAX_BOATS; ++i)
            {
                if (Creature* pSpirit = m_pInstance->GetSingleCreatureFromStorage(aYmironBoatsSpirits[i].uiSpiritTarget))
                {
                    pSpirit->RemoveAllAuras();
                    pSpirit->InterruptNonMeleeSpells(false);
                }
            }
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_SPIRIT_FOUNT:
                DoCastSpellIfCan(pSummoned, SPELL_SPIRIT_FOUNT_BEAM, CAST_TRIGGERED);
                pSummoned->CastSpell(pSummoned, m_bIsRegularMode ? SPELL_SPIRIT_FOUNT : SPELL_SPIRIT_FOUNT_H, true);
                break;
            case NPC_AVENGING_SPIRIT:
                pSummoned->CastSpell(pSummoned, SPELL_SPIRIT_VISUAL, true);
                pSummoned->CastSpell(pSummoned, SPELL_WITHER_TRIGG, true);
                break;
        }
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE || uiPointId != POINT_ID_BOAT)
            return;

        DoCastSpellIfCan(m_creature, SPELL_CHANNEL_YMIRON_SPIRIT);
        DoScriptText(aYmironBoatsSpirits[m_uiPhase - 1].uiYellId, m_creature);
        SetCombatMovement(true);
        m_bIsChannelingSpirit = false;

        if (m_creature->getVictim())
        {
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        }

        if (m_pInstance)
        {
            if(Creature* pTrigger = m_pInstance->GetSingleCreatureFromStorage(aYmironBoatsSpirits[m_uiPhase - 1].uiSpiritTarget))
            {
                m_uiCurrentSpiritGuid = pTrigger->GetObjectGuid();
                pTrigger->CastSpell(m_creature, SPELL_CHANNEL_SPIRIT_YMIRON, true);
                pTrigger->CastSpell(pTrigger, aYmironBoatsSpirits[m_uiPhase - 1].uiSpiritSpell, true);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_bIsChannelingSpirit)
            return;

        if (m_uiBaneTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_BANE : SPELL_BANE_H) == CAST_OK)
                m_uiBaneTimer = urand(20000, 25000);
        }
        else
            m_uiBaneTimer -= uiDiff;

        if (m_uiFetidRotTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FETID_ROT : SPELL_FETID_ROT_H) == CAST_OK)
                m_uiFetidRotTimer = urand(10000, 15000);
        }
        else
            m_uiFetidRotTimer -= uiDiff;

        if (m_uiDarkSlashTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DARK_SLASH) == CAST_OK)
                m_uiDarkSlashTimer = urand(30000, 35000);
        }
        else
            m_uiDarkSlashTimer -= uiDiff;

        if (m_creature->GetHealthPercent() < 100 - m_uiHealthCheck)
        {
            // change phase
            DoCastSpellIfCan(m_creature, SPELL_SCREAMS_OF_THE_DEAD, CAST_TRIGGERED);

            // make the current spirit die
            if (Creature* pSpirit = m_creature->GetMap()->GetCreature(m_uiCurrentSpiritGuid))
            {
                pSpirit->InterruptNonMeleeSpells(false);
                pSpirit->CastSpell(pSpirit, SPELL_SPIRIT_DIES, false);
            }

            m_uiPhase = m_vuiBoatPhases[m_uiBoatPhase];

            m_creature->GetMotionMaster()->MovePoint(POINT_ID_BOAT, aYmironBoatsSpirits[m_uiPhase - 1].fMovePosX, aYmironBoatsSpirits[m_uiPhase - 1].fMovePosY, aYmironBoatsSpirits[m_uiPhase - 1].fMovePosZ);
            SetCombatMovement(false);
            m_bIsChannelingSpirit = true;
            ++m_uiBoatPhase;
            m_uiHealthCheck += 20;
        }

        switch(m_uiPhase)
        {
            case PHASE_BJORN:

                if (m_uiSpiritFountTimer)
                {
                    if (m_uiSpiritFountTimer <= uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_SPIRIT_FOUNT) == CAST_OK)
                            m_uiSpiritFountTimer = 0;
                    }
                    else
                        m_uiSpiritFountTimer -= uiDiff;
                }

                break;
            case PHASE_HALDOR:

                if (m_uiSpiritStrikeTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SPIRIT_STRIKE : SPELL_SPIRIT_STRIKE_H) == CAST_OK)
                        m_uiSpiritStrikeTimer = 5000;
                }
                else
                    m_uiSpiritStrikeTimer -= uiDiff;

                break;
            case PHASE_RANULF:

                if (m_uiSpiritBurstTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SPIRIT_BURST : SPELL_SPIRIT_BURST_H) == CAST_OK)
                        m_uiSpiritBurstTimer = 10000;
                }
                else
                    m_uiSpiritBurstTimer -= uiDiff;

                break;
            case PHASE_TORGYN:

                if (m_uiAvengingSpiritsTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_AVENGING_SPIRITS) == CAST_OK)
                        m_uiAvengingSpiritsTimer = 15000;
                }
                else
                    m_uiAvengingSpiritsTimer -= uiDiff;

                break;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ymiron(Creature* pCreature)
{
    return new boss_ymironAI(pCreature);
}

bool ProcessEventId_event_king_bane(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (instance_pinnacle* pInstance = (instance_pinnacle*)((Creature*)pSource)->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_YMIRON) != IN_PROGRESS)
            return false;

        pInstance->DoProcessKingBaneEvent();
        return true;
    }
    return false;
}

void AddSC_boss_ymiron()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ymiron";
    pNewScript->GetAI = &GetAI_boss_ymiron;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_king_bane";
    pNewScript->pProcessEventId = &ProcessEventId_event_king_bane;
    pNewScript->RegisterSelf();
}
