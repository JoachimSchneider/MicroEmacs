/*======================================================================
 *      EVAR.H: Environment and user variable definitions
 *              for MicroEMACS
 *
 *              written 1993 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#ifndef EVAR_H_
#define EVAR_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#ifdef EVAL_C_


# define MAXVARS        (512)   /* Maximum number of global user variables */

/*  list of recognized environment variables    */

static NOSHARE CONST char *envars[] =
{
    "abbell",                   /* ring bell on abbreviation expansion?     */
    "abcap",                    /* match capitolization in expansions       */
    "abfull",                   /* full expansion? (exceeds abquick)        */
    "abquick",                  /* quick, aggressive expansions enabled?    */
    "acount",                   /* # of chars until next auto-save          */
    "asave",                    /* # of chars between auto-saves            */
    "bufhook",                  /* enter buffer switch hook                 */
    "cbflags",                  /* current buffer flags                     */
    "cbufname",                 /* current buffer name                      */
    "cfname",                   /* current file name                        */
    "cmdhook",                  /* command loop hook                        */
    "cmode",                    /* mode of current buffer                   */
    "cquote",                   /* close quote character                    */
    "curchar",                  /* current character under the cursor       */
    "curcol",                   /* current column pos of cursor             */
    "curline",                  /* current line in file                     */
    "curwidth",                 /* current screen width                     */
    "curwind",                  /* current window ordinal on current screen */
    "cwline",                   /* current screen line in window            */
    "debug",                    /* macro debugging                          */
    "deskcolor",                /* desktop color                            */
    "diagflag",                 /* diagonal mouse movements enabled?        */
    "discmd",                   /* display commands on command line         */
    "disinp",                   /* display command line input characters    */
    "disphigh",                 /* display high bit characters escaped      */
    "dispundo",                 /* display undo depth on command line       */
    "exbhook",                  /* exit buffer switch hook                  */
    "exithook",                 /* exiting emacs hook                       */
    "fcol",                     /* first displayed column in curent window  */
    "fillcol",                  /* current fill column                      */
    "flicker",                  /* flicker supression                       */
    "fmtlead",                  /* format command lead characters           */
    "gflags",                   /* global internal emacs flags              */
    "gmode",                    /* global modes                             */
    "hardtab",                  /* current hard tab size                    */
    "hilight",                  /* region # to hilight (255 to turn off)    */
                                /* BE CAREFUL: The corresponding case       */
                                /* code (EVHILITE) and C-variable (hilite)  */
                                /* are spelled differntly!                  */
    "hjump",                    /* horizontal screen jump size              */
    "hscrlbar",                 /* horizontal scroll bar flag               */
    "hscroll",                  /* horizontal scrolling flag                */
    "isterm",                   /* incremental-search terminator character  */
    "kill",                     /* kill buffer (read only)                  */
    "language",                 /* language of text messages                */
    "lastkey",                  /* last keyboard char struck                */
    "lastmesg",                 /* last string mlwrite()ed                  */
    "line",                     /* text of current line                     */
    "lterm",                    /* current line terminator for writes       */
    "lwidth",                   /* width of current line                    */
    "match",                    /* last matched magic pattern               */
    "mmove",                    /* mouse moves events style                 */
    "modeflag",                 /* Modelines displayed flag                 */
    "msflag",                   /* activate mouse?                          */
    "newscreen",                /* new screen with new buffer?              */
    "numwind",                  /* number of windows on current screen      */
    "oquote",                   /* open quote character                     */
    "orgcol",                   /* screen origin column                     */
    "orgrow",                   /* screen origin row                        */
    "os",                       /* what Operating System?                   */
    "overlap",                  /* line overlap when paging                 */
    "pagelen",                  /* number of lines used by editor           */
    "palette",                  /* current palette string                   */
    "paralead",                 /* paragraph leadin characters              */
    "parindent",                /* paragraph first line indent with fill    */
    "pending",                  /* type ahead pending flag                  */
    "popflag",                  /* pop-up windows active?                   */
    "popwait",                  /* user wait on end of pop-up window?       */
    "posflag",                  /* display point position on modeline?      */
    "progname",                 /* returns current prog name - "MicroEMACS" */
    "ram",                      /* ram in use by malloc                     */
    "readhook",                 /* read file execution hook                 */
    "region",                   /* current region (read only)               */
    "replace",                  /* replacement pattern                      */
    "rval",                     /* child process return value               */
    "scrname",                  /* current screen name                      */
    "search",                   /* search pattern                           */
    "searchpnt",                /* differing search styles (term point)     */
    "seed",                     /* current random number seed               */
    "softtab",                  /* current soft tab size                    */
    "sres",                     /* current screen resolution                */
    "ssave",                    /* safe save flag                           */
    "sscroll",                  /* smooth scrolling flag                    */
    "status",                   /* returns the status of the last command   */
    "sterm",                    /* search terminator character              */
    "target",                   /* target for line moves                    */
    "time",                     /* date and time                            */
    "timeflag",                 /* display time?                            */
    "tpause",                   /* length to pause for paren matching       */
    "undoflag",                 /* currently processing undos               */
    "version",                  /* current version number                   */
    "vscrlbar",                 /* vertical scroll bar flag                 */
    "wchars",                   /* set of characters legal in words         */
    "wline",                    /* # of lines in current window             */
    "wraphook",                 /* wrap word execution hook                 */
    "writehook",                /* write file hook                          */
    "xpos",                     /* current mouse X position                 */
    "yankflag",                 /* point placement at yanked/included text  */
    "ypos"                      /* current mouse Y position                 */
};

