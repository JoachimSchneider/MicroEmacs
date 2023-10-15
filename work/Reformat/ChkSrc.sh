#!/bin/sh -
umask 022
LANG="C"
LC_ALL="C"
export LANG LC_ALL

check()
{
  if ! uncrustify -c /dev/null -l C < "$1" > /dev/null 2>/dev/null
  then
    echo "$1"
  fi
}

for f in "$@"
do
  check "$f"
done


# EOF
