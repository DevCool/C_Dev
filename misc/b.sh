#!/bin/sh
blah="$1"
test_file="$2"
if [ ! "$blah" = "" ]; then
	sed -i "s/Hello welt./$blah/" $test_file
fi
