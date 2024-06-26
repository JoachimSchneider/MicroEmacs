/*======================================================================
 *      Code to handle Abbreviation Expansions
 *      for MicroEMACS 4.00
 *      (C)Copyright 1995 by Daniel M. Lawrence
 *====================================================================*/

/*====================================================================*/
#define ABBREV_C_
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


VOID PASCAL NEAR ab_save P1_(char, c)
/* c: character to add to current word buffer */
{
    char  *s  = NULL;   /* ptr to cycle chars */

    /* only in ABBREV mode */
    if ( (curbp->b_mode & MDABBR) == 0 )
        return;

    /* is the buffer full? */
    if ( ab_pos == ab_end ) {
        /* shift all the letters down one */
        s = ab_word;
        while ( s < ab_end ) {
            *s = *(s+1);
            ++s;
        }
        ab_pos--;
    }

    /* add the character */
    *ab_pos++ = c;
    *ab_pos = 0;
    TRC(("ab_save(): <== `%c'", c));
}

VOID PASCAL NEAR ab_expand P0_()
{
    char  *exp  = NULL;   /* expansion of current symbol */
    char  c     = '\0';   /* current character to insert */

    /* only in ABBREV mode, never in VIEW mode */
    if ( (curbp->b_mode & MDABBR) == 0      ||
         (curbp->b_mode & MDVIEW) == MDVIEW     ) {
        return;
    }

    /* is the current buffer a symbol in the abbreviation table?  */
    if ( ( exp = ab_full?
                    ab_taillookup(ab_word)
                      :
                    ab_lookup(ab_word) ) != NULL )  {
        TRC(("ab_expand(): `%s' ===> `%s'", ab_word, exp));

        /* backwards delete the symbol */
        ldelete(-( (long)STRLEN(ab_word) ), FALSE);

        /* and insert its expansion */
        while ( *exp ) {
            c = *exp++;
            /*
             * If a space was typed, fill column is defined, the argument is
             * non-negative, wrap mode is enabled, and we are now past fill
             * column, perform word wrap.
             */
            if ( c == ' '                         &&
                 (curwp->w_bufp->b_mode & MDWRAP) &&
                 fillcol > 0                      &&
                 getccol(FALSE) > fillcol )           {
                execkey(&wraphook, FALSE, 1);
            }
            linsert(1, c);
        }

        /* ring the bell */
        if ( ab_bell )  {
            TTbeep();
        }
    }

    /* reset the word buffer */
    ab_pos = ab_word;
    *ab_pos = 0;
}

/* ADD_ABBREV:
 *
 * Add a new abbreviation
 */
int PASCAL NEAR add_abbrev P2_(int, f, int, n)
/* f, n:  Numeric flag and argument */
{
    REGISTER int status;        /* status return */
    char sym_name[MAXSYM+1];    /* name of symbol to fetch */
    char value[NSTRING];        /* value to set symbol to */

    /* first get the symbol name */
    status = mlreply(TEXT231, sym_name, MAXSYM + 1);
/*           "Abbreviation to set: " */
    if ( status != TRUE )
        return (status);

    /* get the value for that expansion */
    if ( f == TRUE )
        XSTRCPY( value, int_asc(n) );
    else {
        status = mlreply(TEXT53, &value[0], NSTRING);
/*               "Value: " */
        if ( status == ABORT )
            return (status);
    }

    /* and add the abbreviation to the list */
    return ( ab_insert(sym_name, value) );
}

/* DEL_ABBREV:
 *
 * Delete a single abbreviation
 */
int PASCAL NEAR del_abbrev P2_(int, f, int, n)
/* f, n:  Numeric flag and argument */
{
    REGISTER int status;        /* status return */
    char sym_name[MAXSYM+1];    /* name of symbol to fetch */

    /* first get the symbol name */
    status = mlreply(TEXT232, sym_name, MAXSYM + 1);
/*           "Abbreviation to delete: " */
    if ( status != TRUE )
        return (status);

    /* and yank the abbreviation to the list */
    return ( ab_delete(sym_name) );
}

/* KILL_ABBREVS:
 *
 * Kill all abbreviations
 */
int PASCAL NEAR kill_abbrevs P2_(int, f, int, n)
/* f, n:  Numeric flag and argument */
{
    /* kill them! */
    return ( ab_clean() );
}

/* DESC_ABBREVS:
 */
