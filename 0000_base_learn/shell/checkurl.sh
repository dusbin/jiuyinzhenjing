# ! /bin/bash
. /etc/init.d/functions # for more colour
function Connection(){
   wget --spider -q -o /dev/null --tries=1 -T 5 http://$1
   if [ "$?" -eq 0 ] # if success
      then
      action "http://$1 Monitoring success!" /bin/true
   else
      action "http://$1 Failure of monitoring!" /bin/false
   fi
}
webaddress="
www.baidu.com
www.jd.com
www.taobao.com
www.123haha.com
www.090909.com
www.627ywndw.com
www.aiqiyi.com
www.nginx.org
www.apache.org
"
function main(){
    for U in $webaddress
       do
           Connection $U
       done
}
main


