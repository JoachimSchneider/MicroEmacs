/*======================================================================
 *      basic.c:        Basic movement functions for
 *                      MicroEMACS
 *                      (C)Copyright 1995 by Daniel Lawrence
 *
 * The routines in this file move the cursor around on the screen. They
 * compute a new value for the cursor, then adjust ".". The display code
 * always updates the cursor location, so only moves between lines, or
 * functions that adjust the top line in the window and invalidate the
 * framing, are hard.
 *====================================================================*/

/*====================================================================*/
#define BASIC_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

/* GOTOBOL:
 *
 * Move the cursor to the beginning of the current line. Trivial.
 */
int PASCAL NEAR gotobol P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    set_w_doto(curwp, 0);

    return (TRUE);
}

/* BACKCHAR:
 *
 * Move the cursor backwards by "n" characters. If "n" is less than zero call
 * "forwchar" to actually do the move. Otherwise compute the new cursor
 * location. Error if you try and move out of the buffer. Set the flag if the
 * line pointer for dot changes.
 */
int PASCAL NEAR backchar P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER LINE   *lp;

    if ( n < 0 )
        return ( forwchar(f, -n) );

    while ( n-- ) {
        if ( get_w_doto(curwp) == 0 ) {
            if ( ( lp=lback(curwp->w_dotp) ) == curbp->b_linep )
                return (FALSE);

            curwp->w_dotp  = lp;
            set_w_doto(curwp, get_lused(lp));
            curwp->w_flag |= WFMOVE;
        } else
            set_w_doto(curwp, get_w_doto(curwp) - 1);
    }
#if     DBCS

    return ( stopback() );

#else

    return (TRUE);

#endif
}

/* GOTOEOL:
 *
 * Move the cursor to the end of the current line. Trivial. No errors.
 */
int PASCAL NEAR gotoeol P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    set_w_doto(curwp, get_lused(curwp->w_dotp));

    return (TRUE);
}

/* FORWCHAR:
 *
 * Move the cursor forwards by "n" characters. If "n" is less than zero call
 * "backchar" to actually do the move. Otherwise compute the new cursor
 * location, and move ".". Error if you try and move off the end of the buffer.
 * Set the flag if the line pointer for dot changes.
 */
int PASCAL NEAR forwchar P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    if ( n < 0 )
        return ( backchar(f, -n) );

    while ( n-- ) {
        if ( get_w_doto(curwp) == get_lused(curwp->w_dotp) ) {
            if ( curwp->w_dotp == curbp->b_linep )
                return (FALSE);

            curwp->w_dotp  = lforw(curwp->w_dotp);
            set_w_doto(curwp, 0);
            curwp->w_flag |= WFMOVE;
        } else
            set_w_doto(curwp, get_w_doto(curwp) + 1);
    }
#if     DBCS

    return ( stopforw() );

#else

    return (TRUE);

#endif
}

/* GOTOLINE:
 *
 * Move to a particular line.
 * argument (n) must be a positive integer for
 * this to actually do anything
 */
int PASCAL NEAR gotoline P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int status;        /* status return */
    char arg[NSTRING];          /* buffer to hold argument */

    /* get an argument if one doesnt exist */
    if ( f == FALSE ) {
        if ( ( status = mlreply(TEXT7, arg, NSTRING) ) != TRUE ) {
/*                                    "Line to GOTO: " */
            mlwrite(TEXT8);

/*                              "[Aborted]" */
            return (status);
        }
        n = asc_int(arg);
    }

    if ( n < 1 )                /* if a bogus argument...then leave */
        return (FALSE);

    /* first, we go to the start of the buffer */
    curwp->w_dotp  = lforw(curbp->b_linep);
    set_w_doto(curwp, 0);

    return ( forwline(f, n-1) );
}

/* GOTOBOB:
 *
 * Goto the beginning of the buffer. Massive adjustment of dot. This is
 * considered to be hard motion; it really isn't if the original value of dot is
 * the same as the new value of dot. Normally bound to "M-<".
 */
int PASCAL NEAR gotobob P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    curwp->w_dotp  = lforw(curbp->b_linep);
    set_w_doto(curwp, 0);
    curwp->w_flag |= WFMOVE;

    return (TRUE);
}

/* GOTOEOB:
 *
 * Move to the end of the buffer. Dot is always put at the end of the file (ZJ).
 * The standard screen code does most of the hard parts of update. Bound to
 * "M->".
 */
int PASCAL NEAR gotoeob P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    curwp->w_dotp  = curbp->b_linep;
    set_w_doto(curwp, 0);
    curwp->w_flag |= WFMOVE;

    return (TRUE);
}

