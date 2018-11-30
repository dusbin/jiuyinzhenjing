/**************************************************************
* 文件名称：Common.h
* 创建日期：2017/09/25
* 创建作者：shiqing
* 修改履历：001)[修改原因1]
* 修改作者：001)修改者 YYYY/MM/DD
* All Right Reserved, Copyright(c) Beijing Uxsino Limited 2017.
* **************************************************************/
#ifndef __COMMON_H__
#define	__COMMON_H__


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/********************************************************
 *
 *  UxdbLicense global config macro
 *
 ********************************************************
 */

/*
 * License 版本，发布版本时需要修改
 */
#define UXDB_LICENSE_VERSION_STR	"v2.0.0.6"

/*
 * License 配置文件和路径
 */
#define UXDB_LICENSE_CONFIG_DIR "configure"
#define UXDB_LICENSE_CONFIG_FILE_NAME "host.conf"

/*
 * License 默认安装路径。
 * 如果配置文件中的安装路径为空，则用默认路径。
 * 路径最后不用带/
 */
#ifdef _GUN_LINUX_
#define UXDB_LICENSE_DEFAULT_INSTALL_PATH "/home/uxdb/uxdbinstall/encryptLicense"
#else
#define UXDB_LICENSE_DEFAULT_INSTALL_PATH "."
#endif // _GUN_LINUX_

#define FUNC_OK 0           /*函数返回值成功*/
#define FUNC_ERR -1         /*函数返回值失败*/
#define FUNC_ALARM 1        /*函数返回值告警*/
#define FUNC_DIR_NOT_EXIST 2   /*函数返回值目录不存在*/
#define FUNC_FILE_NOT_EXIST 3  /*函数返回值文件不存在*/

#define FALSE   0
#define TRUE    1
#define ALARM   2
#define DIR_NOT_EXIST 3
#define FILE_NOT_EXIST 4

enum service_ret_code
{
	OK = 0,				/*操作成功*/
	ERR = 1,			/*操作失败 */
	BUSY = 2,			/*忙 */
	CONFIICT = 3,		/*操作冲突 */
	DFSNOTSTART = 4,	/*DFS末启动 */
	OTHER_ERR = 5,		/*其他错误 */
	ALREADY_EXISTS = 9,	/*已经存在*/
};

#define MAX_BUFF_SIZE 1024          /*数据传输的最大长度*/
#define FILEPATHLEN  256            /*文件路径最大长度*/
#define FILENAMELEN  100            /*文件名最大长度*/

#define INTLEN  10                  /*int最大长度*/
#define LONGLEN64BIT  20            /*long最大长度*/

/**
  * @brief  int
  */
typedef int BOOL;

/**
  * @brief  char
  */
typedef char s8;

/**
  * @brief  unsigned char
  */
typedef unsigned char u8;

/**
  * @brief  short
  */
typedef short s16;

/**
  * @brief  unsigned short
  */
typedef unsigned short u16;

/**
  * @brief  int
  */
typedef int s32;

/**
  * @brief  unsigned int
  */
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

/**
 * @brief 封包解包的错误码
 * @enum e_comm_pkg_err
 */
enum e_comm_pkg_err {
	e_comm_pkg_err_ok               = 0,   /**< 正确*/
	e_comm_pkg_err_nomem            = 1,   /**< 内存不足*/
	e_comm_pkg_err_form             = 2,   /**< 格式错误*/
	e_comm_pkg_err_arg              = 3,   /**< 参数错误*/
    e_comm_pkg_err_outof_max_len    = 4,   /**< 数据域的长度超出上限*/
};

typedef struct
{
	unsigned int port;//license服务器程序启动时的端口
}LicenseServerInfo;

void lockFile(int fd);
void unLockFile(int fd);
int is_File_Exist(char *url);
ssize_t	buffer_getline(char **buffer, char **line);

#endif
