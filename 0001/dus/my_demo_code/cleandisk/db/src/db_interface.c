//*******************************************************************************
// 版权所有(C) 2015 uxsino All right reserved
// 文件名称 : db_interface.c
// 当前版本 : 1.0.0.1
// 作    者 : zhaozongpeng (zhaozp@uxsino.com)
// 设计日期 : 2015年12月30日
// 内容摘要 : 数据库操作接口定义
// 修改记录 : 
// 日    期  版    本  修改人   修改摘要

//*******************************************************************************

#include "db_interface.h"
//#include "ServOpLogCommon.h"
//#include "PubListenHostLog.h"

#include "sqlite3.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define DEBUG printf
/********************************
****  macro  define        ******
*********************************/
/*  #define _DB_DEBUG_SWITCH_  ON   // 本文件调试代码开关
#define DEBUG printf*/  

/********************************
**** interface declare     ******
*********************************/

// 初始化sqlite3数据库实例
int db_sqlite3_connect(const char *pszDb_full_path);
int db_sqlite3_connect_ex(const char *pszDb_full_path,int flag);
//查询
int db_sqlite3_query(const char *zSql,     /* SQL to be evaluated */
                char ***pazResult,    /* Results of the query */
                int *pnRow,           /* Number of result rows written here */
                int *pnColumn);       /* Number of result columns written here */   

int db_sqlite3_query_ex(const char *zSql,     /* SQL to be evaluated */
                char ***pazResult,    /* Results of the query */
                int *pnRow,           /* Number of result rows written here */
                int *pnColumn,int flag);       /* Number of result columns written here */   
// 执行sql语句，只返回结果码
int db_sqlite3_exec(const char *zSql);    /* SQL to be evaluated */
int db_sqlite3_exec_ex(const char *zSql,int flag);    /* SQL to be evaluated */
// 删除查询返回的二维表内存
void db_sqlite3_free_query_result(char **result);
// 关闭数据库例
int db_sqlite3_close(void);
int db_sqlite3_close_ex(int flag);


// 插入数据
int db_insert(const char *zSql, const char *pszTbl_name);
int db_insert_ex(const char *zSql, const char *pszTbl_name,int flag);
// 删除表，只返回结果码
int db_drop_tbl(const char *zSql, const char *pszTbl_name);   
int db_drop_tbl_ex(const char *zSql, const char *pszTbl_name,int flag);   
// 删除数据，只返回结果码
int db_del_data(const char *zSql, const char *pszTbl_name);   
int db_del_data_ex(const char *zSql, const char *pszTbl_name,int flag);   
// 创建数据表，只返回结果码
int db_create_tbl(const char *zSql, const char *pszTbl_name);   
int db_create_tbl_ex(const char *zSql, const char *pszTbl_name,int flag);   


// 初始化数据库表
int db_tbl_init();
int db_tbl_init_ex(int flag);


#ifdef _DB_DEBUG_SWITCH_
// 样例，调测调用接口
void db_test_main();
#endif


/********************************
**** global variable define******
*********************************/
static DB_CB_ST *s_pstDb_cb = NULL;                 // 数据库上下文
static DB_TYPE *s_pDb_instance = NULL;              // 全局数据库实例
static DB_CB_ST_EX *s_pstDb_cb_ex[e_max_db_size] = {NULL};
static DB_TYPE *s_pDb_instance_ex[e_max_db_size] = {NULL};

typedef struct _Db_backup_stat
{
    pthread_mutex_t _lock;
    int _stat_flag_serv_op;
    int _stat_flag_file_integrality;
}Db_backup_stat;

static Db_backup_stat s_db_backup_stat;

// 接口初始化列表, sqlite3字段接口中是调用原始数据库函数
#define DB_INTERFACE_LIST  db_sqlite3_connect, \
                           db_sqlite3_query, \
                           db_sqlite3_free_query_result, \
                           db_sqlite3_exec, \
                           db_sqlite3_close, \
                           db_insert, \
                           db_create_tbl, \
                           db_drop_tbl, \
                           db_del_data
                                
