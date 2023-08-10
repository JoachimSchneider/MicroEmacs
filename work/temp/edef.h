/*  EDEF:       Global variable definitions for MicroEMACS 3.9
 *
 *                       written by Daniel Lawrence based on code by Dave G.
 * Conroy, Steve Wilhite and George Jones
 */

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
NOSHARE char palstr[49] = "";           /* palette string       */
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

#else

/* for all the other .C files */

/* initialized global external declarations */

NOSHARE EXTERN int DNEAR fillcol;       /* Current fill column      */
NOSHARE EXTERN short kbdm[DUMMYSZ];     /* Holds keyboard macro data    */
NOSHARE EXTERN char *execstr;           /* pointer to string to execute */
NOSHARE EXTERN char golabel[DUMMYSZ];   /* current line to go to    */
NOSHARE EXTERN char paralead[DUMMYSZ];  /* paragraph leadin chars   */
NOSHARE EXTERN char fmtlead[DUMMYSZ];   /* format command leadin chars  */
NOSHARE EXTERN char mainbuf[DUMMYSZ];   /* name of main buffer      */
NOSHARE EXTERN char lterm[DUMMYSZ];     /* line terminators on file write */
NOSHARE EXTERN unsigned char wordlist[DUMMYSZ];/* characters considered "in
                                                * words" */
NOSHARE EXTERN int DNEAR wlflag;        /* word list enabled flag   */
NOSHARE EXTERN int DNEAR clearflag;     /* clear screen on screen change? */
NOSHARE EXTERN int DNEAR execlevel;     /* execution IF level       */
NOSHARE EXTERN int DNEAR eolexist;      /* does clear to EOL exist? */
NOSHARE EXTERN int DNEAR revexist;      /* does reverse video exist?    */
NOSHARE EXTERN int DNEAR exec_error;    /* macro execution error pending? */
NOSHARE EXTERN int DNEAR flickcode;     /* do flicker supression?   */
NOSHARE EXTERN int DNEAR mouse_move;    /* user allow tracking mouse moves? */
NOSHARE EXTERN int DNEAR mmove_flag;    /* code currently allowing mmoves? */
NOSHARE EXTERN int DNEAR newscreenflag; /* Create new screen on reads? */
NOSHARE EXTERN int DNEAR overlap;       /* overlap when paging screens */
NOSHARE EXTERN int DNEAR parindent;     /* parindent to be used with fill */
CONST EXTERN char *modename[DUMMYSZ];   /* text names of modes      */
CONST EXTERN char modecode[DUMMYSZ];    /* letters to represent modes   */
NOSHARE EXTERN int DNEAR numfunc;       /* number of bindable functions */
NOSHARE EXTERN KEYTAB keytab[DUMMYSZ];  /* key bind to functions table  */
NOSHARE EXTERN NBIND names[DUMMYSZ];    /* name to function table   */
NOSHARE EXTERN int DNEAR gmode;         /* global editor mode       */
NOSHARE EXTERN int DNEAR gflags;        /* global control flag      */
NOSHARE EXTERN int DNEAR gfcolor;       /* global forgrnd color (white) */
NOSHARE EXTERN int DNEAR gbcolor;       /* global backgrnd color (black)*/
NOSHARE EXTERN int DNEAR deskcolor;     /* desktop background color */
NOSHARE EXTERN int DNEAR gasave;        /* global ASAVE size        */
NOSHARE EXTERN int DNEAR gacount;       /* count until next ASAVE   */
NOSHARE EXTERN int DNEAR sgarbf;        /* State of screen unknown  */
NOSHARE EXTERN int DNEAR mpresf;        /* Stuff in message line    */
NOSHARE EXTERN int DNEAR clexec;        /* command line execution flag  */
NOSHARE EXTERN int DNEAR mstore;        /* storing text to macro flag   */
NOSHARE EXTERN int DNEAR discmd;        /* display command flag     */
NOSHARE EXTERN int DNEAR disinp;        /* display input characters */
NOSHARE EXTERN int DNEAR modeflag;      /* display modelines flag   */
NOSHARE EXTERN int DNEAR timeflag;      /* display time         */
NOSHARE EXTERN int DNEAR undoflag;      /* processing undo commands */
NOSHARE EXTERN OBJECT obj;              /* scratch undo object      */
NOSHARE EXTERN int DNEAR undoing;       /* currently undoing a command? */
NOSHARE EXTERN char DNEAR lasttime[DUMMYSZ];/* last time string displayed*/
NOSHARE EXTERN int DNEAR popflag;       /* pop-up windows enabled?  */
NOSHARE EXTERN int DNEAR popwait;       /* user wait on pops enabled?   */
NOSHARE EXTERN int DNEAR posflag;       /* display point position   */
NOSHARE EXTERN int cpending;            /* input character pending? */
NOSHARE EXTERN int charpending;         /* character pushed back    */
NOSHARE EXTERN int DNEAR sscroll;       /* smooth scrolling enabled flag*/
NOSHARE EXTERN int DNEAR hscroll;       /* horizontal scrolling flag    */
NOSHARE EXTERN int DNEAR hscrollbar;    /* horizontal scroll bar flag   */
NOSHARE EXTERN int DNEAR vscrollbar;    /* vertical scroll bar flag */
NOSHARE EXTERN int DNEAR hjump;         /* horizontal jump size     */
NOSHARE EXTERN int DNEAR ssave;         /* safe save flag       */
NOSHARE EXTERN struct BUFFER *bstore;   /* buffer to store macro text to*/
NOSHARE EXTERN int DNEAR vtrow;         /* Row location of SW cursor    */
NOSHARE EXTERN int DNEAR vtcol;         /* Column location of SW cursor */
NOSHARE EXTERN int DNEAR ttrow;         /* Row location of HW cursor    */
NOSHARE EXTERN int DNEAR ttcol;         /* Column location of HW cursor */
NOSHARE EXTERN int DNEAR lbound;        /* leftmost column of current line being
                                         * displayed        */
