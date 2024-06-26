/*======================================================================
 *
 * KEYBOARD.C for MSDOS and OS/2
 *
 * extcode()
 *
 * This routine is common to the MSDOS and OS/2 implementations. It is
 * used to * resolve extended character codes from the keyboard into
 * EMACS printable * character specifications.
 *
 * This implementation can handle the extended AT-style keyboard if one
 * is * fitted.
 *
 * I don't know what happens on an XT but I suspect that it should work
 * as if * there were no extended keys.
 *
 * Jon Saxton 24 Jan 1990
 *
 *====================================================================*/

/*====================================================================*/
#define KEYBOARD_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include <stdio.h>
#include "estruct.h"
#include "eproto.h"
#include "edef.h"
#include "elang.h"


#if ( MSDOS || OS2 )
# if ( !HP150 )

int PASCAL NEAR extcode P1_(unsigned, c)
/* c: byte following a zero extended char byte
 *    High order normally contains 0x00 but may contain 0xE0 if an
 *    AT-style keyboard is attached
 */
{
    /* function keys 1 through 9.  High-order will be 0x00 */
    if ( c >= 0x3B && c < 0x44 )
        return (SPEC | c - 0x3A + '0');

    /* function key 10 */
    if ( c == 0x44 )
        return (SPEC | '0');

    /* shifted function keys */
    if ( c >= 0x54 && c < 0x5D )
        return (SPEC | SHFT | c - 0x53 + '0');

    if ( c == 0x5D )
        return (SPEC | SHFT | '0');

    /* control function keys */
    if ( c >= 0x5E && c < 0x67 )
        return (SPEC | CTRF | c - 0x5D + '0');

    if ( c == 0x67 )
        return (SPEC | CTRF | '0');

    /* ALTed function keys */
    if ( c >= 0x68 && c < 0x71 )
        return (SPEC | ALTD | c - 0x67 + '0');

    if ( c == 0x71 )
        return (SPEC | ALTD | '0');

    /*
     * ALTed number keys
     *
     * This doesn't work for DOS or OS/2.  Using ALT in conjunction with the
     * number keys lets you enter any (decimal) character value you want.  It is
     * therefore commented out.
     *
     *  Wrongo joker... alting the top row of numbers works fine DML
     */
    if ( c >= 0x78 && c < 0x81 )
        return (ALTD | c - 0x77 + '0');

    if ( c == 0x81 )
        return (ALTD | '0');

    /* some others as well */
    switch ( c ) {

    case 3:
        return (0);                     /* null           */

    case 0x0F:
        return (SHFT | CTRF | 'I');     /* backtab        */

    case 0x10:
        return (ALTD | 'Q');

    case 0x11:
        return (ALTD | 'W');

    case 0x12:
        return (ALTD | 'E');

    case 0x13:
        return (ALTD | 'R');

    case 0x14:
        return (ALTD | 'T');

    case 0x15:
        return (ALTD | 'Y');

    case 0x16:
        return (ALTD | 'U');

    case 0x17:
        return (ALTD | 'I');

    case 0x18:
        return (ALTD | 'O');

    case 0x19:
        return (ALTD | 'P');

    case 0x1E:
        return (ALTD | 'A');

    case 0x1F:
        return (ALTD | 'S');

    case 0x20:
        return (ALTD | 'D');

    case 0x21:
        return (ALTD | 'F');

    case 0x22:
        return (ALTD | 'G');

    case 0x23:
        return (ALTD | 'H');

    case 0x24:
        return (ALTD | 'J');

    case 0x25:
        return (ALTD | 'K');

    case 0x26:
        return (ALTD | 'L');

    case 0x2C:
        return (ALTD | 'Z');

    case 0x2D:
        return (ALTD | 'X');

    case 0x2E:
        return (ALTD | 'C');

    case 0x2F:
        return (ALTD | 'V');

    case 0x30:
        return (ALTD | 'B');

    case 0x31:
        return (ALTD | 'N');

    case 0x32:
        return (ALTD | 'M');

    case 0x47:
        return (SPEC | '<');            /* home                   */

    case 0x48:
        return (SPEC | 'P');            /* cursor up              */

    case 0x49:
        return (SPEC | 'Z');            /* page up                */

    case 0x4B:
        return (SPEC | 'B');            /* cursor left            */

    case 0x4C:
        return (SPEC | 'L');            /* NP 5                   */

    case 0x4D:
        return (SPEC | 'F');            /* cursor right           */

    case 0x4F:
        return (SPEC | '>');            /* end                    */

    case 0x50:
        return (SPEC | 'N');            /* cursor down            */

    case 0x51:
        return (SPEC | 'V');            /* page down              */

    case 0x52:
        return (SPEC | 'C');            /* insert                 */

    case 0x53:
        return (SPEC | 'D');            /* delete                 */

    case 0x73:
        return (SPEC | CTRF | 'B');     /* control left           */

    case 0x74:
        return (SPEC | CTRF | 'F');     /* control right          */

    case 0x75:
        return (SPEC | CTRF | '>');     /* control end            */

    case 0x76:
        return (SPEC | CTRF | 'V');     /* control page down      */

    case 0x77:
        return (SPEC | CTRF | '<');     /* control home           */

    case 0x84:
        return (SPEC | CTRF | 'Z');     /* control page up        */

    case 0x8D:
        return (SPEC | CTRF | 'P');     /* control up             */

    case 0x8F:
        return (SPEC | CTRF | 'L');     /* control NP5            */

    case 0x91:
        return (SPEC | CTRF | 'N');     /* control down           */

    case 0x92:
        return (SPEC | CTRF | 'C');     /* control grey insert    */

    case 0x93:
        return (SPEC | CTRF | 'D');     /* control grey delete    */

    case 0x82:
        return (ALTD | '-');            /* alt -                  */

    case 0x83:
        return (ALTD | '=');            /* alt =                  */

    case 0x27:
        return (ALTD | ';');            /* alt ;                  */

    case 0x28:
        return (ALTD | '\'');           /* alt '                  */

    case 0x2B:
        return (ALTD | '\\');           /* alt \                  */

    case 0x1A:
        return (ALTD | '[');            /* alt [                  */

    case 0x1B:
        return (ALTD | ']');            /* alt ]                  */

#  if ( ATKBD || OS2 )

/* F11 and F12 */

    case 0x85:
        return (SPEC | '-');            /* F11                    */

    case 0x86:
        return (SPEC | '=');            /* F12                    */

    case 0x87:
        return (SPEC | SHFT | '-');     /* shift F11              */

    case 0x88:
        return (SPEC | SHFT | '=');     /* shift F12              */

    case 0x89:
        return (SPEC | CTRF | '-');     /* control F11            */

    case 0x8A:
        return (SPEC | CTRF | '=');     /* control F12            */

    case 0x8B:
        return (SPEC | ALTD | '-');     /* alt F11                */

    case 0x8C:
        return (SPEC | ALTD | '=');     /* alt F12                */

/*
 * This implementation distinguishes between the cursor controls on the
 * number * pad and those on the grey keypad if an AT-style keyboard is
 * fitted.
 */

    case 0xE047:
        return (SPEC | 'a');            /* grey home              */

    case 0xE048:
        return (SPEC | 'b');            /* grey cursor up         */

    case 0xE049:
        return (SPEC | 'c');            /* grey page up           */

    case 0xE04B:
        return (SPEC | 'd');            /* grey cursor left       */

    case 0xE04C:
        return (SPEC | 'e');            /* grey center key        */

    case 0xE04D:
        return (SPEC | 'f');            /* grey cursor right      */

    case 0xE04F:
        return (SPEC | 'g');            /* grey end               */

    case 0xE050:
        return (SPEC | 'h');            /* grey cursor down       */

    case 0xE051:
        return (SPEC | 'i');            /* grey page down         */

    case 0xE052:
        return (SPEC | 'j');            /* grey insert            */

    case 0xE053:
        return (SPEC | 'k');            /* grey delete            */

    case 0xE077:
        return (SPEC | CTRF | 'a');     /* control grey home      */

    case 0xE08D:
        return (SPEC | CTRF | 'b');     /* control grey up        */

    case 0xE084:
        return (SPEC | CTRF | 'c');     /* control grey page up   */

    case 0xE073:
        return (SPEC | CTRF | 'd');     /* control grey left      */

    case 0xE074:
        return (SPEC | CTRF | 'f');     /* control grey right     */

    case 0xE075:
        return (SPEC | CTRF | 'g');     /* control grey end       */

    case 0xE091:
        return (SPEC | CTRF | 'h');     /* control grey down      */

    case 0xE076:
        return (SPEC | CTRF | 'i');     /* control grey page down */

    case 0xE092:
        return (SPEC | CTRF | 'j');     /* control grey insert    */

    case 0xE093:
        return (SPEC | CTRF | 'k');     /* control grey delete    */

    case 0xE097:
        return (SPEC | ALTD | 'a');     /* alt grey home          */

    case 0xE098:
        return (SPEC | ALTD | 'b');     /* alt grey cursor up     */

    case 0xE099:
        return (SPEC | ALTD | 'c');     /* alt grey page up       */

    case 0xE09B:
        return (SPEC | ALTD | 'd');     /* alt grey cursor left   */

    case 0xE09D:
        return (SPEC | ALTD | 'f');     /* alt grey cursor right  */

    case 0xE09F:
        return (SPEC | ALTD | 'g');     /* alt grey end           */

    case 0xE0A0:
        return (SPEC | ALTD | 'h');     /* alt grey cursor down   */

    case 0xE0A1:
        return (SPEC | ALTD | 'i');     /* alt grey page down     */

    case 0xE0A2:
        return (SPEC | ALTD | 'j');     /* alt grey insert        */

    case 0xE0A3:
        return (SPEC | ALTD | 'k');     /* alt grey delete        */

    case 0x97:
        return (SPEC | ALTD | 'a');     /* alt grey home          */

    case 0x98:
        return (SPEC | ALTD | 'b');     /* alt grey cursor up     */

    case 0x99:
        return (SPEC | ALTD | 'c');     /* alt grey page up       */

    case 0x9B:
        return (SPEC | ALTD | 'd');     /* alt grey cursor left   */

    case 0x9D:
        return (SPEC | ALTD | 'f');     /* alt grey cursor right  */

    case 0x9F:
        return (SPEC | ALTD | 'g');     /* alt grey end           */

    case 0xA0:
        return (SPEC | ALTD | 'h');     /* alt grey cursor down   */

    case 0xA1:
        return (SPEC | ALTD | 'i');     /* alt grey page down     */

    case 0xA2:
        return (SPEC | ALTD | 'j');     /* alt grey insert        */

    case 0xA3:
        return (SPEC | ALTD | 'k');     /* alt grey delete        */

    case 0xA6:
        return (SPEC | ALTD | 'l');     /* alt grey enter         */

    case 0xA4:
        return (SPEC | ALTD | '/');     /* alt grey /             */

    case 0x37:
        return (SPEC | ALTD | '*');     /* alt grey *             */

    case 0x4A:
        return (SPEC | ALTD | '-');     /* alt grey -             */

    case 0x4E:
        return (SPEC | ALTD | '+');     /* alt grey +             */

    case 0x95:
        return (SPEC | CTRF | '/');     /* ctrl grey /            */

    case 0x96:
        return (SPEC | CTRF | '*');     /* ctrl grey *            */

    case 0x8E:
        return (SPEC | CTRF | '-');     /* ctrl grey -            */

    case 0x90:
        return (SPEC | CTRF | '+');     /* ctrl grey +            */

#  endif

    }
    /**printf("[ALT %d] ", c);**/

    return (ALTD | c);
}

