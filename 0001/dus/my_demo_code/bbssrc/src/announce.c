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
$Id: announce.c,v 1.9 1999/07/31 12:30:45 edwardc Exp $
*/

#include "bbs.h"
#include "bbsgopher.h"

#define MAXITEMS        1024
#define PATHLEN         256
#define A_PAGESIZE      (t_lines - 5)

#define ADDITEM         0
#define ADDGROUP        1
#define ADDMAIL         2
#define ADDGOPHER       3

int     bmonly = 0;
char    Importname[STRLEN];
void    a_menu();

extern char BoardName[];
extern void a_prompt();		/* added by netty */

typedef struct {
	char    title[72];
	char    fname[80];
	char   *host;
	int     port;
}       ITEM;

int     a_fmode = 1;

typedef struct {
	ITEM   *item[MAXITEMS];
	char    mtitle[STRLEN];
	char   *path;
	int     num, page, now;
	int     level;
}       MENU;

void
a_showmenu(pm)
MENU   *pm;
{
	struct stat st;
	struct tm *pt;
	char    title[STRLEN * 2], kind[20];
	char    fname[STRLEN];
	char    ch;
	char    buf[STRLEN], genbuf[STRLEN * 2];
	time_t  mtime;
	int     n;
	clear();
	if (chkmail()) {
		prints("[5m");
		sprintf(genbuf, "[ÄúÓÐÐÅ¼þ]");
	} else
		strcpy(genbuf, pm->mtitle);
	sprintf(buf, "%*s", (80 - strlen(genbuf)) / 2, "");
	prints("[1;44m%s%s%s[m\n", buf, genbuf, buf);
	prints("           [1;32m F[37m ¼Ä»Ø×Ô¼ºµÄÐÅÏä  [32m¡ü¡ý[37m ÒÆ¶¯ [32m ¡ú <Enter> [37m¶ÁÈ¡ [32m ¡û,q[37m Àë¿ª[m\n");
	prints("[1;44;37m ±àºÅ  %-45s ×÷  Õß           %8s [m", "[Àà±ð] ±ê    Ìâ"
		,a_fmode == 2 ? "µµ°¸Ãû³Æ" : "±à¼­ÈÕÆÚ");
	prints("\n");
	if (pm->num == 0)
		prints("      << Ä¿Ç°Ã»ÓÐÎÄÕÂ >>\n");
	for (n = pm->page; n < pm->page + 19 && n < pm->num; n++) {
		strcpy(title, pm->item[n]->title);
		if (a_fmode) {
			sprintf(fname, "%s", pm->item[n]->fname);
			sprintf(genbuf, "%s/%s", pm->path, fname);
			if (a_fmode == 2) {
				ch = (dashf(genbuf) ? ' ' : (dashd(genbuf) ? '/' : ' '));
				fname[10] = '\0';
			} else {
				if (dashf(genbuf) || dashd(genbuf)) {
					stat(genbuf, &st);
					mtime = st.st_mtime;
				} else
					mtime = time(0);

				pt = localtime(&mtime);
				sprintf(fname, "[[1m%02d[m.[1m%02d[m.[1m%02d[m]", pt->tm_year,
					pt->tm_mon + 1, pt->tm_mday);
				ch = ' ';
			}
			if (pm->item[n]->host != NULL) {
				if (pm->item[n]->fname[0] == '0')
					strcpy(kind, "[[1;32mÁ¬ÎÄ[m]");
				else
					strcpy(kind, "[[1;33mÁ¬Ä¿[m]");
			} else if (dashf(genbuf)) {
				strcpy(kind, "[[1;36mÎÄ¼þ[m]");
			} else if (dashd(genbuf)) {
				strcpy(kind, "[[1;37mÄ¿Â¼[m]");
			} else {
				strcpy(kind, "[[1;32m´íÎó[m]");
			}
			if (!strncmp(title, "[Ä¿Â¼] ", 7) || !strncmp(title, "[ÎÄ¼þ] ", 7)
				|| !strncmp(title, "[Á¬Ä¿] ", 7) || !strncmp(title, "[Á¬ÎÄ] ", 7))
				sprintf(genbuf, "%-s %-55.55s%-s%c", kind, title + 7, fname, ch);
			else
				sprintf(genbuf, "%-s %-55.55s%-s%c", kind, title, fname, ch);
			strncpy(title, genbuf, STRLEN * 2);
			title[STRLEN * 2 - 1] = '\0';
		}
		prints("  %3d  %s\n", n + 1, title);
	}
	clrtobot();
	move(t_lines - 1, 0);
	prints("%s", (pm->level & PERM_BOARDS) ?
		"[1;31;44m[°å  Ö÷]  [33mËµÃ÷ h    Àë¿ª q,¡û    ÐÂÔöÎÄÕÂ a    ÐÂÔöÄ¿Â¼ g    ±à¼­µµ°¸ E        [m" :
		"[1;31;44m[¹¦ÄÜ¼ü] [33m ËµÃ÷ h    Àë¿ª q,¡û    ÒÆ¶¯ÓÎ±ê k,¡ü,j,¡ý    ¶ÁÈ¡×ÊÁÏ Rtn,¡ú         [m");
}

