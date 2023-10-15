#!/bin/sh -
umask 022
LANG="C"
LC_ALL="C"
export LANG LC_ALL

mydir=`dirname "$0"`
mydir=`cd "$mydir"; pwd`
cfg="${mydir}/uncrstfy.cfg"
error_exit=43

diag()
{
  echo "DIAG: ${*}" 1>&2
}

error()
{
  echo "ERROR: ${*}" 1>&2

  exit $error_exit
}

Reformat()
{
  [ -d "$2" ] || error "${2} is not a directory"
  echo "SOURCE: ${1}"
  if ! ( uncrustify -c "$cfg" -l C < "$1" > "${2}/${1}" ) 2>&1
  then
    error "Processing ${1}"
  fi
}

dir="$1"
shift

for f in "$@"
do
  Reformat "$f" "$dir"
done


# EOF