# endif
#endif

#if ( IS_UNIX() || VMS  || MPE )

# define NKEYSEQ  (300) /* Number of keymap entries */

typedef struct keyent {                 /* Key mapping entry          */
    struct keyent *samlvl;              /* Character on same level    */
    struct keyent *nxtlvl;              /* Character on next level    */
    unsigned char ch;                   /* Character                  */
    int           code;                 /* Resulting keycode          */
} KEYENT;

/* Needed Prototype */
EXTERN int PASCAL NEAR rec_seq DCL((char *buf, char *bufstart, KEYENT *node));

/* some globals needed here */
/*  Prefix escape sequence table:                                     */
static unsigned char  keyseq[(int)(unsigned char)(-1) + 1];
static KEYENT         keymap[NKEYSEQ];  /* Key map                    */
static KEYENT         *nxtkey = keymap; /* Next free key entry        */
static BUFFER         *seqbuf;          /* For the pop-up buffer      */

/* add-keymap "escape sequence" keyname
 */
int PASCAL NEAR addkeymap P2_(int, f, int, n)
{
    int c   = 0;
    int ec  = 0;
    int idx = 0;
    int col = 0;
    char esc_seq[NSTRING];      /* escape sequence to cook  */
    char codeseq[NSTRING];      /* fn key name              */

    ZEROMEM(esc_seq); /* NECESSARY  */
    ZEROMEM(codeseq);

    if ( clexec == TRUE ) {
        if ( mlreply(NULL, esc_seq, NSTRING) != TRUE ) {
            TTbeep();

            return FALSE;
        }
    } else {
        /* get the key sequence */
        mlwrite(": add-keymap ");
        col = STRLEN(": add-keymap ");
        idx = 0;
        for (;; ) {
            c = tgetc();
            if ( ( ec = ctoec(c) ) == abortc ) {
                TTbeep();

                return FALSE;
            } else if ( c == '\r' ) {
                break;
            } else if ( ec == quotec ) {
                c = tgetc();
            }
            esc_seq[idx++] = c;
            movecursor(term.t_nrow, col); /* Position the cursor  */
            col += echochar(c);
        }

        ostring(" ");
    }

    if ( mlreply(NULL, codeseq, NSTRING) != TRUE ) {    /* find the key name
                                                         * (e.g., S-FN#) */
        TTbeep();

        return FALSE;
    }

    ec = stock(codeseq);

    ostring(codeseq);

    return ( addkey((unsigned char *)esc_seq, ec) );  /* Add to tree */
}

