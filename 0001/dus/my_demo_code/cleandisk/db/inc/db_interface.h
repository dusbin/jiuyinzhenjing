//*******************************************************************************
// 版权所有(C) 2015 uxsino All right reserved
// 文件名称 : db_interface.h
// 当前版本 : 1.0.0.1
// 作    者 : zhaozongpeng (zhaozp@uxsino.com)
// 设计日期 : 2015年12月30日
// 内容摘要 : 数据库操作接口定义
// 修改记录 : 
// 日    期  版    本  修改人   修改摘要

//*******************************************************************************

#ifndef __DB_INTERFACE_H__
#define __DB_INTERFACE_H__
#define ON      1
#define OFF     0


#define DATABASE_FULL_PATH          "/opt/data.db"    // 数据库文件路径，后续考虑加到配置文件
#define DB_TYPE_SQLITE3             1                       // sqlite3 类型
#define DB_SQL_MAX_LEN              256
#define DB_TBL_NAME_MAX_LEN         64
#define DB_BUSY_RETRY_COUNT         3   // 次
#define DB_BUSY_RETRY_DELAY_TIME    10  // ms


// 数据库在删除表或数据后是否释放磁盘空间, 数据文件大时影响性能
#define DB_VACUUM_SPACE_SWITCH      ON              
// 数据库正在被加锁，返回BUSY，开启后重试
#define DB_BUSY_RETRY_SWITCH        ON
// 数据库返回BUSY,重试时，每次延迟时间
#define DB_BUSY_RETRY_DELAY_SWITCH  ON


// 建表语句
#define CREATE_TBL_UX_TBL_INFO "create table if not exists tbl_ux_tbl_info(tname varchar(128) not null primary key,num integer,update_time integer,op_type int);"
// 鉴别日志表
#define CREATE_TBL_SERVOPLOG "CREATE TABLE if not exists 'tbl_servoplog' ('level'  INTEGER NOT NULL,\
'subjectid'  INTEGER NOT NULL,\
'subjectname'  TEXT NOT NULL,\
'objectid'  INTEGER NOT NULL,\
'objectname'  TEXT NOT NULL,\
'time'  TEXT NOT NULL,\
'evtid'  INTEGER NOT NULL,\
'evtdesc'  TEXT NOT NULL,\
'result' INTEGER);"
#define SELECT_TBL_SERVOPLOG_REPEAT_SYS_ALARM "SELECT time FROM tbl_servoplog WHERE \
objectname = '%s' ORDER BY time DESC LIMIT 0,1;"

// 日志备份设置表
typedef enum 
{
    backup_serv_alarm_log = 0,
    backup_file_integrality_log
}log_backup_type;
#define TBL_LOG_BACKUP_CONFIG "tbl_log_backup_config"
#define CREATE_TBL_LOG_BACKUP_CONFIG "create table if not exists tbl_log_backup_config\
('type' TEXT not null,'filenum' TEXT not null,'filesize' TEXT not null,'backupdir' TEXT,'curfilenum' TEXT);"
#define INSERT_LOG_BACKUP_CFG "INSERT INTO tbl_log_backup_config (type,filenum,filesize,curfilenum,backupdir) \
VALUES ('%d','%d','%d','0','%s');"
#define UPDATE_LOG_BACKUP_CONFIG "UPDATE tbl_log_backup_config SET filenum = '%d', filesize = '%d' , backupdir = '%s' WHERE type = '%d';"

#define UPDATE_LOG_BACKUP_CONFIG_CUR "UPDATE tbl_log_backup_config SET filenum = '%d', filesize = '%d' , backupdir = '%s' , curfilenum = '%d' WHERE type = '%d';"

#define UPDATE_LOG_BACKUP_CONFIG_CUR_NUM "UPDATE tbl_log_backup_config SET curfilenum = '%d' WHERE type = '%d';"
#define QUERY_LOG_BACKUP_CONFIG "SELECT filenum,filesize,backupdir,curfilenum FROM tbl_log_backup_config WHERE type = '%d';"

