$! SOF(mk_scancode.com)
$ CC scancode.c
$ CC terminal_vms.c
$ LINK scancode.obj,terminal_vms.obj
$ DIR/TIME scancode.exe
$! EOF(mk_scancode.com)