#define DB_INTERFACE_LIST_EX  db_sqlite3_connect_ex, \
                           db_sqlite3_query_ex, \
                           db_sqlite3_free_query_result, \
                           db_sqlite3_exec_ex, \
                           db_sqlite3_close_ex, \
                           db_insert_ex, \
                           db_create_tbl_ex, \
                           db_drop_tbl_ex, \
                           db_del_data_ex                                                    

static DB_CB_ST s_stDCB_init = {DB_INTERFACE_LIST};
static DB_CB_ST_EX s_stDCB_init_ex = {DB_INTERFACE_LIST_EX};
/********************************
**** interface define      ******
*********************************/

// 获取数据库实例对象
DB_CB_ST* get_db_instance()
{
    return s_pstDb_cb;
}
DB_CB_ST_EX* get_db_instance_ex(int flag)
{
    return s_pstDb_cb_ex[flag];
}
// 初始化数据库
int db_init_ex(int flag)
{
    int iret = DB_ERR_SUC;
    //static DB_CB_ST_EX s_stDCB_init = {DB_INTERFACE_LIST_EX};

    if(NULL != s_pstDb_cb_ex[flag])
    {
        return DB_ERR_SUC;
    }

#if (DB_TYPE_USED == DB_TYPE_SQLITE3)
    // 初始化数据库实例结构
    s_pstDb_cb_ex[flag] = &s_stDCB_init_ex;
#else
    return DB_ERR_DB_TYPE_UNKNOW;
#endif
    
    // 打开数据库
    switch(flag)
    {
        case e_query_db:
            iret = s_pstDb_cb_ex[flag]->db_connect_fun(DATABASE_FULL_PATH,flag);        
            break;
    }
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }

    // 初始化数据表
    iret = db_tbl_init_ex(flag);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }
    DEBUG("LYJ--- FINISH db_init_ex,\n");
    return DB_ERR_SUC;
}
int db_init()
{
    int iret = DB_ERR_SUC;
    

    if(NULL != s_pstDb_cb)
    {
        return DB_ERR_SUC;
    }

//#if (DB_TYPE_USED == DB_TYPE_SQLITE3)
    // 初始化数据库实例结构
    s_pstDb_cb = &s_stDCB_init;
//#else
//    return DB_ERR_DB_TYPE_UNKNOW;
//#endif
    
    // 打开数据库
    iret = s_pstDb_cb->db_connect_fun(DATABASE_FULL_PATH);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }

    // 初始化数据表
    //iret = db_tbl_init();
    //if(DB_ERR_SUC != iret)
    //{
    //    return iret;
    //}
#ifdef _DB_DEBUG_SWITCH_
    // 样例，调测调用接口
    db_test_main();
#endif

    return DB_ERR_SUC;
}

