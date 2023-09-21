/*======================================================================
 *      CHAR.C: Character handling functions for
 *              MicroEMACS 4.00
 *              (C)Copyright 1995 by Daniel Lawrence
 *
 *              ALL THE CODE HERE IS FOR VARIOUS FORMS OF ASCII AND
 *              WILL HAVE TO BE MODIFIED FOR EBCDIC
 *====================================================================*/

/*====================================================================*/
#define CHAR_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include        <stdio.h>
#include        <stdlib.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

/* IS_LETTER:
 *
 * Is the character a letter?  We presume a letter must be either
 * in the upper or lower case tables (even if it gets translated
 * to itself).
 */
int PASCAL NEAR is_letter P1_(char, ch)
{
    return ( is_upper(ch) || is_lower(ch) );
}

/* IS_LOWER:
 *
 * Is the character a lower case letter?  This looks in the
 * lower to uppercase translation table.
 */
int PASCAL NEAR is_lower P1_(char, ch)
{
    return (lowcase[C2I(ch)] != 0);
}

/* IS_UPPER:
 *
 * Is the character a upper case letter?  This looks in the
 * upper to lowercase translation table.
 */
int PASCAL NEAR is_upper P1_(char, ch)
{
    return (upcase[C2I(ch)] != 0);
}

/* CHCASE:
 *
 * Change the case of the current character. First check lower and
 * then upper. If it is not a letter, it gets returned unchanged.
 */
unsigned int PASCAL NEAR chcase P1_(unsigned int, ch)
{
    /* translate lowercase */
    if ( is_lower(ch) )
        return (lowcase[C2I(ch)]);

    /* translate uppercase */
    if ( is_upper(ch) )
        return (upcase[C2I(ch)]);

    /* let the rest pass */
    return (ch);
}

/* UPPERCASE:
 *
 * Change *cp to an upper case character
 */
VOID PASCAL NEAR uppercase P1_(unsigned char *, cp)
/* cp:  Ptr to character to uppercase */
{
    /* translate uppercase */
    if ( is_lower(*cp) )
        *cp = lowcase[C2I(*cp)];
}

/* LOWERCASE:
 *
 * Change *cp to an lower case character
 */
VOID PASCAL NEAR lowercase P1_(unsigned char *, cp)
/* cp:  Ptr to character to lowercase */
{
    /* translate lowercase */
    if ( is_upper(*cp) )
        *cp = upcase[C2I(*cp)];
}

/* UPPERC:
 *
 * Return the upper case equivalant of character ch
 */
int PASCAL NEAR upperc P1_(char, ch)
{
    if ( is_lower(ch) )
        return (lowcase[C2I(ch)]);
    else
        return (ch);
}

/* LOWERC:
 *
 * Return the lower case equivalant of character ch
 */
int PASCAL NEAR lowerc P1_(char, ch)
{
    if ( is_upper(ch) )
        return (upcase[C2I(ch)]);
    else
        return (ch);
}

/* INITCHARS:
 *
 * Initialize the character upper/lower case tables
 */
