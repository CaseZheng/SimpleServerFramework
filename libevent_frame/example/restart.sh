#!/usr/bin/sh

NAME=example

ps -aef | grep $NAME | grep -v grep
killall -9 $NAME

./bin/example
ps -aef | grep $NAME | grep -v grep
