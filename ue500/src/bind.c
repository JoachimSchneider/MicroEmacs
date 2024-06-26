/*======================================================================
 * This file is for functions having to do with key bindings,
 * descriptions, help commands and startup file.
 *
 * written 11-feb-86 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#define BIND_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include "estruct.h"
#include "eproto.h"
#include "edef.h"
#include "elang.h"
#include "epath.h"


/* give me some help!!!!!
 *  bring up a fake buffer and read the help
 * file into it with view mode
 */
int PASCAL NEAR help P2_(int, f, int, n /* prefix flag and argument */)
{
    REGISTER BUFFER *bp     = NULL;   /* buffer pointer to help */
    CONST char      *fname  = NULL;   /* file name of help file */

    /* first check if we are already here */
    bp = bfind("emacs.hlp", FALSE, BFINVS);

    if ( bp == NULL ) {
        fname = flook(pathname[1], FALSE, TRUE);
        if ( fname == NULL ) {
            mlwrite(TEXT12);

/*              "[Help file is not online]" */
            return (FALSE);
        }
    }

    /* split the current window to make room for the help stuff */
    if ( splitwind(FALSE, 1) == FALSE )
        return (FALSE);

    if ( bp == NULL ) {
        /* and read the stuff in */
        if ( getfile(fname, FALSE) == FALSE )
            return (FALSE);
    } else
        swbuffer(bp);

    /* make this window in VIEW mode, update all mode lines */
    curwp->w_bufp->b_mode |= MDVIEW;
    curwp->w_bufp->b_flag |= BFINVS;
    upmode();

    return (TRUE);
}

/* DESKEY:
 *
 * Describe the command for a certain key
 */
int PASCAL NEAR deskey P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int        c;                /* key to describe                        */
    REGISTER CONST char *ptr;             /* string pointer to scan output strings  */
    char                outseq[NSTRING];  /* output buffer for command sequence     */

    /* prompt the user to type us a key to describe */
    mlwrite(TEXT13);
/*      ": describe-key " */

    /* get the command sequence to describe change it to something we can print
     * as well */
    /* and dump it out */
    ostring( cmdstr(c = getckey(FALSE), &outseq[0]) );
    ostring(" ");

    /* find the right ->function */
    if ( ( ptr = getfname( getbind(c) ) ) == NULL )
        ptr = "Not Bound";

    /* output the command sequence */
    ostring(ptr);

    return (TRUE);
}

/* BINDTOKEY:
 *
 * Add a new key to the key binding table
 */
int PASCAL NEAR bindtokey P2_(int, f, int, n)
/* f, n:  Command arguments [IGNORED] */
{
    REGISTER unsigned int c;    /* command key to bind */
    REGISTER ue_fnc_T kfunc;    /* ptr to the requested function to
                                 * bind to  */
    REGISTER KEYTAB *ktp;       /* pointer into the command table   */
    REGISTER int found;         /* matched command flag */
    char outseq[80];            /* output buffer for keystroke sequence */

    /* prompt the user to type in a key to bind */
    /* get the function name to bind it to */
    kfunc = getname(TEXT15);
/*          ": bind-to-key " */
    if ( kfunc == NULL ) {
        mlwrite(TEXT16);
/*          "[No such function]" */

        return (FALSE);
    }
    if ( clexec == FALSE ) {
        ostring(" ");
        TTflush();
    }

    /* get the command sequence to bind */
    c = getckey( (kfunc == f_meta) || (kfunc == cex) || (kfunc == unarg)
                                   || (kfunc == ctrlg) );

    if ( clexec == FALSE ) {
        /* change it to something we can print as well */
        /* and dump it out */
        ostring( cmdstr(c, &outseq[0]) );
    }

    /* if the function is a unique prefix key */
    if ( kfunc == unarg || kfunc == ctrlg || kfunc == quote ) {
        /* search for an existing binding for the prefix key */
        ktp = &keytab[0];
        while ( ktp->k_type != BINDNUL ) {
            if ( ktp->k_type == BINDFNC && ktp->k_ptr.fp == kfunc )
                unbindchar(ktp->k_code);
            ++ktp;
        }

        /* reset the appropriate global prefix variable */
        if ( kfunc == unarg )
            reptc = c;
        if ( kfunc == ctrlg )
            abortc = c;
        if ( kfunc == quote )
            quotec = c;
    }

    /* search the table to see if it exists */
    ktp = &keytab[0];
    found = FALSE;
    while ( ktp->k_type != BINDNUL )  {
        if ( ktp->k_code == (int)c )  {
            found = TRUE;
            break;
        }
        ++ktp;
    }

    if ( found ) {      /* it exists, just change it then */
        ktp->k_ptr.fp = kfunc;
        ktp->k_type = BINDFNC;
    } else {            /* otherwise we need to add it to the end */
        /* if we run out of binding room, bitch */
        if ( ktp >= &keytab[NBINDS] ) {
            mlwrite(TEXT17);

/*              "Binding table FULL!" */
            return (FALSE);
        }

        ktp->k_code = c;                /* add keycode */
        ktp->k_ptr.fp = kfunc;          /* and the function pointer */
        ktp->k_type = BINDFNC;          /* and the binding type */
        ++ktp;                          /* and make sure the next is null */
        ktp->k_code = 0;
        ktp->k_type = BINDNUL;
        ktp->k_ptr.fp = NULL;
    }

    /* if we have rebound the meta key, make the search terminators follow it           */
    if ( kfunc == f_meta ) {
        sterm = c;
        isterm = c;
    }

    return (TRUE);
}

