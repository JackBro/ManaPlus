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

#ifndef NET_EATHENA_PROTOCOL_H
#define NET_EATHENA_PROTOCOL_H

/*********************************
 * Packets from server to client *
 *********************************/
#define SMSG_SERVER_PING             0x007f /**< Contains server tick */
#define SMSG_CONNECTION_PROBLEM      0x0081

#define SMSG_UPDATE_HOST             0x0063 /**< Custom update host packet */
#define SMSG_LOGIN_DATA              0x0069
#define SMSG_LOGIN_ERROR             0x006a
#define SMSG_LOGIN_ERROR2            0x083e

// unused
#define SMSG_CHAR_LOGIN              0x006b
#define SMSG_CHAR_LOGIN2             0x082d
#define SMSG_CHAR_LOGIN_ERROR        0x006c
#define SMSG_CHAR_CREATE_SUCCEEDED   0x006d
#define SMSG_CHAR_CREATE_SUCCEEDED2  0x0221
#define SMSG_CHAR_PINCODE_STATUS     0x08b9

#define SMSG_CHAR_CREATE_FAILED      0x006e
#define SMSG_CHAR_DELETE_SUCCEEDED   0x006f
#define SMSG_CHAR_DELETE_FAILED      0x0070
#define SMSG_CHAR_MAP_INFO           0x0071
// Custom packet reply to password change request
#define SMSG_CHAR_PASSWORD_RESPONSE  0x0062

#define SMSG_CHAR_SWITCH_RESPONSE    0x00b3
#define SMSG_CHANGE_MAP_SERVER       0x0092

#define SMSG_MAP_LOGIN_SUCCESS       0x02eb /**< Contains starting location */
#define SMSG_MAP_QUIT_RESPONSE       0x018b
#define SMSG_PLAYER_UPDATE_1         0x01d8
#define SMSG_PLAYER_UPDATE_2         0x01d9
#define SMSG_PLAYER_MOVE             0x01da /**< A nearby player moves */
#define SMSG_PLAYER_STOP             0x0088 /**< Stop walking, set position */
#define SMSG_PLAYER_MOVE_TO_ATTACK   0x0139 /**< Move to within attack range */
#define SMSG_PLAYER_STAT_UPDATE_1    0x00b0
#define SMSG_PLAYER_STAT_UPDATE_2    0x00b1
#define SMSG_PLAYER_STAT_UPDATE_3    0x0141
#define SMSG_PLAYER_STAT_UPDATE_4    0x00bc
#define SMSG_PLAYER_STAT_UPDATE_5    0x00bd
#define SMSG_PLAYER_STAT_UPDATE_6    0x00be
#define SMSG_PLAYER_GET_EXP          0x07f6
#define SMSG_WHO_ANSWER              0x00c2
#define SMSG_PLAYER_WARP             0x0091 /**< Warp player to map/location */
#define SMSG_PLAYER_INVENTORY        0x0991
#define SMSG_PLAYER_INVENTORY_ADD    0x0990
#define SMSG_PLAYER_INVENTORY_REMOVE 0x00af
#define SMSG_PLAYER_INVENTORY_REMOVE2 0x07fa
#define SMSG_PLAYER_INVENTORY_USE    0x01c8
#define SMSG_PLAYER_EQUIPMENT        0x0992
#define SMSG_PLAYER_EQUIP            0x0999
#define SMSG_PLAYER_UNEQUIP          0x099a
#define SMSG_PLAYER_ATTACK_RANGE     0x013a
#define SMSG_PLAYER_ARROW_EQUIP      0x013c
#define SMSG_PLAYER_ARROW_MESSAGE    0x013b
#define SMSG_PLAYER_SKILLS           0x010f
#define SMSG_PLAYER_SKILL_UP         0x010e
#define SMSG_SKILL_FAILED            0x0110
#define SMSG_SKILL_DAMAGE            0x01de
#define SMSG_SKILL_AUTO_CAST         0x0147
#define SMSG_ITEM_USE_RESPONSE       0x00a8
#define SMSG_ITEM_VISIBLE            0x009d /**< An item is on the floor */
#define SMSG_ITEM_DROPPED            0x084b /**< An item is dropped */
#define SMSG_ITEM_REMOVE             0x00a1 /**< An item disappers */
#define SMSG_BEING_VISIBLE           0x0857
#define SMSG_BEING_MOVE              0x0856 /**< A nearby monster moves */
#define SMSG_BEING_SPAWN             0x0858 /**< A being spawns nearby */
#define SMSG_BEING_MOVE2             0x0086 /**< New eAthena being moves */
#define SMSG_BEING_REMOVE            0x0080
#define SMSG_BEING_CHANGE_LOOKS      0x00c3
// Same as 0x00c3, but 16 bit ID
#define SMSG_BEING_CHANGE_LOOKS2     0x01d7
#define SMSG_BEING_SELFEFFECT        0x019b
#define SMSG_BEING_EMOTION           0x00c0
#define SMSG_BEING_ACTION            0x008a /**< Attack, sit, stand up, ... */
#define SMSG_BEING_ACTION2           0x02e1
#define SMSG_BEING_CHAT              0x008d /**< A being talks */
#define SMSG_COLOR_MESSAGE           0x02c1
#define SMSG_BEING_NAME_RESPONSE     0x0095 /**< Has to be requested */
#define SMSG_BEING_NAME_RESPONSE2    0x0220 /**< Has to be requested */
#define SMSG_BEING_CHANGE_DIRECTION  0x009c
#define SMSG_BEING_RESURRECT         0x0148

