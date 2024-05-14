/*======================================================================
 * EDEF:           Global variable definitions for
 *                 MicroEMACS 3.9
 *
 *                 written by Daniel Lawrence
 *                 based on code by Dave G. Conroy,
 *                 Steve Wilhite and George Jones
 *====================================================================*/

/*====================================================================*/
#ifndef EDEF_H_
#define EDEF_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/*====================================================================*/
/* Define array lengths here: Avoid Literals!                         */
/*====================================================================*/
#define palstr_LEN  49
/*====================================================================*/


/**********************************************************************/
/* Variable declarations of variables that are *defined* in a C-file:*/
/**********************************************************************/
COMMON NOSHARE KEYTAB keytab[NBINDS]; /* main.c via ebind.h */
COMMON NOSHARE NBIND  names[DUMMYSZ]; /* main.c via efunc.h */
#ifndef termdef
/* terminal table defined only in TERM.C */
COMMON NOSHARE TERM term; /* Terminal information.    */
#endif
/**********************************************************************/


#define _K_               ,     /* We need to marshall the `,'  */

/* for MAIN.C:  */
#ifdef  MAIN_C_
# if defined(__cplusplus)
/* C++ Needs this because all constants have *internal linkage* by default. */
#   define CSTDEF         COMMON CONST
# else
#   define CSTDEF         CONST
# endif
# define DCLDEF
# define INIT_(x)         = x
# define NOSZ_
#else
# define CSTDEF           COMMON CONST
# define DCLDEF           COMMON
# define INIT_(x)
# define NOSZ_            DUMMYSZ /* GRRR */
#endif


/* initialized global definitions */

DCLDEF NOSHARE int            fillcol             INIT_(72);            /* Current fill column              */
DCLDEF NOSHARE char           *execstr            INIT_(NULL);          /* pointer to string to execute     */
DCLDEF NOSHARE char           golabel[NPAT]       INIT_("");            /* current line to go to            */
DCLDEF NOSHARE char           paralead[NPAT]      INIT_(" \t");         /* paragraph leadin chars           */
DCLDEF NOSHARE char           fmtlead[NPAT]       INIT_("");            /* format command leadin chars      */
DCLDEF NOSHARE char           mainbuf[NOSZ_]      INIT_("main");        /* name of main buffer              */
DCLDEF NOSHARE int            wlflag              INIT_(FALSE);         /* word list enabled flag           */
DCLDEF NOSHARE int            clearflag           INIT_(TRUE);          /* clear screen on screen change?   */
DCLDEF NOSHARE int            execlevel           INIT_(0);             /* execution IF level               */
DCLDEF NOSHARE int            eolexist            INIT_(TRUE);          /* does clear to EOL exist?         */
DCLDEF NOSHARE int            revexist            INIT_(FALSE);         /* does reverse video exist?        */
DCLDEF NOSHARE int            exec_error          INIT_(FALSE);         /* macro execution error pending?   */
DCLDEF NOSHARE int            flickcode           INIT_(TRUE);          /* do flicker supression?           */
DCLDEF NOSHARE int            mouse_move          INIT_(1);             /* user allow tracking mouse moves? */
DCLDEF NOSHARE int            mmove_flag          INIT_(TRUE);          /* code currently allowing mmoves?  */
DCLDEF NOSHARE int            newscreenflag       INIT_(FALSE);         /* Create new screen on reads?      */
DCLDEF NOSHARE int            overlap             INIT_(2);             /* overlap when paging screens      */
DCLDEF NOSHARE int            parindent           INIT_(0);             /* parindent to be used with fill   */
DCLDEF NOSHARE CONST char     *modename[NOSZ_]                          /* name of modes                    */
    INIT_(
      {
      "WRAP" _K_ "CMODE" _K_ "SPELL" _K_ "EXACT" _K_ "VIEW" _K_ "OVER" _K_ "MAGIC" _K_ "CRYPT" _K_
      "ASAVE" _K_ "REP" _K_ "ABBREV"
      });
