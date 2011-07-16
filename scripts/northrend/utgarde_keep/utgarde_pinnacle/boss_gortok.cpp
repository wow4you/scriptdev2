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
SDName: Boss_Gortok
SD%Complete: 20%
SDComment:
SDCategory: Utgarde Pinnacle
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    SAY_AGGRO               = -1575015,
    SAY_SLAY_1              = -1575016,
    SAY_SLAY_2              = -1575017,
    SAY_DEATH               = -1575018,

    //SPELL_FREEZE_ANIM       = 16245,

    SPELL_IMPALE            = 48261,
    SPELL_IMPALE_H          = 59268,

    SPELL_WITHERING_ROAR    = 48256,
    SPELL_WITHERING_ROAR_H  = 59267,

    SPELL_ARCING_SMASH      = 48260,

    SPELL_ORB_VISUAL        = 48044,

    POINT_ID_CENTER         = 0
};

static const float fOrbMoveLoc[3] = {272.93f, -451.92f, 110.f};

/*######
## boss_gortok
######*/

struct MANGOS_DLL_DECL boss_gortokAI : public ScriptedAI
{
    boss_gortokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiRoarTimer;
    uint32 m_uiImpaleTimer;
    uint32 m_uiArcingSmashTimer;

    void Reset()
    {
        m_uiRoarTimer        = 10000;
        m_uiImpaleTimer      = urand(15000,25000);
        m_uiArcingSmashTimer = urand(10000,30000);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GORTOK, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GORTOK, FAIL);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_STATIS_ORB)
            pSummoned->CastSpell(pSummoned, SPELL_ORB_VISUAL, true);
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE || uiPointId != POINT_ID_CENTER || pSummoned->GetEntry() != NPC_STATIS_ORB)
            return;

        pSummoned->CastSpell(pSummoned, SPELL_AWAKEN_SUBBOSS, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiRoarTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WITHERING_ROAR : SPELL_WITHERING_ROAR_H) == CAST_OK)
                m_uiRoarTimer = 10000;
        }
        else
            m_uiRoarTimer -= uiDiff;

        if(m_uiImpaleTimer < uiDiff)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H) == CAST_OK)
                    m_uiImpaleTimer = urand(15000, 25000);
            }
        }
        else
            m_uiImpaleTimer -= uiDiff;

        if(m_uiArcingSmashTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCING_SMASH) == CAST_OK)
                m_uiArcingSmashTimer = urand(10000, 30000);
        }
        else
            m_uiArcingSmashTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gortok(Creature* pCreature)
{
    return new boss_gortokAI(pCreature);
}

bool ProcessEventId_event_start_gortok(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (instance_pinnacle* pInstance = (instance_pinnacle*)((Creature*)pSource)->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_GORTOK) == DONE || pInstance->GetData(TYPE_GORTOK) == IN_PROGRESS)
            return false;

        if (Creature* pGortok = pInstance->GetSingleCreatureFromStorage(NPC_GORTOK))
        {
            // summon statis orb at statis object location
            if (GameObject* pStasis = pInstance->GetSingleGameObjectFromStorage(GO_STASIS_GENERATOR))
            {
                if (Creature* pOrb = pGortok->SummonCreature(NPC_STATIS_ORB, pStasis->GetPositionX(), pStasis->GetPositionY(), pStasis->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
                    pOrb->GetMotionMaster()->MovePoint(POINT_ID_CENTER, fOrbMoveLoc[0], fOrbMoveLoc[1], fOrbMoveLoc[2]);
            }
        }

        pInstance->SetData(TYPE_GORTOK, IN_PROGRESS);

        return true;
    }
    return false;
}

void AddSC_boss_gortok()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_gortok";
    pNewScript->GetAI = &GetAI_boss_gortok;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_start_gortok";
    pNewScript->pProcessEventId = &ProcessEventId_event_start_gortok;
    pNewScript->RegisterSelf();
}