void
a_additem(pm, title, fname, host, port)
MENU   *pm;
char   *title, *fname, *host;
int     port;
{
	ITEM   *newitem;
	if (pm->num < MAXITEMS) {
		newitem = (ITEM *) malloc(sizeof(ITEM));
		strcpy(newitem->title, title);
		if (host != NULL) {
			newitem->host = (char *) malloc(sizeof(char) * (strlen(host) + 1));
			strcpy(newitem->host, host);
		} else
			newitem->host = host;
		newitem->port = port;
		strcpy(newitem->fname, fname);
		pm->item[(pm->num)++] = newitem;
	}
}

int
a_loadnames(pm)
MENU   *pm;
{
	FILE   *fn;
	ITEM    litem;
	char    buf[PATHLEN], *ptr;
	char    hostname[STRLEN];
	pm->num = 0;
	sprintf(buf, "%s/.Names", pm->path);
	if ((fn = fopen(buf, "r")) == NULL)
		return 0;
	hostname[0] = '\0';
	while (fgets(buf, sizeof(buf), fn) != NULL) {
		if ((ptr = strchr(buf, '\n')) != NULL)
			*ptr = '\0';
		if (strncmp(buf, "Name=", 5) == 0) {
			strncpy(litem.title, buf + 5, 72);
			litem.title[71] = '\0';
		} else if (strncmp(buf, "Path=", 5) == 0) {
			if (strncmp(buf, "Path=~/", 7) == 0)
				strncpy(litem.fname, buf + 7, 80);
			else
				strncpy(litem.fname, buf + 5, 80);
			litem.fname[79] = '\0';
			if ((!strstr(litem.title, "(BM: BMS)") || HAS_PERM(PERM_BOARDS)) &&
				(!strstr(litem.title, "(BM: SYSOPS)") || HAS_PERM(PERM_SYSOP))) {
				if (strstr(litem.fname, "!@#$%")) {
					char   *ptr1, *ptr2, gtmp[STRLEN];
					strcpy(gtmp, litem.fname);
					ptr1 = strtok(gtmp, "!#$%@");
					strcpy(hostname, ptr1);
					ptr2 = strtok(NULL, "@");
					strcpy(litem.fname, ptr2);
					litem.port = atoi(strtok(NULL, "@"));
				}
				a_additem(pm, litem.title, litem.fname, (strlen(hostname) == 0) ?
					NULL : hostname, litem.port);
			}
			hostname[0] = '\0';
		} else if (strncmp(buf, "# Title=", 8) == 0) {
			if (pm->mtitle[0] == '\0')
				strncpy(pm->mtitle, buf + 8, STRLEN);
		} else if (strncmp(buf, "Host=", 5) == 0) {
			strncpy(hostname, buf + 5, STRLEN);
		} else if (strncmp(buf, "Port=", 5) == 0) {
			litem.port = atoi(buf + 5);
		}
	}
	fclose(fn);
	return 1;
}

void
a_savenames(pm)
MENU   *pm;
{
	FILE   *fn;
	ITEM   *item;
	char    fpath[PATHLEN];
	int     n;
	sprintf(fpath, "%s/.Names", pm->path);
	if ((fn = fopen(fpath, "w")) == NULL)
		return;
	fprintf(fn, "#\n");
	if (!strncmp(pm->mtitle, "[Ä¿Â¼] ", 7) || !strncmp(pm->mtitle, "[ÎÄ¼þ] ", 7)
		|| !strncmp(pm->mtitle, "[Á¬Ä¿] ", 7) || !strncmp(pm->mtitle, "[Á¬ÎÄ] ", 7)) {
		fprintf(fn, "# Title=%s\n", pm->mtitle + 7);
	} else {
		fprintf(fn, "# Title=%s\n", pm->mtitle);
	}
	fprintf(fn, "#\n");
	for (n = 0; n < pm->num; n++) {
		item = pm->item[n];
		if (!strncmp(item->title, "[Ä¿Â¼] ", 7) || !strncmp(item->title, "[ÎÄ¼þ] ", 7)
			|| !strncmp(item->title, "[Á¬Ä¿] ", 7) || !strncmp(item->title, "[Á¬ÎÄ] ", 7)) {
			fprintf(fn, "Name=%s\n", item->title + 7);
		} else
			fprintf(fn, "Name=%s\n", item->title);
		if (item->host != NULL) {
			fprintf(fn, "Host=%s\n", item->host);
			fprintf(fn, "Port=%d\n", item->port);
			fprintf(fn, "Type=1\n");
			fprintf(fn, "Path=%s\n", item->fname);
		} else
			fprintf(fn, "Path=~/%s\n", item->fname);
		fprintf(fn, "Numb=%d\n", n + 1);
		fprintf(fn, "#\n");
	}
	fclose(fn);
	chmod(fpath, 0644);
}

