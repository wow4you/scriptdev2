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
SDName: Ebon_Hold
SD%Complete: 80
SDComment: Quest support: 12848, 12733, 12739(and 12742 to 12750), 12727, 12641, 12801
SDCategory: Ebon Hold
EndScriptData */

/* ContentData
npc_a_special_surprise
npc_death_knight_initiate
npc_unworthy_initiate_anchor
npc_unworthy_initiate
go_acherus_soul_prison
npc_eye_of_acherus
npc_highlord_darion_mograine
EndContentData */

#include "precompiled.h"
#include "world_map_ebon_hold.h"
#include "escort_ai.h"

/*######
## npc_a_special_surprise
######*/

enum SpecialSurprise
{
    SAY_EXEC_START_1            = -1609025,                 // speech for all
    SAY_EXEC_START_2            = -1609026,
    SAY_EXEC_START_3            = -1609027,
    SAY_EXEC_PROG_1             = -1609028,
    SAY_EXEC_PROG_2             = -1609029,
    SAY_EXEC_PROG_3             = -1609030,
    SAY_EXEC_PROG_4             = -1609031,
    SAY_EXEC_PROG_5             = -1609032,
    SAY_EXEC_PROG_6             = -1609033,
    SAY_EXEC_PROG_7             = -1609034,
    SAY_EXEC_NAME_1             = -1609035,
    SAY_EXEC_NAME_2             = -1609036,
    SAY_EXEC_RECOG_1            = -1609037,
    SAY_EXEC_RECOG_2            = -1609038,
    SAY_EXEC_RECOG_3            = -1609039,
    SAY_EXEC_RECOG_4            = -1609040,
    SAY_EXEC_RECOG_5            = -1609041,
    SAY_EXEC_RECOG_6            = -1609042,
    SAY_EXEC_NOREM_1            = -1609043,
    SAY_EXEC_NOREM_2            = -1609044,
    SAY_EXEC_NOREM_3            = -1609045,
    SAY_EXEC_NOREM_4            = -1609046,
    SAY_EXEC_NOREM_5            = -1609047,
    SAY_EXEC_NOREM_6            = -1609048,
    SAY_EXEC_NOREM_7            = -1609049,
    SAY_EXEC_NOREM_8            = -1609050,
    SAY_EXEC_NOREM_9            = -1609051,
    SAY_EXEC_THINK_1            = -1609052,
    SAY_EXEC_THINK_2            = -1609053,
    SAY_EXEC_THINK_3            = -1609054,
    SAY_EXEC_THINK_4            = -1609055,
    SAY_EXEC_THINK_5            = -1609056,
    SAY_EXEC_THINK_6            = -1609057,
    SAY_EXEC_THINK_7            = -1609058,
    SAY_EXEC_THINK_8            = -1609059,
    SAY_EXEC_THINK_9            = -1609060,
    SAY_EXEC_THINK_10           = -1609061,
    SAY_EXEC_LISTEN_1           = -1609062,
    SAY_EXEC_LISTEN_2           = -1609063,
    SAY_EXEC_LISTEN_3           = -1609064,
    SAY_EXEC_LISTEN_4           = -1609065,
    SAY_PLAGUEFIST              = -1609066,
    SAY_EXEC_TIME_1             = -1609067,
    SAY_EXEC_TIME_2             = -1609068,
    SAY_EXEC_TIME_3             = -1609069,
    SAY_EXEC_TIME_4             = -1609070,
    SAY_EXEC_TIME_5             = -1609071,
    SAY_EXEC_TIME_6             = -1609072,
    SAY_EXEC_TIME_7             = -1609073,
    SAY_EXEC_TIME_8             = -1609074,
    SAY_EXEC_TIME_9             = -1609075,
    SAY_EXEC_TIME_10            = -1609076,
    SAY_EXEC_WAITING            = -1609077,
    EMOTE_DIES                  = -1609078,

    NPC_PLAGUEFIST              = 29053
};

