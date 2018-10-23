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
$Id: xyz.c,v 1.26 1999/09/07 03:30:33 edwardc Exp $
*/

#define EXTERN
#include "bbs.h"
int     use_define = 0;
extern int iscolor;

extern struct BCACHE *brdshm;
#define TH_LOW	10
#define TH_HIGH	15

int
modify_user_mode(mode)
int     mode;
{
	uinfo.mode = mode;
	update_ulist(&uinfo, utmpent);
	return 0;
}

int
showperminfo(pbits, i)
int     pbits, i;
{
	char    buf[STRLEN];
	sprintf(buf, "%c. %-30s %2s", 'A' + i, (use_define) ? user_definestr[i] : permstrings[i],
		((pbits >> i) & 1 ? "  " : "¡Á"));
	move(i + 6 - ((i > 15) ? 16 : 0), 0 + ((i > 15) ? 40 : 0));
	prints(buf);
	refresh();
	return YEA;
}

unsigned int
setperms(pbits, prompt, numbers, showfunc)
unsigned int pbits;
char   *prompt;
int     numbers;
int     (*showfunc) ();
{
	int     lastperm = numbers - 1;
	int     i, done = NA;
	char    choice[3], buf[80];
	move(4, 0);
	prints("Çë°´ÏÂÄãÒªµÄ´úÂëÀ´Éè¶¨%s£¬°´ Enter ½áÊø.\n", prompt);
	move(6, 0);
	clrtobot();
	for (i = 0; i <= lastperm; i++) {
		(*showfunc) (pbits, i, NA);
	}
	while (!done) {
		sprintf(buf, "Ñ¡Ôñ(ENTER ½áÊø%s): ", ((strcmp(prompt, "È¨ÏŞ") != 0)) ? "" : "£¬0 Í£È¨");
		getdata(t_lines - 1, 0, buf, choice, 2, DOECHO, YEA);
		*choice = toupper(*choice);
		if (*choice == '0')
			return (0);
		else if (*choice == '\n' || *choice == '\0')
			done = YEA;
		else if (*choice < 'A' || *choice > 'A' + lastperm)
			bell();
		else {
			i = *choice - 'A';
			pbits ^= (1 << i);
			if ((*showfunc) (pbits, i, YEA) == NA) {
				pbits ^= (1 << i);
			}
		}
	}
	return (pbits);
}

int
x_userdefine()
{
	int     id;
	unsigned int newlevel;
	extern int nettyNN;
	modify_user_mode(USERDEF);
	if (!(id = getuser(currentuser.userid))) {
		move(3, 0);
		prints("´íÎóµÄÊ¹ÓÃÕß ID...");
		clrtoeol();
		pressreturn();
		clear();
		return 0;
	}
	move(1, 0);
	clrtobot();
	move(2, 0);
	use_define = 1;
	newlevel = setperms(lookupuser.userdefine, "ÈşÊı", NUMDEFINES, showperminfo);
	move(2, 0);
	if (newlevel == lookupuser.userdefine)
		prints("ÈşÊıÃ»ÓĞĞŞ¸Ä...\n");
	else {
		lookupuser.userdefine = newlevel;
		currentuser.userdefine = newlevel;
		substitute_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
		uinfo.pager |= FRIEND_PAGER;
		if (!(uinfo.pager & ALL_PAGER)) {
			if (!DEFINE(DEF_FRIENDCALL))
				uinfo.pager &= ~FRIEND_PAGER;
		}
		uinfo.pager &= ~ALLMSG_PAGER;
		uinfo.pager &= ~FRIENDMSG_PAGER;
		if (DEFINE(DEF_FRIENDMSG)) {
			uinfo.pager |= FRIENDMSG_PAGER;
		}
		if (DEFINE(DEF_ALLMSG)) {
			uinfo.pager |= ALLMSG_PAGER;
			uinfo.pager |= FRIENDMSG_PAGER;
		}
		update_utmp();
		if (DEFINE(DEF_ACBOARD))
			nettyNN = NNread_init();
		prints("ĞÂµÄÈşÊıÉè¶¨Íê³É...\n\n");
	}
	iscolor = (DEFINE(DEF_COLOR)) ? 1 : 0;
	pressreturn();
	clear();
	use_define = 0;
	return 0;
}