void
a_prompt(bot, pmt, buf, len)
int     bot;
char   *pmt, *buf;
int     len;
{
	move(t_lines + bot, 0);
	clrtoeol();
	getdata(t_lines + bot, 0, pmt, buf, len, DOECHO, YEA);
}
/* added by netty to handle post saving into (0)Announce */
int
a_Save(path, key, fileinfo, nomsg)
char   *path, *key;
struct fileheader *fileinfo;
int     nomsg;
{

	char    board[40];
	int     ans = NA;
	if (!nomsg) {
		sprintf(genbuf, "È·¶¨½« [%-.40s] ´æÈëÔÝ´æµµÂð", fileinfo->title);
		if (askyn(genbuf, NA, YEA) == NA)
			return FULLUPDATE;
	}
	sprintf(board, "bm/%s", currentuser.userid);
	if (dashf(board)) {
		if (nomsg)
			ans = YEA;
		else
			ans = askyn("Òª¸½¼ÓÔÚ¾ÉÔÝ´æµµÖ®ááÂð", NA, YEA);
	}
	if (in_mail)
		sprintf(genbuf, "/bin/%s mail/%c/%s/%s %s %s", (ans) ? "cat" : "cp -r",
			toupper(currentuser.userid[0]), currentuser.userid, fileinfo->filename, (ans) ? ">>" : "", board);
	else
		sprintf(genbuf, "/bin/%s boards/%s/%s %s %s", (ans) ? "cat" : "cp -r",
			key, fileinfo->filename, (ans) ? ">>" : "", board);
	system(genbuf);
	if (!nomsg)
		a_prompt(-1, "ÒÑ½«¸ÃÎÄÕÂ´æÈëÔÝ´æµµ, Çë°´<Enter>¼ÌÐø...", genbuf, 2);
	return FULLUPDATE;
}
/* added by netty to handle post saving into (0)Announce */
int
a_Import(path, key, fileinfo, nomsg)
char   *path, *key;
struct fileheader *fileinfo;
int     nomsg;
{

	FILE   *fn;
	char    fname[STRLEN], *ip, bname[STRLEN];
	char    buf[PATHLEN], *ptr;
	int     len, ch;
	MENU    pm;
	char    ans[5];
	modify_user_mode(DIGEST);
	len = strlen(key);
	sprintf(buf, "%s/.Search", path);
	if ((fn = fopen(buf, "r")) != NULL) {
		while (fgets(buf, sizeof(buf), fn) != NULL) {
			if (strncmp(buf, key, len) == 0 && buf[len] == ':' &&
				(ptr = strtok(&buf[len + 1], " \t\n")) != NULL) {
				sprintf(Importname, "%s/%s", path, ptr);
				fclose(fn);
				if (netty_path[0] != '\0') {
					if (!nomsg) {
						sprintf(genbuf, "È·¶¨½«¸ÃÎÄÕÂ·Å½ø %s Âð", netty_path);
						ch = askyn(genbuf, NA, YEA);
					}
					if (ch == YEA || nomsg) {
						pm.path = netty_path;
					} else {
						a_prompt(-1, "Äã¿ÉÒÔµ½¾«»ªÇøÉè¶¨±ðµÄË¿Â·, °´<Enter>½áÊø...", ans, 2);
						return 1;
					}
				} else {
					if (!nomsg) {
						sprintf(genbuf, "È·¶¨½«¸ÃÎÄÕÂ·Å½ø %s Âð", Importname);
						ch = askyn(genbuf, NA, YEA);
					}
					if (ch == YEA || nomsg) {
						pm.path = Importname;
					} else {
						a_prompt(-1, "Äã¸Ä±äÐÄÒâÁË? Çë°´<Enter>½áÊø...", ans, 2);
						return 1;
					}
				}
				strcpy(pm.mtitle, "");
				a_loadnames(&pm);
				strcpy(fname, fileinfo->filename);
				sprintf(bname, "%s/%s", pm.path, fname);
				ip = &fname[strlen(fname) - 1];
				while (dashf(bname)) {
					if (*ip == 'Z')
						ip++, *ip = 'A', *(ip + 1) = '\0';
					else
						(*ip)++;
					sprintf(bname, "%s/%s", pm.path, fname);
				}
				/*
				 * edwardc.990501 °ÑÕûÀí¸Ä³É×÷Õß, ¼´
				 * 
				 */
				sprintf(genbuf, "%-38.38s %s ", fileinfo->title, fileinfo->owner);
				a_additem(&pm, genbuf, fname, NULL, 0);
				a_savenames(&pm);
				if (in_mail)
					sprintf(genbuf, "/bin/cp -r mail/%c/%s/%s %s",
						toupper(currentuser.userid[0]), currentuser.userid, fileinfo->filename, bname);
				else
					sprintf(genbuf, "/bin/cp -r boards/%s/%s %s", key, fileinfo->filename, bname);
				system(genbuf);
				if (!nomsg) {
					a_prompt(-1, "ÒÑ½«¸ÃÎÄÕÂ·Å½ø¾«»ªÇø, Çë°´<Enter>¼ÌÐø...", ans, 2);
				}
				for (ch = 0; ch < pm.num; ch++)
					free(pm.item[ch]);
				return 1;
			}
		}
	}
	return 0;
}

int
a_menusearch(path, key, level)
char   *path, *key;
int     level;
{
	FILE   *fn;
	char    bname[20];
	char    buf[PATHLEN], *ptr;
	char    found[PATHLEN];
	int     searchmode = 0;
	if (key == NULL) {
		key = bname;
		a_prompt(-1, "ÊäÈëÓûËÑÑ°Ö®ÌÖÂÛÇøÃû³Æ: ", key, 18);
		searchmode = 1;
	}
	found[0] = '\0';
	sprintf(buf, "0Announce/.Search");
	if (key[0] != '\0' && (fn = fopen(buf, "r")) != NULL) {
		while (fgets(buf, sizeof(buf), fn) != NULL) {
			if (searchmode && !strstr(buf, "groups/"))
				continue;
			ptr = strchr(buf, ':');
			if (!ptr)
				return 0;
			else {
				*ptr = '\0';
				ptr = strtok(ptr + 1, " \t\n");
			}
			if (!strcasecmp(buf, key)) {
				sprintf(found, "0Announce/%s", ptr);
				break;
			}
		}
		fclose(fn);
		if (found[0]) {
			a_menu("", found, level, 0);
			return 1;
		} else {
			a_prompt(-1, "ÕÒ²»µ½ÄúËùÊäÈëµÄÌÖÂÛÇø, °´<Enter>¼ÌÐø...", buf, 2);
			return 1;
		}
	}
	return 0;
}