/* MACROTOKEY:
 *
 * Bind a key to a macro in the key binding table
 */
int PASCAL NEAR macrotokey P2_(int, f, int, n)
/* f, n:  Command arguments [IGNORED] */
{
    REGISTER unsigned int c;    /* command key to bind */
    REGISTER BUFFER *kmacro;    /* ptr to buffer of macro to bind to key */
    REGISTER KEYTAB *ktp;       /* pointer into the command table */
    REGISTER int found;         /* matched command flag */
    REGISTER int status;        /* error return */
    char outseq[80];            /* output buffer for keystroke sequence */
    char bufn[NBUFN];           /* buffer to hold macro name */

    /* get the buffer name to use */
    if ( ( status=mlreply(TEXT215, &bufn[1], NBUFN-2) ) != TRUE )
/*      ": macro-to-key " */
        return (status);

    /* build the responce string for later */
    XSTRCPY(outseq, TEXT215);
/*         ": macro-to-key " */
    XSTRCAT(outseq, &bufn[1]);

    /* translate it to a buffer pointer */
    bufn[0] = '[';
    XSTRCAT(bufn, "]");
    if ( ( kmacro=bfind(bufn, FALSE, 0) ) == NULL ) {
        mlwrite(TEXT130);

/*      "Macro not defined"*/
        return (FALSE);
    }

    XSTRCAT(outseq, " ");
    mlwrite(outseq);

    /* get the command sequence to bind */
    c = getckey(FALSE);

    /* change it to something we can print as well */
    /* and dump it out */
    ostring( cmdstr(c, &outseq[0]) );

    /* search the table to see if it exists */
    ktp = &keytab[0];
    found = FALSE;
    while ( ktp->k_type != BINDNUL )  {
        if ( ktp->k_code == (int)c )  {
            found = TRUE;
            break;
        }
        ++ktp;
    }

    if ( found ) {      /* it exists, just change it then */
        ktp->k_ptr.buf = kmacro;
        ktp->k_type = BINDBUF;
    } else {            /* otherwise we need to add it to the end */
        /* if we run out of binding room, bitch */
        if ( ktp >= &keytab[NBINDS] ) {
            mlwrite(TEXT17);

/*              "Binding table FULL!" */
            return (FALSE);
        }

        ktp->k_code = c;                /* add keycode */
        ktp->k_ptr.buf = kmacro;                /* and the function pointer */
        ktp->k_type = BINDBUF;          /* and the binding type */
        ++ktp;                          /* and make sure the next is null */
        ktp->k_code = 0;
        ktp->k_type = BINDNUL;
        ktp->k_ptr.fp = NULL;
    }

    return (TRUE);
}

