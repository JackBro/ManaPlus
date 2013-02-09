/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef SDLFONT_H
#define SDLFONT_H

#include <guichan/font.hpp>

#ifdef __WIN32__
#include <SDL/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif

#include <list>
#include <string>

#include "localconsts.h"

const unsigned int CACHES_NUMBER = 256;

class SDLTextChunk;

/**
 * A wrapper around SDL_ttf for allowing the use of TrueType fonts.
 *
 * <b>NOTE:</b> This class initializes SDL_ttf as necessary.
 */
class SDLFont final : public gcn::Font
{
    public:
        /**
         * Constructor.
         *
         * @param filename  Font filename.
         * @param size      Font size.
         */
        SDLFont(std::string filename, const int size, const int style = 0);

        A_DELETE_COPY(SDLFont)

        /**
         * Destructor.
         */
        ~SDLFont();

        void loadFont(std::string filename, const int size,
                      const int style = 0);

        void createSDLTextChunk(SDLTextChunk *const chunk);

        virtual int getWidth(const std::string &text) const A_WARN_UNUSED;

        virtual int getHeight() const A_WARN_UNUSED;

        std::list<SDLTextChunk> *getCache() A_WARN_UNUSED
        { return mCache; }

        /**
         * @see Font::drawString
         */
        void drawString(gcn::Graphics *const graphics,
                        const std::string &text,
                        const int x, const int y);

        void clear();

        void doClean();

        void slowLogic(const int rnd);

        int getCreateCounter() const A_WARN_UNUSED
        { return mCreateCounter; }

        int getDeleteCounter() const A_WARN_UNUSED
        { return mDeleteCounter; }

    private:
        TTF_Font *mFont;
        unsigned mCreateCounter;
        unsigned mDeleteCounter;

        // Word surfaces cache
        mutable std::list<SDLTextChunk> mCache[CACHES_NUMBER];
        int mCleanTime;
};

#endif
