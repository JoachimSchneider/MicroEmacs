/*	EPATH:	This file contains certain info needed to locate the
		MicroEMACS files on a system dependant basis.

									*/

/*	possible names and paths of help files under different OSs	*/

NOSHARE CONST char *pathname[] =

#if	AMIGA
{
	".emacsrc",
	"emacs.hlp",
	"",
	"c:",
	":t/",
	":s/"
};

#elif	TOS
{
	"emacs.rc",
	"emacs.hlp",
	"\\",
	"\\bin\\",
	"\\util\\",
	""
};
 
#elif	FINDER
{
	"emacs.rc",
	"emacs.hlp",
	"/bin",
	"/sys/public",
	""
};

#elif	MSDOS || WINNT || WINXP
{
	"emacs.rc",
	"emacs.hlp",
	"\\sys\\public\\",
	"\\usr\\bin\\",
	"\\bin\\",
	"\\",
	""
};

#elif	OS2
{
        "emacs.rc",
        "emacs.hlp",
        "C:\\OS2\\SYSTEM\\",
        "C:\\OS2\\DLL\\",
        "C:\\OS2\\BIN\\",
        "C:\\OS2\\",
        "\\",
        ""
};

#elif	IS_UNIX()
{
	".emacsrc",
	"emacs.hlp",
	"/usr/local/",
	"/usr/lib/",
	""
};

#elif	VMS
{
	"emacs.rc",
	"emacs.hlp",
	"MICROEMACS$LIB:",
	"SYS$LOGIN:",
	""
};

#elif	WMCS
{
	"emacs.rc",
	"emacs.hlp",
	"",
	"sys$disk/syslib.users/"
};

#elif	AOSVS
/*
    NOTE: you must use the Unix style pathnames here!
*/
{
    "emacs.rc",
    "emacs.hlp",
    "",
    "/macros/",
    "/help/"
};

#elif	MPE
{
	"emacsrc",
	"emacshlp",
	".pub",
	".pub.sys",
	""
};

#else
{
	"",
	"",
	"",
	"",
	""
};
#endif

#define	NPNAMES	(sizeof(pathname)/sizeof(char *))
