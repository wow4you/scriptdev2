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
SDName: Boss_Ormorok
SD%Complete: 50%
SDComment: TODO: Correct timers. Research how spikes work, and attempt code it properly from mangos side.
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO                   = -1576011,
    SAY_KILL                    = -1576012,
    SAY_REFLECT                 = -1576013,
    SAY_ICESPIKE                = -1576014,
    SAY_DEATH                   = -1576015,
    EMOTE_BOSS_GENERIC_FRENZY   = -1000005,

    MAX_SPIKES                  = 8,            // the max number of spikes summoned in a row
    MAX_SPIKES_DISTANCE         = 5,            // distance between the spikes

    SPELL_REFLECTION            = 47981,

    SPELL_CRYSTAL_SPIKES        = 47958,        // starts the spikes event
    SPELL_CRYSTAL_SPIKES_H1     = 57082,
    SPELL_CRYSTAL_SPIKES_H2     = 57083,

    SPELL_SPIKE_SUMMON          = 47947,        // used by the crystal spike trap go
    //SPELL_SPIKE_VISUAL          = 50442,      // used by the crystal spike
    //SPELL_CRYSTAL_SPIKE_PER     = 47941,      // aura which summons all the other spikes

    //SPELL_CRYSTAL_SPIKE_BACK    = 47936,      // spells which summon the crystal spike trap go
    //SPELL_CRYSTAL_SPIKE_FRONT1  = 47942,
    //SPELL_CRYSTAL_SPIKE_FRONT2  = 47943,

    //SPELL_CRYSTAL_SPIKE_DMG     = 47944,      // used by the crystal spike
    //SPELL_CRYSTAL_SPIKE_DMG_H   = 57067,

    SPELL_FRENZY                = 48017,
    SPELL_FRENZY_H              = 57086,

    SPELL_TRAMPLE               = 48016,
    SPELL_TRAMPLE_H             = 57066,

    SPELL_CRYSTALLINE_TANGLER   = 61555,        // on crystalline tanglers
    SPELL_SUMMON_TANGLER_H      = 61564,

    NPC_CRYSTALLINE_TANGLER     = 32665,
    NPC_SPIKE_INITIAL_TRIGGER   = 27101,        // summoned around the boss from the initial spell
    //NPC_SPIKE_TRIGGER           = 27079,      // uses the trap in order to summon the spike
    NPC_SPIKE                   = 27099,        // this npc is summoned by the crystal spike trap when used by crystal spike trigger - in acid

    //GO_CRYSTAL_SPIKE_TRAP       = 188537
};

/*######
## boss_ormorok
######*/

struct MANGOS_DLL_DECL boss_ormorokAI : public ScriptedAI
{
    boss_ormorokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsEnraged;

    uint32 m_uiTrampleTimer;
    uint32 m_uiSpellReflectTimer;
    uint32 m_uiCrystalSpikeTimer;
    uint32 m_uiTanglerTimer;

    void Reset()
    {
        m_bIsEnraged = false;

        m_uiTrampleTimer = urand(10000, 35000);
        m_uiSpellReflectTimer = urand(5000, 10000);
        m_uiCrystalSpikeTimer = urand(15000, 30000);
        m_uiTanglerTimer = 20000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ORMOROK, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_CRYSTALLINE_TANGLER)
        {
            pSummoned->CastSpell(pSummoned, SPELL_CRYSTALLINE_TANGLER, true);

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                pSummoned->AI()->AttackStart(pTarget);
        }
        else if (pSummoned->GetEntry() == NPC_SPIKE_INITIAL_TRIGGER)
        {
            // Summon spikes event should be handled by the periodic dummy aura - now it's workaround
            //pSummoned->CastSpell(pSummoned, SPELL_CRYSTAL_SPIKE_PER, true);

            // summon an initial spike
            pSummoned->CastSpell(pSummoned, SPELL_SPIKE_SUMMON, true);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                m_bIsEnraged = true;
                DoScriptText(EMOTE_BOSS_GENERIC_FRENZY, m_creature);
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : SPELL_FRENZY_H);
            }
        }

        if (m_uiTrampleTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TRAMPLE : SPELL_TRAMPLE_H);
            m_uiTrampleTimer = urand(10000, 35000);
        }
        else
            m_uiTrampleTimer -= uiDiff;

        if (m_uiSpellReflectTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTION);
            m_uiSpellReflectTimer = urand(25000, 40000);
        }
        else
            m_uiSpellReflectTimer -= uiDiff;

        if (m_uiCrystalSpikeTimer < uiDiff)
        {
            DoScriptText(SAY_ICESPIKE, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_CRYSTAL_SPIKES);
            m_uiCrystalSpikeTimer = urand(15000, 30000);
        }
        else
            m_uiCrystalSpikeTimer -= uiDiff;

        if (!m_bIsRegularMode)
        {
            if (m_uiTanglerTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_SUMMON_TANGLER_H);
                m_uiTanglerTimer = urand(15000, 25000);
            }
            else
                m_uiTanglerTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ormorok(Creature* pCreature)
{
    return new boss_ormorokAI(pCreature);
}

struct MANGOS_DLL_DECL npc_crystal_spike_initial_triggerAI : public Scripted_NoMovementAI
{
    npc_crystal_spike_initial_triggerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpikeSummonTimer;
    uint8 m_uiSummonCount;
    uint8 m_uiDistance;

    void Reset()
    {
        m_uiSpikeSummonTimer = 250;     // the periodic dummy spell tick
        m_uiSummonCount = 0;
        m_uiDistance = MAX_SPIKES_DISTANCE;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiSummonCount < MAX_SPIKES)
        {
            // summon the spikes
            if (m_uiSpikeSummonTimer < uiDiff)
            {
                if (m_pInstance)
                {
                    if (Creature* pOrmorok = m_pInstance->GetSingleCreatureFromStorage(NPC_ORMOROK))
                    {
                        // coordinates == TargetOF(47941).position + TargetOf(47941).GetAngle() * GetTickCount() * DIST_PER_TICK

                        float fAngle = m_creature->GetAngle(pOrmorok);
                        float fX, fY;

                        m_creature->GetNearPoint2D(fX, fY, m_uiDistance, fAngle);
                        m_creature->SummonCreature(NPC_SPIKE, fX, fY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
                    }
                }

                ++m_uiSummonCount;
                m_uiDistance += MAX_SPIKES_DISTANCE;
                m_uiSpikeSummonTimer = 250;
            }
            else
                m_uiSpikeSummonTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_crystal_spike_initial_trigger(Creature* pCreature)
{
    return new npc_crystal_spike_initial_triggerAI(pCreature);
}

void AddSC_boss_ormorok()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ormorok";
    pNewScript->GetAI = &GetAI_boss_ormorok;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_crystal_spike_initial_trigger";
    pNewScript->GetAI = &GetAI_npc_crystal_spike_initial_trigger;
    pNewScript->RegisterSelf();
}
