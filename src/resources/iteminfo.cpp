/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "resources/iteminfo.h"

#include "resources/itemdb.h"
#include "configuration.h"

#include "utils/dtor.h"

#include <set>
#include <map>

#include "debug.h"


ItemInfo::ItemInfo(ItemInfo &info)
{
    mType = info.mType;
    mWeight = info.mWeight;
    mView = info.mView;
    mId = info.mId;
    mDrawBefore = info.mDrawBefore;
    mDrawAfter = info.mDrawAfter;
    mDrawPriority = info.mDrawPriority;
    mIsRemoveSprites = info.mIsRemoveSprites;
    mAttackAction = info.mAttackAction;
    mAttackRange = info.mAttackRange;
    mColors = info.mColors;
    mColorList = info.mColorList;
    mHitEffectId = info.mHitEffectId;
    mCriticalHitEffectId = info.mCriticalHitEffectId;
    for (int f = 0; f < 9; f ++)
        mSpriteToItemReplaceMap[f] = 0;
}

ItemInfo::ItemInfo() :
    mType(ITEM_UNUSABLE),
    mWeight(0),
    mView(0),
    mId(0),
    mDrawBefore(-1),
    mDrawAfter(-1),
    mDrawPriority(0),
    mIsRemoveSprites(false),
    mAttackAction(SpriteAction::INVALID),
    mAttackRange(0),
    mColors(0),
    mColorList(""),
    mHitEffectId(0),
    mCriticalHitEffectId(0)
{
    for (int f = 0; f < 9; f ++)
        mSpriteToItemReplaceMap[f] = 0;
}

ItemInfo::~ItemInfo()
{
    delete_all(mSpriteToItemReplaceList);
    mSpriteToItemReplaceList.clear();
    for (int f = 0; f < 9; f ++)
        mSpriteToItemReplaceMap[f] = 0;
}

const std::string &ItemInfo::getSprite(Gender gender) const
{
    if (mView)
    {
        // Forward the request to the item defining how to view this item
        return ItemDB::get(mView).getSprite(gender);
    }
    else
    {
        static const std::string empty = "";
        std::map<int, std::string>::const_iterator i =
            mAnimationFiles.find(gender);

        return (i != mAnimationFiles.end()) ? i->second : empty;
    }
}

void ItemInfo::setAttackAction(std::string attackAction)
{
    if (attackAction.empty())
        mAttackAction = SpriteAction::ATTACK; // (Equal to unarmed animation)
    else
        mAttackAction = attackAction;
}

void ItemInfo::addSound(EquipmentSoundEvent event, const std::string &filename)
{
    mSounds[event].push_back(paths.getStringValue("sfx") + filename);
}

const std::string &ItemInfo::getSound(EquipmentSoundEvent event) const
{
    static const std::string empty;
    std::map< EquipmentSoundEvent,
        std::vector<std::string> >::const_iterator i;

    i = mSounds.find(event);

    if (i == mSounds.end())
        return empty;
    return i->second.size() > 0 ? i->second[rand() % i->second.size()] : empty;
}

std::map<int, int> *ItemInfo::addReplaceSprite(int sprite, int direction)
{
    if (direction == -1)
    {
        SpriteToItemMap *spMap = new SpriteToItemMap();
        for (int f = 0; f < 9; f ++)
        {
            if (!mSpriteToItemReplaceMap[f])
            {
                mSpriteToItemReplaceMap[f] = spMap;
                direction = f;
            }
        }
        if (direction >= 0)
            mSpriteToItemReplaceList.push_back(spMap);
        else
            delete spMap;
    }

    if (direction < 0 || direction >= 9)
        return 0;

    SpriteToItemMap *spMap = mSpriteToItemReplaceMap[direction];

    if (!spMap)
    {
        spMap = new SpriteToItemMap();
        mSpriteToItemReplaceMap[direction] = spMap;
        mSpriteToItemReplaceList.push_back(spMap);
    }

    SpriteToItemMap::iterator it = spMap->find(sprite);
    if (it == spMap->end())
    {
        std::map<int, int> tmp;
        (*mSpriteToItemReplaceMap[direction])[sprite] = tmp;
        it = mSpriteToItemReplaceMap[direction]->find(sprite);
    }
    return &it->second;
}

void ItemInfo::setColorsList(std::string name)
{
    if (name.empty())
    {
        mColors = 0;
        mColorList = "";
    }
    else
    {
        mColors = ColorDB::getColorsList(name);
        mColorList = name;
    }
}

std::string ItemInfo::getDyeColorsString(int color) const
{
    if (!mColors || mColorList.empty())
        return "";

    std::map <int, ColorDB::ItemColor>::iterator it = mColors->find(color);
    if (it == mColors->end())
        return "";

    return it->second.color;
}

const std::string ItemInfo::getDescription(unsigned char color) const
{
    return replaceColors(mDescription, color);
}

const std::string ItemInfo::getName(unsigned char color) const
{
    return replaceColors(mName, color);
}

const std::string ItemInfo::replaceColors(std::string str,
                                          unsigned char color) const
{
    std::string name;
    if (mColors && !mColorList.empty())
    {
        std::map <int, ColorDB::ItemColor>::iterator it = mColors->find(color);
        if (it == mColors->end())
            name = "unknown";
        else
            name = it->second.name;
    }
    else
    {
        name = "unknown";
    }

    str = replaceAll(str, "%color%", name);
    if (name.size() > 0)
        name[0] = static_cast<char>(toupper(name[0]));

    return replaceAll(str, "%Color%", name);
}

SpriteToItemMap *ItemInfo::getSpriteToItemReplaceMap(int direction) const
{
    if (direction < 0 || direction >= 9)
        return 0;

    SpriteToItemMap *spMap = mSpriteToItemReplaceMap[direction];
    if (spMap)
        return spMap;
    if (direction == DIRECTION_UPLEFT || direction == DIRECTION_UPRIGHT)
        return mSpriteToItemReplaceMap[DIRECTION_UP];

    if (direction == DIRECTION_DOWNLEFT || direction == DIRECTION_DOWNRIGHT)
        return mSpriteToItemReplaceMap[DIRECTION_DOWN];

    return 0;
}
