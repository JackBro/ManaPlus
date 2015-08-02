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

#include "client.h"

#include "chatlogger.h"
#include "configmanager.h"
#include "dirs.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "eventsmanager.h"
#include "game.h"
#include "graphicsmanager.h"
#include "itemshortcut.h"
#include "party.h"
#include "settings.h"
#include "soundmanager.h"
#include "spellshortcut.h"
#include "statuseffect.h"
#include "units.h"
#include "touchmanager.h"

#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "input/inputmanager.h"
#include "input/joystick.h"
#include "input/keyboardconfig.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/skin.h"
#include "gui/popupmanager.h"
#include "gui/windowmanager.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/desktop.h"
#include "gui/widgets/windowcontainer.h"
#include "gui/widgets/window.h"

#include "resources/imagehelper.h"
#include "resources/resourcemanager.h"

#include "utils/cpu.h"
#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/fuzzer.h"
#include "utils/gettext.h"
#include "utils/gettexthelper.h"
#ifdef ANDROID
#include "utils/paths.h"
#endif
#include "utils/physfstools.h"
#include "utils/sdlcheckutils.h"
#include "utils/timer.h"

#include "utils/translation/translationmanager.h"

#include "listeners/errorlistener.h"

#include "configuration.h"

#include "net/ipc.h"

#ifdef WIN32
#include <SDL_syswm.h>
#include "utils/specialfolder.h"
#undef ERROR
#endif

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#include <fstream>
#endif
#endif

#include <sys/stat.h>

#include "debug.h"

std::string errorMessage;

Client *client = nullptr;

extern FPSmanager fpsManager;

volatile bool runCounters;
bool isSafeMode = false;
int serverVersion = 0;
unsigned int tmwServerVersion = 0;
int start_time;
unsigned int mLastHost = 0;
unsigned long mSearchHash = 0;
int textures_count = 0;

Client::Client() :
    ActionListener(),
    mCurrentDialog(nullptr),
    mSetupButton(nullptr),
    mVideoButton(nullptr),
    mHelpButton(nullptr),
    mAboutButton(nullptr),
    mThemesButton(nullptr),
    mPerfomanceButton(nullptr),
#ifdef ANDROID
    mCloseButton(nullptr),
#endif
    mState(STATE_GAME),
    mOldState(STATE_START),
    mSkin(nullptr),
    mButtonPadding(1),
    mButtonSpacing(3),
    mPing(0),
    mConfigAutoSaved(false)
{
    WindowManager::init();
}

void Client::testsInit()
{
}

void Client::testsClear()
{
}

// +++ need remove duplicate from here
void Client::gameInit()
{
    logger = new Logger;

    // Load branding information
    if (!settings.options.brandingPath.empty())
        branding.init(settings.options.brandingPath);
    branding.setDefaultValues(getBrandingDefaults());

    Dirs::initRootDir();
    Dirs::initHomeDir();

    // Configure logger
    if (!settings.options.logFileName.empty())
        settings.logFileName = settings.options.logFileName;
    else
        settings.logFileName = settings.localDataDir + "/manaplus.log";
    logger->setLogFile(settings.logFileName);

#ifdef USE_FUZZER
    Fuzzer::init();
#endif
    ConfigManager::backupConfig("config.xml");
    ConfigManager::initConfiguration();
    paths.setDefaultValues(getPathsDefaults());
    logger->log("init 4");
    logger->setDebugLog(config.getBoolValue("debugLog"));
    logger->setReportUnimplimented(config.getBoolValue("unimplimentedLog"));

    config.incValue("runcount");

#ifndef ANDROID
    if (settings.options.test.empty())
        ConfigManager::storeSafeParameters();
#endif

    const ResourceManager *const resman = ResourceManager::getInstance();
    if (!resman->setWriteDir(settings.localDataDir))
    {
        logger->error(strprintf("%s couldn't be set as home directory! "
            "Exiting.", settings.localDataDir.c_str()));
    }

    GettextHelper::initLang();

//    chatLogger = new ChatLogger;
//    if (settings.options.chatLogDir.empty())
//    {
//        chatLogger->setBaseLogDir(settings.localDataDir
//            + std::string("/logs/"));
//    }
//    else
//    {
//        chatLogger->setBaseLogDir(settings.options.chatLogDir);
//    }

    logger->setLogToStandardOut(config.getBoolValue("logToStandardOut"));

    // Log the client version
    logger->log1(FULL_VERSION);
    logger->log("Start configPath: " + config.getConfigPath());

    Dirs::initScreenshotDir();

    // Initialize SDL
    logger->log1("Initializing SDL...");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        logger->safeError(strprintf("Could not initialize SDL: %s",
            SDL_GetError()));
    }
    atexit(SDL_Quit);