/* UNBINDKEY:
 *
 * Delete a key from the key binding table
 */
int PASCAL NEAR unbindkey P2_(int, f, int,n)
/* f, n:  Command arguments [IGNORED] */
{
    REGISTER int c;             /* command key to unbind */
    char outseq[80];            /* output buffer for keystroke sequence */

    /* prompt the user to type in a key to unbind */
    mlwrite(TEXT18);
/*      ": unbind-key " */

    /* get the command sequence to unbind */
    c = getckey(FALSE);                 /* get a command sequence */

    /* change it to something we can print as well */
    /* and dump it out */
    ostring( cmdstr(c, &outseq[0]) );

    /* if it isn't bound, bitch */
    if ( unbindchar(c) == FALSE ) {
        mlwrite(TEXT19);

/*          "[Key not bound]" */
        return (FALSE);
    }

    return (TRUE);
}

/* UNBINDCHAR:
 */
int PASCAL NEAR unbindchar P1_(int, c)
/* c: Command key to unbind */
{
    REGISTER KEYTAB *ktp;       /* pointer into the command table */
    REGISTER KEYTAB *sktp;      /* saved pointer into the command table */
    REGISTER int found;         /* matched command flag */

    /* search the table to see if the key exists */
    ktp = &keytab[0];
    found = FALSE;
    while ( ktp->k_type != BINDNUL ) {
        if ( ktp->k_code == c ) {
            found = TRUE;
            break;
        }
        ++ktp;
    }

    /* if it isn't bound, bitch */
    if ( !found )
        return (FALSE);

    /* save the pointer and scan to the end of the table */
    sktp = ktp;
    while ( ktp->k_type != BINDNUL )
        ++ktp;
    --ktp;              /* backup to the last legit entry */

    /* copy the last entry to the current one */
    sktp->k_code = ktp->k_code;
    sktp->k_type = ktp->k_type;
    if ( sktp->k_type == BINDFNC )
        sktp->k_ptr.fp   = ktp->k_ptr.fp;
    else if ( sktp->k_type == BINDBUF )
        sktp->k_ptr.buf   = ktp->k_ptr.buf;

    /* null out the last one */
    ktp->k_code = 0;
    ktp->k_type = BINDNUL;
    ktp->k_ptr.fp = NULL;

    return (TRUE);
}

/* UNBIND_BUF:
 *
 * Unbind all the keys bound to a buffer (which we can then delete)
 */
VOID PASCAL NEAR unbind_buf P1_(BUFFER *, bp)
/* bp:  Buffer to unbind all keys connected to  */
{
    REGISTER KEYTAB *ktp;       /* pointer into the command table */

    /* search the table to see if the key exists */
    ktp = &keytab[0];
    while ( ktp->k_type != BINDNUL ) {
        if ( ktp->k_type == BINDBUF ) {
            if ( ktp->k_ptr.buf == bp ) {
                unbindchar(ktp->k_code);
                --ktp;
            }
        }
        ++ktp;
    }
}

/* DESBIND:
 *
 * Describe bindings:
 *
 * Bring up a fake buffer and list the key bindings into it with view
 * mode
 */
int PASCAL NEAR desbind P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    return ( buildlist(TRUE, "") );
}

/* APRO:
 * Apropos (List functions that match a substring)
 */
int PASCAL NEAR apro P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    char mstring[NSTRING];      /* string to match cmd names to */
    int status;                 /* status return */

    status = mlreply(TEXT20, mstring, NSTRING - 1);
/*           "Apropos string: " */
    if ( status != TRUE )
        return (status);

    return ( buildlist(FALSE, mstring) );
}

/* BUILDLIST:
 *
 * build a binding list (limited or full)
 */
