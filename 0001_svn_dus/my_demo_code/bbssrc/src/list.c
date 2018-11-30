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
$Id: list.c,v 1.7 1999/09/09 04:38:18 edwardc Exp $
*/

#include "bbs.h"
#define BBS_PAGESIZE    (19)
#define refreshtime     (30)
extern time_t login_start_time;
extern char BoardName[];
int     (*func_list_show) ();
time_t  update_time = 0;
int     freshmode = 0;
int     toggle1 = 0, toggle2 = 0;
int     mailmode, numf;
int     friendmode = 0;
int     usercounter, real_user_names = 0;
int     range, page, readplan, num;
struct user_info *user_record[MAXACTIVE];
struct userec *user_data;

#ifdef WITHOUT_ADMIN_TOOLS
#define kick_user
#endif

int
friend_search(uid, uentp, tblsize)
unsigned short uid;
struct user_info *uentp;
int     tblsize;
{
	int     hi, low, mid;
	int     cmp;
	if (uid == 0) {
		return NA;
	}
	hi = tblsize - 1;
	low = 0;
	while (low <= hi) {
		mid = (low + hi) / 2;
		cmp = uentp->friend[mid] - uid;
		if (cmp == 0) {
			return YEA;
		}
		if (cmp > 0)
			hi = mid - 1;
		else
			low = mid + 1;
	}
	return NA;
}

int
myfriend(uid)
unsigned short uid;
{
	return friend_search(uid, &uinfo, uinfo.fnum);
}

int
hisfriend(uentp)
struct user_info *uentp;
{
	return friend_search(uinfo.uid, uentp, uentp->fnum);
}

int
isreject(uentp)
struct user_info *uentp;
{
	int     i;
	if (HAS_PERM(PERM_SYSOP))
		return NA;
	if (uentp->uid != uinfo.uid) {
		for (i = 0; i < MAXREJECTS && (uentp->reject[i] || uinfo.reject[i]); i++) {
			if (uentp->reject[i] == uinfo.uid || uentp->uid == uinfo.reject[i])
				return YEA;	/* ±ª…ËŒ™∫⁄√˚µ• */
		}
	}
	return NA;
}
print_title()
{

	docmdtitle((friendmode) ? "[∫√≈Û”—¡–±Ì]" : "[ π”√’ﬂ¡–±Ì]",
		" ¡ƒÃÏ[[1;32mt[m] ºƒ–≈[[1;32mm[m] ÀÕ—∂œ¢[[1;32ms[m] º”,ºı≈Û”—[[1;32mo[m,[1;32md[m] ø¥Àµ√˜µµ[[1;32m°˙[m,[1;32mRtn[m] «–ªªƒ£ Ω [[1;32mf[m] «Ûæ»[[1;32mh[m]");
	update_endline();
}
print_title2()
{

	docmdtitle((friendmode) ? "[∫√≈Û”—¡–±Ì]" : "[ π”√’ﬂ¡–±Ì]",
		"        ºƒ–≈[[1;32mm[m] º”,ºı≈Û”—[[1;32mo[m,[1;32md[m] ø¥Àµ√˜µµ[[1;32m°˙[m,[1;32mRtn[m] —°‘Ò[[1;32m°¸[m,[1;32m°˝[m] «Ûæ»[[1;32mh[m]");
	update_endline();
}

void
update_data()
{
	if (readplan == YEA)
		return;
	if (time(0) >= update_time + refreshtime - 1) {
		freshmode = 1;
	}
	signal(SIGALRM, update_data);
	alarm(refreshtime);
	return;
}


int
print_user_info_title()
{
	char    title_str[512];
	char   *field_2;
	move(2, 0);
	clrtoeol();
	field_2 = " π”√’ﬂÍ«≥∆";
	if (real_user_names)
		field_2 = "’Ê µ–’√˚  ";
	sprintf(title_str,
		"[1;44m%s%-12.12s %-16.16s %-16.16s %c %c %-16.16s %5s[m\n",
		"±‡∫≈  ", " π”√’ﬂ¥˙∫≈", field_2, "¿¥◊‘", 'P',
		 /* ((HAS_PERM(PERM_SYSOP|PERM_SEECLOAK)) ? 'C' : ' ') */ 'M', "∂ØÃ¨",
#ifdef SHOW_IDLE_TIME
		" ±:∑÷");
#else
		"");