#ifndef USE_SDL2
    SDL_EnableUNICODE(1);
#endif
    WindowManager::applyKeyRepeat();

    // disable unused SDL events
#ifndef USE_SDL2
    SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
#endif
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

#ifdef WIN32
    Dirs::extractDataDir();
    Dirs::mountDataDir();
#endif

    WindowManager::setIcon();
    ConfigManager::checkConfigVersion();
    logVars();
    Cpu::detect();
#if defined(USE_OPENGL) 
#if !defined(ANDROID) && !defined(__APPLE__) && !defined(__native_client__)
    if (!settings.options.safeMode && settings.options.test.empty()
        && !config.getBoolValue("videodetected"))
    {
        graphicsManager.detectVideoSettings();
    }
#endif
#endif
    updateEnv();
    initGraphics();

#ifndef WIN32
    Dirs::extractDataDir();
    Dirs::mountDataDir();
#endif

    Dirs::updateDataPath();

    // Add the main data directories to our PhysicsFS search path
    if (!settings.options.dataPath.empty())
        resman->addToSearchPath(settings.options.dataPath, false);

    // Add the local data directory to PhysicsFS search path
    resman->addToSearchPath(settings.localDataDir, false);
    TranslationManager::loadCurrentLang();

    WindowManager::initTitle();

    mainGraphics->postInit();

    theme = new Theme;
    Theme::selectSkin();
    touchManager.init();

    // Initialize the item and emote shortcuts.
//    for (unsigned f = 0; f < SHORTCUT_TABS; f ++)
//        itemShortcut[f] = new ItemShortcut(f);
//    emoteShortcut = new EmoteShortcut;
//    dropShortcut = new DropShortcut;

    gui = new Gui();
    gui->postInit(mainGraphics);
    dialogsManager = new DialogsManager;
    popupManager = new PopupManager;

    initSoundManager();
    eventsManager.init();

    // Initialize keyboard
    keyboard.init();
    inputManager.init();

    // Initialise player relations
    Joystick::init();
    WindowManager::createWindows();

    keyboard.update();
    if (joystick)
        joystick->update();

//    if (mState != STATE_ERROR)
//        mState = STATE_CHOOSE_SERVER;

    startTimers();

    const int fpsLimit = config.getIntValue("fpslimit");
    settings.limitFps = fpsLimit > 0;

    SDL_initFramerate(&fpsManager);
    WindowManager::setFramerate(fpsLimit);

    start_time = static_cast<int>(time(nullptr));

//    PlayerInfo::init();

#ifdef ANDROID
#ifndef USE_SDL2
    WindowManager::updateScreenKeyboard(SDL_GetScreenKeyboardHeight(nullptr));
#endif
#endif

    mSkin = theme->load("windowmenu.xml", "");
    if (mSkin)
    {
        mButtonPadding = mSkin->getPadding();
        mButtonSpacing = mSkin->getOption("spacing", 3);
    }
}

Client::~Client()
{
    if (!settings.options.testMode)
        gameClear();
    else
        testsClear();
}

void Client::initSoundManager()
{
    // Initialize sound engine
    try
    {
        if (config.getBoolValue("sound"))
            soundManager.init();

        soundManager.setSfxVolume(config.getIntValue("sfxVolume"));
        soundManager.setMusicVolume(config.getIntValue("musicVolume"));
    }
    catch (const char *const err)
    {
        mState = STATE_ERROR;
        errorMessage = err;
        logger->log("Warning: %s", err);
    }
    soundManager.playMusic(branding.getValue(
        "loginMusic", "Magick - Real.ogg"));
}

void Client::initGraphics()
{
    WindowManager::applyVSync();
    runCounters = config.getBoolValue("packetcounters");

    graphicsManager.initGraphics();

    imageHelper->postInit();
    getConfigDefaults2(config.getDefaultValues());
    WindowManager::applyGrabMode();
    WindowManager::applyGamma();

    mainGraphics->beginDraw();
}

