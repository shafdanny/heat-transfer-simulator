#!/bin/bash
EXEC=./bin/main
DEFAULTOPTION="-i 10000 -e 0 -s 0"

if [ "$#" -gt "0" ]; then
	$EXEC $@
else
	$EXEC $DEFAULTOPTION
fi