void
a_forward(path, pitem, mode)
char   *path;
ITEM   *pitem;
int     mode;
{
	struct shortfile fhdr;
	char    fname[PATHLEN], *mesg;
	sprintf(fname, "%s/%s", path, pitem->fname);
	if (dashf(fname)) {
		strncpy(fhdr.title, pitem->title, STRLEN);
		strncpy(fhdr.filename, pitem->fname, STRLEN);
		fhdr.title[STRLEN - 1] = '\0';
		fhdr.filename[STRLEN - 1] = '\0';
		switch (doforward(path, &fhdr, mode)) {
		case 0:
			mesg = "ÎÄÕÂ×ª¼ÄÍê³É!\n";
			break;
		case -1:
			mesg = "System error!!.\n";
			break;
		case -2:
			mesg = "Invalid address.\n";
			break;
		default:
			mesg = "È¡Ïû×ª¼Ä¶¯×÷.\n";
		}
		prints(mesg);
	} else {
		move(t_lines - 1, 0);
		prints("ÎÞ·¨×ª¼Ä´ËÏîÄ¿.\n");
	}
	pressanykey();
}

void
a_download(fname)
char   *fname;
{
	char   *ptr;
	if (dashf(fname)) {
		ptr = fname;
		if ((ptr = strrchr(fname, '/')) != NULL)
			ptr++;
		sprintf(genbuf, "Ê¹ÓÃ Z-Modem ´«ËÍ %s µµ°¸Âð", ptr);
		if (askyn(genbuf, NA, YEA) == YEA) {
			sprintf(genbuf, "bin/sz -ve %s", fname);
			system(genbuf);
		}
	} else {
		prints("ÎÞ·¨´«ËÍ´ËÏîÄ¿.\n");
		egetch();
	}
}

void
a_newitem(pm, mode)
MENU   *pm;
int     mode;
{
	char    uident[STRLEN];
	char    board[STRLEN], title[STRLEN];
	char    fname[STRLEN], fpath[PATHLEN], fpath2[PATHLEN];
	char   *mesg;
	FILE   *pn;
	char    ans[10], head;
	srand(time(0));

	pm->page = 9999;
	head = 'X';
	switch (mode) {
	case ADDITEM:
		head = 'A';	/* article */
		break;
	case ADDGROUP:
		head = 'D';	/* directory */
		break;
	case ADDMAIL:
		sprintf(board, "bm/%s", currentuser.userid);
		if (!dashf(board)) {
			a_prompt(-1, "ÇëÏÈÖÁ¸ÃÌÖÂÛÇø½«ÎÄÕÂ´æÈëÔÝ´æµµ, °´<Enter>¼ÌÐø...", ans, 2);
			return;
		}
		mesg = "ÇëÊäÈëµµ°¸Ö®Ó¢ÎÄÃû³Æ(¿Éº¬Êý×Ö)£º";
		break;
	case ADDGOPHER:
		{
			int     gport;
			char    ghost[STRLEN], gtitle[STRLEN], gfname[STRLEN];
			a_prompt(-2, "Á¬ÏßµÄÎ»Ö·£º", ghost, STRLEN - 14);
			if (ghost[0] == '\0')
				return;

			/* edwardc.990427 fix up .. @_@ terrible code */

			a_prompt(-2, "Á¬ÏßµÄÄ¿Â¼£º(¿É°´ Enter Ô¤Éè£©", gfname, STRLEN - 14);
			/* ÈÃÁ¬ÏßÄ¿Â¼Ô¤ÉèÎª¿Õ×Ö´®£¬´ó¶à¿ÉÁ¬½Ó */
			if (gfname[0] == '\0')
				gfname[0] = '\0';

			a_prompt(-2, "Á¬ÏßµÄPort£º(Ô¤ÉèÎª 70£©", ans, 6);
			if (ans[0] == '\0')
				strcpy(ans, "70");

			gport = atoi(ans);

			a_prompt(-2, "±êÌâ£º£¨Ô¤ÉèÎªÁ¬ÏßÎ»Ö·)", gtitle, 70);
			if (gtitle[0] == '\0')
				strncpy(gtitle, ghost, STRLEN);

			a_additem(pm, gtitle, gfname, ghost, gport);
			a_savenames(pm);
			return;
		}
	}
	/* edwardc.990320 system will assign a filename for you .. */
	sprintf(fname, "%c%X", head, time(0) + getpid() + getppid() + rand());
	sprintf(fpath, "%s/%s", pm->path, fname);
	if (!valid_fname(fname)) {
		a_prompt(-1, "Ãû³ÆÖ»ÄÜ°üº¬Ó¢ÎÄ¼°Êý×Ö!!", ans, 2);
	} else if (dashf(fpath) || dashd(fpath)) {
		sprintf(genbuf, "ÏµÍ³ÄÚÒÑÓÐ %s Õâ¸öµµ°¸´æÔÚÁË...", fname);
		a_prompt(-1, genbuf, ans, 2);
	} else {
		mesg = "ÇëÊäÈëÎÄ¼þ»òÄ¿Â¼Ö®ÖÐÎÄÃû³Æ£º ";
		a_prompt(-1, mesg, title, 35);
		if (*title == '\0')
			return;
		switch (mode) {
		case ADDITEM:
			vedit(fpath, 0);
			chmod(fpath, 0644);
			break;
		case ADDGROUP:
			mkdir(fpath, 0755);
			chmod(fpath, 0755);
			break;
		case ADDMAIL:
			rename(board, fpath);
			break;
		}
		if (mode != ADDGROUP)
			sprintf(genbuf, "%-38.38s %s ", title, currentuser.userid);
		else {
/*Add by SmallPig*/
			if (HAS_PERM(PERM_SYSOP) || HAS_PERM(PERM_ANNOUNCE)) {
				move(1, 0);
				clrtoeol();
				getdata(1, 0, "°åÖ÷: ", uident, 35, DOECHO, YEA);
				if (uident[0] != '\0')
					sprintf(genbuf, "%-38.38s(BM: %s)", title, uident);
				else
					sprintf(genbuf, "%-38.38s", title);
			} else
				sprintf(genbuf, "%-38.38s", title);
		}
		a_additem(pm, genbuf, fname, NULL, 0);
		a_savenames(pm);
		if (mode == ADDGROUP) {
			sprintf(fpath2, "%s/%s/.Names", pm->path, fname);
			if ((pn = fopen(fpath2, "w")) != NULL) {
				fprintf(pn, "#\n");
				fprintf(pn, "# Title=%s\n", genbuf);
				fprintf(pn, "#\n");
				fclose(pn);
			}
		}
	}
}

