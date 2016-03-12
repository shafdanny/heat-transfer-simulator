#!/bin/bash
EXEC=./bin/main 
DEFAULTOPTION="-i 10000 -e 012345 -s 024 -t 13 -m"

if [ "$#" -gt "0" ]; then
	$EXEC $@
else
	$EXEC $DEFAULTOPTION
fi