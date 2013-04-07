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
SDName: instance_ulduar
SD%Complete:
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

struct sSpawnLocation
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static sSpawnLocation m_aKeepersSpawnLocs[] =
{
    {2036.892f, 25.621f, 411.358f, 3.83f},                  // Freya
    {1939.215f, 42.677f, 411.355f, 5.31f},                  // Mimiron
    {1939.195f, -90.662f, 411.357f, 1.06f},                 // Hodir
    {2036.674f, -73.814f, 411.355f, 2.51f},                 // Thorim
};

instance_ulduar::instance_ulduar(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_ulduar::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiHardBoss, 0, sizeof(m_auiHardBoss));
    memset(&m_auiUlduarKeepers, 0, sizeof(m_auiUlduarKeepers));

    for (uint8 i = 0; i < MAX_SPECIAL_ACHIEV_CRITS; ++i)
        m_abAchievCriteria[i] = false;
}

bool instance_ulduar::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }
    return false;
}

void instance_ulduar::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_LEVIATHAN:
        case NPC_IGNIS:
        case NPC_RAZORSCALE:
        case NPC_COMMANDER:
        case NPC_XT002:
        case NPC_STEELBREAKER:
        case NPC_MOLGEIM:
        case NPC_BRUNDIR:
        case NPC_KOLOGARN:
        case NPC_RIGHT_ARM:
        case NPC_LEFT_ARM:
        case NPC_AURIAYA:
        case NPC_FERAL_DEFENDER:
        case NPC_LEVIATHAN_MK:
        case NPC_RUNIC_COLOSSUS:
        case NPC_RUNE_GIANT:
        case NPC_JORMUNGAR_BEHEMOTH:
        case NPC_ELDER_BRIGHTLEAF:
        case NPC_ELDER_IRONBRACH:
        case NPC_ELDER_STONEBARK:
        case NPC_VEZAX:
        case NPC_SARONITE_ANIMUS:
        case NPC_YOGGSARON:
        case NPC_SARA:
        case NPC_YOGG_BRAIN:
        case NPC_ALGALON:
            break;

        case NPC_MIMIRON:
            if (m_auiEncounter[TYPE_MIMIRON] == DONE)
                SpawnFriendlyKeeper(NPC_MIMIRON_IMAGE);
            break;
        case NPC_HODIR:
            if (m_auiEncounter[TYPE_HODIR] == DONE)
                SpawnFriendlyKeeper(NPC_HODIR_IMAGE);
            break;
        case NPC_THORIM:
            if (m_auiEncounter[TYPE_THORIM] == DONE)
                SpawnFriendlyKeeper(NPC_THORIM_IMAGE);
            break;
        case NPC_FREYA:
            if (m_auiEncounter[TYPE_FREYA] == DONE)
                SpawnFriendlyKeeper(NPC_FREYA_IMAGE);
            break;

        default:
            return;
    }
    m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
}