// 初始化数据库表
int db_tbl_init_ex(int flag)
{
    int iret = DB_ERR_SUC;
    
    char sql_tbl_info[512] = {0};
    switch(flag)
    {
        case e_query_db:
        {
            sprintf(sql_tbl_info,"%s",CREATE_TBL_UX_TBL_INFO);
            break;
        }
        case e_serv_log_db:
        {
            sprintf(sql_tbl_info,"%s",CREATE_TBL_SERVOPLOG);
            break;
        }
        case e_file_integrality_check_log_db:
        {
            sprintf(sql_tbl_info,"%s",CREATE_TBL_FILE_INTEGRALITY_LOG);
            break;
        }
        default:
            break;
    }
    if(NULL == s_pstDb_cb_ex[flag])
    {
        return DB_ERR_ERROR;
    }

    iret = s_pstDb_cb_ex[flag]->db_exec_fun(sql_tbl_info,flag);
    return iret;
}
int db_tbl_init()
{
    int iret = DB_ERR_SUC;
    char *sql_tbl_info = "create table if not exists tbl_ux_tbl_info(tname varchar(128) not null primary key,num integer,update_time integer,op_type int);";    
    char sql_query[256] = {0};
    char sql_insert[256] = {0};
    char **query_rst = NULL;
    int row = 0;
    int col = 0;
    if(NULL == s_pstDb_cb)
    {
        return DB_ERR_SUC;
    }
    
    iret = s_pstDb_cb->db_exec_fun(sql_tbl_info);
    if (DB_ERR_SUC != iret)
    {
        return iret;
    }
    // 鉴别日志设置数据库表初始化
    iret = s_pstDb_cb->db_exec_fun(CREATE_TBL_LOG_BACKUP_CONFIG);
    if (DB_ERR_SUC != iret)
    {
        return iret;
    }
    sprintf(sql_query,"select * from %s",TBL_LOG_BACKUP_CONFIG);
    s_pstDb_cb->db_query_fun(sql_query,&query_rst,&row,&col);
    if (row < 1)
    {
        sprintf(sql_insert,INSERT_LOG_BACKUP_CFG,backup_serv_alarm_log,10,100,DATABASE_FULL_PATH);
        s_pstDb_cb->db_insert_fun(sql_insert,TBL_LOG_BACKUP_CONFIG);
        
        memset(sql_insert,0x0,256);
        sprintf(sql_insert,INSERT_LOG_BACKUP_CFG,backup_file_integrality_log,10,100,DATABASE_FULL_PATH);
        s_pstDb_cb->db_insert_fun(sql_insert,TBL_LOG_BACKUP_CONFIG);
    }
    s_pstDb_cb->db_free_query_result_fun(query_rst);
    // 系统信息监控阈值数据库表初始化
    iret = s_pstDb_cb->db_exec_fun(CREATE_TBL_SYS_INFO_THRESHOLD);
    if (DB_ERR_SUC != iret)
    {
        return iret;
    }
    // 文件完整性检查配置表初始化
    iret = s_pstDb_cb->db_exec_fun(CREATE_TBL_FILE_INTEGRALITY_CONFIG);
    if (DB_ERR_SUC != iret)
    {
        return iret;
    }

	//安全运行检查文件md5码表初始化
	iret = s_pstDb_cb->db_exec_fun(CREATE_TBL_SECURITY_OPER_CHECK);
    if (DB_ERR_SUC != iret)
    {
        return iret;
    }

	//安全基线初始配置表初始化
	iret = s_pstDb_cb->db_exec_fun(CREATE_TBL_BASE_LINE_CHECK);
    if (DB_ERR_SUC != iret)
    {
        return iret;
    }

    //端口管理初始化
    iret = s_pstDb_cb->db_exec_fun(CREATE_TBL_PORT_STATUS);
    if (DB_ERR_SUC != iret)
    {
        return iret;    
    }
    return iret;
}

// 关闭数据库
void db_close_ex(int flag)
{
    int iret = DB_ERR_SUC;
    
    if(NULL == s_pstDb_cb_ex[flag])
    {
        return ;
    }

    // 关闭数据库
    iret = s_pstDb_cb_ex[flag]->db_close_fun(flag);
    if(DB_ERR_SUC != iret)
    {
        printf("db_close err[%d]\n", iret);
    }

    // 释放数据库对象内存
    //uxsino_free(s_pstDb_cb); // 使用局部静态变量初始化，无需释放
    s_pstDb_cb_ex[flag] = NULL;
    
    return;
}
void db_close()
{
    int iret = DB_ERR_SUC;
    
    if(NULL == s_pstDb_cb)
    {
        return ;
    }

    // 关闭数据库
    iret = s_pstDb_cb->db_close_fun();
    if(DB_ERR_SUC != iret)
    {
        printf("db_close err[%d]\n", iret);
    }

    // 释放数据库对象内存
    //uxsino_free(s_pstDb_cb); // 使用局部静态变量初始化，无需释放
    s_pstDb_cb = NULL;
    
    return;
}