#define SMSG_PLAYER_STATUS_CHANGE    0x0119
#define SMSG_PLAYER_GUILD_PARTY_INFO 0x0195
#define SMSG_BEING_STATUS_CHANGE     0x0983
#define SMSG_BEING_STATUS_CHANGE2    0x043f

#define SMSG_NPC_MESSAGE             0x00b4
#define SMSG_NPC_NEXT                0x00b5
#define SMSG_NPC_CLOSE               0x00b6
#define SMSG_NPC_CHOICE              0x00b7 /**< Display a choice */
#define SMSG_NPC_BUY_SELL_CHOICE     0x00c4
#define SMSG_NPC_BUY                 0x00c6
#define SMSG_NPC_SELL                0x00c7
#define SMSG_NPC_BUY_RESPONSE        0x00ca
#define SMSG_NPC_SELL_RESPONSE       0x00cb
#define SMSG_NPC_INT_INPUT           0x0142 /**< Integer input */
#define SMSG_NPC_STR_INPUT           0x01d4 /**< String input */
#define SMSG_NPC_SHOW_PROGRESS_BAR   0x02f0
#define SMSG_PLAYER_CHAT             0x008e /**< Player talks */
#define SMSG_WHISPER                 0x0097 /**< Whisper Recieved */
#define SMSG_WHISPER_RESPONSE        0x09df
#define SMSG_GM_CHAT                 0x009a /**< GM announce */
#define SMSG_GM_CHAT2                0x01c3
#define SMSG_WALK_RESPONSE           0x0087

// Receiving a request to trade
#define SMSG_TRADE_REQUEST           0x01f4
#define SMSG_TRADE_RESPONSE          0x00e7
#define SMSG_TRADE_RESPONSE2         0x01f5
#define SMSG_TRADE_ITEM_ADD          0x080f
#define SMSG_TRADE_ITEM_ADD_RESPONSE 0x01b1 /**< Not standard eAthena! */
#define SMSG_TRADE_OK                0x00ec
#define SMSG_TRADE_CANCEL            0x00ee
#define SMSG_TRADE_COMPLETE          0x00f0

#define SMSG_PARTY_CREATE            0x00fa
#define SMSG_PARTY_INFO              0x00fb
#define SMSG_PARTY_INVITE_RESPONSE   0x02c5
#define SMSG_PARTY_INVITED           0x00fe
#define SMSG_PARTY_SETTINGS          0x07d8
#define SMSG_PARTY_MOVE              0x0104
#define SMSG_PARTY_LEAVE             0x0105
#define SMSG_PARTY_UPDATE_HP         0x0106
#define SMSG_PARTY_UPDATE_COORDS     0x0107
#define SMSG_PARTY_MESSAGE           0x0109
#define SMSG_PARTY_INVITATION_STATS  0x02c9
#define SMSG_PARTY_MEMBER_INFO       0x01e9

