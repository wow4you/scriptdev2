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
SDName: boss_ichoron
SD%Complete: 50
SDComment: Water Globule event NYI (custom changes)
SDCategory: Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

enum
{
    SAY_AGGRO           = -1608019,
    SAY_SHATTERING      = -1608020,
    SAY_SHIELD          = -1608021,
    SAY_SLAY_1          = -1608022,
    SAY_SLAY_2          = -1608023,
    SAY_SLAY_3          = -1608024,
    SAY_ENRAGE          = -1608025,
    SAY_DEATH           = -1608026,
    EMOTE_BUBBLE        = -1608028,

    SPELL_SPLASH                = 59516,
    SPELL_DRAINED               = 59820,
    SPELL_FRENZY                = 54312,
    SPELL_FRENZY_H              = 59522,
    SPELL_PROTECTIVE_BUBBLE     = 54306,
    SPELL_WATER_BLAST           = 54237,
    SPELL_WATER_BLAST_H         = 59520,
    SPELL_WATER_BOLT_VOLLEY     = 54241,
    SPELL_WATER_BOLT_VOLLEY_H   = 59521,
    SPELL_WATER_GLOBULE         = 54260,

    SPELL_WATER_GLOBULE_SPAWN_1 = 54258,
    SPELL_WATER_GLOBULE_SPAWN_2 = 54264,
    SPELL_WATER_GLOBULE_SPAWN_3 = 54265,
    SPELL_WATER_GLOBULE_SPAWN_4 = 54266,
    SPELL_WATER_GLOBULE_SPAWN_5 = 54267,

    SPELL_MERGE                 = 54269,                // used by globules
    SPELL_WATER_GLOBULE_TRANS   = 54268,

    POINT_ID_ICHORON            = 0
};

static const uint32 aWaterGlobuleSpells[5] = {SPELL_WATER_GLOBULE_SPAWN_1, SPELL_WATER_GLOBULE_SPAWN_2, SPELL_WATER_GLOBULE_SPAWN_3, SPELL_WATER_GLOBULE_SPAWN_4, SPELL_WATER_GLOBULE_SPAWN_5};

struct MANGOS_DLL_DECL boss_ichoronAI : public ScriptedAI
{
    boss_ichoronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_violet_hold* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWaterBoltVolleyTimer;
    uint32 m_uiWaterBlastTimer;
    bool m_bIsFrenzy;
    uint32 m_uiBubbleTimer;
    uint32 m_uiGlobuleCount;

    void Reset() override
    {
        m_uiWaterBoltVolleyTimer = urand(10000, 12000);
        m_uiWaterBlastTimer      = 10000;
        m_bIsFrenzy              = false;
        m_uiBubbleTimer          = 0;
        m_uiGlobuleCount         = 0;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        DoScriptText(SAY_AGGRO, m_creature);

        DoCastSpellIfCan(m_creature, SPELL_PROTECTIVE_BUBBLE);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pWho) override
    {
        if (pWho->GetTypeId() != TYPEID_PLAYER)
            return;

        switch (urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustSummonedGlobule()
    {
        ++m_uiGlobuleCount;
    }

    void SummonedGlobuleJustDied()
    {
        if (m_uiGlobuleCount)
        {
            --m_uiGlobuleCount;

            if (!m_uiGlobuleCount)                          // All killed
                m_uiBubbleTimer = 1;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBubbleTimer)
        {
            if (m_uiBubbleTimer <= uiDiff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveAurasDueToSpell(SPELL_DRAINED);

                if (m_creature->GetHealthPercent() > 25.0f)
                {
                    DoCastSpellIfCan(m_creature, SPELL_PROTECTIVE_BUBBLE);
                    DoScriptText(SAY_SHIELD, m_creature);
                }

                m_uiBubbleTimer = 0;
            }
            else
                m_uiBubbleTimer -= uiDiff;

            // return if boss is in drained state
            return;
        }
        else if (!m_creature->HasAura(SPELL_PROTECTIVE_BUBBLE) && !m_bIsFrenzy)
        {
            DoCastSpellIfCan(m_creature, SPELL_DRAINED, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_SPLASH, CAST_TRIGGERED);
            DoScriptText(SAY_SHATTERING, m_creature);

            // there should be 10 spawns - so each target spawns 2 globules
            // Not sure if this is right!
            GuidList lAddGuids;
            if (m_pInstance)
                m_pInstance->GetIchoronTriggerList(lAddGuids);

            for (GuidList::const_iterator itr = lAddGuids.begin(); itr != lAddGuids.end(); ++itr)
            {
                if (Creature* pTarget = m_pInstance->instance->GetCreature(*itr))
                {
                    pTarget->CastSpell(pTarget, SPELL_WATER_GLOBULE_SPAWN_1, true);
                    pTarget->CastSpell(pTarget, SPELL_WATER_GLOBULE_SPAWN_2, true);
                }
            }

            // remove some hp from boss and set it unselectable
            m_creature->SetHealth(m_creature->GetHealth() - m_creature->GetMaxHealth() * 0.3f);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_uiBubbleTimer = 15000;
        }

        if (m_uiWaterBlastTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_WATER_BLAST : SPELL_WATER_BLAST_H) == CAST_OK)
                    m_uiWaterBlastTimer = urand(8000, 14000);
            }
        }
        else
            m_uiWaterBlastTimer -= uiDiff;

