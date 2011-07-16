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
SDName: instance_pinnacle
SD%Complete: 25%
SDComment:
SDCategory: Utgarde Pinnacle
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

instance_pinnacle::instance_pinnacle(Map* pMap) : ScriptedInstance(pMap),
    m_uiGortokBeastsDead(0),
    m_bIsKingBane(false)
{
    Initialize();
}

void instance_pinnacle::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

    for (uint8 i = 0; i < MAX_SPECIAL_ACHIEV_CRITS; ++i)
        m_abAchievCriteria[i] = false;
}

void instance_pinnacle::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_FURBOLG:
        case NPC_WORGEN:
        case NPC_JORMUNGAR:
        case NPC_RHINO:
            m_lGortokBeastsList.push_back(pCreature->GetObjectGuid());
            // no break here -> need to handle both in list and individual these creatures
        case NPC_GORTOK:
        case NPC_STATIS_ORB:
        case NPC_BJORN:
        case NPC_HALDOR:
        case NPC_RANULF:
        case NPC_TORGYN:
        case NPC_YMIRON:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case NPC_FLAME_BRAZIER:
            m_lFlameBraziersList.push_back(pCreature->GetObjectGuid());
            break;
    }
}

void instance_pinnacle::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_SKADI:
            if (m_auiEncounter[TYPE_SKADI] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_YMIRON:
            if (m_auiEncounter[TYPE_YMIRON] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_STASIS_GENERATOR:
            break;
        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_pinnacle::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_SVALA:
            m_auiEncounter[uiType] = uiData;
            // set the achiev to false at the beginning of the event
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_HULK, false);
            break;
        case TYPE_GORTOK:
            if (uiData != SPECIAL)
                m_auiEncounter[uiType] = uiData;
            if (uiData == FAIL)
            {
                m_uiGortokBeastsDead = 0;

                if (Creature* pOrb = GetSingleCreatureFromStorage(NPC_STATIS_ORB))
                    pOrb->ForcedDespawn();

                for (GUIDList::const_iterator itr = m_lGortokBeastsList.begin(); itr != m_lGortokBeastsList.end(); ++itr)
                {
                    if (Creature* pBeast = instance->GetCreature(*itr))
                    {
                        if (!pBeast->isAlive())
                            pBeast->Respawn();
                    }
                }
            }
            else if (uiData == SPECIAL)
            {
                ++m_uiGortokBeastsDead;

                if (Creature* pOrb = GetSingleCreatureFromStorage(NPC_STATIS_ORB))
                {
                    uint8 m_uiMaxBeasts = instance->IsRegularDifficulty() ? 2 : 4;
                    // awaken gortok if all subbosses are dead, else get next boss
                    if (m_uiGortokBeastsDead == m_uiMaxBeasts)
                    {
                        pOrb->CastSpell(pOrb, SPELL_AWAKEN_GORTOK, true);
                        pOrb->ForcedDespawn(10000);
                    }
                    else
                        pOrb->CastSpell(pOrb, SPELL_AWAKEN_SUBBOSS, true);
                }
            }
            break;
        case TYPE_SKADI:
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_DOOR_SKADI);

                // remove exploit check flag on skadi death
                if (Creature* pYmiron = GetSingleCreatureFromStorage(NPC_YMIRON))
                    pYmiron->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_YMIRON:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(GO_DOOR_YMIRON);
            else if (uiData == IN_PROGRESS)
                m_bIsKingBane = true;
            break;
        default:
            error_log("SD2: Instance Pinnacle: SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            return;
    }

    // Saving also SPECIAL for this instance
    if (uiData == DONE || uiData == SPECIAL)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_pinnacle::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_pinnacle::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_pinnacle::SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet)
{
    if (uiType < MAX_SPECIAL_ACHIEV_CRITS)
        m_abAchievCriteria[uiType] = bIsMet;
}

bool instance_pinnacle::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_INCREDIBLE_HULK:
            return m_abAchievCriteria[TYPE_ACHIEV_HULK];
        case ACHIEV_CRIT_GIRL_LOVES_SKADI:
            return m_abAchievCriteria[TYPE_ACHIEV_LOVE_SKADI];
        case ACHIEV_CRIT_KINGS_BANE:
            return m_bIsKingBane;

        default:
            return false;
    }
}

void instance_pinnacle::DoProcessCallFlamesEvent()
{
    for (GUIDList::const_iterator itr = m_lFlameBraziersList.begin(); itr != m_lFlameBraziersList.end(); ++itr)
    {
        if (Creature* pFlame = instance->GetCreature(*itr))
            pFlame->CastSpell(pFlame, SPELL_BALL_OF_FLAME, true);
    }
}

void instance_pinnacle::OnCreatureEvade(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_FURBOLG:
        case NPC_WORGEN:
        case NPC_JORMUNGAR:
        case NPC_RHINO:
            SetData(TYPE_GORTOK, FAIL);
            break;
    }
}

void instance_pinnacle::OnCreatureDeath(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_FURBOLG:
        case NPC_WORGEN:
        case NPC_JORMUNGAR:
        case NPC_RHINO:
            SetData(TYPE_GORTOK, SPECIAL);
            break;
    }
}

InstanceData* GetInstanceData_instance_pinnacle(Map* pMap)
{
    return new instance_pinnacle(pMap);
}

void AddSC_instance_pinnacle()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_pinnacle";
    pNewScript->GetInstanceData = &GetInstanceData_instance_pinnacle;
    pNewScript->RegisterSelf();
}
