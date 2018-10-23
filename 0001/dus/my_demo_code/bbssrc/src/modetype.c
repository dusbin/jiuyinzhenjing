/*
    Pirate Bulletin Board System
    Copyright (C) 1990, Edward Luke, lush@Athena.EE.MsState.EDU
    Eagles Bulletin Board System
    Copyright (C) 1992, Raymond Rocker, rocker@rock.b11.ingr.com
                        Guy Vega, gtvega@seabass.st.usm.edu
                        Dominic Tynes, dbtynes@seabass.st.usm.edu
    Firebird Bulletin Board System
    Copyright (C) 1996, Hsien-Tsung Chang, Smallpig.bbs@bbs.cs.ccu.edu.tw
                        Peng Piaw Foong, ppfoong@csie.ncu.edu.tw

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/*
$Id: modetype.c,v 1.3 1999/07/31 12:30:48 edwardc Exp $
*/

#include "modes.h"

char   *
ModeType(mode)
int     mode;
{
	switch (mode) {
	case IDLE:
		return "";
	case NEW:
		return "新站友注册";
	case LOGIN:
		return "进入本站";
	case DIGEST:
		return "浏览精华区";
	case MMENU:
		return "主选单";
	case ADMIN:
		return "管理者选单";
	case SELECT:
		return "选择讨论区";
	case READBRD:
		return "览遍天下";
	case READNEW:
		return "览新文章";
	case READING:
		return "品味文章";
	case POSTING:
		return "发表文章";
	case MAIL:
		return "处理信笺";
	case SMAIL:
		return "寄语信鸽";
	case RMAIL:
		return "阅览信笺";
	case TMENU:
		return "聊天选单";
	case LUSERS:
		return "环顾四方";
	case FRIEND:
		return "寻找好友";
	case MONITOR:
		return "探视民情";
	case QUERY:
		return "查询网友";
	case TALK:
		return "聊天";
	case PAGE:
		return "呼叫";
	case CHAT1:
		return "Chat1";
	case CHAT2:
		return "Chat2";
	case CHAT3:
		return "Chat3";
	case CHAT4:
		return "Chat4";
	case IRCCHAT:
		return "会谈IRC";
	case LAUSERS:
		return "探视网友";
	case XMENU:
		return "系统资讯";
	case VOTING:
		return "投票";
	case BBSNET:
		return "BBSNET";
	case EDITWELC:
		return "编辑Welc";
	case EDITUFILE:
		return "编辑个人档";
	case EDITSFILE:
		return "编修系统档";
	case ZAP:
		return "订阅讨论区";
	case GAME:
		return "脑力激荡";
	case SYSINFO:
		return "检查系统";
	case ARCHIE:
		return "ARCHIE";
	case DICT:
		return "翻查字典";
	case LOCKSCREEN:
		return "萤幕锁定";
	case NOTEPAD:
		return "留言板";
	case GMENU:
		return "工具箱";
	case MSG:
		return "送讯息";
	case USERDEF:
		return "自订叁数";
	case EDIT:
		return "修改文章";
	case OFFLINE:
		return "自杀中..";
	case EDITANN:
		return "编修精华";
	case WWW:
		return "悠游 WWW";
	case HYTELNET:
		return "Hytelnet";
	case CCUGOPHER:
		return "他站精华";
	case LOOKMSGS:
		return "察看讯息";
	case WFRIEND:
		return "寻人名册";
	case WNOTEPAD:
		return "欲走还留";
	case BBSPAGER:
		return "网路传呼";
	default:
		return "去了那里!?";
	}
}
