#!/usr/bin/sh

NAME=example

ps -aef | grep $NAME
killall -9 $NAME

./bin/example
ps -aef | grep $NAME