CSTDEF NOSHARE char           modecode[NOSZ_]     INIT_("WCSEVOMYARB"); /* letters to represent modes   */
DCLDEF NOSHARE int            numfunc             INIT_(NFUNCS);        /* number of bindable functions     */
DCLDEF NOSHARE int            gmode               INIT_(0);             /* global editor mode               */
DCLDEF NOSHARE int            gflags              INIT_(GFREAD);        /* global control flag              */
DCLDEF NOSHARE int            gfcolor             INIT_(7);             /* global forgrnd color (white)     */
DCLDEF NOSHARE int            gbcolor             INIT_(0);             /* global backgrnd color (black)    */
DCLDEF NOSHARE int            deskcolor           INIT_(0);             /* desktop background color         */
DCLDEF NOSHARE int            gasave              INIT_(256);           /* global ASAVE size                */
DCLDEF NOSHARE int            gacount             INIT_(256);           /* count until next ASAVE           */
DCLDEF NOSHARE int            sgarbf              INIT_(TRUE);          /* TRUE if screen is garbage        */
DCLDEF NOSHARE int            mpresf              INIT_(FALSE);         /* TRUE if message in last line     */
DCLDEF NOSHARE int            clexec              INIT_(FALSE);         /* command line execution flag      */
DCLDEF NOSHARE int            mstore              INIT_(FALSE);         /* storing text to macro flag       */
DCLDEF NOSHARE int            discmd              INIT_(TRUE);          /* display command flag             */
DCLDEF NOSHARE int            disinp              INIT_(TRUE);          /* display input characters         */
DCLDEF NOSHARE int            modeflag            INIT_(TRUE);          /* display modelines flag           */
DCLDEF NOSHARE int            timeflag            INIT_(FALSE);         /* display time                     */
DCLDEF NOSHARE int            undoflag            INIT_(TRUE);          /* processing undo commands         */
DCLDEF NOSHARE int            undoing             INIT_(FALSE);         /* currently undoing a command?     */
DCLDEF NOSHARE char           lasttime[6]         INIT_("");            /* last time string displayed       */
DCLDEF NOSHARE int            popflag             INIT_(TRUE);          /* pop-up windows enabled?          */
DCLDEF NOSHARE int            popwait             INIT_(TRUE);          /* user wait on pops enabled?       */
DCLDEF NOSHARE int            posflag             INIT_(FALSE);         /* display point position           */
DCLDEF NOSHARE int            cpending            INIT_(FALSE);         /* input character pending?         */
DCLDEF NOSHARE int            sscroll             INIT_(FALSE);         /* smooth scrolling enabled flag    */
DCLDEF NOSHARE int            hscroll             INIT_(TRUE);          /* horizontal scrolling flag        */
DCLDEF NOSHARE int            hscrollbar          INIT_(TRUE);          /* horizontal scroll bar flag       */
DCLDEF NOSHARE int            vscrollbar          INIT_(TRUE);          /* vertical scroll bar flag         */
DCLDEF NOSHARE int            hjump               INIT_(1);             /* horizontal jump size             */
#if VMS /* Per default don't use $ssave as it disables VMS file versions: */
DCLDEF NOSHARE int            ssave               INIT_(FALSE);         /* safe save flag                   */
#else
DCLDEF NOSHARE int            ssave               INIT_(TRUE);          /* safe save flag                   */
#endif
DCLDEF NOSHARE struct BUFFER *bstore              INIT_(NULL);          /* buffer to store macro text to    */
DCLDEF NOSHARE int            vtrow               INIT_(0);             /* Row location of SW cursor        */
DCLDEF NOSHARE int            vtcol               INIT_(0);             /* Column location of SW cursor     */
DCLDEF NOSHARE int            ttrow               INIT_(HUGENUM);       /* Row location of HW cursor        */
DCLDEF NOSHARE int            ttcol               INIT_(HUGENUM);       /* Column location of HW cursor     */
DCLDEF NOSHARE int            lbound              INIT_(0);             /* leftmost column of current line being displayed  */
DCLDEF NOSHARE int            taboff              INIT_(0);             /* tab offset for display           */
DCLDEF NOSHARE int            tabsize             INIT_(8);             /* current hard tab size            */
DCLDEF NOSHARE int            stabsize            INIT_(0);             /* current soft tab size (0: use hard tabs) */
DCLDEF NOSHARE int            reptc               INIT_(CTRF | 'U');    /* current universal repeat char    */
DCLDEF NOSHARE int            abortc              INIT_(CTRF | 'G');    /* current abort command char       */
DCLDEF NOSHARE int            sterm               INIT_(CTRF | '[');    /* search terminating character     */
DCLDEF NOSHARE int            isterm              INIT_(CTRF | '[');    /* incremental-search terminating char  */
DCLDEF NOSHARE int            searchtype          INIT_(SRNORM);        /* current search style             */
DCLDEF NOSHARE int            yankflag            INIT_(FALSE);         /* current yank style               */
DCLDEF NOSHARE int            prefix              INIT_(0);             /* currently pending prefix bits    */
DCLDEF NOSHARE int            prenum              INIT_(0);             /*     "       "     numeric arg    */
DCLDEF NOSHARE int            predef              INIT_(TRUE);          /*     "       "     default flag   */
DCLDEF NOSHARE int            quotec              INIT_(CTRF | 'Q');    /* quote char during mlreply()      */
DCLDEF NOSHARE CONST char     *cname[NOSZ_]                             /* names of colors                  */
    INIT_({
        "BLACK" _K_ "RED" _K_ "GREEN" _K_ "YELLOW" _K_ "BLUE" _K_ "MAGENTA" _K_ "CYAN" _K_ "GREY" _K_
        "GRAY" _K_ "LRED" _K_ "LGREEN" _K_ "LYELLOW" _K_ "LBLUE" _K_ "LMAGENTA" _K_ "LCYAN" _K_ "WHITE"
    });

