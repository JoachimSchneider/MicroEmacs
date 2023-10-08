/*======================================================================
 * This file contains the command processing functions for a number of random
 * commands. There is no functional grouping here, for sure.
 *====================================================================*/

/*====================================================================*/
#define RANDOM_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "estruct.h"
#if CYGWIN
# include <unistd.h>
#endif
#include "eproto.h"
#include "edef.h"
#include "elang.h"


/* SHOWCPOS:
 *
 * Display the current position of the cursor, in origin 1 X-Y coordinates, the
 * character that is under the cursor (in hex), and the fraction of the text
 * that is before the cursor. The displayed column is not the current column,
 * but the column that would be used on an infinite width display. Normally this
 * is bound to "C-X =".
 */
int PASCAL NEAR showcpos P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER LINE *lp       = NULL;   /* current line */
    REGISTER long numchars  = 0;      /* # of chars in file */
    REGISTER long numlines  = 0;      /* # of lines in file */
    REGISTER long predchars = 0;      /* # chars preceding point */
    REGISTER long predlines = 0;      /* # lines preceding point */
    REGISTER int  curchar   = 0;      /* character under cursor */
    int           ratio     = 0;
    int           col       = 0;
    int           savepos   = 0;      /* temp save for current offset */
    int           ecol      = 0;      /* column pos/end of current line */

    /* starting at the beginning of the buffer */
    lp = lforw(curbp->b_linep);
    curchar = '\r';

    /* start counting chars and lines */
    numchars = 0;
    numlines = 0L;
    while ( lp != curbp->b_linep ) {

        /* if we are on the current line, record it */
        if ( lp == curwp->w_dotp ) {
            predlines = numlines;
            predchars = numchars + get_w_doto(curwp);
            if ( (get_w_doto(curwp)) == get_lused(lp) )
                curchar = '\r';
            else
                curchar = lgetc(lp, get_w_doto(curwp));
        }

        /* on to the next line */
        ++numlines;
        numchars += get_lused(lp) + 1;
        lp = lforw(lp);
    }

    /* if at end of file, record it */
    if ( curwp->w_dotp == curbp->b_linep ) {
        predlines = numlines;
        predchars = numchars;
    }

    /* Get real column and end-of-line column. */
    col = getccol(FALSE);
    savepos = get_w_doto(curwp);
    set_w_doto(curwp, get_lused(curwp->w_dotp));
    ecol = getccol(FALSE);
    set_w_doto(curwp, savepos);

    ratio = 0;                          /* Ratio before dot. */
    if ( numchars != 0 )
        ratio = (100L * predchars) / numchars;

    /* summarize and report the info */
#if     DBCS
    if ( is2byte(ltext(curwp->w_dotp), ltext(curwp->w_dotp) + get_w_doto(curwp)) ) {
        mlwrite( TEXT220,
/*      "Line %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x%x" */
                 predlines+1, numlines+1, col, ecol, predchars, numchars, ratio,
                 (unsigned char)curchar,
                 (unsigned char)( lgetc(curwp->w_dotp, get_w_doto(curwp)+1) ) );

        return (TRUE);
    }
#endif
    mlwrite(TEXT60,
/*      "Line %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x" */
            predlines + 1,
            numlines + 1,
            col,
            ecol,
            predchars,
            numchars,
            ratio,
            curchar);

    return (TRUE);
}

/* GETLINENUM:
 *
 * Get the a line number
 */
long PASCAL NEAR getlinenum P2_(BUFFER *, bp, LINE *, sline)
/* bp:    Buffer to get current line from */
/* sline: Line to search for              */
{
    REGISTER LINE   *lp;        /* current line */
    REGISTER long numlines;             /* # of lines before point */

    /* starting at the beginning of the buffer */
    lp = lforw(bp->b_linep);

    /* start counting lines */
    numlines = 0L;
    while ( lp != bp->b_linep ) {
        /* if we are on the current line, record it */
        if ( lp == sline )
            break;
        ++numlines;
        lp = lforw(lp);
    }

    /* and return the resulting count */
    return (numlines + 1L);
}


/* GETCCOL:
 *
 * Return current column.  Stop at first non-blank given TRUE argument.
 */
int PASCAL NEAR getccol P1_(int, bflg)
{
    int           i     = 0;
    int           col   = 0;
    unsigned char c     = '\0';
    int           doto  = 0;

    doto  = get_w_doto(curwp);
#if REPAIR_CODE_LINE
    REPAIR(doto <= get_lused(curwp->w_dotp), doto = get_lused(curwp->w_dotp));
#else
    ASRT  (doto <= get_lused(curwp->w_dotp));
#endif
    for ( i = 0; i < doto; ++i )  {
        c = lgetc(curwp->w_dotp, i) & 0xFF;
        if ( c != ' ' && c != '\t' && bflg )
            break;
        if ( c == '\t' && tabsize > 0 )
            col += -(col % tabsize) + (tabsize - 1);
        else {
            if ( disphigh && c > 0x7f ) {
                col += 2;
                c -= 0x80;
            }
            if ( c < 0x20 || c == 0x7f )
                ++col;
        }
        ++col;
    }

    return (col);
}

/* FINDCOL:
 *
 * Return display column in line at char position
 */
int PASCAL NEAR findcol P2_(LINE *, lp, int, pos)
/* lp:  Line to scan      */
/* pos: Character offset  */
{
    REGISTER int c, i, col;

    col = 0;
    for ( i = 0; i < pos; ++i ) {
        c = lgetc(lp, i);
        if ( c == '\t' && tabsize > 0 )
            col += -(col % tabsize) + (tabsize - 1);
        else {
            if ( disphigh && c > 0x7f ) {
                col += 2;
                c -= 0x80;
            }
            if ( c < 0x20 || c == 0x7F )
                ++col;
        }
        ++col;
    }

    return (col);
}

/* SETCCOL:
 *
 * Set current column.
 */
int PASCAL NEAR setccol P1_(int, pos)
/* pos: Position to set cursor  */
{
    REGISTER int c;             /* character being scanned */
    REGISTER int i;             /* index into current line */
    REGISTER int col;           /* current cursor column   */
    REGISTER int llen;          /* length of line in bytes */

    col = 0;
    llen = get_lused(curwp->w_dotp);

    /* scan the line until we are at or past the target column */
    for ( i = 0; i < llen; ++i ) {
        /* upon reaching the target, drop out */
        if ( col >= pos )
            break;

        /* advance one character */
        c = lgetc(curwp->w_dotp, i);
        if ( c == '\t' && tabsize > 0 )
            col += -(col % tabsize) + (tabsize - 1);
        else {
            if ( disphigh && c > 0x7f ) {
                col += 2;
                c -= 0x80;
            }
            if ( c < 0x20 || c == 0x7F )
                ++col;
        }
        ++col;
    }

    /* set us at the new position */
    set_w_doto(curwp, i);

    /* and tell whether we made it */
    return (col >= pos);
}

/* TWIDDLE:
 *
 * Twiddle the two characters on either side of dot. If dot is at the end of the
 * line twiddle the two characters before it. Return with an error if dot is at
 * the beginning of line; it seems to be a bit pointless to make this work. This
 * fixes up a very common typo with a single stroke. Normally bound to "C-T".
 * This always works within a line, so "WFEDIT" is good enough. Backward
 * compatibility forces the save/restore of the cursor position to keep this
 * working as it always has.
 */
int PASCAL NEAR twiddle P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER LINE *dotp;                /* shorthand to current line pointer */
    int saved_doto;                     /* restore the cursor afterwards */
    REGISTER int cl;                    /* characters to swap! */
    REGISTER int cr;

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    /* get the current position */
    saved_doto = get_w_doto(curwp);
    dotp = curwp->w_dotp;

    /* get the 2 chars to swap */
    if ( get_w_doto(curwp) == get_lused(dotp) ) {
        if ( get_w_doto(curwp) <= 0 ) {
            set_w_doto(curwp, saved_doto);

            return (FALSE);
        } else {
            set_w_doto(curwp, get_w_doto(curwp) - 1);
        }
    }
    cr = lgetc(dotp, get_w_doto(curwp));
    if ( get_w_doto(curwp) <= 0 ) {
        set_w_doto(curwp, saved_doto);

        return (FALSE);
    } else {
        set_w_doto(curwp, get_w_doto(curwp) - 1);
    }
    cl = lgetc(dotp, get_w_doto(curwp));

    /* swap the characters */
    obj.obj_char = cl;
    lputc(dotp, get_w_doto(curwp), cr);
    undo_insert(OP_REPC, 1L, obj);
    set_w_doto(curwp, get_w_doto(curwp) + 1);
    obj.obj_char = cr;
    lputc(dotp, get_w_doto(curwp), cl);
    undo_insert(OP_REPC, 1L, obj);

    /* restore the cursor position */
    set_w_doto(curwp, saved_doto);

    /* flag the change */
    lchange(WFEDIT);

    return (TRUE);
}

/* QUOTE:
 *
 * Quote the next character, and insert it into the buffer. All the characters
 * are taken literally, including the newline, which does not then have its line
 * splitting meaning. The character is always read, even if it is inserted 0
 * times, for regularity. Bound to "C-Q". If a mouse action or function key is
 * pressed, its symbolic MicroEMACS name gets inserted!
 */
int PASCAL NEAR quote P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int ec;            /* current extended key fetched     */
    REGISTER int c;             /* current ascii key fetched        */
    REGISTER int status;        /* return value to hold from linstr */
    char key_name[10];          /* name of a keystroke for quoting  */

    if ( curbp->b_mode & MDVIEW )   /* don't allow this command if  */
        return ( rdonly() );        /* we are in read only mode     */

    ec = get_key();

    /* fail this on a negative argument */
    if ( n < 0 )
        return (FALSE);

    /* nothing to insert . . . blow it off */
    if ( n == 0 )
        return (TRUE);

    /* if this is a mouse event or function key, put its name in */
    if ( (ec & MOUS) || (ec & SPEC) ) {
        cmdstr(ec, key_name);
        while ( n-- ) {
            status = linstr(key_name);
            if ( status != TRUE )
                return (status);
        }

        return (TRUE);
    }

    /* otherwise, just insert the raw character */
    c = ectoc(ec);

    return ( linsert(n, c) );
}

/* TAB:
 *
 * Set tab size if given non-default argument (n <> 1).  Otherwise, insert a tab
 * into file.  If given argument, n, of zero, change to hard tabs. If n > 1,
 * simulate tab stop every n-characters using spaces. This has to be done in
 * this slightly funny way because the tab (in ASCII) has been turned into "C-I"
 * (in 10 bit code) already. Bound to "C-I".
 */
int PASCAL NEAR tab P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    if ( n < 0 )
        return (FALSE);

    if ( n == 0 || n > 1 ) {
        stabsize = n;

        return (TRUE);
    }
    if ( !stabsize )
        return ( linsert(1, '\t') );

    return ( linsert(stabsize - (getccol(FALSE) % stabsize), ' ') );
}

/* DETAB:
 *
 * Change tabs to spaces
 */
