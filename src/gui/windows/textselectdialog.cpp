/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "gui/windows/textselectdialog.h"

#include "enums/gui/layouttype.h"

#include "gui/windows/setupwindow.h"

#include "gui/models/namesmodel.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

TextSelectDialog::TextSelectDialog(const std::string &name,
                                   const std::string &selectButton,
                                   const AllowQuit allowQuit) :
    // TRANSLATORS: sell dialog name
    Window(name, Modal_false, nullptr, "sell.xml"),
    ActionListener(),
    SelectionListener(),
    mSelectButtonName(selectButton),
    mText(),
    mSelectButton(nullptr),
    mQuitButton(nullptr),
    mItemList(nullptr),
    mScrollArea(nullptr),
    mModel(nullptr),
    mAllowQuit(allowQuit),
    mTag(0)
{
}

void TextSelectDialog::postInit()
{
    setWindowName("TextSelectDialog");
    setResizable(true);
    setCloseButton(mAllowQuit == AllowQuit_true);
    setStickyButtonLock(true);
    setMinWidth(260);
    setMinHeight(220);
    setDefaultSize(260, 230, ImagePosition::CENTER);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    setActionEventId("OK");
    mModel = new NamesModel;
    mItemList = CREATEWIDGETR(ListBox,
        this,
        mModel,
        "listbox.xml");
    mScrollArea = new ScrollArea(this, mItemList,
        fromBool(getOptionBool("showbackground"), Opaque),
        "sell_background.xml");
    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mSelectButton = new Button(this,
        mSelectButtonName,
        "select",
        this);
    if (mAllowQuit == AllowQuit_true)
    {
        // TRANSLATORS: sell dialog button
        mQuitButton = new Button(this, _("Quit"), "quit", this);
    }

    mSelectButton->setEnabled(false);

    mItemList->setDistributeMousePressed(false);
    mItemList->addSelectionListener(this);
    mItemList->setActionEventId("item");
    mItemList->addActionListener(this);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mScrollArea, 8, 5).setPadding(3);
    if (mQuitButton)
    {
        placer(6, 5, mSelectButton);
        placer(7, 5, mQuitButton);
    }
    else
    {
        placer(7, 5, mSelectButton);
    }

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    center();
    loadWindowState();

    setVisible(Visible_true);
    enableVisibleSound(true);
}

TextSelectDialog::~TextSelectDialog()
{
    delete2(mModel);
}

void TextSelectDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();

    if (eventId == "quit")
    {
        close();
        return;
    }

    const int selectedItem = mItemList->getSelected();

    // The following actions require a valid item selection
    if (selectedItem == -1 ||
        selectedItem >= mModel->getNumberOfElements())
    {
        return;
    }
    else if (eventId == "select")
    {
        const int index = mItemList->getSelected();
        if (index < 0 || index >= CAST_S32(mModel->size()))
            return;
        mText = mModel->getElementAt(index);
        distributeActionEvent();
        close();
    }
}

void TextSelectDialog::updateButtonsAndLabels()
{
    mSelectButton->setEnabled(mItemList->getSelected() > -1);
}

void TextSelectDialog::valueChanged(const SelectionEvent &event A_UNUSED)
{
    updateButtonsAndLabels();
}

void TextSelectDialog::setVisible(Visible visible)
{
    Window::setVisible(visible);

    if (visible == Visible_true)
    {
        if (mItemList)
            mItemList->requestFocus();
    }
    else
    {
        scheduleDelete();
    }
}

void TextSelectDialog::addText(const std::string &text)
{
    if (text.empty())
        return;
    mModel->add(text);
}
