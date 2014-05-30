/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "game.h"

#include "main.h"

#include "actormanager.h"
#include "auctionmanager.h"
#include "animatedsprite.h"
#include "client.h"
#include "commandhandler.h"
#include "configuration.h"
#include "effectmanager.h"
#include "emoteshortcut.h"
#include "eventsmanager.h"
#include "guildmanager.h"
#include "itemshortcut.h"
#include "soundmanager.h"
#include "settings.h"
#include "spellshortcut.h"
#include "touchmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "particle/particle.h"

#include "input/inputmanager.h"
#include "input/joystick.h"
#include "input/keyboardconfig.h"

#include "gui/chatconsts.h"
#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/viewport.h"
#include "gui/windowmanager.h"
#include "gui/windowmenu.h"

#include "gui/fonts/font.h"

#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/emotewindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/minimap.h"
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/whoisonline.h"

#include "gui/widgets/tabs/battletab.h"

#include "gui/widgets/dropshortcutcontainer.h"
#include "gui/widgets/emoteshortcutcontainer.h"
#include "gui/widgets/itemshortcutcontainer.h"
#include "gui/widgets/spellshortcutcontainer.h"

#include "gui/widgets/tabs/gmtab.h"
#include "gui/widgets/tabs/langtab.h"
#include "gui/widgets/tabs/tradetab.h"

#include "net/generalhandler.h"
#include "net/gamehandler.h"
#include "net/net.h"
#include "net/packetcounters.h"

#include "resources/delayedmanager.h"
#include "resources/imagewriter.h"
#include "resources/mapreader.h"
#include "resources/resourcemanager.h"

#include "resources/db/mapdb.h"

#include "resources/map/map.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/langs.h"
#include "utils/mkdir.h"
#include "utils/physfstools.h"
#include "utils/sdlcheckutils.h"
#include "utils/timer.h"

#include "listeners/errorlistener.h"

#include <sstream>
#include <string>

#ifdef USE_MUMBLE
#include "mumblemanager.h"
#endif

#include "debug.h"

QuitDialog *quitDialog = nullptr;
Window *disconnectedDialog = nullptr;

int weightNoticeTime = 0;

bool mStatsReUpdated = false;
const unsigned adjustDelay = 10;

/**
 * Initialize every game sub-engines in the right order
 */
static void initEngines()
{
    actorManager = new ActorManager;
    commandHandler = new CommandHandler;
    effectManager = new EffectManager;
    AuctionManager::init();
    GuildManager::init();

    particleEngine = new Particle();
    particleEngine->setMap(nullptr);
    particleEngine->setupEngine();
    BeingInfo::init();

    Net::getGameHandler()->initEngines();

    keyboard.update();
    if (joystick)
        joystick->update();
}

/**
 * Create all the various globally accessible gui windows
 */