int PASCAL NEAR detab P2_(int, f, int, n)
/* f, n:  Default flag and numeric repeat count */
{
    REGISTER int inc;           /* increment to next line [sgn(n)] */

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    if ( tabsize == 0 )
        return FALSE;

    if ( f == FALSE )
        n = reglines();

    /* loop thru detabbing n lines */
    inc = ( (n > 0) ? 1 : -1 );
    while ( n ) {
        set_w_doto(curwp, 0);           /* start at the beginning */

        /* detab the entire current line */
        while ( get_w_doto(curwp) < get_lused(curwp->w_dotp) ) {
            /* if we have a tab */
            if ( lgetc(curwp->w_dotp, get_w_doto(curwp)) == '\t' ) {
                ldelete(1L, FALSE);
/*              insspace(TRUE, 8 - (get_w_doto(curwp) & 7));*/
                insspace( TRUE, tabsize - (get_w_doto(curwp) % tabsize) );
            }
            forwchar(FALSE, 1);
        }

        /* advance/or back to the next line */
        forwline(TRUE, inc);
        n -= inc;
    }
    set_w_doto(curwp, 0);       /* to the begining of the line */
    thisflag &= ~CFCPCN;        /* flag that this resets the goal column */
    lchange(WFEDIT);            /* yes, we have made at least an edit */

    return (TRUE);
}

/* ENTAB:
 *
 * Change spaces to tabs where posible
 */
int PASCAL NEAR entab P2_(int, f, int, n)
/* f, n:  Default flag and numeric repeat count */
{
    REGISTER int inc;           /* increment to next line [sgn(n)] */
    REGISTER int fspace;        /* pointer to first space if in a run */
    REGISTER int ccol;          /* current cursor column */
    REGISTER char cchar;        /* current character */

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    if ( tabsize == 0 )
        return FALSE;

    if ( f == FALSE )
        n = reglines();

    /* loop thru entabbing n lines */
    inc = ( (n > 0) ? 1 : -1 );
    while ( n ) {
        /* entab the entire current line */

        ccol = set_w_doto(curwp, 0);    /* start at the beginning */
        fspace = -1;

        while ( get_w_doto(curwp) < get_lused(curwp->w_dotp) ) {
            /* see if it is time to compress */
            if ( (fspace >= 0) && (nextab(fspace) <= ccol) )  {
                if ( ccol - fspace < 2 )  {
                    fspace = -1;
                } else {
                    backchar(TRUE, ccol - fspace);
                    ldelete( (long) (ccol - fspace), FALSE );
                    linsert(1, '\t');
                    fspace = -1;
                }
            }

            /* get the current character */
            cchar = lgetc(curwp->w_dotp, get_w_doto(curwp));

            switch ( cchar ) {
            case '\t':                  /* a tab...count em up (no break here)
                                         *  */
                ldelete(1L, FALSE);
                insspace( TRUE, tabsize - (ccol % tabsize) );

            case ' ':                   /* a space...compress? */
                if ( fspace == -1 )
                    fspace = ccol;
                break;

            default:                    /* any other char...just count */
                fspace = -1;
                break;
            }
            ccol++;
            forwchar(FALSE, 1);
        }

        /* advance/or back to the next line */
        forwline(TRUE, inc);
        n -= inc;
        set_w_doto(curwp, 0);   /* start at the beginning */
    }
    set_w_doto(curwp, 0);       /* to the begining of the line */
    thisflag &= ~CFCPCN;        /* flag that this resets the goal column */
    lchange(WFEDIT);            /* yes, we have made at least an edit */

    return (TRUE);
}

/* TRIM:
 *
 * Trim trailing whitespace from the point to eol with no arguments, it
 * trims the current region
 */
int PASCAL NEAR trim P2_(int, f, int, n)
/* f, n:  Default flag and numeric repeat count */
{
    REGISTER LINE *lp;          /* current line pointer */
    REGISTER int offset;        /* original line offset position */
    REGISTER int length;        /* current length */
    REGISTER int inc;           /* increment to next line [sgn(n)] */

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    if ( f == FALSE )
        n = reglines();

    /* loop thru trimming n lines */
    inc = ( (n > 0) ? 1 : -1 );
    while ( n ) {
        lp = curwp->w_dotp;             /* find current line text */
        offset = get_w_doto(curwp);     /* save original offset */
        length = get_lused(lp);         /* find current length */

        /* trim the current line */
        while ( length > offset ) {
            if ( lgetc(lp, length-1) != ' ' &&lgetc(lp, length-1) != '\t' )
                break;
            length--;
        }
        set_lused(lp, length);

        /* advance/or back to the next line */
        forwline(TRUE, inc);
        n -= inc;
    }
    lchange(WFEDIT);
    thisflag &= ~CFCPCN;        /* flag that this resets the goal column */

    return (TRUE);
}

/* OPENLINE:
 *
 * Open up some blank space. The basic plan is to insert a bunch of newlines,
 * and then back up over them. Everything is done by the subcommand procerssors.
 * They even handle the looping. Normally this is bound to "C-O".
 */
int PASCAL NEAR openline P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int i;
    REGISTER int s;

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    if ( n < 0 )
        return (FALSE);

    if ( n == 0 )
        return (TRUE);

    i = n;                              /* Insert newlines. */
    do {
        s = lnewline();
    } while (s == TRUE && --i);
    if ( s == TRUE )            /* Then back up overtop */
        s = backchar(f, n);             /* of them all.     */

    return (s);
}

/* NEWLINE:
 *
 * Insert a newline. Bound to "C-M". If we are in CMODE, do automatic
 * indentation as specified.
 */
int PASCAL NEAR newline P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int s;

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    if ( n < 0 )
        return (FALSE);

    /* if we are in C mode and this is a default <NL> */
    if ( n == 1 && (curbp->b_mode & MDCMOD) &&curwp->w_dotp != curbp->b_linep )
        return ( cinsert() );

    /*
     * If a newline was typed, fill column is defined, the argument is non-
     * negative, wrap mode is enabled, and we are now past fill column, and we
     * are not read-only, perform word wrap.
     */
    if ( (curwp->w_bufp->b_mode & MDWRAP) && fillcol > 0 &&
         getccol(FALSE) > fillcol &&(curwp->w_bufp->b_mode & MDVIEW) == FALSE )
        execkey(&wraphook, FALSE, 1);

    /* insert some lines */
    while ( n-- ) {
        if ( ( s = lnewline() ) != TRUE )
            return (s);
    }

    return (TRUE);
}

/* CINSERT:
 *
 * Insert a newline and indentation for C
 */
int PASCAL NEAR cinsert P0_()
{
    REGISTER char *cptr;        /* string pointer into text to copy */
    REGISTER int i;             /* index into line to copy indent from */
    REGISTER int llen;          /* length of line to copy indent from */
    REGISTER int bracef;        /* was there a brace at the end of line? */
    REGISTER LINE *lp;          /* current line pointer */
    REGISTER int offset;
    char ichar[NSTRING];        /* buffer to hold indent of last line */

    /* trim the whitespace before the point */
    lp = curwp->w_dotp;
    offset = get_w_doto(curwp);
    while ( offset > 0 &&
            (lgetc(lp, offset - 1) == ' ' ||lgetc(lp, offset - 1) == '\t') ) {
        backdel(FALSE, 1);
        offset--;
    }

    /* check for a brace */
    bracef = (offset > 0 && lgetc(lp, offset - 1) == '{');

    /* put in the newline */
    if ( lnewline() == FALSE )
        return (FALSE);

    /* if the new line is not blank... don't indent it! */
    lp = curwp->w_dotp;
    if ( get_lused(lp) != 0 )
        return (TRUE);

    /* hunt for the last non-blank line to get indentation from */
    while ( get_lused(lp) == 0 && lp != curbp->b_linep )
        lp = lback(lp);

    /* grab a pointer to text to copy indentation from */
    cptr = ltext(lp);
    llen = get_lused(lp);

    /* save the indent of the last non blank line */
    i = 0;
    while ( (i < llen) && (cptr[i] == ' ' || cptr[i] == '\t')&&
            (i < NSTRING - 1) ) {
        ichar[i] = cptr[i];
        ++i;
    }
    ichar[i] = 0;               /* terminate it */

    /* insert this saved indentation */
    linstr(ichar);

    /* and one more tab for a brace */
    if ( bracef )
        tab(FALSE, 1);

    return (TRUE);
}

/* INSBRACE:
 *
 * Insert a brace into the text here...we are in CMODE
 */
int PASCAL NEAR insbrace P2_(int, n, int, c)
/* n: Repeat count                        */
/* c: Brace to insert (always } for now)  */
{
    REGISTER int ch;            /* last character before input */
    REGISTER int oc;            /* caractere oppose a c */
    REGISTER int i, count;
    REGISTER int target;        /* column brace should go after */
    REGISTER LINE *oldlp;
    REGISTER int oldoff;

    /* if we aren't at the beginning of the line... */
    if ( get_w_doto(curwp) != 0 )

        /* scan to see if all space before this is white space */
        for ( i = get_w_doto(curwp) - 1; i >= 0; --i ) {
            ch = lgetc(curwp->w_dotp, i);
            if ( ch != ' ' && ch != '\t' )
                return ( linsert(n, c) );
        }

    /* chercher le caractere oppose correspondant */
    switch ( c ) {
    case '}':
        oc = '{';
        break;

    case ']':
        oc = '[';
        break;

    case ')':
        oc = '(';
        break;

    default:
        return (FALSE);
    }

    oldlp = curwp->w_dotp;
    oldoff = get_w_doto(curwp);

    count = 1;
    backchar(FALSE, 1);

    while ( count > 0 ) {
        if ( get_w_doto(curwp) == get_lused(curwp->w_dotp) )
            ch = '\r';
        else
            ch = lgetc(curwp->w_dotp, get_w_doto(curwp));

        if ( ch == c )
            ++count;
        if ( ch == oc )
            --count;

        backchar(FALSE, 1);
        if ( boundry(curwp->w_dotp, get_w_doto(curwp), REVERSE) )
            break;
    }

    if ( count != 0 ) {                         /* no match */
        curwp->w_dotp = oldlp;
        set_w_doto(curwp, oldoff);

        return ( linsert(n, c) );
    }

    set_w_doto(curwp, 0);   /* debut de ligne */
    /* aller au debut de la ligne apres la tabulation */
    while ( ( ch = lgetc(curwp->w_dotp, get_w_doto(curwp)) ) == ' ' || ch == '\t' )
        forwchar(FALSE, 1);

    /* delete back first */
    target = getccol(FALSE);            /* c'est l'indent que l'on doit avoir */
    curwp->w_dotp = oldlp;
    set_w_doto(curwp, oldoff);

    while ( target != getccol(FALSE) ) {
        if ( target < getccol(FALSE) )          /* on doit detruire des
                                                 * caracteres */
            while ( getccol(FALSE) > target )
                backdel(FALSE, 1);

        else {                          /* on doit en inserer */
            if ( tabsize > 0 )
                while ( target - getccol(FALSE) >= tabsize )
                    linsert(1, '\t');

            linsert(target - getccol(FALSE), ' ');
        }
    }

    /* and insert the required brace(s) */
    return ( linsert(n, c) );
}