// 查询  其结构是：第一行是列名，随后的行才是值。
// 遍历的方式和二维数组相同, 用一维数据存储
/*
    数据表内容示例:
    Name        | Age
    -----------------------
    Alice       | 43
    Bob         | 28
    Cindy       | 21

    返回结果数组:
    azResult[0] = "Name";
    azResult[1] = "Age";
    azResult[2] = "Alice";
    azResult[3] = "43";
    azResult[4] = "Bob";
    azResult[5] = "28";
    azResult[6] = "Cindy";
    azResult[7] = "21";

*/
int db_sqlite3_query_ex(const char *zSql,     /* SQL to be evaluated */
                char ***pazResult,    /* Results of the query */
                int *pnRow,           /* Number of result rows written here */
                int *pnColumn,int flag)        /* Number of result columns written here */  
{
    int iret = DB_ERR_SUC;
    char *pzErrmsg = NULL;
    
    if(NULL == s_pDb_instance_ex[flag])
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if(NULL == zSql)
    {
        return DB_ERR_SQL_ERR;
    }

    // 查询数据库
    iret = sqlite3_get_table(s_pDb_instance_ex[flag], 
                    zSql, 
                    pazResult, 
                    pnRow, 
                    pnColumn, 
                    &pzErrmsg);
    if(SQLITE_OK != iret)
    {
        if(NULL == pzErrmsg)
        {
            printf("sqlite3_get_table err[%d]\n", iret);
        } else {
            printf("sqlite3_get_table err[%d][%s]\n", iret, pzErrmsg);
            sqlite3_free(pzErrmsg);
            pzErrmsg = NULL;
        }
        return iret;
    }
    return DB_ERR_SUC;
}
int db_sqlite3_query(const char *zSql,     /* SQL to be evaluated */
                char ***pazResult,    /* Results of the query */
                int *pnRow,           /* Number of result rows written here */
                int *pnColumn)        /* Number of result columns written here */   
{
    int iret = DB_ERR_SUC;
    char *pzErrmsg = NULL;
    
    if(NULL == s_pDb_instance)
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if(NULL == zSql)
    {
        return DB_ERR_SQL_ERR;
    }

    // 查询数据库
    iret = sqlite3_get_table(s_pDb_instance, 
                    zSql, 
                    pazResult, 
                    pnRow, 
                    pnColumn, 
                    &pzErrmsg);
    if(SQLITE_OK != iret)
    {
        if(NULL == pzErrmsg)
        {
            printf("sqlite3_get_table err[%d]\n", iret);
        }
        else
        {
            printf("sqlite3_get_table err[%d][%s]\n", iret, pzErrmsg);
            sqlite3_free(pzErrmsg);
            pzErrmsg = NULL;
        }
        
        return iret;
    }
    return DB_ERR_SUC;
}


// 插入数据
int db_insert(const char *zSql, const char *pszTbl_name) 
{
    int iret = DB_ERR_SUC;
    
    if(NULL == s_pstDb_cb)
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = s_pstDb_cb->db_exec_fun(zSql);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }       

    return DB_ERR_SUC;
}

int db_insert_ex(const char *zSql, const char *pszTbl_name,int flag) 
{
    int iret = DB_ERR_SUC;
    
    if(NULL == s_pstDb_cb_ex[flag])
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = s_pstDb_cb_ex[flag]->db_exec_fun(zSql,flag);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }       

    return DB_ERR_SUC;
}
// 执行sql语句，只返回结果码
int db_sqlite3_exec(const char *zSql)   /* SQL to be evaluated */
{
    int iret = DB_ERR_SUC;
    char *pzErrmsg = NULL;      // 错误信息
    
    if(NULL == s_pDb_instance)
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if(NULL == zSql)
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = sqlite3_exec(s_pDb_instance, 
                        zSql, 
                        NULL, 
                        NULL, 
                        &pzErrmsg);
    if(SQLITE_OK != iret)
    {
        if(NULL == pzErrmsg)
        {
            printf("sqlite3_exec err[%d]\n", iret);
        }
        DEBUG("sqlite3_exec err[%d][%s]\n", iret, pzErrmsg);
        sqlite3_free(pzErrmsg);
        pzErrmsg = NULL;
        return iret;
    }       

    return DB_ERR_SUC;
}
int db_sqlite3_exec_ex(const char *zSql,int flag)   /* SQL to be evaluated */
{
    int iret = DB_ERR_SUC;
    char *pzErrmsg = NULL;      // 错误信息
    
    if(NULL == s_pDb_instance_ex[flag])
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if(NULL == zSql)
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = sqlite3_exec(s_pDb_instance_ex[flag], 
                        zSql, 
                        NULL, 
                        NULL, 
                        &pzErrmsg);
    if(SQLITE_OK != iret)
    {
        if(NULL == pzErrmsg)
        {
            DEBUG("sqlite3_exec err[%d]\n", iret);
        }
        DEBUG("sqlite3_exec err[%d][%s]\n", iret, pzErrmsg);
        sqlite3_free(pzErrmsg);
        pzErrmsg = NULL;
        return iret;
    }       

    return DB_ERR_SUC;
}