#endif
	prints("%s", title_str);
	return 0;
}
show_message(msg)
char    msg[];
{

	move(BBS_PAGESIZE + 3, 0);
	clrtoeol();
	if (msg != NULL)
		prints("[1m%s[m", msg);
	refresh();
}

void 
swap_user_record(a, b)
int     a, b;
{
	struct user_info *c;
	c = user_record[a];
	user_record[a] = user_record[b];
	user_record[b] = c;
}

void
sort_user_record(left, right)
int     left, right;
{
	int     i, last;
	if (left >= right)
		return;
	swap_user_record(left, (left + right) / 2);
	last = left;
	for (i = left + 1; i <= right; i++)
		if (strcasecmp(user_record[i]->userid, user_record[left]->userid) < 0)
			swap_user_record(++last, i);
	swap_user_record(left, last);
	sort_user_record(left, last - 1);
	sort_user_record(last + 1, right);
}

int
fill_userlist()
{
	static int i, i2;
	extern struct UTMPFILE *utmpshm;
	resolve_utmp();
	i2 = 0;
	for (i = 0; i < USHM_SIZE; i++) {
		if (!utmpshm->uinfo[i].active || !utmpshm->uinfo[i].pid
			|| isreject(&utmpshm->uinfo[i])) {
			continue;
		}
		if (!(HAS_PERM(PERM_SYSOP | PERM_SEECLOAK)) && utmpshm->uinfo[i].invisible) {
			continue;
		}
		if (friendmode && !myfriend(utmpshm->uinfo[i].uid)) {
			continue;
		}
		user_record[i2] = &utmpshm->uinfo[i];
		i2++;
	}
	if (!friendmode) {
		int     n;
		numf = 0;
		/* put all friends together */
		for (n = 0; n < i2; n++) {
			if (myfriend(user_record[n]->uid)) {
				swap_user_record(numf++, n);
			}
		}
		sort_user_record(0, numf - 1);
		sort_user_record(numf, i2 - 1);
	} else
		sort_user_record(0, i2 - 1);
	range = i2;
	return i2 == 0 ? -1 : 1;
}

int
do_userlist()
{
	int     i, j;
	char    user_info_str[STRLEN * 2] /* ,pagec */ ;
	int     override;
	struct user_info *uentp;
	move(3, 0);
	print_user_info_title();

	/* edwardc.990909 fix for ≤ª◊„––µƒŒ Ã‚ */
	
	for (i = 0, j = 0; j < BBS_PAGESIZE && i + page < range; i++) {
		uentp = user_record[i + page];
		override = (i + page < numf) || friendmode;
		if (readplan == YEA) {
			return 0;
		}
		if (!uentp->active || !uentp->pid ) {
			continue;	/* ƒ≥»À’˝«…¿Îø™ */
		}
		if (uentp != NULL) {
			sprintf(user_info_str,
				" %3d%2s%s%-12.12s%s %-16.16s %-16.16s %c %c %s%-16.16s[m %5.5s\n",
				i + 1 + page, (override) ? "  " : "", (override) ? "[1;32m" : "", uentp->userid
				,(override) ? "[m" : "", (real_user_names) ? uentp->realname : uentp->username,
				uentp->from, pagerchar(hisfriend(uentp), uentp->pager),
				msgchar(uentp), (uentp->invisible == YEA) ? "[1;36m" : "",
				modestring(uentp->mode, uentp->destuid, 1,
					(uentp->in_chat ? uentp->chatid : NULL)),
#ifdef SHOW_IDLE_TIME
				idle_str(uentp));
#else
				"");
#endif
			clrtoeol();
			prints("%s", user_info_str);
			j++;
		}
	}
	return 0;
}

int
show_userlist()
{
	if (update_time + refreshtime < time(0)) {
		fill_userlist();
		update_time = time(0);
	}
	if (range == 0 /* ||fill_userlist() == 0 */ ) {
		move(2, 0);
		prints("√ª”– π”√’ﬂ£®≈Û”—£©‘⁄¡–±Ì÷–...\n");
		clrtobot();
		if (friendmode) {
			move(BBS_PAGESIZE + 3, 0);
			if (askyn(" «∑Ò◊™ªª≥… π”√’ﬂƒ£ Ω", YEA, NA) == YEA) {
				range = num_visible_users();
				page = -1;
				friendmode = NA;
				return 1;
			}
		} else
			pressanykey();
		return -1;
	}
	do_userlist();
	clrtobot();
	return 1;
}

void
t_rusers()
{
	real_user_names = 1;
	t_users();
	real_user_names = 0;
}