        if (m_uiWaterBoltVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WATER_BOLT_VOLLEY : SPELL_WATER_BOLT_VOLLEY_H) == CAST_OK)
                m_uiWaterBoltVolleyTimer = urand(7000, 12000);
        }
        else
            m_uiWaterBoltVolleyTimer -= uiDiff;

        if (!m_bIsFrenzy && m_creature->GetHealthPercent() < 25.0f)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : SPELL_FRENZY_H, CAST_TRIGGERED) == CAST_OK)
            {
                DoScriptText(SAY_ENRAGE, m_creature);
                m_bIsFrenzy = true;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ichoron(Creature* pCreature)
{
    return new boss_ichoronAI(pCreature);
}

// Small helper script to handle summoned adds
struct MANGOS_DLL_DECL npc_ichoron_summon_triggerAI : public ScriptedAI
{
    npc_ichoron_summon_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();
    }

    instance_violet_hold* m_pInstance;

    void Reset() {}
    void MoveInLineOfSight(Unit* pWho) {}
    void AttackStart(Unit* pWho) {}
    void UpdateAI(const uint32 uiDiff) {}

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->CastSpell(pSummoned, SPELL_WATER_GLOBULE, false);

        if (m_pInstance)
        {
            if (Creature* pBoss = m_pInstance->GetSingleCreatureFromStorage(m_pInstance->GetData(TYPE_ICHORON) != DONE ? NPC_ICHORON : NPC_SWIRLING))
            {
                if (!pBoss->isAlive())
                    return;

                pSummoned->GetMotionMaster()->MovePoint(POINT_ID_ICHORON, pBoss->GetPositionX(), pBoss->GetPositionY(), pBoss->GetPositionZ());

                if (boss_ichoronAI* pIchoAi = dynamic_cast<boss_ichoronAI*>(pBoss->AI()))
                    pIchoAi->JustSummonedGlobule();
            }
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (Creature* pBoss = m_pInstance->GetSingleCreatureFromStorage(m_pInstance->GetData(TYPE_ICHORON) != DONE ? NPC_ICHORON : NPC_SWIRLING))
        {
            if (boss_ichoronAI* pIchoAi = dynamic_cast<boss_ichoronAI*>(pBoss->AI()))
                pIchoAi->SummonedGlobuleJustDied();
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE || uiPointId != POINT_ID_ICHORON)
            return;

        if (m_pInstance)
        {
            if (Creature* pBoss = m_pInstance->GetSingleCreatureFromStorage(m_pInstance->GetData(TYPE_ICHORON) != DONE ? NPC_ICHORON : NPC_SWIRLING))
            {
                if (!pBoss->isAlive())
                    return;

                // Set Achievement as failed
                m_pInstance->SetData(TYPE_ICHORON, SPECIAL);

                // despawn globule and modify boss hp - workaround because of the missing script effect of the spell
                pSummoned->CastSpell(pSummoned, SPELL_MERGE, false);
                pBoss->SetHealth(pBoss->GetHealth() + pBoss->GetMaxHealth() * 0.03f);
                pSummoned->ForcedDespawn(2000);
            }
        }
    }
};

CreatureAI* GetAI_npc_ichoron_summon_trigger(Creature* pCreature)
{
    return new npc_ichoron_summon_triggerAI(pCreature);
}

void AddSC_boss_ichoron()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ichoron";
    pNewScript->GetAI = &GetAI_boss_ichoron;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_ichoron_summon_trigger";
    pNewScript->GetAI = &GetAI_npc_ichoron_summon_trigger;
    pNewScript->RegisterSelf();
}
