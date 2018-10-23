#! /bin/bash
#http://blog.csdn.net/pangudashu/article/details/47664639
#automake-1.15 ubuntu
root_dir=`pwd`
if [  -f "$root_dir/aclocal.m4" ] ; then 
	rm -rf aclocal.m4
fi
if [  -f "$root_dir/autom4te.cache/" ] ; then 
	rm -rf autom4te.cache/
fi
if [  -f "$root_dir/autoscan.log" ] ; then 
	rm -rf autoscan.log
fi
if [  -f "$root_dir/compile" ] ; then 
	rm -rf compile
fi
if [  -f "$root_dir/config.log" ] ; then 
	rm -rf config.log
fi
if [  -f "$root_dir/config.status" ] ; then 
	rm -rf config.status
fi
if [  -f "$root_dir/configure" ] ; then 
	rm -rf configure
fi
if [  -f "$root_dir/configure.in" ] ; then 
	rm -rf configure.in
fi
if [  -f "$root_dir/configure.scan" ] ; then 
	rm -rf configure.scan
fi
if [  -f "$root_dir/depcomp" ] ; then 
	rm -rf depcomp
fi
if [  -f "$root_dir/install-sh" ] ; then 
	rm -rf install-sh
fi
if [  -f "$root_dir/Makefile" ] ; then 
	rm -rf Makefile
fi
if [  -f "$root_dir/Makefile.am" ] ; then 
	rm -rf Makefile.am
fi
if [  -f "$root_dir/Makefile.in" ] ; then 
	rm -rf Makefile.in
fi
if [  -f "$root_dir/missing" ] ; then 
	rm -rf missing
fi
echo "begin..."
files=`ls *.c *.h |xargs echo` #only *.c *.h
autoscan
cp configure.scan configure.in
#deal with configure.in
#手动输入程序名 版本号 邮箱地址
echo "input program's name:"
read program_name
#判断字符串是否相等时，防止空串，给判断前后均加上非空字符串 如 x
if [ "$program_name"x == ""x ];then
	program_name="hello";
fi
echo "input version:"
read version
if [ "$version"x == ""x ];then
	version="0.0.1";
fi
echo "inout your e-mail address:"
read address
if [ "$address"x == ""x ];then
	address="847379962@qq.com";
fi
sed -i 's/FULL-PACKAGE-NAME/'${program_name}'/' configure.in
sed -i 's/VERSION/'${version}'/' configure.in
sed -i 's/BUG-REPORT-ADDRESS/'${address}'/' configure.in
cat configure.in | grep -v AC_CONFIG_HEADERS > configure.in.bak
mv configure.in.bak configure.in
sed -i 's/# Checks for programs./AM_INIT_AUTOMAKE('${program_name}', '${version}')/' configure.in
sed -i 's/AC_OUTPUT/AC_OUTPUT(Makefile)/' configure.in
aclocal
autoconf
touch Makefile.am
echo "AUTOMAKE_OPTIONS=foreign" > Makefile.am
echo "bin_PROGRAMS=${program_name}" >> Makefile.am
echo "hello_SOURCES=${files}" >> Makefile.am
automake --add-missing
./configure
make clean
make


