int
deal_key(ch, allnum, pagenum)
char    ch;
int     allnum, pagenum;
{
	char    buf[STRLEN];
	static int msgflag;
	extern int friendflag;
	char    desc[5];
	if (msgflag == YEA) {
		show_message(NULL);
		msgflag = NA;
	}
	switch (ch) {
	case 'c':
	case 'C':
		if (strcmp(currentuser.userid, user_record[allnum]->userid))
			return 0;
		strcpy(buf, "");
		getdata(BBS_PAGESIZE + 3, 0, "±‰ªªÍ«≥∆: ", buf, NAMELEN, DOECHO, NA);
		if (buf[0] != '\0') {
			strcpy(uinfo.username, buf);
		}
		break;
	case 'k':
	case 'K':
		if (!HAS_PERM(PERM_SYSOP) && strcmp(currentuser.userid,
				user_record[allnum]->userid))
			return 1;
		sprintf(buf, "ƒ„“™∞— %s Ãﬂ≥ˆ’æÕ‚¬",
			user_record[allnum]->userid);
		move(BBS_PAGESIZE + 3, 0);
		if (askyn(buf, NA, NA) == NA)
			break;
		if ((int)kick_user(user_record[allnum]) == 1) {
			sprintf(buf, "%s “—±ªÃﬂ≥ˆ’æÕ‚",
				user_record[allnum]->userid);
		} else {
			sprintf(buf, "%s Œﬁ∑®Ãﬂ≥ˆ’æÕ‚",
				user_record[allnum]->userid);
		}
		msgflag = YEA;
		break;
	case 'h':
	case 'H':
		show_help("help/userlisthelp");
		break;
	case 't':
	case 'T':
		if (!HAS_PERM(PERM_PAGE))
			return 1;
		if (strcmp(currentuser.userid,
				user_record[allnum]->userid))
			ttt_talk(user_record[allnum]);
		else
			return 1;
		break;
	case 'm':
	case 'M':
		if (!HAS_PERM(PERM_POST))
			return 1;
		prints("%s\n", user_record[allnum]->userid);
		pressanykey();
		m_send(user_record[allnum]->userid);
		break;
	case 'f':
	case 'F':
		if (friendmode)
			friendmode = NA;
		else
			friendmode = YEA;
		update_time = 0;
		break;
	case 's':
	case 'S':
		if (!HAS_PERM(PERM_PAGE))
			return 1;
		if (!canmsg(user_record[allnum])) {
			sprintf(buf, "%s “—æ≠πÿ±’—∂œ¢∫ÙΩ–∆˜",
				user_record[allnum]->userid);
			msgflag = YEA;
			break;
		}
		do_sendmsg(user_record[allnum], NULL, 0, user_record[allnum]->pid);
		break;
	case 'o':
	case 'O':
	case 'r':
	case 'R':
		if (ch == 'o' || ch == 'O') {
			friendflag = YEA;
			strcpy(desc, "∫√”—");
		} else {
			friendflag = NA;
			strcpy(desc, "ªµ»À");
		}
		if (!strcmp("guest", currentuser.userid))
			return 0;
		sprintf(buf, "»∑∂®“™∞— %s º”»Î%s√˚µ•¬",
			user_record[allnum]->userid, desc);
		move(BBS_PAGESIZE + 3, 0);
		if (askyn(buf, NA, NA) == NA)
			break;
		if (addtooverride(user_record[allnum]->userid)
			== -1) {
			sprintf(buf, "%s “—‘⁄%s√˚µ•",
				user_record[allnum]->userid, desc);
		} else {
			sprintf(buf, "%s ¡–»Î%s√˚µ•",
				user_record[allnum]->userid, desc);
		}
		msgflag = YEA;
		break;
	case 'd':
	case 'D':
		sprintf(buf, "»∑∂®“™∞— %s ¥”∫√”—√˚µ•…æ≥˝¬",
			user_record[allnum]->userid);
		move(BBS_PAGESIZE + 3, 0);
		if (askyn(buf, NA, NA) == NA)
			break;
		if (deleteoverride(user_record[allnum]->userid, "friends")
			== -1) {
			sprintf(buf, "%s ±æ¿¥æÕ≤ª‘⁄≈Û”—√˚µ•÷–",
				user_record[allnum]->userid);
		} else {
			sprintf(buf, "%s “—¥”≈Û”—√˚µ•“∆≥˝",
				user_record[allnum]->userid);
		}
		msgflag = YEA;
		break;
	default:
		return 0;
	}
	if (friendmode)
		modify_user_mode(FRIEND);
	else
		modify_user_mode(LUSERS);
	if (readplan == NA) {
		print_title();
		clrtobot();
		if (show_userlist() == -1)
			return -1;
		if (msgflag) {
			show_message(buf);
		}
		update_endline();
	}
	return 1;
}