/* FORWLINE:
 *
 * Move forward by full lines. If the number of lines to move is less than zero,
 * call the backward line function to actually do it. The last command controls
 * how the goal column is set. Bound to "C-N". No errors are possible.
 */
int PASCAL NEAR forwline P2_(int, f, int, n)
/* f, n:  Argument flag and argument  */
{
    REGISTER LINE   *dlp;

    if ( n < 0 )
        return ( backline(f, -n) );

    /* if we are on the last line as we start....fail the command */
    if ( curwp->w_dotp == curbp->b_linep )
        return (FALSE);

    /* if the last command was not note a line move, reset the goal column */
    if ( (lastflag&CFCPCN) == 0 )
        curgoal = getccol(FALSE);

    /* flag this command as a line move */
    thisflag |= CFCPCN;

    /* and move the point down */
    dlp = curwp->w_dotp;
    while ( n-- && dlp!=curbp->b_linep )
        dlp = lforw(dlp);

    /* reseting the current position */
    curwp->w_dotp  = dlp;
    set_w_doto(curwp, getgoal(dlp));
    curwp->w_flag |= WFMOVE;
#if     DBCS

    return ( stopback() );

#else

    return (TRUE);

#endif
}

/* BACKLINE:
 *
 * This function is like "forwline", but goes backwards. The scheme is exactly
 * the same. Check for arguments that are less than zero and call your
 * alternate. Figure out the new line and call "movedot" to perform the motion.
 * No errors are possible. Bound to "C-P".
 */
int PASCAL NEAR backline P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    REGISTER LINE   *dlp;

    if ( n < 0 )
        return ( forwline(f, -n) );


    /* if we are on the last line as we start....fail the command */
    if ( lback(curwp->w_dotp) == curbp->b_linep )
        return (FALSE);

    /* if the last command was not note a line move, reset the goal column */
    if ( (lastflag&CFCPCN) == 0 )
        curgoal = getccol(FALSE);

    /* flag this command as a line move */
    thisflag |= CFCPCN;

    /* and move the point up */
    dlp = curwp->w_dotp;
    while ( n-- && lback(dlp)!=curbp->b_linep )
        dlp = lback(dlp);

    /* reseting the current position */
    curwp->w_dotp  = dlp;
    set_w_doto(curwp, getgoal(dlp));
    curwp->w_flag |= WFMOVE;
#if     DBCS

    return ( stopback() );

#else

    return (TRUE);

#endif
}

/* GOTOBOP:
 *
 * go back to the beginning of the current paragraph here we look for a
 * blank line or a character from $paralead to delimit the beginning of
 * a paragraph or $fmtlead to delimit a line before the paragraph
 */
int PASCAL NEAR gotobop P2_(int, f, int, n)
/* f, n:  Default Flag & Numeric argument */
{
    REGISTER int suc;           /* success of last backchar */
    REGISTER int c;             /* current character in scan */
    REGISTER char *sp;          /* ptr into character leadin lists */

    if ( n < 0 )        /* the other way...*/
        return ( gotoeop(f, -n) );

    while ( n-- > 0 ) {         /* for each one asked for */

        /* first scan back until we are in a word */
        suc = backchar(FALSE, 1);
        while ( !inword() && suc )
            suc = backchar(FALSE, 1);
        set_w_doto(curwp, 0);   /* and go to the B-O-Line */

        /* scan back through the text */
        while ( lback(curwp->w_dotp) != curbp->b_linep ) {

            /* at blank line */
            if ( get_lused(curwp->w_dotp) == 0 )
                break;

            /* current line start with member of $paralead? */
            c = lgetc(curwp->w_dotp, 0);
            sp = paralead;
            while ( *sp ) {
                if ( c == *sp )
                    break;
                ++sp;
            }
            if ( c == *sp )
                break;

            /* last line start with member of $fmtlead? */
            c = lgetc(lback(curwp->w_dotp), 0);
            sp = fmtlead;
            while ( *sp ) {
                if ( c == *sp )
                    break;
                ++sp;
            }
            if ( c == *sp )
                break;

            /* back one line... */
            curwp->w_dotp = lback(curwp->w_dotp);
        }

        /* and then forward until we are in a word */
/*      suc = forwchar(FALSE, 1); */
        suc = TRUE;
        while ( suc && !inword() )
            suc = forwchar(FALSE, 1);
    }
    curwp->w_flag |= WFMOVE;            /* force screen update */

    return (TRUE);
}