// 系统信息阈值表
#define TBL_SYS_INFO_THRESHOLD "tbl_sys_info_threshold"
#define CREATE_TBL_SYS_INFO_THRESHOLD "create table if not exists tbl_sys_info_threshold \
('type' TEXT not null,'eq_name' TEXT not null,'threshold' TEXT not null);"
#define INSERT_TBL_SYS_INFO_THRESHOLD "INSERT INTO tbl_sys_info_threshold ('type','eq_name','threshold') \
VALUES('%d','%s','%d');"
#define UPDATE_TBL_SYS_INFO_THRESHOLD "UPDATE tbl_sys_info_threshold SET type = '%d',eq_name = '%s',threshold = '%d' \
WHERE type = '%d' AND eq_name = '%s';"
#define SELECT_TBL_SYS_INFO_THRESHOLD_WHERE_TYPE_NAME "SELECT COUNT(*) FROM tbl_sys_info_threshold WHERE \
type = '%d' AND eq_name = '%s';"
#define SELECT_TBL_SYS_INFO_THRESHOLD_ALL "SELECT type,eq_name,threshold FROM tbl_sys_info_threshold;"
#define SELCT_TBL_SYS_INFO_THRESHOLD_COUNT_TYPE "SELECT COUNT(*) FROM tbl_sys_info_threshold WHERE \
type = '%d';"
#define SELECT_TBL_SYS_INFO_THRESHOLD_BY_NAME "SELECT type,eq_name,threshold FROM tbl_sys_info_threshold WHERE eq_name = '%s';"
// 完整性设置表
#define TBL_FILE_INTEGRALITY_CONFIG "tbl_file_integrality_config"
#define CREATE_TBL_FILE_INTEGRALITY_CONFIG "create table if not exists tbl_file_integrality_config \
('dir' TEXT not null,'file_name' TEXT not null,'md5' TEXT,'config_time' TEXT);"
#define INSERT_TBL_FILE_INTEGRALITY_CONFIG "INSERT INTO tbl_file_integrality_config ('dir','file_name','md5','config_time') \
VALUES('%s','%s','%s','%s');"
#define UPDATE_TBL_FILE_INTEGRALITY_CONFIG "UPDATE tbl_file_integrality_config SET md5 = '%s',config_time = '%s' WHERE \
dir = '%s' AND file_name = '%s';"
#define SELECT_TBL_FILE_INTEGRALITY_CONFIG_WHERE_DIR_FILENAME "SELECT md5,config_time FROM tbl_file_integrality_config WHERE \
dir = '%s' AND file_name = '%s';"
#define DELETE_TBL_FILE_INTEGRALITY_CONFIG_WHERE_DIR_FILENAME "DELETE FROM tbl_file_integrality_config WHERE \
dir = '%s' AND file_name = '%s';"

// 完整性检查日志表
//modify by hx on 2015.05.06
#define DELETE_TBL_FILE_INTEGRALITY_LOG "DELETE FROM tbl_file_integrality_log WHERE dir ='%s' and file_name='%s';"
#define SELECT_TBL_FILE_INTEGRALITY_CONFIG_ALL "SELECT dir,file_name FROM tbl_file_integrality_config;"
//end by hx

#define CREATE_TBL_FILE_INTEGRALITY_LOG "create table if not exists tbl_file_integrality_log \
('dir' TEXT not null,'file_name' TEXT not null,'md5' TEXT,'config_time' INTEGER,'check_result' TEXT);"
#define INSERT_TBL_FILE_INTEGRALITY_LOG "INSERT INTO tbl_file_integrality_log ('dir','file_name','md5','config_time','check_result') \
VALUES('%s','%s','%s',%s,'%d');"
#define SELECT_TBL_FILE_INTEGRALITY_LOG_FILE_NAME "SELECT dir,file_name,md5,config_time,check_result FROM tbl_file_integrality_log \
WHERE file_name LIKE '%%%s%%' ORDER BY config_time DESC LIMIT %d,%d;"
#define SELECT_TBL_FILE_INTEGRALITY_LOG_TIME "SELECT dir,file_name,md5,config_time,check_result FROM tbl_file_integrality_log \
WHERE config_time BETWEEN %s AND %s ORDER BY config_time DESC LIMIT %d,%d;"

