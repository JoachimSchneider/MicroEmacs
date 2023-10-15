#!/bin/sh -
umask 022
LANG="C"
LC_ALL="C"
export LANG LC_ALL
exec uncrustify -c /dev/null --show-config
# EOF
