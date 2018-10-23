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
$Id: delete.c,v 1.6 1999/09/07 03:30:32 edwardc Exp $
*/

#include "bbs.h"
#ifdef WITHOUT_ADMIN_TOOLS
#define kick_user
#endif

int
offline()
{
	char    buf[STRLEN];
	modify_user_mode(OFFLINE);
	clear();
	if (HAS_PERM(PERM_SYSOP) || HAS_PERM(PERM_BOARDS) || HAS_PERM(PERM_ADMINMENU)
		|| HAS_PERM(PERM_SEEULEVELS)) {
		move(1, 0);
		prints("\n\nƒ˙”–÷ÿ»Œ‘⁄…Ì, ≤ªƒ‹ÀÊ±„◊‘…±¿≤!!\n");
		pressreturn();
		clear();
		return;
	}
	if (currentuser.stay < 86400) {
		move(1, 0);
		prints("\n\n∂‘≤ª∆, ƒ˙ªπŒ¥πª◊ ∏Ò÷¥––¥À√¸¡Ó!!\n");
		prints("«Î mail ∏¯ SYSOP Àµ√˜◊‘…±‘≠“Ú, –ª–ª°£\n");
		pressreturn();
		clear();
		return;
	}
	getdata(1, 0, "«Î ‰»Îƒ„µƒ√‹¬Î: ", buf, PASSLEN, NOECHO, YEA);
	if (*buf == '\0' || !checkpasswd(currentuser.passwd, buf)) {
		prints("\n\n∫‹±ß«∏, ƒ˙ ‰»Îµƒ√‹¬Î≤ª’˝»∑°£\n");
		pressreturn();
		clear();
		return;
	}
	getdata(3, 0, "«ÎŒ ƒ„Ω– ≤˜·√˚◊÷? ", buf, NAMELEN, DOECHO, YEA);
	if (*buf == '\0' || strcmp(buf, currentuser.realname)) {
		prints("\n\n∫‹±ß«∏, Œ“≤¢≤ª»œ ∂ƒ„°£\n");
		pressreturn();
		clear();
		return;
	}
	clear();
	move(1, 0);
	prints("[1;5;31mæØ∏Ê[0;1;31m£∫ ◊‘…±··, ƒ˙Ω´Œﬁ∑®‘Ÿ”√¥À’ ∫≈Ω¯»Î±æ’æ£°£°");
	prints("\n\n\n[1;32mµ´’ ∫≈“™‘⁄ 30 ÃÏ··≤≈ª·…æ≥˝°£∫√ƒ—π˝‡∏ :( .....[m\n\n\n");
	if (askyn("ƒ„»∑∂®“™¿Îø™’‚∏ˆ¥Ûº“Õ•", NA, NA) == 1) {
		clear();
		currentuser.userlevel = 0;
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec), usernum);
		mail_info();
		modify_user_mode(OFFLINE);
		kick_user(&uinfo);
		exit(0);
	}
}
getuinfo(fn)
FILE   *fn;
{
	int     num;
	char    buf[40];
	fprintf(fn, "\n\nÀ˚µƒ¥˙∫≈     : %s\n", currentuser.userid);
	fprintf(fn, "À˚µƒÍ«≥∆     : %s\n", currentuser.username);
	fprintf(fn, "’Ê µ–’√˚     : %s\n", currentuser.realname);
	fprintf(fn, "æ”◊°◊°÷∑     : %s\n", currentuser.address);
	fprintf(fn, "µÁ◊”” º˛–≈œ‰ : %s\n", currentuser.email);
	fprintf(fn, "’Ê µ E-mail  : %s\n", currentuser.reginfo);
	fprintf(fn, "Ident ◊ ¡œ   : %s\n", currentuser.ident);
	fprintf(fn, "’ ∫≈Ω®¡¢»’∆⁄ : %s", ctime(&currentuser.firstlogin));
	fprintf(fn, "◊ÓΩ¸π‚¡Ÿ»’∆⁄ : %s", ctime(&currentuser.lastlogin));
	fprintf(fn, "◊ÓΩ¸π‚¡Ÿª˙∆˜ : %s\n", currentuser.lasthost);
	fprintf(fn, "…œ’æ¥Œ ˝     : %d ¥Œ\n", currentuser.numlogins);
	fprintf(fn, "Œƒ’¬ ˝ƒø     : %d\n", currentuser.numposts);
	fprintf(fn, "…œ’æ◊‹ ± ˝   : %d –° ± %d ∑÷÷”\n",
		currentuser.stay / 3600, (currentuser.stay / 60) % 60);
	strcpy(buf, "bTCPRp#@XWBA#VS-DOM-F012345678");
	for (num = 0; num < 30; num++)
		if (!(currentuser.userlevel & (1 << num)))
			buf[num] = '-';
	buf[num] = '\0';
	fprintf(fn, " π”√’ﬂ»®œﬁ   : %s\n\n", buf);
}
mail_info()
{
	FILE   *fn;
	time_t  now;
	char    filename[STRLEN];
	now = time(0);
	sprintf(filename, "tmp/suicide.%s", currentuser.userid);
	if ((fn = fopen(filename, "w")) != NULL) {
		fprintf(fn, "[1m%s[m “—æ≠‘⁄ [1m%24.24s[m µ«º«◊‘…±¡À£¨“‘œ¬ «À˚µƒ◊ ¡œ£¨«Î±£¡Ù...", currentuser.userid
			,ctime(&now));
		getuinfo(fn);
		fclose(fn);
		postfile(filename, "syssecurity", "µ«º«◊‘…±Õ®÷™(30ÃÏ··…˙–ß)...", 2);
		unlink(filename);
	}
	if ((fn = fopen(filename, "w")) != NULL) {
		fprintf(fn, "¥Ûº“∫√,\n\n");
		fprintf(fn, "Œ“ « %s (%s)°£Œ“º∫æ≠µ«º«‘⁄30ÃÏ··¿Îø™’‚¿Ô¡À°£\n\n",
			currentuser.userid, currentuser.username);
		fprintf(fn, "Œ“≤ªª·∏¸≤ªø…ƒ‹Õ¸º«◊‘ %s“‘¿¥‘⁄±æ’æ %d ¥Œ login ÷–◊‹π≤ %d ∑÷÷”∂∫¡Ù∆⁄º‰µƒµ„µ„µŒµŒ°£\n",
			ctime(&currentuser.firstlogin), currentuser.numlogins, currentuser.stay / 60);
		fprintf(fn, "«ÎŒ“µƒ∫√”—∞— %s ¥”ƒ„√«µƒ∫√”—√˚µ•÷–ƒ√µÙ∞…°£“ÚŒ™Œ“º∫æ≠√ª”–»®œﬁ‘Ÿ…œ’æ¡À!\n\n",
			currentuser.userid);
		fprintf(fn, "ªÚ–Ì”–≥Ø“ª»’Œ“ª·ªÿ¿¥µƒ°£ ’‰÷ÿ!! ‘Ÿº˚!!\n\n\n");
		fprintf(fn, "%s Ï∂ %24.24s ¡Ù.\n\n", currentuser.userid, ctime(&now));
		fclose(fn);
		postfile(filename, "notepad", "µ«º«◊‘…±¡Ù—‘...", 2);
		unlink(filename);
	}
}
