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
SDName: instance_oculus
SD%Complete:
SDComment:
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

instance_oculus::instance_oculus(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_oculus::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_oculus::OnPlayerEnter(Player *pPlayer)
{
    // summon end bosses on reload
    DoSummonNextBossIfCan(true);

    // show world state if construct event is in progress
    if (GetData(TYPE_DRAKOS) == DONE && GetData(TYPE_VAROS) != DONE)
    {
        DoUpdateWorldState(WORLD_STATE_CON, 1);
        DoUpdateWorldState(WORLD_STATE_CON_COUNT, m_sConstructsGUIDs.size());
    }
}

void instance_oculus::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_VAROS:
            if (GetData(TYPE_CONSTRUCTS) != DONE)
            {
                pCreature->CastSpell(pCreature, SPELL_CENTRIFUGE_SHIELD, false);
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
        case NPC_DRAKOS:
        case NPC_UROM:
        case NPC_EREGOS:
        case NPC_ETERNOS:
        case NPC_VERDISA:
        case NPC_BELGARISTRASZ:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;

        case NPC_CONSTRUCT:
            m_sConstructsGUIDs.insert(pCreature->GetObjectGuid());
            break;
        case NPC_AZURE_CAPTAIN:
        case NPC_AZURE_GUARDIAN:
            m_sCaptainsGUIDs.insert(pCreature->GetObjectGuid());
            break;
    }
}

void instance_oculus::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CACHE_EREGOS:
        case GO_CACHE_EREGOS_H:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
        case GO_DRAGON_CAGE_DOOR:
            m_lCageDoorGUIDs.push_back(pGo->GetObjectGuid());
            if (m_auiEncounter[TYPE_DRAKOS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
    }
}

void instance_oculus::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_DRAKOS:
            m_auiEncounter[TYPE_DRAKOS] = uiData;
            if (uiData == DONE)
            {
                // open cages
                DoOpenAllCages();

                // varos intro
                if (Creature* pVaros = GetSingleCreatureFromStorage(NPC_VAROS))
                    DoScriptText(SAY_VAROS_INTRO, pVaros);

                // update world state
                DoUpdateWorldState(WORLD_STATE_CON, 1);
                DoUpdateWorldState(WORLD_STATE_CON_COUNT, m_sConstructsGUIDs.size());

                // start timed achiev
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_EREGOS_ID);
            }
            break;
        case TYPE_VAROS:
            m_auiEncounter[TYPE_VAROS] = uiData;
            if (uiData == DONE)
            {
                // summon urom
                DoSummonNextBossIfCan();

                // update world state
                DoUpdateWorldState(WORLD_STATE_CON, 0);
            }
            break;
        case TYPE_UROM:
            m_auiEncounter[TYPE_UROM] = uiData;
            if (uiData == DONE)
                DoSummonNextBossIfCan();
            break;
        case TYPE_EREGOS:
            m_auiEncounter[TYPE_EREGOS] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_EREGOS : GO_CACHE_EREGOS_H);
            break;
        case TYPE_CONSTRUCTS:
            m_auiEncounter[TYPE_CONSTRUCTS] = uiData;
            if (uiData == DONE)
            {
                if (Creature* pVaros = GetSingleCreatureFromStorage(NPC_VAROS))
                {
                    pVaros->InterruptNonMeleeSpells(false);
                    pVaros->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[TYPE_DRAKOS] << " " << m_auiEncounter[TYPE_VAROS] << " " << m_auiEncounter[TYPE_UROM] << " " << m_auiEncounter[TYPE_EREGOS] <<
            " " << m_auiEncounter[TYPE_CONSTRUCTS];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_oculus::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_oculus::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[TYPE_DRAKOS] >> m_auiEncounter[TYPE_VAROS] >> m_auiEncounter[TYPE_UROM] >> m_auiEncounter[TYPE_EREGOS] >> m_auiEncounter[TYPE_CONSTRUCTS];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_oculus::DoOpenAllCages()
{
    // open all doors
    for(GUIDList::const_iterator itr = m_lCageDoorGUIDs.begin(); itr != m_lCageDoorGUIDs.end(); ++itr)
        DoUseDoorOrButton(*itr);

    // get the prisoners out
    if(Creature* pEternos = GetSingleCreatureFromStorage(NPC_ETERNOS))
        pEternos->GetMotionMaster()->MovePoint(0, m_sPrisonersMoveLocs[0].m_fX, m_sPrisonersMoveLocs[0].m_fY, m_sPrisonersMoveLocs[0].m_fZ);

    if(Creature* pVerdisa = GetSingleCreatureFromStorage(NPC_VERDISA))
        pVerdisa->GetMotionMaster()->MovePoint(0, m_sPrisonersMoveLocs[1].m_fX, m_sPrisonersMoveLocs[1].m_fY, m_sPrisonersMoveLocs[1].m_fZ);

    if(Creature* pBelgaristrasz = GetSingleCreatureFromStorage(NPC_BELGARISTRASZ))
        pBelgaristrasz->GetMotionMaster()->MovePoint(0, m_sPrisonersMoveLocs[2].m_fX, m_sPrisonersMoveLocs[2].m_fY, m_sPrisonersMoveLocs[2].m_fZ);
}

void instance_oculus::DoSummonNextBossIfCan(bool bByPlayerEnter)
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    if (GetData(TYPE_UROM) == DONE)
    {
        // return if already summoned
        if (GetSingleCreatureFromStorage(NPC_EREGOS))
            return;

        if (bByPlayerEnter)
            pPlayer->SummonCreature(NPC_EREGOS, m_sOculusBossSpawnLocs[1].m_fX, m_sOculusBossSpawnLocs[1].m_fY, m_sOculusBossSpawnLocs[1].m_fZ, m_sOculusBossSpawnLocs[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*MINUTE*IN_MILLISECONDS);
        else
        {
            if (Creature* pEregos = pPlayer->SummonCreature(NPC_EREGOS, m_sOculusBossSpawnLocs[1].m_fX, m_sOculusBossSpawnLocs[1].m_fY, m_sOculusBossSpawnLocs[1].m_fZ, m_sOculusBossSpawnLocs[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*MINUTE*IN_MILLISECONDS))
                DoScriptText(SAY_EREGOS_SPAWN, pEregos);
        }
    }
    else if (GetData(TYPE_VAROS) == DONE)
    {
        // return if already summoned
        if (GetSingleCreatureFromStorage(NPC_UROM))
            return;

        pPlayer->SummonCreature(NPC_UROM, m_sOculusBossSpawnLocs[0].m_fX, m_sOculusBossSpawnLocs[0].m_fY, m_sOculusBossSpawnLocs[0].m_fZ, m_sOculusBossSpawnLocs[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*MINUTE*IN_MILLISECONDS);
   }
}

void instance_oculus::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DRAKOS: SetData(TYPE_DRAKOS, IN_PROGRESS); break;
        case NPC_VAROS: SetData(TYPE_VAROS, IN_PROGRESS); break;
    }
}

void instance_oculus::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DRAKOS: SetData(TYPE_DRAKOS, FAIL); break;
        case NPC_VAROS: SetData(TYPE_VAROS, FAIL); break;
    }
}

