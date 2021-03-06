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

#ifndef NET_TMWA_BUYINGSTOREHANDLER_H
#define NET_TMWA_BUYINGSTOREHANDLER_H

#include "net/buyingstorehandler.h"

namespace TmwAthena
{

class BuyingStoreHandler final : public Net::BuyingStoreHandler
{
    public:
        BuyingStoreHandler();

        A_DELETE_COPY(BuyingStoreHandler)

        void create(const std::string &name,
                    const int maxMoney,
                    const bool flag,
                    const std::vector<ShopItem*> &items) const override final
                    A_CONST;

        void close() const override final A_CONST;

        void open(const Being *const being) const override final A_CONST;

        void sell(const Being *const being,
                  const int storeId,
                  const Item *const item,
                  const int amount) const override final A_CONST;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_BUYINGSTOREHANDLER_H
