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
SDName: boss_ignis
SD%Complete: 0%
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                           = -1603026,
    SAY_SCORCH_1                        = -1603027,
    SAY_SCORCH_2                        = -1603028,
    SAY_SLAGPOT                         = -1603029,
    SAY_ADDS                            = -1603030,
    SAY_SLAY_1                          = -1603031,
    SAY_SLAY_2                          = -1603032,
    SAY_BERSERK                         = -1603033,
    SAY_DEATH                           = -1603034,

    EMOTE_FLAME_JETS                    = -1603035,

    SPELL_FLAME_JETS                    = 62680,
    SPELL_FLAME_JETS_H                  = 63472,
    SPELL_SLAG_POT                      = 62717,
    SPELL_SLAG_POT_H                    = 63477,
    SPELL_SCORCH                        = 62546,
    SPELL_SCORCH_H                      = 63474,
    SPELL_SCORCH_SUMMON                 = 62551,
    SPELL_STRENGTH_OF_CREATOR           = 64473,            // cast on self - aura
    SPELL_ACTIVATE_CONSTRUCT            = 62488,
    SPELL_BERSERK                       = 26662,

    //iron construct
    SPELL_HEAT                          = 65667,
    SPELL_MOLTEN                        = 62373,
    SPELL_BRITTLE                       = 62382,
    SPELL_SHATTER                       = 62383,
    SPELL_ACTIVATE_CONSTRUCT_HIT        = 63850,
    SPELL_STRENGTH_OF_CREATOR_IGNIS     = 64474,            // cast on ignis
    SPELL_STRENGTH_OF_CREATOR_EFFECT    = 64475,            // dummy spell to remove the buffs
    SPELL_FREEZE_ANIM                   = 63354,

    //scorch target
    SPELL_SCORCH_AURA                   = 62548,
    SPELL_SCORCH_AURA_H                 = 63476,

    //NPC ids
    NPC_IRON_CONSTRUCT                  = 33121,
    NPC_SCORCH                          = 33221,
};

struct MANGOS_DLL_DECL npc_iron_constructAI : public ScriptedAI
{
    npc_iron_constructAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiAuraCheckTimer;
    uint32 m_uiWaterCheckTimer;

    void Reset()
    {
        m_uiAuraCheckTimer  = 1000;
        m_uiWaterCheckTimer = 1000;
        DoCastSpellIfCan(m_creature, SPELL_FREEZE_ANIM);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        if (Creature* pIgnis = m_pInstance->GetSingleCreatureFromStorage(NPC_IGNIS))
            DoCastSpellIfCan(pIgnis, SPELL_STRENGTH_OF_CREATOR_EFFECT, CAST_TRIGGERED);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (m_creature->HasAura(SPELL_BRITTLE))
        {
            if (uiDamage > 5000)
                DoCastSpellIfCan(m_creature, SPELL_SHATTER, true);
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pCaster->GetEntry() == NPC_IGNIS && pSpell->Id == SPELL_ACTIVATE_CONSTRUCT_HIT)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_STRENGTH_OF_CREATOR_IGNIS) == CAST_OK)
            {
                if (m_creature->HasAura(SPELL_FREEZE_ANIM, EFFECT_INDEX_0))
                    m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);
                m_creature->SetInCombatWithZone();
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->HasAura(SPELL_BRITTLE))
            return;

        if (!m_creature->HasAura(SPELL_MOLTEN))
        {
            if (m_uiAuraCheckTimer < uiDiff)
            {
                if (SpellAuraHolder* pHeatAura = m_creature->GetSpellAuraHolder(SPELL_HEAT))
                {
                    if (pHeatAura->GetStackAmount() == 10)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_MOLTEN);
                        m_creature->RemoveAurasDueToSpell(SPELL_HEAT);
                    }
                }
                m_uiAuraCheckTimer = 1000;
            }
            else
                m_uiAuraCheckTimer -= uiDiff;
        }
        else
        {
            if (m_uiWaterCheckTimer < uiDiff)
            {
                if (m_creature->IsInWater())
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_BRITTLE) == CAST_OK)
                        m_creature->RemoveAurasDueToSpell(SPELL_MOLTEN);
                }
                m_uiWaterCheckTimer = 1000;
            }
            else
                m_uiWaterCheckTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_iron_construct(Creature* pCreature)
{
    return new npc_iron_constructAI(pCreature);
}

struct MANGOS_DLL_DECL boss_ignisAI : public ScriptedAI
{
    boss_ignisAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    GuidList m_lIronConstructGuids;

    uint32 m_uiBerserkTimer;
    uint32 m_uiFlameJetsTimer;
    uint32 m_uiSlagPotTimer;
    uint32 m_uiScorchTimer;
    uint32 m_uiConstructTimer;

    void Reset()
    {
        m_uiBerserkTimer        = 10 * MINUTE * IN_MILLISECONDS;
        m_uiFlameJetsTimer      = 20000;
        m_uiSlagPotTimer        = 25000;
        m_uiScorchTimer         = 13000;
        m_uiConstructTimer      = 10000;

        // respawn dead constructs
        if (!m_lIronConstructGuids.empty())
        {
            for (GuidList::iterator itr = m_lIronConstructGuids.begin(); itr != m_lIronConstructGuids.end(); ++itr)
            {
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (!pTemp->isAlive())
                        pTemp->Respawn();
                }
            }
        }
        m_lIronConstructGuids.clear();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, FAIL);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_SCORCH)
            pSummoned->CastSpell(pSummoned, m_bIsRegularMode ? SPELL_SCORCH_AURA : SPELL_SCORCH_AURA_H, false);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        switch (pSpell->Id)
        {
            case SPELL_ACTIVATE_CONSTRUCT_HIT:
                if (pTarget->GetEntry() == NPC_IRON_CONSTRUCT)
                    m_lIronConstructGuids.push_back(pTarget->GetObjectGuid());
                break;
            case SPELL_SCORCH:
            case SPELL_SCORCH_H:
                if (pTarget-> GetEntry() == NPC_IGNIS)
                    DoCastSpellIfCan(m_creature, SPELL_SCORCH_SUMMON, CAST_TRIGGERED);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer < uiDiff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                m_uiBerserkTimer = 60000;
            }
        }
        else
            m_uiBerserkTimer -= uiDiff;

        if (m_uiFlameJetsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FLAME_JETS : SPELL_FLAME_JETS_H) == CAST_OK)
            {
                DoScriptText(EMOTE_FLAME_JETS, m_creature);
                m_uiFlameJetsTimer = 35000;
            }
        }
        else
            m_uiFlameJetsTimer -= uiDiff;

        if (m_uiSlagPotTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SLAG_POT : SPELL_SLAG_POT_H) == CAST_OK)
                {
                    DoScriptText(SAY_SLAGPOT, m_creature);
                    m_uiSlagPotTimer = 30000;
                }
            }
        }
        else
            m_uiSlagPotTimer -= uiDiff;

        if (m_uiConstructTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ACTIVATE_CONSTRUCT) == CAST_OK)
            {
                DoScriptText(SAY_ADDS, m_creature);
                m_uiConstructTimer = 40000;
            }
        }
        else
            m_uiConstructTimer -= uiDiff;

        if (m_uiScorchTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SCORCH : SPELL_SCORCH_H) == CAST_OK)
            {
                DoScriptText(urand(0, 1) ? SAY_SCORCH_1 : SAY_SCORCH_2, m_creature);
                m_uiScorchTimer = 28000;
            }
        }
        else
            m_uiScorchTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ignis(Creature* pCreature)
{
    return new boss_ignisAI(pCreature);
}

void AddSC_boss_ignis()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ignis";
    pNewScript->GetAI = GetAI_boss_ignis;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_iron_construct";
    pNewScript->GetAI = &GetAI_npc_iron_construct;
    pNewScript->RegisterSelf();
}