struct MANGOS_DLL_DECL npc_a_special_surpriseAI : public ScriptedAI
{
    npc_a_special_surpriseAI(Creature *pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiExecuteSpeech_Timer;
    uint32 m_uiExecuteSpeech_Counter;
    ObjectGuid m_playerGuid;

    void Reset()
    {
        m_uiExecuteSpeech_Timer = 0;
        m_uiExecuteSpeech_Counter = 0;
        m_playerGuid.Clear();
    }

    bool MeetQuestCondition(Player* pPlayer)
    {
        switch(m_creature->GetEntry())
        {
            case 29061:                                     // Ellen Stanbridge
                if (pPlayer->GetQuestStatus(12742) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29072:                                     // Kug Ironjaw
                if (pPlayer->GetQuestStatus(12748) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29067:                                     // Donovan Pulfrost
                if (pPlayer->GetQuestStatus(12744) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29065:                                     // Yazmina Oakenthorn
                if (pPlayer->GetQuestStatus(12743) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29071:                                     // Antoine Brack
                if (pPlayer->GetQuestStatus(12750) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29032:                                     // Malar Bravehorn
                if (pPlayer->GetQuestStatus(12739) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29068:                                     // Goby Blastenheimer
                if (pPlayer->GetQuestStatus(12745) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29073:                                     // Iggy Darktusk
                if (pPlayer->GetQuestStatus(12749) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29074:                                     // Lady Eonys
                if (pPlayer->GetQuestStatus(12747) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
            case 29070:                                     // Valok the Righteous
                if (pPlayer->GetQuestStatus(12746) == QUEST_STATUS_INCOMPLETE)
                    return true;
                break;
        }

        return false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_playerGuid || pWho->GetTypeId() != TYPEID_PLAYER || !pWho->IsWithinDist(m_creature, INTERACTION_DISTANCE))
            return;

        if (MeetQuestCondition((Player*)pWho))
            m_playerGuid = pWho->GetObjectGuid();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_playerGuid && !m_creature->getVictim() && m_creature->isAlive())
        {
            if (m_uiExecuteSpeech_Timer < uiDiff)
            {
                Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid);

                if (!pPlayer)
                {
                    Reset();
                    return;
                }

                //TODO: simplify text's selection

                switch(pPlayer->getRace())
                {
                    case RACE_HUMAN:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_5, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_1, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_5, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_7, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_1, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_6, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_ORC:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_6, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_1, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_7, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_8, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_1, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_8, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_DWARF:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_2, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_2, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_3, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_2, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_5, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_2, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_3, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_NIGHTELF:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_5, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_1, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_6, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_2, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_1, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_7, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_UNDEAD:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_3, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_4, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_3, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_1, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_3, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_4, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_TAUREN:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_1, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_5, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_8, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_9, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_1, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_9, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_GNOME:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_4, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_1, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_4, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_6, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_1, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_5, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_TROLL:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_3, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_7, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_2, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_6, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_9, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_10, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_4, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_10, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_BLOODELF:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_1, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_1, m_creature, pPlayer); break;
                            //case 5: //unknown
                            case 6: DoScriptText(SAY_EXEC_THINK_3, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_1, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_1, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                    case RACE_DRAENEI:
                        switch(m_uiExecuteSpeech_Counter)
                        {
                            case 0: DoScriptText(SAY_EXEC_START_1, m_creature, pPlayer); break;
                            case 1: m_creature->SetStandState(UNIT_STAND_STATE_STAND); break;
                            case 2: DoScriptText(SAY_EXEC_PROG_1, m_creature, pPlayer); break;
                            case 3: DoScriptText(SAY_EXEC_NAME_1, m_creature, pPlayer); break;
                            case 4: DoScriptText(SAY_EXEC_RECOG_2, m_creature, pPlayer); break;
                            case 5: DoScriptText(SAY_EXEC_NOREM_1, m_creature, pPlayer); break;
                            case 6: DoScriptText(SAY_EXEC_THINK_4, m_creature, pPlayer); break;
                            case 7: DoScriptText(SAY_EXEC_LISTEN_1, m_creature, pPlayer); break;
                            case 8:
                                if (Creature* pPlaguefist = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUEFIST, 85.0f))
                                    DoScriptText(SAY_PLAGUEFIST, pPlaguefist, pPlayer);
                                break;
                            case 9:
                                DoScriptText(SAY_EXEC_TIME_2, m_creature, pPlayer);
                                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                break;
                            case 10: DoScriptText(SAY_EXEC_WAITING, m_creature, pPlayer); break;
                            case 11:
                                DoScriptText(EMOTE_DIES, m_creature);
                                m_creature->SetDeathState(JUST_DIED);
                                m_creature->SetHealth(0);
                                return;
                        }
                        break;
                }

                if (m_uiExecuteSpeech_Counter >= 9)
                    m_uiExecuteSpeech_Timer = 15000;
                else
                    m_uiExecuteSpeech_Timer = 7000;

                ++m_uiExecuteSpeech_Counter;
            }
            else
                m_uiExecuteSpeech_Timer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_a_special_surprise(Creature* pCreature)
{
    return new npc_a_special_surpriseAI(pCreature);
}

/*######
## npc_death_knight_initiate
######*/

enum
{
    SAY_DUEL_A                  = -1609016,
    SAY_DUEL_B                  = -1609017,
    SAY_DUEL_C                  = -1609018,
    SAY_DUEL_D                  = -1609019,
    SAY_DUEL_E                  = -1609020,
    SAY_DUEL_F                  = -1609021,
    SAY_DUEL_G                  = -1609022,
    SAY_DUEL_H                  = -1609023,
    SAY_DUEL_I                  = -1609024,

    SPELL_DUEL                  = 52996,
    SPELL_DUEL_TRIGGERED        = 52990,
    SPELL_DUEL_VICTORY          = 52994,
    SPELL_DUEL_FLAG             = 52991,

    QUEST_DEATH_CHALLENGE       = 12733,
    FACTION_HOSTILE             = 2068
};

int32 m_auiRandomSay[] =
{
    SAY_DUEL_A, SAY_DUEL_B, SAY_DUEL_C, SAY_DUEL_D, SAY_DUEL_E, SAY_DUEL_F, SAY_DUEL_G, SAY_DUEL_H, SAY_DUEL_I
};

#define GOSSIP_ACCEPT_DUEL      "I challenge you, death knight!"

struct MANGOS_DLL_DECL npc_death_knight_initiateAI : public ScriptedAI
{
    npc_death_knight_initiateAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    ObjectGuid m_duelerGuid;
    uint32 m_uiDuelTimer;
    bool m_bIsDuelInProgress;

    void Reset()
    {
        if (m_creature->getFaction() != m_creature->GetCreatureInfo()->faction_A)
            m_creature->setFaction(m_creature->GetCreatureInfo()->faction_A);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

        m_duelerGuid.Clear();
        m_uiDuelTimer = 5000;
        m_bIsDuelInProgress = false;
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (m_creature->getVictim())
            return;

        if (m_creature->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (!m_bIsDuelInProgress && pSpell->Id == SPELL_DUEL_TRIGGERED && pCaster->GetTypeId() == TYPEID_PLAYER)
        {
            m_duelerGuid = pCaster->GetObjectGuid();
            m_bIsDuelInProgress = true;
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_bIsDuelInProgress && uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;

            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_duelerGuid))
                m_creature->CastSpell(pPlayer, SPELL_DUEL_VICTORY, true);

            //possibly not evade, but instead have end sequenze
            EnterEvadeMode();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (m_bIsDuelInProgress)
            {
                if (m_uiDuelTimer < uiDiff)
                {
                    m_creature->setFaction(FACTION_HOSTILE);

                    if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_duelerGuid))
                        AttackStart(pPlayer);
                }
                else
                    m_uiDuelTimer -= uiDiff;
            }
            return;
        }

        // TODO: spells

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_death_knight_initiate(Creature* pCreature)
{
    return new npc_death_knight_initiateAI(pCreature);
}

bool GossipHello_npc_death_knight_initiate(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_DEATH_CHALLENGE) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ACCEPT_DUEL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU(13433, pCreature->GetObjectGuid());
        return true;
    }
    return false;
}

bool GossipSelect_npc_death_knight_initiate(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        if (npc_death_knight_initiateAI* pInitiateAI = dynamic_cast<npc_death_knight_initiateAI*>(pCreature->AI()))
        {
            if (pInitiateAI->m_bIsDuelInProgress)
                return true;
        }

        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

        int32 uiSayId = rand()% (sizeof(m_auiRandomSay)/sizeof(int32));
        DoScriptText(m_auiRandomSay[uiSayId], pCreature, pPlayer);

        pCreature->CastSpell(pPlayer, SPELL_DUEL, false);
        pCreature->CastSpell(pPlayer, SPELL_DUEL_FLAG, true);
    }
    return true;
}

/*######
## npc_koltira_deathweaver
######*/

enum eKoltira
{
    SAY_BREAKOUT1                   = -1609079,
    SAY_BREAKOUT2                   = -1609080,
    SAY_BREAKOUT3                   = -1609081,
    SAY_BREAKOUT4                   = -1609082,
    SAY_BREAKOUT5                   = -1609083,
    SAY_BREAKOUT6                   = -1609084,
    SAY_BREAKOUT7                   = -1609085,
    SAY_BREAKOUT8                   = -1609086,
    SAY_BREAKOUT9                   = -1609087,
    SAY_BREAKOUT10                  = -1609088,

    SPELL_KOLTIRA_TRANSFORM         = 52899,
    SPELL_ANTI_MAGIC_ZONE           = 52894,

    QUEST_BREAKOUT                  = 12727,

    NPC_CRIMSON_ACOLYTE             = 29007,
    NPC_HIGH_INQUISITOR_VALROTH     = 29001,
    NPC_KOLTIRA_ALT                 = 28447,

    //not sure about this id
    //NPC_DEATH_KNIGHT_MOUNT          = 29201,
    MODEL_DEATH_KNIGHT_MOUNT        = 25278
};

struct MANGOS_DLL_DECL npc_koltira_deathweaverAI : public npc_escortAI
{
    npc_koltira_deathweaverAI(Creature *pCreature) : npc_escortAI(pCreature) { Reset(); }

    uint32 m_uiWave;
    uint32 m_uiWave_Timer;
    ObjectGuid m_valrothGuid;

    void Reset()
    {
        if (!HasEscortState(STATE_ESCORT_ESCORTING))
        {
            m_uiWave = 0;
            m_uiWave_Timer = 3000;
            m_valrothGuid.Clear();
        }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 0:
                DoScriptText(SAY_BREAKOUT1, m_creature);
                break;
            case 1:
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                break;
            case 2:
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                //m_creature->UpdateEntry(NPC_KOLTIRA_ALT); //unclear if we must update or not
                DoCastSpellIfCan(m_creature, SPELL_KOLTIRA_TRANSFORM);
                break;
            case 3:
                SetEscortPaused(true);
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                DoScriptText(SAY_BREAKOUT2, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_ANTI_MAGIC_ZONE);  // cast again that makes bubble up
                break;
            case 4:
                SetRun(true);
                break;
            case 9:
                m_creature->Mount(MODEL_DEATH_KNIGHT_MOUNT);
                break;
            case 10:
                m_creature->Unmount();
                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Player* pPlayer = GetPlayerForEscort())
            pSummoned->AI()->AttackStart(pPlayer);

        if (pSummoned->GetEntry() == NPC_HIGH_INQUISITOR_VALROTH)
            m_valrothGuid = pSummoned->GetObjectGuid();
    }

    void SummonAcolyte(uint32 uiAmount)
    {
        for(uint32 i = 0; i < uiAmount; ++i)
            m_creature->SummonCreature(NPC_CRIMSON_ACOLYTE, 1642.329f, -6045.818f, 127.583f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (HasEscortState(STATE_ESCORT_PAUSED))
        {
            if (m_uiWave_Timer < uiDiff)
            {
                switch(m_uiWave)
                {
                    case 0:
                        DoScriptText(SAY_BREAKOUT3, m_creature);
                        SummonAcolyte(3);
                        m_uiWave_Timer = 20000;
                        break;
                    case 1:
                        DoScriptText(SAY_BREAKOUT4, m_creature);
                        SummonAcolyte(3);
                        m_uiWave_Timer = 20000;
                        break;
                    case 2:
                        DoScriptText(SAY_BREAKOUT5, m_creature);
                        SummonAcolyte(4);
                        m_uiWave_Timer = 20000;
                        break;
                    case 3:
                        DoScriptText(SAY_BREAKOUT6, m_creature);
                        m_creature->SummonCreature(NPC_HIGH_INQUISITOR_VALROTH, 1642.329f, -6045.818f, 127.583f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
                        m_uiWave_Timer = 1000;
                        break;
                    case 4:
                    {
                        Creature* pTemp = m_creature->GetMap()->GetCreature(m_valrothGuid);

                        if (!pTemp || !pTemp->isAlive())
                        {
                            DoScriptText(SAY_BREAKOUT8, m_creature);
                            m_uiWave_Timer = 5000;
                        }
                        else
                        {
                            m_uiWave_Timer = 2500;
                            return;                         //return, we don't want m_uiWave to increment now
                        }
                        break;
                    }
                    case 5:
                        DoScriptText(SAY_BREAKOUT9, m_creature);
                        m_creature->RemoveAurasDueToSpell(SPELL_ANTI_MAGIC_ZONE);
                        m_uiWave_Timer = 2500;
                        break;
                    case 6:
                        DoScriptText(SAY_BREAKOUT10, m_creature);
                        SetEscortPaused(false);
                        break;
                }

                ++m_uiWave;
            }
            else
                m_uiWave_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_koltira_deathweaver(Creature* pCreature)
{
    return new npc_koltira_deathweaverAI(pCreature);
}

bool QuestAccept_npc_koltira_deathweaver(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_BREAKOUT)
    {
        pCreature->SetStandState(UNIT_STAND_STATE_STAND);

        if (npc_koltira_deathweaverAI* pEscortAI = dynamic_cast<npc_koltira_deathweaverAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer, pQuest);
    }
    return true;
}

/*######
##
######*/

enum
{
    SAY_START                       = -1609000,             // 8 texts in total, GetTextId() generates random with this as base
    SAY_AGGRO                       = -1609008,             // 8 texts in total, GetTextId() generates random with this as base

    //SPELL_CHAINED_PESANT_LH         = 54602,              // not used. possible it determine side, where to go get "weapon"
    //SPELL_CHAINED_PESANT_RH         = 54610,
    SPELL_CHAINED_PESANT_CHEST      = 54612,
    SPELL_CHAINED_PESANT_BREATH     = 54613,
    SPELL_INITIATE_VISUAL           = 51519,

    SPELL_BLOOD_STRIKE              = 52374,
    SPELL_DEATH_COIL                = 52375,
    SPELL_ICY_TOUCH                 = 52372,
    SPELL_PLAGUE_STRIKE             = 52373,

    NPC_ANCHOR                      = 29521,
    FACTION_MONSTER                 = 16,

    PHASE_INACTIVE_OR_COMBAT        = 0,
    PHASE_DRESSUP                   = 1,
    PHASE_ACTIVATE                  = 2
};

struct DisplayToSpell
{
    uint32 m_uiDisplayId;
    uint32 m_uiSpellToNewDisplay;
};

DisplayToSpell m_aDisplayToSpell[] =
{
    {25354, 51520},                                         // human M
    {25355, 51534},                                         // human F
    {25356, 51538},                                         // dwarf M
    {25357, 51541},                                         // draenei M
    {25358, 51535},                                         // nelf M
    {25359, 51539},                                         // gnome M
    {25360, 51536},                                         // nelf F
    {25361, 51537},                                         // dwarf F
    {25362, 51540},                                         // gnome F
    {25363, 51542},                                         // draenei F
    {25364, 51543},                                         // orc M
    {25365, 51546},                                         // troll M
    {25366, 51547},                                         // tauren M
    {25367, 51549},                                         // forsaken M
    {25368, 51544},                                         // orc F
    {25369, 51552},                                         // belf F
    {25370, 51545},                                         // troll F
    {25371, 51548},                                         // tauren F
    {25372, 51550},                                         // forsaken F
    {25373, 51551}                                          // belf M
};

/*######
## npc_unworthy_initiate_anchor
######*/

struct MANGOS_DLL_DECL npc_unworthy_initiate_anchorAI : public ScriptedAI
{
    npc_unworthy_initiate_anchorAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    ObjectGuid m_myInitiateGuid;
    ObjectGuid m_myPrisonGuid;

    void Reset() {}

    void NotifyMe(Unit* pSource, GameObject* pGo)
    {
        m_myPrisonGuid = pGo->GetObjectGuid();
        Creature* pInitiate = m_creature->GetMap()->GetCreature(m_myInitiateGuid);

        if (pInitiate && pSource)
        {
            pInitiate->SetLootRecipient(pSource);
            m_creature->CastSpell(pInitiate,SPELL_CHAINED_PESANT_BREATH,true);
        }
    }

    void RegisterCloseInitiate(Creature* pCreature)
    {
        m_myInitiateGuid = pCreature->GetObjectGuid();
    }

    void ResetPrison()
    {
        if (GameObject* pPrison = m_creature->GetMap()->GetGameObject(m_myPrisonGuid))
            pPrison->ResetDoorOrButton();
    }
};

CreatureAI* GetAI_npc_unworthy_initiate_anchor(Creature* pCreature)
{
    return new npc_unworthy_initiate_anchorAI(pCreature);
}

/*######
## npc_unworthy_initiate
######*/

struct MANGOS_DLL_DECL npc_unworthy_initiateAI : public ScriptedAI
{
    npc_unworthy_initiateAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pToTransform = NULL;

        uint32 uiDisplayCount = sizeof(m_aDisplayToSpell)/sizeof(DisplayToSpell);

        for (uint8 i=0; i<uiDisplayCount; ++i)
        {
            // check if we find a match, if not, it's NULL and produce error when used
            if (m_aDisplayToSpell[i].m_uiDisplayId == pCreature->GetDisplayId())
            {
                m_pToTransform = &m_aDisplayToSpell[i];
                break;
            }
        }

        m_uiNormFaction = pCreature->getFaction();
        Reset();
    }

    DisplayToSpell* m_pToTransform;

    ObjectGuid m_myAnchorGuid;
    uint32 m_uiNormFaction;
    uint32 m_uiAnchorCheckTimer;
    uint32 m_uiPhase;
    uint32 m_uiPhaseTimer;
    uint32 m_uiBloodStrike_Timer;
    uint32 m_uiDeathCoil_Timer;
    uint32 m_uiIcyTouch_Timer;
    uint32 m_uiPlagueStrike_Timer;

    void Reset()
    {
        if (m_creature->getFaction() != m_uiNormFaction)
            m_creature->setFaction(m_uiNormFaction);

        m_uiAnchorCheckTimer = 5000;
        m_uiPhase = PHASE_INACTIVE_OR_COMBAT;
        m_uiPhaseTimer = 7500;
        m_uiBloodStrike_Timer = 4000;
        m_uiDeathCoil_Timer = 6000;
        m_uiIcyTouch_Timer = 2000;
        m_uiPlagueStrike_Timer = 5000;
    }

    void JustReachedHome()
    {
        SetAnchor();

        if (Creature* pAnchor = GetAnchor())
        {
            if (npc_unworthy_initiate_anchorAI* pAnchorAI = dynamic_cast<npc_unworthy_initiate_anchorAI*>(pAnchor->AI()))
                pAnchorAI->ResetPrison();
        }
    }

    void JustRespawned()
    {
        if (Creature* pAnchor = GetAnchor())
        {
            if (npc_unworthy_initiate_anchorAI* pAnchorAI = dynamic_cast<npc_unworthy_initiate_anchorAI*>(pAnchor->AI()))
                pAnchorAI->ResetPrison();
        }

        Reset();
    }

    int32 GetTextId()
    {
        return m_uiPhase == PHASE_DRESSUP ? SAY_START-rand()%8 : SAY_AGGRO-rand()%8;
    }

    Creature* GetAnchor()
    {
        if (m_myAnchorGuid)
            return m_creature->GetMap()->GetCreature(m_myAnchorGuid);
        else
            return GetClosestCreatureWithEntry(m_creature, NPC_ANCHOR, INTERACTION_DISTANCE*2);
    }

    void SetAnchor()
    {
        if (Creature* pAnchor = GetAnchor())
        {
            if (npc_unworthy_initiate_anchorAI* pAnchorAI = dynamic_cast<npc_unworthy_initiate_anchorAI*>(pAnchor->AI()))
                pAnchorAI->RegisterCloseInitiate(m_creature);

            pAnchor->CastSpell(m_creature, SPELL_CHAINED_PESANT_CHEST, false);
            m_myAnchorGuid = pAnchor->GetObjectGuid();

            m_uiAnchorCheckTimer = 0;
            return;
        }

        m_uiAnchorCheckTimer = 5000;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_CHAINED_PESANT_BREATH)
        {
            pCaster->InterruptNonMeleeSpells(true);
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);

            m_uiPhase = PHASE_DRESSUP;

            if (Player* pSource = m_creature->GetLootRecipient())
                DoScriptText(GetTextId(), m_creature, pSource);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAnchorCheckTimer)
        {
            if (m_uiAnchorCheckTimer <= uiDiff)
                SetAnchor();
            else
                m_uiAnchorCheckTimer -= uiDiff;
        }

        if (m_uiPhase == PHASE_INACTIVE_OR_COMBAT)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (m_uiBloodStrike_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_BLOOD_STRIKE);
                m_uiBloodStrike_Timer = 9000;
            }
            else
                m_uiBloodStrike_Timer -= uiDiff;

            if (m_uiDeathCoil_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_DEATH_COIL);
                m_uiDeathCoil_Timer = 8000;
            }
            else
                m_uiDeathCoil_Timer -= uiDiff;

            if (m_uiIcyTouch_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_ICY_TOUCH);
                m_uiIcyTouch_Timer = 8000;
            }
            else
                m_uiIcyTouch_Timer -= uiDiff;

            if (m_uiPlagueStrike_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_PLAGUE_STRIKE);
                m_uiPlagueStrike_Timer = 8000;
            }
            else
                m_uiPlagueStrike_Timer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        else
        {
            if (m_uiPhaseTimer < uiDiff)
            {
                if (m_uiPhase == PHASE_DRESSUP)
                {
                    if (m_pToTransform)
                    {
                        m_creature->CastSpell(m_creature, m_pToTransform->m_uiSpellToNewDisplay, true);
                        m_creature->CastSpell(m_creature, SPELL_INITIATE_VISUAL, false);
                    }
                    else
                        error_log("SD2: npc_unworthy_initiate does not have any spell associated with model");

                    m_uiPhase = PHASE_ACTIVATE;
                }
                else
                {
                    m_creature->setFaction(FACTION_MONSTER);

                    m_uiPhase = PHASE_INACTIVE_OR_COMBAT;

                    if (Player* pTarget = m_creature->GetLootRecipient())
                    {
                        DoScriptText(GetTextId(), m_creature, pTarget);
                        AttackStart(pTarget);
                    }
                }

                m_uiPhaseTimer = 5000;
            }
            else
                m_uiPhaseTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_unworthy_initiate(Creature* pCreature)
{
    return new npc_unworthy_initiateAI(pCreature);
}

/*######
## go_acherus_soul_prison
######*/

bool GOUse_go_acherus_soul_prison(Player* pPlayer, GameObject* pGo)
{
    if (Creature* pAnchor = GetClosestCreatureWithEntry(pGo, NPC_ANCHOR, INTERACTION_DISTANCE))
    {
        if (npc_unworthy_initiate_anchorAI* pAnchorAI = dynamic_cast<npc_unworthy_initiate_anchorAI*>(pAnchor->AI()))
            pAnchorAI->NotifyMe(pPlayer, pGo);
    }

    return false;
}

/*######
## npc_eye_of_acherus
######*/

enum
{
    SPELL_EYE_CONTROL       = 51852,        // player control aura
    SPELL_EYE_VISUAL        = 51892,
    SPELL_EYE_FLIGHT        = 51890,        // player flight control
    SPELL_EYE_FLIGHT_BOOST  = 51923,        // flight boost to reach new avalon

    EMOTE_DESTIANTION       = -1609089,
    EMOTE_CONTROL           = -1609090,

    POINT_EYE_DESTINATION   = 0
};

// movement destination coords
static const float aEyeDestination[3] = {1750.8276f, -5873.788f, 147.2266f};

struct MANGOS_DLL_DECL npc_eye_of_acherusAI : public ScriptedAI
{
    npc_eye_of_acherusAI(Creature *pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_isActive;

    void Reset() { m_isActive = false; }

    void JustDied(Unit* pKiller)
    {
        // remove charm aura from player if eye dies
        if (Unit* pCharmer = m_creature->GetCharmer())
            pCharmer->RemoveAurasDueToSpell(SPELL_EYE_CONTROL);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
       if (uiType != POINT_MOTION_TYPE || uiPointId != POINT_EYE_DESTINATION)
            return;

        m_creature->MonsterTextEmote(EMOTE_CONTROL, m_creature, true);
        m_creature->CastSpell(m_creature, SPELL_EYE_FLIGHT, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_isActive)
            return;

        if (m_creature->isCharmed())
        {
            m_creature->MonsterTextEmote(EMOTE_DESTIANTION, m_creature, true);

            m_creature->CastSpell(m_creature, SPELL_EYE_VISUAL, true);
            m_creature->CastSpell(m_creature, SPELL_EYE_FLIGHT_BOOST, true);

            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->GetMotionMaster()->MovePoint(POINT_EYE_DESTINATION, aEyeDestination[0], aEyeDestination[1], aEyeDestination[2]);

            m_isActive = true;
        }
        else
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_npc_eye_of_acherus(Creature* pCreature)
{
    return new npc_eye_of_acherusAI(pCreature);
}

/*######
## npc_highlord_darion_mograine
######*/

enum LightOfDawn
{
    // yells
    SAY_LIGHT_OF_DAWN_INTRO_1           = -1609201,     // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN_INTRO_2           = -1609202,

    SAY_LIGHT_OF_DAWN_PREPARE_1         = -1609203,
    SAY_LIGHT_OF_DAWN_PREPARE_2         = -1609204,
    SAY_LIGHT_OF_DAWN_PREPARE_3         = -1609205,
    SAY_LIGHT_OF_DAWN_PREPARE_4         = -1609206,

    SAY_LIGHT_OF_DAWN_STAND_1           = -1609207,     // Korfax
    SAY_LIGHT_OF_DAWN_STAND_2           = -1609208,     // Lord Maxwell Tyrosus

    SAY_LIGHT_OF_DAWN_BATTLE_1          = -1609209,     // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN_BATTLE_2          = -1609210,
    SAY_LIGHT_OF_DAWN_BATTLE_3          = -1609211,
    SAY_LIGHT_OF_DAWN_BATTLE_4          = -1609212,
    SAY_LIGHT_OF_DAWN_BATTLE_5          = -1609213,
    SAY_LIGHT_OF_DAWN_BATTLE_6          = -1609214,
    SAY_LIGHT_OF_DAWN_BATTLE_7          = -1609215,
    SAY_LIGHT_OF_DAWN_BATTLE_8          = -1609216,
    SAY_LIGHT_OF_DAWN_BATTLE_9          = -1609217,
    SAY_LIGHT_OF_DAWN_BATTLE_10         = -1609218,
    SAY_LIGHT_OF_DAWN_BATTLE_11         = -1609219,
    SAY_LIGHT_OF_DAWN_BATTLE_12         = -1609220,
    SAY_LIGHT_OF_DAWN_BATTLE_13         = -1609221,
    SAY_LIGHT_OF_DAWN_BATTLE_14         = -1609222,
    SAY_LIGHT_OF_DAWN_BATTLE_15         = -1609223,
    SAY_LIGHT_OF_DAWN_BATTLE_16         = -1609224,

    SAY_LIGHT_OF_DAWN_OUTRO_1           = -1609225,     // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN_OUTRO_2           = -1609226,
    SAY_LIGHT_OF_DAWN_OUTRO_3           = -1609227,     // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN_OUTRO_4           = -1609228,     // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN_OUTRO_5           = -1609229,
    SAY_LIGHT_OF_DAWN_OUTRO_6           = -1609230,
    SAY_LIGHT_OF_DAWN_OUTRO_7           = -1609231,     // Highlord Darion Mograine

    SAY_LIGHT_OF_DAWN_VISION_1          = -1609232,     // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN_VISION_2          = -1609233,     // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN_VISION_3          = -1609234,
    SAY_LIGHT_OF_DAWN_VISION_4          = -1609235,     // Darion Mograine
    SAY_LIGHT_OF_DAWN_VISION_5          = -1609236,
    SAY_LIGHT_OF_DAWN_VISION_6          = -1609237,     // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN_VISION_7          = -1609238,     // Darion Mograine
    SAY_LIGHT_OF_DAWN_VISION_8          = -1609239,     // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN_VISION_9          = -1609240,     // Darion Mograine
    SAY_LIGHT_OF_DAWN_VISION_10         = -1609241,     // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN_VISION_11         = -1609242,

    SAY_LIGHT_OF_DAWN_KING_VISIT_1      = -1609243,     // The Lich King
    SAY_LIGHT_OF_DAWN_KING_VISIT_2      = -1609245,
    SAY_LIGHT_OF_DAWN_KING_VISIT_3      = -1609244,     // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN_KING_VISIT_4      = -1609246,     // The Lich King
    SAY_LIGHT_OF_DAWN_KING_VISIT_5      = -1609247,     // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN_KING_VISIT_6      = -1609248,     // The Lich King
    SAY_LIGHT_OF_DAWN_KING_VISIT_7      = -1609249,
    SAY_LIGHT_OF_DAWN_KING_VISIT_8      = -1609250,     // Lord Maxwell Tyrosus
    SAY_LIGHT_OF_DAWN_KING_VISIT_9      = -1609251,     // The Lich King
    SAY_LIGHT_OF_DAWN_KING_VISIT_10     = -1609252,     // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN_KING_VISIT_11     = -1609253,
    SAY_LIGHT_OF_DAWN_KING_VISIT_12     = -1609254,     // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN_KING_VISIT_13     = -1609255,     // The Lich King
    SAY_LIGHT_OF_DAWN_KING_VISIT_14     = -1609256,     // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN_KING_VISIT_15     = -1609257,     // The Lich King
    SAY_LIGHT_OF_DAWN_KING_VISIT_16     = -1609258,
    SAY_LIGHT_OF_DAWN_KING_VISIT_17     = -1609259,

    SAY_LIGHT_OF_DAWN_EPILOGUE_1        = -1609260,     // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN_EPILOGUE_2        = -1609261,
    SAY_LIGHT_OF_DAWN_EPILOGUE_3        = -1609262,
    SAY_LIGHT_OF_DAWN_EPILOGUE_4        = -1609263,
    SAY_LIGHT_OF_DAWN_EPILOGUE_5        = -1609264,
    SAY_LIGHT_OF_DAWN_EPILOGUE_6        = -1609265,
    SAY_LIGHT_OF_DAWN_EPILOGUE_7        = -1609266,
    SAY_LIGHT_OF_DAWN_EPILOGUE_8        = -1609267,
    SAY_LIGHT_OF_DAWN_EPILOGUE_9        = -1609268,     // Highlord Darion Mograine

    // Emotes
    EMOTE_LIGHT_OF_DAWN_ARMY_RISE       = -1609269,  // Emotes
    EMOTE_LIGHT_OF_DAWN_ARMY_MARCH      = -1609270,
    EMOTE_LIGHT_OF_DAWN_TIRION          = -1609271,
    EMOTE_LIGHT_OF_DAWN_FLEE            = -1609272,
    EMOTE_LIGHT_OF_DAWN_KNEEL           = -1609273,
    EMOTE_LIGHT_OF_DAWN_ALEXANDROS      = -1609274,
    EMOTE_LIGHT_OF_DAWN_SHADE           = -1609275,
    EMOTE_LIGHT_OF_DAWN_HUG             = -1609276,
    EMOTE_LIGHT_OF_DAWN_LICH_KING       = -1609277,
    EMOTE_LIGHT_OF_DAWN_ANGRY           = -1609278,
    EMOTE_LIGHT_OF_DAWN_CAST_SPELL      = -1609279,
    EMOTE_LIGHT_OF_DAWN_GRASP           = -1609280,
    EMOTE_LIGHT_OF_DAWN_POWERFULL       = -1609281,
    EMOTE_LIGHT_OF_DAWN_ASHBRINGER      = -1609282,
    EMOTE_LIGHT_OF_DAWN_COLAPSE         = -1609283,
    EMOTE_LIGHT_OF_DAWN_CHARGE          = -1609284,
    EMOTE_LIGHT_OF_DAWN_KING_LEAVE      = -1609285,
    EMOTE_LIGHT_OF_DAWN_LIGHT           = -1609286,

    // Spells
    // Highlord Darion Mograine
    SPELL_HERO_AGGRO_AURA               = 53627,
    SPELL_SCOURGE_AGGRO_AURA            = 53624,
    SPELL_ANTI_MAGIC_ZONE_DARION        = 52893,
    SPELL_DEATH_STRIKE                  = 53639,
    SPELL_DEATH_EMBRACE                 = 53635,
    SPELL_ICY_TOUCH_DARION              = 49723,
    SPELL_PLAGUE_STRIKE_KNIGHTS         = 50688,
    SPELL_THE_MIGHT_OF_MOGRAINE         = 53642,    // on players when battle begins
    SPELL_UNHOLY_BLIGHT                 = 53640,

    SPELL_BIRTH                         = 53603,    // ground shake
    SPELL_THE_LIGHT_OF_DAWN             = 53658,    // light globe
    SPELL_THE_LIGHT_OF_DAWN_AURA        = 53644,    // cast when Tirion arives
    SPELL_ALEXANDROS_MOGRAINE_SPAWN     = 53667,    // spawn effect for Alexandros
    SPELL_MOGRAINE_CHARGE               = 53679,    // charge to the Lich King
    SPELL_ASHBRINGER                    = 53701,    // throw Ashbringer to Tirion
    SPELL_THE_LIGHT_OF_DAWN_CREDIT      = 53606,    // quest credit

    // Lich King spells
    SPELL_APOCALYPSE                    = 53210,    // knocks back all enemies
    SPELL_APOCALYPSE_STUN               = 53745,    // stuns all enemies
    SPELL_TELEPORT_VISUAL               = 52233,    // on leave
    SPELL_SOUL_FEAST_ALEX               = 53677,    // on Alexandros
    SPELL_SOUL_FEAST_TIRION             = 53685,    // on Tirion
    SPELL_ICEBOUND_VISAGE               = 53274,    // ice effect
    SPELL_REBUKE                        = 53680,    // knockback

    // Highlord Tirion Fordring
    //EQUIP_HIGHLORD_TIRION_FORDRING      = 13262,
    SPELL_LAY_ON_HANDS                  = 53778,    // heal effect
    SPELL_REBIRTH_OF_THE_ASHBRINGER     = 53702,    // globe sphere
    SPELL_TIRION_CHARGE                 = 53705,    // on the lich king

    POINT_MOVE_CHAPEL                   = 0,
    POINT_MOVE_OTHER                    = 1,

    // others
    QUEST_ID_LIGHT_OF_DAWN              = 12801,

    GOSSIP_ITEM_READY                   = -3609001
};

struct MANGOS_DLL_DECL npc_highlord_darion_mograineAI : public npc_escortAI
{
    npc_highlord_darion_mograineAI(Creature *pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (world_map_ebon_hold*)pCreature->GetInstanceData();
        Reset();
    }

    world_map_ebon_hold* m_pInstance;

    // event timers
    uint8 m_uiIntroYell;
    uint32 m_uiPrepareTimer;

    uint32 m_uiEventStep;
    uint32 m_uiEventTimer;
    uint32 m_uiFightTimer;
    uint32 m_uiAttackTimer;

    uint8 m_uiLightWarriorsDead;
    uint8 m_uiScourgeWarriorsDead;

    // spell timers
    uint32 m_uiAntimagicZoneTimer;
    uint32 m_uiDeathStrikeTimer;
    uint32 m_uiDeathEmbraceTimer;
    uint32 m_uiIcyTouchTimer;
    uint32 m_uiUnholyBlightTimer;
    uint32 m_uiFightSpeechTimer;

    uint32 m_uiSpawncheck;
    uint32 m_uiTargetcheck;

    // others
    GUIDList lDefendersGUIDs;       // light of dawn defenders
    GUIDList lAttackersGUIDs;       // scourge attackers

    void Reset()
    {
        // reset only when event is not in progress
        if (!HasEscortState(STATE_ESCORT_ESCORTING))
        {
            m_uiIntroYell           = 0;
            m_uiPrepareTimer        = 5*MINUTE*IN_MILLISECONDS;

            m_uiEventStep           = 0;
            m_uiEventTimer          = 3000;
            m_uiFightTimer          = 0;
            m_uiAttackTimer         = 1000;     // timer to change threat

            m_uiLightWarriorsDead   = 0;
            m_uiScourgeWarriorsDead = 0;

            m_uiAntimagicZoneTimer  = urand(1000, 5000);
            m_uiDeathStrikeTimer    = urand(5000, 10000);
            m_uiDeathEmbraceTimer   = urand(5000, 10000);
            m_uiIcyTouchTimer       = urand(5000, 10000);
            m_uiUnholyBlightTimer   = urand(5000, 10000);
            m_uiFightSpeechTimer    = 15000;
        }
    }

    void Aggro(Unit* pWho)
    {
        // cast aggro aura
        DoCastSpellIfCan(m_creature, SPELL_HERO_AGGRO_AURA);
    }

    void JustSummoned(Creature* pSummoned)
    {
        // store summoned guid for easy handle
        switch(pSummoned->GetEntry())
        {
            case NPC_VOLATILE_GHOUL:
            case NPC_WARRIOR_OF_THE_FROZEN_WASTES:
                lAttackersGUIDs.push_back(pSummoned->GetObjectGuid());
                if (m_creature->isInCombat())
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        pSummoned->AI()->AttackStart(pTarget);
                }
                break;
            case NPC_DEFENDER_OF_THE_LIGHT:
                lDefendersGUIDs.push_back(pSummoned->GetObjectGuid());
                pSummoned->AI()->AttackStart(m_creature);
                break;
        }

        // set respawn delay
        pSummoned->SetRespawnDelay(DAY);
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        // should we count the 2 behemots and 5 abominations as well?
        switch(pSummoned->GetEntry())
        {
            case NPC_VOLATILE_GHOUL:
            case NPC_WARRIOR_OF_THE_FROZEN_WASTES:
                ++m_uiScourgeWarriorsDead;
                lAttackersGUIDs.remove(pSummoned->GetObjectGuid());
                break;
            case NPC_DEFENDER_OF_THE_LIGHT:
                ++m_uiLightWarriorsDead;
                lDefendersGUIDs.remove(pSummoned->GetObjectGuid());
                break;
        }

        // update the world states
        if (m_pInstance)
        {
            m_pInstance->DoUpdateBattleWorldState(WORLD_STATE_FORCES_SCOURGE, MAX_FORCES_SCOURGE - m_uiScourgeWarriorsDead);
            m_pInstance->DoUpdateBattleWorldState(WORLD_STATE_FORCES_LIGHT, MAX_FORCES_LIGHT - m_uiLightWarriorsDead);
        }

        // if battle has ended return
        if (m_pInstance->GetData(TYPE_BATTLE) != IN_PROGRESS)
            return;

        // if 5 soldiers are dead summon others
        if (m_uiScourgeWarriorsDead > 0 && m_uiScourgeWarriorsDead % MAX_SCOURGE == 0)
        {
            DoCastSpellIfCan(m_creature, SPELL_BIRTH);

            float fX, fY, fZ;
            for (uint8 i = 0; i < MAX_SCOURGE; i++)
            {
                uint32 uiSummonEntry = urand(0, 1) ? NPC_VOLATILE_GHOUL : NPC_WARRIOR_OF_THE_FROZEN_WASTES;
                m_creature->GetRandomPoint(aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ, 30.0f, fX, fY, fZ);
                m_creature->SummonCreature(uiSummonEntry, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 5000);
            }
        }

        // if 5 light soldiers are dead summon others
        if (m_uiLightWarriorsDead > 0 && m_uiLightWarriorsDead % MAX_SCOURGE == 0)
        {
            float fX, fY, fZ;
            for (uint8 i = 0; i < MAX_SCOURGE; i++)
            {
                m_creature->GetRandomPoint(aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ, 30.0f, fX, fY, fZ);
                m_creature->SummonCreature(NPC_DEFENDER_OF_THE_LIGHT, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 5000);
            }
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE || uiPointId != POINT_MOVE_CHAPEL)
            return;

        if (!m_pInstance)
            return;

        switch(pSummoned->GetEntry())
        {
            // hug father
            case NPC_DARION_MOGRAINE:
                if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                    DoScriptText(EMOTE_LIGHT_OF_DAWN_HUG, pSummoned, pAlexandros);
                break;
            // make the first 2 kneel
            case NPC_KORFAX_CHAMPION_OF_THE_LIGHT:
            case NPC_LORD_MAXWELL_TYROSUS:
                pSummoned->SetStandState(UNIT_STAND_STATE_KNEEL);
            case NPC_COMMANDER_ELIGOR_DAWNBRINGER:
            case NPC_LEONID_BARTHALOMEW_THE_REVERED:
            case NPC_DUKE_NICHOLAS_ZVERENHOFF:
            case NPC_RIMBLAT_EARTHSHATTER:
            case NPC_RAYNE:
                if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                    pSummoned->SetFacingToObject(pTirion);
                break;
                // tirions stops the battle
            case NPC_HIGHLORD_TIRION_FORDRING:
                // bring them in front of the chapel
                DoScriptText(SAY_LIGHT_OF_DAWN_OUTRO_2, pSummoned);
                // set battle to done
                m_pInstance->SetData(TYPE_BATTLE, DONE);

                // scourge fighters die
                // TODO: proper kill the scourge and FIX CRASH!!!
                /*for (GUIDList::const_iterator itr = lAttackersGUIDs.begin(); itr != lAttackersGUIDs.end(); ++itr)
                {
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        pTemp->CastSpell(pTemp, SPELL_THE_LIGHT_OF_DAWN_DAMAGE, true);
                }*/

                // ##### workaround #####
                std::list<Creature* > lCreatureList;
                GetCreatureListWithEntryInGrid(lCreatureList, m_creature, NPC_VOLATILE_GHOUL, 50.0f);
                for (std::list<Creature*>::const_iterator itr = lCreatureList.begin(); itr != lCreatureList.end(); ++itr)
                    (*itr)->DealDamage(*itr, (*itr)->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    //(*itr)->CastSpell((*itr), SPELL_THE_LIGHT_OF_DAWN_DAMAGE, true);

                lCreatureList.clear();
                GetCreatureListWithEntryInGrid(lCreatureList, m_creature, NPC_WARRIOR_OF_THE_FROZEN_WASTES, 50.0f);
                for (std::list<Creature*>::const_iterator itr = lCreatureList.begin(); itr != lCreatureList.end(); ++itr)
                    (*itr)->DealDamage(*itr, (*itr)->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    //(*itr)->CastSpell((*itr), SPELL_THE_LIGHT_OF_DAWN_DAMAGE, true);
                // ##### end of workaround #####

                // light fighters despawn
                for (GUIDList::const_iterator itr = lDefendersGUIDs.begin(); itr != lDefendersGUIDs.end(); ++itr)
                {
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        pTemp->ForcedDespawn();
                }

                // despawn big units
                m_pInstance->DoDespawnArmy();

                // facing and mount
                pSummoned->Unmount();
                pSummoned->SetFacingTo(aEventLocations[1].m_fO);

                m_creature->Unmount();
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);

                DoCastSpellIfCan(m_creature, SPELL_THE_LIGHT_OF_DAWN);

                // death knights are defeated
                if (Creature* pKoltira = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLTIRA_DEATHWEAVER))
                {
                    pKoltira->AttackStop();
                    pKoltira->CombatStop(true);
                    pKoltira->RemoveAllAuras();
                    pKoltira->DeleteThreatList();
                    pKoltira->CastStop();
                    pKoltira->AI()->EnterEvadeMode();
                }
                if (Creature* pThassarian = m_pInstance->GetSingleCreatureFromStorage(NPC_THASSARIAN))
                {
                    pThassarian->AttackStop();
                    pThassarian->CombatStop(true);
                    pThassarian->RemoveAllAuras();
                    pThassarian->DeleteThreatList();
                    pThassarian->CastStop();
                    pThassarian->AI()->EnterEvadeMode();
                }
                // Orbaz flees -> despawn
                if (Creature* pOrbaz = m_pInstance->GetSingleCreatureFromStorage(NPC_ORBAZ_BLOODBANE))
                {
                    DoScriptText(EMOTE_LIGHT_OF_DAWN_FLEE, pOrbaz);
                    pOrbaz->AttackStop();
                    pOrbaz->CombatStop(true);
                    pOrbaz->RemoveAllAuras();
                    pOrbaz->DeleteThreatList();
                    pOrbaz->GetMotionMaster()->MoveTargetedHome();
                    pOrbaz->ForcedDespawn(30000);
                }

                // ligth champs evade to their summon points
                for (uint8 i = 0; i < MAX_LIGHT_CHAMPIONS; i++)
                {
                    if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(aLightArmySpawnLoc[i].m_uiEntry))
                    {
                        if (!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                        {
                            pTemp->AttackStop();
                            pTemp->CombatStop(true);
                            pTemp->RemoveAllAuras();
                            pTemp->DeleteThreatList();
                            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            // need to set a point instead of TargetedHome because we need to reset orientation
                            pTemp->GetMotionMaster()->MovePoint(POINT_MOVE_CHAPEL, aLightArmySpawnLoc[i].m_fX, aLightArmySpawnLoc[i].m_fY, aLightArmySpawnLoc[i].m_fZ);
                        }
                    }
                }

                // clear defenders list
                lDefendersGUIDs.clear();

                // spawn soldiers
                for (uint8 i = 0; i < MAX_LIGHT_GUARDS; i++)
                {
                    if (Creature* pGuard = m_creature->SummonCreature(NPC_DEFENDER_OF_THE_LIGHT, aGuardsSpawnLoc[i].m_fX, aGuardsSpawnLoc[i].m_fY, aGuardsSpawnLoc[i].m_fZ, aGuardsSpawnLoc[i].m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0))
                    {
                        // make guard passive and with weapon
                        pGuard->SetFacingToObject(m_creature);
                        pGuard->HandleEmoteCommand(EMOTE_STATE_ATTACK_UNARMED);            // should be 2 handed when the DB data is correct
                        pGuard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pGuard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                    }
                }

                SetEscortPaused(false);
                break;
        }
    }

    void JustRespawned()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BATTLE, NOT_STARTED);

        npc_escortAI::JustRespawned();
    }

    void WaypointReached(uint32 uiPoint)
    {
        if (!m_pInstance)
            return;

        switch(uiPoint)
        {
            case 0:
                // summon light champions
                for (uint8 i = 0; i < MAX_LIGHT_CHAMPIONS; i++)
                    m_creature->SummonCreature(aLightArmySpawnLoc[i].m_uiEntry, aLightArmySpawnLoc[i].m_fX, aLightArmySpawnLoc[i].m_fY, aLightArmySpawnLoc[i].m_fZ, aLightArmySpawnLoc[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);

                // summon light soldiers
                float fX, fY, fZ;
                for(uint8 i = 0; i < MAX_LIGHT_WARRIORS; ++i)
                {
                    m_creature->GetRandomPoint(aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ, 30.0f, fX, fY, fZ);
                    m_creature->SummonCreature(NPC_DEFENDER_OF_THE_LIGHT, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 5000);
                }
                break;
            case 2:
                // yell dawn 1
                if (Creature* pKorfax = m_pInstance->GetSingleCreatureFromStorage(NPC_KORFAX_CHAMPION_OF_THE_LIGHT))
                    DoScriptText(SAY_LIGHT_OF_DAWN_STAND_1, pKorfax);
                break;
            case 3:
                // yell dawn 2
                if (Creature* pMaxwell = m_pInstance->GetSingleCreatureFromStorage(NPC_LORD_MAXWELL_TYROSUS))
                    DoScriptText(SAY_LIGHT_OF_DAWN_STAND_2, pMaxwell);
                break;
            case 4:
                // start the battle
                SetEscortPaused(true);
                DoCastSpellIfCan(m_creature, SPELL_THE_MIGHT_OF_MOGRAINE);

                // make champs attack him
                for (uint8 i = 0; i < MAX_LIGHT_CHAMPIONS; i++)
                {
                    if (Creature* pChamp = m_pInstance->GetSingleCreatureFromStorage(aLightArmySpawnLoc[i].m_uiEntry))
                    {
                        m_creature->AI()->AttackStart(pChamp);
                        m_creature->SetInCombatWith(pChamp);
                    }
                }

                // max fight timer
                m_uiFightTimer = 4*MINUTE*IN_MILLISECONDS;
                break;
            case 5:
                // battle finished - remove light of dawn aura
                DoScriptText(EMOTE_LIGHT_OF_DAWN_KNEEL, m_creature);
                DoScriptText(SAY_LIGHT_OF_DAWN_OUTRO_3, m_creature);

                if (m_creature->HasAura(SPELL_THE_LIGHT_OF_DAWN))
                    m_creature->RemoveAurasDueToSpell(SPELL_THE_LIGHT_OF_DAWN);

                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);

                if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                    m_creature->SetFacingToObject(pTirion);

                // update guards facing
                for (GUIDList::const_iterator itr = lDefendersGUIDs.begin(); itr != lDefendersGUIDs.end(); ++itr)
                {
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        pTemp->SetFacingToObject(m_creature);
                }

                // escort paused and start cinematic
                m_uiEventTimer = 10000;
                SetEscortPaused(true);
                break;
        }
    }

    void DoSendQuestCredit()
    {
        Map::PlayerList const &PlayerList = m_creature->GetMap()->GetPlayers();

        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            Player* pPlayer = itr->getSource();
            if (pPlayer && pPlayer->GetQuestStatus(QUEST_ID_LIGHT_OF_DAWN) == QUEST_STATUS_INCOMPLETE && pPlayer->isAlive() && m_creature->IsWithinDistInMap(pPlayer, 50.0f))
                pPlayer->CastSpell(pPlayer, SPELL_THE_LIGHT_OF_DAWN_CREDIT, true);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        npc_escortAI::UpdateAI(uiDiff);

        if (m_pInstance->GetData(TYPE_BATTLE) == SPECIAL)
        {
            // intro event and battle timer
            if (m_uiIntroYell == 0 && m_uiPrepareTimer < 3*MINUTE*IN_MILLISECONDS)
            {
                DoScriptText(SAY_LIGHT_OF_DAWN_INTRO_1, m_creature);
                ++m_uiIntroYell;
            }
            else if (m_uiIntroYell == 1 && m_uiPrepareTimer < 2*MINUTE*IN_MILLISECONDS)
            {
                DoScriptText(SAY_LIGHT_OF_DAWN_INTRO_2, m_creature);
                ++m_uiIntroYell;
            }

            // battle timer
            if (m_uiPrepareTimer < uiDiff)
            {
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_BATTLE, IN_PROGRESS);
            }
            else
            {
                m_uiPrepareTimer -= uiDiff;

                if (m_uiPrepareTimer/IN_MILLISECONDS % 60 == 0)
                {
                    if (m_pInstance)
                        m_pInstance->DoUpdateBattleWorldState(WORLD_STATE_BATTLE_TIMER_TIME, m_uiPrepareTimer/(MINUTE*IN_MILLISECONDS));
                }
            }
        }
        else if (m_pInstance->GetData(TYPE_BATTLE) == IN_PROGRESS || m_pInstance->GetData(TYPE_BATTLE) == DONE)
        {
            if (m_uiEventTimer)
            {
                if (m_uiEventTimer <= uiDiff)
                {
                    if (!m_pInstance)
                        return;

                    switch(m_uiEventStep)
                    {
                        case 0:
                            DoScriptText(SAY_LIGHT_OF_DAWN_PREPARE_1, m_creature);
                            m_uiEventTimer = 5000;
                            break;
                        case 1:
                            DoScriptText(SAY_LIGHT_OF_DAWN_PREPARE_2, m_creature);
                            m_uiEventTimer = 10000;
                            break;
                        case 2:
                            DoScriptText(SAY_LIGHT_OF_DAWN_PREPARE_3, m_creature);
                            DoScriptText(EMOTE_LIGHT_OF_DAWN_ARMY_RISE, m_creature);
                            m_uiEventTimer = 3000;
                            break;
                        case 3:
                        case 4:
                        case 5:
                            // summon army takes about 20 secs and it's done on a few stages
                            if (DoCastSpellIfCan(m_creature, SPELL_BIRTH) == CAST_OK)
                            {
                                float fX, fY, fZ;
                                for(uint8 i = 0; i < MAX_SCOURGE; ++i)
                                {
                                    // Note: missing spawn effect for them!
                                    uint32 uiSummonEntry = urand(0, 1) ? NPC_VOLATILE_GHOUL : NPC_WARRIOR_OF_THE_FROZEN_WASTES;
                                    m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 50.0f, fX, fY, fZ);
                                    m_creature->SummonCreature(uiSummonEntry, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 5000);
                                }

                                m_uiEventTimer = 6000;
                            }
                            break;
                        case 6:
                            DoScriptText(SAY_LIGHT_OF_DAWN_PREPARE_4, m_creature);
                            m_uiEventTimer = 2000;
                            break;
                        case 7:
                            // send army emote
                            for(GUIDList::const_iterator itr = lAttackersGUIDs.begin(); itr != lAttackersGUIDs.end(); ++itr)
                            {
                                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                                {
                                    pTemp->SetFacingToObject(m_creature);
                                    pTemp->HandleEmoteCommand(EMOTE_ONESHOT_BATTLEROAR);
                                }
                            }
                            m_uiEventTimer = 6000;
                            break;
                        case 8:
                            // start attack (escort)
                            DoScriptText(EMOTE_LIGHT_OF_DAWN_ARMY_MARCH, m_creature);
                            Start(true);

                            // move the companions as well
                            float fX, fY, fZ;
                            if (Creature* pKoltira = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLTIRA_DEATHWEAVER))
                            {
                                pKoltira->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                m_creature->GetRandomPoint(aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ, 30.0f, fX, fY, fZ);
                                pKoltira->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
                            }
                            if (Creature* pThassarian = m_pInstance->GetSingleCreatureFromStorage(NPC_THASSARIAN))
                            {
                                pThassarian->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                m_creature->GetRandomPoint(aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ, 30.0f, fX, fY, fZ);
                                pThassarian->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
                            }
                            if (Creature* pOrbaz = m_pInstance->GetSingleCreatureFromStorage(NPC_ORBAZ_BLOODBANE))
                            {
                                pOrbaz->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                m_creature->GetRandomPoint(aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ, 30.0f, fX, fY, fZ);
                                pOrbaz->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
                            }

                            // move army
                            for(GUIDList::const_iterator itr = lAttackersGUIDs.begin(); itr != lAttackersGUIDs.end(); ++itr)
                            {
                                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                                {
                                    pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                    m_creature->GetRandomPoint(aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ, 30.0f, fX, fY, fZ);
                                    pTemp->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
                                }
                            }
                            m_pInstance->DoMoveArmy();
                            m_uiEventTimer = 0;
                            break;
                        case 9:
                            // after the battle
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                DoScriptText(SAY_LIGHT_OF_DAWN_OUTRO_4, pTirion);
                                m_uiEventTimer = 21000;
                            }
                            break;
                        case 10:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                DoScriptText(SAY_LIGHT_OF_DAWN_OUTRO_5, pTirion);
                                m_uiEventTimer = 13000;
                            }
                            break;
                        case 11:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                DoScriptText(SAY_LIGHT_OF_DAWN_OUTRO_6, pTirion);
                                m_uiEventTimer = 13000;
                            }
                            break;
                        case 12:
                            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                            DoScriptText(SAY_LIGHT_OF_DAWN_OUTRO_7, m_creature);
                            m_uiEventTimer = 7000;
                            break;
                        case 13:
                            // start Alexandros vision
                            if (Creature* pAlexandros = m_creature->SummonCreature(NPC_HIGHLORD_ALEXANDROS_MOGRAINE, aEventLocations[4].m_fX, aEventLocations[4].m_fY, aEventLocations[4].m_fZ, aEventLocations[4].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000))
                            {
                                DoScriptText(EMOTE_LIGHT_OF_DAWN_ALEXANDROS, pAlexandros);
                                pAlexandros->CastSpell(pAlexandros, SPELL_ALEXANDROS_MOGRAINE_SPAWN, true);
                            }
                            m_uiEventTimer = 4000;
                            break;
                        case 14:
                            if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                            {
                                pAlexandros->GetMotionMaster()->MovePoint(POINT_MOVE_OTHER, aEventLocations[5].m_fX, aEventLocations[5].m_fY, aEventLocations[5].m_fZ);
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_1, pAlexandros);
                                m_creature->SetFacingToObject(pAlexandros);
                            }
                            m_uiEventTimer = 2000;
                            break;
                        case 15:
                            DoScriptText(SAY_LIGHT_OF_DAWN_VISION_2, m_creature);
                            m_uiEventTimer = 4000;
                            break;
                        case 16:
                            DoScriptText(SAY_LIGHT_OF_DAWN_VISION_3, m_creature);
                            m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                            // summon young Darion for 1 min
                            if (Creature* pDarion = m_creature->SummonCreature(NPC_DARION_MOGRAINE, aEventLocations[6].m_fX, aEventLocations[6].m_fY, aEventLocations[6].m_fZ, aEventLocations[6].m_fO, TEMPSUMMON_TIMED_DESPAWN, 1*MINUTE*IN_MILLISECONDS))
                                DoScriptText(EMOTE_LIGHT_OF_DAWN_SHADE, pDarion);
                            m_uiEventTimer = 3000;
                            break;
                        case 17:
                            if (Creature* pDarion = m_pInstance->GetSingleCreatureFromStorage(NPC_DARION_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_4, pDarion);
                            m_uiEventTimer = 3000;
                            break;
                        case 18:
                            if (Creature* pDarion = m_pInstance->GetSingleCreatureFromStorage(NPC_DARION_MOGRAINE))
                            {
                                pDarion->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pDarion->GetMotionMaster()->MovePoint(POINT_MOVE_CHAPEL, aEventLocations[7].m_fX, aEventLocations[7].m_fY, aEventLocations[7].m_fZ);
                            }
                            m_uiEventTimer = 5000;
                            break;
                        case 19:
                            if (Creature* pDarion = m_pInstance->GetSingleCreatureFromStorage(NPC_DARION_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_5, pDarion);
                            m_uiEventTimer = 5000;
                            break;
                        case 20:
                            if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_6, pAlexandros);
                            m_uiEventTimer = 8000;
                            break;
                        case 21:
                            if (Creature* pDarion = m_pInstance->GetSingleCreatureFromStorage(NPC_DARION_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_7, pDarion);
                            m_uiEventTimer = 8000;
                            break;
                        case 22:
                            if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_8, pAlexandros);

                            // move Tirion to the point where the light of dawn is
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                pTirion->SetSplineFlags(SPLINEFLAG_WALKMODE);
                                if (GameObject* pLight = m_pInstance->GetSingleGameObjectFromStorage(GO_LIGHT_OF_DAWN))
                                    pTirion->GetMotionMaster()->MovePoint(POINT_MOVE_OTHER, pLight->GetPositionX(), pLight->GetPositionY(), pLight->GetPositionZ());
                            }
                            m_uiEventTimer = 15000;
                            break;
                        case 23:
                            if (Creature* pDarion = m_pInstance->GetSingleCreatureFromStorage(NPC_DARION_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_9, pDarion);
                            m_uiEventTimer = 11000;
                            break;
                        case 24:
                            if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_10, pAlexandros);
                            m_uiEventTimer = 29000;
                            break;
                        case 25:
                            if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                                DoScriptText(SAY_LIGHT_OF_DAWN_VISION_11, pAlexandros);
                            m_uiEventTimer = 6000;
                            break;
                        case 26:
                            // Lich king visit
                            if (Creature* pLichKing = m_creature->SummonCreature(NPC_THE_LICH_KING, aEventLocations[8].m_fX, aEventLocations[8].m_fY, aEventLocations[8].m_fZ, aEventLocations[8].m_fO, TEMPSUMMON_MANUAL_DESPAWN, 0))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_1, pLichKing);
                            if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                                DoScriptText(EMOTE_LIGHT_OF_DAWN_LICH_KING, pAlexandros);
                            m_uiEventTimer = 2000;
                            break;
                        case 27:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_2, pLichKing);
                                if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                                    pLichKing->CastSpell(pAlexandros, SPELL_SOUL_FEAST_ALEX, false);
                            }
                            m_uiEventTimer = 2000;
                            break;
                        case 28:
                            if (Creature* pAlexandros = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_ALEXANDROS_MOGRAINE))
                                pAlexandros->ForcedDespawn();
                            m_uiEventTimer = 2000;
                            break;
                        case 29:
                            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                            DoScriptText(EMOTE_LIGHT_OF_DAWN_ANGRY, m_creature);
                            DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_3, m_creature);
                            m_uiEventTimer = 3000;
                            break;
                        case 30:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                pLichKing->CastSpell(pLichKing, SPELL_ICEBOUND_VISAGE, true);
                                pLichKing->GetMotionMaster()->MovePoint(POINT_MOVE_CHAPEL, aEventLocations[9].m_fX, aEventLocations[9].m_fY, aEventLocations[9].m_fZ);
                            }
                            m_uiEventTimer = 5000;
                            break;
                        case 31:
                            DoCastSpellIfCan(m_creature, SPELL_MOGRAINE_CHARGE);
                            m_uiEventTimer = 3000;
                            break;
                        case 32:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_4, pLichKing);
                                pLichKing->CastSpell(m_creature, SPELL_APOCALYPSE, true);
                            }
                            // workaround because kick doesn't work
                            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                            m_creature->GetMap()->CreatureRelocation(m_creature, aEventLocations[10].m_fX, aEventLocations[10].m_fY, aEventLocations[10].m_fZ, aEventLocations[10].m_fO);
                            m_creature->SendMonsterMove(aEventLocations[10].m_fX, aEventLocations[10].m_fY, aEventLocations[10].m_fZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                            m_uiEventTimer = 4000;
                            break;
                        case 33:
                            m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                m_creature->SetFacingToObject(pLichKing);
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_5, pTirion);
                            m_uiEventTimer = 8000;
                            break;
                        case 34:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_6, pLichKing);
                            m_uiEventTimer = 15000;
                            break;
                        case 35:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_7, pLichKing);
                            m_uiEventTimer = 17000;
                            break;
                        case 36:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                DoScriptText(EMOTE_LIGHT_OF_DAWN_CAST_SPELL, pLichKing);
                                if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                {
                                    DoScriptText(EMOTE_LIGHT_OF_DAWN_GRASP, pTirion);
                                    pLichKing->CastSpell(pLichKing, SPELL_SOUL_FEAST_TIRION, true);
                                }
                            }
                            m_uiEventTimer = 2000;
                            break;
                        case 37:
                            if (Creature* pMaxwell = m_pInstance->GetSingleCreatureFromStorage(NPC_LORD_MAXWELL_TYROSUS))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_8, pMaxwell);
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                /*for (GUIDList::const_iterator itr = lDefendersGUIDs.begin(); itr != lDefendersGUIDs.end(); ++itr)
                                {
                                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                                    {
                                        pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                                        pTemp->RemoveSplineflag(SPLINEFLAG_WALKMODE);
                                        pTemp->AI()->AttackStart(pLichKing);
                                        pTemp->GetMotionMaster()->MovePoint(POINT_MOVE_OTHER, aEventLocations[13].m_fX, aEventLocations[13].m_fY, aEventLocations[13].m_fZ
                                    }
                                }*/
                                for (uint8 i = 0; i < MAX_LIGHT_CHAMPIONS; i++)
                                {
                                    if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(aLightArmySpawnLoc[i].m_uiEntry))
                                    {
                                        pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                                        pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                        pTemp->AI()->AttackStart(pLichKing);
                                        pTemp->GetMotionMaster()->MovePoint(POINT_MOVE_OTHER, aEventLocations[13].m_fX, aEventLocations[13].m_fY, aEventLocations[13].m_fZ);
                                    }
                                }
                            }
                            m_uiEventTimer = 6000;
                            break;
                        case 38:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                DoScriptText(EMOTE_LIGHT_OF_DAWN_POWERFULL, pLichKing);
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_9, pLichKing);
                                pLichKing->CastSpell(pLichKing, SPELL_APOCALYPSE, true);
                            }
                            // despawn guards
                            /*for (GUIDList::const_iterator itr = lDefendersGUIDs.begin(); itr != lDefendersGUIDs.end(); ++itr)
                            {
                                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                                    pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            }*/
                            for (uint8 i = 0; i < MAX_LIGHT_CHAMPIONS; i++)
                            {
                                if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(aLightArmySpawnLoc[i].m_uiEntry))
                                {
                                    uint32 uiPoint = urand(0, 1) ? 14 : 15;
                                    pTemp->SetStandState(UNIT_STAND_STATE_DEAD);
                                    pTemp->CombatStop();
                                    pTemp->GetRandomPoint(aEventLocations[uiPoint].m_fX, aEventLocations[uiPoint].m_fY, aEventLocations[uiPoint].m_fZ, 10.0f, fX, fY, fZ);
                                    pTemp->GetMap()->CreatureRelocation(m_creature, fX, fY, fZ, aEventLocations[10].m_fO);
                                    pTemp->SendMonsterMove(fX, fY, fZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                                }
                            }
                            m_uiEventTimer = 5000;
                            break;
                        case 39:
                            for (uint8 i = 0; i < MAX_LIGHT_CHAMPIONS; i++)
                            {
                                if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(aLightArmySpawnLoc[i].m_uiEntry))
                                    pTemp->SetStandState(UNIT_STAND_STATE_STAND);
                            }
                            m_uiEventTimer = 1000;
                            break;
                        case 40:
                            DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_10, m_creature);
                            m_uiEventTimer = 5000;
                            break;
                        case 41:
                            DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_11, m_creature);
                            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                            m_uiEventTimer = 1000;
                            break;
                        case 42:
                            DoScriptText(EMOTE_LIGHT_OF_DAWN_ASHBRINGER, m_creature);
                            DoCastSpellIfCan(m_creature, SPELL_ASHBRINGER);
                            m_uiEventTimer = 5000;
                            break;
                        case 43:
                            DoScriptText(EMOTE_LIGHT_OF_DAWN_COLAPSE, m_creature);
                            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                pTirion->CastSpell(pTirion, SPELL_REBIRTH_OF_THE_ASHBRINGER, true);
                            m_pInstance->DoRespawnGameObject(GO_LIGHT_OF_DAWN, 5*MINUTE);
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                pLichKing->CastStop(SPELL_ICEBOUND_VISAGE);
                                pLichKing->RemoveAllAuras();
                            }
                            m_uiEventTimer = 2000;
                            break;
                        case 44:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                if (pTirion->HasAura(SPELL_REBIRTH_OF_THE_ASHBRINGER))
                                    pTirion->RemoveAurasDueToSpell(SPELL_REBIRTH_OF_THE_ASHBRINGER);
                                pTirion->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                            }
                            m_uiEventTimer = 2500;
                            break;
                        case 45:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_12, pTirion);
                            m_uiEventTimer = 4000;
                            break;
                        case 46:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_13, pLichKing);
                            m_uiEventTimer = 5000;
                            break;
                        case 47:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_14, pTirion);
                            m_uiEventTimer = 1000;
                            break;
                        case 48:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                DoScriptText(EMOTE_LIGHT_OF_DAWN_CHARGE, pTirion);
                                pTirion->CastSpell(pTirion, SPELL_TIRION_CHARGE, true);
                            }
                            m_uiEventTimer = 2000;
                            break;
                        case 49:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_15, pLichKing);
                                // make not attack
                                pLichKing->CombatStop();
                                pLichKing->DeleteThreatList();
                            }
                            m_uiEventTimer = 1000;
                            break;
                        case 50:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                pLichKing->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                            m_uiEventTimer = 3000;
                            break;
                        case 51:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_16, pLichKing);
                            m_uiEventTimer = 10000;
                            break;
                        case 52:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_KING_VISIT_17, pLichKing);
                            m_uiEventTimer = 10000;
                            break;
                        case 53:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                                pLichKing->CastSpell(pLichKing, SPELL_TELEPORT_VISUAL, false);
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                pTirion->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pTirion->GetMotionMaster()->MovePoint(POINT_MOVE_OTHER, aEventLocations[11].m_fX, aEventLocations[11].m_fY, aEventLocations[11].m_fZ);
                            }
                            m_uiEventTimer = 2000;
                            break;
                        case 54:
                            if (Creature* pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_THE_LICH_KING))
                            {
                                DoScriptText(EMOTE_LIGHT_OF_DAWN_KING_LEAVE, pLichKing);
                                pLichKing->ForcedDespawn();
                            }
                            m_uiEventTimer = 7000;
                            break;
                        case 55:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                pTirion->CastSpell(m_creature, SPELL_LAY_ON_HANDS, true);
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_1, pTirion);
                            }
                            m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                            m_uiEventTimer = 3000;
                            break;
                        case 56:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                pTirion->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTirion->GetMotionMaster()->MovePoint(POINT_MOVE_OTHER, aEventLocations[12].m_fX, aEventLocations[12].m_fY, aEventLocations[12].m_fZ);
                            }
                            m_uiEventTimer = 5000;
                            break;
                        case 57:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                            {
                                pTirion->SetFacingToObject(m_creature);
                                m_creature->SetFacingToObject(pTirion);
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_2, pTirion);
                            }
                            m_uiEventTimer = 15000;
                            break;
                        case 58:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_3, pTirion);
                            m_uiEventTimer = 7000;
                            break;
                        case 59:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_4, pTirion);
                            m_uiEventTimer = 10000;
                            break;
                        case 60:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_5, pTirion);
                            m_uiEventTimer = 11000;
                            break;
                        case 61:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_6, pTirion);
                            m_uiEventTimer = 10000;
                            break;
                        case 62:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_7, pTirion);
                            m_uiEventTimer = 8000;
                            break;
                        case 63:
                            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
                                DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_8, pTirion);
                            m_uiEventTimer = 10000;
                            break;
                        case 64:
                            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                            DoScriptText(SAY_LIGHT_OF_DAWN_EPILOGUE_9, m_creature);
                            m_uiEventTimer = 10000;
                            break;
                        case 65:
                            // send credit then in 5 min despawn
                            DoSendQuestCredit();
                            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                            m_creature->ForcedDespawn(5*MINUTE*IN_MILLISECONDS);
                            m_uiEventTimer = 0;
                            break;
                    }

                    ++m_uiEventStep;
                }
                else
                    m_uiEventTimer -= uiDiff;
            }

            // Handle battle events
            if (m_uiFightTimer)
            {
                if (m_uiFightTimer <= uiDiff || m_uiLightWarriorsDead >= 100)
                {
                    // summon Tirion and move him to the chapel
                    if (Creature* pTirion = m_creature->SummonCreature(NPC_HIGHLORD_TIRION_FORDRING, aEventLocations[0].m_fX, aEventLocations[0].m_fY, aEventLocations[0].m_fZ, aEventLocations[0].m_fO, TEMPSUMMON_MANUAL_DESPAWN, 0))
                    {
                        pTirion->CastSpell(pTirion, SPELL_THE_LIGHT_OF_DAWN_AURA, true);
                        DoScriptText(SAY_LIGHT_OF_DAWN_OUTRO_1, pTirion);
                        DoScriptText(EMOTE_LIGHT_OF_DAWN_TIRION, pTirion);
                        pTirion->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        pTirion->GetMotionMaster()->MovePoint(POINT_MOVE_CHAPEL, aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ);
                    }
                    m_uiFightTimer = 0;
                }
                else
                    m_uiFightTimer -= uiDiff;

                 // we need to make sure the npcs
                if (m_uiAttackTimer < uiDiff)
                {
                    if (m_pInstance)
                        m_pInstance->DoChangeArmyTargets();

                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        m_creature->AI()->AttackStart(pTarget);
                        m_creature->SetInCombatWith(pTarget);
                        pTarget->SetInCombatWith(m_creature);
                    }

                    if (Creature* pKoltira = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLTIRA_DEATHWEAVER))
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pKoltira->AI()->AttackStart(pTarget);
                            pKoltira->SetInCombatWith(pTarget);
                            pTarget->SetInCombatWith(pKoltira);
                        }
                    }
                    if (Creature* pThassarian = m_pInstance->GetSingleCreatureFromStorage(NPC_THASSARIAN))
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pThassarian->AI()->AttackStart(pTarget);
                            pThassarian->SetInCombatWith(pTarget);
                            pTarget->SetInCombatWith(pThassarian);
                        }
                    }
                    if (Creature* pOrbaz = m_pInstance->GetSingleCreatureFromStorage(NPC_ORBAZ_BLOODBANE))
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pOrbaz->AI()->AttackStart(pTarget);
                            pOrbaz->SetInCombatWith(pTarget);
                            pTarget->SetInCombatWith(pOrbaz);
                        }
                    }

                    for(GUIDList::const_iterator itr = lAttackersGUIDs.begin(); itr != lAttackersGUIDs.end(); ++itr)
                    {
                        if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        {
                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                pTemp->AI()->AttackStart(pTarget);
                                pTemp->SetInCombatWith(pTarget);
                                pTarget->SetInCombatWith(pTemp);
                            }
                        }
                    }
                    m_uiAttackTimer = 5000;
                }
                else
                    m_uiAttackTimer -= uiDiff;

                // battle sounds
                if (m_uiFightSpeechTimer < uiDiff)
                {
                    switch(urand(0, 15))
                    {
                        case 0:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_1, m_creature);  break;
                        case 1:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_2, m_creature);  break;
                        case 2:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_3, m_creature);  break;
                        case 3:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_4, m_creature);  break;
                        case 4:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_5, m_creature);  break;
                        case 5:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_6, m_creature);  break;
                        case 6:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_7, m_creature);  break;
                        case 7:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_8, m_creature);  break;
                        case 8:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_9, m_creature);  break;
                        case 9:  DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_10, m_creature); break;
                        case 10: DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_11, m_creature); break;
                        case 11: DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_12, m_creature); break;
                        case 12: DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_13, m_creature); break;
                        case 13: DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_14, m_creature); break;
                        case 14: DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_15, m_creature); break;
                        case 15: DoScriptText(SAY_LIGHT_OF_DAWN_BATTLE_16, m_creature); break;
                    }
                    m_uiFightSpeechTimer = urand(15000, 20000);
                }
                else
                    m_uiFightSpeechTimer -= uiDiff;

                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                // make sure that darion always stays in the area
                if (!m_creature->IsWithinDist2d(aEventLocations[1].m_fX, aEventLocations[1].m_fY, 50.0f))
                    m_creature->GetMotionMaster()->MovePoint(0, aEventLocations[1].m_fX, aEventLocations[1].m_fY, aEventLocations[1].m_fZ);

                // Darion spells
                if (m_uiAntimagicZoneTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_ANTI_MAGIC_ZONE_DARION) == CAST_OK)
                        m_uiAntimagicZoneTimer = urand(25000, 30000);
                }
                else
                    m_uiAntimagicZoneTimer -= uiDiff;

                if (m_uiDeathStrikeTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEATH_STRIKE) == CAST_OK)
                        m_uiDeathStrikeTimer = urand(5000, 10000);
                }
                else
                    m_uiDeathStrikeTimer -= uiDiff;

                if (m_uiDeathEmbraceTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEATH_EMBRACE) == CAST_OK)
                        m_uiDeathEmbraceTimer = urand(5000, 10000);
                }
                else
                    m_uiDeathEmbraceTimer -= uiDiff;

                if (m_uiIcyTouchTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ICY_TOUCH_DARION) == CAST_OK)
                        m_uiIcyTouchTimer = urand(5000, 10000);
                }
                else
                    m_uiIcyTouchTimer -= uiDiff;

                if (m_uiUnholyBlightTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_UNHOLY_BLIGHT) == CAST_OK)
                        m_uiUnholyBlightTimer = urand(5000, 10000);
                }
                else
                    m_uiUnholyBlightTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        }
    }
};