/* list-keymappings
 */
int PASCAL NEAR listkeymaps P2_(int, f, int, n)
{
    char outseq[NSTRING]; /* output buffer for key sequence */

    ZEROMEM(outseq);

    /*
     * Get a buffer for it.
     */
    seqbuf = bfind("Key sequence list", TRUE, BFINVS);

    if ( seqbuf == NULL || bclear(seqbuf) == FALSE ) {
        mlwrite("Cannot display key sequences list");

        return (FALSE);
    }

    /*
     * Build the list, pop it if all went well.
     */
    *outseq = '"';
    if ( rec_seq(outseq + 1, outseq, keymap) == TRUE ) {
        wpopup(seqbuf);
        mlerase();

        return (TRUE);
    }

    return FALSE;
}

/* recursively track through the tree, finding the escape sequences and their
 * function name equivalents.
 */
int PASCAL NEAR rec_seq P3_(char *, buf, char *, bufstart, KEYENT *, node)
{
    if ( node == NULL )
        return TRUE;

    *buf = node->ch;

    if ( node->nxtlvl == NULL ) {
        *(buf + 1) = '"';
        *(buf + 2) = '\0';
        pad(bufstart, 20);
        cmdstr(node->code, bufstart + 20);
        if ( addline(seqbuf, bufstart) != TRUE )
            return FALSE;
    } else if ( rec_seq(buf + 1, bufstart, node->nxtlvl) != TRUE )
        return FALSE;

    return ( rec_seq(buf, bufstart, node->samlvl) );
}