DCLDEF NOSHARE EWINDOW        *swindow            INIT_(NULL);          /* saved window pointer             */
DCLDEF NOSHARE int            cryptflag           INIT_(FALSE);         /* currently encrypting?            */
DCLDEF NOSHARE int            oldcrypt            INIT_(FALSE);         /* using old(broken) encryption?    */
DCLDEF NOSHARE short          kbdm[NKBDM];                              /* Macro                            */
DCLDEF NOSHARE short          *kbdend             INIT_(&kbdm[0]);      /* ptr to end of the keyboard       */
DCLDEF NOSHARE int            kbdmode             INIT_(STOP);          /* current keyboard macro mode      */
DCLDEF NOSHARE int            kbdrep              INIT_(0);             /* number of repetitions            */
DCLDEF NOSHARE int            restflag            INIT_(FALSE);         /* restricted use?                  */
DCLDEF NOSHARE int            lastkey             INIT_(0);             /* last keystoke                    */
DCLDEF NOSHARE long           seed                INIT_(1L);            /* random number seed               */
DCLDEF NOSHARE long           envram              INIT_(0l);            /* # of bytes current in use by malloc  */
DCLDEF NOSHARE long           access_time         INIT_(0L);            /* counter of buffer access         */
DCLDEF NOSHARE int            macbug              INIT_(FALSE);         /* macro debugging flag             */
DCLDEF NOSHARE int            mouseflag           INIT_(TRUE);          /* use the mouse?                   */
DCLDEF NOSHARE int            diagflag            INIT_(FALSE);         /* diagonal mouse movements?        */
CSTDEF NOSHARE char           errorm[NOSZ_]       INIT_("ERROR");       /* error literal                    */
CSTDEF NOSHARE char           truem[NOSZ_]        INIT_("TRUE");        /* true literal                     */
CSTDEF NOSHARE char           falsem[NOSZ_]       INIT_("FALSE");       /* false litereal                   */
DCLDEF NOSHARE int            cmdstatus           INIT_(TRUE);          /* last command status              */
DCLDEF NOSHARE char           palstr[palstr_LEN]  INIT_("");            /* palette string                   */
DCLDEF NOSHARE char           lastmesg[NSTRING]   INIT_("");            /* last message posted              */
DCLDEF NOSHARE char           rval[NSTRING]       INIT_("0");           /* result of last procedure/sub     */
DCLDEF NOSHARE char           *lastptr            INIT_(NULL);          /* ptr to lastmesg[]                */
DCLDEF NOSHARE int            saveflag            INIT_(0);             /* Flags, saved with the $target var  */
DCLDEF NOSHARE char           *fline              INIT_(NULL);          /* dynamic return line              */
DCLDEF NOSHARE int            flen                INIT_(0);             /* current length of fline          */
DCLDEF NOSHARE int            eexitflag           INIT_(FALSE);         /* EMACS exit flag                  */
DCLDEF NOSHARE int            eexitval            INIT_(0);             /* and the exit return value        */
DCLDEF NOSHARE int            xpos                INIT_(0);             /* current column mouse is positioned to  */
DCLDEF NOSHARE int            ypos                INIT_(0);             /* current screen row        "      */
DCLDEF NOSHARE int            nclicks             INIT_(0);             /* cleared on any non-mouse event   */
DCLDEF NOSHARE int            disphigh            INIT_(FALSE);         /* display high bit chars escaped   */
DCLDEF NOSHARE int            dispundo            INIT_(FALSE);         /* display undo stack depth on comand line  */
DCLDEF NOSHARE int            defferupdate        INIT_(FALSE);         /* if TRUE, update(TRUE) should be called before
                                                                         * yielding to another Windows application  */