#define SMSG_PLAYER_STORAGE_ITEMS    0x0995 /**< Item list for storage */
#define SMSG_PLAYER_STORAGE_EQUIP    0x0996 /**< Equipment list for storage */
#define SMSG_PLAYER_STORAGE_STATUS   0x00f2 /**< Slots used and total slots */
#define SMSG_PLAYER_STORAGE_ADD      0x01c4 /**< Add item/equip to storage */
// Remove item/equip from storage
#define SMSG_PLAYER_STORAGE_REMOVE   0x00f6
#define SMSG_PLAYER_STORAGE_CLOSE    0x00f8 /**< Storage access closed */

#define SMSG_ADMIN_KICK_ACK          0x00cd

#define SMSG_GUILD_CREATE_RESPONSE   0x0167
#define SMSG_GUILD_POSITION_INFO     0x016c
#define SMSG_GUILD_MEMBER_LOGIN      0x016d
#define SMSG_GUILD_MASTER_OR_MEMBER  0x014e
#define SMSG_GUILD_BASIC_INFO        0x01b6
#define SMSG_GUILD_ALIANCE_INFO      0x014c
#define SMSG_GUILD_MEMBER_LIST       0x0154
#define SMSG_GUILD_POS_NAME_LIST     0x0166
#define SMSG_GUILD_POS_INFO_LIST     0x0160
#define SMSG_GUILD_POSITION_CHANGED  0x0174
#define SMSG_GUILD_MEMBER_POS_CHANGE 0x0156
#define SMSG_GUILD_EMBLEM            0x0152
#define SMSG_GUILD_SKILL_INFO        0x0162
#define SMSG_GUILD_NOTICE            0x016f
#define SMSG_GUILD_INVITE            0x016a
#define SMSG_GUILD_INVITE_ACK        0x0169
#define SMSG_GUILD_LEAVE             0x015a
#define SMSG_GUILD_EXPULSION         0x015c
#define SMSG_GUILD_EXPULSION_LIST    0x0163
#define SMSG_GUILD_MESSAGE           0x017f
#define SMSG_GUILD_SKILL_UP          0x010e
#define SMSG_GUILD_REQ_ALLIANCE      0x0171
#define SMSG_GUILD_REQ_ALLIANCE_ACK  0x0173
#define SMSG_GUILD_DEL_ALLIANCE      0x0184
#define SMSG_GUILD_OPPOSITION_ACK    0x0181
#define SMSG_GUILD_BROKEN            0x015e

#define SMSG_QUEST_ADD               0x02b3
#define SMSG_QUEST_LIST              0x02b1
#define SMSG_QUEST_LIST_OBJECTIVES   0x02b2
#define SMSG_QUEST_UPDATE_OBJECTIVES 0x02b5
#define SMSG_QUEST_REMOVE            0x02b4

#define SMSG_MVP                     0x010c
#define SMSG_RANKS_LIST              0x097d
#define SMSG_MONSTER_HP              0x0977
#define SMSG_PLAYER_HP               0x080e

#define SMSG_MAIL_OPEN_WINDOW        0x0260

#define SMSG_PET_MESSAGE             0x01aa

/**********************************
 *  Packets from client to server *
 **********************************/

#define CMSG_LOGIN_REGISTER          0x0064
#define CMSG_NAME_REQUEST            0x096a

// Custom change password packet
#define CMSG_CHAR_PASSWORD_CHANGE    0x0061
#define CMSG_CHAR_SERVER_CONNECT     0x0065
#define CMSG_CHAR_SELECT             0x0066
#define CMSG_CHAR_CREATE             0x0970
#define CMSG_CHAR_DELETE             0x0068
#define CMSG_CHAR_CREATE_PIN         0x08ba

#define CMSG_MAP_SERVER_CONNECT      0x022d
#define CMSG_CLIENT_PING             0x035f /**< Send to server with tick */
#define CMSG_MAP_LOADED              0x007d
#define CMSG_CLIENT_QUIT             0x018A

