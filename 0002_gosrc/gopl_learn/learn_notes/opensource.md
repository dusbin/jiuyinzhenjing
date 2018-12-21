# gogs使用
1. 源码安装
    ```
    $ git clone https://github.com/gogs/gogs.git
    $ cd gogs
    $ make
    $ ./gogs web
    $ web access : http://:3000
2. 使用标签构建
    Gogs 默认并没有支持一些功能，这些功能需要在构建时明确使用构建标签（build tags）来支持。

    目前使用标签构建的功能如下：
    ```
    sqlite3：SQLite3 数据库支持
    pam：PAM 授权认证支持
    cert：生成自定义证书支持
    miniwinsvc：Windows 服务内置支持（或者您可以使用 NSSM 来创建服务）
    ```
    例如，您需要支持以上所有功能，则需要先删除 $GOPATH/pkg/{GOOS_GOARCH}/github.com/gogs/gogs 目录，然后执行以下命令：
    ```
    $ go get -u -tags "sqlite pam cert" github.com/gogs/gogs
    $ cd $GOPATH/src/github.com/gogs/gogs
    $ go build -tags "sqlite pam cert"
    ```
    安装完成后可继续参照 配置与运行。
3. 参考资料

    [源码安装](https://gogs.io/docs/installation/install_from_source.html)
