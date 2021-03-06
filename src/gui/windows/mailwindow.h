/*
 *  The ManaPlus Client
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

#ifndef GUI_WINDOWS_MAILWINDOW_H
#define GUI_WINDOWS_MAILWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class ExtendedListBox;
class ExtendedNamesModel;
class ScrollArea;

struct MailMessage;

/**
 * A dialog to choose between buying or selling at a shop.
 *
 * \ingroup Interface
 */
class MailWindow final : public Window,
                         public ActionListener
{
    public:
        MailWindow();

        A_DELETE_COPY(MailWindow)

        ~MailWindow();

        void action(const ActionEvent &event) override final;

        void addMail(MailMessage *const message);

        void clear() override final;

        void showMessage(MailMessage *const mail);

        void removeMail(const int id);

        void viewNext(const int id);

        void viewPrev(const int id);

        void mouseClicked(MouseEvent &event) override final;

        void postConnection();

        void createMail(const std::string &to);

    private:
        std::vector<MailMessage*> mMessages;
        std::map<int, MailMessage*> mMessagesMap;
        ExtendedNamesModel *mMailModel;
        ExtendedListBox *mListBox;
        ScrollArea *mListScrollArea;
        Button *mRefreshButton;
        Button *mNewButton;
        Button *mDeleteButton;
        Button *mReturnButton;
        Button *mOpenButton;
};

extern MailWindow *mailWindow;

#endif  // GUI_WINDOWS_MAILWINDOW_H