void Client::gameClear()
{
    if (logger)
        logger->log1("Quitting1");

    eventsManager.shutdown();
    WindowManager::deleteWindows();
    if (windowContainer)
        windowContainer->slowLogic();

    stopTimers();

    delete2(ipc);

    if (logger)
        logger->log1("Quitting2");

    delete2(mCurrentDialog);
    delete2(popupManager);
    delete2(dialogsManager);
    delete2(gui);

    if (logger)
        logger->log1("Quitting3");

    graphicsManager.deleteRenderers();

    if (logger)
        logger->log1("Quitting4");

    XML::cleanupXML();

    if (logger)
        logger->log1("Quitting5");

    // Shutdown sound
    soundManager.close();

    if (logger)
        logger->log1("Quitting6");

    touchManager.clear();
    ResourceManager::deleteInstance();

    if (logger)
        logger->log1("Quitting8");

    WindowManager::deleteIcon();

    if (logger)
        logger->log1("Quitting9");

    delete2(joystick);

    keyboard.deinit();

    if (logger)
        logger->log1("Quitting10");

    soundManager.shutdown();
    touchManager.shutdown();

#ifdef DEBUG_CONFIG
    config.enableKeyLogging();
#endif
    config.removeOldKeys();
    config.write();
    serverConfig.write();

    config.clear();
    serverConfig.clear();

    if (logger)
        logger->log1("Quitting11");

#ifdef USE_PROFILER
    Perfomance::clear();
#endif

#ifdef DEBUG_OPENGL_LEAKS
    if (logger)
        logger->log("textures left: %d", textures_count);
#endif

//    delete2(chatLogger);
    TranslationManager::close();
}

#define ADDBUTTON(var, object) var = object; \
    x -= var->getWidth() + mButtonSpacing; \
    var->setPosition(x, mButtonPadding); \
    top->add(var);

void Client::stateGame()
{
    if (!gui)
        return;

    BasicContainer2 *const top = static_cast<BasicContainer2*>(
        gui->getTop());

    if (!top)
        return;

    CREATEWIDGETV(desktop, Desktop, nullptr);
    top->add(desktop);
    int x = top->getWidth() - mButtonPadding;
    ADDBUTTON(mSetupButton, new Button(desktop,
        // TRANSLATORS: setup tab quick button
        _("Setup"), "Setup", this))
    ADDBUTTON(mPerfomanceButton, new Button(desktop,
        // TRANSLATORS: perfoamance tab quick button
        _("Performance"), "Perfomance", this))
    ADDBUTTON(mVideoButton, new Button(desktop,
        // TRANSLATORS: video tab quick button
        _("Video"), "Video", this))
    ADDBUTTON(mThemesButton, new Button(desktop,
        // TRANSLATORS: theme tab quick button
        _("Theme"), "Themes", this))
    ADDBUTTON(mAboutButton, new Button(desktop,
        // TRANSLATORS: theme tab quick button
        _("About"), "about", this))
    ADDBUTTON(mHelpButton, new Button(desktop,
        // TRANSLATORS: theme tab quick button
        _("Help"), "help", this))
#ifdef ANDROID
    ADDBUTTON(mCloseButton, new Button(desktop,
        // TRANSLATORS: close quick button
        _("Close"), "close", this))
#endif
    desktop->setSize(mainGraphics->getWidth(),
        mainGraphics->getHeight());
}