int PASCAL NEAR desc_abbrevs P2_(int, f, int, n)
/* f, n:  Numeric flag and argument */
{
    REGISTER BUFFER *abbbuf;    /* buffer to put abbreviation list into */
    REGISTER ABBREV *cur_node;    /* ptr to current abbreviation */
    char outseq[NSTRING];       /* output buffer for keystroke sequence */

    /* and get a buffer for it */
    abbbuf = bfind(TEXT234, TRUE, BFINVS);
/*         "Abbreviation list" */
    if ( abbbuf == NULL || bclear(abbbuf) == FALSE ) {
        mlwrite(TEXT235);

/*          "Can not display abbreviation list" */
        return (FALSE);
    }

    /* let us know this is in progress */
    mlwrite(TEXT233);
/*      "[Building Abbreviation list]" */

    /* build the abbreviation list */
    cur_node = ab_head;
    while ( cur_node != (ABBREV *)NULL ) {

        /* add in the abbreviation symbol name */
        XSTRCPY(outseq, cur_node->ab_sym);
        pad(outseq, 20);

        /* add it's expansion */
        strncat(outseq, cur_node->ab_exp, NSTRING - 20);
        outseq[NSTRING - 1] = 0;

        /* and add it as a line into the buffer */
        if ( addline(abbbuf, outseq) != TRUE )
            return (FALSE);

        cur_node = cur_node->ab_next;
    }

    /* display the list */
    wpopup(abbbuf);
    mlerase();          /* clear the mode line */

    return (TRUE);
}

/* INS_ABBREVS:
 *
 * Insert a list of all the current abbreviations into the current buffer
 */
int PASCAL NEAR ins_abbrevs P2_(int, f, int, n)
/* f, n:  Numeric flag and argument */
{
    REGISTER ABBREV *cur_node;    /* ptr to current abbreviation */

    /* insert the abbreviation list in the current buffer */
    cur_node = ab_head;
    while ( cur_node != (ABBREV *)NULL ) {

        /* insert the abbreviation symbol as a line */
        if ( addline(curbp, cur_node->ab_sym) != TRUE )
            return (FALSE);

        /* now a line with the expansion */
        if ( addline(curbp, cur_node->ab_exp) != TRUE )
            return (FALSE);

        cur_node = cur_node->ab_next;
    }

    return (TRUE);
}

/* DEF_ABBREVS:
 */
int PASCAL NEAR def_abbrevs P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER BUFFER *bp;        /* ptr to buffer to dump */
    REGISTER LINE *lp;          /* ptr to current line in our buffer */
    REGISTER int llength;       /* length of the current line being examined */
    char cur_sym[MAXSYM+1];     /* current symbol being defined */
    char cur_exp[NSTRING];      /* current expansion */

    /* get the buffer to load abbreviations from */
    bp = getdefb();
    bp = getcbuf(TEXT236, bp ? bp->b_bname : mainbuf, TRUE);
/*           "Define Abbreviations in buffer" */
    if ( bp == NULL )
        return (ABORT);

    /* step throught the buffer */
    lp = lforw(bp->b_linep);
    while ( lp != bp->b_linep ) {

        /* get a symbol name */
        llength = get_lused(lp);
        if ( llength > MAXSYM )
            llength = MAXSYM;
        xstrncpy(cur_sym, ltext(lp), llength);
        cur_sym[llength] = 0;

        /* advance to the next line in the buffer */
        lp = lforw(lp);
        if ( lp == bp->b_linep )
            break;

        /* and an expansion for that symbol */
        llength = get_lused(lp);
        if ( llength > MAXSYM )
            llength = MAXSYM;
        xstrncpy(cur_exp, ltext(lp), llength);
        cur_exp[llength] = 0;

        /* add it to the current abbreviation list */
        ab_insert(cur_sym, cur_exp);

        /* on to the next pair */
        lp = lforw(lp);
    }

    return TRUE;
}

/* AB_INIT:
 */
VOID PASCAL NEAR ab_init P0_()
{
    ab_head   = (ABBREV *)NULL;         /* abbreviation list empty */
    ab_bell   = FALSE;                  /* no ringing please! */
    ab_cap    = FALSE;                  /* don't match capatilization on
                                         * expansion */
    ab_full   = FALSE;                  /* no full expansion */
    ab_quick  = FALSE;                  /* no aggressive expansion */
    ab_pos    = ab_word;                /* no word accumulated yet */
    ab_end    = &ab_word[NELEM(ab_word) - 1]; /* ptr to detect end of
                                               * this buffer  */
}

/* AB_INSERT:
 *
 * Insert a <sym> in the abbreviation list defined as <expansion>
 */
