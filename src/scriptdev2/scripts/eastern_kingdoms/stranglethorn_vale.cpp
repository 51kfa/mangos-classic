/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
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
SDName: Stranglethorn_Vale
SD%Complete: 100
SDComment: Quest support: 592
SDCategory: Stranglethorn Vale
EndScriptData */

/* ContentData
mob_yenniku
EndContentData */

#include "precompiled.h"
#include "World.h"
#include "common_ex.h"

/*######
## mob_yenniku
######*/

enum
{
    SPELL_YENNIKUS_RELEASE      = 3607,

    QUEST_ID_SAVING_YENNIKU     = 592,

    FACTION_ID_HORDE_GENERIC    = 83,                       // Note: faction may not be correct!
};

struct mob_yennikuAI : public ScriptedAI
{
    mob_yennikuAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiResetTimer;

    void Reset() override { m_uiResetTimer = 0; }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell) override
    {
        if (pSpell->Id == SPELL_YENNIKUS_RELEASE && pCaster->GetTypeId() == TYPEID_PLAYER)
        {
            if (!m_uiResetTimer && ((Player*)pCaster)->GetQuestStatus(QUEST_ID_SAVING_YENNIKU) == QUEST_STATUS_INCOMPLETE)
            {
                m_uiResetTimer = 60000;
                EnterEvadeMode();
            }
        }
    }

    void EnterEvadeMode() override
    {
        if (m_uiResetTimer)
        {
            m_creature->RemoveAllAurasOnEvade();
            m_creature->DeleteThreatList();
            m_creature->CombatStop(true);
            m_creature->LoadCreatureAddon(true);

            m_creature->SetLootRecipient(NULL);

            m_creature->HandleEmote(EMOTE_STATE_STUN);
            m_creature->SetFactionTemporary(FACTION_ID_HORDE_GENERIC, TEMPFACTION_RESTORE_REACH_HOME);
        }
        else
            ScriptedAI::EnterEvadeMode();
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiResetTimer)
        {
            if (m_uiResetTimer <= uiDiff)
            {
                m_creature->HandleEmote(EMOTE_STATE_NONE);
                m_uiResetTimer = 0;
                EnterEvadeMode();
            }
            else
                m_uiResetTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_yenniku(Creature* _Creature)
{
    return new mob_yennikuAI(_Creature);
}

/*######
## Night time event where the creatures sleep
## Only certain tigers and panthers actually have a sleeping animation/stance
## 
## TIGERS: Stranglethorn Tigress (772), Stranglethorn Tiger (682),  Young Stranglethorn Tiger (681),  Elder Stranglethorn Tiger (1085) 
## PANTHERS: Shadowmaw Panther (684), Elder Shadowmaw Panther (1713),  Young Panther (683),  Panther (736) 
######*/

enum
{
	START_SLEEPING = 21, // 24 hour clock, therefore 21 = 9pm
	END_SLEEPING = 5     // 5 = 5am
};

struct mob_sleeping_creature : public ScriptedAI
{
	mob_sleeping_creature(Creature* pCreature):ScriptedAI(pCreature){Reset();}
	uint16 uSleepTime;
	float fSpawnPositionX;
	float fSpawnPositionY;
	float fSpawnPositionZ;

	void Reset() override
	{
		uSleepTime = 0;
		fSpawnPositionX = m_creature->GetPositionX();
		fSpawnPositionY = m_creature->GetPositionY();
		fSpawnPositionZ = m_creature->GetPositionZ();
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->isInCombat())
		{
			time_t t = sWorld.GetGameTime();
			struct tm *tmp = gmtime(&t);
			if (tmp->tm_hour >= START_SLEEPING || tmp->tm_hour < END_SLEEPING)
			{
				Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
				for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
				{
					if (Player* pPlayer = itr->getSource())
					{
						if (pPlayer->isGameMaster())
							break;
						float fPlayerXposition = pPlayer->GetPositionX();
						float fPlayerYposition = pPlayer->GetPositionY();
						float fPlayerZposition = pPlayer->GetPositionZ();
						if (IsNearWaypoint(fPlayerXposition, fPlayerYposition, fPlayerZposition, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 4, 4, 4))
						{
							m_creature->SetStandState(UNIT_STAND_STATE_STAND);
							return;
						}
					}
				}
				if (uSleepTime < uiDiff)
				{
					uint32 uSpawn = rand() % 60;
					if (uSpawn >= 1 || uSpawn <= 40)
					{
						m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
						uint32 uDistance = 5 + rand() % 10;
						if (IsNearWaypoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), fSpawnPositionX, fSpawnPositionY, fSpawnPositionZ, 20, 20, 20))
						{
							m_creature->GetMotionMaster()->MoveRandomAroundPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), uDistance, 0);
						}
						else
						{
							m_creature->GetMotionMaster()->MovePoint(0, fSpawnPositionX, fSpawnPositionY, fSpawnPositionZ);
						}
					}
					uSleepTime = 600000 + rand() % 600000;
				}
				else
					uSleepTime -= uiDiff;
			}

			if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;
			m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
			if (m_creature->isAttackReady())
			{
				DoMeleeAttackIfReady();
				m_creature->resetAttackTimer();
			}
		}
	}
};

CreatureAI* GetAI_CreatureSleep(Creature* pCreature)
{
	return new mob_sleeping_creature(pCreature);
}

void AddSC_stranglethorn_vale()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mob_yenniku";
    pNewScript->GetAI = &GetAI_mob_yenniku;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_sleeping_creature";
	pNewScript->GetAI = &GetAI_CreatureSleep;
	pNewScript->RegisterSelf();
}
