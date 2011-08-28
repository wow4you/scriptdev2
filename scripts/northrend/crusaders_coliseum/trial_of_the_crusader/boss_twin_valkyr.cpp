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
SDName: trial_of_the_crusader
SD%Complete:
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    SAY_AGGRO                           = -1649056,
    SAY_BERSERK                         = -1649057,
    SAY_COLORSWITCH                     = -1649058,
    SAY_DEATH                           = -1649059,
    SAY_SLAY_1                          = -1649060,
    SAY_SLAY_2                          = -1649061,
    SAY_TO_BLACK                        = -1649062,
    SAY_TO_WHITE                        = -1649063,

    // Fjola
    SPELL_LIGHT_TWIN_SPIKE              = 66075,
    SPELL_LIGHT_SHIELD                  = 65858,
    SPELL_LIGHT_TWIN_PACT               = 65876,
    SPELL_LIGHT_VORTEX                  = 66046,
    SPELL_LIGHT_TOUCH                   = 65950,

    // Eydis
    SPELL_DARK_TWIN_SPIKE               = 66069,
    SPELL_DARK_SHIELD                   = 65874,
    SPELL_DARK_TWIN_PACT                = 65875,
    SPELL_DARK_VORTEX                   = 66058,
    SPELL_DARK_TOUCH                    = 66001,

    SPELL_SHARE_DAMAGE                  = 66133, // NYI
    SPELL_TWIN_POWER                    = 65916,
    SPELL_BERSERK                       = 64238,

    SPELL_LIGHT_ESSENCE                 = 65686,
    SPELL_DARK_ESSENCE                  = 65684,

    SPELL_EMPOWERED_DARK                = 65724,
    SPELL_EMPOWERED_LIGHT               = 65748,

    SPELL_UNLEASHED_DARK = 65808,
    SPELL_UNLEASHED_LIGHT = 65795,

    SPELL_SUMMON_LIGHT_BULLET = 66078,
    SPELL_SUMMON_DARK_BULLET = 66107,

    NPC_UNLEASHED_LIGHT = 34630,
    NPC_UNLEASHED_DARK = 34628,

    MAX_SPECIAL_ABILITIES = 4
};

/*######
## boss_eydis
######*/

struct MANGOS_DLL_DECL boss_eydisAI : public ScriptedAI
{
    boss_eydisAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiDarkTouchTimer;
    uint32 m_uiBerserkTimer;

    bool m_bIsCastingTwinPact;

    void Reset() override
    {
        m_uiDarkTouchTimer = urand(10000, 15000);

        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;
        else
            m_uiBerserkTimer = 6 * MINUTE * IN_MILLISECONDS;

        m_bIsCastingTwinPact = false;
    }

    void Aggro(Unit* /*pWho*/) override
    {
        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();
    }

    void DamageTaken(Unit* /*pDealer*/, uint32& uiDamage) override
    {
        // Hack - Share damage
        if (m_pInstance)
        {
            if (Creature* pFjola = m_pInstance->GetSingleCreatureFromStorage(NPC_FJOLA))
            {
                if (pFjola->isAlive())
                {
                    uiDamage /= 2;
                    pFjola->DealDamage(pFjola, uiDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            }
        }

        // Hack, interrupt twin pakt
        if (uiDamage && m_bIsCastingTwinPact && !m_creature->HasAura(SPELL_DARK_SHIELD) && !m_creature->HasAura(67256) && !m_creature->HasAura(67257) && !m_creature->HasAura(67258))
        {
            m_creature->InterruptNonMeleeSpells(false);
            m_bIsCastingTwinPact = false;
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell) override
    {
        if (pCaster == m_creature && (pSpell->Id == SPELL_DARK_TWIN_PACT || pSpell->Id == 67303 || pSpell->Id == 67304 || pSpell->Id == 67305))
            m_bIsCastingTwinPact = false;
    }

    void DoCastTwinPact()
    {
        DoCastSpellIfCan(m_creature, SPELL_DARK_SHIELD, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_DARK_TWIN_PACT, CAST_INTERRUPT_PREVIOUS);

        m_bIsCastingTwinPact = true;
    }

    void KilledUnit(Unit* /*pWho*/) override
    {
        DoScriptText((urand(0, 1)) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetMap()->GetDifficulty() >= RAID_DIFFICULTY_10MAN_HEROIC)
        {
            if (m_uiDarkTouchTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_DARK_TOUCH) == CAST_OK)
                    m_uiDarkTouchTimer = urand(10000, 15000);
            }
            else
                m_uiDarkTouchTimer -= uiDiff;
        }

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_TRIGGERED) == CAST_OK)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eydis(Creature* pCreature)
{
    return new boss_eydisAI(pCreature);
}

/*######
## boss_fjola
######*/

struct MANGOS_DLL_DECL boss_fjolaAI : public ScriptedAI
{
    boss_fjolaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiLightTwinSpikeTimer;
    uint32 m_uiSpecialAbilityTimer;
    uint32 m_uiLightTouchTimer;
    uint32 m_uiBerserkTimer;

