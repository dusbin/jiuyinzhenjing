/*************************************************************
* @file Common.c
* 创建日期：2017/09/25
* 创建作者：shiqing
* 修改履历：001)[修改原因1]
* 修改作者：001)修改者 YYYY/MM/DD
* All Right Reserved, Copyright(c) Beijing Uxsino Limited 2017.
**************************************************************/
#include "Common.h"
#include <fcntl.h>
#include "CommonOperator.h"
#include <sys/stat.h>

#ifndef WIN32

#else
	#include "win32.h"
#endif

/*
 * 功能：为正在处理的文件加锁
 */
void lockFile(int fd)
{
	if(0 > fd)
	{
		DEBUG_LOG("%s\n", "fd is null");
		return;
	}

#ifndef WIN32
	static struct flock lock;

	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int ret = fcntl(fd, F_SETLKW, &lock);
	if(0 < ret)
	{
		ERR_LOG("%s\n", "lock file err");
	}
#else  /* WIN32 defined */
	HANDLE handle = (HANDLE)_get_osfhandle(fd);
	OVERLAPPED overlap = {0};
	BOOL ok;

	if (INVALID_HANDLE_VALUE == handle) {
			fprintf(stderr, "!!! get os handle for file descriptor %d failed\n", fd);

			return;
	}

	overlap.Offset     = 0;
	overlap.OffsetHigh = 0;
	ok = LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, 0xFFFFFFFFu, 0xFFFFFFFFu, &overlap);
	if (FALSE == ok)
	{
		fprintf(stderr, "!!! LockFileEx file descriptor %d failed: %d\n", fd, GetLastError());
		return;
	}
#endif /* WIN32 */
	TRACE_LOG("%s\n", "==");
}

/*
 * 功能：为正在处理的文件解锁
 */
void unLockFile(int fd)
{
	if(0 > fd)
	{
		ERR_LOG("%s\n", "fd is null");
		return ;
	}

#ifndef WIN32
	static struct flock lock;

	lock.l_type = F_UNLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int ret = fcntl(fd, F_SETLKW, &lock);
	if(0 < ret)
	{
		ERR_LOG("%s\n", "lock file err");
	}
#else   /* defined(WIN32) */
	HANDLE handle = (HANDLE)_get_osfhandle(fd);
	OVERLAPPED overlap = {0};
	BOOL ok;

	memset((void *)&overlap, 0, sizeof(overlap));
	overlap.Offset     = 0;
	overlap.OffsetHigh = 0;

	ok = UnlockFileEx(handle, 0, 0xFFFFFFFFu, 0xFFFFFFFFu, &overlap);
	if (FALSE == ok)
	{
		fprintf(stderr, "!!! UnlockFileEx file handle %p failed: %d\n", handle, GetLastError());
		return;
	}
#endif  /* WIN32 */
	TRACE_LOG("%s\n", "==");
}

/*
 * 功能：检测文件是否存在
 */
int is_File_Exist(char *url)
{
	struct stat st;
	if (lstat(url, &st) < 0)
	{
		return FUNC_ERR;
    }
	if (-1 == S_ISREG(st.st_mode))
	{
		return FUNC_ERR;
	}
	else
	{
		return FUNC_OK;
	}
}

/*
 * 功能：根据传入的字符串来以回车进行分割，最终获取每行信息
 */
ssize_t	buffer_getline(char **buffer, char **line)
{
    ssize_t line_size;
    char *p, *e;

    for(p=e=*buffer;*e && *e!=EOF && *e!='\n';e++)
        ;
    if(*e == EOF)
        return -1;
    *e = '\0';
    line_size = e-p;
    *buffer += line_size+1;
    *line = p;
    return line_size;
}
