#!/bin/sh -


if [ "X${1}" = "X" ]; then
  arg="all"
else
  arg="$1"
fi


( make -f makefile.bsd41 "$arg" 2>&1 )            \
    | grep -v 'before array or function: ignored' \
    | grep -v 'warning: statement not reached'


# EOF