/* addkey --- Add key to key map
 *
 * Adds a new escape sequence to the sequence table. I am not going to
 * try to explain this table to you in detail. However, in short, it
 * creates a tree which can easily be transversed to see if input is in
 * a sequence which can be translated to a function key (arrows and
 * find/select/do etc. are treated like function keys).
 *
 * Replaces code in SMG.C, MPE.C, POSIX.C, and UNIX.C Nothing returned
 *
 *======================================================================
  typedef struct keyent {             /o Key mapping entry            o/
    struct keyent *samlvl;            /o Character on same level      o/
    struct keyent *nxtlvl;            /o Character on next level      o/
    unsigned char ch;                 /o Character                    o/
    int           code;               /o Resulting keycode            o/
  } KEYENT;

  ch:     Character in the character sequence seq
  code:   MicroEMACS keycode fn
  samlvl: Points to a KEYENT with a ch at the *same* position in seq as
          this one --- a horizontal arrow in the picture below.
  nxtlvl: Points to a KEYENT with a ch at the *next* position in seq
          --- a vertical arrow in the picture below.

  Step One: Start with seq = "ABC", fn = r

    A(r)
      |
      v
    B(r)
      |
      v
    C(r)


  Step Two: Add seq = "XY", fn = q

    A(r) --> X(q)
      |       |
      v       v
    B(r)     Y(q)
      |
      v
    C(r)


  Step Three: Add seq = "ABD", fn = s

    A(r) --------> X(q)
      |             |
      v             v
    B(r)           Y(q)
      |
      v
    C(r) --> D(s)


  Those KEYENT records with nxtlvl == NULL give the mapping between seq
  and fn, so the last picture could also be

    A(0) --------> X(0)
      |             |
      v             v
    B(0)           Y(q)
      |
      v
    C(r) --> D(s)


  Here U(x) --> R
        |
        v
        S

  means the KEYENT

  {
    R;    /o Character on same level      o/
    S;    /o Character on next level      o/
    U;    /o Character                    o/
    x;    /o Resulting keycode            o/
  }

 Overwriting does currently not work:
 - Longer sequences tries to override a shorther one:
   `if ( *seq == cur->ch )' chrashes because NULL == cur, which was set
   as `cur = cur->nxtlvl;' in the previous step.
 - Shorter sequences cannot overwrite longer ones.
 *======================================================================
 *
 * seq - character sequence fn  - Resulting keycode
 */