static void createGuiWindows()
{
    if (setupWindow)
        setupWindow->clearWindowsForReset();

    if (emoteShortcut)
        emoteShortcut->load();

    // Create dialogs
    emoteWindow = new EmoteWindow;
    emoteWindow->postInit();
    chatWindow = new ChatWindow;
    chatWindow->postInit();
    tradeWindow = new TradeWindow;
    equipmentWindow = new EquipmentWindow(PlayerInfo::getEquipment(),
        player_node);
    equipmentWindow->postInit();
    beingEquipmentWindow = new EquipmentWindow(nullptr, nullptr, true);
    beingEquipmentWindow->postInit();
    beingEquipmentWindow->setVisible(false);
    statusWindow = new StatusWindow;
    miniStatusWindow = new MiniStatusWindow;
    inventoryWindow = new InventoryWindow(PlayerInfo::getInventory());
    inventoryWindow->postInit();
    shopWindow = new ShopWindow;
    shopWindow->postInit();
    skillDialog = new SkillDialog;
    skillDialog->postInit();
    minimap = new Minimap;
    debugWindow = new DebugWindow;
    debugWindow->postInit();
    itemShortcutWindow = new ShortcutWindow(
        "ItemShortcut", "items.xml", 83, 460);

    for (unsigned f = 0; f < SHORTCUT_TABS; f ++)
    {
        itemShortcutWindow->addTab(toString(f + 1),
            new ItemShortcutContainer(nullptr, f));
    }
    if (config.getBoolValue("showDidYouKnow"))
    {
        didYouKnowWindow->setVisible(true);
        didYouKnowWindow->loadData();
    }

    emoteShortcutWindow = new ShortcutWindow("EmoteShortcut",
        new EmoteShortcutContainer(nullptr),
        "emotes.xml",
        130, 480);
    outfitWindow = new OutfitWindow();
    dropShortcutWindow = new ShortcutWindow("DropShortcut",
        new DropShortcutContainer(nullptr),
        "drops.xml");

    spellShortcutWindow = new ShortcutWindow("SpellShortcut", "spells.xml",
                                             265, 328);
    for (unsigned f = 0; f < SPELL_SHORTCUT_TABS; f ++)
    {
        spellShortcutWindow->addTab(toString(f + 1),
            new SpellShortcutContainer(nullptr, f));
    }

    botCheckerWindow = new BotCheckerWindow;
    whoIsOnline = new WhoIsOnline;
    whoIsOnline->postInit();
    killStats = new KillStats;
    socialWindow = new SocialWindow;
    socialWindow->postInit();
    questsWindow = new QuestsWindow;

    // TRANSLATORS: chat tab header
    localChatTab = new ChatTab(chatWindow, _("General"), GENERAL_CHANNEL);
    localChatTab->setAllowHighlight(false);
    if (config.getBoolValue("showChatHistory"))
        localChatTab->loadFromLogFile("#General");

    if (serverVersion >= 8 && serverConfig.getValue("enableLangTab", 1))
    {
        const std::string lang = getLangShort();
        if (lang.size() == 2)
        {
            langChatTab = new LangTab(chatWindow, lang);
            langChatTab->setAllowHighlight(false);
        }
    }

    // TRANSLATORS: chat tab header
    debugChatTab = new ChatTab(chatWindow, _("Debug"), "");
    debugChatTab->setAllowHighlight(false);

    if (config.getBoolValue("enableTradeTab"))
    {
        tradeChatTab = new TradeTab(chatWindow);
        tradeChatTab->setAllowHighlight(false);
    }
    else
    {
        tradeChatTab = nullptr;
    }

    if (config.getBoolValue("enableBattleTab"))
    {
        battleChatTab = new BattleTab(chatWindow);
        battleChatTab->setAllowHighlight(false);
    }
    else
    {
        battleChatTab = nullptr;
    }

    if (player_node && !gmChatTab && config.getBoolValue("enableGmTab")
        && player_node->getGMLevel() > 0)
    {
        gmChatTab = new GmTab(chatWindow);
    }

    if (!isSafeMode && chatWindow)
        chatWindow->loadState();

    if (setupWindow)
        setupWindow->externalUpdate();

    if (player_node)
        player_node->updateStatus();

    Net::getGeneralHandler()->gameStarted();
}

/**
 * Destroy all the globally accessible gui windows
 */
static void destroyGuiWindows()
{
    Net::getGeneralHandler()->gameEnded();

    if (whoIsOnline)
        whoIsOnline->setAllowUpdate(false);

    if (auctionManager)
        auctionManager->clear();

    if (guildManager)
        guildManager->clear();

    delete2(windowMenu);
    delete2(localChatTab)  // Need to do this first, so it can remove itself
    delete2(debugChatTab)
    delete2(tradeChatTab)
    delete2(battleChatTab)
    delete2(langChatTab)
    delete2(gmChatTab);
    logger->log("start deleting");
    delete2(emoteWindow);
    delete2(chatWindow)
    logger->log("end deleting");
    delete2(statusWindow)
    delete2(miniStatusWindow)
    delete2(inventoryWindow)
    delete2(shopWindow)
    delete2(skillDialog)
    delete2(minimap)
    delete2(equipmentWindow)
    delete2(beingEquipmentWindow)
    delete2(tradeWindow)
    delete2(debugWindow)
    delete2(itemShortcutWindow)
    delete2(emoteShortcutWindow)
    delete2(outfitWindow)
    delete2(socialWindow)
    delete2(dropShortcutWindow);
    delete2(spellShortcutWindow);
    delete2(botCheckerWindow);
    delete2(questsWindow);
    delete2(whoIsOnline);
    delete2(killStats);

    if (auctionManager && AuctionManager::getEnableAuctionBot())
        auctionManager->reload();

    if (guildManager && GuildManager::getEnableGuildBot())
        guildManager->reload();
}