int PASCAL NEAR buildlist P2_(int, type, CONST char *, mstring)
/* type:    True = full list,   false = partial list  */
/* mstring: Match string if a partial list            */
{
    REGISTER KEYTAB *ktp;       /* pointer into the command table */
    REGISTER NBIND *nptr;       /* pointer into the name binding table */
    REGISTER BUFFER *listbuf;    /* buffer to put binding list into */
    REGISTER BUFFER *bp;        /* buffer ptr for function scan */
    int cpos;                   /* current position to use in outseq */
    char outseq[80];            /* output buffer for keystroke sequence */
    int first_entry;            /* is this the first macro listing? */

    /* get a buffer for the binding list */
    listbuf = bfind(TEXT21, TRUE, BFINVS);
/*         "Binding list" */
    if ( listbuf == NULL || bclear(listbuf) == FALSE ) {
        mlwrite(TEXT22);

/*          "Can not display binding list" */
        return (FALSE);
    }

    /* let us know this is in progress */
    mlwrite(TEXT23);
/*      "[Building binding list]" */

    /* build the contents of this window, inserting it line by line */
    nptr = &names[0];
    while ( nptr->n_func != NULL ) {

        /* add in the command name */
        XSTRCPY(outseq, nptr->n_name);
        cpos = STRLEN(outseq);

        /* if we are executing an apropos command..... */
        if ( type == FALSE &&
             /* and current string doesn't include the search string */
             strinc(outseq, mstring) == FALSE )
            goto fail;

        /* search down any keys bound to this */
        ktp = &keytab[0];
        while ( ktp->k_type != BINDNUL ) {
            if ( ktp->k_type == BINDFNC &&ktp->k_ptr.fp == nptr->n_func ) {
                /* padd out some spaces */
                while ( cpos < 25 )
                    outseq[cpos++] = ' ';

                /* add in the command sequence */
                cmdstr(ktp->k_code, &outseq[cpos]);

                /* and add it as a line into the buffer */
                if ( addline(listbuf, outseq) != TRUE )
                    return (FALSE);

                cpos = 0;                       /* and clear the line */
            }
            ++ktp;
        }

        /* if no key was bound, we need to dump it anyway */
        if ( cpos > 0 ) {
            outseq[cpos] = 0;
            if ( addline(listbuf, outseq) != TRUE )
                return (FALSE);
        }

fail:   /* and on to the next name */
        ++nptr;
    }

    /* scan all buffers looking for macroes and their bindings */
    first_entry = TRUE;
    bp = bheadp;
    while ( bp ) {

        /* is this buffer a macro? */
        if ( bp->b_bname[0] != '[' )
            goto bfail;

        /* add in the command name */
        XSTRCPY(outseq, bp->b_bname);
        cpos = STRLEN(outseq);

        /* if we are executing an apropos command..... */
        if ( type == FALSE &&
             /* and current string doesn't include the search string */
             strinc(outseq, mstring) == FALSE )
            goto bfail;

        /* search down any keys bound to this macro */
        ktp = &keytab[0];
        while ( ktp->k_type != BINDNUL ) {
            if ( ktp->k_type == BINDBUF &&ktp->k_ptr.buf == bp ) {
                /* padd out some spaces */
                while ( cpos < 25 )
                    outseq[cpos++] = ' ';

                /* add in the command sequence */
                cmdstr(ktp->k_code, &outseq[cpos]);

                /* and add it as a line into the buffer */
                if ( addline(listbuf, outseq) != TRUE )
                    return (FALSE);

                cpos = 0;                       /* and clear the line */
            }
            ++ktp;
        }

        /* add a blank line between the key and macro lists */
        if ( first_entry == TRUE ) {
            if ( addline(listbuf, "") != TRUE )
                return (FALSE);

            first_entry = FALSE;
        }

        /* if no key was bound, we need to dump it anyway */
        if ( cpos > 0 ) {
            outseq[cpos] = 0;
            if ( addline(listbuf, outseq) != TRUE )
                return (FALSE);
        }

bfail:  /* and on to the next buffer */
        bp = bp->b_bufp;
    }

    wpopup(listbuf);
    mlerase();          /* clear the mode line */

    return (TRUE);
}

/* STRINC:
 *
 * Does source include sub?
 */