void instance_ulduar::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
            // -----------------    Doors & Other   -----------------
            // The siege
        case GO_SHIELD_WALL:
            break;
        case GO_LEVIATHAN_GATE:
            if (m_auiEncounter[TYPE_LEVIATHAN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_XT002_GATE:
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[TYPE_XT002] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            if (m_auiEncounter[TYPE_LEVIATHAN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_BROKEN_HARPOON:
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;

            // Archivum
        case GO_IRON_ENTRANCE_DOOR:
            break;
        case GO_ARCHIVUM_DOOR:
            if (m_auiEncounter[TYPE_ASSEMBLY])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARCHIVUM_CONSOLE:
        case GO_UNIVERSE_FLOOR_ARCHIVUM:
            // Celestial Planetarium
        case GO_CELESTIAL_ACCES:
        case GO_CELESTIAL_DOOR:
        case GO_UNIVERSE_FLOOR_CELESTIAL:
        case GO_AZEROTH_GLOBE:
            break;
            // Shattered Hallway
        case GO_KOLOGARN_BRIDGE:
            pGo->SetGoState(GO_STATE_ACTIVE);
            if (m_auiEncounter[TYPE_KOLOGARN] == DONE)
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_SHATTERED_DOOR:
            break;

            // -----------------    The Keepers    -----------------
            // Hodir
        case GO_HODIR_EXIT:
            if (m_auiEncounter[TYPE_HODIR])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ICE_WALL:
            if (m_auiEncounter[TYPE_HODIR])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ENTER:
            break;
            // Mimiron
        case G0_MIMIRON_BUTTON:
            if (m_auiEncounter[TYPE_MIMIRON] == NOT_STARTED)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_MIMIRON_DOOR_1:
        case GO_MIMIRON_DOOR_2:
        case GO_MIMIRON_DOOR_3:
        case GO_MIMIRON_ELEVATOR:
        case GO_MIMIRON_TEL1:
        case GO_MIMIRON_TEL2:
        case GO_MIMIRON_TEL3:
        case GO_MIMIRON_TEL4:
        case GO_MIMIRON_TEL5:
        case GO_MIMIRON_TEL6:
        case GO_MIMIRON_TEL7:
        case GO_MIMIRON_TEL8:
        case GO_MIMIRON_TEL9:
            // Thorim
        case GO_DARK_IRON_PORTCULIS:
        case GO_RUNED_STONE_DOOR:
        case GO_THORIM_STONE_DOOR:
        case GO_LIGHTNING_FIELD:
            break;
        case GO_DOOR_LEVER:
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;

            // Prison
        case GO_ANCIENT_GATE:
            DoOpenMadnessDoorIfCan();
            break;
        case GO_VEZAX_GATE:
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[TYPE_VEZAX])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_YOGG_GATE:
        case GO_BRAIN_DOOR1:
        case GO_BRAIN_DOOR2:
        case GO_BRAIN_DOOR3:
            break;

            // -----------------    Chests    -----------------
            // Kologarn
        case GO_CACHE_OF_LIVING_STONE_10:
        case GO_CACHE_OF_LIVING_STONE_25:

            // Hodir
        case GO_CACHE_OF_WINTER_10:
        case GO_CACHE_OF_WINTER_25:
        case GO_CACHE_OF_RARE_WINTER_10:
        case GO_CACHE_OF_RARE_WINTER_25:

            // Thorim
        case GO_CACHE_OF_STORMS_10:
        case GO_CACHE_OF_STORMS_25:
        case GO_CACHE_OF_STORMS_10_H:
        case GO_CACHE_OF_STORMS_25_H:

            // Mimiron
        case GO_CACHE_OF_INOV_10:
        case GO_CACHE_OF_INOV_25:
        case GO_CACHE_OF_INOV_10_H:
        case GO_CACHE_OF_INOV_25_H:

            // Alagon
        case GO_GIFT_OF_OBSERVER_10:
        case GO_GIFT_OF_OBSERVER_25:
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

// Used in order to unlock the door to Vezax
void instance_ulduar::DoOpenMadnessDoorIfCan()
{
    if (m_auiEncounter[TYPE_MIMIRON] == DONE && m_auiEncounter[TYPE_HODIR] == DONE && m_auiEncounter[TYPE_THORIM] == DONE && m_auiEncounter[TYPE_FREYA] == DONE)
    {
        if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_ANCIENT_GATE))
            pDoor->SetGoState(GO_STATE_ACTIVE);
    }
}

void instance_ulduar::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_LEVIATHAN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_SHIELD_WALL);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_XT002_GATE);
                DoUseDoorOrButton(GO_LIGHTNING_FIELD);
            }
            break;
        case TYPE_IGNIS:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_RAZORSCALE:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_XT002:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_XT002_GATE);
            break;
        case TYPE_ASSEMBLY:
            // Don't set the same encounter data twice
            if (uiData == m_auiEncounter[uiType])
                return;
            m_auiEncounter[uiType] = uiData;
            // don't continue for encounter = special
            if (uiData == SPECIAL)
                return;
            DoUseDoorOrButton(GO_IRON_ENTRANCE_DOOR);
            if (uiData == DONE)
                DoUseDoorOrButton(GO_ARCHIVUM_DOOR);
            else if (uiData == IN_PROGRESS)
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEV_BRUNDIR, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_MOLGEIM, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_STEELBREAKER, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_STUNNED, true);
            }
            break;
        case TYPE_KOLOGARN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_SHATTERED_DOOR);
            if (uiData == DONE)
            {
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_LIVING_STONE_10 : GO_CACHE_OF_LIVING_STONE_25, 30 * MINUTE);
                if (GameObject* pBridge = GetSingleGameObjectFromStorage(GO_KOLOGARN_BRIDGE))
                    pBridge->SetGoState(GO_STATE_READY);
            }
            break;
        case TYPE_AURIAYA:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEV_CAT_LADY, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_NINE_LIVES, false);
            }
            break;
            // Keepers
        case TYPE_MIMIRON:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_MIMIRON_DOOR_1);
            DoUseDoorOrButton(GO_MIMIRON_DOOR_2);
            DoUseDoorOrButton(GO_MIMIRON_DOOR_3);
            if (uiData == DONE)
            {
                if (GetData(TYPE_MIMIRON_HARD) != DONE)
                    DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_INOV_10 : GO_CACHE_OF_INOV_25, 30 * MINUTE);
                SpawnFriendlyKeeper(NPC_MIMIRON_IMAGE);
            }
            break;
        case TYPE_HODIR:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_HODIR_ENTER);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_HODIR_ICE_WALL);
                DoUseDoorOrButton(GO_HODIR_EXIT);
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_WINTER_10 : GO_CACHE_OF_WINTER_25, 30 * MINUTE);
                SpawnFriendlyKeeper(NPC_HODIR_IMAGE);
            }
            break;
        case TYPE_THORIM:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_LIGHTNING_FIELD);
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_DARK_IRON_PORTCULIS);
            if (uiData == DONE)
            {
                if (GetData(TYPE_THORIM_HARD) != DONE)
                    DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_STORMS_10 : GO_CACHE_OF_STORMS_25, 30 * MINUTE);
                SpawnFriendlyKeeper(NPC_THORIM_IMAGE);
            }
            break;
        case TYPE_FREYA:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                SpawnFriendlyKeeper(NPC_FREYA_IMAGE);
            break;
            // Prison
        case TYPE_VEZAX:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(GO_VEZAX_GATE);
            break;
        case TYPE_YOGGSARON:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_YOGG_GATE);
            break;

            // Celestial Planetarium
        case TYPE_ALGALON:
            m_auiEncounter[uiType] = uiData;
            // TODO: need to find the proper way to use these
            DoUseDoorOrButton(GO_CELESTIAL_DOOR);
            DoUseDoorOrButton(GO_UNIVERSE_FLOOR_CELESTIAL);
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_GIFT_OF_OBSERVER_10 : GO_GIFT_OF_OBSERVER_25, 30 * MINUTE);
            break;

            // Hard modes
        case TYPE_LEVIATHAN_HARD:
            m_auiHardBoss[0] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_XT002_HARD:
            m_auiHardBoss[1] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_HODIR_HARD:
            m_auiHardBoss[2] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_RARE_WINTER_10 : GO_CACHE_OF_RARE_WINTER_25, 30 * MINUTE);
            break;
        case TYPE_THORIM_HARD:
            m_auiHardBoss[3] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_STORMS_10_H : GO_CACHE_OF_STORMS_25_H, 30 * MINUTE);
            break;
        case TYPE_MIMIRON_HARD:
            m_auiHardBoss[4] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_INOV_10_H : GO_CACHE_OF_INOV_25_H, 30 * MINUTE);
            break;
        case TYPE_VEZAX_HARD:
            m_auiHardBoss[5] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_YOGGSARON_HARD:
            m_auiHardBoss[6] = uiData;                      // TODO: add extra loot
            break;

            // Ulduar keepers
        case TYPE_KEEPER_HODIR:
            m_auiUlduarKeepers[0] = uiData;
            break;
        case TYPE_KEEPER_THORIM:
            m_auiUlduarKeepers[1] = uiData;
            break;
        case TYPE_KEEPER_FREYA:
            m_auiUlduarKeepers[2] = uiData;
            break;
        case TYPE_KEEPER_MIMIRON:
            m_auiUlduarKeepers[3] = uiData;
            break;

            // mini boss
        case TYPE_RUNIC_COLOSSUS:
            m_auiMiniBoss[0] = uiData;
            if (uiData == DONE)
            {
                // Open
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_RUNED_STONE_DOOR))
                    if (pGo->getLootState() == GO_READY)
                        pGo->UseDoorOrButton();
            }
            else
            {
                // Close
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_RUNED_STONE_DOOR))
                    if (pGo->getLootState() == GO_ACTIVATED)
                        pGo->ResetDoorOrButton();
            }
            break;
        case TYPE_RUNE_GIANT:
            m_auiMiniBoss[1] = uiData;
            if (uiData == DONE)
            {
                // Open
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_THORIM_STONE_DOOR))
                    if (pGo->getLootState() == GO_READY)
                        pGo->UseDoorOrButton();
            }
            else
            {
                // Close
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_THORIM_STONE_DOOR))
                    if (pGo->getLootState() == GO_ACTIVATED)
                        pGo->ResetDoorOrButton();
            }
            break;
        case TYPE_LEVIATHAN_MK:
            m_auiMiniBoss[2] = uiData;
            break;
        case TYPE_VX001:
            m_auiMiniBoss[3] = uiData;
            //if (uiData == DONE)     // just for animation :)
            //{
            //    for(uint8 i = 0; i < 9; i++)
            //        DoUseDoorOrButton(m_uiMimironTelGUID[i]);
            //}
            break;
        case TYPE_AERIAL_UNIT:
            m_auiMiniBoss[4] = uiData;
            break;
        case TYPE_YOGG_BRAIN:
            m_auiMiniBoss[5] = uiData;
            break;

            //phases
        case TYPE_MIMIRON_PHASE:
            m_uiMimironPhase = uiData;
            break;
        case TYPE_YOGG_PHASE:
            m_uiYoggPhase = uiData;
            break;
        case TYPE_VISION_PHASE:
            m_uiVisionPhase = uiData;
            break;
    }

    DoOpenMadnessDoorIfCan();

    if (uiData == DONE || uiData == FAIL)
    {
        OUT_SAVE_INST_DATA;

        // Save all encounters, hard bosses, keepers and teleporters
        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                   << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                   << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
                   << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
                   << m_auiEncounter[12] << " " << m_auiEncounter[13] << " " << m_auiHardBoss[0] << " "
                   << m_auiHardBoss[1] << " " << m_auiHardBoss[2] << " " << m_auiHardBoss[2] << " "
                   << m_auiHardBoss[4] << " " << m_auiHardBoss[5] << " " << m_auiHardBoss[6] << " "
                   << m_auiUlduarKeepers[0] << " " << m_auiUlduarKeepers[1] << " " << m_auiUlduarKeepers[2] << " " << m_auiUlduarKeepers[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

// TODO: implement all hard mode loot here!
bool instance_ulduar::CheckConditionCriteriaMeet(Player const* pPlayer, uint32 uiInstanceConditionId, WorldObject const* pConditionSource, uint32 conditionSourceType) const
{
    switch (uiInstanceConditionId)
    {
        case TYPE_XT002_HARD:
            break;
    }

    script_error_log("instance_ulduar::CheckConditionCriteriaMeet called with unsupported Id %u. Called with param plr %s, src %s, condition source type %u",
                     uiInstanceConditionId, pPlayer ? pPlayer->GetGuidStr().c_str() : "NULL", pConditionSource ? pConditionSource->GetGuidStr().c_str() : "NULL", conditionSourceType);
    return false;
}

uint32 instance_ulduar::GetData(uint32 uiType) const
{
    switch (uiType)
    {
        case TYPE_LEVIATHAN:
            return m_auiEncounter[0];
        case TYPE_IGNIS:
            return m_auiEncounter[1];
        case TYPE_RAZORSCALE:
            return m_auiEncounter[2];
        case TYPE_XT002:
            return m_auiEncounter[3];
        case TYPE_ASSEMBLY:
            return m_auiEncounter[4];
        case TYPE_KOLOGARN:
            return m_auiEncounter[5];
        case TYPE_AURIAYA:
            return m_auiEncounter[6];
        case TYPE_MIMIRON:
            return m_auiEncounter[7];
        case TYPE_HODIR:
            return m_auiEncounter[8];
        case TYPE_THORIM:
            return m_auiEncounter[9];
        case TYPE_FREYA:
            return m_auiEncounter[10];
        case TYPE_VEZAX:
            return m_auiEncounter[11];
        case TYPE_YOGGSARON:
            return m_auiEncounter[12];
        case TYPE_ALGALON:
            return m_auiEncounter[13];

            // Hard modes
        case TYPE_LEVIATHAN_HARD:
            return m_auiHardBoss[0];
        case TYPE_XT002_HARD:
            return m_auiHardBoss[1];
        case TYPE_HODIR_HARD:
            return m_auiHardBoss[2];
        case TYPE_THORIM_HARD:
            return m_auiHardBoss[3];
        case TYPE_MIMIRON_HARD:
            return m_auiHardBoss[4];
        case TYPE_VEZAX_HARD:
            return m_auiHardBoss[5];
        case TYPE_YOGGSARON_HARD:
            return m_auiHardBoss[6];

            // Ulduar Keepers
        case TYPE_KEEPER_HODIR:
            return m_auiUlduarKeepers[0];
        case TYPE_KEEPER_THORIM:
            return m_auiUlduarKeepers[1];
        case TYPE_KEEPER_FREYA:
            return m_auiUlduarKeepers[2];
        case TYPE_KEEPER_MIMIRON:
            return m_auiUlduarKeepers[3];

            // mini boss
        case TYPE_RUNE_GIANT:
            return m_auiMiniBoss[1];
        case TYPE_RUNIC_COLOSSUS:
            return m_auiMiniBoss[0];
        case TYPE_LEVIATHAN_MK:
            return m_auiMiniBoss[2];
        case TYPE_VX001:
            return m_auiMiniBoss[3];
        case TYPE_AERIAL_UNIT:
            return m_auiMiniBoss[4];
        case TYPE_YOGG_BRAIN:
            return m_auiMiniBoss[5];

        case TYPE_MIMIRON_PHASE:
            return m_uiMimironPhase;
        case TYPE_YOGG_PHASE:
            return m_uiYoggPhase;
        case TYPE_VISION_PHASE:
            return m_uiVisionPhase;
    }

    return 0;
}

// Spawn the friendly keepers in the central chamber
void instance_ulduar::SpawnFriendlyKeeper(uint32 uiWho)
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    switch (uiWho)
    {
        case NPC_MIMIRON_IMAGE: pPlayer->SummonCreature(NPC_MIMIRON_IMAGE, m_aKeepersSpawnLocs[1].m_fX, m_aKeepersSpawnLocs[1].m_fY, m_aKeepersSpawnLocs[1].m_fZ, m_aKeepersSpawnLocs[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
        case NPC_HODIR_IMAGE:   pPlayer->SummonCreature(NPC_HODIR_IMAGE,   m_aKeepersSpawnLocs[2].m_fX, m_aKeepersSpawnLocs[2].m_fY, m_aKeepersSpawnLocs[2].m_fZ, m_aKeepersSpawnLocs[2].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
        case NPC_THORIM_IMAGE:  pPlayer->SummonCreature(NPC_THORIM_IMAGE,  m_aKeepersSpawnLocs[3].m_fX, m_aKeepersSpawnLocs[3].m_fY, m_aKeepersSpawnLocs[3].m_fZ, m_aKeepersSpawnLocs[3].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
        case NPC_FREYA_IMAGE:   pPlayer->SummonCreature(NPC_FREYA_IMAGE,   m_aKeepersSpawnLocs[0].m_fX, m_aKeepersSpawnLocs[0].m_fY, m_aKeepersSpawnLocs[0].m_fZ, m_aKeepersSpawnLocs[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
    }
}

void instance_ulduar::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_SANCTUM_SENTRY:
            if (GetData(TYPE_AURIAYA) == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_CAT_LADY, false);
            break;
    }
}

void instance_ulduar::Load(const char* strIn)
{
    if (!strIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(strIn);

    std::istringstream loadStream(strIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
               >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8]
               >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11] >> m_auiEncounter[12] >> m_auiEncounter[13]
               >> m_auiHardBoss[0] >> m_auiHardBoss[1] >> m_auiHardBoss[2] >> m_auiHardBoss[3] >> m_auiHardBoss[4] >> m_auiHardBoss[5] >> m_auiHardBoss[6]
               >> m_auiUlduarKeepers[0] >> m_auiUlduarKeepers[1] >> m_auiUlduarKeepers[2] >> m_auiUlduarKeepers[3];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_ulduar::SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet)
{
    if (uiType < MAX_SPECIAL_ACHIEV_CRITS)
        m_abAchievCriteria[uiType] = bIsMet;
}

bool instance_ulduar::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* /*pSource*/, Unit const* /*pTarget*/, uint32 /*uiMiscValue1 = 0*/) const
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_SARONITE_N:
        case ACHIEV_CRIT_SARONITE_H:
            return GetData(TYPE_VEZAX_HARD) == DONE;
        case ACHIEV_CRIT_CAT_LADY_N:
        case ACHIEV_CRIT_CAT_LADY_H:
            return m_abAchievCriteria[TYPE_ACHIEV_CAT_LADY];
        case ACHIEV_CRIT_NINE_LIVES_N:
        case ACHIEV_CRIT_NINE_LIVES_H:
            return m_abAchievCriteria[TYPE_ACHIEV_NINE_LIVES];
        case ACHIEV_CRIT_BRUNDIR_N:
        case ACHIEV_CRIT_BRUNDIR_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_BRUNDIR];
        case ACHIEV_CRIT_MOLGEIM_N:
        case ACHIEV_CRIT_MOLGEIM_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_MOLGEIM];
        case ACHIEV_CRIT_STEELBREAKER_N:
        case ACHIEV_CRIT_STEELBREAKER_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_STEELBREAKER];
        case ACHIEV_CRIT_STUNNED_BRUND_N:
        case ACHIEV_CRIT_STUNNED_STEEL_N:
        case ACHIEV_CRIT_STUNNED_MOLG_N:
        case ACHIEV_CRIT_STUNNED_BRUND_H:
        case ACHIEV_CRIT_STUNNED_STEEL_H:
        case ACHIEV_CRIT_STUNNED_MOLG_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_STUNNED];

        default:
            return false;
    }
}

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

void AddSC_instance_ulduar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_ulduar";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ulduar;
    pNewScript->RegisterSelf();
}