Game *Game::mInstance = nullptr;

Game::Game() :
    mCurrentMap(nullptr),
    mMapName(""),
    mValidSpeed(true),
    mNextAdjustTime(cur_time + adjustDelay),
    mAdjustLevel(0),
    mAdjustPerfomance(config.getBoolValue("adjustPerfomance")),
    mLowerCounter(0),
    mPing(0),
    mTime(cur_time + 1),
    mTime2(cur_time + 10)
{
    touchManager.setInGame(true);
    spellManager = new SpellManager;
    spellShortcut = new SpellShortcut;

//    assert(!mInstance);
    if (mInstance)
        logger->log("error: double game creation");
    mInstance = this;

    config.incValue("gamecount");

    disconnectedDialog = nullptr;

    // Create the viewport
    viewport = new Viewport;
    viewport->setSize(mainGraphics->mWidth, mainGraphics->mHeight);
    PlayerInfo::clear();

    BasicContainer2 *const top = static_cast<BasicContainer2*>(gui->getTop());
    if (top)
        top->add(viewport);
    viewport->requestMoveToBottom();

    AnimatedSprite::setEnableCache(mainGraphics->getOpenGL()
        && config.getBoolValue("enableDelayedAnimations"));

    CompoundSprite::setEnableDelay(
        config.getBoolValue("enableCompoundSpriteDelay"));

    createGuiWindows();
    windowMenu = new WindowMenu(nullptr);

    if (windowContainer)
        windowContainer->add(windowMenu);

    initEngines();

    // Initialize beings
    if (actorManager)
        actorManager->setPlayer(player_node);

    Net::getGameHandler()->ping(tick_time);

    if (setupWindow)
        setupWindow->setInGame(true);
    clearKeysArray();

    if (guildManager && GuildManager::getEnableGuildBot())
        guildManager->requestGuildInfo();

    if (player_node)
        player_node->updatePets();
}

Game::~Game()
{
    touchManager.setInGame(false);
    config.write();
    serverConfig.write();
    resetAdjustLevel();
    destroyGuiWindows();

    AnimatedSprite::setEnableCache(false);

    delete2(actorManager)
    if (client->getState() != STATE_CHANGE_MAP)
        delete2(player_node)
    delete2(commandHandler)
    delete2(effectManager)
    delete2(particleEngine)
    delete2(viewport)
    delete2(mCurrentMap)
    delete2(spellManager)
    delete2(spellShortcut)
    delete2(auctionManager)
    delete2(guildManager)
#ifdef USE_MUMBLE
    delete2(mumbleManager)
#endif

    Being::clearCache();
    mInstance = nullptr;
    PlayerInfo::gameDestroyed();
}

void Game::addWatermark()
{
    if (!boldFont || !config.getBoolValue("addwatermark"))
        return;
    mainGraphics->setColorAll(theme->getColor(Theme::TEXT, 255),
        theme->getColor(Theme::TEXT_OUTLINE, 255));
    boldFont->drawString(mainGraphics, settings.serverName, 100, 50);
}

bool Game::createScreenshot()
{
    if (!mainGraphics)
        return false;

    SDL_Surface *screenshot = nullptr;

    if (!config.getBoolValue("showip") && gui)
    {
        mainGraphics->setSecure(true);
        mainGraphics->prepareScreenshot();
        gui->draw();
        addWatermark();
        screenshot = mainGraphics->getScreenshot();
        mainGraphics->setSecure(false);
    }
    else
    {
        addWatermark();
        screenshot = mainGraphics->getScreenshot();
    }

    if (!screenshot)
        return false;

    return saveScreenshot(screenshot);
}

