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
$Id: bm.c,v 1.2 1999/07/31 12:30:46 edwardc Exp $
*/

#include "bbs.h"
extern  cmpbnames();

int
addtodeny(uident)
char   *uident;
{
	char    buf[50], strtosave[STRLEN];
	setbfile(genbuf, currboard, "deny_users");
	if (seek_in_file(genbuf, uident))
		return -1;
	getdata(2, 0, "输入说明: ", buf, 40, DOECHO, YEA);
	sprintf(strtosave, "%-12s %s", uident, buf);
	return addtofile(genbuf, strtosave);
}

int
deldeny(uident)
char   *uident;
{
	char    fn[STRLEN];
	setbfile(fn, currboard, "deny_users");
	return del_from_file(fn, uident);
}

int
deny_user()
{
	char    uident[STRLEN];
	char    ans[8], repbuf[STRLEN];
	int     count;
	if (!chk_currBM(currBM)) {
		return DONOTHING;
	}
	if (check_anonyboard(currboard) == YEA) {
		clear();
		prints("\n匿名版版主不知道发文者身份, 故不应知道该限制谁发表文章\n");
		prints("因此设定无法 Post 的名单之权限不开放给版主使用\n");
		prints("若有需要设定到此名单, 请向通知sysop处理\n");
		pressreturn();
		if (!HAS_PERM(PERM_SYSOP)) {
			clear();
			return FULLUPDATE;
		}
	}
	while (1) {
		clear();
		prints("设定无法 Post 的名单\n");
		setbfile(genbuf, currboard, "deny_users");
		count = listfilecontent(genbuf);
		if (count)
			getdata(1, 0, "(A)增加 (D)删除 or (E)离开 [E]: ", ans, 7, DOECHO, YEA);
		else
			getdata(1, 0, "(A)增加 or (E)离开 [E]: ", ans, 7, DOECHO, YEA);
		if (*ans == 'A' || *ans == 'a') {
			move(1, 0);
			usercomplete("增加无法 POST 的使用者: ", uident);
			if (*uident != '\0') {
				if (addtodeny(uident) == 1) {
					sprintf(repbuf, "取消 %s 在 %s 的 POST 权力",
						uident, currboard);
					securityreport(repbuf);
				}
			}
		} else if ((*ans == 'D' || *ans == 'd') && count) {
			move(1, 0);
			namecomplete("删除无法 POST 的使用者: ", uident);
			move(1, 0);
			clrtoeol();
			if (uident[0] != '\0') {
				if (deldeny(uident)) {
					sprintf(repbuf, "恢复 %s 在 %s 的 POST 权力",
						uident, currboard);
					securityreport(repbuf);
				}
			}
		} else
			break;
	}
	clear();
	return FULLUPDATE;
}