int PASCAL NEAR strinc P2_(CONST char *, source, CONST char *, sub)
/* source:  String to search in   */
/* sub:     Substring to look for */
{
    CONST char *sp;       /* ptr into source */
    CONST char *nxtsp;    /* next ptr into source */
    CONST char *tp;       /* ptr into substring */

    /* for each character in the source string */
    sp = source;
    while ( *sp ) {
        tp = sub;
        nxtsp = sp;

        /* is the substring here? */
        while ( *tp ) {
            if ( *nxtsp++ != *tp )
                break;
            else
                tp++;
        }

        /* yes, return a success */
        if ( *tp == 0 )
            return (TRUE);

        /* no, onward */
        sp++;
    }

    return (FALSE);
}

/* GETCKEY:
 *
 * Get a command key sequence from the keyboard
 */
unsigned int PASCAL NEAR getckey P1_(int, mflag)
/* mflag: Going for a meta sequence?  */
{
    REGISTER unsigned int c;            /* character fetched */
    char tok[NSTRING];                  /* command incoming */

    /* check to see if we are executing a command line */
    if ( clexec ) {
        macarg(tok);            /* get the next token */

        return ( stock(tok) );
    }

    /* or the normal way */
    if ( mflag )
        c = get_key();
    else
        c = getcmd();

    return (c);
}

/* STARTUP:
 *
 * Execute the startup file
 */
int PASCAL NEAR startup P1_(CONST char *, sfname)
/* sfname:  Name of startup file (null if default)  */
{
    CONST char  *fname  = NULL;   /* resulting file name to execute */
    char        name[NSTRING];    /* name with extention */

    ZEROMEM(name);
    ASRT(NULL != sfname);

    /* look up the startup file */
    if ( *sfname )  {
        /* default the extention */
        XSTRCPY(name, sfname);
        if ( sindex(name, ".") == 0 )
            XSTRCAT(name, ".cmd");

        fname = flook(name, TRUE, TRUE);
    } else          {
        /* DON'T AUTOMATICALLY LOAD STARTUP FROM CURRENT DIRECTORY: */
        fname = flook(pathname[0], TRUE, FALSE);
    }

    /* if it isn't around, don't sweat it */
    if ( fname == NULL )
        return (TRUE);

    /* otherwise, execute the sucker */
    return ( dofile(fname) );
}

/* FLOOK:
 *
 * Look up the existance of a file along the normal or PATH environment
 * variable.
 *
 * LOOKUP ORDER:
 *
 *         if contains path:
 *
 *                 absolute
 *
 *         else
 *
 *                 HOME environment directory
 *                 all directories along PATH environment
 *                 directories in table from EPATH.H
 */