/* INSPOUND:
 *
 * Insert a # into the text here...we are in CMODE
 */
int PASCAL NEAR inspound P0_()
{
    REGISTER int ch;            /* last character before input */
    REGISTER int i;

    /* if we are at the beginning of the line, no go */
    if ( get_w_doto(curwp) == 0 )
        return ( linsert(1, '#') );

    /* scan to see if all space before this is white space */
    for ( i = get_w_doto(curwp) - 1; i >= 0; --i ) {
        ch = lgetc(curwp->w_dotp, i);
        if ( ch != ' ' && ch != '\t' )
            return ( linsert(1, '#') );
    }

    /* delete back first */
    while ( getccol(FALSE) >= 1 )
        backdel(FALSE, 1);

    /* and insert the required pound */
    return ( linsert(1, '#') );
}

/* DEBLANK:
 *
 * Delete blank lines around dot. What this command does depends if dot is
 * sitting on a blank line. If dot is sitting on a blank line, this command
 * deletes all the blank lines above and below the current line. If it is
 * sitting on a non blank line then it deletes all of the blank lines after the
 * line. Normally this command is bound to "C-X C-O". Any argument is ignored.
 */
int PASCAL NEAR deblank P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER LINE   *lp1;
    REGISTER LINE   *lp2;
    long nld;

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    lp1 = curwp->w_dotp;
    while ( get_lused(lp1) == 0 && ( lp2 = lback(lp1) ) != curbp->b_linep )
        lp1 = lp2;
    lp2 = lp1;
    nld = 0;
    while ( ( lp2 = lforw(lp2) ) != curbp->b_linep && get_lused(lp2) == 0 )
        ++nld;
    if ( nld == 0 )
        return (TRUE);

    curwp->w_dotp = lforw(lp1);
    set_w_doto(curwp, 0);

    return ( ldelete(nld, FALSE) );
}

/* INDENT:
 *
 * Insert a newline, then enough tabs and spaces to duplicate the indentation of
 * the previous line. Tabs are every tabsize characters. Quite simple. Figure
 * out the indentation of the current line. Insert a newline by calling the
 * standard routine. Insert the indentation by inserting the right number of
 * tabs and spaces. Return TRUE if all ok. Return FALSE if one of the subcomands
 * failed. Normally bound to "C-J".
 */

int PASCAL NEAR indent P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int nicol;
    REGISTER int c;
    REGISTER int i;

    if ( curbp->b_mode & MDVIEW )       /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    if ( n < 0 )
        return (FALSE);

    while ( n-- ) {
        nicol = 0;
        for ( i = 0; i < get_lused(curwp->w_dotp); ++i ) {
            c = lgetc(curwp->w_dotp, i);
            if ( c != ' ' && c != '\t' )  {
                break;
            }
            if ( c == '\t' )  {
                if ( tabsize > 0 )  {
                    nicol += -(nicol % tabsize) + (tabsize - 1);
                } else {
                    break;
                }
            }

            ++nicol;
        }
        if ( lnewline() == FALSE )
            return FALSE;

        if ( tabsize == 0 ) {
            if ( linsert(nicol, ' ') == FALSE )
                return (FALSE);
        } else if ( ( (i = nicol / tabsize) != 0 &&
                      linsert(i, '\t') == FALSE )||
                    ( (i = nicol % tabsize) != 0 && linsert(i, ' ') == FALSE ) )
            return (FALSE);
    }

    return (TRUE);
}

/* FORWDEL:
 *
 * Delete forward. This is real easy, because the basic delete routine does all
 * of the work. Watches for negative arguments, and does the right thing. If any
 * argument is present, it kills rather than deletes, to prevent loss of text if
 * typed with a big argument. Normally bound to "C-D".
 */
int PASCAL NEAR forwdel P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    /* Don't allow this in read-only mode */
    if ( curbp->b_mode & MDVIEW )
        return ( rdonly() );

    /* with a negative argument, this is a backwards delete */
    if ( n < 0 )
        return ( backdel(f, -n) );

    /* with an argument, flag this to go to the kill buffer! */
    if ( f != FALSE ) {
        if ( (lastflag & CFKILL) == 0 )
            next_kill();
        thisflag |= CFKILL;
    }

    return ( ldelete( (long)n, f ) );
}

/* BACKDEL:
 *
 * Delete backwards. This is quite easy too, because it's all done with other
 * functions. Just move the cursor back, and delete forwards. Like delete
 * forward, this actually does a kill if presented with an argument. Bound to
 * both "RUBOUT" and "C-H".
 */
int PASCAL NEAR backdel P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int status;

    /* Don't do this command in read-only mode */
    if ( curbp->b_mode & MDVIEW )
        return ( rdonly() );

    /* with a negative argument, this becomes a delete forward */
    if ( n < 0 )
        return ( forwdel(f, -n) );

    /* with an argument, flag this to go to the kill buffer! */
    if ( f != FALSE ) {
        if ( (lastflag & CFKILL) == 0 )
            next_kill();
        thisflag |= CFKILL;
    }

    /* make sure the cursor gets back to the right place on an undo */
    undo_insert(OP_CPOS, 0L, obj);

    /* and now delete the characters */
    if ( ( status = backchar(f, n) ) == TRUE )
        status = ldelete( (long) n, f );

    return (status);
}

/* KILLTEXT:
 *
 * Kill text. If called without an argument, it kills from dot to the end of the
 * line, unless it is at the end of the line, when it kills the newline. If
 * called with an argument of 0, it kills from the start of the line to dot. If
 * called with a positive argument, it kills from dot forward over that number
 * of newlines. If called with a negative argument it kills backwards that
 * number of newlines. Normally bound to "C-K".
 */
int PASCAL NEAR killtext P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER LINE *nextp  = NULL;
    long          chunk   = 0;

    /* Don't do this command in read-only mode */
    if ( curbp->b_mode & MDVIEW )
        return ( rdonly() );

    /* flag this as a kill */
    if ( (lastflag & CFKILL) == 0 )
        next_kill();
    thisflag |= CFKILL;

    /* make sure the cursor gets back to the right place on an undo */
    undo_insert(OP_CPOS, 0L, obj);

    if ( f == FALSE ) {
        chunk = get_lused(curwp->w_dotp) - get_w_doto(curwp);
        if ( chunk == 0 )
            chunk = 1;
    } else if ( n == 0 ) {
        chunk = (-1) * get_w_doto(curwp);
    } else if ( n > 0 ) {
        chunk = get_lused(curwp->w_dotp) - get_w_doto(curwp) + 1;
        nextp = lforw(curwp->w_dotp);
        while ( --n ) {
            if ( nextp == curbp->b_linep )
                return (FALSE);

            chunk += get_lused(nextp) + 1;
            nextp = lforw(nextp);
        }
    } else if ( n < 0 ) {
        chunk = (-1) * get_w_doto(curwp);
        nextp = lback(curwp->w_dotp);
        while ( n++ ) {
            if ( nextp == curbp->b_linep )
                return (FALSE);

            chunk -= get_lused(nextp) + 1;
            nextp = lback(nextp);
        }
    }

    return ( ldelete(chunk, TRUE) );
}

/* SETMOD:
 *
 * Prompt and set an editor mode
 */
int PASCAL NEAR setmod P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    return ( adjustmode(TRUE, FALSE) );
}

/* DELMODE:
 *
 * Prompt and delete an editor mode
 */
int PASCAL NEAR delmode P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    return ( adjustmode(FALSE, FALSE) );
}

/* SETGMODE:
 *
 * Prompt and set a global editor mode
 */
int PASCAL NEAR setgmode P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    return ( adjustmode(TRUE, TRUE) );
}

/* DELGMODE:
 *
 * Prompt and delete a global editor mode
 */
int PASCAL NEAR delgmode P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    return ( adjustmode(FALSE, TRUE) );
}

/* ADJUSTMODE:
 *
 * Change the editor mode status
 */
int PASCAL NEAR adjustmode P2_(int, kind, int, global)
/* kind:    True = set,          false = delete               */
/* global:  True = global flag,  false = current buffer flag  */
{
    REGISTER char *scan;        /* scanning pointer to convert prompt */
    REGISTER int i;             /* loop index */
    REGISTER int status;        /* error return on input */
#if     COLOR
    REGISTER int uflag;         /* was modename uppercase?  */
#endif
    char prompt[50];            /* string to prompt user with */
    char cbuf[NPAT];            /* buffer to recieve mode name into */

    /* build the proper prompt string */
    if ( global )
        XSTRCPY(prompt, TEXT62);
/*                "Global mode to " */
    else
        XSTRCPY(prompt, TEXT63);
/*                "Mode to " */

    if ( kind == TRUE )
        XSTRCAT(prompt, TEXT64);
/*                 "add: " */
    else
        XSTRCAT(prompt, TEXT65);
/*                 "delete: " */

    /* prompt the user and get an answer */

    status = mlreply(prompt, cbuf, NPAT - 1);
    if ( status != TRUE )
        return (status);

    /* make it uppercase */

    scan = cbuf;
#if     COLOR
    uflag = (*scan >= 'A' && *scan <= 'Z');
#endif
    while ( *scan )
        uppercase( (unsigned char *) scan++ );

    /* test it first against the colors we know */
    if ( ( i = lookup_color(cbuf) ) != -1 ) {

#if     COLOR
        /* finding the match, we set the color */
        if ( global ) {
            if ( uflag ) {
                gfcolor = i;
                TRC( ("Globally setting foreground color to %s/%d",
                      (char *)cbuf, (int)i) );
            } else {
                gbcolor = i;
                TRC( ("Globally setting background color to %s/%d",
                      (char *)cbuf, (int)i) );
            }
# if     WINDOW_TEXT & 0
            refresh_screen(first_screen);
# endif
        } else if ( uflag ) {
            curwp->w_fcolor = i;
            TRC( ("Locally setting foreground color to %s/%d", (char *)cbuf,
                  (int)i) );
        } else {
            curwp->w_bcolor = i;
            TRC( ("Locally setting background color to %s/%d", (char *)cbuf,
                  (int)i) );
        }

        curwp->w_flag |= WFCOLR;
#endif
        mlerase();

        return (TRUE);
    }

    /* test it against the modes we know */

    for ( i = 0; i < NUMMODES; i++ ) {
        if ( strcmp(cbuf, modename[i]) == 0 ) {
            /* finding a match, we process it */
            if ( kind == TRUE )
                if ( global ) {
                    gmode |= (1 << i);
                    if ( (1 << i) == MDOVER )
                        gmode &= ~MDREPL;
                    else if ( (1 << i) == MDREPL )
                        gmode &= ~MDOVER;
                } else {
                    curbp->b_mode |= (1 << i);
                    if ( (1 << i) == MDOVER )
                        curbp->b_mode &= ~MDREPL;
                    else if ( (1 << i) == MDREPL )
                        curbp->b_mode &= ~MDOVER;
                }
            else if ( global )
                gmode &= ~(1 << i);
            else
                curbp->b_mode &= ~(1 << i);
            /* display new mode line */
            if ( global == 0 )
                upmode();
            mlerase();                  /* erase the junk */

            return (TRUE);
        }
    }

    mlwrite(TEXT66);

/*      "No such mode!" */
    return (FALSE);
}