bool Game::saveScreenshot(SDL_Surface *const screenshot)
{
    std::string screenshotDirectory = settings.screenshotDir;
    if (mkdir_r(screenshotDirectory.c_str()) != 0)
    {
        logger->log("Directory %s doesn't exist and can't be created! "
                    "Setting screenshot directory to home.",
                    screenshotDirectory.c_str());
        screenshotDirectory = std::string(PhysFs::getUserDir());
    }

    // Search for an unused screenshot name
    std::stringstream filename;
    std::fstream testExists;
    bool found = false;
    static unsigned int screenshotCount = 0;

    time_t rawtime;
    char buffer [100];
    time(&rawtime);
    struct tm *const timeinfo = localtime(&rawtime);
    strftime(buffer, 99, "%Y-%m-%d_%H-%M-%S", timeinfo);

    const std::string serverName = settings.serverName;
    std::string screenShortStr;
    if (serverName.empty())
    {
        screenShortStr = strprintf("%s_Screenshot_%s_",
            branding.getValue("appName", "ManaPlus").c_str(),
            buffer);
    }
    else
    {
        screenShortStr = strprintf("%s_Screenshot_%s_%s_",
            branding.getValue("appName", "ManaPlus").c_str(),
            serverName.c_str(), buffer);
    }

    do
    {
        screenshotCount++;
        filename.str("");
        filename << screenshotDirectory << "/";
        filename << screenShortStr << screenshotCount << ".png";
        testExists.open(filename.str().c_str(), std::ios::in);
        found = !testExists.is_open();
        testExists.close();
    }
    while (!found);

    const std::string fileNameStr = filename.str();
    const bool success = ImageWriter::writePNG(screenshot, fileNameStr);
    if (success)
    {
        if (localChatTab)
        {
            // TRANSLATORS: save file message
            std::string str = strprintf(_("Screenshot saved as %s"),
                fileNameStr.c_str());
            localChatTab->chatLog(str, ChatMsgType::BY_SERVER);
        }
    }
    else
    {
        if (localChatTab)
        {
            // TRANSLATORS: save file message
            localChatTab->chatLog(_("Saving screenshot failed!"),
                                  ChatMsgType::BY_SERVER);
        }
        logger->log1("Error: could not save screenshot.");
    }

    MSDL_FreeSurface(screenshot);
    return success;
}

void Game::logic()
{
    BLOCK_START("Game::logic")
    handleInput();

    // Handle all necessary game logic
    ActorSprite::actorLogic();
    if (actorManager)
        actorManager->logic();
    if (particleEngine)
        particleEngine->update();
    if (mCurrentMap)
        mCurrentMap->update();

    BLOCK_END("Game::logic")
}

void Game::slowLogic()
{
    BLOCK_START("Game::slowLogic")
    if (player_node)
        player_node->slowLogic();
    const int time = cur_time;
    if (mTime != time)
    {
        if (valTest(Updated))
            mValidSpeed = false;

        mTime = time + 1;
        if (botCheckerWindow)
            botCheckerWindow->slowLogic();
        if (debugWindow)
            debugWindow->slowLogic();
        if (killStats)
            killStats->update();
        if (socialWindow)
            socialWindow->slowLogic();
        if (whoIsOnline)
            whoIsOnline->slowLogic();
        Being::reReadConfig();
        if (killStats)
            killStats->recalcStats();

        if (time > mTime2 || mTime2 - time > 10)
        {
            mTime2 = time + 10;
            config.writeUpdated();
            serverConfig.writeUpdated();
        }
    }

    if (shopWindow)
        shopWindow->updateTimes();
    if (mainGraphics->getOpenGL())
        DelayedManager::delayedLoad();
    if (guildManager)
        guildManager->slowLogic();
    PacketCounters::update();

    // Handle network stuff
    if (!Net::getGameHandler()->isConnected())
    {
        if (client->getState() == STATE_CHANGE_MAP)
            return;  // Not a problem here

        if (client->getState() != STATE_ERROR)
        {
            if (!disconnectedDialog)
            {
                // TRANSLATORS: error message text
                errorMessage = _("The connection to the server was lost.");
                disconnectedDialog = DialogsManager::openErrorDialog(
                    // TRANSLATORS: error message header
                    _("Network Error"), errorMessage, false);
                disconnectedDialog->addActionListener(&errorListener);
                disconnectedDialog->requestMoveToTop();
            }
        }

        if (viewport && !errorMessage.empty())
        {
            const Map *const map = viewport->getMap();
            if (map)
            {
                logger->log("state: %d", client->getState());
                map->saveExtraLayer();
            }
        }
        DialogsManager::closeDialogs();
        WindowManager::setFramerate(config.getIntValue("fpslimit"));
        mNextAdjustTime = cur_time + adjustDelay;
        if (client->getState() != STATE_ERROR)
            errorMessage.clear();
    }
    else
    {
        if (Net::getGameHandler()->mustPing()
            && get_elapsed_time1(mPing) > 3000)
        {
            mPing = tick_time;
            Net::getGameHandler()->ping(tick_time);
        }

        if (mAdjustPerfomance)
            adjustPerfomance();
        if (disconnectedDialog)
        {
            disconnectedDialog->scheduleDelete();
            disconnectedDialog = nullptr;
        }
    }
    BLOCK_END("Game::slowLogic")
}