DCLDEF NOSHARE int            notquiescent        INIT_(1);             /* <=0 only when getkey called directly
                                                                         * by editloop()                    */
DCLDEF NOSHARE int            fbusy               INIT_(FALSE);         /* indicates file activity if FREADING or FWRITING.
                                                                         * Used by abort mechanism          */
DCLDEF NOSHARE int            hilite              INIT_(10);            /* current region to highlight (255 if none)  */
#define hilite_IsValid()      ( 0 <= hilite && hilite < NMARKS - 1 )
#define hilite_InValidate()   ( hilite = 0xFF )
CASRT(NMARKS -1 <= 0xFF);


/* uninitialized global definitions */

DCLDEF NOSHARE OBJECT         obj;                /* scratch undo object                    */
DCLDEF NOSHARE int            charpending;        /* character pushed back                  */
DCLDEF NOSHARE int            kill_index;         /* current index into kill ring           */
DCLDEF NOSHARE KILL           *kbufp[NRING];      /* current kill buffer chunk pointer      */
DCLDEF NOSHARE KILL           *kbufh[NRING];      /* kill buffer header pointer             */
DCLDEF NOSHARE int            kskip[NRING];       /* # of bytes to skip in 1st kill chunk   */
DCLDEF NOSHARE short          *kbdptr;            /* current position in keyboard buf       */
DCLDEF NOSHARE char           lterm[NSTRING];     /* line terminators on file write         */
DCLDEF NOSHARE unsigned char  wordlist[256];      /* characters considered "in words"       */
DCLDEF NOSHARE int            kused[NRING];       /* # of bytes used in last kill chunk     */
DCLDEF NOSHARE int            oquote;             /* open quote variable                    */
DCLDEF NOSHARE int            cquote;             /* close quote variable                   */
DCLDEF NOSHARE int            currow;             /* Cursor row                             */
DCLDEF NOSHARE int            curcol;             /* Cursor column                          */
DCLDEF NOSHARE int            thisflag;           /* Flags, this command                    */
DCLDEF NOSHARE int            lastflag;           /* Flags, last command                    */
DCLDEF NOSHARE int            curgoal;            /* Goal for C-P, C-N                      */
DCLDEF NOSHARE EWINDOW        *curwp;             /* Current window                         */
DCLDEF NOSHARE BUFFER         *curbp;             /* Current buffer                         */
DCLDEF NOSHARE EWINDOW        *wheadp;            /* Head of list of windows                */
DCLDEF NOSHARE BUFFER         *bheadp;            /* Head of list of buffers                */
DCLDEF NOSHARE UTABLE         *uv_head;           /* head of list of user variables         */
DCLDEF NOSHARE UTABLE         *uv_global;         /* global variable table                  */
DCLDEF NOSHARE ABBREV         *ab_head;           /* head of the abbreviation list          */
DCLDEF NOSHARE int            ab_bell;            /* are we ringing the bell on completion? */
DCLDEF NOSHARE int            ab_cap;             /* match capatilization on expansion?     */
DCLDEF NOSHARE int            ab_full;            /* full expansion? (exceeds ab_quick)?    */
DCLDEF NOSHARE int            ab_quick;           /* aggressive completion enabled?         */
DCLDEF NOSHARE char           ab_word[NSTRING];   /* current word being typed               */
DCLDEF NOSHARE char           *ab_pos;            /* current place in ab_word               */
DCLDEF NOSHARE char           *ab_end;            /* ptr to physical end of ab_word         */
DCLDEF NOSHARE SCREEN_T       *first_screen;      /* Head and current screen in list        */
DCLDEF NOSHARE BUFFER         *blistp;            /* Buffer for C-X C-B                     */
DCLDEF NOSHARE BUFFER         *ulistp;            /* Buffer for C-X U                       */
DCLDEF NOSHARE BUFFER         *slistp;            /* Buffer for A-B                         */
DCLDEF NOSHARE char           sres[NBUFN];        /* current screen resolution              */
DCLDEF NOSHARE char           os[NBUFN];          /* what OS are we running under           */
DCLDEF NOSHARE unsigned char  lowcase[HICHAR];    /* lower casing map                       */
DCLDEF NOSHARE unsigned char  upcase[HICHAR];     /* upper casing map                       */
DCLDEF NOSHARE unsigned char  pat[NPAT];          /* Search pattern                         */
DCLDEF NOSHARE unsigned char  tap[NPAT];          /* Reversed pattern array.                */
DCLDEF NOSHARE unsigned char  rpat[NPAT];         /* replacement pattern                    */