/* CLRMES:
 *
 * This function simply clears the message line, mainly for macro usage
 */
int PASCAL NEAR clrmes P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    mlforce("");

    return (TRUE);
}

/* WRITEMSG:
 *
 * This function writes a string on the message line mainly for macro usage
 */
int PASCAL NEAR writemsg P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int status;
    char buf[NPAT];             /* buffer to recieve message into */

    if ( ( status = mlreply(TEXT67, buf, NPAT - 1) ) != TRUE )
/*                "Message to write: " */
        return (status);

    /* write the message out */
    mlforce(buf);

    return (TRUE);
}

/* GETFENCE:
 *
 * The cursor is moved to a matching fence
 */
int PASCAL NEAR getfence P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER LINE *oldlp;       /* original line pointer */
    REGISTER int oldoff;        /* and offset */
    REGISTER int sdir;          /* direction of search (1/-1) */
    REGISTER int count;         /* current fence level count */
    REGISTER char ch;           /* fence type to match against */
    REGISTER char ofence;       /* open fence */
    REGISTER char c;            /* current character in scan */
    REGISTER int qlevel;        /* quote level */

    /* save the original cursor position */
    oldlp = curwp->w_dotp;
    oldoff = get_w_doto(curwp);

    /* get the current character */
    if ( oldoff == get_lused(oldlp) )
        ch = '\r';
    else
        ch = lgetc(oldlp, oldoff);

    /* setup proper matching fence */
    switch ( ch ) {
    case '(':
        ofence = ')';
        sdir = FORWARD;
        break;

    case '{':
        ofence = '}';
        sdir = FORWARD;
        break;

    case '[':
        ofence = ']';
        sdir = FORWARD;
        break;

    case ')':
        ofence = '(';
        sdir = REVERSE;
        break;

    case '}':
        ofence = '{';
        sdir = REVERSE;
        break;

    case ']':
        ofence = '[';
        sdir = REVERSE;
        break;

    default:
        TTbeep();

        return (FALSE);
    }

    /* set up for scan */
    count = 1;
    qlevel = 0;

    /* scan until we find it, or reach the end of file */
    while ( count > 0 ) {
        if ( sdir == FORWARD )
            forwchar(FALSE, 1);
        else
            backchar(FALSE, 1);

        if ( get_w_doto(curwp) == get_lused(curwp->w_dotp) )
            c = '\r';
        else
            c = lgetc(curwp->w_dotp, get_w_doto(curwp));

        /* these only happen when we are outside quotes */
        if ( (oquote == 0) || (qlevel == 0) ) {

            if ( c == ch )
                ++count;
            if ( c == ofence )
                --count;
        }

        /* if we are quoting.... */
        if ( oquote != 0 ) {
            if ( oquote == cquote ) {

                /* only one quote character */
                if ( c == oquote )
                    qlevel = 1 - qlevel;
            } else {
                /* track opens and closes */
                if ( c == oquote )
                    ++qlevel;
                if ( c == cquote )
                    --qlevel;
            }
        }

        /* at the end/beginning of the buffer.... abort */
        if ( boundry(curwp->w_dotp, get_w_doto(curwp), sdir) )
            break;
    }

    /* if count is zero, we have a match, move the sucker */
    if ( count == 0 ) {
        curwp->w_flag |= WFMOVE;

        return (TRUE);
    }

    /* restore the current position */
    curwp->w_dotp = oldlp;
    set_w_doto(curwp, oldoff);
    TTbeep();

    return (FALSE);
}

/* FMATCH:
 *
 * Close fences are matched against their partners, and if on screen
 * the cursor briefly lights there
 */
int PASCAL NEAR fmatch P1_(char, ch)
/* ch:  Fence type to match against */
{
    REGISTER LINE *oldlp;       /* original line pointer */
    REGISTER int oldoff;        /* and offset */
    REGISTER LINE *toplp;       /* top line in current window */
    REGISTER int count;         /* current fence level count */
    REGISTER char opench;       /* open fence */
    REGISTER char c;            /* current character in scan */
    REGISTER int qlevel;        /* quote level */
    REGISTER int i;

    /* first get the display update out there */
    update(FALSE);

    /* save the original cursor position */
    oldlp = curwp->w_dotp;
    oldoff = get_w_doto(curwp);

    /* setup proper open fence for passed close fence */
    if ( ch == ')' )
        opench = '(';
    else if ( ch == '}' )
        opench = '{';
    else
        opench = '[';

    /* find the top line and set up for scan */
    toplp = lback(curwp->w_linep);
    count = 1;
    qlevel = 0;
    backchar(FALSE, 1);

    /* scan back until we find it, or reach past the top of the window */
    while ( count > 0 && curwp->w_dotp != toplp ) {

        backchar(FALSE, 1);
        if ( get_w_doto(curwp) == get_lused(curwp->w_dotp) )
            c = '\r';
        else
            c = lgetc(curwp->w_dotp, get_w_doto(curwp));

        /* these only happen when we are outside quotes */
        if ( (oquote == 0) || (qlevel == 0) ) {

            if ( c == ch )
                ++count;
            if ( c == opench )
                --count;
        }

        /* if we are quoting.... */
        if ( oquote != 0 ) {
            if ( oquote == cquote ) {

                /* only one quote character */
                if ( c == oquote )
                    qlevel = 1 - qlevel;
            } else {
                /* track opens and closes */
                if ( c == oquote )
                    ++qlevel;
                if ( c == cquote )
                    --qlevel;
            }
        }

        /* stop at the beginning of the buffer */
        if ( curwp->w_dotp == lforw(curwp->w_bufp->b_linep) &&
             get_w_doto(curwp) == 0 )
            break;
    }

    /* if count is zero, we have a match, display the sucker */
    /* there is a real machine dependant timing problem here we have yet to
     * solve......... */
    if ( count == 0 ) {
#if     WINDOW_MSWIN
        update(FALSE);
        term.t_sleep (term.t_pause);
#else
        for ( i = 0; i < term.t_pause; i++ )
            update(FALSE);
#endif
    }

    /* restore the current position */
    curwp->w_dotp = oldlp;
    set_w_doto(curwp, oldoff);

    return (TRUE);
}

/* ISTRING:
 *
 * Ask for and insert a string into the current buffer at the current point
 */
int PASCAL NEAR istring P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int status;        /* status return code */
    char tstring[NPAT + 1];     /* string to add */

    /* ask for string to insert */
    status = nextarg(TEXT68, tstring, NPAT, sterm);
/*            "String to insert: " */
    if ( status != TRUE )
        return (status);

    if ( f == FALSE )
        n = 1;

    if ( n < 0 )
        n = -n;

    /* insert it */
    while ( n-- && ( 0 != (status = linstr(tstring)) ) )
        ;

    return (status);
}

/* OVSTRING:
 *
 * Ask for and overwite a string into the current buffer at the
 * current point
 */
int PASCAL NEAR ovstring P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int status;        /* status return code */
    char tstring[NPAT + 1];     /* string to add */

    /* ask for string to insert */
    status = nextarg(TEXT69, tstring, NPAT, sterm);
/*            "String to overwrite: " */
    if ( status != TRUE )
        return (status);

    if ( f == FALSE )
        n = 1;

    if ( n < 0 )
        n = -n;

    /* insert it */
    while ( n-- && ( 0 != (status = lover(tstring)) ) )
        ;

    return (status);
}

/* LOOKUP_COLOR:
 */
int PASCAL NEAR lookup_color P1_(char *, sp)
/* sp:  Name to look up */
{
    REGISTER int i;             /* index into color list */

    /* test it against the colors we know */
    for ( i = 0; i < NCOLORS; i++ ) {
        if ( strcmp(sp, cname[i]) == 0 )
            return (i);
    }

    return (-1);
}


/*====================================================================*/
/* The x<ident> functions have the same funcionaly like the <ident>   */
/* functions found in newer C libraries.                              */
/*====================================================================*/

/* We want to use a working `assert' inside of some of these
 * functions therefor we use the following construct:
 */
#ifdef NDEBUG
# define  NDEBUF_WAS_DEFINED_
# undef   NDEBUG
#endif

/* XSTRCPY:
 *
 * strcpy() possibly overlapping regions:
 */
#if ( 0 ) /* Old --- straightforward --- implementation */
char *PASCAL NEAR xstrcpy P2_(char *, s1, CONST char *, s2)
{
    char  *s  = NULL;

    ASRT(NULL != s1);
    ASRT(NULL != s2);

    ASRT(NULL != (s = ROOM((STRLEN(s2) + 1) * SIZEOF(char))));
    strcpy(s, s2);
    strcpy(s1, s);
    CLROOM(s);

    return s1;
}
#else
char *PASCAL NEAR xstrcpy P2_(char *, s1, CONST char *, s2)
{
    if ( NULL != s1 && NULL != s2 ) {
        if        ( s1 <  s2 )  {
            int i = 0;

            for ( i = 0; '\0' != s2[i]; i++ ) {
                s1[i] = s2[i];
            }
            s1[i] = '\0';
        } else if ( s1 == s2 )  {
            /**EMPTY**/
        } else if ( s1 >  s2 )  {
            int i = 0;

            for ( i = STRLEN(s2); i >= 0; i-- ) {
                s1[i] = s2[i];
            }
        } else                  { /* Possible on e.g. OS/400 */
            strcpy(s1, s2);
        }
    } else if ( NULL == s1 )  {
        if ( '\0' != *s2 )  {
            /* No `ASRT()' here: So we may use this function
             * inside of `ASRT()'
             */
            assert(0);
            abort();
        }
    } else if ( NULL == s2 )  {
        *s1 = '\0';
    }

    return s1;
}
#endif

/* XSTRNCPY:
 *
 * strncpy() possibly overlapping regions:
 */
