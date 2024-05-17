$ !
$ ! MAANSIMAKE - For those VMS users who do not have MMS
$ !
$ !
$ ! Set up some symbols.  Even if you use this file
$ ! to make MicroEmacs, you should look through
$ ! MESMG.MMS and MESMG.OPT to see what your options
$ ! are.  This file will create an SMG version of MicroEmacs.
$ ! If you prefer an ANSI version, look at MEANSI.MMS and
$ ! MEANSI.OPT.
$ !
$ ! Be sure to set the correct directory for medir!
$ !
$	medir = "QDATA:[USERS.JOACHIM.UE500.VMS.BUILD]"
$	curdir = f$trnlnm("sys$disk") + f$directory()
$	cc_cmd = "cc /nolist"
$	lnk_cmd = "link /map/brief"
$ !
$ ! Let's do it!
$ !
$	set default 'medir'
$	delete *.OBJ;*
$       delete *.EXE;*
$       copy /REPLACE [--.SRC]*.*  *.*
$       copy /REPLACE [--.H]*.*    *.*
$	copy /REPLACE [-]*.h       *.*
$	copy /REPLACE [-]*.opt     *.*
$ !
$ !
$	cc_cmd abbrev
$	cc_cmd ansi
$	cc_cmd basic
$	cc_cmd bind
$	cc_cmd buffer
$	cc_cmd char
$	cc_cmd crypt
$	cc_cmd display
$ !!	cc_cmd dolock
$	cc_cmd eval
$	cc_cmd exec
$	cc_cmd file
$	cc_cmd fileio
$	cc_cmd history
$	cc_cmd input
$	cc_cmd isearch
$       cc_cmd keyboard
$	cc_cmd line
$	cc_cmd lock
$	cc_cmd main
$ !	cc_cmd me
$	cc_cmd mouse
$	cc_cmd random
$	cc_cmd region
$	cc_cmd replace
$	cc_cmd search
$	cc_cmd screen
$ !	cc_cmd smg
$     	cc_cmd tags
$	cc_cmd undo
$	cc_cmd vms
$	cc_cmd window
$	cc_cmd word
$ !
$	lnk_cmd meansi/option
$	set default 'curdir'
$	exit
