echo update begin
set timehour=%time:~0,2%
if /i %timehour% LSS 10 (
	set timehour=0%time:~1,1%
)
ren backtime* backtime_%date:~0,4%年%date:~5,2%月%date:~8,2%日%timehour%时%time:~3,2%分%time:~6,2%秒.txt
echo update end