int
deal_key2(ch, allnum, pagenum)
char    ch;
int     allnum, pagenum;
{
	char    buf[STRLEN];
	static int msgflag;
	if (msgflag == YEA) {
		show_message(NULL);
		msgflag = NA;
	}
	switch (ch) {
	case 'h':
	case 'H':
		show_help("help/usershelp");
		break;
	case 'm':
	case 'M':
		if (!HAS_PERM(PERM_POST))
			return 1;
		m_send(user_data[allnum - pagenum].userid);
		break;
	case 'o':
	case 'O':
		if (!strcmp("guest", currentuser.userid))
			return 0;
		sprintf(buf, "»∑∂®“™∞— %s º”»Î∫√”—√˚µ•¬",
			user_data[allnum - pagenum].userid);
		move(BBS_PAGESIZE + 3, 0);
		if (askyn(buf, NA, NA) == NA)
			break;
		if (addtooverride(user_data[allnum - pagenum].userid)
			== -1) {
			sprintf(buf, "%s “—‘⁄≈Û”—√˚µ•",
				user_data[allnum - pagenum].userid);
			show_message(buf);
		} else {
			sprintf(buf, "%s ¡–»Î≈Û”—√˚µ•",
				user_data[allnum - pagenum].userid);
			show_message(buf);
		}
		msgflag = YEA;
		if (!friendmode)
			return 1;
		break;
	case 'f':
	case 'F':
		toggle1++;
		if (toggle1 >= 3)
			toggle1 = 0;
		break;
	case 't':
	case 'T':
		toggle2++;
		if (toggle2 >= 3)
			toggle2 = 0;
		break;
	case 'd':
	case 'D':
		sprintf(buf, "»∑∂®“™∞— %s ¥”∫√”—√˚µ•…æ≥˝¬",
			user_data[allnum - pagenum].userid);
		move(BBS_PAGESIZE + 3, 0);
		if (askyn(buf, NA, NA) == NA)
			break;
		if (deleteoverride(user_data[allnum - pagenum].userid, "friends")
			== -1) {
			sprintf(buf, "%s ±æ¿¥æÕ≤ª‘⁄≈Û”—√˚µ•÷–",
				user_data[allnum - pagenum].userid);
			show_message(buf);
		} else {
			sprintf(buf, "%s “—¥”≈Û”—√˚µ•“∆≥˝",
				user_data[allnum - pagenum].userid);
			show_message(buf);
		}
		msgflag = YEA;
		if (!friendmode)
			return 1;
		break;
	default:
		return 0;
	}
	modify_user_mode(LAUSERS);
	if (readplan == NA) {
		print_title2();
		move(3, 0);
		clrtobot();
		if (Show_Users() == -1)
			return -1;
		update_endline();
	}
	redoscr();
	return 1;
}

int
countusers(uentp)
struct userec *uentp;
{
	static int totalusers;
	char    permstr[10];
	if (uentp == NULL) {
		int     c = totalusers;
		totalusers = 0;
		return c;
	}
	if (uentp->numlogins != 0 && uleveltochar(permstr, uentp->userlevel) != 0)
		totalusers++;
	return 0;
}
printuent(uentp)
struct userec *uentp;
{
	static int i;
	char    permstr[10];
	char    msgstr[18];
	int     override;
	if (uentp == NULL) {
		printutitle();
		i = 0;
		return 0;
	}
	if (uentp->numlogins == 0 ||
		uleveltochar(permstr, uentp->userlevel) == 0)
		return 0;
	if (i < page || i >= page + BBS_PAGESIZE || i >= range) {
		i++;
		if (i >= page + BBS_PAGESIZE || i >= range)
			return QUIT;
		else
			return 0;
	}
	uleveltochar(&permstr, uentp->userlevel);
	switch (toggle1) {
	case 0:
		sprintf(msgstr, "%-.16s", Ctime(&uentp->lastlogin));
		break;
	case 1:
		sprintf(msgstr, "%-.16s", uentp->lasthost);
		break;
	case 2:
	default:
		sprintf(msgstr, "%-.11s%.4s",
			Ctime(&uentp->firstlogin), Ctime(&uentp->firstlogin) + 20);
		break;
	}
	user_data[i - page] = *uentp;
	override = myfriend(searchuser(uentp->userid));
	prints(" %5d%2s%s%-14s%s %-19s  %5d %5d %6s %-16s\n", i + 1,
		(override) ? "  " : "",
		(override) ? "[1;32m" : "", uentp->userid, (override) ? "[m" : "",
#if defined(ACTS_REALNAMES)
		HAS_PERM(PERM_SYSOP) ? uentp->realname : uentp->username,
#else
		uentp->username,
#endif
		uentp->numlogins,
		(toggle2 == 0) ? uentp->numposts : (toggle2 == 1) ? uentp->stay / 3600 : uentp->nummails,
		HAS_PERM(PERM_SEEULEVELS) ? permstr : "", msgstr);
	i++;
	usercounter++;
	return 0;
}