#if ( 0 ) /* Old --- straightforward --- implementation */
char *PASCAL NEAR xstrncpy P3_(char *, s1, CONST char *, s2, int, n)
{
    int   l2  = 0;
    char *s   = NULL;

    ASRT(0 <= n);
    if ( 0 == n ) {
        return s1;
    }
    ASRT(NULL != s1);
    ASRT(NULL != s2);

    l2  = STRLEN(s2);
    l2  = MAX2(l2, n);
    ASRT(NULL != (s = ROOM((l2 + 1) * SIZEOF(char))));
    strncpy(s, s2, n);  /* This will always succedd and result in
                         * a '\0'-terminated s. */

    strncpy(s1, s, n);
    CLROOM(s);

    return s1;
}
#else
char *PASCAL NEAR xstrncpy P3_(char *, s1, CONST char *, s2, int, n)
{
    if ( NULL != s1 && NULL != s2 ) {
        int l2  = 0;

        l2  = STRLEN(s2) + 1;
        if        ( s1 <  s2 )  {
            int ncpy  = MIN2(l2, n);
            int i     = 0;

            for ( i = 0; i < ncpy; i++ )  {
                s1[i] = s2[i];
            }
            for ( ; i < n; i++ )  {
                s1[i] = '\0';
            }
        } else if ( s1 == s2 )  {
            /**EMPTY**/
        } else if ( s1 >  s2 )  {
            int i = 0;

            for ( i = n - 1; i >= l2; i-- ) {
                s1[i] = '\0';
            }
            for ( ; i >= 0; i-- ) {
                s1[i] = s2[i];
            }
        } else                  { /* Possible on e.g. OS/400 */
            strncpy(s1, s2, n);
        }
    } else if ( NULL == s1 )  {
        if ( '\0' != *s2 )  {
            /* No `ASRT()' here: So we may use this function
             * inside of `ASRT()'
             */
            assert(0);
            abort();
        }
    } else if ( NULL == s2 )  {
        *s1 = '\0';
    }

    return s1;
}
#endif

/* XSTRCAT:
 *
 * strcat of possibly overlapping regions
 */
char *PASCAL NEAR xstrcat P2_(char *, s1, CONST char *, s2)
{
    int l = 0;

    l = (NULL == s1)? 0 : STRLEN(s1);

    return xstrcpy(s1 + l, s2);
}

/* XSTRLCPY:
 *
 * Like FreeBSD's strlcpy(): Equivalent semantics:
 *  n = strlcpy(dst, src, len);
 *  ---
 *  n = snprintf(dst, len, "%s", src);
 */
int PASCAL NEAR xstrlcpy P3_(char *, s1, CONST char *, s2, int, n)
{
    int l2  = 0;

    if ( NULL != s1 && NULL != s2 ) {
        /* Characters (not counting the terminating '\0') to copy:  */
        int ncpy  = 0;

        l2    = STRLEN(s2);
        ncpy  = MIN2(l2, n - 1);

        if ( 0 > ncpy ) {
            return l2;
        }

        if        ( s1 <  s2 )  {
            int i     = 0;

            for ( i = 0; i < ncpy; i++ )  {
                s1[i] = s2[i];
            }
            s1[i] = '\0';
        } else if ( s1 == s2 )  {
            /**EMPTY**/
        } else if ( s1 >  s2 )  {
            int i = 0;

            s1[ncpy]  = '\0';

            for ( i = ncpy - 1; i >= 0; i-- ) {
                s1[i] = s2[i];
            }
        } else                  { /* Possible on e.g. OS/400 */
            /* No `ASRT()' here: So we may use this function
             * inside of `ASRT()'
             */
            assert(0);
            abort();
        }
    } else if ( NULL == s1 )  {
        if ( NULL != s2 && '\0' != *s2 )  {
            /* No `ASRT()' here: So we may use this function
             * inside of `ASRT()'
             */
            assert(0);
            abort();
        }
    } else if ( NULL == s2 )  {
        if ( 0 < n )  {
            *s1 = '\0';
        }
    }

    return l2;
}

/* XSTRLCAT:
 *
 * Like FreeBSD's strlcat(): Equivalent semantics:
 *  n = strlcat(dst, src, len);
 *  ---
 *  dup = strdup(dst);
 *  n = snprintf(dst, len, "%s%s", dup, src);
 *  free(dup);
 */
int PASCAL NEAR xstrlcat P3_(char *, s1, CONST char *, s2, int, n)
{
    int l   = 0;

    l = (NULL == s1)? 0 : STRLEN(s1);

    return xstrlcpy(s1 + l, s2, n - l) + l;
}

/* SFSTRCPY_:
 *
 *  if size .GE. 0 copy src to dst using xstrlcpy(dst, src, SIZEOF(dst))
 *  else           copy src to dst using xstrcpy(dst, src) and log
 *                 a warning message.
 */
char *PASCAL NEAR sfstrcpy_ P5_(char *, dst, int, dst_size,
                                const char *, src,
                                const char *, file, int, line)
{
    if        ( 2 <= dst_size ) {
        xstrlcpy(dst, src, dst_size);
    } else if ( 0 <= dst_size ) {
        xstrlcpy(dst, src, dst_size);
        TRCK(("Warning safe string copy with size %d", dst_size), file, line);
    } else                      {
        xstrcpy(dst, src);
        TRCK(("%s", "Warning unsafe string copy"), file, line);
    }

    return dst;
}

/* SFSTRCAT_:
 *
 *  if size .GE. 0 append src to dst using xstrlcat(dst, src, SIZEOF(dst))
 *  else           append src to dst using xstrcat(dst, src) and log a
 *                 warning message.
 */
char *PASCAL NEAR sfstrcat_ P5_(char *, dst, int, dst_size,
                                const char *, src,
                                const char *, file, int, line)
{
    if        ( 2 <= dst_size ) {
        xstrlcat(dst, src, dst_size);
    } else if ( 0 <= dst_size ) {
        xstrlcat(dst, src, dst_size);
        TRCK(("Warning safe string cat with size %d", dst_size), file, line);
    } else                      {
        xstrcat(dst, src);
        TRCK(("%s", "Warning unsafe string cat"), file, line);
    }

    return dst;
}

static FILE *mytmpfile P0_()
{
# if !CYGWIN
    return tmpfile();
# else
/* `tmpfile() does *not* work with cygwin in the windows console! */
    {
        char  *fname  = NULL;
        FILE  *fp     = NULL;

        if ( NULL == (fname = gettmpfname("mytmpfile")) ) {
            TRC(("%s", "mytempfile(): gettmpfname() failed"));

            return NULL;
        }
        if ( NULL == (fp = fopen(fname, "wb+")) ) {
            TRC(("mytempfile(): fopen(\"%s\") failed", fname));

            return NULL;
        }

        unlink(fname);  /* ``Silly delete'' */

        return fp;
    }
# endif
}

/* XVSNPRINTF:
 *
 * An unelegant but portable version of C99 vsnprintf():
 * May be called with NULL == s .AND. 0 == n to get the size that would
 * be the result of an unrestricted write.
 * Returns the number of characters (not including the trailing '\0')
 * that would have been written if n were large enough.
 */
int PASCAL NEAR xvsnprintf P4_(char *, s, size_t, n, CONST char *, fmt,
                               va_list, ap)
{
    int nn  = n;                /* Want a signed value */
    int rc  = 0;
    int nr  = 0;                /* Number of chars to read */
    static FILE *fp = NULL;

    ASRT(0    <= nn);
    ASRT(NULL != fmt);

    if ( NULL == fp ) {           /* One-time initialization */
        if ( NULL == ( fp = mytmpfile() ) ) { /* ANSI C: Opened in wb+ mode */
            return (-1);
        }
        /*
         * Buffering: No real IO for not too large junks
         */
        if ( 0 != setvbuf(fp, NULL, _IOFBF, 0) ) {
            return (-2);
        }
    }

    rewind(fp);
    if ( 0 > ( rc = vfprintf(fp, fmt, ap) ) ) {
        return (-3);
    }

    if ( 0 == nn ) {
        /*
         * EMPTY
         */
    } else if ( 1 == nn ) {
        ASRT(NULL != s);

        s[0] = '\0';
    } else {
        ASRT(NULL != s);

        if ( 0 == rc ) {
            s[0] = '\0';
        } else {                /* Read back characters written: */
            rewind(fp);
            nr = MIN2(rc, nn - 1);
            if ( 1 != fread(s, nr, 1, fp) ) {
                return (-4);
            }
            s[nr] = '\0';
        }
    }

    if ( 0 < nn ) {
        TRC(("xvsnprintf: %s", s));
    }

    return rc;
}

/* XSNPRINTF:
 *
 * Like the C99 snprintf():
 * May be called with NULL == s .AND. 0 == n to get the size that would
 * be the result of an unrestricted write.
 * Returns the number of characters (not including the trailing '\0')
 * that would have been written if n were large enough.
 */
#if VARG
int CDECL NEAR  xsnprintf (va_alist)
    va_dcl
#else
int CDECL NEAR  xsnprintf (char *s, size_t n, CONST char *fmt, ...)
#endif
{
    int         rc    = 0;
    va_list     ap;
#if VARG
    char        *s    = NULL;
    size_t      n     = 0;
    CONST char  *fmt  = NULL;
#endif

    ZEROMEM(ap);

#if VARG
    va_start(ap);
    s   = va_arg(ap, char *);
    n   = va_arg(ap, size_t);
    fmt = va_arg(ap, CONST char *);
#else
    va_start(ap, fmt);
#endif
    ASRT(NULL != fmt);
    rc = xvsnprintf(s, n, fmt, ap);
    va_end(ap);

    return rc;
}

/* XVASPRINTF:
 *
 * Like GNU C vasprintf:
 * Allocate (using malloc()) a string large enough to hold the
 * resulting string.
 */
int PASCAL NEAR xvasprintf P3_(char **, ret, CONST char *, fmt, va_list, ap)
{
    int     rc  = 0;
    int     len = 0;
    char    *cp = NULL;
    va_list aq;

    ZEROMEM(aq);
    VA_COPY(aq, ap);
    ASRT(NULL != ret);
    ASRT(NULL != fmt);

    if ( 0 > (len = xvsnprintf(NULL, 0, fmt, ap)) ) {
        *ret  = NULL;

        return len;
    }
    len += 1;
    ASRT(NULL != (cp = ROOM(len * SIZEOF(char))));

    if ( 0 > (rc  = xvsnprintf(cp, len, fmt, aq)) ) {
        CLROOM(cp);
    }
    VA_END(aq);
    *ret  = cp;   /* NULL on error, see above */

    return rc;
}

/* XASPRINTF:
 *
 * Like GNU C asprintf:
 * Allocate (using malloc()) a string large enough to hold the
 * resulting string.
 */
#if VARG
int CDECL NEAR  xasprintf (va_alist)
    va_dcl
#else
int CDECL NEAR  xasprintf (char **ret, CONST char *fmt, ...)
#endif
{
    int     rc  = 0;
    va_list ap;
#if VARG
    char        **ret = NULL;
    CONST char  *fmt  = NULL;
#endif

    ZEROMEM(ap);

#if VARG
    va_start(ap);
    ret = va_arg(ap, char **);
    fmt = va_arg(ap, CONST char *);
#else
    va_start(ap, fmt);
#endif

    ASRT(NULL != ret);
    ASRT(NULL != fmt);

    rc = xvasprintf(ret, fmt, ap);
    va_end(ap);

    return rc;
}

/* XSTRTOK_R:
 */