int
x_cloak()
{
	modify_user_mode(GMENU);
	report("toggle cloak");
	uinfo.invisible = (uinfo.invisible) ? NA : YEA;
	update_utmp();
	if (!uinfo.in_chat) {
		move(1, 0);
		clrtoeol();
		prints("ÒşÉíÊõ (cloak) ÒÑ¾­%sÁË!",
			(uinfo.invisible) ? "Æô¶¯" : "Í£Ö¹");
		pressreturn();
	}
	return 0;
}

int
x_state()
{
#if defined(LINUX)
	char    buf[80];
	modify_user_mode(XMENU);
	sprintf(buf, "tmp/procinfo.%s", currentuser.userid);
	sprintf(genbuf, "/usr/local/bin/procinfo -F %s", buf);
	system(genbuf);
	ansimore(buf, YEA);
	unlink(buf);
#endif
	return FULLUPDATE;
}

void
x_edits()
{
	int     aborted;
	char    ans[7], buf[STRLEN];
	int     ch, num, confirm;
	static char *e_file[] =
	{"plans", "signatures", "notes", "logout", NULL};
	static char *explain_file[] =
	{"¸öÈËËµÃ÷µµ", "Ç©Ãûµµ", "×Ô¼ºµÄ±¸ÍüÂ¼", "ÀëÕ¾µÄ»­Ãæ", NULL};
	modify_user_mode(GMENU);
	clear();
	move(1, 0);
	prints("±àĞŞ¸öÈËµµ°¸\n\n");
	for (num = 0; e_file[num] != NULL && explain_file[num] != NULL; num++) {
		prints("[[1;32m%d[m] %s\n", num + 1, explain_file[num]);
	}
	prints("[[1;32m%d[m] ¶¼²»Ïë¸Ä\n", num + 1);

	getdata(num + 5, 0, "ÄãÒª±àĞŞÄÄÒ»Ïî¸öÈËµµ°¸: ", ans, 2, DOECHO, YEA);
	if (ans[0] - '0' <= 0 || ans[0] - '0' > num || ans[0] == '\n' || ans[0] == '\0')
		return;

	ch = ans[0] - '0' - 1;
	setuserfile(genbuf, e_file[ch]);
	move(3, 0);
	clrtobot();
	sprintf(buf, "(E)±à¼­ (D)É¾³ı %s? [E]: ", explain_file[ch]);
	getdata(3, 0, buf, ans, 2, DOECHO, YEA);
	if (ans[0] == 'D' || ans[0] == 'd') {
		confirm = askyn("ÄãÈ·¶¨ÒªÉ¾³ıÕâ¸öµµ°¸", NA, NA);
		if (confirm != 1) {
			move(5, 0);
			prints("È¡ÏûÉ¾³ıĞĞ¶¯\n");
			pressreturn();
			clear();
			return;
		}
		unlink(genbuf);
		move(5, 0);
		prints("%s ÒÑÉ¾³ı\n", explain_file[ch]);
		sprintf(buf, "delete %s", explain_file[ch]);
		report(buf);
		pressreturn();
		clear();
		return;
	}
	modify_user_mode(EDITUFILE);
	aborted = vedit(genbuf, NA);
	clear();
	if (!aborted) {
		prints("%s ¸üĞÂ¹ı\n", explain_file[ch]);
		sprintf(buf, "edit %s", explain_file[ch]);
		if (!strcmp(e_file[ch], "signatures")) {
			set_numofsig();
			prints("ÏµÍ³ÖØĞÂÉè¶¨ÒÔ¼°¶ÁÈëÄãµÄÇ©Ãûµµ...");
		}
		report(buf);
	} else
		prints("%s È¡ÏûĞŞ¸Ä\n", explain_file[ch]);
	pressreturn();
}

void
x_lockscreen()
{
	char    buf[PASSLEN + 1];
	time_t  now;
	modify_user_mode(LOCKSCREEN);
	move(9, 0);
	clrtobot();
	update_endline();
	buf[0] = '\0';
	now = time(0);
	move(9, 0);
	prints("\n[1;37m       _       _____   ___     _   _   ___     ___       __");
	prints("\n      ( )     (  _  ) (  _`\\  ( ) ( ) (  _`\\  (  _`\\    |  |");
	prints("\n      | |     | ( ) | | ( (_) | |/'/' | (_(_) | | ) |   |  |");
	prints("\n      | |  _  | | | | | |  _  | , <   |  _)_  | | | )   |  |");
	prints("\n      | |_( ) | (_) | | (_( ) | |\\`\\  | (_( ) | |_) |   |==|");
	prints("\n      (____/' (_____) (____/' (_) (_) (____/' (____/'   |__|[m\n");
	prints("\n[1;36mÓ©Ä»ÒÑÔÚ[33m %19s[36m Ê±±»[32m %-12s [36mÔİÊ±Ëø×¡ÁË...[m", ctime(&now), currentuser.userid);
	while (*buf == '\0' || !checkpasswd(currentuser.passwd, buf)) {
		move(18, 0);
		clrtobot();
		update_endline();
		getdata(19, 0, "ÇëÊäÈëÄãµÄÃÜÂëÒÔ½âËø: ", buf, PASSLEN, NOECHO, YEA);
	}
}

