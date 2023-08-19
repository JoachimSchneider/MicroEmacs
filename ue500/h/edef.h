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


#ifdef  maindef

/* for MAIN.C */

/* initialized global definitions */

NOSHARE int DNEAR fillcol = 72;         /* Current fill column      */
NOSHARE short kbdm[NKBDM];              /* Macro            */
NOSHARE char *execstr = NULL;           /* pointer to string to execute */
NOSHARE char golabel[NPAT] = "";        /* current line to go to    */
NOSHARE char paralead[NPAT] = " \t";    /* paragraph leadin chars   */
NOSHARE char fmtlead[NPAT] = "";        /* format command leadin chars  */
NOSHARE char mainbuf[] = "main";        /* name of main buffer      */
NOSHARE char lterm[NSTRING];            /* line terminators on file write */
NOSHARE unsigned char wordlist[256];    /* characters considered "in words" */
NOSHARE int DNEAR wlflag = FALSE;       /* word list enabled flag   */
NOSHARE int DNEAR clearflag = TRUE;     /* clear screen on screen change? */
NOSHARE int DNEAR execlevel = 0;        /* execution IF level       */
NOSHARE int DNEAR eolexist = TRUE;      /* does clear to EOL exist? */
NOSHARE int DNEAR revexist = FALSE;     /* does reverse video exist?    */
NOSHARE int DNEAR exec_error = FALSE;   /* macro execution error pending? */
NOSHARE int DNEAR flickcode = TRUE;     /* do flicker supression?   */
NOSHARE int DNEAR mouse_move = 1;       /* user allow tracking mouse moves? */
NOSHARE int DNEAR mmove_flag = TRUE;    /* code currently allowing mmoves? */
NOSHARE int DNEAR newscreenflag = FALSE;/* Create new screen on reads? */
NOSHARE int DNEAR overlap = 2;          /* overlap when paging screens */
NOSHARE int DNEAR parindent = 0;        /* parindent to be used with fill */
CONST char *modename[] =                /* name of modes        */
{
    "WRAP", "CMODE", "SPELL", "EXACT", "VIEW", "OVER", "MAGIC", "CRYPT",
    "ASAVE", "REP", "ABBREV"
};
CONST char modecode[] = "WCSEVOMYARB";  /* letters to represent modes   */
NOSHARE int DNEAR numfunc = NFUNCS;     /* number of bindable functions */
NOSHARE int DNEAR gmode = 0;            /* global editor mode       */
NOSHARE int DNEAR gflags = GFREAD;      /* global control flag      */
NOSHARE int DNEAR gfcolor = 7;          /* global forgrnd color (white) */
NOSHARE int DNEAR gbcolor = 0;          /* global backgrnd color (black)*/
NOSHARE int DNEAR deskcolor = 0;        /* desktop background color */
NOSHARE int DNEAR gasave = 256;         /* global ASAVE size        */
NOSHARE int DNEAR gacount = 256;        /* count until next ASAVE   */
NOSHARE int DNEAR sgarbf = TRUE;        /* TRUE if screen is garbage    */
NOSHARE int DNEAR mpresf = FALSE;       /* TRUE if message in last line */
NOSHARE int DNEAR clexec = FALSE;       /* command line execution flag  */
NOSHARE int DNEAR mstore = FALSE;       /* storing text to macro flag   */
NOSHARE int DNEAR discmd = TRUE;        /* display command flag     */
NOSHARE int DNEAR disinp = TRUE;        /* display input characters */
NOSHARE int DNEAR modeflag = TRUE;      /* display modelines flag   */
NOSHARE int DNEAR timeflag = FALSE;     /* display time         */
NOSHARE int DNEAR undoflag = TRUE;      /* processing undo commands */
NOSHARE OBJECT obj;                     /* scratch undo object      */
NOSHARE int DNEAR undoing = FALSE;      /* currently undoing a command? */
NOSHARE char DNEAR lasttime[6] = "";    /* last time string displayed   */
NOSHARE int DNEAR popflag = TRUE;       /* pop-up windows enabled?  */
NOSHARE int DNEAR popwait = TRUE;       /* user wait on pops enabled?   */
NOSHARE int DNEAR posflag = FALSE;      /* display point position   */
NOSHARE int cpending = FALSE;           /* input character pending? */
NOSHARE int charpending;                /* character pushed back    */
NOSHARE int DNEAR sscroll = FALSE;      /* smooth scrolling enabled flag*/
NOSHARE int DNEAR hscroll = TRUE;       /* horizontal scrolling flag    */
NOSHARE int DNEAR hscrollbar = TRUE;    /* horizontal scroll bar flag   */
NOSHARE int DNEAR vscrollbar = TRUE;    /* vertical scroll bar flag */
NOSHARE int DNEAR hjump = 1;            /* horizontal jump size     */
NOSHARE int DNEAR ssave = TRUE;         /* safe save flag       */
NOSHARE struct BUFFER *bstore = NULL;   /* buffer to store macro text to*/
NOSHARE int DNEAR vtrow = 0;            /* Row location of SW cursor    */
NOSHARE int DNEAR vtcol = 0;            /* Column location of SW cursor */
NOSHARE int DNEAR ttrow = HUGENUM;      /* Row location of HW cursor    */
NOSHARE int DNEAR ttcol = HUGENUM;      /* Column location of HW cursor */
NOSHARE int DNEAR lbound = 0;           /* leftmost column of current line being
                                         * displayed        */