/*  Various "Hook" execution variables  */

DCLDEF NOSHARE KEYTAB         readhook;           /* executed on all file reads             */
DCLDEF NOSHARE KEYTAB         wraphook;           /* executed when wrapping text            */
DCLDEF NOSHARE KEYTAB         cmdhook;            /* executed before looking for a command  */
DCLDEF NOSHARE KEYTAB         writehook;          /* executed on all file writes            */
DCLDEF NOSHARE KEYTAB         exbhook;            /* executed when exiting a buffer         */
DCLDEF NOSHARE KEYTAB         bufhook;            /* executed when entering a buffer        */
DCLDEF NOSHARE KEYTAB         exithook;           /* executed when exiting emacs            */

/* The variables matchline and matchoff hold the line and offset position of the
 * *start* of the match. The variable patmatch holds the string that satisfies
 * the search command.
 */
DCLDEF NOSHARE int            matchlen;
DCLDEF NOSHARE int            matchoff;
DCLDEF NOSHARE LINE           *matchline;
DCLDEF NOSHARE char           *patmatch           INIT_(NULL);

#if     MAGIC

/*
 * The variables magical and rmagical determine if there were actual
 * metacharacters in the search and replace strings -
 * if not, then we don't have to use the slower MAGIC mode search functions.
 */
DCLDEF NOSHARE short int      magical             INIT_(FALSE);
DCLDEF NOSHARE short int      rmagical            INIT_(FALSE);

DCLDEF NOSHARE MC             mcpat[NPAT];        /* the magic pattern                      */
DCLDEF NOSHARE MC             tapcm[NPAT];        /* the reversed magic pattern             */
DCLDEF NOSHARE MC             mcdeltapat[2];      /* the no-magic pattern                   */
DCLDEF NOSHARE MC             tapatledcm[2];      /* the reversed no-magic pattern          */
DCLDEF NOSHARE RMC            rmcpat[NPAT];       /* the replacement magic array            */
DCLDEF NOSHARE char           *grpmatch[MAXGROUPS]; /* holds groups found in search         */

#endif

DCLDEF NOSHARE  DELTA         deltapat;           /* Forward pattern delta structure.       */
DCLDEF NOSHARE  DELTA         tapatled;           /* Reverse pattern delta structure.       */


/* directive name table: This holds the names of all the directives.... */
DCLDEF NOSHARE CONST char     *dname[NOSZ_]
    INIT_({
        "if" _K_ "else" _K_ "endif" _K_ "goto" _K_ "return" _K_ "endm" _K_ "while" _K_ "endwhile" _K_
        "break" _K_ "force"
    });
/* directive lengths                                                    */
CSTDEF NOSHARE short int  dname_len[NUMDIRS]  INIT_({ 2 _K_ 4 _K_ 5 _K_ 4 _K_ 6 _K_ 4 _K_ 5 _K_ 7 _K_ 5 _K_ 5 });

/*  var needed for macro debugging output */
DCLDEF NOSHARE char           outline[NSTRING];   /* global string to hold debug line text  */

/* System message notification (at the moment, VMS only): */
#if     VMS
DCLDEF NOSHARE char           brdcstbuf[1024];    /* Broadcast messages                     */
DCLDEF NOSHARE int            pending_msg         INIT_(FALSE);         /* Flag - have we notified user yet.*/
#endif

#if     HANDLE_WINCH
DCLDEF NOSHARE int            winch_flag          INIT_(0);             /* Window size changed flag         */
#endif


/*====================================================================*/
#undef CSTDEF
#undef DCLDEF
#undef INIT_
#undef _K_
#undef NOSZ_
/*====================================================================*/



/*====================================================================*/
#endif/**#ifndef EEDEF_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