/* GOTOEOP:
 *
 * go forword to the end of the current paragraph looking for a member
 * of $paralead or $fmtlead or a blank line to delimit the start of the
 * next paragraph
 */
int PASCAL NEAR gotoeop P2_(int, f, int, n)
/* f, n:  Default Flag & Numeric argument */
{
    REGISTER int suc;           /* success of last backchar */
    REGISTER int c;             /* current character in scan */
    REGISTER char *sp;          /* ptr into character leadin lists */

    if ( n < 0 )        /* the other way...*/
        return ( gotobop(f, -n) );

    while ( n-- > 0 ) {         /* for each one asked for */

        /* first scan forward until we are in a word */
        suc = forwchar(FALSE, 1);
        while ( !inword() && suc )
            suc = forwchar(FALSE, 1);

        /* and go to the B-O-Line */
        set_w_doto(curwp, 0);

        /* of next line if not at EOF */
        if ( suc )
            curwp->w_dotp = lforw(curwp->w_dotp);

        /* scan forward */
        while ( curwp->w_dotp != curbp->b_linep ) {

            /* at blank line */
            if ( get_lused(curwp->w_dotp) == 0 )
                break;

            /* current line start with member of $paralead? */
            c = lgetc(curwp->w_dotp, 0);
            sp = paralead;
            while ( *sp ) {
                if ( c == *sp )
                    break;
                ++sp;
            }
            if ( c == *sp )
                break;

            /* current line start with member of $fmtlead? */
            c = lgetc(curwp->w_dotp, 0);
            sp = fmtlead;
            while ( *sp ) {
                if ( c == *sp )
                    break;
                ++sp;
            }
            if ( c == *sp )
                break;

            /* forward one line... */
            curwp->w_dotp = lforw(curwp->w_dotp);
        }

        /* and then backward until we are in a word */
        suc = backchar(FALSE, 1);
        while ( suc && !inword() ) {
            suc = backchar(FALSE, 1);
        }
        set_w_doto(curwp, get_lused(curwp->w_dotp));  /* and to the EOL */
    }
    curwp->w_flag |= WFMOVE;            /* force screen update */

    return (TRUE);
}

/* GETGOAL:
 *
 * This routine, given a pointer to a LINE, and the current cursor goal column,
 * return the best choice for the offset. The offset is returned. Used by "C-N"
 * and "C-P".
 */
int PASCAL NEAR getgoal P1_(LINE *, dlp)
{
    REGISTER int c;
    REGISTER int col;
    REGISTER int newcol;
    REGISTER int dbo;

    col = 0;
    dbo = 0;
    while ( dbo != get_lused(dlp) ) {
        c = lgetc(dlp, dbo);
        newcol = col;
        if ( c == '\t' && tabsize > 0 )
            newcol += -(newcol % tabsize) + (tabsize - 1);
        else if ( c<0x20 || c==0x7F )
            ++newcol;
        ++newcol;
        if ( newcol > curgoal )
            break;
        col = newcol;
        ++dbo;
    }

    return (dbo);
}

/* FORWPAGE:
 *
 * Scroll forward by a specified number of lines, or by a full page if no
 * argument. Bound to "C-V". The overlap in the arithmetic on the window size is
 * overlap between screens. This defaults to overlap value in ITS EMACS. Because
 * this zaps the top line in the window, we have to do a hard update.
 */
int PASCAL NEAR forwpage P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER LINE   *lp;

    /*
     * Calculate the lines to scroll, taking into account the $overlap count and
     * whether the modeline is on or not.
     */
    if ( f == FALSE ) {
        n = curwp->w_ntrows - overlap + !modeflag;                /* Default
                                                                   * scroll.
                                                                   *      */
        if ( n <= 0 )                           /* Forget the overlap   */
            n = 1;                              /* if tiny window.      */
    } else if ( n < 0 )
        return ( backpage(f, -n) );

    lp = curwp->w_linep;
    while ( n-- && lp!=curbp->b_linep )
        lp = lforw(lp);
    curwp->w_linep = lp;
    curwp->w_dotp  = lp;
    set_w_doto(curwp, 0);
    curwp->w_flag |= WFHARD;

    return (TRUE);
}

/* BACKPAGE:
 *
 * This command is like "forwpage", but it goes backwards. overlap, like above,
 * is the overlap between the two windows. The value is from the ITS EMACS
 * manual. Bound to "M-V". We do a hard update for exactly the same reason.
 */
