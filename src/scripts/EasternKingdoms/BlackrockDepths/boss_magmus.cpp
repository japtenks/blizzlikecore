/*
 * Copyright (C) 2011-2013 BlizzLikeGroup <http://blizzlike.servegame.com/>
 * Please, look at the CREDITS.md file.
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: Boss_Magmus
Complete(%): 100
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_FIERYBURST                                       = 13900,
    SPELL_WARSTOMP                                         = 24375
};

enum eEnums
{
    DATA_THRONE_DOOR                              = 24 // not id or guid of doors but number of enum in blackrock_depths.h
};

struct boss_magmusAI : public ScriptedAI
{
    boss_magmusAI(Creature* c) : ScriptedAI(c) {}

    uint32 FieryBurst_Timer;
    uint32 WarStomp_Timer;

    void Reset()
    {
        FieryBurst_Timer = 5000;
        WarStomp_Timer =0;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //FieryBurst_Timer
        if (FieryBurst_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_FIERYBURST);
            FieryBurst_Timer = 6000;
        } else FieryBurst_Timer -= diff;

        //WarStomp_Timer
        if (me->GetHealth()*100 / me->GetMaxHealth() < 51)
        {
            if (WarStomp_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_WARSTOMP);
                WarStomp_Timer = 8000;
            } else WarStomp_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
    // When he die open door to last chamber
    void JustDied(Unit* who)
    {
        if (ScriptedInstance* pInstance = who->GetInstanceData())
            pInstance->HandleGameObject(pInstance->GetData64(DATA_THRONE_DOOR), true);
    }
};
CreatureAI* GetAI_boss_magmus(Creature* pCreature)
{
    return new boss_magmusAI (pCreature);
}

void AddSC_boss_magmus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_magmus";
    newscript->GetAI = &GetAI_boss_magmus;
    newscript->RegisterSelf();
}