int PASCAL NEAR addkey P2_(unsigned char *, seq, int, fn)
{
    int     first       = 0;
    int     grow        = 0;    /* expand existing character sequence */
    KEYENT  *cur        = NULL;
    KEYENT  *nxtcur     = NULL;
    KEYENT  *lastmatch  = NULL;

    /* Skip on null sequences or single character sequences. */
    if ( seq == NULL || STRLEN( (char *)seq ) < 2 )
        return FALSE;

#  if ( 0 )
    TRC(("addkey(): seq = <%s>", (char *)seq));
#  endif
    /* If no keys defined, go directly to insert mode */
    first = !0;
    if ( nxtkey != keymap ) {
        /* Start at top of key map */
        cur = keymap;

        /* Loop until matches are exhausted */
        while ( *seq ) {
            /* Do we match current character */
            if ( *seq == cur->ch ) {
                first = 0;
                lastmatch = cur;
                /* Advance to next level */
                seq++;
                /* Try next character on next level */
                nxtcur = cur->nxtlvl;

                /* Stop if no more */
                if ( nxtcur ) {
                    cur = nxtcur;
                } else        {
                    grow = !0;  /* expand existing entry  */
                    break;
                }
            } else {
                /* Try next character on same level */
                nxtcur = cur->samlvl;

                /* Stop if no more */
                if ( nxtcur ) {
                    cur = nxtcur;
                } else        {
                    break;
                }
            }
        }

        /* truncate or update existing character sequence: */
        if ( !*seq )  {
            /* lastmatch is .NE. NULL because seq got only updated if
             * there was a match. */
            lastmatch->code   = fn;
            lastmatch->nxtlvl = NULL;

            return TRUE;
        }
    }

    /* Check for room in keymap */
    if ( STRLEN( (char *)seq ) > NKEYSEQ - (nxtkey - keymap) ) {
        mlwrite("No more room for key entries.");

        return FALSE;
    }

    /* If first character in sequence is inserted, add to prefix table */
    if ( first )
        keyseq[*seq] = 1;

    /* If characters are left over, insert them into list */
    for ( first = !0; *seq; first = 0 ) {
        /* Make new entry */
        nxtkey->ch = *seq++;
        nxtkey->code = fn;

        /* If root, nothing to do */
        if ( nxtkey != keymap ) {
            /* In this case cur is .NE. NULL. */
            /* Set first to samlvl, others to nxtlvl */
            if ( first && !grow )
                cur->samlvl = nxtkey;
            else
                cur->nxtlvl = nxtkey;
        }

        /* Advance to next key */
        ASRT(nxtkey - keymap < NELEM(keymap));
        cur = nxtkey++;
    }

    return TRUE;
}

