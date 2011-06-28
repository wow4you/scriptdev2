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

    // spells
    SPELL_ARCANE_SHIELD             = 53813,
    SPELL_ARCANE_EXPLOSION          = 51110,
    SPELL_ARCANE_EXPLOSION_H        = 59377,
    SPELL_FROSTBOMB                 = 51103,
    SPELL_TIME_BOMB                 = 51121,
    SPELL_TIME_BOMB_H               = 59376,
    SPELL_SUMMON_MENAGERIE_1        = 50476,
    SPELL_SUMMON_MENAGERIE_2        = 50495,
    SPELL_SUMMON_MENAGERIE_3        = 50496,
    SPELL_TELEPORT                  = 51112,

    // npcs
    NPC_PHANTASMAL_CLOUDSCRAPER     = 27645,
    NPC_PHANTASMAL_MAMMOTH          = 27642,
    NPC_PHANTASMAL_WOLF             = 27644,

    NPC_PHANTASMAL_AIR              = 27650,
    NPC_PHANTASMAL_FIRE             = 27651,
    NPC_PHANTASMAL_WATER            = 27653,

    NPC_PHANTASMAL_MURLOC           = 27649,
    NPC_PHANTASMAL_NAGAL            = 27648,
    NPC_PHANTASMAL_OGRE             = 27647,

    MAX_MOBS                        = 3,

    // phases
    PHASE_PLATFORMS                 = 0,
    PHASE_CENTER                    = 1
};

static uint32 uiTrashPacks[MAX_MOBS][MAX_MOBS]=
{
    {NPC_PHANTASMAL_CLOUDSCRAPER,   NPC_PHANTASMAL_MAMMOTH, NPC_PHANTASMAL_WOLF},
    {NPC_PHANTASMAL_AIR,            NPC_PHANTASMAL_FIRE,    NPC_PHANTASMAL_WATER},
    {NPC_PHANTASMAL_MURLOC,         NPC_PHANTASMAL_NAGAL,   NPC_PHANTASMAL_OGRE},
};

struct MANGOS_DLL_DECL boss_uromAI : public ScriptedAI
{
    boss_uromAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        // randomize mobs
        for(uint8 i = 0; i < MAX_MOBS; ++i)
            m_vuiTrashPacksIds.push_back(i);

        std::random_shuffle(m_vuiTrashPacksIds.begin(), m_vuiTrashPacksIds.end());

        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint8 m_uiPhase;
    uint8 m_uiPlatformPhase;
    bool m_bIsTeleporting;
    uint32 m_uiExplosionTimer;
    uint32 m_uiTeleportTimer;
    uint32 m_uiFrostBombTimer;
    uint32 m_uiTimeBombTimer;

    std::vector<uint32> m_vuiTrashPacksIds;

    void Reset()
    {
        DoCastSpellIfCan(m_creature, SPELL_ARCANE_SHIELD);

        m_bIsTeleporting    = false;
        m_uiPlatformPhase   = 0;
        m_uiExplosionTimer  = 0;
        m_uiPhase           = PHASE_PLATFORMS;
        m_uiTeleportTimer   = urand(30000, 35000);
        m_uiFrostBombTimer  = urand(5000, 8000);
        m_uiTimeBombTimer   = urand(20000, 25000);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_UROM, IN_PROGRESS);
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
            m_pInstance->SetData(TYPE_UROM, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_UROM, FAIL);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_uiPlatformPhase < 3)
        {
            if (!m_bIsTeleporting)
            {
                // Don't take damage, just summon the trash mobs pack
                m_bIsTeleporting = true;
                m_creature->InterruptNonMeleeSpells(false);
                DoSpawnTrashPack();

                // teleport to next platform and spawn adds
                switch(m_uiPlatformPhase)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_MENAGERIE_1) == CAST_OK)
                            DoScriptText(SAY_SUMMON_1, m_creature);
                        break;
                    case 1:
                        if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_MENAGERIE_2) == CAST_OK)
                            DoScriptText(SAY_SUMMON_2, m_creature);
                        break;
                    case 2:
                        if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_MENAGERIE_3) == CAST_OK)
                            DoScriptText(SAY_SUMMON_3, m_creature);
                        break;
                }
            }

            return;
        }
        else
        {
            if (m_uiPhase == PHASE_PLATFORMS)
            {
                DoScriptText(SAY_AGGRO, m_creature);
                m_uiPhase = PHASE_CENTER;
                m_creature->InterruptNonMeleeSpells(false);
            }
            ScriptedAI::AttackStart(pWho);
        }
    }

    void JustSummoned(Creature* pSummon)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummon->AI()->AttackStart(pTarget);
    }

    void DoSpawnTrashPack()
    {
        float fX, fY, fZ;
        for(uint8 i = 0; i < MAX_MOBS; ++i)
        {
            m_creature->GetNearPoint(m_creature, fX, fY, fZ, 0, 10.0f, 2*M_PI_F/3 * i);
            m_creature->SummonCreature(uiTrashPacks[m_vuiTrashPacksIds[m_uiPlatformPhase]][i], fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_creature->HasAura(SPELL_ARCANE_SHIELD) || m_uiPhase == PHASE_PLATFORMS)
            uiDamage = 0;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        switch(pSpell->Id)
        {
            case SPELL_SUMMON_MENAGERIE_3:
            case SPELL_SUMMON_MENAGERIE_2:
            case SPELL_SUMMON_MENAGERIE_1:
                m_creature->RemoveAllAuras();
                DoCastSpellIfCan(m_creature, SPELL_ARCANE_SHIELD);
                m_bIsTeleporting = false;
                ++m_uiPlatformPhase;
                break;
            case SPELL_TELEPORT:
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARCANE_EXPLOSION : SPELL_ARCANE_EXPLOSION_H, CAST_TRIGGERED);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiPhase == PHASE_PLATFORMS)
        {
            // set the boss shield
            if (!m_creature->HasAura(SPELL_ARCANE_SHIELD))
                DoCastSpellIfCan(m_creature, SPELL_ARCANE_SHIELD);
        }
        else if (m_uiPhase == PHASE_CENTER)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (m_uiExplosionTimer)
            {
                if (m_uiExplosionTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARCANE_EXPLOSION : SPELL_ARCANE_EXPLOSION_H) == CAST_OK)
                    {
                        m_creature->GetMotionMaster()->Clear();
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        m_uiExplosionTimer = 0;
                    }
                }
                else
                    m_uiExplosionTimer -= uiDiff;
            }

            if (m_uiTeleportTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT) == CAST_OK)
                {
                    DoScriptText(urand(0, 1) ? SAY_EXPLOSION_1 : SAY_EXPLOSION_2, m_creature);
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_uiTeleportTimer = urand(30000, 35000);
                    m_uiExplosionTimer = 1000;
                }
            }
            else
                m_uiTeleportTimer -= uiDiff;

            if (m_uiFrostBombTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBOMB) == CAST_OK)
                    m_uiFrostBombTimer = urand(5000, 8000);
            }
            else
                m_uiFrostBombTimer -= uiDiff;

            if (m_uiTimeBombTimer <= uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_TIME_BOMB : SPELL_TIME_BOMB_H) == CAST_OK)
                        m_uiTimeBombTimer = urand(20000, 25000);
                }
            }
            else
                m_uiTimeBombTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
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