void
a_moveitem(pm)
MENU   *pm;
{
	ITEM   *tmp;
	char    newnum[STRLEN];
	int     num, n;
	sprintf(genbuf, "ÇëÊäÈëµÚ %d ÏîµÄÐÂ´ÎÐò: ", pm->now + 1);
	a_prompt(-2, genbuf, newnum, 6);
	num = (newnum[0] == '$') ? 9999 : atoi(newnum) - 1;
	if (num >= pm->num)
		num = pm->num - 1;
	else if (num < 0)
		return;
	tmp = pm->item[pm->now];
	if (num > pm->now) {
		for (n = pm->now; n < num; n++)
			pm->item[n] = pm->item[n + 1];
	} else {
		for (n = pm->now; n > num; n--)
			pm->item[n] = pm->item[n - 1];
	}
	pm->item[num] = tmp;
	pm->now = num;
	a_savenames(pm);
}

void
a_copypaste(pm, paste)
MENU   *pm;
int     paste;
{
	static char title[STRLEN], filename[STRLEN], fpath[PATHLEN];
	ITEM   *item;
	char    newpath[PATHLEN];
	move(t_lines - 1, 0);
	if (!paste) {
		item = pm->item[pm->now];
		strcpy(title, item->title);
		strcpy(filename, item->fname);
		sprintf(genbuf, "%s/%s", pm->path, filename);
		strncpy(fpath, genbuf, PATHLEN);
		fpath[PATHLEN - 1] = '\0';
		prints("µµ°¸±êÊ¶Íê³É. (×¢Òâ! Õ³ÌùÎÄÕÂáá²ÅÄÜ½«ÎÄÕÂ delete!)");
		egetch();
	} else {
		sprintf(newpath, "%s/%s", pm->path, filename);
		if (*title == '\0' || *filename == '\0') {
			prints("ÇëÏÈÊ¹ÓÃ copy ÃüÁîÔÙÊ¹ÓÃ paste ÃüÁî. ");
			egetch();
		} else if (dashf(newpath) || dashd(newpath)) {
			prints("%s µµ°¸/Ä¿Â¼ÒÑ¾­´æÔÚ. ", filename);
			egetch();
		} else if (strstr(newpath, fpath) != NULL) {
			prints("ÎÞ·¨½«Ä¿Â¼°á½ø×Ô¼ºµÄ×ÓÄ¿Â¼ÖÐ, »áÔì³ÉËÀ»ØÈ¦.");
			egetch();
		} else {
			sprintf(genbuf, "ÄúÈ·¶¨ÒªÕ³Ìù %s µµ°¸/Ä¿Â¼Âð", filename);
			if (askyn(genbuf, NA, YEA) == YEA) {
				sprintf(genbuf, "/bin/cp -r %s %s", fpath, newpath);
				system(genbuf);
				a_additem(pm, title, filename, NULL, 0);
				a_savenames(pm);
			}
		}
	}
	pm->page = 9999;
}

void
a_delete(pm)
MENU   *pm;
{
	ITEM   *item;
	char    fpath[PATHLEN];
	int     n;
	item = pm->item[pm->now];
	move(t_lines - 2, 0);
	prints("%5d  %-50s\n", pm->now + 1, item->title);
	if (item->host == NULL) {
		sprintf(fpath, "%s/%s", pm->path, item->fname);
		if (dashf(fpath)) {
			if (askyn("É¾³ý´Ëµµ°¸, È·¶¨Âð", NA, YEA) == NA)
				return;
			unlink(fpath);
		} else if (dashd(fpath)) {
			if (askyn("É¾³ýÕû¸ö×ÓÄ¿Â¼, ±ð¿ªÍæÐ¦Å¶, È·¶¨Âð", NA, YEA) == NA)
				return;
			deltree(fpath);
		}
	} else {
		if (askyn("É¾³ýÁ¬ÏßÑ¡Ïî, È·¶¨Âð", NA, YEA) == NA)
			return;
	}
	free(item);
	(pm->num)--;
	for (n = pm->now; n < pm->num; n++)
		pm->item[n] = pm->item[n + 1];
	a_savenames(pm);
}

