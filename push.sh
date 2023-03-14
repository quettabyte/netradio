#!/bin/bash
#


git add .
git commit -m "submit something"

/usr/bin/expect << eof
set timeout 3
spawn git push -u origin master
expect "//gitee.com':"
send "cokeice\n"
expect "@gitee.com':"
send "3w.CNGNWLF.com\n
expect eof
eof