VOID PASCAL NEAR initchars P0_()
{
    REGISTER int index;         /* index into tables */

    /* all of both tables to zero */
    for ( index = 0; index < HICHAR; index++ ) {
        lowcase[index] = 0;
        upcase[index] = 0;
    }

    /* lower to upper, upper to lower */
    for ( index = 'a'; index <= 'z'; index++ ) {
        lowcase[index] = index ^ DIFCASE;
        upcase[index ^ DIFCASE] = index;
    }
#if ( IS_UNIX() )
    /* and for those international characters! */
    for ( index = (unsigned char)'\340';
          index <= (unsigned char)'\375'; index++ ) {
        lowcase[index] = index ^ DIFCASE;
        upcase[index ^ DIFCASE] = index;
    }
#endif

#if     MSDOS
    /* setup various extended IBM-PC characters */
    upcase[0x80]  = 0x87;       /* C with a cedilla */
    lowcase[0x81] = 0x9a;       /* U with an umlaut */
    lowcase[0x82] = 0x90;       /* E with an acute accent */
    lowcase[0x83] = 0x83;       /* A with a circumflex */
    lowcase[0x84] = 0x8e;       /* A with an umlaut */
    lowcase[0x85] = 0x85;       /* A with a grave accent */
    lowcase[0x86] = 0x8f;       /* A with a circle */
    lowcase[0x87] = 0x80;       /* C with a cedilla */
    lowcase[0x88] = 0x88;       /* E with a circumflex */
    lowcase[0x89] = 0x89;       /* E with an umlaut */
    lowcase[0x8a] = 0x8a;       /* E with a grave accent */
    lowcase[0x8b] = 0x8b;       /* I with an umlaut */
    lowcase[0x8c] = 0x8c;       /* I with a circumflex */
    lowcase[0x8d] = 0x8d;       /* I with a grave accent */
    upcase[0x8e]  = 0x84;       /* A with an umlaut */
    upcase[0x8f]  = 0x86;       /* A with a circle */
    upcase[0x90]  = 0x82;       /* E with an acute accent */
    lowcase[0x91] = 0x92;       /* AE diphthong */
    upcase[0x92]  = 0x91;       /* AE diphthong */
    lowcase[0x93] = 0x93;       /* O with a circumflex */
    lowcase[0x94] = 0x99;       /* O with an umlaut */
    lowcase[0x95] = 0x95;       /* O with an acute accent */
    lowcase[0x96] = 0x96;       /* u with a circumflex */
    lowcase[0x97] = 0x97;       /* U with an grave accent */
    lowcase[0x98] = 0x98;       /* y with an umlaut */
    upcase[0x99]  = 0x94;       /* O with an umlaut */
    upcase[0x9a]  = 0x81;       /* U with an umlaut */
    lowcase[0xa0] = 0xa0;       /* a with an acute accent */
    lowcase[0xa1] = 0xa1;       /* i with an acute accent */
    lowcase[0xa2] = 0xa2;       /* o with an acute accent */
    lowcase[0xa3] = 0xa3;       /* u with an acute accent */
    lowcase[0xa4] = 0xa5;       /* n with a tilde */
    upcase[0xa5]  = 0xa4;       /* N with a tilde */
#endif
#if     VMS
    /* setup DEC Multinational Character Set */
    upcase[ 192]  = 224;        /* A with a grave accent */
    upcase[ 193]  = 225;        /* A with an acute accent */
    upcase[ 194]  = 226;        /* A with a circumflex */
    upcase[ 195]  = 227;        /* A with a tilde */
    upcase[ 196]  = 228;        /* A with an umlaut */
    upcase[ 197]  = 229;        /* A with a ring */
    upcase[ 198]  = 230;        /* AE diphthong */
    upcase[ 199]  = 231;        /* C with a cedilla */
    upcase[ 200]  = 232;        /* E with a grave accent */
    upcase[ 201]  = 233;        /* E with an acute accent */
    upcase[ 202]  = 234;        /* E with circumflex */
    upcase[ 203]  = 235;        /* E with an umlaut */
    upcase[ 204]  = 236;        /* I with a grave accent */
    upcase[ 205]  = 237;        /* I with an acute accent */
    upcase[ 206]  = 238;        /* I with circumflex */
    upcase[ 207]  = 239;        /* I with an umlaut */
    upcase[ 209]  = 241;        /* N with a tilde */
    upcase[ 210]  = 242;        /* O with a grave accent */
    upcase[ 211]  = 243;        /* O with an acute accent */
    upcase[ 212]  = 244;        /* O with circumflex */
    upcase[ 213]  = 245;        /* O with a tilde */
    upcase[ 214]  = 246;        /* O with an umlaut */
    upcase[ 215]  = 247;        /* OE ligature */
    upcase[ 216]  = 248;        /* O with a slash */
    upcase[ 217]  = 249;        /* U with a grave accent */
    upcase[ 218]  = 250;        /* U with an acute accent */
    upcase[ 219]  = 251;        /* U with circumflex */
    upcase[ 220]  = 252;        /* U with an umlaut */
    upcase[ 221]  = 253;        /* Y with an umlaut */

    lowcase[ 223]  = 223;       /* German lowercase sharp s */

    lowcase[ 224]  = 192;       /* a with a grave accent */
    lowcase[ 225]  = 193;       /* a with an acute accent */
    lowcase[ 226]  = 194;       /* a with a circumflex */
    lowcase[ 227]  = 195;       /* a with a tilde */
    lowcase[ 228]  = 196;       /* a with an umlaut */
    lowcase[ 229]  = 197;       /* a with a ring */
    lowcase[ 230]  = 198;       /* ae diphthong */
    lowcase[ 231]  = 199;       /* c with a cedilla */
    lowcase[ 232]  = 200;       /* e with a grave accent */
    lowcase[ 233]  = 201;       /* e with an acute accent */
    lowcase[ 234]  = 202;       /* e with circumflex */
    lowcase[ 235]  = 203;       /* e with an umlaut */
    lowcase[ 236]  = 204;       /* i with a grave accent */
    lowcase[ 237]  = 205;       /* i with an acute accent */
    lowcase[ 238]  = 206;       /* i with circumflex */
    lowcase[ 239]  = 207;       /* i with an umlaut */
    lowcase[ 241]  = 209;       /* n with a tilde */
    lowcase[ 242]  = 210;       /* o with a grave accent */
    lowcase[ 243]  = 211;       /* o with an acute accent */
    lowcase[ 244]  = 212;       /* o with circumflex */
    lowcase[ 245]  = 213;       /* o with a tilde */
    lowcase[ 246]  = 214;       /* o with an umlaut */
    lowcase[ 247]  = 215;       /* oe ligature */
    lowcase[ 248]  = 216;       /* o with a slash */
    lowcase[ 249]  = 217;       /* u with a grave accent */
    lowcase[ 250]  = 218;       /* u with an acute accent */
    lowcase[ 251]  = 219;       /* u with circumflex */
    lowcase[ 252]  = 220;       /* u with an umlaut */
    lowcase[ 253]  = 221;       /* y with an umlaut */

#endif
}