#define SELECT_TBL_FILE_INTEGRALITY_LOG_RESULT "SELECT dir,file_name,md5,config_time,check_result FROM tbl_file_integrality_log \
WHERE check_result = '%s' ORDER BY config_time DESC LIMIT %d,%d;"
//安全运行检查表
#define TBL_SECURITY_OPER_CHECK "tbl_security_oper_check"
#define CREATE_TBL_SECURITY_OPER_CHECK "create table if not exists tbl_security_oper_check(file_path varchar(256),md5 varchar(256));"
#define INSERT_TBL_SECURITY_OPER_CHECK "INSERT INTO tbl_security_oper_check (file_path,md5) VALUES(\"%s\",\"%s\");"

#define TBL_BASE_LINE_CHECK "tbl_base_line_check"
#define CREATE_TBL_BASE_LINE_CHECK "create table if not exists tbl_base_line_check(class varchar(256),value TEXT);"
#define INSERT_TBL_BASE_LINE_CHECK "INSERT INTO tbl_base_line_check (class,value) VALUES(\"%s\",\"%s\");"
#define INSERT_TBL_PASSWD_DAY "INSERT INTO tbl_base_line_check (class,value) VALUES(\"%s\",\"%s\");"
#define SELECT_TBL_PASSWD_DAY "SELECT value FROM tbl_base_line_check WHERE class = '%s';"

#define TBL_SERVER_BASE_INFO "tbl_server_base_info"
#define CREATE_TBL_SERVER_BASE_INFO "create table if not exists tbl_server_base_info(class varchar(256),value TEXT);"
#define INSERT_TBL_SERVER_BASE_INFO "INSERT INTO tbl_server_base_info (class,value) VALUES(\"%s\",\"%s\");"
#define SELECT_TBL_SERVER_BASE_INFO "SELECT value FROM tbl_server_base_info WHERE class = '%s';"
#define DELETE_TBL_SERVER_BASE_INFO "delete from tbl_server_base_info;"

#define TBL_PORT_STATUS "tbl_port_status"
#define CREATE_TBL_PORT_STATUS "create table if not exists tbl_port_status(class varchar(256),value TEXT);"
#define INSERT_TBL_PORT_STATUS "INSERT INTO tbl_port_status (class,value) VALUES(\"%s\",\"%s\");"
#define SELECT_TBL_PORT_STATUS "SELECT * FROM tbl_port_status;"
#define UPDATE_TBL_PORT_STATUS "UPDATE tbl_port_status SET value = '%s' WHERE class = '%s';"
#define DELETE_TBL_PORT_STATUS "DELETE FROM tbl_port_status WHERE class ='%s' ;"


#if (DB_TYPE_USED == DB_TYPE_SQLITE3)
struct sqlite3;
#define DB_TYPE struct sqlite3                              // 数据库类型定义
#else
#define DB_TYPE void
#endif


typedef enum db_flag
{
    e_query_db = 0,
    e_serv_log_db,
    e_file_integrality_check_log_db,        // 文件完整性检查日志
    e_max_db_size
}DB_FLAG;

#define DB_MODULE_ID        0x20                            // 模块ID
#define MAKECODE(module, code) (module << 16 | code)

//#define SERV_ALARM_LOG_BACKUP_PATH ALARM_LOG_FULL_PATH
//#define SERV_FILE_INTEGRALITY_LOG_BACKUP_PATH FILE_LOG_FULL_PATH


