#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#define MAX 100
struct mould
{
	int id;
	char path[MAX];
	void *moudelhand;
};
struct master
{
	struct mould mou[MAX];
	int num;		
};
struct master g_uxcfg;
typedef int (*CAC_FUNC)(int, int);
CAC_FUNC cac_func = NULL;
int jp_moudel_load();
int jp_moudel_unload();