    uint8 m_uiSpecialAbilityIndex;
    uint8 m_uiSpecialAbilityOrder[MAX_SPECIAL_ABILITIES];

    bool m_bIsCastingTwinPact;

    GuidList m_lEssenceGuids;

    void Reset() override
    {
        m_uiLightTwinSpikeTimer = 20000;
        m_uiSpecialAbilityTimer = 45000;
        m_uiLightTouchTimer = urand(10000, 15000);

        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;
        else
            m_uiBerserkTimer = 6 * MINUTE * IN_MILLISECONDS;

        m_uiSpecialAbilityIndex = 0;
        GenerateRandomSpecialAbilityOrder();

        m_bIsCastingTwinPact = false;

        m_lEssenceGuids.clear();
    }

    void GenerateRandomSpecialAbilityOrder()
    {
        for (uint8 i = 0; i < MAX_SPECIAL_ABILITIES; ++i)
            m_uiSpecialAbilityOrder[i] = i;

        // Generate random order
        for (uint8 i = 0; i < MAX_SPECIAL_ABILITIES; ++i)
        {
            uint8 uiIdx = urand(0, MAX_SPECIAL_ABILITIES - 1);

            if (uiIdx == i)
                continue;

            uint8 uiHelp = m_uiSpecialAbilityOrder[i];
            m_uiSpecialAbilityOrder[i] = m_uiSpecialAbilityOrder[uiIdx];
            m_uiSpecialAbilityOrder[uiIdx] = uiHelp;
        }
    }

    void JustReachedHome() override
    {
        // Despawn Essences
        for (GuidList::const_iterator itr = m_lEssenceGuids.begin(); itr != m_lEssenceGuids.end(); ++itr)
            if (Creature* pEssence = m_creature->GetMap()->GetCreature(*itr))
                pEssence->ForcedDespawn();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_TWIN_VALKYR, FAIL);