char *PASCAL NEAR xstrtok_r P3_(char *, str, CONST char *, sep,
                                char **, next)
{
    char        *res  = NULL;
    CONST char  *sp   = sep;

    ASRT(NULL != next);

    if ( NULL == str )  {
        if ( NULL == (str = *next) )  {
            return NULL;
        }
    }
    *next = NULL;

    res = str;

    if ( NULL == sep )  {
        return  res;
    }

    for ( ; *str; str++ ) {
        for (sp = sep; *sp; sp++ ) {
            if ( *str == *sp ) {
                *next = str + 1;
                *str = '\0';

                return  res;
            }
        }
    }

    return  res;
}


/*====================================================================*/


/* ASTRCATC:
 *
 * Concatenate character c to string str and malloc the result.
 * Input string must either be NULL or malloced.
 */
char *PASCAL NEAR astrcatc P2_(CONST char *, str, CONST char, c)
{
    char  *nstr = NULL;
    int   len   = 0;

    if ( NULL == str ) {
        len = 1 + 1;
        ASRT(NULL != (nstr = ROOM(len * SIZEOF(char))));
    } else {
        len = STRLEN(str) + 1 + 1;
        ASRT(NULL != (nstr = REROOM(str, len * SIZEOF(char))));
    }
    nstr[len - 2]  = c;
    nstr[len - 1]  = '\0';

    return nstr;
}

/* ASTRCAT:
 *
 * Concatenate string d to string str and malloc the result.
 * Input string must either be NULL or malloced.
 */
char *PASCAL NEAR astrcat P2_(CONST char *, str, CONST char *, s)
{
    char  *nstr     = NULL;
    int   len       = 0;
    CONST char  *xs = (NULL == s)? "" : s;
    int   slen      = STRLEN(xs);

    if ( NULL == str ) {
        len = slen + 1;
        ASRT(NULL != (nstr = ROOM(len * SIZEOF(char))));
        strcpy(nstr, xs);
    } else {
        len = STRLEN(str) + slen + 1;
        ASRT(NULL != (nstr = REROOM(str, len * SIZEOF(char))));
        strcat(nstr, xs);
    }

    return nstr;
}

/* We want to use a working `assert' inside of some of these
 * functions therefor we use the following construct:
 */
#ifdef NDEBUF_WAS_DEFINED_
# define  NDEBUF
# undef   NDEBUF_WAS_DEFINED_
#endif

/*====================================================================*/


#if UEMACS_FEATURE_USE_STATIC_STACK
/*--------------------------------------------------------------------*/
/* A Stack ADT to be used for returning pointers to static variables  */
/* (e.g. char arrays) from functions: It allows (limited) indirect    */
/* recursion of such functions.                                       */
/*--------------------------------------------------------------------*/

typedef struct  STACK_S_  {
    int   stacksize;
    int   len;
    int   sp;
    char  *arr;
} STACK_T_;

VOIDP  NewStack P2_(int, stacksize, int, len)
{
    STACK_T_  *stack = NULL;

    ASRT(0 < stacksize);
    ASRT(0 < len);

    ASRT(NULL != (stack = (STACK_T_ *)ROOM(SIZEOF(*stack))));
    stack->stacksize = stacksize;
    stack->len       = len;
    stack->sp        = (-1);
    ASRT(NULL != (stack->arr = ROOM(stacksize * len)));

    return (VOIDP)stack;
}

char  *NextStackElem_ P3_(CONST VOIDP, stack, CONST char *, file, int, line)
{
    STACK_T_  *stk  = (STACK_T_ *)stack;

    ASRT(NULL != stk);

    TRCK(("NextStackElem: sp = %d", stk->sp), file, line);
    if ( (-1) > stk->sp ) {
        return NULL;
    }
    stk->sp++;
    if ( stk->stacksize <= stk->sp ) {
        return NULL;
    }

    return &stk->arr[stk->sp * stk->len];
}

char  *DecStackPtr_ P3_(CONST VOIDP, stack, CONST char *, file, int, line)
{
    STACK_T_  *stk  = (STACK_T_ *)stack;

    ASRT(NULL != stk);

    TRCK(("DecStackPtr: sp = %d", stk->sp), file, line);
    stk->sp--;
    if ( (-1) > stk->sp ) {
        return NULL;
    }

    return &stk->arr[(stk->sp + 1) * stk->len];
}

VOID  DelStack P1_(CONST VOIDP, stack)
{
    STACK_T_  *stk  = (STACK_T_ *)stack;

    ASRT(NULL != stk);

    CLROOM(stk->arr);
    CLROOM(stk);
}
#endif

/*====================================================================*/


FILE *PASCAL NEAR GetTrcFP P0_()
{
    static int  FirstCall = !0;
    static FILE *TrcFP    = NULL;

    if ( FirstCall )  {
        FirstCall = 0;

        {
            char *fname  = getenv(TRC_FILE_ENVVAR);

            if ( NULL == fname ) {
                TrcFP = NULL;
            } else {
                if ( NULL != (TrcFP = fopen(fname, "a")) )  {
                    setbuf(TrcFP, NULL);
                } else {
                    TrcFP = stderr;
                }
            }
        }
    }

    return TrcFP;
}

int          DebugMessage_lnno_   = 0;
CONST char  *DebugMessage_fname_  = (CONST char *)"";
#if VARG
int CDECL NEAR  DebugMessage (va_alist)
    va_dcl
#else
int CDECL NEAR  DebugMessage (CONST char *fmt, ...)
#endif
{
    int         rc    = 0;
    va_list     ap;
#if VARG
    CONST char  *fmt  = NULL;
#endif
    FILE        *TFP  = GetTrcFP();

    ZEROMEM(ap);

    if ( TFP )  {
      fprintf(TFP, "%s (%s/%03d): ", "TRC", DebugMessage_fname_,
              DebugMessage_lnno_);
#if VARG
      va_start(ap);
      fmt = va_arg(ap, CONST char *);
#else
      va_start(ap, fmt);
#endif
      rc = vfprintf(TFP, fmt, ap);
      va_end(ap);
      fprintf(TFP, "%s", "\n");
      fflush(TFP);
    }

    return rc;
}


/*====================================================================*/


char PASCAL NEAR  lputc_ P5_(LINE *, lp, int, n, char, c,
                             CONST char *, fnam, int, lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
#if ( !0 )
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);
#else
    ASRTK_TRC(lp->l_used_ <= lp->l_size_,
              ("lp->l_used_ = %d, lp->lsize_ = %d",
              lp->l_used_, lp->l_size_),
              fnam, lno);
#endif
    ASRTK(0 <= n,                     fnam, lno);
    ASRTK(n < lp->l_size_,            fnam, lno);

    return ( lp->l_text_[n] = c );
}

#undef  FUNC_
#define FUNC_ lgetc_
#if ( IS_UNIX() )
unsigned char PASCAL NEAR FUNC_ P4_(LINE *, lp, int, n, CONST char *,
                                    fnam, int, lno)
#else
         char PASCAL NEAR FUNC_ P4_(LINE *, lp, int, n, CONST char *,
                                    fnam, int, lno)
#endif
{
    ASRTK(NULL != lp,                     fnam, lno);
#if ( !0 )
    ASRTK(lp->l_used_ <= lp->l_size_,     fnam, lno);
#else
    ASRTK_TRC(lp->l_used_ <= lp->l_size_,
              ("lp->l_used_ = %d, lp->lsize_ = %d",
              lp->l_used_, lp->l_size_),
              fnam, lno);
#endif
    ASRTK((0 <= n) && (n <= lp->l_used_), fnam, lno);

    if ( n == lp->l_used_ ) {
#if ( 0 )
        TRCK(("%s(): Read at Buffer Boundry: l_size_ = %d, l_used_ = %d, l_text_[%d] = '%c'",
              FSTR_, (int)lp->l_size_, n, n, lp->l_text_[n]), fnam, lno);
#endif

        if ( n == lp->l_size_ ) {
            return ( '\0' );
        } else {
            return ( lp->l_text_[n] );
        }
    }

    return ( lp->l_text_[n] );
}

#undef  FUNC_
#define FUNC_ lgetcp_
char *PASCAL NEAR FUNC_ P4_(LINE *, lp, int, n, CONST char *,
                            fnam, int, lno)
{
    ASRTK(NULL != lp,                     fnam, lno);
#if ( !0 )
    ASRTK(lp->l_used_ <= lp->l_size_,     fnam, lno);
#else
    ASRTK_TRC(lp->l_used_ <= lp->l_size_,
              ("lp->l_used_ = %d, lp->lsize_ = %d",
              lp->l_used_, lp->l_size_),
              fnam, lno);
#endif
    ASRTK((0 <= n) && (n <= lp->l_used_), fnam, lno);

#if ( 0 )
    if ( n == lp->l_used_ ) {
        TRCK(("%s(): Read at Buffer Boundry: l_size_ = %d, l_used_ = %d, l_text_[%d] = '%c'",
              FSTR_, (int)lp->l_size_, n, n, lp->l_text_[n]), fnam, lno);
    }
#endif

    return ( &(lp->l_text_[n]) );
}

int PASCAL NEAR get_lused_ P3_(LINE *, lp, CONST char *, fnam, int, lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
#if ( !0 )
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);
#else
    ASRTK_TRC(lp->l_used_ <= lp->l_size_,
              ("lp->l_used_ = %d, lp->lsize_ = %d",
              lp->l_used_, lp->l_size_),
              fnam, lno);
#endif

    return ( (lp)->l_used_ );
}

int PASCAL NEAR set_lused_ P4_(LINE *, lp, int, used,
                               CONST char *, fnam, int, lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
#if ( !0 )
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);
#else
    ASRTK_TRC(lp->l_used_ <= lp->l_size_,
              ("lp->l_used_ = %d, lp->lsize_ = %d",
              lp->l_used_, lp->l_size_),
              fnam, lno);
#endif
    ASRTK(0 <= used,                  fnam, lno);
    ASRTK(used <= lp->l_size_,        fnam, lno);

    return ( (lp)->l_used_ = used );
}

int PASCAL NEAR get_lsize_ P3_(LINE *, lp, CONST char *, fnam, int, lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
#if ( !0 )
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);
#else
    ASRTK_TRC(lp->l_used_ <= lp->l_size_,
              ("lp->l_used_ = %d, lp->lsize_ = %d",
              lp->l_used_, lp->l_size_),
              fnam, lno);
#endif

    return ( (lp)->l_size_ );
}

#undef  FUNC_
#define FUNC_ get_w_doto_
int PASCAL NEAR FUNC_ P3_(EWINDOW *, wp, CONST char *, fnam, int, lno)
{
    ASRTK(NULL != wp,                                 fnam, lno);
    ASRTK(NULL != wp->w_dotp,                         fnam, lno);
    ASRTK(0 <= wp->w_dotp->l_used_,                   fnam, lno);
    ASRTK(wp->w_dotp->l_used_ <= wp->w_dotp->l_size_, fnam, lno);
    ASRTK(0 <= wp->w_doto_,                           fnam, lno);

    if ( wp->w_dotp->l_used_ < wp->w_doto_ )  {
        TRCK(("%s(): w_dotp->l_used_(%d) < wp->w_doto_(%d)",
              FSTR_, wp->w_dotp->l_used_, wp->w_doto_), fnam, lno);
    }

    return ( wp->w_doto_ );
}

