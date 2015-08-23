/*
 *  The ManaPlus Client
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

#include "net/eathena/admin.h"

#include "logger.h"

#include "being/being.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

namespace EAthena
{

void Admin::processAdminGetLoginAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readString(24, "login");
}

void Admin::processSetTileType(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ here need set collision tile for map
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt16("type");
    msg.readString(16, "map name");
}

void Admin::processAccountStats(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ need show in other players stats window, nick in mStatsName
    msg.readUInt8("str");
    msg.readUInt8("need str");
    msg.readUInt8("agi");
    msg.readUInt8("need agi");
    msg.readUInt8("vit");
    msg.readUInt8("need vit");
    msg.readUInt8("int");
    msg.readUInt8("need int");
    msg.readUInt8("dex");
    msg.readUInt8("need dex");
    msg.readUInt8("luk");
    msg.readUInt8("need luk");
    msg.readInt16("attack");
    msg.readInt16("refine");
    msg.readInt16("matk max");
    msg.readInt16("matk min");
    msg.readInt16("item def");
    msg.readInt16("plus def");
    msg.readInt16("mdef");
    msg.readInt16("plus mdef");
    msg.readInt16("hit");
    msg.readInt16("flee");
    msg.readInt16("flee2/10");
    msg.readInt16("cri/10");
    msg.readInt16("speed");
    msg.readInt16("zero");
}

}  // namespace EAthena