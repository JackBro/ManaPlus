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

#include "gui/focushandler.h"

#include "gui/gui.h"

#include "gui/widgets/window.h"

#include "debug.h"

void FocusHandler::requestModalFocus(gcn::Widget *widget)
{
    /* If there is another widget with modal focus, remove its modal focus
     * and put it on the modal widget stack.
     */
    if (mModalFocusedWidget && mModalFocusedWidget != widget)
    {
        mModalStack.push_front(mModalFocusedWidget);
        mModalFocusedWidget = nullptr;
    }

    gcn::FocusHandler::requestModalFocus(widget);
}

void FocusHandler::releaseModalFocus(gcn::Widget *widget)
{
    mModalStack.remove(widget);

    if (mModalFocusedWidget == widget)
    {
        gcn::FocusHandler::releaseModalFocus(widget);

        /* Check if there were any previously modal widgets that'd still like
         * to regain their modal focus.
         */
        if (!mModalStack.empty())
        {
            gcn::FocusHandler::requestModalFocus(mModalStack.front());
            mModalStack.pop_front();
        }
    }
}

void FocusHandler::remove(gcn::Widget *widget)
{
    releaseModalFocus(widget);

    gcn::FocusHandler::remove(widget);
}

void FocusHandler::tabNext()
{
    gcn::FocusHandler::tabNext();

    checkForWindow();
}

void FocusHandler::tabPrevious()
{
    gcn::FocusHandler::tabPrevious();

    checkForWindow();
}

void FocusHandler::checkForWindow() const
{
    if (mFocusedWidget)
    {
        gcn::Widget *widget = mFocusedWidget->getParent();

        while (widget)
        {
            Window *const window = dynamic_cast<Window*>(widget);

            if (window)
            {
                window->requestMoveToTop();
                break;
            }

            widget = widget->getParent();
        }
    }
}

void FocusHandler::distributeFocusGainedEvent(const gcn::Event &focusEvent)
{
    if (gui)
        gui->distributeGlobalFocusGainedEvent(focusEvent);
    gcn::FocusHandler::distributeFocusGainedEvent(focusEvent);
}
