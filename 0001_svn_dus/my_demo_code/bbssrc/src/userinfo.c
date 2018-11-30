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
$Id: userinfo.c,v 1.10 1999/07/31 12:30:49 edwardc Exp $
*/

#include "bbs.h"

extern time_t login_start_time;
extern char fromhost[60];
char   *
genpasswd();
char   *sysconf_str();

void
disply_userinfo(u, real)
struct userec *u;
int     real;
{
	int     num, diff;
	int     exp;
#ifdef REG_EXPIRED
	time_t  nextreg, now;
#endif

	move(2, 0);
	clrtobot();
	prints("ÄúµÄ´úºÅ     : %-40s", u->userid);
	if (real)
		prints("      ĞÔ±ğ : %s", (u->gender == 'M' ? "ÄĞ" : "Å®"));
	prints("\nÄúµÄêÇ³Æ     : %s\n", u->username);
	prints("ÕæÊµĞÕÃû     : %-40s", u->realname);
	if (real)
		prints("  ³öÉúÈÕÆÚ : %d/%d/%d", u->birthmonth, u->birthday, u->birthyear + 1900);
	prints("\n¾Ó×¡×¡Ö·     : %s\n", u->address);
	prints("µç×ÓÓÊ¼şĞÅÏä : %s\n", u->email);
	if (real) {
		prints("ÕæÊµ E-mail  : %s\n", u->reginfo);
		if HAS_PERM
			(PERM_ADMINMENU)
				prints("Ident ×ÊÁÏ   : %s\n", u->ident);
	}
	prints("ÖÕ¶Ë»úĞÎÌ¬   : %s\n", u->termtype);
	prints("ÕÊºÅ½¨Á¢ÈÕÆÚ : %s", ctime(&u->firstlogin));
	prints("×î½ü¹âÁÙÈÕÆÚ : %s", ctime(&u->lastlogin));
	if (real) {

		/* edwardc.990410 ÏÔÊ¾ÏÂ´ÎÉí·İÈ·ÈÏÊ±¼ä±È½ÏÊµÓÃ ? :p */
#ifndef REG_EXPIRED
		prints("Éí·İÈ·ÈÏÈÕÆÚ : %s", (u->lastjustify == 0) ? "Î´Ôø×¢²á\n" : ctime(&u->lastjustify));
#else
		prints("Éí·İÈ·ÈÏ     : %s", (u->lastjustify == 0) ? "Î´Ôø×¢²á" : "ÒÑÍê³É£¬ÓĞĞ§ÆÚÏŞ: ");
		nextreg = u->lastjustify + REG_EXPIRED * 86400;
		now = time(0);
		sprintf(genbuf, "£¬»¹ÓĞ %d Ìì\n", (nextreg - now) / 86400);
		prints("%s%s", (u->lastjustify == 0) ? "" : (char *) Ctime(&nextreg)
			,(u->lastjustify == 0) ? "\n" : genbuf);
#endif

		prints("×î½ü¹âÁÙ»úÆ÷ : %s\n", u->lasthost);
		prints("ÎÄÕÂÊıÄ¿     : %d\n", u->numposts);
		prints("Ë½ÈËĞÅÏä     : %d ·â\n", u->nummails);
	}
	prints("ÉÏÕ¾´ÎÊı     : %d ´Î\n", u->numlogins);
	prints("ÉÏÕ¾×ÜÊ±Êı   : %d Ğ¡Ê± %d ·ÖÖÓ\n", u->stay / 3600, (u->stay / 60) % 60);
	exp = countexp(u);
	prints("¾­ÑéÖµ       : %d  (%s)\n", exp, cexp(exp));
	exp = countperf(u);
	prints("±íÏÖÖµ       : %d  (%s)\n", exp, cperf(exp));
	if (real) {
		strcpy(genbuf, "bTCPRp#@XWBA#VS-DOM-F012345678\0");
		for (num = 0; num < strlen(genbuf) - 1; num++)
			if (!(u->userlevel & (1 << num)))
				genbuf[num] = '-';
		prints("Ê¹ÓÃÕßÈ¨ÏŞ   : %s\n", genbuf);
	} else {
		diff = (time(0) - login_start_time) / 60;
		prints("Í£ÁôÆÚ¼ä     : %d Ğ¡Ê± %02d ·Ö\n", diff / 60, diff % 60);
		prints("Ó©Ä»´óĞ¡     : %dx%d\n", t_lines, t_columns);
	}
	prints("\n");
	if (u->userlevel & PERM_BOARDS) {
		prints("  ÄúÊÇ±¾Õ¾µÄ°åÖ÷, ¸ĞĞ»ÄúµÄ¸¶³ö.\n");
	} else if (u->userlevel & PERM_LOGINOK) {
		prints("  ÄúµÄ×¢²á³ÌĞòÒÑ¾­Íê³É, »¶Ó­¼ÓÈë±¾Õ¾.\n");
	} else if (u->lastlogin - u->firstlogin < 3 * 86400) {
		prints("  ĞÂÊÖÉÏÂ·, ÇëÔÄ¶Á Announce ÌÖÂÛÇø.\n");
	} else {
		prints("  ×¢²áÉĞÎ´³É¹¦, ÇëÈş¿¼±¾Õ¾½øÕ¾»­ÃæËµÃ÷.\n");
	}
}


