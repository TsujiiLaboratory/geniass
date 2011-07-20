#!/bin/sh

# Wrapping script for GeniaSS

# TODO: Does not check for the existance of Ruby
# TODO: Does not check for the existance of the binary
# TODO: Path conversion etc. should be done by the program
# XXX: Can't pass arguments to the program and doing so duplicates the logic

if [ $# -ne 2 ] && [ $# -ne 3 ]; then
	echo "Usage: run_geniass.sh <in-file> <out-file> [path-to-ruby]" 1>&2
	exit 1
fi

PROG_DIR=`dirname $0`

# Turn file-paths into absolute paths
IN_FILE=`readlink -f $1`
OUT_FILE=`readlink -f $2`
RUBY=$3

# The program uses relative paths and must be run with CWD set to its root
cd $PROG_DIR && ./geniass $IN_FILE $OUT_FILE $RUBY
