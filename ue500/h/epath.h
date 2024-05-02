/*======================================================================
 *      EPATH:  This file contains certain info needed to locate the
 *              MicroEMACS files on a system dependant basis.
 *
 *      possible names and paths of help files under different OSs
 *====================================================================*/

/*====================================================================*/
#ifndef EPATH_H_
#define EPATH_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#ifdef BIND_C_


/*======================================================================
 * pathname[0]:             Startup File
 * pathname[1]:             Help File
 * pathname[2:NPNAMES - 1]: Search Path for bind.c:flook(),
 *                          `""' would result in current directory, but
 *                          normaly won't get activated as this case is
 *                          handled separately in flook(); only the
 *                          startup file would be loaded from current
 *                          directory if you specify `""'.
 *
 * - flook() is used in exec.c to execute command files. For some
 *   platforms (e.g.MSDOS, Windows, OS2) also to execute binaries.
 * - In bind.c:startup() it is used to locate the startup file.
 *====================================================================*/

static NOSHARE CONST char *pathname[] =

# if     AMIGA
{
    ".emacsrc", "emacs.hlp", "", "c:", ":t/", ":s/"
};

# elif   TOS
{
    "emacs.rc", "emacs.hlp", "\\", "\\bin\\", "\\util\\"
};

# elif   FINDER
{
    "emacs.rc", "emacs.hlp", "/bin", "/sys/public"
};

# elif   MSDOS || WINNT || WINXP || CYGWIN || DJGPP_DOS
{
    "emacs.rc", "emacs.hlp", "\\sys\\public\\", "\\usr\\bin\\", "\\bin\\", "\\"
};

# elif   OS2
{
    "emacs.rc", "emacs.hlp", "C:\\OS2\\SYSTEM\\", "C:\\OS2\\DLL\\",
    "C:\\OS2\\BIN\\", "C:\\OS2\\", "\\"
};

# elif   IS_UNIX()
{
    ".emacsrc", "emacs.hlp", "/usr/local/", "/usr/lib/"
};

# elif   VMS
{
    "emacs.rc", "emacs.hlp", "SYS$LOGIN:", "MICROEMACS$LIB:"
};

# elif   WMCS
{
    "emacs.rc", "emacs.hlp", "", "sys$disk/syslib.users/"
};

# elif   AOSVS
/*
 *   NOTE: you must use the Unix style pathnames here!
 */
{
    "emacs.rc", "emacs.hlp", "", "/macros/", "/help/"
};

# elif   MPE
{
    "emacsrc", "emacshlp", ".pub", ".pub.sys"
};

# else
{
    "", "", "", "", ""
};
# endif

# define NPNAMES ( SIZEOF (pathname)/SIZEOF (char *) )


#endif  /* BIND_C_  */


/*====================================================================*/
#endif/**#ifndef EPATH_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
