/*
 * functions.h		-- include file for define functions individual
 *	
 * Copyright (c) 1999, Edward Ping-Da Chuang <edwardc@edwardc.dhs.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * CVS: $Id: functions.h,v 1.14 1999/10/01 14:29:22 edwardc Exp $
 */

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#define RNDSIGN
/* 乱数签名档 */

//#define MAGIC_PASS
/* 站长级 ID 上站询问 Magic Pass */

#define REFUSE_LESS60SEC
/* 60 秒内不可重覆 login */

#define TALK_LOG
/* 聊天纪录功能 */

#define CODE_VALID
/* 暗码认证 */

//#define MSG_CANCLE_BY_CTRL_C
/* 用 ctrl-c 来否略讯息 */

#define LOG_MY_MESG
/* 讯息纪录中纪录自己所发出的讯息 */

#define COLOR_POST_DATE
/* 文章日期颜色 */

#define BIGGER_MOVIE
/* 加大活动看板空间 (七行) */

#define ALWAYS_SHOW_BRDNOTE
/* 每次进板都会 show 出进板画面 */

#define BBSD
/* 使用 BBS daemon, 关掉则可使用 in.zbbsd, telnetd 或使用 bbsrf */

//#define MUDCHECK_BEFORELOGIN
/* 类似某些 MUD server 会要求使用者登入之前按键来确定不是使用程式来连线
   Ctrl + C 有时可改为 Ctrl + D, Ctrl + Z .. */
   
#endif /* _FUNCTIONS_H_ */