NOSHARE int DNEAR taboff = 0;           /* tab offset for display   */
NOSHARE int DNEAR tabsize = 8;          /* current hard tab size    */
NOSHARE int DNEAR stabsize = 0;         /* current soft tab size (0: use hard
                                         * tabs)  */
NOSHARE int DNEAR reptc = CTRL | 'U';   /* current universal repeat char*/
NOSHARE int DNEAR abortc = CTRL | 'G';  /* current abort command char   */
NOSHARE int DNEAR sterm = CTRL | '[';   /* search terminating character */
NOSHARE int DNEAR isterm = CTRL | '[';  /* incremental-search terminating char
                                         */
NOSHARE int DNEAR searchtype = SRNORM;  /* current search style     */
NOSHARE int DNEAR yankflag = FALSE;     /* current yank style       */

NOSHARE int DNEAR prefix = 0;           /* currently pending prefix bits */
NOSHARE int DNEAR prenum = 0;           /*     "       "     numeric arg */
NOSHARE int DNEAR predef = TRUE;        /*     "       "     default flag */

NOSHARE int DNEAR quotec = CTRL | 'Q';  /* quote char during mlreply() */
NOSHARE CONST char *cname[] =           /* names of colors      */
{
    "BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGENTA", "CYAN", "GREY",
    "GRAY", "LRED", "LGREEN", "LYELLOW", "LBLUE", "LMAGENTA", "LCYAN", "WHITE"
};

NOSHARE int kill_index;                 /* current index into kill ring */
NOSHARE KILL *kbufp[NRING];             /* current kill buffer chunk pointer*/
NOSHARE KILL *kbufh[NRING];             /* kill buffer header pointer   */
NOSHARE int kskip[NRING];               /* # of bytes to skip in 1st kill chunk
                                         */
NOSHARE int kused[NRING];               /* # of bytes used in last kill chunk*/
NOSHARE EWINDOW *swindow = NULL;        /* saved window pointer     */
NOSHARE int cryptflag = FALSE;          /* currently encrypting?    */
NOSHARE int oldcrypt = FALSE;           /* using old(broken) encryption? */
NOSHARE short *kbdptr;                  /* current position in keyboard buf */
NOSHARE short *kbdend = &kbdm[0];       /* ptr to end of the keyboard */
NOSHARE int DNEAR kbdmode = STOP;       /* current keyboard macro mode  */
NOSHARE int DNEAR kbdrep = 0;           /* number of repetitions    */
NOSHARE int DNEAR restflag = FALSE;     /* restricted use?      */
NOSHARE int DNEAR lastkey = 0;          /* last keystoke        */
NOSHARE long DNEAR seed = 1L;           /* random number seed       */
NOSHARE long envram = 0l;               /* # of bytes current in use by malloc
                                         */
NOSHARE long access_time = 0L;          /* counter of buffer access */
NOSHARE int DNEAR macbug = FALSE;       /* macro debugging flag     */
NOSHARE int DNEAR mouseflag = TRUE;     /* use the mouse?       */
NOSHARE int DNEAR diagflag = FALSE;     /* diagonal mouse movements?    */
CONST char errorm[] = "ERROR";          /* error literal        */
CONST char truem[] = "TRUE";            /* true literal         */
CONST char falsem[] = "FALSE";          /* false litereal       */
NOSHARE int DNEAR cmdstatus = TRUE;     /* last command status      */
NOSHARE char palstr[palstr_LEN] = "";   /* palette string       */
NOSHARE char lastmesg[NSTRING] = "";    /* last message posted      */
NOSHARE char rval[NSTRING] = "0";       /* result of last procedure/sub */
NOSHARE char *lastptr = NULL;           /* ptr to lastmesg[]        */
NOSHARE int DNEAR saveflag = 0;         /* Flags, saved with the $target var */
NOSHARE char *fline = NULL;             /* dynamic return line      */
NOSHARE int DNEAR flen = 0;             /* current length of fline  */
NOSHARE int DNEAR eexitflag = FALSE;    /* EMACS exit flag      */
NOSHARE int DNEAR eexitval = 0;         /* and the exit return value    */
NOSHARE int xpos = 0;           /* current column mouse is positioned to*/
NOSHARE int ypos = 0;           /* current screen row        "      */
NOSHARE int nclicks = 0;        /* cleared on any non-mouse event   */
NOSHARE int disphigh = FALSE;   /* display high bit chars escaped   */
NOSHARE int dispundo = FALSE;   /* display undo stack depth on comand line */
NOSHARE int defferupdate = FALSE;       /* if TRUE, update(TRUE) should be
                                         * called before yielding to another
                                         * Windows application */
NOSHARE int notquiescent = 1;           /* <=0 only when getkey called directly
                                         * by editloop () */
NOSHARE int fbusy = FALSE;              /* indicates file activity if FREADING
                                         * or FWRITING. Used by abort mechanism
                                         */
NOSHARE int hilite = 10;                /* current region to highlight (255 if
                                         * none) */

/* uninitialized global definitions */

NOSHARE int oquote;             /* open quote variable */
NOSHARE int cquote;             /* close quote variable */
NOSHARE int DNEAR currow;       /* Cursor row           */
NOSHARE int DNEAR curcol;       /* Cursor column        */
NOSHARE int DNEAR thisflag;     /* Flags, this command      */
NOSHARE int DNEAR lastflag;     /* Flags, last command      */
NOSHARE int DNEAR curgoal;      /* Goal for C-P, C-N        */
NOSHARE EWINDOW *curwp;         /* Current window       */
NOSHARE BUFFER *curbp;          /* Current buffer       */
NOSHARE EWINDOW *wheadp;        /* Head of list of windows  */
NOSHARE BUFFER *bheadp;         /* Head of list of buffers  */
NOSHARE UTABLE *uv_head;        /* head of list of user variables */
NOSHARE UTABLE *uv_global;      /* global variable table */
NOSHARE ABBREV *ab_head;        /* head of the abbreviation list */
NOSHARE int DNEAR ab_bell;      /* are we ringing the bell on completion? */
NOSHARE int DNEAR ab_cap;       /* match capatilization on expansion? */
NOSHARE int DNEAR ab_quick;     /* aggressive completion enabled? */
NOSHARE char ab_word[NSTRING];  /* current word being typed */
NOSHARE char *ab_pos;           /* current place in ab_word */
NOSHARE char *ab_end;           /* ptr to physical end of ab_word */
NOSHARE SCREEN_T *first_screen; /* Head and current screen in list */
NOSHARE BUFFER *blistp;         /* Buffer for C-X C-B       */
NOSHARE BUFFER *ulistp;         /* Buffer for C-X U     */
NOSHARE BUFFER *slistp;         /* Buffer for A-B       */

NOSHARE char sres[NBUFN];       /* current screen resolution    */
NOSHARE char os[NBUFN];         /* what OS are we running under */

NOSHARE char lowcase[HICHAR];   /* lower casing map     */
NOSHARE char upcase[HICHAR];    /* upper casing map     */

NOSHARE unsigned char pat[NPAT];        /* Search pattern       */
NOSHARE unsigned char tap[NPAT];        /* Reversed pattern array.  */
NOSHARE unsigned char rpat[NPAT];       /* replacement pattern      */

/*  Various "Hook" execution variables  */

NOSHARE KEYTAB readhook;        /* executed on all file reads */
NOSHARE KEYTAB wraphook;        /* executed when wrapping text */
NOSHARE KEYTAB cmdhook;         /* executed before looking for a command */
NOSHARE KEYTAB writehook;       /* executed on all file writes */
NOSHARE KEYTAB exbhook;         /* executed when exiting a buffer */
NOSHARE KEYTAB bufhook;         /* executed when entering a buffer */
NOSHARE KEYTAB exithook;        /* executed when exiting emacs */

/* The variables matchline and matchoff hold the line and offset position of the
 * *start* of the match. The variable patmatch holds the string that satisfies
 * the search command.
 */
NOSHARE int matchlen;
NOSHARE int matchoff;
NOSHARE LINE    *matchline;
NOSHARE char *patmatch = NULL;

# if     MAGIC
/*
 * The variables magical and rmagical determine if there were actual
 * metacharacters in the search and replace strings -
 * if not, then we don't have to use the slower MAGIC mode search functions.
 */
NOSHARE short int DNEAR magical = FALSE;
NOSHARE short int DNEAR rmagical = FALSE;

NOSHARE MC mcpat[NPAT];         /* the magic pattern        */
NOSHARE MC tapcm[NPAT];         /* the reversed magic pattern   */
NOSHARE MC mcdeltapat[2];       /* the no-magic pattern     */
NOSHARE MC tapatledcm[2];       /* the reversed no-magic pattern*/
NOSHARE RMC rmcpat[NPAT];       /* the replacement magic array  */
NOSHARE char *grpmatch[MAXGROUPS];      /* holds groups found in search */

# endif

DELTA deltapat;         /* Forward pattern delta structure.*/
DELTA tapatled;         /* Reverse pattern delta structure.*/

/* directive name table:
 *       This holds the names of all the directives.... */

CONST char *dname[] =
{
    "if", "else", "endif", "goto", "return", "endm", "while", "endwhile",
    "break", "force"
};
/* directive lengths        */
CONST short int dname_len[NUMDIRS] = { 2, 4, 5, 4, 6, 4, 5, 7, 5, 5 };

/*  var needed for macro debugging output   */
NOSHARE char outline[NSTRING];  /* global string to hold debug line text */

/*
 * System message notification (at the moment, VMS only).
 */
# if     VMS
NOSHARE char brdcstbuf[1024];           /* Broadcast messages */
NOSHARE int pending_msg = FALSE;        /* Flag - have we notified user yet.*/
# endif

# if     HANDLE_WINCH
int winch_flag=0;                       /* Window size changed flag */
# endif

#else /* !maindef */

/* for all the other .C files */

/* initialized global external declarations */

extern NOSHARE int DNEAR fillcol;       /* Current fill column      */
extern NOSHARE short kbdm[DUMMYSZ];     /* Holds keyboard macro data    */
extern NOSHARE char *execstr;           /* pointer to string to execute */
extern NOSHARE char golabel[DUMMYSZ];   /* current line to go to    */
extern NOSHARE char paralead[DUMMYSZ];  /* paragraph leadin chars   */
extern NOSHARE char fmtlead[DUMMYSZ];   /* format command leadin chars  */
extern NOSHARE char mainbuf[DUMMYSZ];   /* name of main buffer      */
extern NOSHARE char lterm[DUMMYSZ];     /* line terminators on file write */
extern NOSHARE unsigned char wordlist[DUMMYSZ];/* characters considered "in
                                                * words" */
extern NOSHARE int DNEAR wlflag;        /* word list enabled flag   */
extern NOSHARE int DNEAR clearflag;     /* clear screen on screen change? */
extern NOSHARE int DNEAR execlevel;     /* execution IF level       */
extern NOSHARE int DNEAR eolexist;      /* does clear to EOL exist? */
extern NOSHARE int DNEAR revexist;      /* does reverse video exist?    */
extern NOSHARE int DNEAR exec_error;    /* macro execution error pending? */
extern NOSHARE int DNEAR flickcode;     /* do flicker supression?   */
extern NOSHARE int DNEAR mouse_move;    /* user allow tracking mouse moves? */
extern NOSHARE int DNEAR mmove_flag;    /* code currently allowing mmoves? */
extern NOSHARE int DNEAR newscreenflag; /* Create new screen on reads? */
extern NOSHARE int DNEAR overlap;       /* overlap when paging screens */
extern NOSHARE int DNEAR parindent;     /* parindent to be used with fill */
extern CONST char *modename[DUMMYSZ];   /* text names of modes      */
extern CONST char modecode[DUMMYSZ];    /* letters to represent modes   */
extern NOSHARE int DNEAR numfunc;       /* number of bindable functions */
extern NOSHARE KEYTAB keytab[DUMMYSZ];  /* key bind to functions table  */
extern NOSHARE NBIND names[DUMMYSZ];    /* name to function table   */
extern NOSHARE int DNEAR gmode;         /* global editor mode       */
extern NOSHARE int DNEAR gflags;        /* global control flag      */
extern NOSHARE int DNEAR gfcolor;       /* global forgrnd color (white) */
extern NOSHARE int DNEAR gbcolor;       /* global backgrnd color (black)*/
extern NOSHARE int DNEAR deskcolor;     /* desktop background color */
extern NOSHARE int DNEAR gasave;        /* global ASAVE size        */
extern NOSHARE int DNEAR gacount;       /* count until next ASAVE   */
extern NOSHARE int DNEAR sgarbf;        /* State of screen unknown  */
extern NOSHARE int DNEAR mpresf;        /* Stuff in message line    */
extern NOSHARE int DNEAR clexec;        /* command line execution flag  */
extern NOSHARE int DNEAR mstore;        /* storing text to macro flag   */
extern NOSHARE int DNEAR discmd;        /* display command flag     */
extern NOSHARE int DNEAR disinp;        /* display input characters */
extern NOSHARE int DNEAR modeflag;      /* display modelines flag   */
extern NOSHARE int DNEAR timeflag;      /* display time         */
extern NOSHARE int DNEAR undoflag;      /* processing undo commands */
extern NOSHARE OBJECT obj;              /* scratch undo object      */
extern NOSHARE int DNEAR undoing;       /* currently undoing a command? */
extern NOSHARE char DNEAR lasttime[DUMMYSZ];/* last time string displayed*/
extern NOSHARE int DNEAR popflag;       /* pop-up windows enabled?  */
extern NOSHARE int DNEAR popwait;       /* user wait on pops enabled?   */
extern NOSHARE int DNEAR posflag;       /* display point position   */
extern NOSHARE int cpending;            /* input character pending? */
extern NOSHARE int charpending;         /* character pushed back    */
extern NOSHARE int DNEAR sscroll;       /* smooth scrolling enabled flag*/
extern NOSHARE int DNEAR hscroll;       /* horizontal scrolling flag    */
extern NOSHARE int DNEAR hscrollbar;    /* horizontal scroll bar flag   */
extern NOSHARE int DNEAR vscrollbar;    /* vertical scroll bar flag */
extern NOSHARE int DNEAR hjump;         /* horizontal jump size     */
extern NOSHARE int DNEAR ssave;         /* safe save flag       */
extern NOSHARE struct BUFFER *bstore;   /* buffer to store macro text to*/
extern NOSHARE int DNEAR vtrow;         /* Row location of SW cursor    */
extern NOSHARE int DNEAR vtcol;         /* Column location of SW cursor */
extern NOSHARE int DNEAR ttrow;         /* Row location of HW cursor    */
extern NOSHARE int DNEAR ttcol;         /* Column location of HW cursor */
extern NOSHARE int DNEAR lbound;        /* leftmost column of current line being
                                         * displayed        */
extern NOSHARE int DNEAR taboff;        /* tab offset for display   */
extern NOSHARE int DNEAR tabsize;       /* current hard tab size    */
extern NOSHARE int DNEAR stabsize;      /* current soft tab size (0: use hard
                                         * tabs)  */
extern NOSHARE int DNEAR reptc;         /* current universal repeat char*/
extern NOSHARE int DNEAR abortc;        /* current abort command char   */
extern NOSHARE int DNEAR sterm;         /* search terminating character */
extern NOSHARE int DNEAR isterm;        /* incremental-search terminating char
                                         */
extern NOSHARE int DNEAR searchtype;    /* current search style     */
extern NOSHARE int DNEAR yankflag;      /* current yank style       */

extern NOSHARE int DNEAR prefix;        /* currently pending prefix bits */
extern NOSHARE int DNEAR prenum;        /*     "       "     numeric arg */
extern NOSHARE int DNEAR predef;        /*     "       "     default flag */