// 删除查询返回的二维表内存
void db_sqlite3_free_query_result(char **result)
{
    if((NULL == result) || (NULL == *result))
    {
        return;
    }
    
    sqlite3_free_table(result);
}

// 关闭数据库例
int db_sqlite3_close(void)
{
    int iret = DB_ERR_SUC;
    if(NULL == s_pDb_instance)
    {
        return iret;
    }
    
    iret = sqlite3_close(s_pDb_instance);
    if(SQLITE_OK != iret)
    {
        printf("sqlite3_close err:%d\n", iret);
        return iret;
    }
    s_pDb_instance = NULL;
    
    return DB_ERR_SUC;
}

int db_sqlite3_close_ex(int flag)
{
    int iret = DB_ERR_SUC;
    if(NULL == s_pDb_instance_ex[flag])
    {
        return iret;
    }
    
    iret = sqlite3_close(s_pDb_instance_ex[flag]);
    if(SQLITE_OK != iret)
    {
        printf("sqlite3_close err:%d\n", iret);
        return iret;
    }
    s_pDb_instance_ex[flag] = NULL;
    
    return DB_ERR_SUC;
}

// 初始化sqlite3数据库实例
int db_sqlite3_connect(const char *pszDb_full_path)
{
    int iret = DB_ERR_SUC;
    if(NULL != s_pDb_instance)
    {
        return DB_ERR_DB_OPENED;
    }
    
    if(NULL == pszDb_full_path)
    {
        return DB_ERR_AGRS_ERR;
    }

    // 打开数据库
    iret = sqlite3_open(pszDb_full_path, &s_pDb_instance);
    if(SQLITE_OK != iret)
    {
        s_pDb_instance = NULL;
        printf("sqlite3_open err:%d\n", iret);
        return iret;
    }
    
    return DB_ERR_SUC;
}

int db_sqlite3_connect_ex(const char *pszDb_full_path,int flag)
{
    int iret = DB_ERR_SUC;
    if(NULL != s_pDb_instance_ex[flag])
    {
        return DB_ERR_DB_OPENED;
    }
    
    if(NULL == pszDb_full_path)
    {
        return DB_ERR_AGRS_ERR;
    }

    // 打开数据库
    iret = sqlite3_open(pszDb_full_path, &s_pDb_instance_ex[flag]);
    if(SQLITE_OK != iret)
    {
        s_pDb_instance_ex[flag] = NULL;
        printf("sqlite3_open err:%d\n", iret);
        return iret;
    }
    
    return DB_ERR_SUC;
}

// 删除表，只返回结果码
int db_drop_tbl(const char *zSql, const char *pszTbl_name)   
{
    int iret = DB_ERR_SUC;
    char sql[DB_SQL_MAX_LEN] = {0};
    
    if(NULL == s_pstDb_cb)
    {
        return DB_ERR_NULL_DBCB_ERR;
    }
    
    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行删除表sql
    iret = s_pstDb_cb->db_exec_fun(zSql);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }

    // 更新表信息表中的内容
    sprintf(sql, "delete from tbl_ux_tbl_info where tname=\"%s\";", pszTbl_name);
    iret = s_pstDb_cb->db_exec_fun(sql);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }
    
#if (ON == DB_VACUUM_SPACE_SWITCH)
    // 释放磁盘空间
    sprintf(sql, "vacuum;");
    iret = s_pstDb_cb->db_exec_fun(sql);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }
#endif
    
    return DB_ERR_SUC;
}

int db_drop_tbl_ex(const char *zSql, const char *pszTbl_name,int flag)   
{
    int iret = DB_ERR_SUC;
    char sql[DB_SQL_MAX_LEN] = {0};
    
    if(NULL == s_pstDb_cb_ex[flag])
    {
        return DB_ERR_NULL_DBCB_ERR;
    }
    
    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行删除表sql
    iret = s_pstDb_cb_ex[flag]->db_exec_fun(zSql,flag);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }

    // 更新表信息表中的内容
    if (flag == e_query_db)
    {
        sprintf(sql, "delete from tbl_ux_tbl_info where tname=\"%s\";", pszTbl_name);
        iret = s_pstDb_cb_ex[flag]->db_exec_fun(sql,flag);
        if(DB_ERR_SUC != iret)
        {
            return iret;
        }
    }
    
    
#if (ON == DB_VACUUM_SPACE_SWITCH)
    // 释放磁盘空间
    sprintf(sql, "vacuum;");
    iret = s_pstDb_cb_ex[flag]->db_exec_fun(sql,flag);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }
#endif
    
    return DB_ERR_SUC;
}

// 删除数据，只返回结果码
int db_del_data(const char *zSql, const char *pszTbl_name)
{
    int iret = DB_ERR_SUC;
    
    if(NULL == s_pstDb_cb)
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = s_pstDb_cb->db_exec_fun(zSql);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }       

    return DB_ERR_SUC;
}

int db_del_data_ex(const char *zSql, const char *pszTbl_name,int flag)
{
    int iret = DB_ERR_SUC;
    
    if(NULL == s_pstDb_cb_ex[flag])
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = s_pstDb_cb_ex[flag]->db_exec_fun(zSql,flag);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }       

    return DB_ERR_SUC;
}

// 创建数据表，只返回结果码
int db_create_tbl(const char *zSql, const char *pszTbl_name)  
{
    int iret = DB_ERR_SUC;
    char sql[DB_SQL_MAX_LEN] = {0};
    
    if(NULL == s_pstDb_cb)
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = s_pstDb_cb->db_exec_fun(zSql);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }       

    // 更新表信息表中的内容，数据预留后期优化,避免频繁刷新库表
    sprintf(sql, "insert into tbl_ux_tbl_info(tname, num, update_time, op_type) values(\"%s\",0,0,0);", pszTbl_name);
    iret = s_pstDb_cb->db_exec_fun(sql);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }
    
    return DB_ERR_SUC;
}

int db_create_tbl_ex(const char *zSql, const char *pszTbl_name,int flag)  
{
    int iret = DB_ERR_SUC;
    char sql[DB_SQL_MAX_LEN] = {0};
    
    if(NULL == s_pstDb_cb_ex[flag])
    {
        return DB_ERR_NULL_DBCB_ERR;
    }

    if((NULL == zSql) || (NULL == pszTbl_name))
    {
        return DB_ERR_AGRS_ERR;
    }

    // 执行SQL语句
    iret = s_pstDb_cb_ex[flag]->db_exec_fun(zSql,flag);
    if(DB_ERR_SUC != iret)
    {
        return iret;
    }       

    // 更新表信息表中的内容，数据预留后期优化,避免频繁刷新库表
    if (flag == e_query_db)
    {
        sprintf(sql, "insert into tbl_ux_tbl_info(tname, num, update_time, op_type) values(\"%s\",0,0,0);", pszTbl_name);
        iret = s_pstDb_cb_ex[flag]->db_exec_fun(sql,flag);
        if(DB_ERR_SUC != iret)
        {
            return iret;
        }
    }
    
    
    return DB_ERR_SUC;
}

