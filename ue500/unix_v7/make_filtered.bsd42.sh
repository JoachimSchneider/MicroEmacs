#!/bin/sh -

( make -f makefile.bsd42 "$@" 2>&1 )              \
    | grep -v 'before array or function: ignored' \
    | grep -v 'warning: statement not reached'


# EOF