CONST char *PASCAL NEAR flook P3_(CONST char *, fname, int, hflag, int, cflag)
/* fname: Base file name to search for                  */
/* hflag: Look in the HOME environment variable first?  */
/* cflag: Use current directory?                        */
{
    REGISTER char       *home = NULL;   /* path to home directory   */
    REGISTER char       *path = NULL;   /* environmental PATH var   */
    REGISTER CONST char *cp   = NULL;   /* temp pointer CONST str   */
    REGISTER char       *sp   = NULL;   /* pointer into path spec   */
    REGISTER int        i     = 0;      /* index                    */
    static char         fspec[NFILEN];  /* full path spec to search */

    ZEROMEM(fspec);
    ASRT(NULL != fname);

    /* If we have a qualified path.. check only there!  */
    cp = fname;
    while ( *cp ) {
#if     AMIGA
        if ( *cp == ':' || *cp == '/' )
elif    AOSVS | MV_UX
        if ( *cp == ':' )
#elif   VMS
        if ( *cp == ':' || *cp == ']' )
#elif   TOS
        if ( *cp == ':' || *cp == '\\' )
#elif   IS_UNIX()
        if ( *cp == '/' )
#elif   WMCS
        if ( *cp == '_' || *cp == '/' )
#else /* e.g. MSDOS | OS2 | WINNT | WINXP | FINDER  */
        if ( *cp == ':' || *cp == '\\' || *cp == '/' )
#endif
        {
            if ( ffropen(fname) == FIOSUC ) {
                ffclose();

                return (fname);
            } else
                return (NULL);
        }
        ++cp;
    }

#if ENVFUNC

    if ( hflag ) {
# if WMCS
        home = getenv("SYS$HOME");
# else
        home = getenv("HOME");
# endif
        if ( home != NULL ) {
            /* build home dir file spec */
            XSTRCPY(fspec, home);
# if WMCS
            XSTRCAT(fspec, fname);
# else
            XSTRCAT(fspec, DIRSEPSTR);
            XSTRCAT(fspec, fname);
# endif

            /* and try it out */
            if ( ffropen(fspec) == FIOSUC ) {
                ffclose();

                return (fspec);
            }
        }
    }
#endif  /* ENVFUNC */

    /* If cflag is set current directory overides everything except
     * HOME var */
    if ( cflag )  {
        if ( ffropen(fname) == FIOSUC ) {
            ffclose();

            return (fname);
        }
    }

#if ENVFUNC
    /* get the PATH variable */
# if WMCS
    path = getenv("OPT$PATH");
# else
#  if OS2
    path = getenv("DPATH");
#  else
    path = getenv("PATH");
#  endif
# endif
    if ( path != NULL )
        while ( *path ) {
            /* build next possible file spec */
            sp = fspec;
# if     TOS
            while ( *path && (*path != PATHCHR) && (*path != ',') )
# else
            while ( *path && (*path != PATHCHR) )
# endif
                *sp++ = *path++;

            /* add a terminating dir separator if we need it */
            if ( (sp != fspec) && (*(sp-1) != DIRSEPCHAR) )
                *sp++ = DIRSEPCHAR;
            *sp = 0;
            XSTRCAT(fspec, fname);

            /* and try it out */
            if ( ffropen(fspec) == FIOSUC ) {
                ffclose();

                return (fspec);
            }

# if     TOS && MWC
            if ( (*path == PATHCHR) || (*path == ',') )
# else
            if ( *path == PATHCHR )
# endif
                ++path;
        }
#endif  /* ENVFUNC */

    /* look it up via the old table method */
    for ( i = 2; i < NPNAMES; i++ ) {
        if ( !*pathname[i] && !cflag )  {
            continue;
        }
        XSTRCPY(fspec, pathname[i]);
        XSTRCAT(fspec, fname);

        /* and try it out */
        if ( ffropen(fspec) == FIOSUC ) {
            ffclose();

            return (fspec);
        }
    }

    return (NULL);      /* no such luck */
}

/* CMDSTR:
 *
 * Change a key command to a string we can print out. Return the string
 * passed in.
 */
char *PASCAL NEAR cmdstr P2_(int , c, char *, seq)
/* c:   Sequence to translate           */
/* seq: Destination string for sequence */
{
    char *ptr;          /* pointer into current position in sequence */

    ptr = seq;

    /* apply ^X sequence if needed */
    if ( c & CTLX ) {
        *ptr++ = '^';
        *ptr++ = 'X';
    }

    /* apply ALT key sequence if needed */
    if ( c & ALTD ) {
        *ptr++ = 'A';
        *ptr++ = '-';
    }

    /* apply Shifted sequence if needed */
    if ( c & SHFT ) {
        *ptr++ = 'S';
        *ptr++ = '-';
    }

    /* apply MOUS sequence if needed */
    if ( c & MOUS ) {
        *ptr++ = 'M';
        *ptr++ = 'S';
    }

    /* apply meta sequence if needed */
    if ( c & META ) {
        *ptr++ = 'M';
        *ptr++ = '-';
    }

    /* apply SPEC sequence if needed */
    if ( c & SPEC ) {
        *ptr++ = 'F';
        *ptr++ = 'N';
    }

    /* apply control sequence if needed */
    if ( c & CTRF ) {

        /* non normal spaces look like @ */
        if ( ptr == seq && ( (c & 255) == ' ' ) )
            c = '@';

        *ptr++ = '^';
    }

    c = c & 255;        /* strip the prefixes */

    /* and output the final sequence */
    *ptr++ = c;
    *ptr = 0;           /* terminate the string */

    return (seq);
}

/* GETBIND:
 *
 * This function looks a key binding up in the binding table
 */
