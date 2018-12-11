# ! /bin/sh
number(){
        if [ $i -le 9 ];then
                echo "0000"$i
        else if [ $i -le 99 ];then
                echo "000"$i
        else if [ $i -le 999 ];then
                echo "00"$i
        else if [ $i -le 9999 ];then
                echo "0"$i
        else 
                echo $i
        fi
	fi
	fi
	fi
}
i=1
while [ $i -le 16827 ]
do
	curl "https://studygolang.com/articles/"$i -o "articles_"$i.html
	title=`number`"_"`cat articles_$i.html |grep "<title>"|awk -F'>' '{print$2}'|awk -F'<' '{printf$1}'i|awk -F'/' '{printf$1"_"$2}'`.html
	mv articles_$i.html "$title"
	let i++
done
