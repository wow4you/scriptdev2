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
SDName: boss_eregos
SD%Complete:
SDComment:
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

enum
{
    SAY_AGGRO                       = -1578012,
    SAY_ARCANE_SHIELD               = -1578013,
    SAY_FIRE_SHIELD                 = -1578014,
    SAY_NATURE_SHIELD               = -1578015,
    SAY_FRENZY                      = -1578016,
    SAY_KILL_1                      = -1578017,
    SAY_KILL_2                      = -1578018,
    SAY_KILL_3                      = -1578019,
    SAY_DEATH                       = -1578020,

    // spells
    SPELL_ARCANE_BARRAGE            = 50804,
    SPELL_ARCANE_BARRAGE_H          = 59381,
    SPELL_ARCANE_VOLLEY             = 51153,
    SPELL_ARCANE_VOLLEY_H           = 59382,
    SPELL_ENRAGED_ASSAULT           = 51170,
    SPELL_PLANAR_ANOMALIES          = 57959,
    SPELL_SUMMON_LEY_WHELP          = 51175,
    SPELL_PLANAR_SHIFT              = 51162,

    MAX_WHELPS                      = 2,        // The max number of whelps summoned at once
    MAX_ANOMALIES                   = 5,

    //NPC_PLANAR_ANOMALY              = 30879,
    //NPC_GREATER_LEY_WHELP           = 28276,
};

struct MANGOS_DLL_DECL boss_eregosAI : public ScriptedAI
{
    boss_eregosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiArcaneBarrageTimer;
    uint32 m_uiArcaneVolleyTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSummonWhelpsTimer;
    uint8 m_uiHpPercent;

    void Reset()
    {
        m_uiArcaneBarrageTimer  = 5000;
        m_uiArcaneVolleyTimer   = 15000;
        m_uiEnrageTimer         = 25000;
        m_uiSummonWhelpsTimer   = 2000;
        m_uiHpPercent           = 60;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_EREGOS, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_EREGOS, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EREGOS, FAIL);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_creature->HasAura(SPELL_PLANAR_SHIFT))
            return;

        if(m_creature->GetHealthPercent() < m_uiHpPercent)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_PLANAR_SHIFT) == CAST_OK)
            {
                // summon 5 anomalies (shold it depend on the players in instance?)
                for(uint8 i = 0; i < MAX_ANOMALIES; i++)
                    DoCastSpellIfCan(m_creature, SPELL_PLANAR_ANOMALIES, CAST_TRIGGERED);

                switch(urand(0, 2))
                {
                    case 0: DoScriptText(SAY_ARCANE_SHIELD, m_creature); break;
                    case 1: DoScriptText(SAY_FIRE_SHIELD, m_creature); break;
                    case 2: DoScriptText(SAY_NATURE_SHIELD, m_creature); break;
                }

                // set next phase to 20%
                m_uiHpPercent -= 40;
            }
        }

        if(m_uiArcaneBarrageTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_ARCANE_BARRAGE : SPELL_ARCANE_BARRAGE_H) == CAST_OK)
                    m_uiArcaneBarrageTimer = urand(3000, 5000);
            }
        }
        else
            m_uiArcaneBarrageTimer -= uiDiff;

        if(m_uiSummonWhelpsTimer < uiDiff)
        {
            for(uint8 i = 0; i < MAX_WHELPS; i++)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_LEY_WHELP, CAST_TRIGGERED) == CAST_OK)
                    m_uiSummonWhelpsTimer = 20000;
            }
        }
        else
            m_uiSummonWhelpsTimer -= uiDiff;

        if(m_uiArcaneVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARCANE_VOLLEY : SPELL_ARCANE_VOLLEY_H) == CAST_OK)
                m_uiArcaneVolleyTimer = urand(12000, 15000);
        }
        else
            m_uiArcaneVolleyTimer -= uiDiff;

        if(m_uiEnrageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGED_ASSAULT) == CAST_OK)
            {
                DoScriptText(SAY_FRENZY, m_creature);
                m_uiEnrageTimer = urand(20000, 30000);
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eregos(Creature* pCreature)
{
    return new boss_eregosAI(pCreature);
}

void AddSC_boss_eregos()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_eregos";
    pNewScript->GetAI = &GetAI_boss_eregos;
    pNewScript->RegisterSelf();
}