KEYTAB *getbind P1_(int, c)
/* c: Key to find what is bound to it */
{
    REGISTER KEYTAB *ktp;

    /* scan through the binding table, looking for the key's entry */
    ktp = &keytab[0];
    while ( ktp->k_type != BINDNUL ) {
        if ( ktp->k_code == c )
            return (ktp);

        ++ktp;
    }

    /* no such binding */
    return ( (KEYTAB *)NULL );
}

/* GETFNAME:
 *
 * This function takes a ptr to KEYTAB entry and gets the name
 * associated with it
 */
CONST char *PASCAL NEAR getfname P1_(KEYTAB *, key)
/* key: Key binding to return a name of */
{
    REGISTER ue_fnc_T func;     /* ptr to the requested function */
    REGISTER NBIND *nptr;       /* pointer into the name binding table */
    REGISTER BUFFER *bp;        /* ptr to buffer to test */
    REGISTER BUFFER *kbuf;      /* ptr to requested buffer */

    /* if this isn't a valid key, it has no name */
    if ( key == NULL )
        return (NULL);

    /* skim through the binding table, looking for a match */
    if ( key->k_type == BINDFNC ) {
        func = key->k_ptr.fp;
        nptr = &names[0];
        while ( nptr->n_func != NULL ) {
            if ( nptr->n_func == func )
                return (nptr->n_name);

            ++nptr;
        }

        return (NULL);
    }

    /* skim through the buffer list looking for a match */
    if ( key->k_type == BINDBUF ) {
        kbuf = key->k_ptr.buf;
        bp = bheadp;
        while ( bp ) {
            if ( bp == kbuf )
                return (bp->b_bname);

            bp = bp->b_bufp;
        }

        return (NULL);
    }

    return (NULL);
}

/* FNCMATCH:
 *
 * match fname to a function in the names table and return any match
 * or NULL if none
 */
ue_fnc_T fncmatch P1_(CONST char *, fname)
{
    int nval;

    if ( ( nval = binary(fname, namval, numfunc, NSTRING) ) == -1 )
        return (NULL);
    else
        return (names[nval].n_func);
}

/* NAMVAL:
 */
CONST char *PASCAL NEAR namval P1_(int, index)
/* index: Index of name to fetch out of the name table  */
{
    return (names[index].n_name);
}

/* STOCK:
 *
 * String key name TO Command Key
 *
 * A key binding consists of one or more prefix functions followed by
 * a keystroke.  Allowable prefixes must be in the following order:
 *
 * ^X      preceeding control-X
 * A-      similtaneous ALT key (on PCs mainly)
 * S-      shifted function key
 * MS      mouse generated keystroke
 * M-      Preceding META key
 * FN      function key
 * ^       control key
 *
 * Meta and ^X prefix of lower case letters are converted to upper
 * case.  Real control characters are automatically converted to
 * the ^A form.
*/
unsigned int PASCAL NEAR stock P1_(CONST char *, keyname)
/* keyname: Name of key to translate to Command key form  */
{

    char                    *keynameA   = xstrdup(keyname);
    unsigned char           *keynamePtr = (unsigned char *)keynameA;
    REGISTER unsigned int c;        /* key sequence to return */

    /* parse it up */
    c = 0;

    /* Do ^X prefix */
    if ( *keynamePtr == '^' && *(keynamePtr+1) == 'X' ) {
        if ( *(keynamePtr+2) != 0 ) {      /* Key is not bare ^X */
            c |= CTLX;
            keynamePtr += 2;
        }
    }

    /* and the ALT key prefix */
    if ( *keynamePtr == 'A' && *(keynamePtr+1) == '-' ) {
        c |= ALTD;
        keynamePtr += 2;
    }

    /* and the SHIFTED prefix */
    if ( *keynamePtr == 'S' && *(keynamePtr+1) == '-' ) {
        c |= SHFT;
        keynamePtr += 2;
    }

    /* and the mouse (MOUS) prefix */
    if ( *keynamePtr == 'M' && *(keynamePtr+1) == 'S' ) {
        c |= MOUS;
        keynamePtr += 2;
    }

    /* then the META prefix */
    if ( *keynamePtr == 'M' && *(keynamePtr+1) == '-' ) {
        c |= META;
        keynamePtr += 2;
    }

    /* next the function prefix */
    if ( *keynamePtr == 'F' && *(keynamePtr+1) == 'N' ) {
        c |= SPEC;
        keynamePtr += 2;
    }

    /* a control char?  (NOT Always upper case anymore) */
    if ( *keynamePtr == '^' && *(keynamePtr+1) != 0 ) {
        c |= CTRF;
        ++keynamePtr;
        if ( *keynamePtr == '@' )
            *keynamePtr = ' ';
    }

    /* A literal control character? (Boo, hiss) */
    if ( *keynamePtr < 32 ) {
        c |= CTRF;
        *keynamePtr += '@';
    }

    /* make sure we are not lower case if used with ^X or M- */
    if ( !( c & (MOUS|SPEC|ALTD|SHFT) ) ) { /* If not a special key */
        if ( c & (CTLX|META) ) {    /* If is a prefix */
            uppercase( (unsigned char *)keynamePtr );         /* Then make sure
                                                               * it's upper case
                                                               */
        }
    }

    /* the final sequence... */
    c |= *keynamePtr;
    CLROOM(keynameA);

    return (c);
}