heavyload()
{
#ifndef BBSD
	double  cpu_load[3];
	get_load(cpu_load);
	if (cpu_load[0] > 15)
		return 1;
	else
		return 0;
#else
 #ifdef chkload
	register int load;
	register time_t uptime;
	
	if ( time(0) > uptime ) {
		load = chkload(load ? TH_LOW : TH_HIGH );
		uptime = time(0) + load + 45;
	}
	return load;
 #else
 	return 0;
 #endif
#endif		
}

void
exec_cmd(umode, pager, cmdfile, param1)
int     umode, pager;
char   *cmdfile, *param1;
{
	char    buf[STRLEN * 2];
	int     save_pager;
	if (num_useshell() >= 20) {
		clear();
		prints("Ì«¶àÈËÊ¹ÓÃÍâ²¿³ÌÊ½ÁË£¬ÄãµÈÒ»ÏÂÔÙÓÃ°É...");
		pressanykey();
		return;
	}
	if (!HAS_PERM(PERM_SYSOP) && heavyload()) {
		clear();
		prints("±§Ç¸£¬Ä¿Ç°ÏµÍ³¸ººÉ¹ıÖØ£¬´Ë¹¦ÄÜÔİÊ±²»ÄÜÖ´ĞĞ...");
		pressanykey();
		return;
	}
	if (!dashf(cmdfile)) {
		move(2, 0);
		prints("no %s\n", cmdfile);
		pressreturn();
		return;
	}
	save_pager = uinfo.pager;
	if (pager == NA) {
		uinfo.pager = 0;
	}
	modify_user_mode(umode);
	sprintf(buf, "/bin/sh %s %s %s %d", cmdfile, param1, currentuser.userid, getpid());
	report(buf);
	reset_tty();
	do_exec(buf, NULL);
	restore_tty();
	uinfo.pager = save_pager;
	clear();
}
#ifdef IRC
void
t_irc()
{
	exec_cmd(IRCCHAT, NA, "bin/irc.sh", "");
}
#endif				/* IRC */

void
x_www()
{
	exec_cmd(WWW, NA, "bin/www.sh", "");
}