NOSHARE EXTERN int DNEAR taboff;        /* tab offset for display   */
NOSHARE EXTERN int DNEAR tabsize;       /* current hard tab size    */
NOSHARE EXTERN int DNEAR stabsize;      /* current soft tab size (0: use hard
                                         * tabs)  */
NOSHARE EXTERN int DNEAR reptc;         /* current universal repeat char*/
NOSHARE EXTERN int DNEAR abortc;        /* current abort command char   */
NOSHARE EXTERN int DNEAR sterm;         /* search terminating character */
NOSHARE EXTERN int DNEAR isterm;        /* incremental-search terminating char
                                         */
NOSHARE EXTERN int DNEAR searchtype;    /* current search style     */
NOSHARE EXTERN int DNEAR yankflag;      /* current yank style       */

NOSHARE EXTERN int DNEAR prefix;        /* currently pending prefix bits */
NOSHARE EXTERN int DNEAR prenum;        /*     "       "     numeric arg */
NOSHARE EXTERN int DNEAR predef;        /*     "       "     default flag */

NOSHARE EXTERN int DNEAR quotec;        /* quote char during mlreply() */
NOSHARE EXTERN CONST char *cname[DUMMYSZ];/* names of colors        */

NOSHARE EXTERN int kill_index;          /* current index into kill ring */
NOSHARE EXTERN KILL *kbufp[DUMMYSZ];    /* current kill buffer chunk pointer */
NOSHARE EXTERN KILL *kbufh[DUMMYSZ];    /* kill buffer header pointer   */
NOSHARE EXTERN int kskip[DUMMYSZ];      /* # of bytes to skip in 1st kill chunk
                                         */
NOSHARE EXTERN int kused[DUMMYSZ];      /* # of bytes used in kill buffer*/
NOSHARE EXTERN EWINDOW *swindow;        /* saved window pointer     */
NOSHARE EXTERN int cryptflag;           /* currently encrypting?    */
NOSHARE EXTERN int oldcrypt;            /* using old(broken) encryption? */
NOSHARE EXTERN short *kbdptr;           /* current position in keyboard buf */
NOSHARE EXTERN short *kbdend;           /* ptr to end of the keyboard */
NOSHARE EXTERN int kbdmode;             /* current keyboard macro mode  */
NOSHARE EXTERN int kbdrep;              /* number of repetitions    */
NOSHARE EXTERN int restflag;            /* restricted use?      */
NOSHARE EXTERN int lastkey;             /* last keystoke        */
NOSHARE EXTERN long seed;               /* random number seed       */
NOSHARE EXTERN long envram;             /* # of bytes current in use by malloc
                                         */