#undef  FUNC_
#define FUNC_ set_w_doto_
int PASCAL NEAR FUNC_ P4_(EWINDOW *, wp, int, doto,
                          CONST char *, fnam, int, lno)
{
    ASRTK(NULL != wp,                                 fnam, lno);
    ASRTK(NULL != wp->w_dotp,                         fnam, lno);
    ASRTK(0 <= wp->w_dotp->l_used_,                   fnam, lno);
    ASRTK(wp->w_dotp->l_used_ <= wp->w_dotp->l_size_, fnam, lno);
    ASRTK(0 <= wp->w_doto_,                           fnam, lno);

    if ( 0 > doto ) {
        TRCK(("%s(): Negativ doto: %d. REWRITTEN TO %d.",
              FSTR_, doto, 0), fnam, lno);

        doto  = 0;
    }
    if ( wp->w_dotp->l_used_ < doto ) {
        TRCK(("%s(): Too large doto: %d. REWRITTEN TO: %d.",
              FSTR_, doto, wp->w_dotp->l_used_), fnam, lno);
        doto  = wp->w_dotp->l_used_;  /* TODO: Use `l_used_ - 1'? */
    }

    return ( (wp)->w_doto_ = doto );
}

#undef  FUNC_
#define FUNC_ get_b_doto_
int PASCAL NEAR FUNC_ P3_(BUFFER *, bp, CONST char *, fnam, int, lno)
{
    ASRTK(NULL != bp,                                 fnam, lno);
    ASRTK(NULL != bp->b_dotp,                         fnam, lno);
    ASRTK(0 <= bp->b_dotp->l_used_,                   fnam, lno);
    ASRTK(bp->b_dotp->l_used_ <= bp->b_dotp->l_size_, fnam, lno);
    ASRTK(0 <= bp->b_doto_,                           fnam, lno);

    if ( bp->b_dotp->l_used_ < bp->b_doto_ )  {
        TRCK(("%s(): b_dotp->l_used_(%d) < bp->b_doto_(%d)",
              FSTR_, bp->b_dotp->l_used_, bp->b_doto_), fnam, lno);
    }

    return ( (bp)->b_doto_ );
}

#undef  FUNC_
#define FUNC_ set_b_doto_
int PASCAL NEAR FUNC_ P4_(BUFFER *, bp, int, doto,
                          CONST char *, fnam, int, lno)
{
    ASRTK(NULL != bp,                                 fnam, lno);
    ASRTK(NULL != bp->b_dotp,                         fnam, lno);
    ASRTK(0 <= bp->b_dotp->l_used_,                   fnam, lno);
    ASRTK(bp->b_dotp->l_used_ <= bp->b_dotp->l_size_, fnam, lno);
    ASRTK(0 <= bp->b_doto_,                           fnam, lno);

    if ( 0 > doto ) {
        TRCK(("%s(): Negativ doto: %d. REWRITTEN TO %d.",
              FSTR_, doto, 0), fnam, lno);

        doto  = 0;
    }
    if ( bp->b_dotp->l_used_ < doto ) {
        TRCK(("%s(): Too large doto: %d. REWRITTEN TO: %d.",
              FSTR_, doto, bp->b_dotp->l_used_), fnam, lno);
        doto  = bp->b_dotp->l_used_;  /* TODO: Use `l_used_ - 1'? */
    }

    return ( (bp)->b_doto_ = doto );
}


/*====================================================================*/


/*====================================================================*/
/* The generic transformation functions which implement the           */
/* transformations of regions, paragraphs and buffers in a generic    */
/* and *undoable* way:                                                */
/*====================================================================*/


int PASCAL NEAR TransformRegion P2_(filter_func_T, filter, VOIDP, argp)
{
    LINE    *linep  = NULL;
    int     loffs   = 0;
    long    rsize   = 0;
    char    *rtext  = xstrdup("");
    char    *rstart = xstrdup("");
    int     i       = 0;
    REGION  region;

    ZEROMEM(region);
    ASRT(NULL != filter);

    if ( getregion(&region) != TRUE ) {
        return FALSE;
    }

    linep = region.r_linep;                     /* Current line.    */
    loffs = region.r_offset;                    /* Current offset.  */
    rsize = region.r_size;
    for (  i = 0; i < loffs; i++ )  {
        rstart  = astrcatc(rstart, lgetc(linep, i));
    }
    while ( rsize-- ) {
        if ( loffs == get_lused(linep) ) {      /* End of line.         */
            rtext = astrcatc(rtext, '\r');
            linep = lforw(linep);
            loffs = 0;
        } else {                                /* Middle of line.  */
            rtext = astrcatc(rtext, lgetc(linep, loffs));
            ++loffs;
        }
    }

    /* make sure the cursor gets back to the right place on an undo */
    undo_insert(OP_CPOS, 0L, obj);

    /* delete the region */
    curwp->w_dotp = region.r_linep;
    set_w_doto(curwp, region.r_offset);

    if ( TRUE != ldelete(region.r_size, TRUE) ) {
        CLROOM(rstart);
        CLROOM(rtext);

        return FALSE;
    }


    /*===============================================================*/
    /* Do something with rtext here.                                 */
    /*===============================================================*/
    {
        char  *outline  = (*filter)(rstart, rtext, argp);
        CLROOM(rstart);
        CLROOM(rtext);
        linstr(outline);
        CLROOM(outline);
    }

    return TRUE;
}

int PASCAL NEAR TransformParagraph P2_(filter_func_T, filter,
                                       VOIDP, argp)
{
    /* make sure the cursor gets back to the right place on an undo */
    undo_insert(OP_CPOS, 0L, obj);

    /* mark out the end and beginning of the para to transform  */
    gotoeop(FALSE, 1);

    /* set the mark here */
    curwp->w_markp[0] = curwp->w_dotp;
    curwp->w_marko[0] = get_w_doto(curwp);

    /* go to the beginning of the paragraph */
    gotobop(FALSE, 1);
    set_w_doto(curwp, 0); /* force us to the beginning of line */

    return TransformRegion(filter, argp);
}

int PASCAL NEAR TransformBuffer P2_(filter_func_T, filter, VOIDP, argp)
{
    /* make sure the cursor gets back to the right place on an undo */
    undo_insert(OP_CPOS, 0L, obj);

    /* mark out the end and beginning of the para to transform  */
    gotoeob(FALSE, 1);

    /* set the mark here */
    curwp->w_markp[0] = curwp->w_dotp;
    curwp->w_marko[0] = get_w_doto(curwp);

    /* go to the beginning of the paragraph */
    gotobob(FALSE, 1);
    set_w_doto(curwp, 0); /* force us to the beginning of line */

    return TransformRegion(filter, argp);
}


/*====================================================================*/
/* The (static) filter functions which do the actual work:            */
/*====================================================================*/

#if ( 0 )
static char *PASCAL NEAR  filter_test_00 P3_(CONST char *, rstart,
                                             CONST char *, rtext, VOIDP, argp)
{
    char  *res  = NULL;
    char  *str  = NULL;
    int   i     = 0;
    char  c     = '\0';

    ASRT(NULL != rstart);
    ASRT(NULL != rtext);

    str = xstrdup(rstart);
    for ( i = 0; '\0' != (c = str[i]); i++ )  {
        str[i]  = toupper(c);
    }

    xasprintf(&res, "<%s><%s>", str, rtext);
    CLROOM(str);

    return res;
}
#endif

static char *PASCAL NEAR  filter_test_01 P3_(CONST char *, rstart,
                                             CONST char *, rtext,
                                             VOIDP, argp)
{
    char  *res  = NULL;

    ASRT(NULL != rstart);
    ASRT(NULL != rtext);

    xasprintf(&res, "%s", rtext);

    return res;
}

static char *PASCAL NEAR  filter_test P3_(CONST char *, rstart,
                                          CONST char *, rtext,
                                          VOIDP, argp)
{
    return filter_test_01(rstart, rtext, argp);
}

static int PASCAL NEAR  dsplen P1_(CONST char *, s)
{
    int res = 0;

    if ( NULL == s )  {
        return 0;
    }

    for ( ; '\0' != *s; s++ ) {
        switch ( *s ) {
            case '\t':
                if ( 0 < tabsize )  {
                    res = (res / tabsize + 1) * tabsize;
                }
                break;
            default:
                res++;
                break;
        }
    }

    return  res;
}

/* Format the space separated words in input into a paragraph, where each
 * line starts with start and its length should not exceed fcol (except
 * if start + first word already exceed fcol).
 * The result will be allocated with malloc().
 *
 * The routine skips leading and ignores trailing blanks. Words may be
 * separated by one ore more blanks.
 */
static char *PASCAL NEAR  format_para P5_(CONST char *,  start,
                                          CONST char *,  input,
                                          int,           fcol,
/* Skip start in first line:  */          int,           ommit,
                                          int,           parindent)
{
    char        *res  = xstrdup("");
    int         slen  = 0;
    CONST char  *ip   = input;
    char        *cp   = 0;
    int         col   = 0;
    int         i     = 0;

    ASRT(NULL != start);
    ASRT(NULL != input);
    slen  = dsplen(start);

    while ( ' ' == *ip )  ip++;   /* Skip space */
    if ( ! *ip )  return res;     /* Shortcut   */

    if ( ! ommit )  {
        res =  astrcat(res, start);
    }
    col = slen;

    if ( NULL == (cp = (char *)strchr(ip, ' ')) ) {
        for ( i = 0; i < parindent; i++ ) {
            res = astrcatc(res, ' ');
        }

        return  astrcat(res, ip);
    } else {
        for ( i = 0; i < parindent; i++ ) {
            res = astrcatc(res, ' ');
            col++;
        }
        for (; ip < cp; ip++ )  {
            res = astrcatc(res, *ip);
            col++;
        }
        while ( ' ' == *ip )  ip++;   /* Skip space */
        if ( ! *ip )  return res;     /* Shortcut   */
    }

    for ( ;; )  {
        int ncol  = col;  /* Column of character before next space  */

        if ( NULL != (cp  = (char *)strchr(ip, ' ')) )  {
            ncol  += ((CONST char *)cp - ip) + 1;
        } else {
            ncol  += STRLEN(ip) + 1;
        }

        if ( fcol <= ncol ) {
            res = astrcatc(res, '\r');
            res = astrcat(res, start);
            col = slen;
        } else {
            res = astrcatc(res, ' ');
            col++;
        }

        if ( NULL == cp ) {
            return  astrcat(res, ip);
        } else {
            for (; ip < cp; ip++ )  {
                res = astrcatc(res, *ip);
                col++;
            }
            while ( ' ' == *ip )  ip++;   /* Skip space */
            if ( ! *ip )  return res;     /* Shortcut   */
        }
    }

    /***return  res;***/  /***NOT_REACHED***/
}

static char *PASCAL NEAR  filter_fill P3_(CONST char *, rstart,
                                          CONST char *, rtext,
                                          VOIDP, argp)