/* TRANSBIND:
 *
 * String key name to binding name....
 */
CONST char *PASCAL NEAR transbind P1_(CONST char *, skey)
/* skey:  Name of key to get binding for  */
{
    CONST char  *bindname;

    bindname = getfname( getbind( stock(skey) ) );
    if ( bindname == NULL )
        bindname = errorm;

    return (bindname);
}

/* EXECKEY:
 *
 * Execute a function bound to a key
 */
int PASCAL NEAR execkey P3_(KEYTAB *, key, int , f, int, n)
/* key:   Key to execute          */
/* f, n:  Arguments to C function */
{
    REGISTER int status;        /* error return */
#if     LOGFLG
    FILE *fp;                           /* file handle for log file */
    char outseq[32];
#endif

    if ( key->k_type == BINDFNC ) {

#if     LOGFLG
        /* append the current command to the log file */
        cmdstr(key->k_code, &outseq);
        fp = fopen("emacs.log", "a");
        fprintf(fp,
                "<[%s] %s %s %d>\n",
                outseq,
                getfname(key),
                f == TRUE ? "TRUE" : "FALSE",
                n);
        fclose(fp);
#endif

        undo_insert(OP_CMND, 1, obj);

        return ( ( *(key->k_ptr.fp) )(f, n) );
    }

    if ( key->k_type == BINDBUF ) {
        while ( n-- ) {
            status = dobuf(key->k_ptr.buf);
            if ( status != TRUE )
                return (status);
        }
    }

    return (TRUE);
}

/* SET_KEY:
 *
 * Set a KEYTAB to the given name of the given type
 */
int set_key P2_(KEYTAB *, key, CONST char *, name)
/* key:   Ptr to key to set           */
/* name:  Name of function or buffer  */
{
    ue_fnc_T        ktemp;        /* temp function pointer to assign  */
    REGISTER BUFFER *kmacro;      /* ptr to buffer of macro to bind
                                   * to key                           */
    char            bufn[NBUFN];  /* buffer to hold macro name        */

    /* are we unbinding it? */
    if ( *name == 0 ) {
        key->k_type = BINDNUL;

        return (TRUE);
    }

    /* bind to a built in function? */
    if ( ( ktemp = fncmatch(name) ) != NULL ) {
        key->k_ptr.fp = ktemp;
        key->k_type = BINDFNC;

        return (TRUE);
    }

    /* is it a procedure/macro? */
    XSTRCPY(bufn, "[");
    XSTRCAT(bufn, name);
    XSTRCAT(bufn, "]");
    if ( ( kmacro=bfind(bufn, FALSE, 0) ) != NULL ) {
        key->k_ptr.buf = kmacro;
        key->k_type = BINDBUF;

        return (TRUE);
    }

    /* not anything we can bind to */
    mlwrite(TEXT16);

/*      "[No such function]" */
    return (FALSE);
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
