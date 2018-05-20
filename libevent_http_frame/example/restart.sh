#!/usr/bin/sh

NAME=libevent_http_example

ps -aef | grep $NAME | grep -v grep
killall -9 $NAME

./bin/$NAME
ps -aef | grep $NAME | grep -v grep