void
a_newname(pm)
MENU   *pm;
{
	ITEM   *item;
	char    fname[STRLEN];
	char    fpath[PATHLEN];
	char   *mesg;
	item = pm->item[pm->now];
	a_prompt(-2, "ÐÂµµÃû: ", fname, 12);
	if (*fname == '\0')
		return;
	sprintf(fpath, "%s/%s", pm->path, fname);
	if (!valid_fname(fname)) {
		mesg = "²»ºÏ·¨µµ°¸Ãû³Æ.";
	} else if (dashf(fpath) || dashd(fpath)) {
		mesg = "ÏµÍ³ÖÐÒÑÓÐ´Ëµµ°¸´æÔÚÁË.";
	} else {
		sprintf(genbuf, "%s/%s", pm->path, item->fname);
		if (rename(genbuf, fpath) == 0) {
			strcpy(item->fname, fname);
			a_savenames(pm);
			return;
		}
		mesg = "µµÃû¸ü¸ÄÊ§°Ü!!";
	}
	prints(mesg);
	egetch();
}

void
a_manager(pm, ch)
MENU   *pm;
int     ch;
{
	char    uident[STRLEN];
	ITEM   *item;
	char    fpath[PATHLEN], changed_T[STRLEN], ans[5];
	if (pm->num > 0) {
		item = pm->item[pm->now];
		sprintf(fpath, "%s/%s", pm->path, item->fname);
	}
	switch (ch) {
	case 'a':
		a_newitem(pm, ADDITEM);
		break;
	case 'g':
		a_newitem(pm, ADDGROUP);
		break;
	case 'i':
		a_newitem(pm, ADDMAIL);
		break;
	case 'G':
		if (HAS_PERM(PERM_SYSOP))
			a_newitem(pm, ADDGOPHER);
		break;
	case 'p':
		a_copypaste(pm, 1);
		break;
	case 'f':
		pm->page = 9999;
		sprintf(genbuf, "Â·¾¶Îª %s, ÒªÉèÎªË¿Â·Âð", pm->path);
		if (askyn(genbuf, NA, YEA) == YEA) {
			strcpy(netty_path, pm->path);
			a_prompt(-1, "ÒÑ½«¸ÃÂ·¾¶ÉèÎªË¿Â·, Çë°´<Enter>¼ÌÐø...", ans, 2);
		}
		break;
	}
	if (pm->num > 0)
		switch (ch) {
		case 's':
			if (++a_fmode >= 3)
				a_fmode = 1;
			pm->page = 9999;
			break;
		case 'M':
			a_moveitem(pm);
			pm->page = 9999;
			break;
		case 'D':
			a_delete(pm);
			pm->page = 9999;
			break;
		case 'V':
		case 'v':
			if (HAS_PERM(PERM_SYSOP)) {
				if (ch == 'v')
					sprintf(fpath, "%s/.Names", pm->path);
				else
					sprintf(fpath, "0Announce/.Search");

				if (dashf(fpath)) {
					modify_user_mode(EDITANN);
					vedit(fpath, 0);
					modify_user_mode(DIGEST);
				}
				pm->page = 9999;
			}
			break;
		case 'T':
			a_prompt(-2, "ÐÂ±êÌâ: ", changed_T, 35);
			/*
			 * modified by netty to properly handle title
			 * change,add bm by SmallPig
			 */
			if (*changed_T) {
				if (dashf(fpath)) {
					sprintf(genbuf, "%-38.38s %s ", changed_T, currentuser.userid);
					strncpy(item->title, genbuf, 72);
					item->title[71] = '\0';
				} else if (dashd(fpath)) {
					if (HAS_PERM(PERM_SYSOP) || HAS_PERM(PERM_ANNOUNCE)) {
						move(1, 0);
						clrtoeol();
						/*
						 * usercomplete("°åÖ÷:
						 * ",uident) ;
						 */
						getdata(1, 0, "°åÖ÷: ", uident, 35, DOECHO, YEA);
						if (uident[0] != '\0')
							sprintf(genbuf, "%-38.38s(BM: %s)", changed_T, uident);
						else
							sprintf(genbuf, "%-38.38s", changed_T);
					} else
						sprintf(genbuf, "%-38.38s", changed_T);

					strncpy(item->title, genbuf, 72);
				} else if (pm->item[pm->now]->host != NULL)
					strncpy(item->title, changed_T, 72);
				item->title[71] = '\0';
				a_savenames(pm);
			}
			pm->page = 9999;
			break;
		case 'E':
			if (dashf(fpath)) {
				modify_user_mode(EDITANN);
				vedit(fpath, 0);
				modify_user_mode(DIGEST);
			}
			pm->page = 9999;
			break;
		case 'n':
			a_newname(pm);
			pm->page = 9999;
			break;
		case 'c':
			a_copypaste(pm, 0);
			break;

		}
}

