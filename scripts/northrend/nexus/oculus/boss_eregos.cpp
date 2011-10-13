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

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_EREGOS, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
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

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

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
