/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NET_EATHENA_BEING_H
#define NET_EATHENA_BEING_H

#include "net/ea/beinghandler.h"

#include "net/eathena/beingtype.h"
#include "net/eathena/messagehandler.h"

namespace EAthena
{
    namespace BeingNet
    {
        void processBeingChangeLook2(Net::MessageIn &msg);
        void processBeingChangeLookCards(Net::MessageIn &msg);
        void processBeingVisible(Net::MessageIn &msg);
        void processBeingMove(Net::MessageIn &msg);
        void processBeingSpawn(Net::MessageIn &msg);
        void processMapTypeProperty(Net::MessageIn &msg);
        void processMapType(Net::MessageIn &msg);
        void processSkillCasting(Net::MessageIn &msg);
        void processBeingStatusChange(Net::MessageIn &msg);
        void processBeingStatusChange2(Net::MessageIn &msg);
        void processBeingMove2(Net::MessageIn &msg);
        void processBeingAction2(Net::MessageIn &msg);
        void processMonsterHp(Net::MessageIn &msg);
        void processSkillAutoCast(Net::MessageIn &msg);
        void processRanksList(Net::MessageIn &msg);
        void processBeingChangeDirection(Net::MessageIn &msg);
        void processBeingChangeLookContinue(Net::MessageIn &msg,
                                            Being *const dstBeing,
                                            const uint8_t type,
                                            const int id,
                                            const int id2,
                                            const int *cards) A_NONNULL(2);
        void processBeingSpecialEffect(Net::MessageIn &msg);
        void processBeingSpecialEffectNum(Net::MessageIn &msg);
        void processBeingSoundEffect(Net::MessageIn &msg);
        void processSkillGroundNoDamage(Net::MessageIn &msg);
        void processSkillEntry(Net::MessageIn &msg);
        void processPlaterStatusChange(Net::MessageIn &msg);
        void processPlaterStatusChangeNoTick(Net::MessageIn &msg);
        void processBeingResurrect(Net::MessageIn &msg);
        void processPlayerGuilPartyInfo(Net::MessageIn &msg);
        void processBeingRemoveSkil(Net::MessageIn &msg);
        void processBeingFakeName(Net::MessageIn &msg);
        void processBeingStatUpdate1(Net::MessageIn &msg);
        void processPlaterStatusChange2(Net::MessageIn &msg);
        void processBeingSelfEffect(Net::MessageIn &msg);
        void processMobInfo(Net::MessageIn &msg);
        void processBeingAttrs(Net::MessageIn &msg);
        void processMonsterInfo(Net::MessageIn &msg);
        void processClassChange(Net::MessageIn &msg);
        void processSpiritBalls(Net::MessageIn &msg);
        void processSpiritBallSingle(Net::MessageIn &msg);
        void processBladeStop(Net::MessageIn &msg);
        void processComboDelay(Net::MessageIn &msg);
        void processWddingEffect(Net::MessageIn &msg);
        void processBeingSlide(Net::MessageIn &msg);
        void processStarsKill(Net::MessageIn &msg);
        void processBlacksmithRanksList(Net::MessageIn &msg);
        void processAlchemistRanksList(Net::MessageIn &msg);
        void processTaekwonRanksList(Net::MessageIn &msg);
        void processPkRanksList(Net::MessageIn &msg);
        void processGladiatorFeelRequest(Net::MessageIn &msg);
        void processBossMapInfo(Net::MessageIn &msg);
        void processBeingFont(Net::MessageIn &msg);
        void processBeingMilleniumShield(Net::MessageIn &msg);
        void processBeingCharm(Net::MessageIn &msg);
        void processBeingViewEquipment(Net::MessageIn &msg);
        void processPvpSet(Net::MessageIn &msg);
        void processNameResponse2(Net::MessageIn &msg);
        Being *createBeing2(Net::MessageIn &msg,
                            const BeingId id,
                            const int16_t job,
                            const BeingType::BeingType beingType);
    }  // namespace Being
}  // namespace EAthena

#endif  // NET_EATHENA_BEING_H