int
allusers()
{
	countusers(NULL);
	if (apply_record(PASSFILE, countusers, sizeof(struct userec)) == -1) {
		return 0;
	}
	return countusers(NULL);
}

int
mailto(uentp)
struct userec *uentp;
{
	char    filename[STRLEN];
	sprintf(filename, "tmp/mailall.%s", currentuser.userid);
	if ((uentp->userlevel == PERM_BASIC && mailmode == 1) ||
		(uentp->userlevel & PERM_POST && mailmode == 2) ||
		(uentp->userlevel & PERM_BOARDS && mailmode == 3) ||
		(uentp->userlevel & PERM_CHATCLOAK && mailmode == 4)) {
		mail_file(filename, uentp->userid, save_title);
	}
	return 1;
}
mailtoall(mode)
int     mode;
{

	mailmode = mode;
	if (apply_record(PASSFILE, mailto, sizeof(struct userec)) == -1) {
		prints("No Users Exist");
		pressreturn();
		return 0;
	}
	return;
}
Show_Users()
{

	usercounter = 0;
	modify_user_mode(LAUSERS);
	printuent((struct userec *) NULL);
	if (apply_record(PASSFILE, printuent, sizeof(struct userec)) == -1) {
		prints("No Users Exist");
		pressreturn();
		return 0;
	}
	clrtobot();
	return 0;
}
setlistrange(i)
int     i;
{
	range = i;
}


do_query(star, curr)
int     star, curr;
{
	if (user_record[curr]->userid != NULL) {
		clear();
		t_query(user_record[curr]->userid);
		move(t_lines - 1, 0);
		prints("[0;1;37;44m¡ƒÃÏ[[1;32mt[37m] ºƒ–≈[[1;32mm[37m] ÀÕ—∂œ¢[[1;32ms[37m] º”,ºı≈Û”—[[1;32mo[37m,[1;32md[37m] —°‘Ò π”√’ﬂ[[1;32m°¸[37m,[1;32m°˝[37m] «–ªªƒ£ Ω [[1;32mf[37m] «Ûæ»[[1;32mh[37m][m");
	}
}
do_query2(star, curr)
int     star, curr;
{
	if (user_data[curr - star].userid != NULL) {
		t_query(user_data[curr - star].userid);
		move(t_lines - 1, 0);
		prints("[0;1;37;44m          ºƒ–≈[[1;32mm[37m] º”,ºı≈Û”—[[1;32mo[37m,[1;32md[37m] ø¥Àµ√˜µµ[[1;32m°˙[37m,[1;32mRtn[37m] —°‘Ò[[1;32m°¸[37m,[1;32m°˝[37m] «Ûæ»[[1;32mh[37m]          [m");
	}
}
Users()
{
	range = allusers();
	modify_user_mode(LAUSERS);
	clear();
	user_data = (struct userec *) calloc(sizeof(struct userec), BBS_PAGESIZE);
	choose(NA, 0, print_title2, deal_key2, Show_Users, do_query2);
	clear();
	free(user_data);
	return;
}