/*  and its preprocesor definitions     */

# define EVABBELL        (0)
# define EVABCAP         (1)
# define EVABFULL        (2)
# define EVABQUICK       (3)
# define EVACOUNT        (4)
# define EVASAVE         (5)
# define EVBUFHOOK       (6)
# define EVCBFLAGS       (7)
# define EVCBUFNAME      (8)
# define EVCFNAME        (9)
# define EVCMDHK        (10)
# define EVCMODE        (11)
# define EVCQUOTE       (12)
# define EVCURCHAR      (13)
# define EVCURCOL       (14)
# define EVCURLINE      (15)
# define EVCURWIDTH     (16)
# define EVCURWIND      (17)
# define EVCWLINE       (18)
# define EVDEBUG        (19)
# define EVDESKCLR      (20)
# define EVDIAGFLAG     (21)
# define EVDISCMD       (22)
# define EVDISINP       (23)
# define EVDISPHIGH     (24)
# define EVDISPUNDO     (25)
# define EVEXBHOOK      (26)
# define EVEXITHOOK     (27)
# define EVFCOL         (28)
# define EVFILLCOL      (29)
# define EVFLICKER      (30)
# define EVFMTLEAD      (31)
# define EVGFLAGS       (32)
# define EVGMODE        (33)
# define EVHARDTAB      (34)
# define EVHILITE       (35)
# define EVHJUMP        (36)
# define EVHSCRLBAR     (37)
# define EVHSCROLL      (38)
# define EVISTERM       (39)
# define EVKILL         (40)
# define EVLANG         (41)
# define EVLASTKEY      (42)
# define EVLASTMESG     (43)
# define EVLINE         (44)
# define EVLTERM        (45)
# define EVLWIDTH       (46)
# define EVMATCH        (47)
# define EVMMOVE        (48)
# define EVMODEFLAG     (49)
# define EVMSFLAG       (50)
# define EVNEWSCRN      (51)
# define EVNUMWIND      (52)
# define EVOQUOTE       (53)
# define EVORGCOL       (54)
# define EVORGROW       (55)
# define EVOS           (56)
# define EVOVERLAP      (57)
# define EVPAGELEN      (58)
# define EVPALETTE      (59)
# define EVPARALEAD     (60)
# define EVPARINDENT    (61)
# define EVPENDING      (62)
# define EVPOPFLAG      (63)
# define EVPOPWAIT      (64)
# define EVPOSFLAG      (65)
# define EVPROGNAME     (66)
# define EVRAM          (67)
# define EVREADHK       (68)
# define EVREGION       (69)
# define EVREPLACE      (70)
# define EVRVAL         (71)
# define EVSCRNAME      (72)
# define EVSEARCH       (73)
# define EVSEARCHPNT    (74)
# define EVSEED         (75)
# define EVSOFTTAB      (76)
# define EVSRES         (77)
# define EVSSAVE        (78)
# define EVSSCROLL      (79)
# define EVSTATUS       (80)
# define EVSTERM        (81)
# define EVTARGET       (82)
# define EVTIME         (83)
# define EVTIMEFLAG     (84)
# define EVTPAUSE       (85)
# define EVUNDOFLAG     (86)
# define EVVERSION      (87)
# define EVVSCRLBAR     (88)
# define EVWCHARS       (89)
# define EVWLINE        (90)
# define EVWRAPHK       (91)
# define EVWRITEHK      (92)
# define EVXPOS         (93)
# define EVYANKFLAG     (94)
# define EVYPOS         (95)

/*  list of recognized user functions   */

typedef struct UFUNC {
    CONST char  *f_name;  /* name of function         */
    int         f_type;   /* 1 = monamic, 2 = dynamic */
} UFUNC;

# define NILNAMIC       (0)
# define MONAMIC        (1)
# define DYNAMIC        (2)
# define TRINAMIC       (3)

