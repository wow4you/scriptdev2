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
SDName: Boss_Colossus
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010,

    // collosus' abilities
    SPELL_EMERGE                = 54850,
    SPELL_MIGHTY_BLOW           = 54719,
    SPELL_MORTAL_STRIKES        = 54715,
    SPELL_MORTAL_STRIKES_H      = 59454,

    // elemental's abilities
    SPELL_MERGE                 = 54878,
    SPELL_SURGE                 = 54801,
    SPELL_MOJO_VOLLEY           = 59453,
    SPELL_MOJO_VOLLEY_H         = 54849
};

/*######
## boss_colossus
######*/

struct MANGOS_DLL_DECL boss_colossusAI : public ScriptedAI
{
    boss_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bFirstEmerge;

    uint32 m_uiMightyBlowTimer;

    void Reset()
    {
        m_bFirstEmerge = false;
        m_uiMightyBlowTimer = 10000;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_MORTAL_STRIKES : SPELL_MORTAL_STRIKES_H);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_MERGE)
        {
            // re-activate colossus here
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            if (m_creature->HasAura(SPELL_FREEZE_ANIM))
                m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);

            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            ((Creature*)pCaster)->ForcedDespawn();
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ELEMENTAL)
        {
            // handle elemental stuff
            if (m_creature->GetHealthPercent() < 25.0f)
                pSummoned->SetHealth(pSummoned->GetHealth()*.5);

            if (m_creature->getVictim())
                pSummoned->AI()->AttackStart(m_creature->getVictim());
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (!m_bFirstEmerge && m_creature->GetHealthPercent() < 50.0f)
        {
            m_bFirstEmerge = true;
            DoEmergeElemental();
        }
        else if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (!m_creature->HasAura(SPELL_FREEZE_ANIM))
                DoEmergeElemental();
        }
    }

    void DoEmergeElemental()
    {
        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        DoCastSpellIfCan(m_creature, SPELL_FREEZE_ANIM);
        DoCastSpellIfCan(m_creature, SPELL_EMERGE, CAST_TRIGGERED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_creature->HasAura(SPELL_FREEZE_ANIM))
            return;

        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlowTimer = 10000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

struct MANGOS_DLL_DECL boss_elementalAI : public ScriptedAI
{
    boss_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSurgeTimer;

    void Reset()
    {
        m_uiSurgeTimer = 15000;
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_MOJO_VOLLEY : SPELL_MOJO_VOLLEY_H, CAST_TRIGGERED);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (m_creature->GetHealthPercent() < 50.0f)
        {
            if (m_pInstance)
            {
                if (Creature* pColossus = m_pInstance->GetSingleCreatureFromStorage(NPC_COLOSSUS))
                {
                    // if Colossus has low hp percent then skip this
                    if (pColossus->GetHealthPercent() > 25.0f)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_MERGE);
                        m_creature->GetMotionMaster()->MovePoint(0, pColossus->GetPositionX(), pColossus->GetPositionY(), pColossus->GetPositionZ());
                    }
                }
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (Creature* pColossus = m_pInstance->GetSingleCreatureFromStorage(NPC_COLOSSUS))
                pColossus->AI()->EnterEvadeMode();
        }

        m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            // kill colossus on death
            if (Creature* pColossus = m_pInstance->GetSingleCreatureFromStorage(NPC_COLOSSUS))
                pColossus->DealDamage(pColossus, pColossus->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSurgeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_SURGE) == CAST_OK)
                    m_uiSurgeTimer = 15000;
            }
        }
        else
            m_uiSurgeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_elemental(Creature* pCreature)
{
    return new boss_elementalAI(pCreature);
}

void AddSC_boss_colossus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_colossus";
    pNewScript->GetAI = &GetAI_boss_colossus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_elemental";
    pNewScript->GetAI = &GetAI_boss_elemental;
    pNewScript->RegisterSelf();
}