#define CMSG_CHAT_MESSAGE            0x00f3
#define CMSG_CHAT_WHISPER            0x0096
#define CMSG_CHAT_WHO                0x00c1

#define CMSG_CREAYE_CHAT_ROOM        0x00d5

#define CMSG_SKILL_LEVELUP_REQUEST   0x0112
#define CMSG_STAT_UPDATE_REQUEST     0x00bb
#define CMSG_SKILL_USE_BEING         0x083c
#define CMSG_SKILL_USE_POSITION      0x0438
// Variant of 0x116 with 80 char string at end (unsure of use)
#define CMSG_SKILL_USE_POSITION_MORE 0x096a
#define CMSG_SKILL_USE_MAP           0x011b

#define CMSG_PLAYER_INVENTORY_USE    0x0439
#define CMSG_PLAYER_INVENTORY_DROP   0x0362
#define CMSG_PLAYER_EQUIP            0x0998
#define CMSG_PLAYER_UNEQUIP          0x00ab
#define CMSG_PLAYER_USE_CARD         0x017a
#define CMSG_PLAYER_INSERT_CARD      0x017c

#define CMSG_ITEM_PICKUP             0x07e4
#define CMSG_PLAYER_CHANGE_DIR       0x0202
#define CMSG_PLAYER_CHANGE_DEST      0x0437
#define CMSG_PLAYER_CHANGE_ACT       0x0369
#define CMSG_PLAYER_RESTART          0x00b2
#define CMSG_PLAYER_EMOTE            0x00bf
#define CMSG_PLAYER_ATTACK           0x0369
#define CMSG_PLAYER_STOP_ATTACK      0x0118
#define CMSG_WHO_REQUEST             0x00c1

#define CMSG_NPC_TALK                0x0090
#define CMSG_NPC_NEXT_REQUEST        0x00b9
#define CMSG_NPC_CLOSE               0x0146
#define CMSG_NPC_LIST_CHOICE         0x00b8
#define CMSG_NPC_INT_RESPONSE        0x0143
#define CMSG_NPC_STR_RESPONSE        0x01d5
#define CMSG_NPC_BUY_SELL_REQUEST    0x00c5
#define CMSG_NPC_BUY_REQUEST         0x00c8
#define CMSG_NPC_SELL_REQUEST        0x00c9

#define CMSG_TRADE_REQUEST           0x00e4
#define CMSG_TRADE_RESPONSE          0x00e6
#define CMSG_TRADE_ITEM_ADD_REQUEST  0x00e8
#define CMSG_TRADE_CANCEL_REQUEST    0x00ed
#define CMSG_TRADE_ADD_COMPLETE      0x00eb
#define CMSG_TRADE_OK                0x00ef

#define CMSG_PARTY_CREATE            0x00f9
#define CMSG_PARTY_CREATE2           0x01e8
#define CMSG_PARTY_INVITE            0x00fc
#define CMSG_PARTY_INVITE2           0x0802
#define CMSG_PARTY_INVITED           0x00ff
#define CMSG_PARTY_INVITED2          0x02c7
#define CMSG_PARTY_LEAVE             0x0100
#define CMSG_PARTY_SETTINGS          0x0102
#define CMSG_PARTY_KICK              0x0103
#define CMSG_PARTY_MESSAGE           0x0108
#define CMSG_PARTY_CHANGE_LEADER     0x07da

#define CMSG_MOVE_TO_STORAGE         0x07ec /** Move item to storage */
#define CSMG_MOVE_FROM_STORAGE       0x0364 /** Remove item from storage */
#define CMSG_CLOSE_STORAGE           0x0193 /** Request storage close */

#define CMSG_MOVE_TO_CART            0x0126
#define CMSG_MOVE_FROM_CART          0x0127
#define CMSG_CHANGE_CART             0x01af
#define CMSG_MOVE_FROM_STORAGE_TO_CART 0x0128
#define CMSG_MOVE_FROM_CART_TO_STORAGE 0x0129

