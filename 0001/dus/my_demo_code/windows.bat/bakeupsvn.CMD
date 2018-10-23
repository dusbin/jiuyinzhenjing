echo 开始备份svn。。。
c:
cd \Program Files\VisualSVN Server\bin
echo 开始备份dus。。。
set timehour=%time:~0,2%
if /i %timehour% LSS 10 (
	set timehour=0%time:~1,1%
)
svnadmin dump D:\Repositories\dus >D:\dus_%date:~0,4%%date:~5,2%%date:~8,2%%timehour%%time:~3,2%%time:~6,2%.bak
echo 开始备份others
svnadmin dump D:\Repositories\others >D:\others_%date:~0,4%%date:~5,2%%date:~8,2%%timehour%%time:~3,2%%time:~6,2%.bak
explorer D: