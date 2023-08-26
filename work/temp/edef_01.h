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
COMMON NOSHARE NBIND  names[];        /* main.c via efunc.h */  /***HEREHEREHERE***/
#ifndef termdef
/* terminal table defined only in TERM.C */
COMMON NOSHARE TERM term; /* Terminal information.    */
#endif
/**********************************************************************/


/* for MAIN.C:  */
#ifdef  MAIN_C_
# define VARMOD
# define VARINIT(x) = x
# define _K_        ,     /* We need to marshall the `,'  */
# define NOSZ_
#else
# define VARMOD COMMON
# define VARINIT(x)
# define NOSZ_  DUMMYSZ /* GRRR */
#endif


/* initialized global definitions */

VARMOD NOSHARE int fillcol    VARINIT(72);         /* Current fill column      */
VARMOD NOSHARE short          kbdm[NKBDM];              /* Macro            */
VARMOD NOSHARE char           *execstr VARINIT(NULL);           /* pointer to string to execute */
VARMOD NOSHARE char           golabel[NPAT] VARINIT("");        /* current line to go to    */
VARMOD NOSHARE char           paralead[NPAT] VARINIT(" \t");    /* paragraph leadin chars   */
VARMOD NOSHARE char           fmtlead[NPAT] VARINIT("");        /* format command leadin chars  */
VARMOD NOSHARE char           mainbuf[NOSZ_] VARINIT("main");        /* name of main buffer      */
VARMOD NOSHARE char           lterm[NSTRING];            /* line terminators on file write */
VARMOD NOSHARE unsigned char  wordlist[256];    /* characters considered "in words" */
VARMOD NOSHARE int            wlflag     VARINIT(FALSE);       /* word list enabled flag   */
VARMOD NOSHARE int            clearflag  VARINIT(TRUE);     /* clear screen on screen change? */
VARMOD NOSHARE int            execlevel  VARINIT(0);        /* execution IF level       */
VARMOD NOSHARE int            eolexist   VARINIT(TRUE);      /* does clear to EOL exist? */
VARMOD NOSHARE int            revexist   VARINIT(FALSE);     /* does reverse video exist?    */
VARMOD NOSHARE int            exec_error VARINIT(FALSE);   /* macro execution error pending? */
VARMOD NOSHARE int            flickcode  VARINIT(TRUE);     /* do flicker supression?   */
VARMOD NOSHARE int            mouse_move VARINIT(1);       /* user allow tracking mouse moves? */
VARMOD NOSHARE int            mmove_flag VARINIT(TRUE);    /* code currently allowing mmoves? */
VARMOD NOSHARE int            newscreenflag VARINIT(FALSE);/* Create new screen on reads? */
VARMOD NOSHARE int            overlap VARINIT(2);          /* overlap when paging screens */
VARMOD NOSHARE int            parindent VARINIT(0);        /* parindent to be used with fill */
VARMOD NOSHARE CONST char     *modename[NOSZ_]                 /* name of modes        */
    VARINIT(
      {
      "WRAP" _K_ "CMODE" _K_ "SPELL" _K_ "EXACT" _K_ "VIEW" _K_ "OVER" _K_ "MAGIC" _K_ "CRYPT" _K_
      "ASAVE" _K_ "REP" _K_ "ABBREV"
      });
VARMOD NOSHARE CONST char     modecode[NOSZ_] VARINIT("WCSEVOMYARB");  /* letters to represent modes   */
VARMOD NOSHARE int            numfunc VARINIT(NFUNCS);     /* number of bindable functions */
VARMOD NOSHARE int            gmode VARINIT(0);            /* global editor mode       */
VARMOD NOSHARE int            gflags VARINIT(GFREAD);      /* global control flag      */
VARMOD NOSHARE int            gfcolor VARINIT(7);          /* global forgrnd color (white) */
VARMOD NOSHARE int            gbcolor VARINIT(0);          /* global backgrnd color (black)*/
VARMOD NOSHARE int            deskcolor VARINIT(0);        /* desktop background color */
VARMOD NOSHARE int            gasave VARINIT(256);         /* global ASAVE size        */
VARMOD NOSHARE int            gacount VARINIT(256);        /* count until next ASAVE   */
VARMOD NOSHARE int            sgarbf VARINIT(TRUE);        /* TRUE if screen is garbage    */
VARMOD NOSHARE int            mpresf VARINIT(FALSE);       /* TRUE if message in last line */
VARMOD NOSHARE int            clexec VARINIT(FALSE);       /* command line execution flag  */
VARMOD NOSHARE int            mstore VARINIT(FALSE);       /* storing text to macro flag   */
VARMOD NOSHARE int            discmd VARINIT(TRUE);        /* display command flag     */
VARMOD NOSHARE int            disinp VARINIT(TRUE);        /* display input characters */
VARMOD NOSHARE int            modeflag VARINIT(TRUE);      /* display modelines flag   */
VARMOD NOSHARE int            timeflag VARINIT(FALSE);     /* display time         */
VARMOD NOSHARE int            undoflag VARINIT(TRUE);      /* processing undo commands */
VARMOD NOSHARE OBJECT         obj;                     /* scratch undo object      */
VARMOD NOSHARE int            undoing VARINIT(FALSE);      /* currently undoing a command? */
VARMOD NOSHARE char           lasttime[6] VARINIT("");    /* last time string displayed   */
VARMOD NOSHARE int            popflag VARINIT(TRUE);       /* pop-up windows enabled?  */
VARMOD NOSHARE int            popwait VARINIT(TRUE);       /* user wait on pops enabled?   */
VARMOD NOSHARE int            posflag VARINIT(FALSE);      /* display point position   */
VARMOD NOSHARE int            cpending VARINIT(FALSE);           /* input character pending? */
VARMOD NOSHARE int            charpending;                /* character pushed back    */
VARMOD NOSHARE int            sscroll VARINIT(FALSE);      /* smooth scrolling enabled flag*/
VARMOD NOSHARE int            hscroll VARINIT(TRUE);       /* horizontal scrolling flag    */
VARMOD NOSHARE int            hscrollbar VARINIT(TRUE);    /* horizontal scroll bar flag   */
VARMOD NOSHARE int            vscrollbar VARINIT(TRUE);    /* vertical scroll bar flag */
VARMOD NOSHARE int            hjump VARINIT(1);            /* horizontal jump size     */
VARMOD NOSHARE int            ssave VARINIT(TRUE);         /* safe save flag       */
VARMOD NOSHARE struct BUFFER *bstore VARINIT(NULL);   /* buffer to store macro text to*/
VARMOD NOSHARE int            vtrow VARINIT(0);            /* Row location of SW cursor    */
VARMOD NOSHARE int            vtcol VARINIT(0);            /* Column location of SW cursor */
VARMOD NOSHARE int            ttrow VARINIT(HUGENUM);      /* Row location of HW cursor    */
VARMOD NOSHARE int            ttcol VARINIT(HUGENUM);      /* Column location of HW cursor */
VARMOD NOSHARE int            lbound VARINIT(0);           /* leftmost column of current line being
                                         * displayed        */
VARMOD NOSHARE int            taboff VARINIT(0);           /* tab offset for display   */
VARMOD NOSHARE int            tabsize VARINIT(8);          /* current hard tab size    */
VARMOD NOSHARE int            stabsize VARINIT(0);         /* current soft tab size (0: use hard
                                         * tabs)  */
VARMOD NOSHARE int            reptc VARINIT(CTRL | 'U');   /* current universal repeat char*/
VARMOD NOSHARE int            abortc VARINIT(CTRL | 'G');  /* current abort command char   */
VARMOD NOSHARE int            sterm VARINIT(CTRL | '[');   /* search terminating character */
VARMOD NOSHARE int            isterm VARINIT(CTRL | '[');  /* incremental-search terminating char
                                         */
VARMOD NOSHARE int            searchtype VARINIT(SRNORM);  /* current search style     */
VARMOD NOSHARE int            yankflag VARINIT(FALSE);     /* current yank style       */

VARMOD NOSHARE int            prefix VARINIT(0);           /* currently pending prefix bits */
VARMOD NOSHARE int            prenum VARINIT(0);           /*     "       "     numeric arg */
VARMOD NOSHARE int            predef VARINIT(TRUE);        /*     "       "     default flag */

VARMOD NOSHARE int            quotec VARINIT(CTRL | 'Q');  /* quote char during mlreply() */
VARMOD NOSHARE CONST char     *cname[NOSZ_]           /* names of colors      */
    VARINIT({
        "BLACK" _K_ "RED" _K_ "GREEN" _K_ "YELLOW" _K_ "BLUE" _K_ "MAGENTA" _K_ "CYAN" _K_ "GREY" _K_
        "GRAY" _K_ "LRED" _K_ "LGREEN" _K_ "LYELLOW" _K_ "LBLUE" _K_ "LMAGENTA" _K_ "LCYAN" _K_ "WHITE"
    });

VARMOD NOSHARE int            kill_index;                 /* current index into kill ring */
VARMOD NOSHARE KILL           *kbufp[NRING];             /* current kill buffer chunk pointer*/
VARMOD NOSHARE KILL           *kbufh[NRING];             /* kill buffer header pointer   */
VARMOD NOSHARE int            kskip[NRING];               /* # of bytes to skip in 1st kill chunk
                                         */
VARMOD NOSHARE int            kused[NRING];               /* # of bytes used in last kill chunk*/
VARMOD NOSHARE EWINDOW        *swindow VARINIT(NULL);        /* saved window pointer     */
VARMOD NOSHARE int            cryptflag VARINIT(FALSE);          /* currently encrypting?    */
VARMOD NOSHARE int            oldcrypt VARINIT(FALSE);           /* using old(broken) encryption? */
VARMOD NOSHARE short          *kbdptr;                  /* current position in keyboard buf */
VARMOD NOSHARE short          *kbdend VARINIT(&kbdm[0]);       /* ptr to end of the keyboard */
VARMOD NOSHARE int            kbdmode VARINIT(STOP);       /* current keyboard macro mode  */
VARMOD NOSHARE int            kbdrep VARINIT(0);           /* number of repetitions    */
VARMOD NOSHARE int            restflag VARINIT(FALSE);     /* restricted use?      */
VARMOD NOSHARE int            lastkey VARINIT(0);          /* last keystoke        */
VARMOD NOSHARE long           seed VARINIT(1L);           /* random number seed       */
VARMOD NOSHARE long           envram VARINIT(0l);               /* # of bytes current in use by malloc
                                         */
VARMOD NOSHARE long           access_time VARINIT(0L);          /* counter of buffer access */
VARMOD NOSHARE int            macbug VARINIT(FALSE);       /* macro debugging flag     */
VARMOD NOSHARE int            mouseflag VARINIT(TRUE);     /* use the mouse?       */
VARMOD NOSHARE int            diagflag VARINIT(FALSE);     /* diagonal mouse movements?    */
VARMOD NOSHARE CONST char     errorm[NOSZ_] VARINIT("ERROR");          /* error literal        */
VARMOD NOSHARE CONST char     truem[NOSZ_] VARINIT("TRUE");            /* true literal         */
VARMOD NOSHARE CONST char     falsem[NOSZ_] VARINIT("FALSE");          /* false litereal       */
VARMOD NOSHARE int            cmdstatus VARINIT(TRUE);     /* last command status      */
VARMOD NOSHARE char           palstr[palstr_LEN] VARINIT("");   /* palette string       */
VARMOD NOSHARE char           lastmesg[NSTRING] VARINIT("");    /* last message posted      */
VARMOD NOSHARE char           rval[NSTRING] VARINIT("0");       /* result of last procedure/sub */
VARMOD NOSHARE char           *lastptr VARINIT(NULL);           /* ptr to lastmesg[]        */
VARMOD NOSHARE int            saveflag VARINIT(0);         /* Flags, saved with the $target var */
VARMOD NOSHARE char           *fline VARINIT(NULL);             /* dynamic return line      */
VARMOD NOSHARE int            flen VARINIT(0);             /* current length of fline  */
VARMOD NOSHARE int            eexitflag VARINIT(FALSE);    /* EMACS exit flag      */
VARMOD NOSHARE int            eexitval VARINIT(0);         /* and the exit return value    */
VARMOD NOSHARE int            xpos VARINIT(0);           /* current column mouse is positioned to*/
VARMOD NOSHARE int            ypos VARINIT(0);           /* current screen row        "      */
VARMOD NOSHARE int            nclicks VARINIT(0);        /* cleared on any non-mouse event   */
VARMOD NOSHARE int            disphigh VARINIT(FALSE);   /* display high bit chars escaped   */
VARMOD NOSHARE int            dispundo VARINIT(FALSE);   /* display undo stack depth on comand line */
VARMOD NOSHARE int            defferupdate VARINIT(FALSE);       /* if TRUE, update(TRUE) should be
                                         * called before yielding to another
                                         * Windows application */
VARMOD NOSHARE int            notquiescent VARINIT(1);           /* <=0 only when getkey called directly
                                         * by editloop () */
VARMOD NOSHARE int            fbusy VARINIT(FALSE);              /* indicates file activity if FREADING
                                         * or FWRITING. Used by abort mechanism
                                         */
VARMOD NOSHARE int            hilite VARINIT(10);                /* current region to highlight (255 if
                                         * none) */

/* uninitialized global definitions */

VARMOD NOSHARE int oquote;             /* open quote variable */
VARMOD NOSHARE int cquote;             /* close quote variable */
VARMOD NOSHARE int currow;       /* Cursor row           */
VARMOD NOSHARE int curcol;       /* Cursor column        */
VARMOD NOSHARE int thisflag;     /* Flags, this command      */
VARMOD NOSHARE int lastflag;     /* Flags, last command      */
VARMOD NOSHARE int curgoal;      /* Goal for C-P, C-N        */
VARMOD NOSHARE EWINDOW *curwp;         /* Current window       */
VARMOD NOSHARE BUFFER *curbp;          /* Current buffer       */
VARMOD NOSHARE EWINDOW *wheadp;        /* Head of list of windows  */
VARMOD NOSHARE BUFFER *bheadp;         /* Head of list of buffers  */
VARMOD NOSHARE UTABLE *uv_head;        /* head of list of user variables */
VARMOD NOSHARE UTABLE *uv_global;      /* global variable table */
VARMOD NOSHARE ABBREV *ab_head;        /* head of the abbreviation list */
VARMOD NOSHARE int ab_bell;      /* are we ringing the bell on completion? */
VARMOD NOSHARE int ab_cap;       /* match capatilization on expansion? */
VARMOD NOSHARE int ab_quick;     /* aggressive completion enabled? */
VARMOD NOSHARE char ab_word[NSTRING];  /* current word being typed */
VARMOD NOSHARE char *ab_pos;           /* current place in ab_word */
VARMOD NOSHARE char *ab_end;           /* ptr to physical end of ab_word */
VARMOD NOSHARE SCREEN_T *first_screen; /* Head and current screen in list */
VARMOD NOSHARE BUFFER *blistp;         /* Buffer for C-X C-B       */
VARMOD NOSHARE BUFFER *ulistp;         /* Buffer for C-X U     */
VARMOD NOSHARE BUFFER *slistp;         /* Buffer for A-B       */

VARMOD NOSHARE char sres[NBUFN];       /* current screen resolution    */
VARMOD NOSHARE char os[NBUFN];         /* what OS are we running under */

VARMOD NOSHARE char lowcase[HICHAR];   /* lower casing map     */
VARMOD NOSHARE char upcase[HICHAR];    /* upper casing map     */

VARMOD NOSHARE unsigned char pat[NPAT];        /* Search pattern       */
VARMOD NOSHARE unsigned char tap[NPAT];        /* Reversed pattern array.  */
VARMOD NOSHARE unsigned char rpat[NPAT];       /* replacement pattern      */

/*  Various "Hook" execution variables  */

VARMOD NOSHARE KEYTAB readhook;        /* executed on all file reads */
VARMOD NOSHARE KEYTAB wraphook;        /* executed when wrapping text */
VARMOD NOSHARE KEYTAB cmdhook;         /* executed before looking for a command */
VARMOD NOSHARE KEYTAB writehook;       /* executed on all file writes */
VARMOD NOSHARE KEYTAB exbhook;         /* executed when exiting a buffer */
VARMOD NOSHARE KEYTAB bufhook;         /* executed when entering a buffer */
VARMOD NOSHARE KEYTAB exithook;        /* executed when exiting emacs */

/* The variables matchline and matchoff hold the line and offset position of the
 * *start* of the match. The variable patmatch holds the string that satisfies
 * the search command.
 */
VARMOD NOSHARE int matchlen;
VARMOD NOSHARE int matchoff;
VARMOD NOSHARE LINE    *matchline;
VARMOD NOSHARE char *patmatch VARINIT(NULL);

#if     MAGIC
/*
 * The variables magical and rmagical determine if there were actual
 * metacharacters in the search and replace strings -
 * if not, then we don't have to use the slower MAGIC mode search functions.
 */
VARMOD NOSHARE short int magical VARINIT(FALSE);
VARMOD NOSHARE short int rmagical VARINIT(FALSE);

VARMOD NOSHARE MC mcpat[NPAT];         /* the magic pattern        */
VARMOD NOSHARE MC tapcm[NPAT];         /* the reversed magic pattern   */
VARMOD NOSHARE MC mcdeltapat[2];       /* the no-magic pattern     */
VARMOD NOSHARE MC tapatledcm[2];       /* the reversed no-magic pattern*/
VARMOD NOSHARE RMC rmcpat[NPAT];       /* the replacement magic array  */
VARMOD NOSHARE char *grpmatch[MAXGROUPS];      /* holds groups found in search */

#endif

VARMOD NOSHARE  DELTA deltapat;         /* Forward pattern delta structure.*/
VARMOD NOSHARE  DELTA tapatled;         /* Reverse pattern delta structure.*/

/* directive name table:
 *       This holds the names of all the directives.... */

VARMOD NOSHARE CONST char *dname[NOSZ_]
  VARINIT({
    "if" _K_ "else" _K_ "endif" _K_ "goto" _K_ "return" _K_ "endm" _K_ "while" _K_ "endwhile" _K_
    "break" _K_ "force"
  });
/* directive lengths        */
VARMOD NOSHARE CONST short int dname_len[NUMDIRS] VARINIT({ 2 _K_ 4 _K_ 5 _K_ 4 _K_ 6 _K_ 4 _K_ 5 _K_ 7 _K_ 5 _K_ 5 });

/*  var needed for macro debugging output   */
VARMOD NOSHARE char outline[NSTRING];  /* global string to hold debug line text */

/*
 * System message notification (at the moment, VMS only).
 */
#if     VMS
VARMOD NOSHARE char brdcstbuf[1024];           /* Broadcast messages */
VARMOD NOSHARE int pending_msg VARINIT(FALSE);        /* Flag - have we notified user yet.*/
#endif

#if     HANDLE_WINCH
VARMOD NOSHARE int winch_flag VARINIT(0);             /* Window size changed flag */
#endif


/*====================================================================*/
#undef VARMOD
#undef VARINIT
#undef _K_
#undef NOSZ_
/*====================================================================*/



/*====================================================================*/
#endif/**#ifndef EEDEF_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
