#!/usr/bin/sh

NAME=libevent_tcp_example

ps -aef | grep $NAME | grep -v grep
killall -9 $NAME && sleep 1 && ./bin/$NAME
ps -aef | grep $NAME | grep -v grep
