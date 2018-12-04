1. 解压 `tar -xvf google-perftools.tar.gz`
2. `cd google-perftools`
3. 查看配置说明 `./configure -h`
4. 选择简单的安装 `mkdir /perftools`
5. `./configure --disable-cpu-profiler --disable-heap-profiler --disable-heap-checker --disable-debugalloc --enable-minimal --disable-shared`
6. make && make install