void
a_menu(maintitle, path, lastlevel, lastbmonly)
char   *maintitle, *path;
int     lastlevel, lastbmonly;
{
	MENU    me;
	char    fname[PATHLEN], tmp[STRLEN];
	int     ch;
	char   *bmstr;
	char    buf[STRLEN];
	int     bmonly;
	int     number = 0;
	modify_user_mode(DIGEST);
	me.path = path;
	strcpy(me.mtitle, maintitle);
	me.level = lastlevel;
	bmonly = lastbmonly;
	a_loadnames(&me);

	strcpy(buf, me.mtitle);
	bmstr = strstr(buf, "(BM:");
	if (bmstr != NULL) {
		if (chk_currBM(bmstr + 4))
			me.level |= PERM_BOARDS;
		else if (bmonly == 1 && !(me.level & PERM_BOARDS))
			return;
	}
	if (strstr(me.mtitle, "(BM: BMS)") || strstr(me.mtitle, "(BM: SECRET)") ||
		strstr(me.mtitle, "(BM: SYSOPS)"))
		bmonly = 1;

	strcpy(buf, me.mtitle);
	bmstr = strstr(buf, "(BM:");

	me.page = 9999;
	me.now = 0;
	while (1) {
		if (me.now >= me.num && me.num > 0) {
			me.now = me.num - 1;
		} else if (me.now < 0) {
			me.now = 0;
		}
		if (me.now < me.page || me.now >= me.page + A_PAGESIZE) {
			me.page = me.now - (me.now % A_PAGESIZE);
			a_showmenu(&me);
		}
		move(3 + me.now - me.page, 0);
		prints("->");
		ch = egetch();
		move(3 + me.now - me.page, 0);
		prints("  ");
		if (ch == 'Q' || ch == 'q' || ch == KEY_LEFT || ch == EOF)
			break;
		switch (ch) {
		case KEY_UP:
		case 'K':
		case 'k':
			if (--me.now < 0)
				me.now = me.num - 1;
			break;
		case KEY_DOWN:
		case 'J':
		case 'j':
			if (++me.now >= me.num)
				me.now = 0;
			break;
		case KEY_PGUP:
		case Ctrl('B'):
			if (me.now >= A_PAGESIZE)
				me.now -= A_PAGESIZE;
			else if (me.now > 0)
				me.now = 0;
			else
				me.now = me.num - 1;
			break;
		case KEY_PGDN:
		case Ctrl('F'):
		case ' ':
			if (me.now < me.num - A_PAGESIZE)
				me.now += A_PAGESIZE;
			else if (me.now < me.num - 1)
				me.now = me.num - 1;
			else
				me.now = 0;
			break;
		case Ctrl('C'):
			if (!HAS_PERM(PERM_POST))
				break;
/* add by Ghostrider for bugs repair */
			sprintf(fname, "%s/%s", path, me.item[me.now]->fname);
/* end of adding */
			if (!dashf(fname))
				break;
			if (me.now < me.num) {
				char    bname[30];
				clear();
				if (get_a_boardname(bname, "ÇëÊäÈëÒª×ªÌùµÄÌÖÂÛÇøÃû³Æ: ")) {
					move(1, 0);
					sprintf(tmp, "ÄãÈ·¶¨Òª×ªÌùµ½ %s °åÂð", bname);
					if (askyn(tmp, NA, NA) == 1) {
						postfile(fname, bname, me.item[me.now]->title, 2);
						move(3, 0);
						sprintf(tmp, "[1mÒÑ¾­°ïÄã×ªÌùÖÁ %s °åÁË[m", bname);
						prints(tmp);
						refresh();
						sleep(1);
					}
				}
				me.page = 9999;
			}
			show_message(NULL);
			break;
		case 'h':
			show_help("help/announcereadhelp");
			me.page = 9999;
			break;
		case '\n':
		case '\r':
			if (number > 0) {
				me.now = number - 1;
				number = 0;
				continue;
			}
		case 'R':
		case 'r':
		case KEY_RIGHT:
			if (me.now < me.num) {
				if (me.item[me.now]->host != NULL) {
					if (me.item[me.now]->fname[0] == '0') {
						if (get_con(me.item[me.now]->host, me.item[me.now]->port) != -1) {
							char    tmpfile[30];

							GOPHER  tmp;
							extern GOPHER *tmpitem;
							tmpitem = &tmp;
							strcpy(tmp.server, me.item[me.now]->host);
							strcpy(tmp.file, me.item[me.now]->fname);
							sprintf(tmp.title, "0%s", me.item[me.now]->title);
							tmp.port = me.item[me.now]->port;
							enterdir(me.item[me.now]->fname);
							setuserfile(tmpfile, "gopher.tmp");
							savetmpfile(tmpfile);
							ansimore(tmpfile, YEA);
							unlink(tmpfile);
						}
					} else {
						gopher(me.item[me.now]->host, me.item[me.now]->fname,
							me.item[me.now]->port, me.item[me.now]->title);
					}
					me.page = 9999;
					break;
				} else
					sprintf(fname, "%s/%s", path, me.item[me.now]->fname);
				if (dashf(fname)) {
					ansimore(fname, YEA);
				} else if (dashd(fname)) {
					a_menu(me.item[me.now]->title, fname, me.level, bmonly);
				}
				me.page = 9999;
			}
			break;
		case '/':
			if (a_menusearch(path, NULL, me.level))
				me.page = 9999;
			break;
		case 'F':
		case 'U':
			if (me.now < me.num && HAS_PERM(PERM_BASIC)) {
				a_forward(path, me.item[me.now], ch == 'U');
				me.page = 9999;
			}
			break;
		case 'Z':
			if (me.now < me.num && HAS_PERM(PERM_BASIC)) {
				sprintf(fname, "%s/%s", path, me.item[me.now]->fname);
				a_download(fname);
				me.page = 9999;
			}
			break;
		case '!':
			if (!Q_Goodbye())
				break;	/* youzi leave */
		}
		if (ch >= '0' && ch <= '9') {
			number = number * 10 + (ch - '0');
			ch = '\0';
		} else {
			number = 0;
		}
		if (me.level & PERM_BOARDS)
			a_manager(&me, ch);
	}
	for (ch = 0; ch < me.num; ch++)
		free(me.item[ch]);
}

