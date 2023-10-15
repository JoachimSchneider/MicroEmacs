#!/bin/sh -
umask 022
LANG="C"
LC_ALL="C"
export LANG LC_ALL

mydir=`dirname "$0"`
mydir=`cd "$mydir"; pwd`
cfg="${mydir}/uncrstfy.cfg"


Test_Reformat()
{
  echo "SOURCE: ${1}"
  if ! ( uncrustify -c "$cfg" -l C < "$1" > /dev/null ) 2>&1
  then
    echo "ERROR: ${1}"
  fi
}

for f in "$@"
do
  Test_Reformat "$f"
done


# EOF