NOSHARE EXTERN long access_time;        /* counter of buffer access */
NOSHARE EXTERN int DNEAR macbug;        /* macro debugging flag     */
NOSHARE EXTERN int DNEAR mouseflag;     /* use the mouse?       */
NOSHARE EXTERN int DNEAR diagflag;      /* diagonal mouse movements?    */
CONST EXTERN char errorm[DUMMYSZ];      /* error literal        */
CONST EXTERN char truem[DUMMYSZ];       /* true literal         */
CONST EXTERN char falsem[DUMMYSZ];      /* false litereal       */
NOSHARE EXTERN int DNEAR cmdstatus;     /* last command status      */
NOSHARE EXTERN char palstr[DUMMYSZ];    /* palette string       */
NOSHARE EXTERN char lastmesg[DUMMYSZ];  /* last message posted      */
NOSHARE EXTERN char rval[DUMMYSZ];      /* result of last procedure */
NOSHARE EXTERN char *lastptr;           /* ptr to lastmesg[]        */
NOSHARE EXTERN int DNEAR saveflag;      /* Flags, saved with the $target var */
NOSHARE EXTERN char *fline;             /* dynamic return line */
NOSHARE EXTERN int DNEAR flen;          /* current length of fline */
NOSHARE EXTERN int DNEAR eexitflag;     /* EMACS exit flag */
NOSHARE EXTERN int DNEAR eexitval;      /* and the exit return value */
NOSHARE EXTERN int xpos;                /* current column mouse is positioned to
                                         */
NOSHARE EXTERN int ypos;                /* current screen row        "   */
NOSHARE EXTERN int nclicks;             /* cleared on any non-mouse event*/
NOSHARE EXTERN int disphigh;            /* display high bit chars escaped*/
NOSHARE EXTERN int dispundo;    /* display undo stack depth on comand line */
NOSHARE EXTERN int defferupdate;        /* if TRUE, update(TRUE) should be
                                         * called before yielding to another
                                         * Windows application */
NOSHARE EXTERN int notquiescent;        /* <=0 only when getkey called directly
                                         * by editloop () */
NOSHARE EXTERN int fbusy;               /* indicates file activity if FREADING
                                         * or FWRITING. Used by abort mechanism
                                         */
NOSHARE EXTERN int hilite;              /* current region to highlight (255 if
                                         * none) */

/* uninitialized global external declarations */

NOSHARE EXTERN int oquote;              /* open quote variable */
NOSHARE EXTERN int cquote;              /* close quote variable */
NOSHARE EXTERN int DNEAR currow;        /* Cursor row           */
NOSHARE EXTERN int DNEAR curcol;        /* Cursor column        */
NOSHARE EXTERN int DNEAR thisflag;      /* Flags, this command      */
NOSHARE EXTERN int DNEAR lastflag;      /* Flags, last command      */
NOSHARE EXTERN int DNEAR curgoal;       /* Goal for C-P, C-N        */
NOSHARE EXTERN EWINDOW *curwp;          /* Current window       */
NOSHARE EXTERN BUFFER *curbp;           /* Current buffer       */
NOSHARE EXTERN EWINDOW *wheadp;         /* Head of list of windows  */
NOSHARE EXTERN BUFFER *bheadp;          /* Head of list of buffers  */
NOSHARE EXTERN ABBREV *ab_head;         /* head of the abbreviation list */
NOSHARE EXTERN UTABLE *uv_head;         /* head of list of user variables */
NOSHARE EXTERN UTABLE *uv_global;       /* global variable table */
NOSHARE EXTERN int DNEAR ab_bell;       /* are we ringing the bell on
                                         * completion? */