void
x_hytelnet()
{
	exec_cmd(HYTELNET, NA, "bin/hytelnet.sh", "");
}
/* ppfoong */
void
x_excearchie()
{
	char    buf[STRLEN];
	char   *s;
	if (heavyload()) {
		clear();
		prints("±§Ç¸£¬Ä¿Ç°ÏµÍ³¸ººÉ¹ıÖØ£¬´Ë¹¦ÄÜÔİÊ±²»ÄÜÖ´ĞĞ...");
		pressanykey();
		return;
	}
	modify_user_mode(ARCHIE);
	clear();
	prints("\n\n[1;33m       _______ ______ ______ _______ _______ _______");
	prints("\n      |   _   |   __ \\      |   |   |_     _|    ___|");
	prints("\n      |       |      <   ---|       |_|   |_|    ___|");
	prints("\n      |___|___|___|__|______|___|___|_______|_______|[0m");
	prints("\n\n\n»¶Ó­Ê¹ÓÃ ARCHIE ¹¦ÄÜ");
	prints("\n\n±¾¹¦ÄÜ½«ÎªÄúÁĞ³öÔÚÄÄ¸ö FTP Õ¾´æÓĞÄúÓûÑ°ÕÒµÄµµ°¸.");
	prints("\n\nÇëÊäÈëÓûËÑÑ°µÄ×Ö´®, »òÖ±½Ó°´ <ENTER> È¡Ïû¡£");
	getdata(13, 0, ">", buf, 20, DOECHO, YEA);
	if (buf[0] == '\0') {
		prints("\nÈ¡ÏûËÑÑ°.....\n");
		pressanykey();
		return;
	}
	for (s = buf; *s != '\0'; s++) {
		if (isspace(*s)) {
			prints("\nÒ»´ÎÖ»ÄÜËÑÑ°Ò»¸ö×Ö´®À², ²»ÄÜÌ«Ì°ĞÄà¸!!");
			pressanykey();
			return;
		}
	}
	exec_cmd(ARCHIE, YEA, "bin/archie.sh", buf);
	sprintf(buf, "tmp/archie.%s.%05d", currentuser.userid, uinfo.pid);
	if (1 /* dashf(buf) */ ) {
		if (askyn("Òª½«½á¹û¼Ä»ØĞÅÏäÂğ", NA, NA) == YEA)
			mail_file(buf, currentuser.userid, "ARCHIE ËÑÑ°½á¹û");
		ansimore(buf, YEA);
		unlink(buf);
	}
}
/* ppfoong */
void
x_dict()
{
	char    buf[STRLEN];
	char   *s;
	int     whichdict;
	if (heavyload()) {
		clear();
		prints("±§Ç¸£¬Ä¿Ç°ÏµÍ³¸ººÉ¹ıÖØ£¬´Ë¹¦ÄÜÔİÊ±²»ÄÜÖ´ĞĞ...");
		pressanykey();
		return;
	}
	modify_user_mode(DICT);
	clear();
	prints("\n[1;32m     _____  __        __   __");
	prints("\n    |     \\|__|.----.|  |_|__|.-----.-----.---.-.----.--.--.");
	prints("\n    |  --  |  ||  __||   _|  ||  _  |     |  _  |   _|  |  |");
	prints("\n    |_____/|__||____||____|__||_____|__|__|___._|__| |___  |");
	prints("\n                                                     |_____|[m");
	prints("\n\n\n»¶Ó­Ê¹ÓÃ±¾Õ¾µÄ×Öµä¡£");
	prints("\n±¾×ÖµäÖ÷ÒªÎª[1;33m¡¸Ó¢ºº¡¹[m²¿·Ö, µ«Òà¿É×÷[1;33m¡¸ººÓ¢¡¹[m²éÑ¯¡£");
	prints("\n\nÏµÍ³½«¸ù¾İÄúËùÊäÈëµÄ×Ö´®, ×Ô¶¯ÅĞ¶ÏÄúËùÒª·­²éµÄÊÇÓ¢ÎÄ×Ö»¹ÊÇÖĞÎÄ×Ö¡£");
	prints("\n\n\nÇëÊäÈëÄúÓû·­²éµÄÓ¢ÎÄ×Ö»òÖĞÎÄ×Ö, »òÖ±½Ó°´ <ENTER> È¡Ïû¡£");
	getdata(15, 0, ">", buf, 30, DOECHO, YEA);
	if (buf[0] == '\0') {
		prints("\nÄú²»Ïë²éÁËà¸...");
		pressanykey();
		return;
	}
	for (s = buf; *s != '\0'; s++) {
		if (isspace(*s)) {
			prints("\nÒ»´ÎÖ»ÄÜ²éÒ»¸ö×ÖÀ², ²»ÄÜÌ«Ì°ĞÄà¸!!");
			pressanykey();
			return;
		}
	}
	whichdict = YEA;
	for (s = buf; *s != '\0'; s++) {
		if (!(isalpha(*s) || *s == '-')) {
			whichdict = NA;
			break;
		}
	}
	if (whichdict)
		exec_cmd(DICT, YEA, "bin/cdict.sh", buf);
	else
		exec_cmd(DICT, YEA, "bin/edict.sh", buf);
	sprintf(buf, "tmp/dict.%s.%05d", currentuser.userid, uinfo.pid);
	if (dashf(buf)) {
		if (askyn("Òª½«½á¹û¼Ä»ØĞÅÏäÂğ", NA, NA) == YEA)
			mail_file(buf, currentuser.userid, "×Öµä²éÑ¯½á¹û");
		ansimore(buf, YEA);
		unlink(buf);
	}
}
/* Add By P.P.Foong */
void
x_sysinfo()
{
	char    buf[STRLEN];
	sprintf(buf, "tmp/sysinfo.%s", currentuser.userid);
	if (!dashf(buf)) {
		exec_cmd(SYSINFO, YEA, "bin/sysinfo.sh", "");
		ansimore(buf, YEA);
	}
	unlink(buf);
}
/* Add By P.P.Foong */
void
x_game()
{
	exec_cmd(GAME, YEA, "bin/game.sh", "");
}