/*
 * Fill the region --- i.e.reformat it so that:
 * (1) Multiple empty lines are converted into one empty line.
 * (2) Text is wrapped at fillcol.
 *
 * If NULL != argp argp points to an integer whose value gives
 * the first line indent of every paragraph.
 */
{
    typedef enum  { IS_TEXT, IS_SPACE } state_T;

    char    *res      = xstrdup("");  /* Reformatted region           */
    state_T state     = IS_TEXT;
    char    *start    = NULL;         /* Each line will start with it */
    char    *text     = NULL;         /* rtext without                */
                                      /*  (lead|trail)ing \r          */
    int     nbef      = 0;            /* Lines before text            */
    int     naft      = 0;            /* Lines aftertext              */
    int     tpos      = 0;
    char    *pptext   = xstrdup("");  /* Preprocessed text            */
    char    *lptr     = NULL;
    int     sflag     = FALSE;
    char    *context  = NULL;
    int     parindent = (NULL == argp)? 0 : *(int *)argp;

    ASRT(NULL != rstart);
    ASRT(NULL != rtext);

    {
        CONST char  *ccp  = rtext;
        int         l     = 0;
        int         nsp   = 0;
        int         ncr   = 0;

        while ( *ccp && ISSPACE(*ccp) ) {
            if ( '\r' == *ccp )  {
                nsp = 0;
                ncr++;
            } else {
                nsp++;
            }

            ccp++;
        }
        while ( 0 < nsp-- ) ccp--;
        nbef  = ncr;
        text  = xstrdup(ccp);

        nsp = 0;
        ncr = 0;
        if ( 0 < (l = STRLEN(text)) ) {
            char  *cp = &text[l - 1];

            while ( 0 < l && ISSPACE(*cp) ) {
                if ( '\r' == *cp )  {
                    nsp = 0;  /**NOT_USED**/
                    ncr++;
                } else {
                    nsp++;    /**NOT_USED**/
                }
                *cp = '\0';

                cp--;
                l--;
            }
        }
        naft  = ncr;
    }

    sflag = ( rstart && 0 < STRLEN(rstart) );

    /* start: It contains the string wich will be prepended to every
     *        line:
     */
    if ( rstart && *rstart )  {
        start = xstrdup(rstart);
    } else {
        int   i = 0;
        char  c = '\0';

        start = xstrdup("");
        while ( '\0' != (c = text[i]) && ISSPACE(c) )   {
            start = astrcatc(start, c);
            i++;
        }
        tpos  +=  i;
    }

    {
        int   ncr = 0;  /* Number of '\r' in a seq. of white space    */
        int   nsp = 0;  /* Number of white space in this sequence.    */
        int   i   = 0;
        char  c   = '\0';

        /* pptext: <word>' '<word>' '<word>\r<word>' '<word>' '<word> */
        for ( i = tpos; '\0' != (c = text[i]); i++ )    {
            switch ( state )  {
                case IS_TEXT:
                    if ( ISSPACE(c) ) {
                        if ( '\r' == c )  {
                            ncr++;
                        }
                        nsp++;
                        state = IS_SPACE;
                    } else {
                        pptext  = astrcatc(pptext, c);
                    }
                    break;
                case IS_SPACE:
                    if ( ISSPACE(c) ) {
                        if ( '\r' == c )  {
                            ncr++;
                        }
                        nsp++;
                    } else {
                        if ( 2 <= ncr ) {
                            pptext  = astrcatc(pptext, '\r');
                        } else if ( 0 < nsp ) {
                            pptext  = astrcatc(pptext, ' ');
                        }
                        ncr = 0;
                        nsp = 0;
                        pptext  = astrcatc(pptext, c);
                        state = IS_TEXT;
                    }
                    break;
                default:
                    ASRT(!"IMPOSSIBLE");
                    break;
            }
        }
    }


    while ( 0 < nbef-- )  {
        res = astrcatc(res, '\r');
    }

    lptr  = xstrtok_r(pptext, "\r", &context);  /* .NE. NULL  */
    for ( ;; )  {
        char  *para = format_para(start, lptr, fillcol, sflag,
                                  parindent);

        sflag = FALSE;
        res = astrcat(res, para);
        CLROOM(para);
        res = astrcat(res, "\r\r");

        if ( NULL == (lptr  = xstrtok_r(NULL, "\r", &context)) )  {
            int l = STRLEN(res) - 1;

            for ( ; 0 <= l && ISSPACE(res[l]); l-- )  {
                res[l]  = '\0';
            }

            break;
        }
    }

    while ( 0 < naft-- )  {
        res = astrcatc(res, '\r');
    }


    CLROOM(pptext);
    CLROOM(text);
    CLROOM(start);

    return res;
}

static char *PASCAL NEAR  filter_indent P3_(CONST char *, rstart,
                                            CONST char *, rtext,
                                            VOIDP, argp)
/*
 * Indent the region:
 *
 * IF '\r' is NOT followd by <white space> OPT '\r' THEN
 *   IF 0 < stabsize THEN
 *     Replace '\r' ---> '\r' + stabsize x ' '
 *   ELSE
 *     Replace '\r' ---> '\r' + '\t'
 *   ENDIF
 * ENDIF
 *
 * argp (IF NOT NULL) points to an integer containing the number on indents.
 */
#define filter_indent_do_indent_(cp)  do  {                         \
    CONST char  *sp_  = (cp);                                       \
    CONST char  *np_  = sp_ + 1;                                    \
                                                                    \
    while ( ISSPACE(*np_) )  np_++;                                 \
    if ( *np_ && '\r' != *np_ ) {                                   \
        if ( !((curbp->b_mode & MDCMOD) && ('#' == *(sp_ + 1))) ) { \
            int j_ = 0;                                             \
                                                                    \
            for ( j_ = 0; j_ < count; j_++ )  {                     \
                if ( 0 < stabsize ) {                               \
                    int i_ = 0;                                     \
                                                                    \
                    for ( i_ = 0; i_ < stabsize; i_++ ) {           \
                        res = astrcatc(res, ' ');                   \
                    }                                               \
                } else {                                            \
                    res = astrcatc(res, '\t');                      \
                }                                                   \
            }                                                       \
        }                                                           \
    }                                                               \
} while ( 0 )
{
    char        *res  = xstrdup("");  /* Reformatted region */
    CONST char  *cp   = rtext;
    int         count = 1;

    ASRT(NULL != rtext);

    if ( NULL != argp ) {
        count = *(int *)argp;

        if ( 0 >= count ) {
            return astrcat(res, cp);
        }
    }

    if ( ! (rstart && *rstart) )  {
        filter_indent_do_indent_(cp);
    }
    for ( ; *cp; cp++ ) {
        res = astrcatc(res, *cp);

        if ( '\r' == *cp )  {
            filter_indent_do_indent_(cp);
        }
    }

    return res;
}
#undef  filter_indent_do_indent_


static char *PASCAL NEAR  filter_undent P3_(CONST char *, rstart,
                                            CONST char *, rtext,
                                            VOIDP, argp)
/*
 * undent the region:
 *
 * IF 0 < stabsize THEN
 *   Replace '\r' + stabsize x ' '  ---> '\r'
 * ELSE
 *   Replace '\r' + '\t'            ---> '\r'
 * ENDIF
 *
 * argp (IF NOT NULL) points to an integer containing the number on undents.
 */
#define filter_undent_do_undent_(pcp)  do  {                    \
    CONST char  **sp_ = (pcp);                                  \
    CONST char  *cp_  = *sp_;                                   \
    int   j_    = 0;                                            \
                                                                \
    for ( j_ = 0; j_ < count; j_++ )  {                         \
        if ( 0 < stabsize ) {                                   \
            int i_ = 0;                                         \
                                                                \
            for ( i_ = 0; i_ < stabsize; i_++ ) {               \
                if ( ' ' == *cp_ )  {                           \
                    cp_++;                                      \
                }                                               \
            }                                                   \
        } else {                                                \
            if ( '\t' == *cp_ )  {                              \
                cp_++;                                          \
            }                                                   \
        }                                                       \
    }                                                           \
    *sp_  = cp_;                                                \
} while ( 0 )
{
    char        *res  = xstrdup("");  /* Reformatted region */
    CONST char  *cp   = rtext;
    int         count = 1;

    ASRT(NULL != rtext);

    if ( NULL != argp ) {
        count = *(int *)argp;

        if ( 0 >= count ) {
            return astrcat(res, cp);
        }
    }

    if ( ! (rstart && *rstart) )  {
        filter_undent_do_undent_(&cp);
    }
    while ( *cp ) {
        res = astrcatc(res, *cp);

        if ( '\r' == *cp++ )  {
            filter_undent_do_undent_(&cp);
        }
    }

    return res;
}
#undef  filter_undent_do_undent_


/*====================================================================*/
/* The actual transormation routines:                                 */
/*====================================================================*/

int PASCAL NEAR trRegFill P2_(int, f, int, n)
/* (f, n): Argument flag and num                        */
/* If f == TRUE: n: Indent of first line of a paragraph */
{
    if ( f == FALSE ) {
        n = parindent;
    }

    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformRegion(&filter_fill, &n);
}

int PASCAL NEAR trRegIndent P2_(int, f, int, n)
/* (f, n): Argument flag and numeric repeat count */
{
    if ( f == FALSE ) {
        n = 1;
    }

    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformRegion(&filter_indent, &n);
}

int PASCAL NEAR trRegUndent P2_(int, f, int, n)
/* (f, n): Argument flag and numeric repeat count */
{
    if ( f == FALSE ) {
        n = 1;
    }

    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformRegion(&filter_undent, &n);
}

int PASCAL NEAR trRegTest_ P2_(int, f, int, n)
/* (f, n): Ignored arguments  */
{
    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformRegion(&filter_test, NULL);
}

int PASCAL NEAR trParTest_ P2_(int, f, int, n)
/* (f, n): Ignored arguments  */
{
    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformParagraph(&filter_test, NULL);
}

int PASCAL NEAR trParFill P2_(int, f, int, n)
/* (f, n): Argument flag and num                        */
/* If f == TRUE: n: Indent of first line of a paragraph */
{
    if ( f == FALSE ) {
        n = parindent;
    }

    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformParagraph(&filter_fill, &n);
}

int PASCAL NEAR trBufFill P2_(int, f, int, n)
/* (f, n): Argument flag and num                        */
/* If f == TRUE: n: Indent of first line of a paragraph */
{
    if ( f == FALSE ) {
        n = parindent;
    }

    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformBuffer(&filter_fill, &n);
}

int PASCAL NEAR trBufTest_ P2_(int, f, int, n)
/* (f, n): Ignored arguments  */
{
    /*===============================================================*/
    /* Don't do this command in read-only mode */
    if ( curbp->b_mode&MDVIEW ) {
        return ( rdonly() );
    }

    /* flag this command as a kill */
    if ( (lastflag & CFKILL) == 0 ) {
        next_kill();
    }
    thisflag |= CFKILL;
    /*===============================================================*/

    return TransformBuffer(&filter_test, NULL);
}


/*====================================================================*/



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