NOSHARE EXTERN int DNEAR ab_cap;        /* match capatilization on expansion? */
NOSHARE EXTERN int DNEAR ab_quick;      /* aggressive completion enabled? */
NOSHARE EXTERN char ab_word[DUMMYSZ];   /* current word being typed */
NOSHARE EXTERN char *ab_pos;            /* current place in ab_word */
NOSHARE EXTERN char *ab_end;            /* ptr to physical end of ab_word */
NOSHARE EXTERN SCREEN_T *first_screen;  /* Head and current screen in list */
NOSHARE EXTERN BUFFER *blistp;          /* Buffer for C-X C-B       */
NOSHARE EXTERN BUFFER *ulistp;          /* Buffer for C-X U     */
NOSHARE EXTERN BUFFER *slistp;          /* Buffer for A-B       */

NOSHARE EXTERN char sres[NBUFN];        /* current screen resolution    */
NOSHARE EXTERN char os[NBUFN];          /* what OS are we running under */

NOSHARE EXTERN char lowcase[HICHAR];    /* lower casing map     */
NOSHARE EXTERN char upcase[HICHAR];     /* upper casing map     */

NOSHARE EXTERN unsigned char pat[DUMMYSZ]; /* Search pattern        */
NOSHARE EXTERN unsigned char tap[DUMMYSZ]; /* Reversed pattern array.   */
NOSHARE EXTERN unsigned char rpat[DUMMYSZ]; /* replacement pattern  */

/*  Various "Hook" execution variables  */

NOSHARE EXTERN KEYTAB readhook;         /* executed on all file reads */
NOSHARE EXTERN KEYTAB wraphook;         /* executed when wrapping text */
NOSHARE EXTERN KEYTAB cmdhook;          /* executed before looking for a cmd */
NOSHARE EXTERN KEYTAB writehook;        /* executed on all file writes */
NOSHARE EXTERN KEYTAB exbhook;          /* executed when exiting a buffer */
NOSHARE EXTERN KEYTAB bufhook;          /* executed when entering a buffer */
NOSHARE EXTERN KEYTAB exithook;         /* executed when exiting emacs */

NOSHARE EXTERN int matchlen;
NOSHARE EXTERN int matchoff;
NOSHARE EXTERN LINE *matchline;
NOSHARE EXTERN char *patmatch;

# if     MAGIC
NOSHARE EXTERN short int magical;
NOSHARE EXTERN short int rmagical;
NOSHARE EXTERN MC mcpat[NPAT];          /* the magic pattern        */
NOSHARE EXTERN MC tapcm[NPAT];          /* the reversed magic pattern   */
NOSHARE EXTERN MC mcdeltapat[2];        /* the no-magic pattern     */
NOSHARE EXTERN MC tapatledcm[2];        /* the reversed no-magic pattern*/
NOSHARE EXTERN RMC rmcpat[NPAT];        /* the replacement magic array  */
NOSHARE EXTERN char *grpmatch[MAXGROUPS];       /* holds groups found in search
                                                 */
# endif

NOSHARE EXTERN DELTA deltapat;  /* Forward pattern delta structure.*/
NOSHARE EXTERN DELTA tapatled;  /* Reverse pattern delta structure.*/

CONST EXTERN char *dname[DUMMYSZ];      /* directive name table     */
CONST EXTERN short int dname_len[NUMDIRS]; /* directive lengths     */

/*  var needed for macro debugging output   */
NOSHARE EXTERN char outline[DUMMYSZ];   /* global string to hold debug line text
                                         */

/*
 * System message notification (at the moment, VMS only).
 */
# if     VMS
NOSHARE EXTERN char brdcstbuf[1024];            /* Broadcast messages */
NOSHARE EXTERN int pending_msg;         /* Flag - have we notified user yet.*/
# endif

# if     HANDLE_WINCH
NOSHARE EXTERN int winch_flag;          /* Window size changed flag */
# endif

#endif

/* terminal table defined only in TERM.C */

#ifndef termdef
NOSHARE EXTERN TERM term;               /* Terminal information.    */
#endif