/* Cook input characters, using the key sequences stored by addkey().
 *
 * To use, we need a grabwait(), grabnowait(), qin() and qrep() function.
 */
VOID cook P0_()
{
    REGISTER unsigned char  ch    = '\0';
    KEYENT                  *cur  = NULL;

    qin( ch = grabwait() ); /* Get first character untimed */

    /*
     * Skip if the key isn't a special leading escape sequence.
     */
    if ( keyseq[ch] == 0 ) {
        /* But if it is a '\0', make it a (0/1/32). */
        if ( ch == 0 ) {
            qin(CTRF >> 8); /* control */
            qin(32); /* space */
        }

        return;
    }

    /* Start at root of keymap */
    cur = keymap;

    /* Loop until keymap exhausts */
    while ( cur ) {
        /* Did we find a matching character */
        if ( cur->ch == ch ) {
            /* Is this the end */
            if ( cur->nxtlvl == NULL ) {
                /* Replace all characters with a new sequence */
                qrep(cur->code);

                return;
            } else {
                /* Advance to next level */
                cur = cur->nxtlvl;

                /* Get next character, timed */
                ch = grabnowait();
                if ( ch == grabnowait_TIMEOUT ) {
                    return;
                }

                /* Queue character */
                qin(ch);
            }
        } else {
            /* Try next character on same level */
            cur = cur->samlvl;
        }
    }
}

/* Cook input characters, using the key sequences stored by addkey().
 *
 * To use, we need a grabwait(), grabnowait(), qin() and qrep() function.
 */
int cook_nowait P0_()
{
    REGISTER unsigned char  ch    = '\0';
    KEYENT                  *cur  = NULL;

    if ( grabnowait_TIMEOUT == (ch = grabnowait()) )  {
        return FALSE;
    } else {
        qin(ch);
    }

    /*
     * Skip if the key isn't a special leading escape sequence.
     */
    if ( keyseq[ch] == 0 ) {
        /* But if it is a '\0', make it a (0/1/32). */
        if ( ch == 0 ) {
            qin(CTRF >> 8); /* control */
            qin(32); /* space */
        }

        return TRUE;
    }

    /* Start at root of keymap */
    cur = keymap;

    /* Loop until keymap exhausts */
    while ( cur ) {
        /* Did we find a matching character */
        if ( cur->ch == ch ) {
            /* Is this the end */
            if ( cur->nxtlvl == NULL ) {
                /* Replace all characters with a new sequence */
                qrep(cur->code);

                return TRUE;
            } else {
                /* Advance to next level */
                cur = cur->nxtlvl;

                /* Get next character, timed */
                ch = grabnowait();
                if ( ch == grabnowait_TIMEOUT ) {
                    return TRUE;
                }

                /* Queue character */
                qin(ch);
            }
        } else {
            /* Try next character on same level */
            cur = cur->samlvl;
        }
    }

    return TRUE;
}
#endif  /* ( IS_UNIX() || VMS || MPE ) */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