extern NOSHARE int DNEAR quotec;        /* quote char during mlreply() */
extern NOSHARE CONST char *cname[DUMMYSZ];/* names of colors        */

extern NOSHARE int kill_index;          /* current index into kill ring */
extern NOSHARE KILL *kbufp[DUMMYSZ];    /* current kill buffer chunk pointer */
extern NOSHARE KILL *kbufh[DUMMYSZ];    /* kill buffer header pointer   */
extern NOSHARE int kskip[DUMMYSZ];      /* # of bytes to skip in 1st kill chunk
                                         */
extern NOSHARE int kused[DUMMYSZ];      /* # of bytes used in kill buffer*/
extern NOSHARE EWINDOW *swindow;        /* saved window pointer     */
extern NOSHARE int cryptflag;           /* currently encrypting?    */
extern NOSHARE int oldcrypt;            /* using old(broken) encryption? */
extern NOSHARE short *kbdptr;           /* current position in keyboard buf */
extern NOSHARE short *kbdend;           /* ptr to end of the keyboard */
extern NOSHARE int kbdmode;             /* current keyboard macro mode  */
extern NOSHARE int kbdrep;              /* number of repetitions    */
extern NOSHARE int restflag;            /* restricted use?      */
extern NOSHARE int lastkey;             /* last keystoke        */
extern NOSHARE long seed;               /* random number seed       */
extern NOSHARE long envram;             /* # of bytes current in use by malloc
                                         */
extern NOSHARE long access_time;        /* counter of buffer access */
extern NOSHARE int DNEAR macbug;        /* macro debugging flag     */
extern NOSHARE int DNEAR mouseflag;     /* use the mouse?       */
extern NOSHARE int DNEAR diagflag;      /* diagonal mouse movements?    */
extern CONST char errorm[DUMMYSZ];      /* error literal        */
extern CONST char truem[DUMMYSZ];       /* true literal         */
extern CONST char falsem[DUMMYSZ];      /* false litereal       */
extern NOSHARE int DNEAR cmdstatus;     /* last command status      */
extern NOSHARE char palstr[DUMMYSZ];    /* palette string       */
extern NOSHARE char lastmesg[DUMMYSZ];  /* last message posted      */
extern NOSHARE char rval[DUMMYSZ];      /* result of last procedure */
extern NOSHARE char *lastptr;           /* ptr to lastmesg[]        */
extern NOSHARE int DNEAR saveflag;      /* Flags, saved with the $target var */
extern NOSHARE char *fline;             /* dynamic return line */
extern NOSHARE int DNEAR flen;          /* current length of fline */
extern NOSHARE int DNEAR eexitflag;     /* EMACS exit flag */
extern NOSHARE int DNEAR eexitval;      /* and the exit return value */
extern NOSHARE int xpos;                /* current column mouse is positioned to
                                         */
extern NOSHARE int ypos;                /* current screen row        "   */
extern NOSHARE int nclicks;             /* cleared on any non-mouse event*/
extern NOSHARE int disphigh;            /* display high bit chars escaped*/
extern NOSHARE int dispundo;    /* display undo stack depth on comand line */
extern NOSHARE int defferupdate;        /* if TRUE, update(TRUE) should be
                                         * called before yielding to another
                                         * Windows application */
extern NOSHARE int notquiescent;        /* <=0 only when getkey called directly
                                         * by editloop () */
extern NOSHARE int fbusy;               /* indicates file activity if FREADING
                                         * or FWRITING. Used by abort mechanism
                                         */
extern NOSHARE int hilite;              /* current region to highlight (255 if
                                         * none) */

/* uninitialized global external declarations */

extern NOSHARE int oquote;              /* open quote variable */
extern NOSHARE int cquote;              /* close quote variable */
extern NOSHARE int DNEAR currow;        /* Cursor row           */
extern NOSHARE int DNEAR curcol;        /* Cursor column        */
extern NOSHARE int DNEAR thisflag;      /* Flags, this command      */
extern NOSHARE int DNEAR lastflag;      /* Flags, last command      */
extern NOSHARE int DNEAR curgoal;       /* Goal for C-P, C-N        */
extern NOSHARE EWINDOW *curwp;          /* Current window       */
extern NOSHARE BUFFER *curbp;           /* Current buffer       */
extern NOSHARE EWINDOW *wheadp;         /* Head of list of windows  */
extern NOSHARE BUFFER *bheadp;          /* Head of list of buffers  */
extern NOSHARE ABBREV *ab_head;         /* head of the abbreviation list */
extern NOSHARE UTABLE *uv_head;         /* head of list of user variables */
extern NOSHARE UTABLE *uv_global;       /* global variable table */
extern NOSHARE int DNEAR ab_bell;       /* are we ringing the bell on
                                         * completion? */