            // The first who evades despawns the sister
            if (Creature* pOtherSister = m_pInstance->GetSingleCreatureFromStorage(NPC_EYDIS))
                pOtherSister->ForcedDespawn();
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* /*pWho*/) override
    {
        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_TWIN_VALKYR, IN_PROGRESS);
            // Possibly not the correct place, but close gate here
            m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE);
        }
    }

    void DamageTaken(Unit* /*pDealer*/, uint32& uiDamage) override
    {
        // Hack - Share damage
        if (m_pInstance)
        {
            if (Creature* pEydis = m_pInstance->GetSingleCreatureFromStorage(NPC_EYDIS))
            {
                if (pEydis->isAlive())
                {
                    uiDamage /= 2;
                    pEydis->DealDamage(pEydis, uiDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            }
        }

        // Hack, interrupt twin pakt
        if (uiDamage && m_bIsCastingTwinPact && !m_creature->HasAura(SPELL_LIGHT_SHIELD) && !m_creature->HasAura(67259) && !m_creature->HasAura(67260) && !m_creature->HasAura(67261))
        {
            m_creature->InterruptNonMeleeSpells(false);
            m_bIsCastingTwinPact = false;
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pCaster == m_creature && (pSpell->Id == SPELL_LIGHT_TWIN_PACT || pSpell->Id == 67306 || pSpell->Id == 67307 || pSpell->Id == 67308))
            m_bIsCastingTwinPact = false;
    }

    void KilledUnit(Unit* /*pWho*/) override
    {
        DoScriptText((urand(0, 1)) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_DEATH, m_creature);

        // Despawn Essences
        for (GuidList::const_iterator itr = m_lEssenceGuids.begin(); itr != m_lEssenceGuids.end(); ++itr)
            if (Creature* pEssence = m_creature->GetMap()->GetCreature(*itr))
                pEssence->ForcedDespawn();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TWIN_VALKYR, DONE);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLightTwinSpikeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_LIGHT_TWIN_SPIKE) == CAST_OK)
                m_uiLightTwinSpikeTimer = 20000;
        }
        else
            m_uiLightTwinSpikeTimer -= uiDiff;

        if (m_uiSpecialAbilityTimer < uiDiff)
        {
            switch (m_uiSpecialAbilityOrder[m_uiSpecialAbilityIndex])
            {
                case 0: // Fjola Light Vortex
                    DoScriptText(SAY_TO_WHITE, m_creature);
                    DoCastSpellIfCan(m_creature, SPELL_LIGHT_VORTEX);
                    break;
                case 1: // Fjola Twin Pact
                    DoCastSpellIfCan(m_creature, SPELL_LIGHT_SHIELD, CAST_TRIGGERED);
                    DoCastSpellIfCan(m_creature, SPELL_LIGHT_TWIN_PACT, CAST_INTERRUPT_PREVIOUS);
                    m_bIsCastingTwinPact = true;

                    // The other twin gains TWIN_POWER
                    if (m_pInstance)
                        if (Creature* pEydis = m_pInstance->GetSingleCreatureFromStorage(NPC_EYDIS))
                            pEydis->CastSpell(pEydis, SPELL_TWIN_POWER, true);

                    break;
                case 2: // Eydis Dark Vortex
                    if (m_pInstance)
                    {
                        if (Creature* pEydis = m_pInstance->GetSingleCreatureFromStorage(NPC_EYDIS))
                        {
                            DoScriptText(SAY_TO_BLACK, pEydis);
                            pEydis->CastSpell(pEydis, SPELL_DARK_VORTEX, false);
                        }
                    }
                    break;
                case 3: // Eydis Twin Pact
                    if (m_pInstance)
                    {
                        if (Creature* pEydis = m_pInstance->GetSingleCreatureFromStorage(NPC_EYDIS))
                        {
                            if (boss_eydisAI* pEydisAi = dynamic_cast<boss_eydisAI*>(pEydis->AI()))
                                pEydisAi->DoCastTwinPact();

                            // The other twin gains TWIN_POWER
                            DoCastSpellIfCan(m_creature, SPELL_TWIN_POWER, CAST_TRIGGERED);
                        }
                    }
                    break;
            }

            ++m_uiSpecialAbilityIndex;

            if (m_uiSpecialAbilityIndex == MAX_SPECIAL_ABILITIES)
            {
                GenerateRandomSpecialAbilityOrder();
                m_uiSpecialAbilityIndex = 0;
            }

            m_uiSpecialAbilityTimer = 45000;
        }
        else
            m_uiSpecialAbilityTimer -= uiDiff;

        if (m_creature->GetMap()->GetDifficulty() >= RAID_DIFFICULTY_10MAN_HEROIC)
        {
            if (m_uiLightTouchTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_LIGHT_TOUCH) == CAST_OK)
                    m_uiLightTouchTimer = urand(10000, 15000);
            }
            else
                m_uiLightTouchTimer -= uiDiff;
        }

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_TRIGGERED) == CAST_OK)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }

    void JustSummoned(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
            case NPC_LIGHT_ESSENCE:
            case NPC_DARK_ESSENCE:
                m_lEssenceGuids.push_back(pCreature->GetObjectGuid());
                break;
        }
    }
};

CreatureAI* GetAI_boss_fjola(Creature* pCreature)
{
    return new boss_fjolaAI(pCreature);
}

/*######
## mob_essence
######*/

bool GossipHello_mob_essence(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_LIGHT_ESSENCE)
    {
        pPlayer->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
        pPlayer->CastSpell(pPlayer, SPELL_LIGHT_ESSENCE, true);
    }
    else if (pCreature->GetEntry() == NPC_DARK_ESSENCE)
    {
        pPlayer->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
        pPlayer->CastSpell(pPlayer, SPELL_DARK_ESSENCE, true);
    }

    return true;
}

void AddSC_twin_valkyr()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_fjola";
    pNewScript->GetAI = &GetAI_boss_fjola;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_eydis";
    pNewScript->GetAI = &GetAI_boss_eydis;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_essence";
    pNewScript->pGossipHello = &GossipHello_mob_essence;
    pNewScript->RegisterSelf();
}