int
t_friends()
{
	char    buf[STRLEN];
	modify_user_mode(FRIEND);
	friendmode = YEA;
	setuserfile(buf, "friends");
	if (!dashf(buf)) {
		move(1, 0);
		clrtobot();
		prints("ƒ„…–Œ¥¿˚”√ Info -> Override …Ë∂®∫√”—√˚µ•£¨À˘“‘...\n");
		range = 0;
	} else {
		num_alcounter();
		range = count_friends;
	}
	if (range == 0) {
		move(2, 0);
		clrtobot();
		prints("ƒø«∞Œﬁ∫√”—…œœﬂ\n");
		move(BBS_PAGESIZE + 3, 0);
		if (askyn(" «∑Ò◊™ªª≥… π”√’ﬂƒ£ Ω", YEA, NA) == YEA) {
			range = num_visible_users();
			page = -1;
			friendmode = NA;
			update_time = 0;
			choose(YEA, 0, print_title, deal_key, show_userlist, do_query);
			clear();
			return;
		}
	} else {
		update_time = 0;
		choose(YEA, 0, print_title, deal_key, show_userlist, do_query);
	}
	clear();
	friendmode = NA;
	return;
}

int
t_users()
{
	friendmode = NA;
	modify_user_mode(LUSERS);
	range = num_visible_users();
	if (range == 0) {
		move(3, 0);
		clrtobot();
		prints("ƒø«∞Œﬁ π”√’ﬂ…œœﬂ\n");
	}
	update_time = 0;
	choose(YEA, 0, print_title, deal_key, show_userlist, do_query);
	clear();
	return 0;
}

int
choose(update, defaultn, title_show, key_deal, list_show, read)
int     update;
int     defaultn;
int     (*title_show) ();
int     (*key_deal) ();
int     (*list_show) ();
int     (*read) ();
{
	int     ch, number, deal;
	readplan = NA;
	(*title_show) ();
	func_list_show = list_show;
	signal(SIGALRM, SIG_IGN);
	if (update == 1)
		update_data();
	page = -1;
	number = 0;
	num = defaultn;
	while (1) {
		if (num <= 0)
			num = 0;
		if (num >= range)
			num = range - 1;
		if (page < 0 || freshmode == 1) {
			freshmode = 0;
			page = (num / BBS_PAGESIZE) * BBS_PAGESIZE;
			move(3, 0);
			clrtobot();
			if ((*list_show) () == -1)
				return -1;
			update_endline();
		}
		if (num < page || num >= page + BBS_PAGESIZE) {
			page = (num / BBS_PAGESIZE) * BBS_PAGESIZE;
			if ((*list_show) () == -1)
				return -1;
			update_endline();
			continue;
		}
		if (readplan == YEA) {
			if ((*read) (page, num) == -1)
				return num;
		} else {
			move(3 + num - page, 0);
			prints(">", number);
		}
		ch = egetch();
		if (readplan == NA)
			move(3 + num - page, 0);
		prints(" ");
		if (ch == 'q' || ch == 'e' || ch == KEY_LEFT || ch == EOF) {
			if (readplan == YEA) {
				readplan = NA;
				move(1, 0);
				clrtobot();
				if ((*list_show) () == -1)
					return -1;
				(*title_show) ();
				continue;
			}
			break;
		}
		deal = (*key_deal) (ch, num, page);
		if (range == 0)
			break;
		if (deal == 1)
			continue;
		else if (deal == -1)
			break;
		switch (ch) {
		case 'P':
		case 'b':
		case Ctrl('B'):
		case KEY_PGUP:
			if (num == 0)
				num = range - 1;
			else
				num -= BBS_PAGESIZE;
			break;
		case ' ':
			if (readplan == YEA) {
				if (++num >= range)
					num = 0;
				break;
			}
		case 'N':
		case Ctrl('F'):
		case KEY_PGDN:
			if (num == range - 1)
				num = 0;
			else
				num += BBS_PAGESIZE;
			break;
		case 'p':
		case 'l':
		case KEY_UP:
			if (num-- <= 0)
				num = range - 1;
			break;
		case 'n':
		case 'j':
		case KEY_DOWN:
			if (++num >= range)
				num = 0;
			break;
		case '$':
		case KEY_END:
			num = range - 1;
			break;
		case KEY_HOME:
			num = 0;
			break;
		case '\n':
		case '\r':
			if (number > 0) {
				num = number - 1;
				break;
			}
			/* fall through */
		case KEY_RIGHT:
			{
				if (readplan == YEA) {
					if (++num >= range)
						num = 0;
				} else
					readplan = YEA;
				break;
			}
		default:
			;
		}
		if (ch >= '0' && ch <= '9') {
			number = number * 10 + (ch - '0');
			ch = '\0';
		} else {
			number = 0;
		}
	}
	signal(SIGALRM, SIG_IGN);
	return -1;
}