extern NOSHARE int DNEAR ab_cap;        /* match capatilization on expansion? */
extern NOSHARE int DNEAR ab_quick;      /* aggressive completion enabled? */
extern NOSHARE char ab_word[DUMMYSZ];   /* current word being typed */
extern NOSHARE char *ab_pos;            /* current place in ab_word */
extern NOSHARE char *ab_end;            /* ptr to physical end of ab_word */
extern NOSHARE SCREEN_T *first_screen;  /* Head and current screen in list */
extern NOSHARE BUFFER *blistp;          /* Buffer for C-X C-B       */
extern NOSHARE BUFFER *ulistp;          /* Buffer for C-X U     */
extern NOSHARE BUFFER *slistp;          /* Buffer for A-B       */

extern NOSHARE char sres[NBUFN];        /* current screen resolution    */
extern NOSHARE char os[NBUFN];          /* what OS are we running under */

extern NOSHARE char lowcase[HICHAR];    /* lower casing map     */
extern NOSHARE char upcase[HICHAR];     /* upper casing map     */

extern NOSHARE unsigned char pat[DUMMYSZ]; /* Search pattern        */
extern NOSHARE unsigned char tap[DUMMYSZ]; /* Reversed pattern array.   */
extern NOSHARE unsigned char rpat[DUMMYSZ]; /* replacement pattern  */

/*  Various "Hook" execution variables  */

extern NOSHARE KEYTAB readhook;         /* executed on all file reads */
extern NOSHARE KEYTAB wraphook;         /* executed when wrapping text */
extern NOSHARE KEYTAB cmdhook;          /* executed before looking for a cmd */
extern NOSHARE KEYTAB writehook;        /* executed on all file writes */
extern NOSHARE KEYTAB exbhook;          /* executed when exiting a buffer */
extern NOSHARE KEYTAB bufhook;          /* executed when entering a buffer */
extern NOSHARE KEYTAB exithook;         /* executed when exiting emacs */

extern NOSHARE int matchlen;
extern NOSHARE int matchoff;
extern NOSHARE LINE *matchline;
extern NOSHARE char *patmatch;

# if     MAGIC
extern NOSHARE short int magical;
extern NOSHARE short int rmagical;
extern NOSHARE MC mcpat[NPAT];          /* the magic pattern        */
extern NOSHARE MC tapcm[NPAT];          /* the reversed magic pattern   */
extern NOSHARE MC mcdeltapat[2];        /* the no-magic pattern     */
extern NOSHARE MC tapatledcm[2];        /* the reversed no-magic pattern*/
extern NOSHARE RMC rmcpat[NPAT];        /* the replacement magic array  */
extern NOSHARE char *grpmatch[MAXGROUPS];       /* holds groups found in search
                                                 */
# endif

extern NOSHARE DELTA deltapat;  /* Forward pattern delta structure.*/
extern NOSHARE DELTA tapatled;  /* Reverse pattern delta structure.*/

extern CONST char *dname[DUMMYSZ];      /* directive name table     */
extern CONST short int dname_len[NUMDIRS]; /* directive lengths     */

/*  var needed for macro debugging output   */
extern NOSHARE char outline[DUMMYSZ];   /* global string to hold debug line text
                                         */

/*
 * System message notification (at the moment, VMS only).
 */
# if     VMS
extern NOSHARE char brdcstbuf[1024];            /* Broadcast messages */
extern NOSHARE int pending_msg;         /* Flag - have we notified user yet.*/
# endif

# if     HANDLE_WINCH
extern NOSHARE int winch_flag;          /* Window size changed flag */
# endif

#endif  /* maindef  */


/* terminal table defined only in TERM.C */

#ifndef termdef
extern NOSHARE TERM term;               /* Terminal information.    */
#endif



/*====================================================================*/
#endif/**#ifndef EEDEF_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