void instance_oculus::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DRAKOS: SetData(TYPE_DRAKOS, DONE); break;
        case NPC_VAROS: SetData(TYPE_VAROS, DONE); break;
        case NPC_CONSTRUCT:
            if (m_sConstructsGUIDs.find(pCreature->GetObjectGuid()) != m_sConstructsGUIDs.end())
            {
                m_sConstructsGUIDs.erase(pCreature->GetObjectGuid());

                // update world state
                DoUpdateWorldState(WORLD_STATE_CON_COUNT, m_sConstructsGUIDs.size());

                // check if event complete
                if (m_sConstructsGUIDs.empty())
                    SetData(TYPE_CONSTRUCTS, DONE);
            }
            break;
        case NPC_AZURE_CAPTAIN:
        case NPC_AZURE_GUARDIAN:
            if (m_sCaptainsGUIDs.find(pCreature->GetObjectGuid()) != m_sCaptainsGUIDs.end())
                m_sCaptainsGUIDs.erase(pCreature->GetObjectGuid());
            break;
    }
}

// WARNING!!! Event is not blizzlike
// The event should spawn a azure ring captain which should cast a blue laser bean on the platform
// This is a temporary workaround
void instance_oculus::DoCallAzureRingCaptain()
{
    if (m_sCaptainsGUIDs.empty())
        return;

    // get random azure captain to attack Varos' victim
    if (Creature* pVaros = GetSingleCreatureFromStorage(NPC_VAROS))
    {
        if (pVaros->getVictim())
        {
            GUIDSet::iterator iter = m_sCaptainsGUIDs.begin();
            advance(iter, urand(0, m_sCaptainsGUIDs.size()-1));

            if (Creature* pCaptain = instance->GetCreature(*iter))
                pCaptain->AI()->AttackStart(pVaros->getVictim());
        }
    }
}

InstanceData* GetInstanceData_instance_oculus(Map* pMap)
{
    return new instance_oculus(pMap);
}

bool ProcessEventId_event_call_ring_captain(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (instance_oculus* pInstance = (instance_oculus*)((Creature*)pSource)->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_VAROS) != IN_PROGRESS)
            return false;

        pInstance->DoCallAzureRingCaptain();
        return true;
    }
    return false;
}

void AddSC_instance_oculus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_oculus";
    pNewScript->GetInstanceData = &GetInstanceData_instance_oculus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_call_ring_captain";
    pNewScript->pProcessEventId = &ProcessEventId_event_call_ring_captain;
    pNewScript->RegisterSelf();
}
