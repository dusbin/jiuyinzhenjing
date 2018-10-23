#! /bin/bash
if [ ! -s "/opt/uxsino/cdps/agent/etc/" ] ; then
  mkdir -p /opt/uxsino/cdps/agent/etc/
fi
echo "init file name begin"
sqlite3 -init upgradeDB.sql /opt/uxsino/cdps/agent/etc/dzb.db <<EOF
.quit
EOF

echo "init-------------end"

echo "select data"
echo "file_id    dev_id     file_name    file_type  file_role  file_lev"
echo "select * from t_file_auth;">/tmp/select.sql
data=`sqlite3 -init /tmp/select.sql /opt/uxsino/cdps/agent/etc/dzb.db <<EOF
.quit
EOF`
echo '' $data >/tmp/data
sed -e 's/|/          /g' /tmp/data >/tmp/data.bak
mv /tmp/data.bak /tmp/data
cat /tmp/data
rm -rf /tmp/select.sql
rm -rf /tmp/data