int
uinfo_query(u, real, unum)
struct userec *u;
int     real, unum;
{
	struct userec newinfo;
	char    ans[3], buf[STRLEN], genbuf[128];
	char    src[STRLEN], dst[STRLEN];
	int     i, fail = 0, netty_check = 0;
	time_t  now;
	struct tm *tmnow;
	memcpy(&newinfo, u, sizeof(currentuser));
	getdata(t_lines - 1, 0, real ?
		"ÇëÑ¡Ôñ (0)½áÊø (1)ĞŞ¸Ä×ÊÁÏ (2)Éè¶¨ÃÜÂë (3) ¸Ä ID ==> [0]" :
		"ÇëÑ¡Ôñ (0)½áÊø (1)ĞŞ¸Ä×ÊÁÏ (2)Éè¶¨ÃÜÂë (3) Ñ¡Ç©Ãûµµ ==> [0]",
		ans, 2, DOECHO, YEA);
	clear();
	refresh();

	now = time(0);
	tmnow = localtime(&now);

	i = 3;
	move(i++, 0);
	if (ans[0] != '3' || real)
		prints("Ê¹ÓÃÕß´úºÅ: %s\n", u->userid);

	switch (ans[0]) {
	case '1':
		move(1, 0);
		prints("ÇëÖğÏîĞŞ¸Ä,Ö±½Ó°´ <ENTER> ´ú±íÊ¹ÓÃ [] ÄÚµÄ×ÊÁÏ¡£\n");

		sprintf(genbuf, "êÇ³Æ [%s]: ", u->username);
		getdata(i++, 0, genbuf, buf, NAMELEN, DOECHO, YEA);
		if (buf[0])
			strncpy(newinfo.username, buf, NAMELEN);
		if (!real && buf[0])
			strncpy(uinfo.username, buf, 40);

		sprintf(genbuf, "ÕæÊµĞÕÃû [%s]: ", u->realname);
		getdata(i++, 0, genbuf, buf, NAMELEN, DOECHO, YEA);
		if (buf[0])
			strncpy(newinfo.realname, buf, NAMELEN);

		sprintf(genbuf, "¾Ó×¡µØÖ· [%s]: ", u->address);
		getdata(i++, 0, genbuf, buf, STRLEN - 10, DOECHO, YEA);
		if (buf[0])
			strncpy(newinfo.address, buf, NAMELEN);

		sprintf(genbuf, "µç×ÓĞÅÏä [%s]: ", u->email);
		getdata(i++, 0, genbuf, buf, NAMELEN, DOECHO, YEA);
		if (buf[0]) {
			netty_check = 1;
			strncpy(newinfo.email, buf, NAMELEN);
		}
		sprintf(genbuf, "ÖÕ¶Ë»úĞÎÌ¬ [%s]: ", u->termtype);
		getdata(i++, 0, genbuf, buf, 16, DOECHO, YEA);
		if (buf[0])
			strncpy(newinfo.termtype, buf, 16);

		sprintf(genbuf, "³öÉúÄê [%d]: ", u->birthyear + 1900);
		getdata(i++, 0, genbuf, buf, 5, DOECHO, YEA);
		if (buf[0] && atoi(buf) > tmnow->tm_year + 1805 && atoi(buf) < tmnow->tm_year + 1897)
			newinfo.birthyear = atoi(buf) - 1900;

		sprintf(genbuf, "³öÉúÔÂ [%d]: ", u->birthmonth);
		getdata(i++, 0, genbuf, buf, 3, DOECHO, YEA);
		if (buf[0] && atoi(buf) >= 1 && atoi(buf) <= 12)
			newinfo.birthmonth = atoi(buf);

		sprintf(genbuf, "³öÉúÈÕ [%d]: ", u->birthday);
		getdata(i++, 0, genbuf, buf, 3, DOECHO, YEA);
		if (buf[0] && atoi(buf) >= 1 && atoi(buf) <= 31)
			newinfo.birthday = atoi(buf);

		sprintf(genbuf, "ĞÔ±ğ [%c]: ", u->gender);
		getdata(i++, 0, genbuf, buf, 2, DOECHO, YEA);
		if (buf[0]) {
			if (strchr("MmFf", buf[0]))
				newinfo.gender = toupper(buf[0]);
		}
		if (real) {
			sprintf(genbuf, "ÕæÊµEmail[%s]: ", u->reginfo);
			getdata(i++, 0, genbuf, buf, 62, DOECHO, YEA);
			if (buf[0])
				strncpy(newinfo.reginfo, buf, STRLEN - 16);

			sprintf(genbuf, "ÉÏÏß´ÎÊı [%d]: ", u->numlogins);
			getdata(i++, 0, genbuf, buf, 10, DOECHO, YEA);
			if (atoi(buf) > 0)
				newinfo.numlogins = atoi(buf);

			sprintf(genbuf, "ÎÄÕÂÊıÄ¿ [%d]: ", u->numposts);
			getdata(i++, 0, genbuf, buf, 10, DOECHO, YEA);
			if (atoi(buf) > 0)
				newinfo.numposts = atoi(buf);

		}
		break;
	case '2':
		if (!real) {
			getdata(i++, 0, "ÇëÊäÈëÔ­ÃÜÂë: ", buf, PASSLEN, NOECHO, YEA);
			if (*buf == '\0' || !checkpasswd(u->passwd, buf)) {
				prints("\n\nºÜ±§Ç¸, ÄúÊäÈëµÄÃÜÂë²»ÕıÈ·¡£\n");
				fail++;
				break;
			}
		}
		getdata(i++, 0, "ÇëÉè¶¨ĞÂÃÜÂë: ", buf, PASSLEN, NOECHO, YEA);
		if (buf[0] == '\0') {
			prints("\n\nÃÜÂëÉè¶¨È¡Ïû, ¼ÌĞøÊ¹ÓÃ¾ÉÃÜÂë\n");
			fail++;
			break;
		}
		strncpy(genbuf, buf, PASSLEN);

		getdata(i++, 0, "ÇëÖØĞÂÊäÈëĞÂÃÜÂë: ", buf, PASSLEN, NOECHO, YEA);
		if (strncmp(buf, genbuf, PASSLEN)) {
			prints("\n\nĞÂÃÜÂëÈ·ÈÏÊ§°Ü, ÎŞ·¨Éè¶¨ĞÂÃÜÂë¡£\n");
			fail++;
			break;
		}
		buf[8] = '\0';
		strncpy(newinfo.passwd, genpasswd(buf), ENCPASSLEN);
		break;
	case '3':
		if (!real) {
			sprintf(genbuf, "Ä¿Ç°Ê¹ÓÃÇ©Ãûµµ [%d]: ", u->signature);
			getdata(i++, 0, genbuf, buf, 16, DOECHO, YEA);
			if (atoi(buf) > 0)
				newinfo.signature = atoi(buf);
		} else {
			getdata(i++, 0, "ĞÂµÄÊ¹ÓÃÕß´úºÅ: ", genbuf, IDLEN + 1, DOECHO, YEA);
			if (*genbuf != '\0') {
				if (getuser(genbuf)) {
					prints("\n´íÎó! ÒÑ¾­ÓĞÍ¬Ñù ID µÄÊ¹ÓÃÕß\n");
					fail++;
				} else {
					strncpy(newinfo.userid, genbuf, IDLEN + 2);
				}
			}
		}
		break;
	default:
		clear();
		return 0;
	}
	if (fail != 0) {
		pressreturn();
		clear();
		return 0;
	}
	if (askyn("È·¶¨Òª¸Ä±äÂğ", NA, YEA) == YEA) {
		if (real) {
			char    secu[STRLEN];
			sprintf(secu, "ĞŞ¸Ä %s µÄ»ù±¾×ÊÁÏ»òÃÜÂë¡£", u->userid);
			securityreport(secu);
		}
		if (strcmp(u->userid, newinfo.userid)) {

			sprintf(src, "mail/%c/%s", toupper(u->userid[0]), u->userid);
			sprintf(dst, "mail/%c/%s", toupper(newinfo.userid[0]), newinfo.userid);
			rename(src, dst);
			sethomepath(src, u->userid);
			sethomepath(dst, newinfo.userid);
			rename(src, dst);
			sethomefile(src, u->userid, "register");
			unlink(src);
			sethomefile(src, u->userid, "register.old");
			unlink(src);
			setuserid(unum, newinfo.userid);
		}
		if ((netty_check == 1)) {
			sprintf(genbuf, "%s", BBSHOST);
			if ((sysconf_str("EMAILFILE") != NULL) &&
				(!strstr(newinfo.email, genbuf)) &&
				(!invalidaddr(newinfo.email)) &&
				(!invalid_email(newinfo.email))) {
				strcpy(u->email, newinfo.email);
				send_regmail(u);
			} else {
				if (sysconf_str("EMAILFILE") != NULL) {
					move(t_lines - 5, 0);
					prints("\nÄúËùÌîµÄµç×ÓÓÊ¼şµØÖ· ¡¾[1;33m%s[m¡¿\n", newinfo.email);
					prints("²¢·ÇºÏ·¨Ö® UNIX ÕÊºÅ£¬ÏµÍ³²»»áÍ¶µİ×¢²áĞÅ£¬Çë°ÑËüĞŞÕıºÃ...\n");
					pressanykey();
					/*
					 * edwardc.990521 if there's wrong
					 * email, do not update anything.
					 */
					return 0;
				}
			}
		}
		memcpy(u, &newinfo, sizeof(newinfo));
		set_safe_record();
		if (netty_check == 1) {
			newinfo.userlevel &= ~(PERM_LOGINOK | PERM_PAGE);
			sethomefile(src, newinfo.userid, "register");
			sethomefile(dst, newinfo.userid, "register.old");
			rename(src, dst);
		}
		substitute_record(PASSFILE, &newinfo, sizeof(newinfo), unum);
	}
	clear();
	return 0;
}

