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

#ifndef DYETOOL_CLIENT_H
#define DYETOOL_CLIENT_H

#include "enums/state.h"

#include "listeners/actionlistener.h"

#include "net/serverinfo.h"

#ifdef USE_SDL2
#include <SDL2_framerate.h>
#else
#include <SDL_framerate.h>
#endif

#include "localconsts.h"

class Button;
class Desktop;
class Skin;
class Window;

extern bool isSafeMode;
extern int serverVersion;
extern unsigned int tmwServerVersion;
extern int start_time;
extern int textures_count;

extern std::string errorMessage;

/**
 * The core part of the client. This class initializes all subsystems, runs
 * the event loop, and shuts everything down again.
 */
class Client final : public ActionListener
{
    public:
        Client();

        A_DELETE_COPY(Client)

        ~Client();

        void gameInit();

        void testsInit();

        int gameExec();

        static int testsExec();

        void setState(const State state)
        { mState = state; }

        State getState() const A_WARN_UNUSED
        { return mState; }

        void action(const ActionEvent &event) override final;

        void moveButtons(const int width);

        void windowRemoved(const Window *const window);

        void slowLogic();

    private:
        void stateGame();

        void initSoundManager();

        static void initGraphics();

        void gameClear();

        void testsClear();

        static void logVars();

        Window *mCurrentDialog;
        Button *mSetupButton;
        Button *mVideoButton;
        Button *mHelpButton;
        Button *mAboutButton;
        Button *mThemesButton;
        Button *mPerfomanceButton;
#ifdef ANDROID
        Button *mCloseButton;
#endif

        State mState;
        State mOldState;

        Skin *mSkin;
        int mButtonPadding;
        int mButtonSpacing;
        int mPing;
        bool mConfigAutoSaved;
};

extern Client *client;
extern unsigned int mLastHost;
extern unsigned long mSearchHash;

#endif  // DYETOOL_CLIENT_H