int PASCAL NEAR ab_insert P2_(char *, sym, CONST char *, expansion)
/* sym:       Symbol to expand    */
/* expansion: String to expand to */
{
    ABBREV *new_node;           /* pointer to the newly allocated node */
    ABBREV *cur_node;           /* pointer to travel down list */

    /* nothing longer than MAXSYM please */
    if ( STRLEN(sym) > MAXSYM )
        sym[MAXSYM + 1] = 0;

    /* is this already defined? */
    if ( ab_lookup(sym) != NULL )
        ab_delete(sym);

    /* allocate a new node to hold abbreviation */
    new_node = (ABBREV *)ROOM(SIZEOF(ABBREV) + STRLEN(expansion) +1);
    if ( new_node == NULL )
        return (FALSE);

    /* copy data to that node */
    XSTRCPY(new_node->ab_sym, sym);
    /* new_node->ab_exp has faked length: Use `xstrcpy' (safe here) */
    xstrcpy(new_node->ab_exp, expansion);

    /* do we have an empty list */
    if ( ab_head == NULL ) {

        ab_head = new_node;
        new_node->ab_next = NULL;

    } else {

        /* does our new node go before the first */
        if ( strcmp(sym, ab_head->ab_sym) < 0 ) {

            /* insert the node before the first node */
            new_node->ab_next = ab_head;
            ab_head = new_node;

        } else {

            /* search for the right place to insert */
            cur_node = ab_head;
            while ( cur_node->ab_next != NULL ) {

                if ( strcmp(sym, cur_node->ab_next->ab_sym) > 0 ) {

                    /* insert after cur_node */
                    new_node->ab_next = cur_node->ab_next;
                    cur_node->ab_next = new_node;

                    return (TRUE);
                }

                /* step to the next node */
                cur_node = cur_node->ab_next;
            }

            /* insert after the last node */
            cur_node->ab_next = new_node;
            new_node->ab_next = NULL;
        }
    }

    return (TRUE);
}

/* AB_LOOKUP:
 *
 * Look up and return the expansion of <sym>. Return a NULL if it is
 * not in the list
 */
char *PASCAL NEAR ab_lookup P1_(CONST char *, sym)
/* sym: Name of the symbol to look up */
{
    ABBREV  *cur_node = NULL; /* ptr to look through list */

    /* starting at the head, step through the list */
    cur_node = ab_head;
    while ( cur_node != NULL ) {
        /* if there is a match, return the expansion */
        if ( strcmp(sym, cur_node->ab_sym) == 0 ) {
            return (cur_node->ab_exp);
        }
        cur_node = cur_node->ab_next;
    }

    /* at the end, return NULL */
    return (NULL);
}

/* AB_TAILLOOKUP:
 *
 * Look up and return the expansion of tails of <sym>. Return a NULL
 * if there is no match: In sym try matches starting from 0 to
 * STRLEN(sym) - 1.
 */
char *PASCAL NEAR ab_taillookup P1_(CONST char *, sym)
/* sym: Name of the symbol to look up */
{
    if ( ! sym && *sym )  {
        return NULL;
    }

    {
        int         l     = 0;
        int         i     = 0;
        char        *exp  = NULL;
        static char rcbuf[NSTRING];

        ZEROMEM(rcbuf);

        l = STRLEN(sym);
        for (i = 0; i < l; i++) {
            if ( NULL != (exp = ab_lookup(sym + i)) ) {
                xstrlcpy(rcbuf, sym, MIN2(SIZEOF(rcbuf), i + 1));
                xstrlcat(rcbuf, exp, SIZEOF(rcbuf));

                return rcbuf;
            }
        }

        return NULL;
    }
}

/* AB_DELETE:
 *
 * Delete <sym> from the abbreviation list
 */
int PASCAL NEAR ab_delete P1_(CONST char *, sym)
{

    ABBREV *cur_node, *previous;        /* ptr to look through list */

    /* start at beginning */
    previous=NULL;
    cur_node=ab_head;

    /* step through the list */
    while ( cur_node!=NULL ) {

        /* if there is a match, delete the node */
        if ( previous == NULL && strcmp(sym, cur_node->ab_sym) == 0 ) {

            /*important: resets our head pointer*/
            ab_head=cur_node->ab_next;
            CLROOM(cur_node);

            return (TRUE);

        } else if ( strcmp(sym, cur_node->ab_sym) == 0&& cur_node != NULL ) {
            previous->ab_next=NULL;
            CLROOM(cur_node);

            return (TRUE);
        }

        /*makes sure our previous pointer steps with cur_node*/
        previous = cur_node;
        cur_node = cur_node->ab_next;
    }

    /* at the end, no match to delete */
    return (FALSE);
}

/* AB_CLEAN:
 */
int PASCAL NEAR ab_clean P0_()
{

    ABBREV *cur_node;           /* ptr to look through list */
    ABBREV *next;               /* ptr to next abbreviation */

    /* start at the beginning, */
    cur_node = ab_head;

    /* cycle through the list */
    while ( cur_node != (ABBREV *)NULL ) {
        next = cur_node->ab_next;
        CLROOM(cur_node);
        cur_node = next;
    }

    /* and re-init the list */
    ab_head = (ABBREV *)NULL;

    return (TRUE);
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
