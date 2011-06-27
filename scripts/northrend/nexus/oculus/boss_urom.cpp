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
SDName: boss_urom
SD%Complete:
SDComment:
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

enum
{
    SAY_SUMMON_1                    = -1578001,
    SAY_SUMMON_2                    = -1578002,
    SAY_SUMMON_3                    = -1578003,
    SAY_AGGRO                       = -1578004,
    SAY_EXPLOSION_1                 = -1578005,
    SAY_EXPLOSION_2                 = -1578006,
    SAY_KILL_1                      = -1578007,
    SAY_KILL_2                      = -1578008,
    SAY_KILL_3                      = -1578009,
    SAY_DEATH                       = -1578010,
};

struct MANGOS_DLL_DECL boss_uromAI : public ScriptedAI
{
    boss_uromAI(Creature* pCreature) : ScriptedAI(pCreature)
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
            m_pInstance->SetData(TYPE_UROM, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_UROM, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_UROM, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_urom(Creature* pCreature)
{
    return new boss_uromAI(pCreature);
}

void AddSC_boss_urom()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_urom";
    pNewScript->GetAI = &GetAI_boss_urom;
    pNewScript->RegisterSelf();
}
