/*======================================================================
 *      EPATH:  This file contains certain info needed to locate the
 *              MicroEMACS files on a system dependent basis.
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
 * spathnam[0]:             Global Startup File
 * spathnam[1]:             Local Startup File
 * spathnam[2]:             Help File
 * spathnam[3:NPNAMES - 1]: Search Path for bind.c:flook(),
 *                          `""' would result in current directory, but
 *                          won't get activated as this case is
 *                          handled separately in flook().
 *
 * - flook() is used in exec.c to execute command files. For some
 *   platforms (e.g.MSDOS, Windows, OS2) also to execute binaries.
 * - In bind.c:startup() it is used to locate the startup file.
 *====================================================================*/

static NOSHARE CONST char *spathnam[] =

# if     AMIGA
{
    "emacs.ini", ".emacsrc", "emacs.hlp", "", "c:", ":t/", ":s/"
};

# else
# if   TOS
{
    "emacs.ini", "emacs.rc", "emacs.hlp", "\\", "\\bin\\", "\\util\\"
};

# else
# if   FINDER
{
    "emacs.ini", "emacs.rc", "emacs.hlp", "/bin/", "/sys/public/"
};

# else
# if   MSDOS || WINNT || WINXP || CYGWIN || DJGPP_DOS
{
    "emacs.ini", "emacs.rc", "emacs.hlp", "\\sys\\public\\",
    "\\usr\\bin\\", "\\bin\\", "\\"
};

# else
# if   OS2
{
    "emacs.rc", "emacs.hlp", "C:\\OS2\\SYSTEM\\", "C:\\OS2\\DLL\\",
    "C:\\OS2\\BIN\\", "C:\\OS2\\", "\\"
};

# else
# if   b_IS_UNIX
{
    "emacs.ini", ".emacsrc", "emacs.hlp", "/usr/local/",
    "/usr/lib/"
};

# else
# if   VMS
{
    "emacs.ini", "emacs.rc", "emacs.hlp", "SYS$LOGIN:",
    "MICROEMACS$LIB:"
};

# else
# if   WMCS
{
    "emacs.ini", "emacs.rc", "emacs.hlp", "", "sys$disk/syslib.users/"
};

# else
# if   AOSVS
/*
 *   NOTE: you must use the Unix style pathnames here!
 */
{
    "emacs.ini", "emacs.rc", "emacs.hlp", "", "/macros/", "/help/"
};

# else
# if   MPE
{
    "emacs.ini", "emacsrc", "emacshlp", ".pub", ".pub.sys"
};

# else
{
    "", "", "", "", "", ""
};
# endif
# endif
# endif
# endif
# endif
# endif
# endif
# endif
# endif
# endif

# define NPNAMES ( SIZEOF (spathnam)/SIZEOF (char *) )


#endif  /* BIND_C_  */


/*====================================================================*/
#endif/**#ifndef EPATH_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