int
linkto(path, fname, title)
char   *path, *title, *fname;
{
	MENU    pm;
	pm.path = path;

	strcpy(pm.mtitle, title);
	a_loadnames(&pm);
	a_additem(&pm, title, fname, NULL, 0);
	a_savenames(&pm);
}

int
add_grp(group, gname, bname, title)
char    group[STRLEN], bname[STRLEN], title[STRLEN], gname[STRLEN];
{
	FILE   *fn;
	char    buf[PATHLEN];
	char    searchname[STRLEN];
	char    gpath[STRLEN * 2];
	char    bpath[STRLEN * 2];
	sprintf(buf, "0Announce/.Search");
	sprintf(searchname, "%s: groups/%s/%s", bname, group, bname);
	sprintf(gpath, "0Announce/groups/%s", group);
	sprintf(bpath, "%s/%s", gpath, bname);
	if (!dashd("0Announce")) {
		mkdir("0Announce", 0755);
		chmod("0Announce", 0755);
		if ((fn = fopen("0Announce/.Names", "w")) == NULL)
			return;
		fprintf(fn, "#\n");
		fprintf(fn, "# Title=%s ¾«»ªÇø¹«²¼À¸\n", BoardName);
		fprintf(fn, "#\n");
		fclose(fn);
	}
	if (!seek_in_file(buf, bname))
		addtofile(buf, searchname);
	if (!dashd("0Announce/groups")) {
		mkdir("0Announce/groups", 0755);
		chmod("0Announce/groups", 0755);

		linkto("0Announce", "groups", "ÌÖÂÛÇø¾«»ª");
	}
	if (!dashd(gpath)) {
		mkdir(gpath, 0755);
		chmod(gpath, 0755);
		linkto("0Announce/groups", group, gname);
	}
	if (!dashd(bpath)) {
		mkdir(bpath, 0755);
		chmod(bpath, 0755);
		linkto(gpath, bname, title);
		sprintf(buf, "%s/.Names", bpath);
		if ((fn = fopen(buf, "w")) == NULL) {
			return -1;
		}
		fprintf(fn, "#\n");
		fprintf(fn, "# Title=%s\n", title);
		fprintf(fn, "#\n");
		fclose(fn);
	}
	return 1;

}

int
del_grp(grp, bname, title)
char    grp[STRLEN], bname[STRLEN], title[STRLEN];
{
	char    buf[STRLEN], buf2[STRLEN], buf3[30];
	char    gpath[STRLEN * 2];
	char    bpath[STRLEN * 2];
	char    check[30];
	int     i, n;
	MENU    pm;
	strncpy(buf3, grp, 29);
	buf3[29] = '\0';
	sprintf(buf, "0Announce/.Search");
	sprintf(gpath, "0Announce/groups/%s", buf3);
	sprintf(bpath, "%s/%s", gpath, bname);
	deltree(bpath);

	pm.path = gpath;
	a_loadnames(&pm);
	for (i = 0; i < pm.num; i++) {
		strcpy(buf2, pm.item[i]->title);
		strcpy(check, strtok(pm.item[i]->fname, "/~\n\b"));
		if (strstr(buf2, title) && !strcmp(check, bname)) {
			free(pm.item[i]);
			(pm.num)--;
			for (n = i; n < pm.num; n++)
				pm.item[n] = pm.item[n + 1];
			a_savenames(&pm);
			break;
		}
	}
}

int
edit_grp(bname, grp, title, newtitle)
char    bname[STRLEN], grp[STRLEN], title[STRLEN], newtitle[100];
{
	char    buf[STRLEN], buf2[STRLEN], buf3[30];
	char    gpath[STRLEN * 2];
	char    bpath[STRLEN * 2];
	int     i;
	MENU    pm;
	strncpy(buf3, grp, 29);
	buf3[29] = '\0';
	sprintf(buf, "0Announce/.Search");
	sprintf(gpath, "0Announce/groups/%s", buf3);
	sprintf(bpath, "%s/%s", gpath, bname);
	if (!seek_in_file(buf, bname))
		return 0;

	pm.path = gpath;
	a_loadnames(&pm);
	for (i = 0; i < pm.num; i++) {
		strncpy(buf2, pm.item[i]->title, STRLEN);
		buf2[STRLEN - 1] = '\0';
		if (strstr(buf2, title) && strstr(pm.item[i]->fname, bname)) {
			strcpy(pm.item[i]->title, newtitle);
			break;
		}
	}
	a_savenames(&pm);
	pm.path = bpath;
	a_loadnames(&pm);
	strcpy(pm.mtitle, newtitle);
	a_savenames(&pm);
}

void
Announce()
{
	sprintf(genbuf, "%s ¾«»ªÇø¹«²¼À¸", BoardName);
	a_menu(genbuf, "0Announce", (HAS_PERM(PERM_ANNOUNCE) || HAS_PERM(PERM_SYSOP)) ? PERM_BOARDS : 0, 0);
	clear();
}
