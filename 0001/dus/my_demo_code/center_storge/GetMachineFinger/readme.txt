libshcrypt.a由集中存储项目中project中cryptsslserver编译
libsmbios_c.a为集中存储项目中project中hardinfo中的静态库
ar -r libshcrypt.a shcrypt.o ../hardinfo/hinfo.o certinterface.o uucode.o