void
x_info()
{
	modify_user_mode(GMENU);
	if (!strcmp("guest", currentuser.userid)) {
		disply_userinfo(&currentuser, 0);
		pressreturn();
		return;
	}
	disply_userinfo(&currentuser, 1);
	uinfo_query(&currentuser, 0, usernum);
}

void
getfield(line, info, desc, buf, len)
int     line, len;
char   *info, *desc, *buf;
{
	char    prompt[STRLEN];
	sprintf(genbuf, "  Ô­ÏÈÉè¶¨: %-46.46s [1;32m(%s)[m",
		(buf[0] == '\0') ? "(Î´Éè¶¨)" : buf, info);
	move(line, 0);
	prints(genbuf);
	sprintf(prompt, "  %s: ", desc);
	getdata(line + 1, 0, prompt, genbuf, len, DOECHO, YEA);
	if (genbuf[0] != '\0') {
		strncpy(buf, genbuf, len);
	}
	move(line, 0);
	clrtoeol();
	prints("  %s: %s\n", desc, buf);
	clrtoeol();
}

void
x_fillform()
{
	char    rname[NAMELEN], addr[STRLEN];
	char    phone[STRLEN], dept[STRLEN], assoc[STRLEN];
	char    ans[5], *mesg, *ptr;
	FILE   *fn;
	time_t  now;
	modify_user_mode(NEW);
	move(3, 0);
	clrtobot();
	if (!strcmp("guest", currentuser.userid)) {
		prints("±§Ç¸, ÇëÓÃ new ÉêÇëÒ»¸öĞÂÕÊºÅááÔÙÌîÉêÇë±í.");
		pressreturn();
		return;
	}
	if (currentuser.userlevel & PERM_LOGINOK) {
		prints("ÄúÒÑ¾­Íê³É±¾Õ¾µÄÊ¹ÓÃÕß×¢²áÊÖĞø, »¶Ó­¼ÓÈë±¾Õ¾µÄĞĞÁĞ.");
		pressreturn();
		return;
	}
	if ((fn = fopen("new_register", "r")) != NULL) {
		while (fgets(genbuf, STRLEN, fn) != NULL) {
			if ((ptr = strchr(genbuf, '\n')) != NULL)
				*ptr = '\0';
			if (strncmp(genbuf, "userid: ", 8) == 0 &&
				strcmp(genbuf + 8, currentuser.userid) == 0) {
				fclose(fn);
				prints("Õ¾³¤ÉĞÎ´´¦ÀíÄúµÄ×¢²áÉêÇëµ¥, ÇëÄÍĞÄµÈºò.");
				pressreturn();
				return;
			}
		}
		fclose(fn);
	}
	move(3, 0);
	if (askyn("ÄúÈ·¶¨ÒªÌîĞ´×¢²áµ¥Âğ", NA, NA) == NA)
		return;
	strncpy(rname, currentuser.realname, NAMELEN);
	strncpy(addr, currentuser.address, STRLEN);
	dept[0] = phone[0] = assoc[0] = '\0';
	while (1) {
		move(3, 0);
		clrtoeol();
		prints("%s ÄúºÃ, Çë¾İÊµÌîĞ´ÒÔÏÂµÄ×ÊÁÏ:\n", currentuser.userid);
		getfield(6, "ÇëÓÃÖĞÎÄ", "ÕæÊµĞÕÃû", rname, NAMELEN);
		getfield(8, "Ñ§Ğ£Ïµ¼¶»ò¹«Ë¾Ö°³Æ", "Ñ§Ğ£Ïµ¼¶", dept, STRLEN);
		getfield(10, "°üÀ¨ÇŞÊÒ»òÃÅÅÆºÅÂë", "Ä¿Ç°×¡Ö·", addr, STRLEN);
		getfield(12, "°üÀ¨¿ÉÁªÂçÊ±¼ä", "ÁªÂçµç»°", phone, STRLEN);
#ifdef NEED_ASSOC		/* edwardc.990410 i think it's not general
				 * enough */
		getfield(14, "Ğ£ÓÑ»á»ò±ÏÒµÑ§Ğ£", "Ğ£ ÓÑ »á", assoc, STRLEN);
#endif
		mesg = "ÒÔÉÏ×ÊÁÏÊÇ·ñÕıÈ·, °´ Q ·ÅÆú×¢²á (Y/N/Quit)? [N]: ";
		getdata(t_lines - 1, 0, mesg, ans, 3, DOECHO, YEA);
		if (ans[0] == 'Q' || ans[0] == 'q')
			return;
		if (ans[0] == 'Y' || ans[0] == 'y')
			break;
	}
	strncpy(currentuser.realname, rname, NAMELEN);
	strncpy(currentuser.address, addr, STRLEN);
	if ((fn = fopen("new_register", "a")) != NULL) {
		now = time(NULL);
		fprintf(fn, "usernum: %d, %s", usernum, ctime(&now));
		fprintf(fn, "userid: %s\n", currentuser.userid);
		fprintf(fn, "realname: %s\n", rname);
		fprintf(fn, "dept: %s\n", dept);
		fprintf(fn, "addr: %s\n", addr);
		fprintf(fn, "phone: %s\n", phone);
#ifdef NEED_ASSOC
		fprintf(fn, "assoc: %s\n", assoc);
#endif
		fprintf(fn, "----\n");
		fclose(fn);
	}
	setuserfile(genbuf, "mailcheck");
	if ((fn = fopen(genbuf, "w")) == NULL) {
		fclose(fn);
		return;
	}
	fprintf(fn, "usernum: %d\n", usernum);
	fclose(fn);
}