int PASCAL NEAR backpage P2_(int, f, int, n)
{
    REGISTER LINE   *lp;

    /*
     * Calculate the lines to scroll, taking into account the $overlap count and
     * whether the modeline is on or not.
     */
    if ( f == FALSE ) {
        n = curwp->w_ntrows - overlap + !modeflag;                /* Default
                                                                   * scroll.
                                                                   *      */
        if ( n <= 0 )                           /* Don't blow up if the */
            n = 1;                              /* window is tiny.      */
    } else if ( n < 0 )
        return ( forwpage(f, -n) );

    lp = curwp->w_linep;
    while ( n-- && lback(lp)!=curbp->b_linep )
        lp = lback(lp);
    curwp->w_linep = lp;
    curwp->w_dotp  = lp;
    set_w_doto(curwp, 0);
    curwp->w_flag |= WFHARD;

    return (TRUE);
}

/* SETMARK:
 *
 * Set the mark in the current window to the value of "." in the window. No
 * errors are possible. Bound to "M-.".
 */
int PASCAL NEAR setmark P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    /* make sure it is in range */
    if ( f == FALSE )
        n = 0;
    n %= NMARKS;

    curwp->w_markp[n] = curwp->w_dotp;
    curwp->w_marko[n] = get_w_doto(curwp);
    mlwrite(TEXT9, n);

/*              "[Mark %d set]" */
    return (TRUE);
}

/* REMMARK:
 *
 * Remove the mark in the current window. Bound to ^X <space>
 */
int PASCAL NEAR remmark P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    /* make sure it is in range */
    if ( f == FALSE )
        n = 0;
    n %= NMARKS;

    curwp->w_markp[n] = NULL;
    curwp->w_marko[n] = 0;
    mlwrite(TEXT10, n);

/*              "[Mark %d removed]" */
    return (TRUE);
}

/* SWAPMARK:
 *
 * Swap the values of "." and "mark" in the current window. This is pretty easy,
 * bacause all of the hard work gets done by the standard routine that moves the
 * mark about. The only possible error is "no mark". Bound to
 * "C-X C-X".
 */
int PASCAL NEAR swapmark P2_(int, f, int, n)
/* f, n:  Argument flag and num */
{
    REGISTER LINE   *odotp;
    REGISTER int odoto;

    /* make sure it is in range */
    if ( f == FALSE )
        n = 0;
    n %= NMARKS;

    if ( curwp->w_markp[n] == NULL ) {
        mlwrite(TEXT11, n);

/*                      "No mark %d in this window" */
        return (FALSE);
    }
    odotp = curwp->w_dotp;
    odoto = get_w_doto(curwp);
    curwp->w_dotp  = curwp->w_markp[n];
    set_w_doto(curwp, curwp->w_marko[n]);
    curwp->w_markp[n] = odotp;
    curwp->w_marko[n] = odoto;
    curwp->w_flag |= WFMOVE;

    return (TRUE);
}

/* GOTOMARK:
 *
 * Goto a mark in the current window. This is pretty easy, bacause all of the
 * hard work gets done by the standard routine that moves the mark about. The
 * only possible error is "no mark". Bound to "M-^G".
 */
int PASCAL NEAR gotomark P2_(int, f, int, n)
/* f, n:  Default and numeric args  */
{
    /* make sure it is in range */
    if ( f == FALSE )
        n = 0;
    n %= NMARKS;

    if ( curwp->w_markp[n] == NULL ) {
        mlwrite(TEXT11, n);

/*                      "No mark %d in this window" */
        return (FALSE);
    }
    curwp->w_dotp  = curwp->w_markp[n];
    set_w_doto(curwp, curwp->w_marko[n]);
    curwp->w_flag |= WFMOVE;

    return (TRUE);
}

#if     DBCS

/* STOPFORW:
 *
 * Advance a char if we are on the second byte of a DBCS character
 */
int PASCAL NEAR stopforw P0_()
{
    /* don't stop on the second byte of a 2 byte character */
    if ( get_w_doto(curwp) > 0 &&
         is2byte(ltext(curwp->w_dotp),
                 ltext(curwp->w_dotp) + get_w_doto(curwp) - 1) )
        return ( forwchar(TRUE, 1) );

    return (TRUE);
}

/* STOPBACK:
 *
 * Retreat a char if we are on the second byte of a DBCS character
 */
int PASCAL NEAR stopback P0_()
{
    /* don't stop on the second byte of a 2 byte character */
    if ( get_w_doto(curwp) > 0 &&
         is2byte(ltext(curwp->w_dotp),
                 ltext(curwp->w_dotp) + get_w_doto(curwp) - 1) )
        return ( backchar(TRUE, 1) );

    return (TRUE);
}

#endif  /* DBCS */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
