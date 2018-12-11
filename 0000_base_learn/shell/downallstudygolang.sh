# ! /bin/sh
i=1
while [ $i -le 16827 ]
do
	curl "https://studygolang.com/articles/"$i -o "articles_"$i.html
	title=$i_`cat articles_$i.html |grep "<title>"|awk -F'>' '{print$2}'|awk -F'<' '{printf$1}'`.html
	mv articles_$i.html "$title"
	let i++
done