int Client::gameExec()
{
    int lastTickTime = tick_time;

    while (mState != STATE_EXIT)
    {
        PROFILER_START();
        if (eventsManager.handleEvents())
            continue;

        BLOCK_START("Client::gameExec 4")
        if (gui)
            gui->logic();
        cur_time = static_cast<int>(time(nullptr));
        int k = 0;
        while (lastTickTime != tick_time && k < 40)
        {
//            if (mGame)
//                mGame->logic();
//            else
            if (gui)
                gui->handleInput();

            ++lastTickTime;
            k ++;
        }
        soundManager.logic();

        logic_count += k;
        if (gui)
            gui->slowLogic();
//        if (mGame)
//            mGame->slowLogic();
        slowLogic();
        BLOCK_END("Client::gameExec 4")

        // This is done because at some point tick_time will wrap.
        lastTickTime = tick_time;

        // Update the screen when application is visible, delay otherwise.
        if (!WindowManager::getIsMinimized())
        {
            frame_count++;
            if (gui)
                gui->draw();
            mainGraphics->updateScreen();
        }
        else
        {
            SDL_Delay(100);
        }

        BLOCK_START("~Client::SDL_framerateDelay")
        if (settings.limitFps)
            SDL_framerateDelay(&fpsManager);
        BLOCK_END("~Client::SDL_framerateDelay")

        BLOCK_START("Client::gameExec 6")
//        if (mState == STATE_CONNECT_GAME)
//        {
//            stateConnectGame1();
//        }
        BLOCK_END("Client::gameExec 6")

        if (mState != mOldState)
        {
            BLOCK_START("Client::gameExec 7")

            mOldState = mState;

            // Get rid of the dialog of the previous state
            delete2(mCurrentDialog);

            // State has changed, while the quitDialog was active, it might
            // not be correct anymore
//            if (mQuitDialog)
//            {
//                mQuitDialog->scheduleDelete();
//                mQuitDialog = nullptr;
//            }
            BLOCK_END("Client::gameExec 7")

            BLOCK_START("Client::gameExec 8")
            switch (mState)
            {
                case STATE_GAME:
                    stateGame();
                    break;

                case STATE_LOAD_DATA:
                {
                    BLOCK_START("Client::gameExec STATE_LOAD_DATA")
                    logger->log1("State: LOAD DATA");

                    const ResourceManager *const resman
                        = ResourceManager::getInstance();

                    // If another data path has been set,
                    // we don't load any other files...
                    if (settings.options.dataPath.empty())
                    {
                        // Add customdata directory
                        resman->searchAndAddArchives(
                            "customdata/",
                            "zip",
                            false);
                    }

                    if (!settings.options.skipUpdate)
                    {
                        resman->searchAndAddArchives(
                            settings.updatesDir + "/local/",
                            "zip",
                            false);

                        resman->addToSearchPath(settings.localDataDir
                            + dirSeparator + settings.updatesDir + "/local/",
                            false);
                    }

                    logger->log("Init paths");
                    paths.init("paths.xml", UseResman_true);
                    paths.setDefaultValues(getPathsDefaults());
                    TranslationManager::loadCurrentLang();

                    if (desktop)
                        desktop->reloadWallpaper();

                    mState = STATE_GET_CHARACTERS;
                    BLOCK_END("Client::gameExec STATE_LOAD_DATA")
                    break;
                }
                case STATE_START:
                default:
                    mState = STATE_FORCE_QUIT;
                    break;
            }
            BLOCK_END("Client::gameExec 8")
        }
        PROFILER_END();
    }

    return 0;
}

void Client::action(const ActionEvent &event)
{
    std::string tab;
    const std::string &eventId = event.getId();

    if (eventId == "close")
    {
        setState(STATE_FORCE_QUIT);
        return;
    }
    if (eventId == "Setup")
    {
        tab.clear();
    }
    else if (eventId == "help")
    {
        inputManager.executeAction(InputAction::WINDOW_HELP);
        return;
    }
    else if (eventId == "about")
    {
        inputManager.executeAction(InputAction::WINDOW_ABOUT);
        return;
    }
    else if (eventId == "Video")
    {
        tab = "Video";
    }
    else if (eventId == "Themes")
    {
        tab = "Theme";
    }
    else if (eventId == "Perfomance")
    {
        tab = "Perfomance";
    }
    else
    {
        return;
    }
}

void Client::moveButtons(const int width)
{
    if (mSetupButton)
    {
        int x = width - mSetupButton->getWidth() - mButtonPadding;
        mSetupButton->setPosition(x, mButtonPadding);
#ifndef WIN32
        x -= mPerfomanceButton->getWidth() + mButtonSpacing;
        mPerfomanceButton->setPosition(x, mButtonPadding);

        x -= mVideoButton->getWidth() + mButtonSpacing;
        mVideoButton->setPosition(x, mButtonPadding);

        x -= mThemesButton->getWidth() + mButtonSpacing;
        mThemesButton->setPosition(x, mButtonPadding);

        x -= mAboutButton->getWidth() + mButtonSpacing;
        mAboutButton->setPosition(x, mButtonPadding);

        x -= mHelpButton->getWidth() + mButtonSpacing;
        mHelpButton->setPosition(x, mButtonPadding);
#ifdef ANDROID
        x -= mCloseButton->getWidth() + mButtonSpacing;
        mCloseButton->setPosition(x, mButtonPadding);
#endif
#endif
    }
}

void Client::windowRemoved(const Window *const window)
{
    if (mCurrentDialog == window)
        mCurrentDialog = nullptr;
}

void Client::logVars()
{
#ifdef ANDROID
    logger->log("APPDIR: %s", getenv("APPDIR"));
    logger->log("DATADIR2: %s", getSdStoragePath().c_str());
#endif
}

void Client::slowLogic()
{
}

int Client::testsExec()
{
    return 0;
}