// 数据库实例对象，及相关操作接口
typedef struct db_context_block
{
    int (*db_connect_fun)(const char*);             // 连接数据库
    int (*db_query_fun)(const char *,               // 查询数据库，返回结果为二维表
                    char ***, 
                    int *, 
                    int *);
    void (*db_free_query_result_fun)(char **result);            // 删除查询返回的二维表内存
    int (*db_exec_fun)(const char *);                           // 执行sql语句,只返回执行结果
    int (*db_close_fun)(void);                                  // 关闭数据库实例
    int (*db_insert_fun)(const char *, const char *);           // 插入数据库，执行插入sql语句
    int (*db_create_tbl_fun)(const char *, const char *);       // 执行创建表 sql语句
    int (*db_drop_tbl_fun)(const char *, const char *);         // 删除表会释入占用的磁盘空间
    int (*db_del_data_fun)(const char *, const char *);         // 执行删除数据 sql语句
}DB_CB_ST;

typedef struct db_context_block_ex
{
    int (*db_connect_fun)(const char*,int);             // 连接数据库
    int (*db_query_fun)(const char *,               // 查询数据库，返回结果为二维表
                    char ***, 
                    int *, 
                    int *,
                    int);
    void (*db_free_query_result_fun)(char **result);            // 删除查询返回的二维表内存
    int (*db_exec_fun)(const char *,int);                           // 执行sql语句,只返回执行结果
    int (*db_close_fun)(int);                                  // 关闭数据库实例
    int (*db_insert_fun)(const char *, const char *,int);           // 插入数据库，执行插入sql语句
    int (*db_create_tbl_fun)(const char *, const char *,int);       // 执行创建表 sql语句
    int (*db_drop_tbl_fun)(const char *, const char *,int);         // 删除表会释入占用的磁盘空间
    int (*db_del_data_fun)(const char *, const char *,int);         // 执行删除数据 sql语句    
}DB_CB_ST_EX;
// 数据库操作返回码定义
typedef enum db_errcode
{
    DB_ERR_SUC                      = 0x00,
    DB_ERR_ERROR                    = MAKECODE(DB_MODULE_ID, 0x00), // 错误码起始值 0x20 0000
    DB_ERR_MALLOC_ERR               = MAKECODE(DB_MODULE_ID, 0x01), // 错误码值 0x20 0001 malloc 失败
    DB_ERR_AGRS_ERR                 = MAKECODE(DB_MODULE_ID, 0x02), // 错误码值 0x20 0002 入参错误
    DB_ERR_NULL_DBCB_ERR            = MAKECODE(DB_MODULE_ID, 0x03), // 错误码值 0x20 0003 DB_CB_ST*指针为空，没有初始化
    DB_ERR_SQL_ERR                  = MAKECODE(DB_MODULE_ID, 0x04), // 错误码值 0x20 0004 SQL语句错误
    DB_ERR_QUERY_ERR                = MAKECODE(DB_MODULE_ID, 0x05), // 错误码值 0x20 0005 查询失败
    DB_ERR_DB_TYPE_UNKNOW           = MAKECODE(DB_MODULE_ID, 0x06), // 错误码值 0x20 0006 数据库类型错误
    DB_ERR_DB_OPENED                = MAKECODE(DB_MODULE_ID, 0x07), // 错误码值 0x20 0007 数据库已经打开
}DB_ERR_EN;





// 获取数据库实例
DB_CB_ST* get_db_instance();
DB_CB_ST_EX* get_db_instance_ex(int flag);
// 初始化数据库
int db_init();
int db_init_ex(int);
// 关闭数据库
void db_close();
void db_close_ex(int);

// 初始化备份状态
void init_backup_db_stat_flag(void);
// 设置指定数据库的备份状态0未进行备份 1进行备份
void set_backup_db_stat_flag(int stat,int db_type);
// 获取指定数据库的备份状态
int get_backup_db_stat_flag(int db_type);
// 释放备份状态标记
void release_backup_db_stat_flag(void);
// 获取服务器基本信息中uuid
int get_server_base_info_HostUuid(char *ret);


#endif