// 初始化备份状态
void init_backup_db_stat_flag(void)
{
    pthread_mutex_init(&(s_db_backup_stat._lock),NULL);
    s_db_backup_stat._stat_flag_file_integrality = 0;
    s_db_backup_stat._stat_flag_serv_op = 0;
}
// 设置指定数据库的备份状态
void set_backup_db_stat_flag(int stat,int db_type)
{
    pthread_mutex_lock(&(s_db_backup_stat._lock));
    if (db_type == backup_serv_alarm_log)
        s_db_backup_stat._stat_flag_serv_op = stat;
    else if (db_type == backup_file_integrality_log)
        s_db_backup_stat._stat_flag_file_integrality = stat;
    pthread_mutex_unlock(&(s_db_backup_stat._lock));
}
// 获取指定数据库的备份状态
int get_backup_db_stat_flag(int db_type)
{
    int stat = 0;
    pthread_mutex_lock(&(s_db_backup_stat._lock));
    if (db_type == backup_serv_alarm_log)
        stat = s_db_backup_stat._stat_flag_serv_op;
    else if (db_type == backup_file_integrality_log)
        stat = s_db_backup_stat._stat_flag_file_integrality;
    pthread_mutex_unlock(&(s_db_backup_stat._lock));
    return stat;
}
// 释放备份状态标记
void release_backup_db_stat_flag(void)
{
    pthread_mutex_destroy(&(s_db_backup_stat._lock));
    s_db_backup_stat._stat_flag_file_integrality = 0;
    s_db_backup_stat._stat_flag_serv_op = 0;
}

///////////////////////////////////////////////////////////////
// 样例，调测代码
// 调用db_test_main 可测试建表，插入，查询，清表，删表
//#ifdef _DB_DEBUG_SWITCH_
//  db_test_main();
//#endif    
//
///////////////////////////////////////////////////////////////
#ifdef _DB_DEBUG_SWITCH_    //调试代码
#include <stdio.h>
#include <time.h>
const int const_data_count = 99999;
time_t timep;

#define TIME_PRINT(x) do{\
        time (&timep);\
        DEBUG("%s:%s\n",x, asctime(gmtime(&timep)));\
    }while(0)
    
int db_test_insert_commit();
int db_test_create_table();
void db_test_show_data(char **pszData, int row, int column);
int db_test_query();
int db_test_drop_table_data();
int db_test_drop_table();



void db_test_main()
{
    int iret = 0;

    db_test_drop_table();
    do
    {
        // 建表
        iret = db_test_create_table();
        if(0 != iret)
        {
            break;
        }

        // 插入数据
        iret = db_test_insert_commit();
        if(0 != iret)
        {
            break;
        }

        // 查询
        iret = db_test_query();
        if(0 != iret)
        {
            printf("db_test_query err\n");
            break;
        }

        // 删除所有数据,效率比较低
        iret = db_test_drop_table_data();
        if(0 != iret)
        {
            printf("db_test_drop_table_data err\n");
            break;
        }

        // 删除表，表中数据不用直接删除表
        iret = db_test_drop_table();
        if(0 != iret)
        {
            printf("db_test_drop_table err\n");
            break;
        }
        
    }while(1000);
    printf("db_test_main %d\n", iret);

    return;
}

// 事务方式插入测试数据
int db_test_insert_commit()
{

    char sql[1024] = {0};
    int iret = 0;
    int icount = 0; // 数据记录条数

    const DB_CB_ST *pstDB = get_db_instance();
    if(NULL == pstDB)
    {
        printf("get_db_instance NULL\n");
        return -1;
    }   

    do
    {
        TIME_PRINT("insert begin");
        
        // 开启事务
        iret = pstDB->db_insert_fun("begin;", "db_test");
        if(0 != iret)
        {
            break;
        }
        
        for(icount = 0; icount < const_data_count; icount++)
        {
            memset(sql, 0x00, sizeof(sql));
            sprintf(sql, 
                    "insert into db_test(sid, sname,time) values(%d, \"n_%d\", \"2015-01-01\");\n", 
                    icount, icount);

            iret = pstDB->db_insert_fun(sql, , "db_test");
            if(0 != iret)
            {
                break;
            }
        }       
    }
    while(0);

    if(0 != iret)
    {
        iret = pstDB->db_insert_fun("rollback;", "db_test");
        return iret;
    }
    iret = pstDB->db_insert_fun("commit;", "db_test");
    TIME_PRINT("insert end");
    return iret;
}