/* SETLOWER:
 *
 * Set a character in the lowercase map
 */
int PASCAL NEAR setlower P2_(char *, ch, char *, val)
/* ch:  Ptr to character to set */
/* val: Value to set it to      */
{
    return (lowcase[C2I(*ch)] = C2I(*val));
}

/* SETUPPER:
 *
 * Set a character in the uppercase map
 */
int PASCAL NEAR setupper P2_(char *, ch, char *, val)
/* ch:  Ptr to character to set */
/* val: Value to set it to      */
{
    return (upcase[C2I(*ch)] = C2I(*val));
}

#if (WINXP | ZTC | TURBO | MSC) == 0

/* STRREV:
 *
 * Reverse string in place.  Code here for those compilers that do not
 * have the function in their own library.
 */
char *strrev P1_(char *, our_str)
{
    REGISTER char   *beg_str, *end_str;
    REGISTER char the_char;

    end_str = beg_str = our_str;
    end_str += STRLEN(beg_str);

    do {
        the_char = *--end_str;
        *end_str = *beg_str;
        *beg_str++ = the_char;
    } while (end_str > beg_str);

    return (our_str);
}

#endif

#if     DBCS

/* IS2BYTE:
 *
 * is this character a 2 byte character prefix code?
 */
int PASCAL NEAR is2byte P2_(char *, sp, char *, cp)
/* sp:  Ptr to beginning of string containing character to test */
/* cp:  Ptr to character to test                                */
{
    REGISTER char *cc;          /* pointer to current character */

    cc = sp;
    while ( *cc ) {
        if ( cc > cp )
            return (FALSE);

        if ( cc == cp )
            return ( is2char(*cp) );

        if ( is2char(*cc) )
            ++cc;
        ++cc;
    }

    return (FALSE);
}
#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
