1. 导入sql文件到mysql数据库 datatable 为要导入的数据库名称
    `mysql -u username -p datatable < sqlfile.sql`
2. 查看mysql服务端口
    ```
    mysql -u root -p
    mysql>show global variables like 'port';
3. 创建&删除数据库
    ```
    create DATABASE gogs;
    drop DATABASE gogs;
4. 列出所有数据库
    ```
    show databases;
5. 列出所有表
    ```
    show tables;
6. 退出
    ```
    exit