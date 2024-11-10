/*======================================================================
 *      UNIX:   Unix specific terminal driver
 *              for MicroEMACS 4.0
 *      (C)Copyright 1995 D. Lawrence, C. Smith
 *
 *----------------------------------------------------------------------
 *
 *      New features: (as of version 3.10)
 *
 *      1. Timeouts waiting on a function key have been changed from
 *      35000 to 500000 microseconds.
 *
 *      2. Additional keymapping entries can be made from the command
 *      language by issuing a 'set $palette xxx'.  The format of
 *      xxx is a string as follows:
 *              "KEYMAP keybinding escape-sequence".
 *      To add "<ESC><[><A>" as a keybinding of FNN, issue:
 *              "KEYMAP FNN ~e[A".
 *      Note that the "~e" sequence represents the escape character in
 *      the MicroEMACS command language.
 *
 *      3. Colors are supported.  Under AIX the colors will be pulled
 *      in automaticly.  For other environments, you can either add
 *      the termcap entries, C0 to D7.  Or the colors may be defined
 *      using the command language by issuing a 'set $palette xxx'
 *      command.  The format of xxx is a string as follows:
 *              "CLRMAP # escape-sequence".
 *      The number is a number from 0 to 15, where 0 to 7 is the
 *      foreground colors, and 8 to 15 as background colors.
 *      To add foreground color 0 for ansi terminals, issue:
 *              "CLRMAP 0 ~e[30m".
 *
 *      'Porting notes:
 *
 *      I have tried to create this file so that it should work
 *      as well as possible without changes on your part.
 *
 *      However, if something does go wrong, read the following
 *      helpful hints:
 *
 *      1. On SUN-OS4, there is a problem trying to include both
 *      the termio.h and ioctl.h files.  I wish Sun would get their
 *      act together.  Even though you get lots of redefined messages,
 *      it shouldn't cause any problems with the final object.
 *
 *      2. In the type-ahead detection code, the individual UNIX
 *      system either has a FIONREAD or a FIORDCHK ioctl call.
 *      Hopefully, your system uses one of them and this be detected
 *      correctly without any intervention.
 *
 *      3. Also lookout for directory handling.  The SCO Xenix system
 *      is the weirdest I've seen, requiring a special load file
 *      (see below).  Some machine call the result of a readdir() call
 *      a "struct direct" or "struct dirent".  Includes files are
 *      named differently depending of the O/S.  If your system doesn't
 *      have an opendir()/closedir()/readdir() library call, then
 *      you should use the public domain utility "ndir".
 *
 *      To compile:
 *              Compile all files normally.
 *      To link:
 *              Select one of the following operating systems:
 *                      SCO Xenix:
 *                              use "-ltermcap" and "-lx";
 *                      SUN 3 and 4:
 *                              use "-ltermcap";
 *                      IBM-RT, IBM-AIX, ATT UNIX, Altos UNIX, Interactive:
 *                              use "-lcurses".
 *
 *      - 20 feb 95     New version 4.00 features
 *        We added new code to implient a TERMIOS driver
 *====================================================================*/

/*====================================================================*/
#define UNIXTERM_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/** Include files **/
#include <stdio.h>              /* Standard I/O definitions */
#include "estruct.h"            /* Emacs definitions        */
#include "eproto.h"             /* Function definitions     */


/*==============================================================*/
/* FEATURES                                                     */
/*==============================================================*/




/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