bool GossipHello_npc_highlord_darion_mograine(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(QUEST_ID_LIGHT_OF_DAWN) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_READY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_highlord_darion_mograine(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction ==  GOSSIP_ACTION_INFO_DEF + 1)
    {
        if (world_map_ebon_hold* pInstance = (world_map_ebon_hold*)pCreature->GetInstanceData())
        {
            // set data to special in order to start the event
            pInstance->SetData(TYPE_BATTLE, SPECIAL);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            return true;
        }
    }
    pPlayer->CLOSE_GOSSIP_MENU();

    return false;
}

CreatureAI* GetAI_npc_highlord_darion_mograine(Creature* pCreature)
{
    return new npc_highlord_darion_mograineAI(pCreature);
}

struct MANGOS_DLL_DECL npc_fellow_death_knightAI : public ScriptedAI
{
    npc_fellow_death_knightAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (world_map_ebon_hold*)pCreature->GetInstanceData();
        Reset();
    }

    world_map_ebon_hold* m_pInstance;

    uint32 m_uiIcyTouchTimer;
    uint32 m_uiBloodStrikeTimer;
    uint32 m_uiPlagueStrikeTimer;

    void Reset()
    {
            m_uiBloodStrikeTimer    = urand(5000, 10000);
            m_uiIcyTouchTimer       = urand(5000, 10000);
            m_uiPlagueStrikeTimer   = urand(5000, 10000);
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_HERO_AGGRO_AURA);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE || uiPointId != POINT_MOVE_CHAPEL)
            return;

        // make the death knights kneel
        if (m_creature->HasAura(SPELL_THE_LIGHT_OF_DAWN))
            m_creature->RemoveAurasDueToSpell(SPELL_THE_LIGHT_OF_DAWN);

        m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);

        if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_TIRION_FORDRING))
            m_creature->SetFacingToObject(pTirion);
    }

    void EnterEvadeMode()
    {
        if (!m_pInstance)
            return;

        // if evade while the battle is in progress start attacking another target
        if (m_pInstance->GetData(TYPE_BATTLE) == IN_PROGRESS)
        {
            if (Creature* pDarion = m_pInstance->GetSingleCreatureFromStorage(NPC_HIGHLORD_DARION_MOGRAINE))
            {
                if (Unit* pTarget = pDarion->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->AI()->AttackStart(pTarget);
            }
        }
        else
        {
            if (m_creature->GetEntry() != NPC_ORBAZ_BLOODBANE)
            {
                // cast light of dawn
                m_creature->Unmount();
                m_creature->CastSpell(m_creature, SPELL_THE_LIGHT_OF_DAWN, true);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
            }

            // move to chapel points
            switch(m_creature->GetEntry())
            {
                case NPC_THASSARIAN:
                    m_creature->GetMotionMaster()->MovePoint(POINT_MOVE_CHAPEL, aEventLocations[3].m_fX, aEventLocations[3].m_fY, aEventLocations[3].m_fZ);
                    break;
                case NPC_KOLTIRA_DEATHWEAVER:
                    m_creature->GetMotionMaster()->MovePoint(POINT_MOVE_CHAPEL, aEventLocations[2].m_fX, aEventLocations[2].m_fY, aEventLocations[2].m_fZ);
                    break;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPlagueStrikeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_PLAGUE_STRIKE_KNIGHTS) == CAST_OK)
                m_uiPlagueStrikeTimer = urand(5000, 10000);
        }
        else
            m_uiPlagueStrikeTimer -= uiDiff;

        if (m_uiIcyTouchTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ICY_TOUCH_DARION) == CAST_OK)
                m_uiIcyTouchTimer = urand(5000, 10000);
        }
        else
            m_uiIcyTouchTimer -= uiDiff;

        if (m_uiBloodStrikeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLOOD_STRIKE) == CAST_OK)
                m_uiBloodStrikeTimer = urand(5000, 10000);
        }
        else
            m_uiBloodStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_fellow_death_knight(Creature* pCreature)
{
    return new npc_fellow_death_knightAI(pCreature);
}