static NOSHARE UFUNC  funcs[] =
{
    { "abbrev",   MONAMIC   },  /* look up abbreviation */
    { "abs",      MONAMIC   },  /* absolute value of a number */
    { "add",      DYNAMIC   },  /* add two numbers together */
    { "and",      DYNAMIC   },  /* logical and */
    { "ascii",    MONAMIC   },  /* char to integer conversion */
    { "band",     DYNAMIC   },  /* bitwise and   9-10-87  jwm */
    { "bind",     MONAMIC   },  /* loopup what function name is bound to a key */
    { "bnot",     MONAMIC   },  /* bitwise not */
    { "bor",      DYNAMIC   },  /* bitwise or    9-10-87  jwm */
    { "bxor",     DYNAMIC   },  /* bitwise xor   9-10-87  jwm */
    { "call",     MONAMIC   },  /* call a procedure */
    { "cat",      DYNAMIC   },  /* concatenate string */
    { "chr",      MONAMIC   },  /* integer to char conversion */
    { "divide",   DYNAMIC   },  /* division */
    { "env",      MONAMIC   },  /* retrieve a system environment var */
    { "equal",    DYNAMIC   },  /* logical equality check */
    { "exist",    MONAMIC   },  /* check if a file exists */
    { "find",     MONAMIC   },  /* look for a file on the path... */
    { "greater",  DYNAMIC   },  /* logical greater than */
    { "group",    MONAMIC   },  /* return group match in MAGIC mode */
    { "gtc",      NILNAMIC  },  /* get 1 emacs command */
    { "gtk",      NILNAMIC  },  /* get 1 charater */
    { "indirect", MONAMIC   },  /* evaluate indirect value */
    { "isnum",    MONAMIC   },  /* is the arg a number? */
    { "left",     DYNAMIC   },  /* left string(string, len) */
    { "length",   MONAMIC   },  /* string length */
    { "less",     DYNAMIC   },  /* logical less than */
    { "lower",    MONAMIC   },  /* lower case string */
    { "mid",      TRINAMIC  },  /* mid string(string, pos, len) */
    { "mkcol",    MONAMIC   },  /* column position of a mark */
    { "mkline",   MONAMIC   },  /* line number of a mark */
    { "modulo",   DYNAMIC   },  /* mod */
    { "negate",   MONAMIC   },  /* negate */
    { "not",      MONAMIC   },  /* logical not */
    { "or",       DYNAMIC   },  /* logical or */
    { "reverse",  MONAMIC   },  /* reverse */
    { "right",    DYNAMIC   },  /* right string(string, pos) */
    { "rnd",      MONAMIC   },  /* get a random number */
    { "sequal",   DYNAMIC   },  /* string logical equality check */
    { "sgreater", DYNAMIC   },  /* string logical greater than */
    { "sindex",   DYNAMIC   },  /* find the index of one string in another */
    { "sless",    DYNAMIC   },  /* string logical less than */
    { "slower",   DYNAMIC   },  /* set lower to upper char translation */
    { "subtract", DYNAMIC   },  /* subtraction */
    { "supper",   DYNAMIC   },  /* set upper to lower char translation */
    { "times",    DYNAMIC   },  /* multiplication */
    { "trim",     MONAMIC   },  /* trim whitespace off the end of a string */
    { "true",     MONAMIC   },  /* Truth of the universe logical test */
    { "upper",    MONAMIC   },  /* uppercase string */
    { "xlate",    TRINAMIC  }   /* XLATE character string translation */
};

/*  and its preprocesor definitions     */

# define UFABBREV        (0)
# define UFABS           (1)
# define UFADD           (2)
# define UFAND           (3)
# define UFASCII         (4)
# define UFBAND          (5)
# define UFBIND          (6)
# define UFBNOT          (7)
# define UFBOR           (8)
# define UFBXOR          (9)
# define UFCALL         (10)
# define UFCAT          (11)
# define UFCHR          (12)
# define UFDIV          (13)
# define UFENV          (14)
# define UFEQUAL        (15)
# define UFEXIST        (16)
# define UFFIND         (17)
# define UFGREATER      (18)
# define UFGROUP        (19)
# define UFGTCMD        (20)
# define UFGTKEY        (21)
# define UFIND          (22)
# define UFISNUM        (23)
# define UFLEFT         (24)
# define UFLENGTH       (25)
# define UFLESS         (26)
# define UFLOWER        (27)
# define UFMID          (28)
# define UFMKCOL        (29)
# define UFMKLINE       (30)
# define UFMOD          (31)
# define UFNEG          (32)
# define UFNOT          (33)
# define UFOR           (34)
# define UFREVERSE      (35)
# define UFRIGHT        (36)
# define UFRND          (37)
# define UFSEQUAL       (38)
# define UFSGREAT       (39)
# define UFSINDEX       (40)
# define UFSLESS        (41)
# define UFSLOWER       (42)
# define UFSUB          (43)
# define UFSUPPER       (44)
# define UFTIMES        (45)
# define UFTRIM         (46)
# define UFTRUTH        (47)
# define UFUPPER        (48)
# define UFXLATE        (49)


#endif  /* EVAL_C_  */


/*====================================================================*/
#endif/**#ifndef EVAR_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