#define CMSG_ADMIN_ANNOUNCE          0x0099
#define CMSG_ADMIN_LOCAL_ANNOUNCE    0x019C
#define CMSG_ADMIN_HIDE              0x019D
#define CMSG_ADMIN_KICK              0x00CC
#define CMSG_ADMIN_KICK_ALL          0x00ce
#define CMSG_ADMIN_MUTE              0x0149
#define CMSG_ADMIN_RESET_PLAYER      0x0197
#define CMSG_ADMIN_GOTO              0x01bb
#define CMSG_ADMIN_RECALL            0x01bd
#define CMSG_ADMIN_MUTE              0x0149
#define CMSG_ADMIN_MUTE_NAME         0x0212

#define CMSG_GUILD_CHECK_MASTER      0x014d
#define CMSG_GUILD_REQUEST_INFO      0x014f
#define CMSG_GUILD_REQUEST_EMBLEM    0x0151
#define CMSG_GUILD_CHANGE_EMBLEM     0x0153
#define CMSG_GUILD_CHANGE_MEMBER_POS 0x0155
#define CMSG_GUILD_LEAVE             0x0159
#define CMSG_GUILD_EXPULSION         0x015b
#define CMSG_GUILD_BREAK             0x015d
#define CMSG_GUILD_CHANGE_POS_INFO   0x0161
#define CMSG_GUILD_CREATE            0x0165
#define CMSG_GUILD_INVITE            0x0168
#define CMSG_GUILD_INVITE_REPLY      0x016b
#define CMSG_GUILD_CHANGE_NOTICE     0x016e
#define CMSG_GUILD_ALLIANCE_REQUEST  0x0170
#define CMSG_GUILD_ALLIANCE_REPLY    0x0172
#define CMSG_GUILD_MESSAGE           0x017e
#define CMSG_GUILD_OPPOSITION        0x0180
#define CMSG_GUILD_ALLIANCE_DELETE   0x0183

#define CMSG_SOLVE_CHAR_NAME         0x0368
#define CMSG_IGNORE_ALL              0x00d0
#define CMSG_REQUEST_RANKS           0x097c
#define CMSG_SET_SHORTCUTS           0x02ba
#define CMSG_NPC_COMPLETE_PROGRESS_BAR 0x02f1
#define CMSG_NPC_PRODUCE_MIX         0x018e
#define CMSG_NPC_COOKING             0x025b
#define CMSG_NPC_REPAIR              0x01fd
#define CMSG_NPC_REFINE              0x0222
#define CMSG_NPC_IDENTIFY            0x0178
#define CMSG_NPC_SELECT_ARROW        0x01ae
#define CMSG_NPC_SELECT_AUTO_SPELL   0x01ce

#define CMSG_PLAYER_MAPMOVE          0x0140
#define CMSG_REMOVE_OPTION           0x012a
#define CMSG_PLAYER_SET_MEMO         0x011d

#define CMSG_PET_CATCH               0x019f
#define CMSG_PET_REQUEST_STATE       0x01a9
#define CMSG_PET_SET_NAME            0x01a5

#define SMSG_SOLVE_CHAR_NAME         0x0194
#define SMSG_SKILL_CASTING           0x07fb
#define SMSG_SKILL_CAST_CANCEL       0x01b9
#define SMSG_SKILL_NO_DAMAGE         0x011a

#define SMSG_BEING_IP_RESPONSE       0x020c
#define SMSG_PVP_MAP_MODE            0x0199
#define SMSG_PVP_SET                 0x019a
#define SMSG_MAP_TYPE_PROPERTY2      0x099b
#define SMSG_IGNORE_ALL_RESPONSE     0x00d2
#define SMSG_ONLINE_LIST             0x0211
#define SMSG_NPC_COMMAND             0x0212

#define CMSG_ONLINE_LIST             0x0210
#define CMSG_SET_STATUS              0x0213

#define SMSG_UPDATE_HOST2            0x7534

#define SMSG_MAP_ACCOUNT_ID          0x0283
#define SMSG_PLAYER_SHORTCUTS        0x07d9
#define SMSG_PLAYER_SHOW_EQUIP       0x02da
#define SMSG_NPC_CUTIN               0x01b3
#define SMSG_NPC_VIEWPOINT           0x0144

#endif  // NET_EATHENA_PROTOCOL_H