void AddSC_ebon_hold()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_a_special_surprise";
    pNewScript->GetAI = &GetAI_npc_a_special_surprise;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_death_knight_initiate";
    pNewScript->GetAI = &GetAI_npc_death_knight_initiate;
    pNewScript->pGossipHello = &GossipHello_npc_death_knight_initiate;
    pNewScript->pGossipSelect = &GossipSelect_npc_death_knight_initiate;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_koltira_deathweaver";
    pNewScript->GetAI = &GetAI_npc_koltira_deathweaver;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_koltira_deathweaver;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_unworthy_initiate";
    pNewScript->GetAI = &GetAI_npc_unworthy_initiate;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_unworthy_initiate_anchor";
    pNewScript->GetAI = &GetAI_npc_unworthy_initiate_anchor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_acherus_soul_prison";
    pNewScript->pGOUse = &GOUse_go_acherus_soul_prison;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_eye_of_acherus";
    pNewScript->GetAI = &GetAI_npc_eye_of_acherus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_highlord_darion_mograine";
    pNewScript->GetAI = &GetAI_npc_highlord_darion_mograine;
    pNewScript->pGossipHello =  &GossipHello_npc_highlord_darion_mograine;
    pNewScript->pGossipSelect = &GossipSelect_npc_highlord_darion_mograine;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_fellow_death_knight";
    pNewScript->GetAI = &GetAI_npc_fellow_death_knight;
    pNewScript->RegisterSelf();
}
