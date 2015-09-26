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

#include "net/ea/npcrecv.h"

#include "gui/viewport.h"

#include "gui/windows/npcdialog.h"

#include "net/messagein.h"
#include "net/npchandler.h"

#include "utils/langs.h"

#include "debug.h"

namespace Ea
{

namespace NpcRecv
{
    NpcDialog *mDialog = nullptr;
    bool mRequestLang = false;
}  // namespace NpcRecv

void NpcRecv::processNpcChoice(Net::MessageIn &msg)
{
    msg.readInt16("len");
    npcHandler->getNpc(msg, NpcAction::Other);
    mRequestLang = false;

    if (mDialog)
    {
        mDialog->choiceRequest();
        mDialog->parseListItems(msg.readString(msg.getLength() - 8,
            "select items"));
    }
    else
    {
        msg.readString(msg.getLength() - 8, "select items");
    }
}

void NpcRecv::processNpcMessage(Net::MessageIn &msg)
{
    msg.readInt16("len");
    npcHandler->getNpc(msg, NpcAction::Other);
    mRequestLang = false;

    const std::string message = msg.readString(msg.getLength() - 8, "message");
    // ignore future legacy npc commands.
    if (message.size() > 3 && message.substr(0, 3) == "###")
        return;
    if (mDialog)
        mDialog->addText(message);
}

void NpcRecv::processNpcClose(Net::MessageIn &msg)
{
    // Show the close button
    npcHandler->getNpc(msg, NpcAction::Close);
    mRequestLang = false;
    if (mDialog)
        mDialog->showCloseButton();
}

void NpcRecv::processNpcNext(Net::MessageIn &msg)
{
    // Show the next button
    npcHandler->getNpc(msg, NpcAction::Next);
    mRequestLang = false;
    if (mDialog)
        mDialog->showNextButton();
}

void NpcRecv::processNpcIntInput(Net::MessageIn &msg)
{
    // Request for an integer
    npcHandler->getNpc(msg, NpcAction::Other);
    mRequestLang = false;
    if (mDialog)
        mDialog->integerRequest(0);
}

void NpcRecv::processNpcStrInput(Net::MessageIn &msg)
{
    // Request for a string
    BeingId npcId = npcHandler->getNpc(msg, NpcAction::Other);
    if (mRequestLang)
    {
        mRequestLang = false;
        npcHandler->stringInput(npcId, getLangSimple());
    }
    else if (mDialog)
    {
        mDialog->textRequest("");
    }
}

void NpcRecv::processNpcCommand(Net::MessageIn &msg)
{
    const BeingId npcId = npcHandler->getNpc(msg, NpcAction::Other);
    mRequestLang = false;

    const int cmd = msg.readInt16("cmd");
    const BeingId id = msg.readBeingId("id");
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    switch (cmd)
    {
        case 0:
            mRequestLang = true;
            break;

        case 1:
            if (viewport)
                viewport->moveCameraToActor(npcId);
            break;

        case 2:
            if (viewport)
            {
                if (id == BeingId_zero)
                    viewport->moveCameraToPosition(x, y);
                else
                    viewport->moveCameraToActor(id, x, y);
            }
            break;

        case 3:
            if (viewport)
                viewport->returnCamera();
            break;

        case 4:
            if (viewport)
            {
                viewport->moveCameraRelative(x, y);
            }
            break;
        case 5:  // close dialog
            if (mDialog)
                mDialog->restoreCamera();
            npcHandler->closeDialog(npcId);
            break;
        case 6:  // show avatar
            if (mDialog)
            {
                mDialog->showAvatar(fromInt(id, BeingTypeId));
            }
            break;
        case 7:  // set avatar direction
            if (mDialog)
            {
                mDialog->setAvatarDirection(
                    Net::MessageIn::fromServerDirection(
                    static_cast<uint8_t>(id)));
            }
            break;
        case 8:  // set avatar action
            if (mDialog)
                mDialog->setAvatarAction(toInt(id, int));
            break;
        case 9:  // clear npc dialog
            if (mDialog)
                mDialog->clearRows();
            break;
        case 10:  // send selected item id
        {
            int invSize = toInt(id, int);
            if (!invSize)
                invSize = 1;
            if (mDialog)
                mDialog->itemRequest(invSize);
            break;
        }
        case 11:  // send selected item index
        {
            int invSize = toInt(id, int);
            if (!invSize)
                invSize = 1;
            if (mDialog)
                mDialog->itemIndexRequest(invSize);
            break;
        }
        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void NpcRecv::processChangeTitle(Net::MessageIn &msg)
{
    msg.readInt16("len");
    npcHandler->getNpc(msg, NpcAction::Other);
    mRequestLang = false;
    const std::string str = msg.readString(-1, "title");
    if (mDialog)
        mDialog->setCaption(str);
}

}  // namespace Ea