// 创建表
int db_test_create_table()
{
    int iret = 0;
    char *strsql1 = "create table if not exists db_test(sid integer not null primary key,sname varchar(256),time date)";
    char *strsql2 = "create table if not exists ux_group(GID integer not null primary key,group_name varchar(256),user_list TEXT)";

    DB_CB_ST *pstDB = get_db_instance();
    if(NULL == pstDB)
    {
        printf("get_db_instance NULL\n");
        return -1;
    }   

    iret = pstDB->db_create_tbl_fun(strsql1, "db_test");   
    return iret;
}

int db_test_drop_table()
{
    int iret = 0;
    char *strsql = "drop table db_test;";
    DB_CB_ST *pstDB = get_db_instance();
    if(NULL == pstDB)
    {
        printf("get_db_instance NULL\n");
        return -1;
    }   

    TIME_PRINT("drop table begin");
    iret = pstDB->db_drop_tbl_fun(strsql, "db_test");
    TIME_PRINT("drop table end");
    return iret;
}

int db_test_drop_table_data()
{
    int iret = 0;
    char *strsql = "delete from db_test;";
    DB_CB_ST *pstDB = get_db_instance();
    if(NULL == pstDB)
    {
        printf("get_db_instance NULL\n");
        return -1;
    }   

    TIME_PRINT("drop table data begin");
    iret = pstDB->db_del_data_fun(strsql, "db_test");
    TIME_PRINT("drop table data end");
    return iret;
}

// 第一行为表头；按一维数据来读数据
void db_test_show_data(char **pszData, int row, int column)
{
    int i = 0;
    int j = 0;

    for(; i <= row; i++)
    {
        for(j = 0; j < column; j++)
        {
            printf("%s\t", pszData[i * column + j]);
        }
        printf("\n");
    }
    
    return;
}


int db_test_query()
{
    int iret = 0;
    char sql[1024] = {0};

    int irow = 0;
    int icolumn = 0;
    char **pszresult = NULL;
    int offset = 0;
    
    DB_CB_ST *pstDB = get_db_instance();
    if(NULL == pstDB)
    {
        printf("get_db_instance NULL\n");
        return -1;
    }

    TIME_PRINT("select begin");
    // 查询所有数据
    do
    {
        memset(sql, 0x00, sizeof(sql));
        sprintf(sql, "select * from db_test order by sid limit 200 offset %d;", offset);

        iret = pstDB->db_query_fun(sql, &pszresult, &irow, &icolumn);
        if(0 != iret)
        {
            printf("db_query_fun err\n");
            break;
        }

        // 显示数据
        //db_test_show_data(pszresult, irow, icolumn);
        
        // 释放查询结果内存
        pstDB->db_free_query_result_fun(pszresult);
        pszresult = NULL;

        if(irow < 200)
        {
            break;
        }

        offset += 200;
    }while(!iret);

    // 释放查询结果内存
    pstDB->db_free_query_result_fun(pszresult);
    pszresult= NULL;
    TIME_PRINT("select end");
    return iret;
}


#endif      //调试代码

/*
* nh 2017.05.19 
* 成功:返回服务器基本信息中的主机唯一ID；失败:返回NULL
*/
int get_server_base_info_HostUuid(char * ret)
{
    //char* ret = NULL;
	int rt = 0;
    char sql[1024] = {0};
	int row = 0, col = 0;
	DB_CB_ST* db_cb_st = NULL;
	char** data = NULL;
   
	db_cb_st = get_db_instance();
	if(NULL == db_cb_st){
		return NULL;
	}
	
	sprintf(sql, SELECT_TBL_SERVER_BASE_INFO, "HostUuid");
	rt = db_cb_st->db_query_fun(sql, &data, &row, &col);
	if(DB_ERR_SUC != rt){
		db_cb_st->db_exec_fun("rollback;");
	}
	db_cb_st->db_exec_fun("commit;");
	if(row > 0){
		strcpy(ret, data[1]);
	}
	else{
		ret = NULL;
	}

    db_cb_st->db_free_query_result_fun(data);
    data = NULL;

	return rt;
}