void
x_showuser()
{
	char    buf[STRLEN];
	modify_user_mode(SYSINFO);
	clear();
	stand_title("±¾Õ¾Ê¹ÓÃÕß×ÊÁÏ²éÑ¯");
	ansimore("etc/showuser.msg", NA);
	getdata(20, 0, "Parameter: ", buf, 30, DOECHO, YEA);
	if ((buf[0] == '\0') || dashf("tmp/showuser.result"))
		return;
	securityreport("²éÑ¯Ê¹ÓÃÕß×ÊÁÏ");
	exec_cmd(SYSINFO, YEA, "bin/showuser.sh", buf);
	sprintf(buf, "tmp/showuser.result");
	if (dashf(buf)) {
		mail_file(buf, currentuser.userid, "Ê¹ÓÃÕß×ÊÁÏ²éÑ¯½á¹û");
		unlink(buf);
	}
}

void
ent_bnet()
{
	clear();
	exec_cmd(BBSNET, YEA, "bin/bbsnet.sh", "");
}

void
fill_date()
{
	time_t  now, next;
	char   *buf, *buf2, *index, index_buf[10];
	char    d_value[20];
	char    h[3], m[3], s[3];
	int     foo, foo2, i, flag = 0;
	struct tm *mytm;
	FILE   *fp;
	now = time(0);
	resolve_boards();

	if (now < brdshm->fresh_date && strlen(brdshm->date) != 0)
		return;

	mytm = localtime(&now);
	strftime(h, 3, "%H", mytm);
	strftime(m, 3, "%M", mytm);
	strftime(s, 3, "%S", mytm);

	next = (time_t) time(0) - ((atoi(h) * 3600) + (atoi(m) * 60) + atoi(s)) +
		86400;		/* Ëã³ö½ñÌì 0:0:00 µÄÊ±¼ä, È»ááÔÙÍùáá¼ÓÒ»Ìì */

	sprintf(genbuf, "¼ÍÄîÈÕ¸üĞÂ, ÏÂÒ»´Î¸üĞÂÊ±¼ä %s", Cdate(&next));
	report(genbuf);

	buf = (char *) malloc(80);
	buf2 = (char *) malloc(80);
	index = (char *) malloc(10);
	bzero(d_value, 20);

	fp = fopen(DEF_FILE, "r");

	if (fp == NULL)
		return;

	now = time(0);
	mytm = localtime(&now);
	strftime(index_buf, 10, "%m%d", mytm);

	while (fgets(buf, 80, fp) != NULL) {
		if (buf[0] == ';' || buf[0] == '#' || buf[0] == ' ')
			continue;

		sscanf(buf, "%s %s", index, buf2);

		if (*index == 0 || *buf2 == 0)
			continue;

		if (strcmp(index, "0000") == 0 || strcmp(index_buf, index) == 0) {

			foo = strlen(buf2);

			if (foo < 14 && foo > 0) {

				foo2 = (14 - foo) / 2;
				if (foo2 > 1) {

					strcpy(brdshm->date, "");

					for (i = 0; i < foo2; i++)
						strcat(brdshm->date, " ");

					strcat(brdshm->date, buf2);

					for (i = 0; i < 14 - (foo + foo2); i++)
						strcat(brdshm->date, " ");

				} else {
					strcpy(brdshm->date, buf2);
				}

			} else {
				strcpy(brdshm->date, buf2);
			}

			if (strcmp(index, "0000") == 0) {
				strcpy(d_value, brdshm->date);
			} else {
				flag = 1;
			}

		}
	}

	fclose(fp);

	if (flag == 0) {
		if (d_value[0] == '\0')
			strcpy(brdshm->date, DEF_VALUE);
		else
			strcpy(brdshm->date, d_value);
	}
	brdshm->fresh_date = next;

	free(buf);
	free(buf2);
	free(index);

	return;
}

int
is_birth(user)
struct userec user;
{
	struct tm *tm;
	time_t now;
	
	now = time(0);
	tm = localtime(&now);

	if ( strcasecmp(user.userid, "guest") == 0 )
		return NA;
		
	if ( user.birthmonth == (tm->tm_mon + 1) && user.birthday == tm->tm_mday )
		return YEA;
	else
		return NA;
}