void Game::adjustPerfomance()
{
    FUNC_BLOCK("Game::adjustPerfomance", 1)
    const int time = cur_time;
    if (mNextAdjustTime <= adjustDelay)
    {
        mNextAdjustTime = time + adjustDelay;
    }
    else if (mNextAdjustTime < static_cast<unsigned>(time))
    {
        mNextAdjustTime = time + adjustDelay;

        if (mAdjustLevel > 3 || !player_node || player_node->getHalfAway()
            || player_node->getAway())
        {
            return;
        }

        int maxFps = WindowManager::getFramerate();
        if (maxFps != config.getIntValue("fpslimit"))
            return;

        if (!maxFps)
            maxFps = 30;
        else if (maxFps < 10)
            return;

        if (fps < maxFps - 10)
        {
            if (mLowerCounter < 2)
            {
                mLowerCounter ++;
                mNextAdjustTime = cur_time + 1;
                return;
            }
            mLowerCounter = 0;
            mAdjustLevel ++;
            switch (mAdjustLevel)
            {
                case 1:
                {
                    if (config.getBoolValue("beingopacity"))
                    {
                        config.setValue("beingopacity", false);
                        config.setSilent("beingopacity", true);
                        if (localChatTab)
                        {
                            localChatTab->chatLog("Auto disable Show "
                                "beings transparency", ChatMsgType::BY_SERVER);
                        }
                    }
                    else
                    {
                        mNextAdjustTime = time + 1;
                        mLowerCounter = 2;
                    }
                    break;
                }
                case 2:
                    if (Particle::emitterSkip < 4)
                    {
                        Particle::emitterSkip = 4;
                        if (localChatTab)
                        {
                            localChatTab->chatLog("Auto lower Particle "
                                "effects", ChatMsgType::BY_SERVER);
                        }
                    }
                    else
                    {
                        mNextAdjustTime = time + 1;
                        mLowerCounter = 2;
                    }
                    break;
                case 3:
                    if (!config.getBoolValue("alphaCache"))
                    {
                        config.setValue("alphaCache", true);
                        config.setSilent("alphaCache", false);
                        if (localChatTab)
                        {
                            localChatTab->chatLog("Auto enable opacity cache",
                                ChatMsgType::BY_SERVER);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::resetAdjustLevel()
{
    if (!mAdjustPerfomance)
        return;

    mNextAdjustTime = cur_time + adjustDelay;
    switch (mAdjustLevel)
    {
        case 1:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            break;
        case 2:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            Particle::emitterSkip = config.getIntValue(
                "particleEmitterSkip") + 1;
            break;
        default:
        case 3:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            Particle::emitterSkip = config.getIntValue(
                "particleEmitterSkip") + 1;
            config.setValue("alphaCache",
                config.getBoolValue("alphaCache"));
            break;
    }
    mAdjustLevel = 0;
}

void Game::handleMove()
{
    if (!player_node)
        return;

    // Moving player around
    if (player_node->isAlive()
        && chatWindow
        && !chatWindow->isInputFocused()
        && !InventoryWindow::isAnyInputFocused()
        && !quitDialog)
    {
        NpcDialog *const dialog = NpcDialog::getActive();
        if (dialog)
            return;

        // Ignore input if either "ignore" key is pressed
        // Stops the character moving about if the user's window manager
        // uses "ignore+arrow key" to switch virtual desktops.
        if (inputManager.isActionActive(InputAction::IGNORE_INPUT_1) ||
            inputManager.isActionActive(InputAction::IGNORE_INPUT_2))
        {
            return;
        }

        unsigned char direction = 0;

        // Translate pressed keys to movement and direction
        if (inputManager.isActionActive(InputAction::MOVE_UP) ||
            (joystick && joystick->isUp()))
        {
            direction |= BeingDirection::UP;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (inputManager.isActionActive(InputAction::MOVE_DOWN) ||
                 (joystick && joystick->isDown()))
        {
            direction |= BeingDirection::DOWN;
            setValidSpeed();
            player_node->cancelFollow();
        }

        if (inputManager.isActionActive(InputAction::MOVE_LEFT) ||
            (joystick && joystick->isLeft()))
        {
            direction |= BeingDirection::LEFT;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (inputManager.isActionActive(InputAction::MOVE_RIGHT) ||
                 (joystick && joystick->isRight()))
        {
            direction |= BeingDirection::RIGHT;
            setValidSpeed();
            player_node->cancelFollow();
        }
        else if (inputManager.isActionActive(InputAction::MOVE_FORWARD))
        {
            direction = player_node->getDirection();
            setValidSpeed();
            player_node->cancelFollow();
        }

        if (!inputManager.isActionActive(InputAction::EMOTE) || direction == 0)
            moveInDirection(direction);
    }
}

void Game::moveInDirection(const unsigned char direction)
{
    if (!viewport)
        return;

    if (!viewport->getCameraMode())
    {
        if (player_node)
            player_node->specialMove(direction);
    }
    else
    {
        int dx = 0;
        int dy = 0;
        if (direction & BeingDirection::LEFT)
            dx = -5;
        else if (direction & BeingDirection::RIGHT)
            dx = 5;

        if (direction & BeingDirection::UP)
            dy = -5;
        else if (direction & BeingDirection::DOWN)
            dy = 5;
        viewport->moveCamera(dx, dy);
    }
}

void Game::updateFrameRate(int fpsLimit)
{
    if (!fpsLimit)
    {
        if (player_node && player_node->getAway())
        {
            if (settings.inputFocused || settings.mouseFocused)
                fpsLimit = config.getIntValue("fpslimit");
            else
                fpsLimit = config.getIntValue("altfpslimit");
        }
        else
        {
            fpsLimit = config.getIntValue("fpslimit");
        }
    }
    WindowManager::setFramerate(fpsLimit);
    mNextAdjustTime = cur_time + adjustDelay;
}

/**
 * The huge input handling method.
 */
void Game::handleInput()
{
    BLOCK_START("Game::handleInput 1")
    if (joystick)
        joystick->logic();

    eventsManager.handleGameEvents();

    // If the user is configuring the keys then don't respond.
    if (!player_node || !keyboard.isEnabled() || player_node->getAway())
    {
        BLOCK_END("Game::handleInput 1")
        return;
    }

    // If pressed outfits keys, stop processing keys.
    if (inputManager.isActionActive(InputAction::WEAR_OUTFIT)
        || inputManager.isActionActive(InputAction::COPY_OUTFIT)
        || (setupWindow && setupWindow->isWindowVisible()))
    {
        BLOCK_END("Game::handleInput 1")
        return;
    }

    handleMove();
    InputManager::handleRepeat();
    BLOCK_END("Game::handleInput 1")
}

/**
 * Changes the currently active map. Should only be called while the game is
 * running.
 */
void Game::changeMap(const std::string &mapPath)
{
    BLOCK_START("Game::changeMap")

    resetAdjustLevel();
    ResourceManager *const resman = ResourceManager::getInstance();
    resman->cleanProtected();

    if (viewport)
        viewport->clearPopup();

    // Clean up floor items, beings and particles
    if (actorManager)
        actorManager->clear();

    // Close the popup menu on map change so that invalid options can't be
    // executed.
    if (viewport)
    {
        viewport->closePopupMenu();
        viewport->cleanHoverItems();
    }

    // Unset the map of the player so that its particles are cleared before
    // being deleted in the next step
    if (player_node)
        player_node->setMap(nullptr);

    if (particleEngine)
        particleEngine->clear();

    mMapName = mapPath;

    std::string fullMap = paths.getValue("maps", "maps/").append(
        mMapName).append(".tmx");
    std::string realFullMap = paths.getValue("maps", "maps/").append(
        MapDB::getMapName(mMapName)).append(".tmx");

    if (!PhysFs::exists(realFullMap.c_str()))
        realFullMap.append(".gz");

    // Attempt to load the new map
    Map *const newMap = MapReader::readMap(fullMap, realFullMap);

    if (mCurrentMap)
        mCurrentMap->saveExtraLayer();

    if (newMap)
        newMap->addExtraLayer();

    if (socialWindow)
        socialWindow->setMap(newMap);

    // Notify the minimap and actorManager about the map change
    if (minimap)
        minimap->setMap(newMap);
    if (actorManager)
        actorManager->setMap(newMap);
    if (particleEngine)
        particleEngine->setMap(newMap);
    if (viewport)
        viewport->setMap(newMap);

    // Initialize map-based particle effects
    if (newMap)
        newMap->initializeParticleEffects(particleEngine);


    // Start playing new music file when necessary
    const std::string oldMusic = mCurrentMap
        ? mCurrentMap->getMusicFile() : "";
    const std::string newMusic = newMap ? newMap->getMusicFile() : "";
    if (newMusic != oldMusic)
    {
        if (newMusic.empty())
            soundManager.fadeOutMusic();
        else
            soundManager.fadeOutAndPlayMusic(newMusic);
    }

    if (mCurrentMap)
        mCurrentMap->saveExtraLayer();

    delete mCurrentMap;
    mCurrentMap = newMap;

    if (questsWindow)
        questsWindow->setMap(mCurrentMap);

#ifdef USE_MUMBLE
    if (mumbleManager)
        mumbleManager->setMap(mapPath);
#endif

    if (player_node)
        player_node->recreateItemParticles();

    Net::getGameHandler()->mapLoadedEvent();
    BLOCK_END("Game::changeMap")
}

void Game::updateHistory(const SDL_Event &event)
{
    if (!player_node || !player_node->getAttackType())
        return;

    if (static_cast<int>(event.key.keysym.sym) != -1)
    {
        bool old = false;

        const int key = keyboard.getKeyIndex(event);
        const int time = cur_time;
        int idx = -1;
        for (int f = 0; f < MAX_LASTKEYS; f ++)
        {
            LastKey &lastKey = mLastKeys[f];
            if (lastKey.key == key)
            {
                idx = f;
                old = true;
                break;
            }
            else if (idx >= 0 && lastKey.time < mLastKeys[idx].time)
            {
                idx = f;
            }
        }
        if (idx < 0)
        {
            idx = 0;
            for (int f = 0; f < MAX_LASTKEYS; f ++)
            {
                LastKey &lastKey = mLastKeys[f];
                if (lastKey.key == -1 ||  lastKey.time < mLastKeys[idx].time)
                    idx = f;
            }
        }

        if (idx < 0)
            idx = 0;

        LastKey &keyIdx = mLastKeys[idx];
        if (!old)
        {
            keyIdx.time = time;
            keyIdx.key = key;
            keyIdx.cnt = 0;
        }
        else
        {
            keyIdx.cnt++;
        }
    }
}

void Game::checkKeys()
{
    const int timeRange = 120;
    const int cntInTime = 130;

    if (!player_node || !player_node->getAttackType())
        return;

    const int time = cur_time;
    for (int f = 0; f < MAX_LASTKEYS; f ++)
    {
        LastKey &lastKey = mLastKeys[f];
        if (lastKey.key != -1)
        {
            if (lastKey.time + timeRange < time)
            {
                if (lastKey.cnt > cntInTime)
                    mValidSpeed = false;
                lastKey.key = -1;
            }
        }
    }
}

void Game::setValidSpeed()
{
    clearKeysArray();
    mValidSpeed = true;
}

void Game::clearKeysArray()
{
    for (int f = 0; f < MAX_LASTKEYS; f ++)
    {
        mLastKeys[f].time = 0;
        mLastKeys[f].key = -1;
        mLastKeys[f].cnt = 0;
    }
}

void Game::videoResized(const int width, const int height)
{
    if (viewport)
        viewport->setSize(width, height);
    if (windowMenu)
        windowMenu->setPosition(width - windowMenu->getWidth(), 0);
}
