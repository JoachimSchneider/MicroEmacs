/*      EPROTO:         Global function prototypes and declarations MicroEMACS
 * 4.00
 *
 *                       written by Daniel Lawrence based on code by Dave G.
 * Conroy, Steve Wilhite and George Jones
 */


extern char *xstrcpy(char *s1, CONST char *s2);             /* strcpy() possibly
                                                             * overlapping
                                                             * regions  */
extern char *xstrncpy(char *s1, CONST char *s2, int n);     /* strncpy()
                                                             * possibly
                                                             * overlapping
                                                             * regions */
extern int  xsnprintf(char *s, size_t n, CONST char *fmt, ...); /* Like C99
                                                                 * snprintf() */
extern char *xstrdup(CONST char *str);


/**********************************************************************/
#define TRC_FILE_ENVVAR "EMACS_TRC_FILE"
extern FILE *GetTrcFP(void);

extern int         DebugMessage_lnno_;
extern CONST char *DebugMessage_fname_;
extern int         DebugMessage(CONST char *fmt, ...);
#if ( defined( EMACS_TRC) )
# define  TRC(arg)  do {                        \
        DebugMessage_fname_ = __FILE__;         \
        DebugMessage_lnno_ = __LINE__;          \
        DebugMessage arg;                       \
    } while ( 0 )
# define  TRCK(arg, file, line)  do {           \
        DebugMessage_fname_ = (file);           \
        DebugMessage_lnno_ = (line);            \
        DebugMessage arg;                       \
    } while ( 0 )
#else
# define  TRC(arg)              do {} while ( 0 )
# define  TRCK(arg, file, line) do {} while ( 0 )
#endif
/**********************************************************************/

/**********************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <time.h>
/**********************************************************************/
#define STR(s)        ( ( NULL == (s) )?                                        \
                        ( (CONST char *)"@NIL@" ) : ( (CONST char *)(s) ) )
#define NELEM(A)      ( sizeof ( (A) )/sizeof ( (A)[0] ) )
#define SIZEOF(e)     ( (int)sizeof((e)) )
#define ZEROMEM(x)    ( memset(&(x), 0, sizeof((x))) )
#define MAXIMUM(x, y) ( ((x) < (y))? (y) : (x) )
#define MINIMUM(x, y) ( ((x) > (y))? (y) : (x) )
#define MIN2(x, y)    ( MINIMUM((x), (y)) )
#define MAX2(x, y)    ( MAXIMUM((x), (y)) )
#define MIN3(x, y, z) ( MIN2((x), MIN2((y), (z))) )
#define MAX3(x, y, z) ( MAX2((x), MAX2((y), (z))) )
/**********************************************************************/
#define MKSTRING(t)       #t
#define MKXSTRING(t)      MKSTRING(t)
/**********************************************************************/
/* If possible use XCONCAT* below -- CONCAT* might result in          */
/* undefined behaviour due to unspecified evaluation order for        */
/* multiple '##' operators.                                           */
/**********************************************************************/
#define CONCAT2(x, y)                           \
    x##y
#define CONCAT3(x, y, z)                        \
    x##y##z
#define CONCAT4(x1, x2, x3, x4)                 \
    x1##x2##x3##x4
#define CONCAT5(x1, x2, x3, x4, x5)             \
    x1##x2##x3##x4##x5
#define CONCAT6(x1, x2, x3, x4, x5, x6)         \
    x1##x2##x3##x4##x5##x6
#define CONCAT7(x1, x2, x3, x4, x5, x6, x7)     \
    x1##x2##x3##x4##x5##x6##x7
#define CONCAT8(x1, x2, x3, x4, x5, x6, x7, x8) \
    x1##x2##x3##x4##x5##x6##x7##x8
#define CONCAT9(x1, x2, x3, x4, x5, x6, x7, x8, x9)     \
    x1##x2##x3##x4##x5##x6##x7##x8##x9
/**********************************************************************/
/* Expand argument macros, concatenate from left to right.            */
/**********************************************************************/
#define XCONCAT2(x, y)                          \
    CONCAT2(x, y)
#define XCONCAT3(x, y, z)                       \
    XCONCAT2(XCONCAT2(x, y), z)
#define XCONCAT4(x1, x2, x3, x4)                \
    XCONCAT2(XCONCAT3(x1, x2, x3), x4)
#define XCONCAT5(x1, x2, x3, x4, x5)            \
    XCONCAT2(XCONCAT4(x1, x2, x3, x4), x5)
#define XCONCAT6(x1, x2, x3, x4, x5, x6)        \
    XCONCAT2(XCONCAT5(x1, x2, x3, x4, x5), x6)
#define XCONCAT7(x1, x2, x3, x4, x5, x6, x7)            \
    XCONCAT2(XCONCAT6(x1, x2, x3, x4, x5, x6), x7)
#define XCONCAT8(x1, x2, x3, x4, x5, x6, x7, x8)        \
    XCONCAT2(XCONCAT7(x1, x2, x3, x4, x5, x6, x7), x8)
#define XCONCAT9(x1, x2, x3, x4, x5, x6, x7, x8, x9)            \
    XCONCAT2(XCONCAT8(x1, x2, x3, x4, x5, x6, x7, x8), x9)
/*
 * This macro tests for pointer to something versus array of something.
 * It fails for the definition w. initialization `type *p = (type *)&p;'
 */
#define IS_ARRAY(a) ( (char *)(&(a)) == (char *)(&((a)[0])) )
/**********************************************************************/

/**********************************************************************/
static char *uitostr_memacs(unsigned int i)
{
    unsigned int  base  = 10;

    /*********************************************************************
     * a hat n (> 0) Dezimalstellen <==> 10^(n-1) <= a < 10^n
     *                              <==> n -1 <= log_10(a) < n
     * Also n <= 1 + log_10(a).
     *
     * a hat m (> 0) Stellen im 256-er System
     *                              <==> 256^(m-1) <= a < 256^n
     *                              <==> m -1 <= log_256(a) < m
     * Ausserdem m <= sizeof(a), also log_256(a) < sizeof(a)
     *
     * Umrechnung der Logarithmen:
     *  y = log_b x <==> b^y = x
     *  ==> y*log_a(b) = log_a(x)
     *  ==> log_a(x) = log_a(b)*log_b(x)
     *
     * log_10(a) = log_10(256)*log_256(a) < log_10(256)*sizeof(a)
     *
     * Damit n <= 1 + 3*sizeof(a)
     *
     ********************************************************************/
    static char buf[1 + 3*sizeof(i) + 1];
    int           pos   = sizeof(buf) - 2;
    unsigned int  rest  = 0;

    ZEROMEM(buf);

    for (;;)
    {
        rest  = i % base;
        i     = i / base;
        buf[pos--]  = '0' + rest;
        if ( 0 == i )
        {
            break;
        }
    }

    return buf + pos + 1;
}
/**********************************************************************/
#define eputs(s)      fputs((s), GetTrcFP())
#define eputi(i)      eputs(uitostr_memacs((unsigned int)(i)))
/**********************************************************************/

/**********************************************************************/
/*
 * CASRT(condition):
 *
 * Use this macro to test a condition at compile time instead of
 * runtime.  If condition is false, the compiler will abort with an
 * error.  This is really useful, if you want to check e.g. sizes of
 * structures against given needs.  Example: CASRT( sizeof(int) ==
 * sizeof(long) );
 */
#define CASRT(condition)  typedef int XCONCAT3(dummy_, __LINE__, _)[(condition)?1:-1];

/* We will use fputs instead of fprintf, because fprintf
   might use malloc(), but we want to use ASRT to exit
   when malloc() fails  */
#define ASRT(e) do {                                                    \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs(__FILE__); eputs(", Line: ");        \
            eputi(__LINE__); eputs("\n");                               \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- abort ...\n");                                   \
            fflush(GetTrcFP());                                         \
            abort();                                                    \
        }                                                               \
    } while (0)

#define ASRTM(e, m) do {                                                \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs(__FILE__); eputs(", Line: ");        \
            eputi(__LINE__); eputs("\n");                               \
            eputs("\t"); eputs((m)); eputs("\n");                       \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- abort ...\n");                                   \
            fflush(GetTrcFP());                                         \
            abort();                                                    \
        }                                                               \
    } while (0)

#define ASRTK(e, file, line) do {                                       \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs((file)); eputs(", Line: ");          \
            eputi((line)); eputs("\n");                                 \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- abort ...\n");                                   \
            fflush(GetTrcFP());                                         \
            abort();                                                    \
        }                                                               \
    } while (0)

#define ASRTMK(e, m, file, line) do {                                   \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs((file)); eputs(", Line: ");          \
            eputi((line)); eputs("\n");                                 \
            eputs("\t"); eputs((m)); eputs("\n");                       \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- abort ...\n");                                   \
            fflush(GetTrcFP())                                          \
                abort();                                                \
        }                                                               \
    } while (0)
/**********************************************************************/


/**********************************************************************/
/* colors respresented as parameters to WORDs and Screen positions */

#define CLRVAL  unsigned int

/* formal parameters to procedures are stored as a linked list of argument names
 * using the following simple structure:
 */

typedef struct PARG {
    struct PARG *next;          /* ptr to next linked argument */
    char name[NVSIZE+1];        /* name of the argument */
} PARG;

/* UNDO definitions and types */

typedef int OPTYPE;     /* type of operation being recorded/played back */

#define OP_CMND         1       /* beginning of command */
#define OP_DELC         2       /* delete a single character */
#define OP_INSC         3       /* insert a single character */
#define OP_DSTR         4       /* delete a string of characters */
#define OP_ISTR         5       /* insert a string of characters */
#define OP_REPC         6       /* replace a character */
#define OP_CPOS         7       /* set the cursor position */

/* object to be undone! */

typedef union OBJECT {
    char obj_char;              /* a character */
    char obj_string[1];         /* many characters */
    char *obj_sptr;             /* a ptr to a character */
} OBJECT;

typedef struct UNDO_OBJ {
    struct UNDO_OBJ *next;      /* ptr to next undo object */
    OPTYPE type;                /* type of operation */
    long line_num;              /* line offset from buffer beginning */
    int offset;                 /* offset into that line */
    long count;                 /* repetitions? */
    OBJECT undo_obj;            /* object to be undone */
} UNDO_OBJ;

/*
 * There is a window structure allocated for every active display window. The
 * windows are kept in a big list, in top to bottom screen order, with the
 * listhead at "wheadp". Each window contains its own values of dot and mark.
 * The flag field contains some bits that are set by commands to guide
 * redisplay. Although this is a bit of a compromise in terms of decoupling, the
 * full blown redisplay is just too expensive to run for every input character.
 */
typedef struct  EWINDOW {
    struct  EWINDOW *w_wndp;            /* Next window                  */
    struct  BUFFER  *w_bufp;            /* Buffer displayed in window   */
    struct  LINE    *w_linep;           /* Top line in the window       */
    struct  LINE    *w_dotp;            /* Line containing "."          */
    int             w_doto;             /* Byte offset for "."          */
    struct  LINE    *w_markp[NMARKS];   /* Line containing "mark"       */
    int             w_marko[NMARKS];    /* Byte offset for "mark"       */
    char            w_toprow;           /* Origin 0 top row of window   */
    char            w_ntrows;           /* # of rows of text in window  */
    char            w_force;            /* If NZ, forcing row.          */
    char            w_flag;             /* Flags.                       */
#if     COLOR
    char            w_fcolor;           /* current forground color      */
    char            w_bcolor;           /* current background color     */
#endif
    int             w_fcol;             /* first column displayed       */
}       EWINDOW;

#define WFFORCE 0x01                    /* Window needs forced reframe  */
#define WFMOVE  0x02                    /* Movement from line to line   */
#define WFEDIT  0x04                    /* Editing within a line        */
#define WFHARD  0x08                    /* Better to a full display     */
#define WFMODE  0x10                    /* Update mode line.            */
#define WFCOLR  0x20                    /* Needs a color change         */

/* This structure holds the information about each line appearing on the video
 * display. The redisplay module uses an array of virtual display lines. On
 * systems that do not have direct access to display memory, there is also an
 * array of physical display lines used to minimize video updating. In most
 * cases, these two arrays are unique. If WINDOW_MSWIN is 1, there is a pair of
 * such arrays in each SCREEN_T structure.
 */

typedef struct  VIDEO {
    int v_flag;                         /* Flags */
#if     COLOR
    int v_fcolor;                       /* current forground color */
    int v_bcolor;                       /* current background color */
    int v_rfcolor;                      /* requested forground color */
    int v_rbcolor;                      /* requested background color */
#endif
    int v_left;                         /* left edge of reverse video */
    int v_right;                        /* right right of reverse video */
#if     INSDEL && MEMMAP == 0
    int v_rline;                        /* requested screen line # */
#endif
    char v_text[1];                     /* Screen data. */
}       VIDEO;

#define VFNEW   0x0001                  /* contents not meaningful yet  */
#define VFCHG   0x0002                  /* Changed flag                 */
#define VFEXT   0x0004                  /* extended (beyond column 80)  */
#define VFCOL   0x0008                  /* color change requested       */

/*
 *      This structure holds the information about each separate "screen"
 * within the current editing session.  On a character based system, these
 * screens overlay each other, and can individually be brought to front. On a
 * windowing system like MicroSoft Windows 3.0, OS/2, the Macintosh, Intuition,
 * Sunview or X-windows, each screen is represented in an OS window.  The
 * terminolgy is wrong in emacs.....
 *
 *      EMACS           The outside World screen          window window
 *          pane
 */

typedef struct SCREEN_T {
    struct SCREEN_T *s_next_screen;     /* link to next screen in list */
    EWINDOW *s_first_window;            /* head of linked list of windows */
    EWINDOW *s_cur_window;              /* current window in this screen */
    char *s_screen_name;                /* name of the current window */
    short s_roworg;                     /* row origin of stored screen */
    short s_colorg;                     /* column origin of stored screen */
    short s_nrow;                       /* row width of stored screen */
    short s_ncol;                       /* column origin of stored screen */
#if WINDOW_MSWIN
    VIDEO **s_virtual;                  /* virtual screen contents */
    VIDEO **s_physical;                 /* physical screen contents */
    HWND s_drvhandle;                   /* handle for the "term" driver */
#endif
} SCREEN_T;

/*
 * Text is kept in buffers. A buffer header, described below, exists for every
 * buffer in the system. The buffers are kept in a big list, so that commands
 * that search for a buffer by name can find the buffer header. There is a safe
 * store for the dot and mark in the header, but this is only valid if the
 * buffer is not being displayed (that is, if "b_nwnd" is 0). The text for the
 * buffer is kept in a circularly linked list of lines, with a pointer to the
 * header line in "b_linep".  Buffers may be "Inactive" which means the files
 * associated with them have not been read in yet.  These get read in at "use
 * buffer" time. Some buffers are really procedures and have a little extra
 * information stored with them.
 */
typedef struct  BUFFER {
    struct  BUFFER  *b_bufp;            /* Link to next BUFFER          */
    struct  LINE    *b_dotp;            /* Link to "." LINE structure   */
    int              b_doto;            /* Offset of "." in above LINE  */
    struct  LINE    *b_markp[NMARKS];   /* The same as the above two,   */
    int              b_marko[NMARKS];   /* but for the "mark"           */
    int              b_fcol;            /* first col to display         */
    struct  LINE    *b_linep;           /* Link to the header LINE      */
    struct  LINE    *b_topline;         /* Link to narrowed top text    */
    struct  LINE    *b_botline;         /* Link to narrowed bottom text */
    char            b_active;           /* window activated flag        */
    char            b_nwnd;             /* Count of windows on buffer   */
    char            b_exec;             /* how many active executions   */
    char            b_flag;             /* Flags                        */
    int             b_mode;             /* editor mode of this buffer   */
    char            b_fname[NFILEN];    /* File name                    */
    char            b_bname[NBUFN];     /* Buffer name                  */
#if     CRYPT
    char            b_key[NPAT];        /* current encrypted key        */
#endif
    int             b_numargs;          /* number of arguments to procedure */
    PARG            *b_args;            /* ptr to the first argument    */
    UNDO_OBJ        *undo_head;         /* head of undo stack for buffer */
    long            undo_count;         /* # of undo operations stacked */
    long            last_access;        /* time of last access          */
}       BUFFER;

#define BFINVS  0x01                    /* Internal invisable buffer    */
#define BFCHG   0x02                    /* Changed since last write     */
#define BFTRUNC 0x04                    /* buffer was truncated when read */
#define BFNAROW 0x08                    /* buffer has been narrowed     */

#define NOTPROC -1                      /* buffer is not a procedure */

/*      mode flags      */
#define NUMMODES        11              /* # of defined modes           */

#define MDWRAP  0x0001                  /* word wrap                    */
#define MDCMOD  0x0002                  /* C indentation and fence match*/
#define MDSPELL 0x0004                  /* spell error parsing          */
#define MDEXACT 0x0008                  /* Exact matching for searches  */
#define MDVIEW  0x0010                  /* read-only buffer             */
#define MDOVER  0x0020                  /* overwrite mode               */
#define MDMAGIC 0x0040                  /* regular expresions in search */
#define MDCRYPT 0x0080                  /* encrytion mode active        */
#define MDASAVE 0x0100                  /* auto-save mode               */
#define MDREPL  0x0200                  /* replace mode                 */
#define MDABBR  0x0400                  /* abbreviation expansion mode  */

/*
 * The starting position of a region, and the size of the region in characters,
 * is kept in a region structure.  Used by the region commands.
 */
typedef struct  {
    struct  LINE *r_linep;              /* Origin LINE address.         */
    int     r_offset;                   /* Origin LINE offset.          */
    long    r_size;                     /* Length in characters.        */
}       REGION;

/*
 * All text is kept in circularly linked lists of "LINE" structures. These begin
 * at the header line (which is the blank line beyond the end of the buffer).
 * This line is pointed to by the "BUFFER". Each line contains a the number of
 * bytes in the line (the "used" size), the size of the text array, and the
 * text. /o The end of line is not stored as a byte; it's implied. o/
 * The routine lalloc() allocates one addtional character which si set to '\0'
 * by the room() routine.
 */
typedef struct  LINE {
    struct  LINE *l_fp;                 /* Link to the next line        */
    struct  LINE *l_bp;                 /* Link to the previous line    */
    int     l_size_;                    /* Allocated size               */
    int     l_used_;                    /* Used size                    */
    char    l_text_[1];                 /* A bunch of characters.       */
}       LINE;

#define lforw(lp)       ( (lp)->l_fp )
#define lback(lp)       ( (lp)->l_bp )
#if ( IS_UNIX() )
# define lgetc_ORG_(lp, n)    ( (unsigned char)(lp)->l_text_[(n)] )
#else
# define lgetc_ORG_(lp, n)    ( (lp)->l_text_[(n)] )
#endif

#define lputc_ORG_(lp, n, c) ( (lp)->l_text_[(n)]=(c) )

static char lputc_(LINE *lp, int n, char c, const char *fnam, int lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);
    ASRTK(0 <= n,                     fnam, lno);
    ASRTK(n < lp->l_size_,            fnam, lno);

    return ( lp->l_text_[n] = c );
}
#define lputc(lp, n, c) ( lputc_((lp), (n), (c), __FILE__, __LINE__) )

#if ( IS_UNIX() )
static unsigned char lgetc_(LINE *lp, int n, const char *fnam, int lno)
#else
    static char lgetc_(LINE *lp, int n, const char *fnam, int lno)
#endif
{
    ASRTK(NULL != lp,                     fnam, lno);
    ASRTK(lp->l_used_ <= lp->l_size_,     fnam, lno);
    ASRTK((0 <= n) && (n <= lp->l_used_), fnam, lno);

    if ( n == lp->l_used_ ) {
        TRCK(("Read at Buffer Boundry: l_size_ = %d, l_used_ = %d, l_text_[%d] = %c", 
              (int)lp->l_size_, n, n, lp->l_text_[n]), fnam, lno);

        if ( n == lp->l_size_ ) {
            return ( '\0' );
        } else {
            return ( lp->l_text_[n] );
        }
    }

    return ( lp->l_text_[n] );
}
#define lgetc(lp, n)    ( lgetc_((lp), (n), __FILE__, __LINE__) )

static char *lgetcp_(LINE *lp, int n, const char *fnam, int lno)
{
    ASRTK(NULL != lp,                     fnam, lno);
    ASRTK(lp->l_used_ <= lp->l_size_,     fnam, lno);
    ASRTK((0 <= n) && (n <= lp->l_used_), fnam, lno);

    if ( n == lp->l_used_ ) {
        TRCK(("Read at Buffer Boundry: l_size_ = %d, l_used_ = %d, l_text_[%d] = %c", 
              (int)lp->l_size_, n, n, lp->l_text_[n]), fnam, lno);
    }

    return ( &(lp->l_text_[n]) );
}
#define lgetcp(lp, n)   ( lgetcp_((lp), (n), __FILE__, __LINE__) )

#define lused_ORG(lp)       ( (lp)->l_used_ )
static int get_lused_(LINE *lp, const char *fnam, int lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);

    return ( (lp)->l_used_ );
}
#define get_lused(lp)   ( get_lused_((lp), __FILE__, __LINE__) )
static int set_lused_(LINE *lp, int used, const char *fnam, int lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);
    ASRTK(0 <= used,                  fnam, lno);
    ASRTK(used <= lp->l_size_,        fnam, lno);

    return ( (lp)->l_used_ = used );
}
#define set_lused(lp, used) ( set_lused_((lp), (used), __FILE__, __LINE__) )
#define lsize_ORG(lp)       ( (lp)->l_size_ )
static int get_lsize_(LINE *lp, const char *fnam, int lno)
{
    ASRTK(NULL != lp,                 fnam, lno);
    ASRTK(lp->l_used_ <= lp->l_size_, fnam, lno);

    return ( (lp)->l_size_ );
}
#define get_lsize(lp)   ( get_lsize_((lp), __FILE__, __LINE__) )
#define ltext_ORG(lp)       ( (lp)->l_text_ )
#define ltext(lp)       ( lgetcp(lp, 0) )

/*      This structure is used to hold a user variables name and its current
 * value. These are used for both the global and the local symbol tables.
 */

typedef struct UVAR {
    char u_name[NVSIZE + 1];                   /* name of user variable */
    char *u_value;                             /* value (string)        */
} UVAR;

#define VT_NONE         0       /* don't declare it if not found */
#define VT_LOCAL        1       /* local to the current procedure */
#define VT_GLOBAL       2       /* global to all procedures */

/*      A UTABLE is a user variable table.... containing some header information
 * and an array of user variable names and definitions. They are held together
 * in a linked list, the last member of the list being the global user variable
 * table.
 */

typedef struct UTABLE {
    struct UTABLE *next;        /* ptr to next user variable table */
    int size;                   /* max number of variables in table */
    BUFFER *bufp;               /* ptr to buffer holding procedure assosiated
                                 * with this symbol table. */
    UVAR uv[1];                 /* list of variable names/definitions in this
                                 * variable table */
} UTABLE;

/*
 * The editor communicates with the display using a high level interface. A
 * "TERM" structure holds useful variables, and indirect pointers to routines
 * that do useful operations. The low level get and put routines are here too.
 * This lets a terminal, in addition to having non standard commands, have funny
 * get and put character code too. The calls might get changed to
 * "termp->t_field" style in the future, to make it possible to run more than
 * one terminal type.
 */
#if     PROTO
typedef struct  {
    short t_mrow;                       /* max number of rows allowable */
    short t_nrow;                       /* current number of rows used  */
    short t_mcol;                       /* max Number of columns.       */
    short t_ncol;                       /* current Number of columns.   */
    short t_roworg;                     /* origin row (normally zero)   */
    short t_colorg;                     /* origin column (normally zero)*/
    short t_margin;                     /* min margin for extended lines*/
    short t_scrsiz;                     /* size of scroll region "      */
    int t_pause;                        /* # times thru update to pause */
    int (PASCAL NEAR *t_open)(void);        /* Open terminal at the start.*/
    int (PASCAL NEAR *t_close)(void);       /* Close terminal at end.   */
    int (PASCAL NEAR *t_kopen)(void);       /* Open keyboard            */
    int (PASCAL NEAR *t_kclose)(void);      /* Close keyboard           */
    int (PASCAL NEAR *t_getchar)(void);     /* Get character from keyboard. */
    int (PASCAL NEAR *t_putchar)(int);      /* Put character to display.*/
    int (PASCAL NEAR *t_flush)(void);       /* Flush output buffers.    */
    int (PASCAL NEAR *t_move)(int, int);    /* Move the cursor, origin 0.*/
    int (PASCAL NEAR *t_eeol)(void);        /* Erase to end of line.    */
    int (PASCAL NEAR *t_eeop)(void);        /* Erase to end of page.    */
    int (PASCAL NEAR *t_clrdesk)(void);     /* Clear the page totally   */
    int (PASCAL NEAR *t_beep)(void);        /* Beep.                    */
    int (PASCAL NEAR *t_rev)(int);          /* set reverse video state  */
    int (PASCAL NEAR *t_rez)(char *);       /* change screen resolution */
# if     COLOR
    int (PASCAL NEAR *t_setfor)(int);       /* set forground color      */
    int (PASCAL NEAR *t_setback)(int);      /* set background color     */
# endif
# if     INSDEL
    int (PASCAL NEAR *t_insline)(int);     /* insert a screen line      */
    int (PASCAL NEAR *t_delline)(int);     /* delete a screen line      */
# endif
# if     WINDOW_MSWIN
    int (PASCAL NEAR *t_sleep)(int);       /* go to sleep for a while   */
    int (PASCAL NEAR *t_newscr)(SCREEN_T *);      /* create new screen display
                                                   */
    int (PASCAL NEAR *t_delscr)(SCREEN_T *);      /* destroy screen display */
    int (PASCAL NEAR *t_selscr)(SCREEN_T *);      /* select screen display */
    int (PASCAL NEAR *t_sizscr)(SCREEN_T *);      /* resize screen display */
    int (PASCAL NEAR *t_topscr)(SCREEN_T *);      /* bring screen to top  */
# endif
}       TERM;
#else   /* TERM structure, no prototyping.*/

typedef struct  {
    short t_mrow;                       /* max number of rows allowable */
    short t_nrow;                       /* current number of rows used  */
    short t_mcol;                       /* max Number of columns.       */
    short t_ncol;                       /* current Number of columns.   */
    short t_roworg;                     /* origin row (normally zero)   */
    short t_colorg;                     /* origin column (normally zero)*/
    short t_margin;                     /* min margin for extended lines*/
    short t_scrsiz;                     /* size of scroll region "      */
    int t_pause;                        /* # times thru update to pause */
    int (PASCAL NEAR *t_open)();        /* Open terminal at the start.  */
    int (PASCAL NEAR *t_close)();       /* Close terminal at end.       */
    int (PASCAL NEAR *t_kopen)();       /* Open keyboard                */
    int (PASCAL NEAR *t_kclose)();      /* close keyboard               */
    int (PASCAL NEAR *t_getchar)();     /* Get character from keyboard. */
    int (PASCAL NEAR *t_putchar)();     /* Put character to display.    */
    int (PASCAL NEAR *t_flush)();       /* Flush output buffers.        */
    int (PASCAL NEAR *t_move)();        /* Move the cursor, origin 0.   */
    int (PASCAL NEAR *t_eeol)();        /* Erase to end of line.        */
    int (PASCAL NEAR *t_eeop)();        /* Erase to end of page.        */
    int (PASCAL NEAR *t_clrdesk)();     /* Clear the page totally       */
    int (PASCAL NEAR *t_beep)();        /* Beep.                        */
    int (PASCAL NEAR *t_rev)();         /* set reverse video state      */
    int (PASCAL NEAR *t_rez)();         /* change screen resolution     */
# if     COLOR
    int (PASCAL NEAR *t_setfor)();      /* set forground color          */
    int (PASCAL NEAR *t_setback)();     /* set background color         */
# endif
# if     INSDEL
    int (PASCAL NEAR *t_insline)();     /* insert a screen line         */
    int (PASCAL NEAR *t_delline)();     /* delete a screen line         */
# endif
# if     WINDOW_MSWIN
    int (PASCAL NEAR *t_sleep)();       /* go to sleep for a while      */
    int (PASCAL NEAR *t_newscr)();      /* create new screen display    */
    int (PASCAL NEAR *t_delscr)();      /* destroy screen display       */
    int (PASCAL NEAR *t_selscr)();      /* select screen display        */
    int (PASCAL NEAR *t_sizscr)();      /* resize screen display        */
    int (PASCAL NEAR *t_topscr)();      /* bring screen to top          */
# endif
}       TERM;
#endif


/*      TEMPORARY macros for terminal I/O  (to be placed in a machine dependant
 * place later) */

#define TTopen          (*term.t_open)
#define TTclose         (*term.t_close)
#define TTkopen         (*term.t_kopen)
#define TTkclose        (*term.t_kclose)
#define TTgetc          (*term.t_getchar)
#define TTputc          (*term.t_putchar)
#define TTflush         (*term.t_flush)
#define TTmove          (*term.t_move)
#define TTeeol          (*term.t_eeol)
#define TTeeop          (*term.t_eeop)
#define TTclrdesk       (*term.t_clrdesk)
#define TTbeep          (*term.t_beep)
#define TTrev           (*term.t_rev)
#define TTrez           (*term.t_rez)
#if     COLOR
# define TTforg          (*term.t_setfor)
# define TTbacg          (*term.t_setback)
#endif

/*      Structure for the table of current key bindings         */

ETYPE EPOINTER {
    int (PASCAL NEAR *fp)();            /* C routine to invoke */
    BUFFER *buf;                        /* buffer to execute */
};

typedef struct  {
    short k_code;               /* Key code                     */
    short k_type;               /* binding type (C function or EMACS buffer) */
    ETYPE EPOINTER k_ptr;       /* ptr to thing to execute */
}       KEYTAB;

/*      structure for the name binding table            */

typedef struct {
    char *n_name;                       /* name of function key */
    int (PASCAL NEAR *n_func)();        /* function name is bound to */
}       NBIND;

/*      The editor holds deleted text chunks in the KILL buffer. The kill buffer
 * is logically a stream of ascii characters, however due to its unpredicatable
 * size, it gets implemented as a linked list of chunks. (The d_ prefix is for
 * "deleted" text, as k_ was taken up by the keycode structure)
 */

typedef struct KILL {
    struct KILL *d_next;       /* link to next chunk, NULL if last */
    char d_chunk[KBLOCK];       /* deleted text */
} KILL;

/*      When emacs's command interpetor needs to get a variable's name, rather
 * than it's value, it is passed back as a VDESC variable description structure.
 * The v_num field is a index into the appropriate variable table.
 */

typedef struct VDESC {
    int v_type;         /* type of variable */
    int v_num;          /* ordinal pointer to variable in list */
    UTABLE *v_ut;       /* ptr to appropriate user table if user var */
} VDESC;

/*      The !WHILE directive in the execution language needs to stack references
 * to pending whiles. These are stored linked to each currently open procedure
 * via a linked list of the following structure
 */

typedef struct WHBLOCK {
    LINE *w_begin;              /* ptr to !while statement */
    LINE *w_end;                /* ptr to the !endwhile statement*/
    int w_type;                 /* block type */
    struct WHBLOCK *w_next;     /* next while */
} WHBLOCK;

#define BTWHILE         1
#define BTBREAK         2

/*      SWORDs are syntactical words to highlight in a different foreground
 * color. WORDSETs are named lists of these WORDs.
 */

typedef struct SWORD {
    struct SWORD *wd_next;      /* ptr to the next word in the list */
    char *wd_text;              /* the actual word */
    CLRVAL wd_color;            /* color to display in */
    int wd_symflag;             /* TRUE  = symbol overiding word boundries FALSE
                                 * = word w/ space/symbol delimiters */
} SWORD;

typedef struct WORDSET {
    struct WORDSET *next;
    int tmp;
} WORDSET;

/*      Abbreviations are short symbols that expand to longer strings when typed
 * into a buffer with no intervening whitespace or commands. This structure
 * grows dynamically as needed.
 */

typedef struct ABBREV {
    struct ABBREV *ab_next;             /* pointer to the next abbreviation */
    char ab_sym[MAXSYM + 1];            /* name to expand */
    char ab_exp[1];                     /* string to expand to */
} ABBREV;

/* Search definitions... */

/* HICHAR - 1 is the largest character we will deal with. BMAPSIZE represents
 * the number of bytes in the bitmap.
 */
#define HICHAR          256
#define BMAPSIZE        HICHAR >> 3

/*
 * Jump table structures.
 */
typedef struct {
    int jump;
    int patlen;
    int delta[HICHAR];
    char patrn[NPAT];
} DELTA;

#if     MAGIC
/*
 * Defines for the metacharacters in the regular expression search routines.
 *  MCNIL and GROUP are used in both search and replace metachar-arrays.
 */
# define MCNIL           0      /* Like the '\0' for strings.*/
# define JMPTABLE        1
# define LITSTRING       2      /* Literal string.*/
# define LITCHAR         3      /* Literal character.*/
# define ANY             4      /* Any character but the <NL>.*/
# define CCL             5
# define NCCL            6
# define BOL             7
# define EOL             8
# define BOWRD           9
# define EOWRD           10
# define GRPBEG          11     /* Signal start of group.*/
# define GRPEND          12     /* Signal end of group.*/
# define GROUP           13     /* String of group match.*/
# define DITTO           14     /* Replacement with match string.*/

# define CLOSURE         0x0100 /* An or-able value for a closure modifier.*/
# define CLOSURE_1       0x0200 /* An or-able value for a closure modifier.*/
# define ZEROONE         0x0400 /* An or-able value for a closure modifier.*/

# define ALLCLOS         (CLOSURE | CLOSURE_1 | ZEROONE)
# define MASKCLO         (~ALLCLOS)

# define MC_ANY          '.'    /* 'Any' character (except newline).*/
# define MC_CCL          '['    /* Character class.*/
# define MC_NCCL         '^'    /* Negate character class.*/
# define MC_RCCL         '-'    /* Range in character class.*/
# define MC_ECCL         ']'    /* End of character class.*/
# define MC_BOL          '^'    /* Beginning of line.*/
# define MC_EOL          '$'    /* End of line.*/
# define MC_CLOSURE      '*'    /* Closure - zero to many characters match.*/
# define MC_CLOSURE_1    '+'    /* Closure - one to many characters match.*/
# define MC_ZEROONE      '?'    /* Closure - zero to one characters match.*/
# define MC_DITTO        '&'    /* Use matched string in replacement.*/
# define MC_GRPBEG       '('    /* Start of group (begun with a backslash).*/
# define MC_GRPEND       ')'    /* End of group (begun with a backslash).*/
# define MC_BOWRD        '<'    /* Beginning of word (begun with a backslash).*/
# define MC_EOWRD        '>'    /* End of word (begun with a backslash).*/
# define MC_ESC          '\\'   /* Escape - suppress meta-meaning.*/

# define MAXGROUPS       10             /* 1 + maximum # of r. e. groups. */
# define BIT(n)          ( 1 << (n) )   /* An integer with one bit set.*/

/* Typedefs that define the bitmap type for searching (EBITMAP), the
 * meta-character structure for MAGIC mode searching (MC), and the
 * meta-character structure for MAGIC mode replacment (RMC).
 */
typedef char    *EBITMAP;

typedef struct {
    short int mc_type;
    union {
        int lchar;
        int group_no;
        char    *lstring;
        DELTA   *jmptable;
        EBITMAP cclmap;
    } u;
} MC;

typedef struct {
    short int mc_type;
    union {
        int group_no;
        char    *rstr;
    } u;
} RMC;
#endif


/**********************************************************************/
#if     PROTO

/***    global function prototypes      ***/

# if     WINDOW_MSWIN
extern char * PASCAL fullpathname (char *PathName, int Nbuf);
extern int PASCAL NEAR vtinitscr (SCREEN_T *sp, int nrow, int ncol);
extern int PASCAL NEAR vtsizescr (SCREEN_T *sp, int nrow, int ncol);
extern int PASCAL cutregion (int f, int n);
extern int PASCAL clipregion (int f, int n);
extern int PASCAL insertclip (int f, int n);
extern int PASCAL helpengine (int f, int n);
extern int PASCAL minimizescreen (int f, int n);
extern int PASCAL maximizescreen (int f, int n);
extern int PASCAL restorescreen (int f, int n);
extern int PASCAL tilescreens (int f, int n);
extern int PASCAL cascadescreens (int f, int n);
extern int PASCAL bindtomenu (int f, int n);
extern int PASCAL macrotomenu (int f, int n);
extern int PASCAL unbindmenu (int f, int n);
extern int PASCAL execmenu (int f, int n);
extern int PASCAL longop (int f);
extern int PASCAL filenamedlg (char *prompt, char *buf, int nbuf, int fullpath);
extern int PASCAL NEAR vtfreescr (SCREEN_T *sp);
extern int PASCAL NEAR unlist_screen(SCREEN_T *sp);
extern int PASCAL mlhistory(VOID);
extern int PASCAL updscrollbars (SCREEN_T *sp, char w_flag);
extern VOID PASCAL NEAR vtscreen (SCREEN_T *sp);
# endif

# if CALLED
extern int emacs(int argc, char *argv[]);
# endif

# if HANDLE_WINCH
extern VOID winch_changed(int);
extern VOID winch_new_size(VOID);
# endif

# if DEBUG_SEARCH
int PASCAL NEAR mc_list(int f, int n);
int PASCAL NEAR rmc_list(int f, int n);
VOID PASCAL NEAR mctype_cat(char pline[], int mc_type);
# endif

extern VOID undo_insert(OPTYPE op_type, long count, OBJECT op_erand);
extern int undo_op(VOID);
extern VOID undo_dump(VOID);
extern VOID undo_zot(BUFFER *bp);
extern int PASCAL NEAR undo(int f, int n);
extern int PASCAL NEAR undo_delete(int f, int n);
extern int PASCAL NEAR undo_list(int f, int n);
extern VOID *room(int);
extern VOID *reroom(VOID *, int);

extern int PASCAL NEAR ab_insert(char *sym, char *expansion);
extern char *PASCAL NEAR ab_lookup(char *sym);
extern int PASCAL NEAR ab_delete(char *sym);
extern int PASCAL NEAR ab_clean(VOID);
extern BUFFER *PASCAL NEAR bfind(char *bname, int cflag, int bflag);
extern BUFFER *PASCAL NEAR getcbuf(char *prompt, char *defval, int createflag);
extern BUFFER *PASCAL NEAR getdefb(VOID);
extern BUFFER *PASCAL NEAR getoldb(VOID);
extern SCREEN_T *PASCAL NEAR init_screen(char *, BUFFER *);
extern SCREEN_T *PASCAL NEAR lookup_screen(char *scr_name);
extern SCREEN_T *PASCAL NEAR index_screen(int scr_num);
extern int PASCAL NEAR screen_index(SCREEN_T *sp);
extern int PASCAL NEAR insert_screen(SCREEN_T *sp);
extern int PASCAL NEAR select_screen(SCREEN_T *sp, int announce);
extern int PASCAL NEAR free_screen(SCREEN_T *sp);
extern char *Eallocate(unsigned nbytes);
extern char *dolock(CONST char *fname);
extern char *getpath(char *filespec);
extern char *gtname(char *filespec);
extern char *PASCAL NEAR bytecopy(char *dst, CONST char *src, int maxlen);
extern char *PASCAL NEAR cmdstr(int c, char *seq);
extern char *PASCAL NEAR copystr(char *);
extern char *PASCAL NEAR complete(char *prompt,
                                  char *defval,
                                  int  type,
                                  int  maxlen);
extern char *PASCAL NEAR envval(int i);
extern CONST char *PASCAL NEAR fixnull(CONST char *s);
extern CONST char *PASCAL NEAR flook(CONST char *fname, int hflag);
extern char *PASCAL NEAR funval(int i);
extern char *PASCAL NEAR getctext(char *rline);
extern char *PASCAL NEAR getffile(char *fspec);
extern char *PASCAL NEAR getfname(KEYTAB *key);
extern char *PASCAL NEAR getkill(VOID);
extern char *PASCAL NEAR getnfile(VOID);
extern CONST char *PASCAL NEAR getreg(char *value);
extern CONST char *PASCAL NEAR getval(char *token);
extern char *PASCAL NEAR getwlist(char *buf);
extern CONST char *PASCAL NEAR gtenv(CONST char *vname);
extern char *PASCAL NEAR gtfilename(char *prompt);
extern CONST char *PASCAL NEAR gtfun(CONST char *fname);
extern CONST char *PASCAL NEAR gtusr(CONST char *vname);
extern char *PASCAL NEAR int_asc(int i);
extern char *PASCAL NEAR long_asc(long num);
extern CONST char *PASCAL NEAR ltos(int val);
extern CONST char *PASCAL NEAR makename(char *bname, CONST char *fname);
extern char *PASCAL NEAR mklower(char *str);
extern char *PASCAL NEAR mkupper(char *str);
extern char *PASCAL NEAR namval(int index);
extern char *PASCAL NEAR timeset(VOID);
extern char *PASCAL NEAR token(char *src, char *tok, int size);
extern CONST char *PASCAL NEAR transbind(CONST char *skey);
extern char *PASCAL NEAR trimstr(char *s);
extern char *PASCAL NEAR xlat(char *source, char *lookup, char *trans);
extern char *undolock(CONST char *fname);
extern char *PASCAL NEAR regtostr(char *buf, REGION *region);
extern int PASCAL NEAR lowerc(char ch);
extern int PASCAL NEAR cycle_ring(int f, int n);
extern int PASCAL NEAR upperc(char ch);
# if     ZTC || TURBO || IC
extern int ( PASCAL NEAR *PASCAL NEAR fncmatch(char *fname) )(int, int);
extern int ( PASCAL NEAR *PASCAL NEAR getname(char *prompt) )(int, int);
# else  /* Sun (and others?) screwed up the prototyping.*/
extern int ( PASCAL NEAR *PASCAL NEAR fncmatch(char *fname) )(VOID);
extern int ( PASCAL NEAR *PASCAL NEAR getname(char *prompt) )(VOID);
# endif
extern int PASCAL NEAR asc_int(char *st);
extern int dolhello(VOID);
extern int dspram(VOID);
extern int lckerror(char *errstr);
extern int lckhello(VOID);
extern int xlock(CONST char *fname);
extern int lockchk(CONST char *fname);
extern int lockrel(VOID);
extern int mousehello(VOID);
extern int nocrypt(VOID);
extern int PASCAL NEAR absv(int x);
extern int PASCAL NEAR add_abbrev(int f, int n);
extern int PASCAL NEAR del_abbrev(int f, int n);
extern int PASCAL NEAR kill_abbrevs(int f, int n);
extern int PASCAL NEAR ins_abbrevs(int f, int n);
extern int PASCAL NEAR def_abbrevs(int f, int n);
extern int PASCAL NEAR addline(BUFFER *bp, char *text);
extern int PASCAL NEAR amatch(MC *mcptr, int direct, LINE **pcwline,
                              int *pcwoff);
extern int PASCAL NEAR backhunt(int f, int n);
extern int PASCAL NEAR backsearch(int f, int n);
extern int PASCAL NEAR biteq(int bc, EBITMAP cclmap);
extern int PASCAL NEAR bktoshell(int f, int n);
extern int PASCAL NEAR boundry(LINE *curline, int curoff, int dir);
extern int PASCAL NEAR cclmake(char **ppatptr, MC *mcptr);
extern int PASCAL NEAR checknext(int chr, int dir);
extern int PASCAL NEAR clear_ring(int f, int n);
extern int PASCAL NEAR delins(int dlength, char *instr, int use_rmc);
extern int PASCAL NEAR desfunc(int f, int n);
extern int PASCAL NEAR dispvar(int f, int n);
extern int PASCAL NEAR echochar(unsigned char c);
extern int PASCAL NEAR echostring(char *, int, int);
extern int PASCAL NEAR eq(register unsigned char bc, register unsigned char pc);
extern long PASCAL NEAR ernd(VOID);
extern int PASCAL NEAR execkey(KEYTAB *key, int f, int n);
extern int PASCAL NEAR fbound(DELTA *tbl,
                              int   jump,
                              LINE  **pcurline,
                              int   *pcuroff,
                              int   dir);
extern int PASCAL NEAR fexist(char *fname);
extern int PASCAL NEAR findcol(LINE *lp, int pos);
extern int PASCAL NEAR fisearch(int f, int n);
# if     FLABEL
extern int PASCAL NEAR fnclabel(int f, int n);
# endif
extern int PASCAL NEAR forwhunt(int f, int n);
extern int PASCAL NEAR forwsearch(int f, int n);
extern int PASCAL NEAR getcwnum(VOID);
extern int PASCAL NEAR getgoal(LINE *dlp);
extern int PASCAL NEAR getstring(unsigned char *buf, int nbuf, int eolchar);
extern int PASCAL NEAR gettwnum(VOID);
extern int PASCAL NEAR gettyp(char *token);
extern int PASCAL NEAR getkey(VOID);
extern int PASCAL NEAR getwpos(VOID);
extern int PASCAL NEAR get_char(VOID);
extern int PASCAL NEAR global_var(int f, int n);
# if     DBCS
extern int PASCAL NEAR is2byte(char *sp, char *cp);
# endif
extern int PASCAL NEAR is_letter(char ch);
extern int PASCAL NEAR is_lower(char ch);
extern int PASCAL NEAR is_num(char *st);
extern int PASCAL NEAR isearch(int dir);
extern int PASCAL NEAR is_upper(char ch);
extern int PASCAL NEAR kinsert(int back, char c);
extern int PASCAL NEAR ldelnewline(VOID);
extern int PASCAL NEAR linstr(CONST char *instr);
extern int PASCAL NEAR liteq(LINE **curline,
                             int  *curpos,
                             int  direct,
                             char *lstring);
extern int PASCAL NEAR litmake(char **ppatptr, MC *mcptr);
extern int PASCAL NEAR lnewline(VOID);
extern int PASCAL NEAR local_var(int f, int n);
extern int PASCAL NEAR lookup_color(char *sp);
extern int PASCAL NEAR lover(char *ostr);
extern int PASCAL NEAR mceq(unsigned char bc, MC *mt);
extern int PASCAL NEAR mcscanner(MC  *mcpatrn,
                                 int direct,
                                 int beg_or_end,
                                 int repeats);
extern int PASCAL NEAR mcstr(VOID);
extern int PASCAL NEAR mlprompt(char *, char *, int);
extern int PASCAL NEAR movelocalpoint(int n, int *pcuroff, LINE **pcurline);
extern int PASCAL NEAR nextch(LINE **pcurline, int *pcuroff, int dir);
extern int PASCAL NEAR pop(BUFFER *popbuffer);
extern int PASCAL NEAR qreplace(int f, int n);
extern int PASCAL NEAR readpattern(char *prompt, char apat[], int srch);
# if     WINDOW_TEXT
extern VOID PASCAL NEAR refresh_screen(SCREEN_T *sp);
# endif
extern int PASCAL NEAR reglines(VOID);
extern int PASCAL NEAR rename_screen(int f, int n);
extern int PASCAL NEAR replaces(int kind, int f, int n);
extern int PASCAL NEAR risearch(int f, int n);
extern int PASCAL NEAR rmcstr(VOID);
extern int PASCAL NEAR savematch(VOID);
extern int PASCAL NEAR scanmore(int dir);
# if MAGIC == 0
extern int PASCAL NEAR scanner(int direct, int beg_or_end, int repeats);
# endif
extern int PASCAL NEAR setlower(char *ch, char *val);
extern int PASCAL NEAR setlower(char *ch, char *val);
extern int PASCAL NEAR setupper(char *ch, char *val);
extern int PASCAL NEAR setupper(char *ch, char *val);
extern int PASCAL NEAR setvar(int f, int n);
extern int PASCAL NEAR sindex(char *source, char *pattern);
extern int PASCAL NEAR sreplace(int f, int n);
extern int PASCAL NEAR stol(char *val);
# if     DBCS
extern int PASCAL NEAR stopback(VOID);
extern int PASCAL NEAR stopforw(VOID);
# endif
extern int PASCAL NEAR svar(VDESC *var, char *value);
extern int PASCAL NEAR tgetc(VOID);
extern int PASCAL NEAR uneat(VOID);
extern int PASCAL NEAR unlist_screen(SCREEN_T *sp);
extern int PASCAL NEAR upscreen(int f, int n);
extern int PASCAL NEAR vtinit(VOID);
extern int PASCAL NEAR yank(int f, int n);
extern int PASCAL NEAR yank_pop(int f, int n);
extern int Erelease(char *mp);
extern int set_key(KEYTAB *key, char *name);
extern int xunlock(char *fname);
extern KEYTAB *getbind(int c);
extern LINE *PASCAL NEAR lalloc(int used);
extern LINE *PASCAL NEAR mouseline(EWINDOW *wp, int row);
extern long PASCAL NEAR getlinenum(BUFFER *bp, LINE *sline);
extern int PASCAL NEAR addkey(unsigned char * seq, int fn);
extern int PASCAL NEAR addkeymap(int f, int n);
extern int PASCAL NEAR adjustmode(int kind, int global);
extern int PASCAL NEAR anycb(VOID);
extern int PASCAL NEAR apro(int f, int n);
extern int PASCAL NEAR backchar(int f, int n);
extern int PASCAL NEAR backdel(int f, int n);
extern int PASCAL NEAR backline(int f, int n);
extern int PASCAL NEAR backpage(register int f, register int n);
extern int PASCAL NEAR backword(int f, int n);
extern int PASCAL NEAR bclear(BUFFER *bp);
extern int PASCAL NEAR binary(CONST char *key,
                              char *(PASCAL NEAR *tval)(),
                              int        tlength,
                              int        klength);
extern int PASCAL NEAR bindtokey(int f, int n);
extern int PASCAL NEAR buildlist(int type, char *mstring);
extern int PASCAL NEAR capword(int f, int n);
extern int PASCAL NEAR cex(int f, int n);
extern int PASCAL NEAR cinsert(VOID);
extern int PASCAL NEAR clean(VOID);
extern int PASCAL NEAR clrmes(int f, int n);
extern int PASCAL NEAR copyregion(int f, int n);
extern int PASCAL NEAR ctlxe(int f, int n);
extern int PASCAL NEAR ctlxlp(int f, int n);
extern int PASCAL NEAR ctlxrp(int f, int n);
extern int PASCAL NEAR ctoec(int c);
extern int PASCAL NEAR ctrlg(int f, int n);
extern int PASCAL NEAR cycle_screens(int f, int n);
extern VOID PASCAL NEAR dcline(int argc, char *argv[], int firstflag);
extern int PASCAL NEAR deblank(int f, int n);
extern int PASCAL NEAR debug(BUFFER *bp, char *eline, int *skipflag);
extern int PASCAL NEAR delbword(int f, int n);
extern int PASCAL NEAR delete_screen(int f, int n);
extern int PASCAL NEAR delfword(int f, int n);
extern int PASCAL NEAR delgmode(int f, int n);
extern int PASCAL NEAR delmode(int f, int n);
extern int PASCAL NEAR delwind(int f, int n);
extern int PASCAL NEAR desc_abbrevs(int f, int n);
extern int PASCAL NEAR desbind(int f, int n);
extern int PASCAL NEAR deskey(int f, int n);
extern int PASCAL NEAR desvars(int f, int n);
extern int PASCAL NEAR detab(int f, int n);
extern int PASCAL NEAR dobuf(BUFFER *bp);
extern int PASCAL NEAR docmd(char *cline);
extern int PASCAL NEAR dofile(CONST char *fname);
extern int PASCAL NEAR ectoc(int c);
extern VOID PASCAL NEAR edinit(char bname[]);
extern int PASCAL NEAR editloop(VOID);
extern int PASCAL NEAR endword(int f, int n);
extern int PASCAL NEAR enlargewind(int f, int n);
extern int PASCAL NEAR entab(int f, int n);
extern int PASCAL NEAR execbuf(int f, int n);
extern int PASCAL NEAR execcmd(int f, int n);
extern int PASCAL NEAR execfile(int f, int n);
extern int PASCAL NEAR execprg(int f, int n);
extern int PASCAL NEAR execproc(int f, int n);
extern int PASCAL NEAR execute(int c, int f, int n);
extern int PASCAL NEAR ffclose(VOID);
extern int PASCAL NEAR ffgetline(int *nbytes);
extern int PASCAL NEAR ffputline(char buf[], int nbuf);
extern int PASCAL NEAR ffropen(CONST char *fn);
extern int PASCAL NEAR ffwopen(char *fn, char *mode);
extern int PASCAL NEAR fileapp(int f, int n);
extern int PASCAL NEAR find_screen(int f, int n);
extern int PASCAL NEAR filefind(int f, int n);
extern int PASCAL NEAR filename(int f, int n);
extern int PASCAL NEAR fileread(int f, int n);
extern int PASCAL NEAR filesave(int f, int n);
extern int PASCAL NEAR filewrite(int f, int n);
extern int PASCAL NEAR fillpara(int f, int n);
extern int PASCAL NEAR f_filter(int f, int n);
extern VOID PASCAL NEAR findvar(char *var, VDESC *vd, int size, int scope);
extern int PASCAL NEAR fmatch(char ch);
extern int PASCAL NEAR forwchar(int f, int n);
extern int PASCAL NEAR forwdel(int f, int n);
extern int PASCAL NEAR forwline(int f, int n);
extern int PASCAL NEAR forwpage(int f, int n);
extern int PASCAL NEAR forwword(int f, int n);
extern int PASCAL NEAR getccol(int bflg);
extern int PASCAL NEAR getcmd(VOID);
extern int PASCAL NEAR getfence(int f, int n);
extern int PASCAL NEAR getfile(CONST char fname[], int lockfl);
extern int PASCAL NEAR get_key(VOID);
extern int PASCAL NEAR getregion(REGION *rp);
extern int PASCAL NEAR gotobob(int f, int n);
extern int PASCAL NEAR gotobol(int f, int n);
extern int PASCAL NEAR gotobop(int f, int n);
extern int PASCAL NEAR gotoeob(int f, int n);
extern int PASCAL NEAR gotoeol(int f, int n);
extern int PASCAL NEAR gotoeop(int f, int n);
extern int PASCAL NEAR gotoline(int f, int n);
extern int PASCAL NEAR gotomark(int f, int n);
extern int PASCAL NEAR help(int f, int n);
extern int PASCAL NEAR ifile(char fname[]);
extern int PASCAL NEAR indent(int f, int n);
extern int PASCAL NEAR indent_region(int f, int n);
extern int PASCAL NEAR insbrace(int n, int c);
extern int PASCAL NEAR insfile(int f, int n);
extern int PASCAL NEAR inspound(VOID);
extern int PASCAL NEAR insspace(int f, int n);
extern int PASCAL NEAR inword(VOID);
extern int PASCAL NEAR isinword(char c);
extern int PASCAL NEAR ismodeline(EWINDOW *wp, int row);
extern int PASCAL NEAR istring(int f, int n);
extern int PASCAL NEAR killbuffer(int f, int n);
extern int PASCAL NEAR killpara(int f, int n);
extern int PASCAL NEAR killregion(int f, int n);
extern int PASCAL NEAR killtext(int f, int n);
extern int PASCAL NEAR lchange(register int flag);
extern int PASCAL NEAR ldelete(long n, int kflag);
extern int PASCAL NEAR lfree(LINE *lp);
extern int PASCAL NEAR linsert(int n, char c);
extern int PASCAL NEAR listbuffers(int f, int n);
extern int PASCAL NEAR list_screens(int f, int n);
extern int PASCAL NEAR lowerregion(int f, int n);
extern int PASCAL NEAR lowerword(int f, int n);
extern int PASCAL NEAR lowrite(char c);
extern int PASCAL NEAR macarg(char *tok);
extern int PASCAL NEAR macrotokey(int f, int n);
extern int PASCAL NEAR makelist(int iflag);
extern int PASCAL NEAR movelocalpoint();
extern int PASCAL NEAR undolist();
extern int PASCAL NEAR mouse_screen(VOID);
extern int PASCAL NEAR screenlist(int iflag);
extern int PASCAL NEAR meexit(int status);
extern int PASCAL NEAR f_meta(int f, int n);
extern int PASCAL NEAR mlreply(char *prompt, char *buf, int nbuf);
extern int PASCAL NEAR mlyesno(char *prompt);
extern int PASCAL NEAR mouseoffset(EWINDOW *wp, LINE *lp, int col);
extern int PASCAL NEAR movemd(int f, int n);
extern int PASCAL NEAR movemu(int f, int n);
extern int PASCAL NEAR mregdown(int f, int n);
extern int PASCAL NEAR mmove(int f, int n);
extern int PASCAL NEAR mregup(int f, int n);
extern int PASCAL NEAR mvdnwind(int f, int n);
extern int PASCAL NEAR mvupwind(int f, int n);
extern int PASCAL NEAR namebuffer(int f, int n);
extern int PASCAL NEAR namedcmd(int f, int n);
extern int PASCAL NEAR narrow(int f, int n);
extern int PASCAL NEAR newline(int f, int n);
extern int PASCAL NEAR new_col_org(int f, int n);
extern int PASCAL NEAR new_row_org(int f, int n);
extern int PASCAL NEAR newsize(int f, int n);
extern int PASCAL NEAR newwidth(int f, int n);
extern int PASCAL NEAR nextarg(char *prompt,
                               char *buffer,
                               int  size,
                               int  terminator);
extern int PASCAL NEAR nextbuffer(int f, int n);
extern int PASCAL NEAR nextdown(int f, int n);
extern int PASCAL NEAR nextup(int f, int n);
extern int PASCAL NEAR nextwind(int f, int n);
extern int PASCAL NEAR nullproc(int f, int n);
extern int PASCAL NEAR onlywind(int f, int n);
extern int PASCAL NEAR openline(int f, int n);
extern int PASCAL NEAR ostring(char *s);
extern int PASCAL NEAR outstring(char *s);
extern int PASCAL NEAR ovstring(int f, int n);
extern int PASCAL NEAR pipecmd(int f, int n);
extern int PASCAL NEAR popbuffer(int f, int n);
extern int PASCAL NEAR prevwind(int f, int n);
extern int PASCAL NEAR putctext(char *iline);
extern int PASCAL NEAR putline(int row, int col, char buf[]);
extern int PASCAL NEAR quickexit(int f, int n);
extern int PASCAL NEAR quit(int f, int n);
extern int PASCAL NEAR quote(int f, int n);
extern int PASCAL NEAR rdonly(VOID);
extern int PASCAL NEAR readin(const char fname[], int lockfl);
extern int PASCAL NEAR refresh(int f, int n);
extern int PASCAL NEAR remmark(int f, int n);
extern int PASCAL NEAR reposition(int f, int n);
extern int PASCAL NEAR resetkey(VOID);
extern int PASCAL NEAR resize(int f, int n);
extern int PASCAL NEAR resizm(int f, int n);
extern int PASCAL NEAR resizm2(int f, int n);
extern int PASCAL NEAR resterr(VOID);
extern int PASCAL NEAR restwnd(int f, int n);
extern int PASCAL NEAR savewnd(int f, int n);
extern int PASCAL NEAR scwrite(int  row,
                               char *outstr,
                               int  forg,
                               int  bacg,
                               int  revleft,
                               int  revright);
extern int PASCAL NEAR setccol(int pos);
extern int PASCAL NEAR setekey(int f, int n);
extern int PASCAL NEAR setfillcol(int f, int n);
extern int PASCAL NEAR setgmode(int f, int n);
extern int PASCAL NEAR setmark(int f, int n);
extern int PASCAL NEAR setmod(int f, int n);
extern int PASCAL NEAR setwlist(char *wclist);
extern int PASCAL NEAR shellprog(char *cmd);
extern int PASCAL NEAR showcpos(int f, int n);
extern int PASCAL NEAR showfiles(int f, int n);
extern int PASCAL NEAR listkeymaps(int f, int n);
extern int PASCAL NEAR shrinkwind(int f, int n);
extern int PASCAL NEAR spal(char *pstr);
extern int PASCAL NEAR spawn(int f, int n);
extern int PASCAL NEAR spawncli(int f, int n);
extern int PASCAL NEAR splitwind(int f, int n);
extern int PASCAL NEAR startup(char *sfname);
extern int PASCAL NEAR storeproc(int f, int n);
extern int PASCAL NEAR strinc(char *source, char *sub);
extern int PASCAL NEAR swapmark(int f, int n);
extern int PASCAL NEAR swbuffer(BUFFER *bp);
extern int PASCAL NEAR tab(int f, int n);
extern int PASCAL NEAR trim(int f, int n);
extern int PASCAL NEAR ttclose(VOID);
extern int PASCAL NEAR ttflush(VOID);
extern int PASCAL NEAR ttgetc(VOID);
extern int PASCAL NEAR ttopen(VOID);
extern int PASCAL NEAR ttputc(int c);
extern int PASCAL NEAR twiddle(int f, int n);
extern int PASCAL NEAR typahead(VOID);
extern int PASCAL NEAR unarg(VOID);
extern int PASCAL NEAR unbindchar(int c);
extern int PASCAL NEAR unbindkey(int f, int n);
extern int PASCAL NEAR undent_region(int f, int n);
extern int PASCAL NEAR unmark(int f, int n);
extern int PASCAL NEAR upperregion(int f, int n);
extern int PASCAL NEAR upperword(int f, int n);
extern int PASCAL NEAR usebuffer(int f, int n);
extern int PASCAL NEAR viewfile(int f, int n);
extern int PASCAL NEAR vteeol(VOID);
extern int PASCAL NEAR vtmove(int row, int col);
extern int PASCAL NEAR vtputc(int c);
extern int PASCAL NEAR vttidy(VOID);
extern int PASCAL NEAR widen(int f, int n);
extern int PASCAL NEAR wordcount(int f, int n);
extern int PASCAL NEAR wrapword(int f, int n);
extern int PASCAL NEAR writemsg(int f, int n);
extern int PASCAL NEAR writeout(char *fn, char *mode);
extern int PASCAL NEAR zotbuf(BUFFER *bp);
extern unsigned int PASCAL NEAR chcase(register unsigned int ch);
extern unsigned int PASCAL NEAR getckey(int mflag);
extern unsigned int PASCAL NEAR stock(CONST char *keyname);
# if     VARARG && VARG
#  if     GCC
extern VOID CDECL NEAR mlwrite(va_alist);
#  else
extern VOID CDECL NEAR mlwrite(va_dcl);
#  endif /* GCC */
# else
extern VOID CDECL NEAR mlwrite(char *fmt, ...);
# endif
extern VOID PASCAL NEAR ab_init(VOID);
extern VOID PASCAL NEAR ab_save(char c);
extern VOID PASCAL NEAR ab_expand(VOID);
extern VOID PASCAL NEAR clist_buffer(char *name, int *cpos);
extern VOID PASCAL NEAR clist_command(char *name, int *cpos);
extern VOID PASCAL NEAR clist_file(char *name, int *cpos);
extern VOID PASCAL NEAR comp_buffer(char *name, int *cpos);
extern VOID PASCAL NEAR comp_command(char *name, int *cpos);
# if     !WINDOW_MSWIN
extern VOID PASCAL NEAR comp_file(char *name, int *cpos);
# endif
extern VOID PASCAL NEAR ecrypt(char *bptr, unsigned len);
extern VOID PASCAL NEAR errormesg(char *mesg, BUFFER *bp, LINE *lp);
extern VOID PASCAL NEAR flong_asc(char buf[], int width, long num);
extern VOID PASCAL NEAR freewhile(WHBLOCK *wp);
extern VOID PASCAL NEAR getdtime(char *ts);
extern VOID PASCAL NEAR initchars(VOID);
extern VOID PASCAL NEAR kdelete(VOID);
extern VOID PASCAL NEAR lowercase(unsigned char *cp);
extern VOID PASCAL NEAR mcclear(VOID);
extern VOID PASCAL NEAR mlabort(char *s);
extern VOID PASCAL NEAR mlerase(VOID);
extern VOID PASCAL NEAR mlferase(VOID);
extern VOID PASCAL NEAR mlforce(char *s);
extern VOID PASCAL NEAR mlout(int c);
extern VOID PASCAL NEAR mlputf(int s);
extern VOID PASCAL NEAR mlputi(int i, int r);
extern VOID PASCAL NEAR mlputli(long l, int r);
extern VOID PASCAL NEAR mlputs(char *s);
extern VOID PASCAL NEAR mlrquery(VOID);
extern VOID PASCAL NEAR modeline(EWINDOW *wp);
extern VOID PASCAL NEAR movecursor(int row, int col);
extern VOID PASCAL NEAR next_kill(VOID);
extern VOID PASCAL NEAR pad(char *s, int len);
extern VOID PASCAL NEAR reeat(int c);
extern VOID PASCAL NEAR reframe(EWINDOW *wp);
extern VOID PASCAL NEAR reform(char *para);
extern VOID PASCAL NEAR rmcclear(VOID);
extern VOID PASCAL NEAR setbit(int bc, EBITMAP cclmap);
extern VOID PASCAL NEAR setjtable(VOID);
extern VOID PASCAL NEAR unbind_buf(BUFFER *bp);
extern VOID PASCAL NEAR unqname(char *name);
extern VOID PASCAL NEAR updall(EWINDOW *wp);
extern VOID PASCAL NEAR update(int force);
extern VOID PASCAL NEAR update_size(VOID);
extern VOID PASCAL NEAR upddex(VOID);
extern VOID PASCAL NEAR updext(VOID);
extern VOID PASCAL NEAR updgar(VOID);
extern VOID PASCAL NEAR updone(EWINDOW *wp);
extern VOID PASCAL NEAR updpos(VOID);
extern VOID PASCAL NEAR updupd(int force);
extern VOID PASCAL NEAR upmode(VOID);
extern VOID PASCAL NEAR uppercase(unsigned char *cp);
extern VOID PASCAL NEAR upwind(VOID);
extern VOID PASCAL NEAR varinit(VOID);
extern VOID PASCAL NEAR varclean(UTABLE *ut);
extern VOID PASCAL NEAR uv_init(UTABLE *ut);
extern VOID PASCAL NEAR uv_clean(UTABLE *ut);
extern VOID PASCAL NEAR vtfree(VOID);
extern VOID cook(VOID);
extern VOID qin(int ch);
extern VOID qrep(int ch);
extern EWINDOW *PASCAL NEAR mousewindow(int row);
extern int PASCAL NEAR wpopup(BUFFER *popbuf);

# if     CTAGS
extern int PASCAL NEAR tagword(int f, int n);   /* vi-like tagging */
extern int PASCAL NEAR retagword(int f, int n); /* Try again (if redefined) */
extern int PASCAL NEAR backtagword(int f, int n); /* return from tagged word */
# endif

/* some library redefinitions */

# if WINXP == 0
char *strrev(char *);
# endif

# if WINXP || WINNT || WINDOW_MSWIN || (MSDOS && IC) || GCC || VMS
#  include <stdlib.h>
#  include <string.h>
# else
char *getenv(char *);
char *strcat(char *, char *);
char *strcpy(char *, char *);
int  strncmp(char *, char *, int);
char *strchr(char *, int);
int  strcmp(char *, char *);
#  if     XVT == 0 || XVTDRIVER == 0
int  strlen(char *);
#   if RAMSIZE == 0
char *malloc(int);
VOID free(char *);
#   endif
char *realloc(char *block, int siz);
#  endif
# endif

#else

/***    global function declarations    ***/
# if CALLED
extern int emacs();
# endif

# if HANDLE_WINCH
extern VOID winch_changed();
extern VOID winch_new_size();
# endif

# if DEBUG_SEARCH
int PASCAL NEAR mc_list();
int PASCAL NEAR rmc_list();
VOID PASCAL NEAR mctype_cat();
# endif


extern VOID undo_insert();
extern int undo_op();
extern VOID undo_dump();
extern VOID undo_zot();
extern int PASCAL NEAR undo();
extern int PASCAL NEAR undo_delete();
extern int PASCAL NEAR undo_list();
extern VOID *room();
extern VOID *reroom();

extern int PASCAL NEAR ab_insert();
extern char *PASCAL NEAR ab_lookup();
extern int PASCAL NEAR ab_delete();
extern int PASCAL NEAR ab_clean();
extern BUFFER *PASCAL NEAR bfind();
extern BUFFER *PASCAL NEAR getcbuf();
extern BUFFER *PASCAL NEAR getdefb();
extern BUFFER *PASCAL NEAR getoldb();
extern SCREEN_T *PASCAL NEAR init_screen();
extern SCREEN_T *PASCAL NEAR lookup_screen();
extern SCREEN_T *PASCAL NEAR index_screen();
extern int PASCAL NEAR screen_index();
extern int PASCAL NEAR insert_screen();
extern int PASCAL NEAR select_screen();
extern int PASCAL NEAR free_screen();
extern char *Eallocate();
extern char *dolock();
extern char *getpath();
extern char *gtname();
extern char *PASCAL NEAR bytecopy();
extern char *PASCAL NEAR cmdstr();
extern char *PASCAL NEAR copystr();
extern char *PASCAL NEAR complete();
extern char *PASCAL NEAR envval();
extern CONST char *PASCAL NEAR fixnull();
extern CONST char *PASCAL NEAR flook();
extern char *PASCAL NEAR funval();
extern char *PASCAL NEAR getctext();
extern char *PASCAL NEAR getffile();
extern char *PASCAL NEAR getfname();
extern char *PASCAL NEAR getkill();
extern char *PASCAL NEAR getnfile();
extern CONST char *PASCAL NEAR getreg();
extern CONST char *PASCAL NEAR getval();
extern char *PASCAL NEAR getwlist();
extern CONST char *PASCAL NEAR gtenv();
extern char *PASCAL NEAR gtfilename();
extern CONST char *PASCAL NEAR gtfun();
extern CONST char *PASCAL NEAR gtusr();
extern char *PASCAL NEAR int_asc();
extern char *PASCAL NEAR long_asc();
extern CONST char *PASCAL NEAR ltos();
extern CONST char *PASCAL NEAR makename();
extern char *PASCAL NEAR mklower();
extern char *PASCAL NEAR mkupper();
extern char *PASCAL NEAR namval();
extern char *PASCAL NEAR timeset();
extern char *PASCAL NEAR token();
extern CONST char *PASCAL NEAR transbind();
extern char *PASCAL NEAR trimstr();
extern char *PASCAL NEAR xlat();
extern char *undolock();
extern char *PASCAL NEAR regtostr();
extern int PASCAL NEAR lowerc();
extern int PASCAL NEAR upperc();
extern int ( PASCAL NEAR *PASCAL NEAR fncmatch() )();
extern int ( PASCAL NEAR *PASCAL NEAR getname() )();
extern int PASCAL NEAR asc_int();
extern int dolhello();
extern int dspram();
extern int lckerror();
extern int lckhello();
extern int xlock();
extern int lockchk();
extern int lockrel();
extern int mousehello();
extern int nocrypt();
extern int PASCAL NEAR absv();
extern int PASCAL NEAR add_abbrev();
extern int PASCAL NEAR del_abbrev();
extern int PASCAL NEAR kill_abbrevs();
extern int PASCAL NEAR ins_abbrevs();
extern int PASCAL NEAR def_abbrevs();
extern int PASCAL NEAR addkey();
extern int PASCAL NEAR addkeymap();
extern int PASCAL NEAR addline();
extern int PASCAL NEAR amatch();
extern int PASCAL NEAR backhunt();
extern int PASCAL NEAR backsearch();
extern int PASCAL NEAR biteq();
extern int PASCAL NEAR bktoshell();
extern int PASCAL NEAR boundry();
extern int PASCAL NEAR cclmake();
extern int PASCAL NEAR checknext();
extern int PASCAL NEAR clear_ring();
extern int PASCAL NEAR cycle_ring();
extern int PASCAL NEAR delins();
extern int PASCAL NEAR desfunc();
extern int PASCAL NEAR dispvar();
extern int PASCAL NEAR echochar();
extern int PASCAL NEAR echostring();
extern int PASCAL NEAR eq();
extern long PASCAL NEAR ernd();
extern int PASCAL NEAR execkey();
extern int PASCAL NEAR fbound();
extern int PASCAL NEAR fexist();
extern int PASCAL NEAR findcol();
extern int PASCAL NEAR fisearch();
# if     FLABEL
extern int PASCAL NEAR fnclabel();
# endif
extern int PASCAL NEAR forwhunt();
extern int PASCAL NEAR forwsearch();
extern int PASCAL NEAR getcwnum();
extern int PASCAL NEAR getgoal();
extern int PASCAL NEAR getstring();
extern int PASCAL NEAR gettwnum();
extern int PASCAL NEAR gettyp();
extern int PASCAL NEAR getkey();
extern int PASCAL NEAR getwpos();
extern int PASCAL NEAR get_char();
extern int PASCAL NEAR global_var();
# if     DBCS
extern int PASCAL NEAR is2byte();
# endif
extern int PASCAL NEAR is_letter();
extern int PASCAL NEAR is_lower();
extern int PASCAL NEAR is_num();
extern int PASCAL NEAR isearch();
extern int PASCAL NEAR is_upper();
extern int PASCAL NEAR kinsert();
extern int PASCAL NEAR ldelnewline();
extern int PASCAL NEAR linstr();
extern int PASCAL NEAR liteq();
extern int PASCAL NEAR litmake();
extern int PASCAL NEAR lnewline();
extern int PASCAL NEAR local_var();
extern int PASCAL NEAR lookup_color();
extern int PASCAL NEAR lover();
extern int PASCAL NEAR mceq();
extern int PASCAL NEAR mcscanner();
extern int PASCAL NEAR mcstr();
extern int PASCAL NEAR mlprompt();
extern int PASCAL NEAR nextch();
extern int PASCAL NEAR pop();
extern int PASCAL NEAR qreplace();
extern int PASCAL NEAR readpattern();
# if     WINDOW_TEXT
extern VOID PASCAL NEAR refresh_screen();
# endif
extern int PASCAL NEAR reglines();
extern int PASCAL NEAR rename_screen();
extern int PASCAL NEAR replaces();
extern int PASCAL NEAR risearch();
extern int PASCAL NEAR rmcstr();
extern int PASCAL NEAR savematch();
extern int PASCAL NEAR scanmore();
extern int PASCAL NEAR scanner();
extern int PASCAL NEAR setlower();
extern int PASCAL NEAR setlower();
extern int PASCAL NEAR setupper();
extern int PASCAL NEAR setupper();
extern int PASCAL NEAR setvar();
extern int PASCAL NEAR sindex();
extern int PASCAL NEAR sreplace();
extern int PASCAL NEAR stol();
# if     DBCS
extern int PASCAL NEAR stopback();
extern int PASCAL NEAR stopforw();
# endif
extern int PASCAL NEAR svar();
extern int PASCAL NEAR tgetc();
extern int PASCAL NEAR uneat();
extern int PASCAL NEAR unlist_screen();
extern int PASCAL NEAR upscreen();
extern int PASCAL NEAR vtinit();
extern int PASCAL NEAR yank();
extern int PASCAL NEAR yank_pop();
extern int Erelease();
extern int set_key();
extern int xunlock();
extern KEYTAB *getbind();
extern LINE *PASCAL NEAR lalloc();
extern LINE *PASCAL NEAR mouseline();
extern long PASCAL NEAR getlinenum();
extern int PASCAL NEAR adjustmode();
extern int PASCAL NEAR anycb();
extern int PASCAL NEAR apro();
extern int PASCAL NEAR backchar();
extern int PASCAL NEAR backdel();
extern int PASCAL NEAR backline();
extern int PASCAL NEAR backpage();
extern int PASCAL NEAR backword();
extern int PASCAL NEAR bclear();
extern int PASCAL NEAR binary();
extern int PASCAL NEAR bindtokey();
extern int PASCAL NEAR buildlist();
extern int PASCAL NEAR capword();
extern int PASCAL NEAR cex();
extern int PASCAL NEAR cinsert();
extern int PASCAL NEAR clean();
extern int PASCAL NEAR clrmes();
extern int PASCAL NEAR copyregion();
extern int PASCAL NEAR ctlxe();
extern int PASCAL NEAR ctlxlp();
extern int PASCAL NEAR ctlxrp();
extern int PASCAL NEAR ctoec();
extern int PASCAL NEAR ctrlg();
extern int PASCAL NEAR cycle_screens();
extern VOID PASCAL NEAR dcline();
extern int PASCAL NEAR deblank();
extern int PASCAL NEAR debug();
extern int PASCAL NEAR delbword();
extern int PASCAL NEAR delete_screen();
extern int PASCAL NEAR delfword();
extern int PASCAL NEAR delgmode();
extern int PASCAL NEAR delmode();
extern int PASCAL NEAR delwind();
extern int PASCAL NEAR desbind();
extern int PASCAL NEAR desc_abbrevs();
extern int PASCAL NEAR deskey();
extern int PASCAL NEAR desvars();
extern int PASCAL NEAR detab();
extern int PASCAL NEAR dobuf();
extern int PASCAL NEAR docmd();
extern int PASCAL NEAR dofile();
extern int PASCAL NEAR ectoc();
extern VOID PASCAL NEAR edinit();
extern int PASCAL NEAR editloop();
extern int PASCAL NEAR endword();
extern int PASCAL NEAR enlargewind();
extern int PASCAL NEAR entab();
extern int PASCAL NEAR execbuf();
extern int PASCAL NEAR execcmd();
extern int PASCAL NEAR execfile();
extern int PASCAL NEAR execprg();
extern int PASCAL NEAR execproc();
extern int PASCAL NEAR execute();
extern int PASCAL NEAR ffclose();
extern int PASCAL NEAR ffgetline();
extern int PASCAL NEAR ffputline();
extern int PASCAL NEAR ffropen();
extern int PASCAL NEAR ffwopen();
extern int PASCAL NEAR fileapp();
extern int PASCAL NEAR find_screen();
extern int PASCAL NEAR filefind();
extern int PASCAL NEAR filename();
extern int PASCAL NEAR fileread();
extern int PASCAL NEAR filesave();
extern int PASCAL NEAR filewrite();
extern int PASCAL NEAR fillpara();
extern int PASCAL NEAR f_filter();
extern VOID PASCAL NEAR findvar();
extern int PASCAL NEAR fmatch();
extern int PASCAL NEAR forwchar();
extern int PASCAL NEAR forwdel();
extern int PASCAL NEAR forwline();
extern int PASCAL NEAR forwpage();
extern int PASCAL NEAR forwword();
extern int PASCAL NEAR getccol();
extern int PASCAL NEAR getcmd();
extern int PASCAL NEAR getfence();
extern int PASCAL NEAR getfile();
extern int PASCAL NEAR get_key();
extern int PASCAL NEAR getregion();
extern int PASCAL NEAR gotobob();
extern int PASCAL NEAR gotobol();
extern int PASCAL NEAR gotobop();
extern int PASCAL NEAR gotoeob();
extern int PASCAL NEAR gotoeol();
extern int PASCAL NEAR gotoeop();
extern int PASCAL NEAR gotoline();
extern int PASCAL NEAR gotomark();
extern int PASCAL NEAR help();
extern int PASCAL NEAR ifile();
extern int PASCAL NEAR indent();
extern int PASCAL NEAR indent_region();
extern int PASCAL NEAR insbrace();
extern int PASCAL NEAR insfile();
extern int PASCAL NEAR inspound();
extern int PASCAL NEAR insspace();
extern int PASCAL NEAR inword();
extern int PASCAL NEAR isinword();
extern int PASCAL NEAR ismodeline();
extern int PASCAL NEAR istring();
extern int PASCAL NEAR killbuffer();
extern int PASCAL NEAR killpara();
extern int PASCAL NEAR killregion();
extern int PASCAL NEAR killtext();
extern int PASCAL NEAR lchange();
extern int PASCAL NEAR ldelete();
extern int PASCAL NEAR lfree();
extern int PASCAL NEAR linsert();
extern int PASCAL NEAR listbuffers();
extern int PASCAL NEAR list_screens();
extern int PASCAL NEAR lowerregion();
extern int PASCAL NEAR lowerword();
extern int PASCAL NEAR lowrite();
extern int PASCAL NEAR macarg();
extern int PASCAL NEAR macrotokey();
extern int PASCAL NEAR makelist();
extern int PASCAL NEAR undolist();
extern int PASCAL NEAR mouse_screen();
extern int PASCAL NEAR screenlist();
extern int PASCAL NEAR meexit();
extern int PASCAL NEAR f_meta();
extern int PASCAL NEAR mlreply();
extern int PASCAL NEAR mlyesno();
extern int PASCAL NEAR mouseoffset();
extern int PASCAL NEAR movemd();
extern int PASCAL NEAR movemu();
extern int PASCAL NEAR mregdown();
extern int PASCAL NEAR mmove();
extern int PASCAL NEAR mregup();
extern int PASCAL NEAR mvdnwind();
extern int PASCAL NEAR mvupwind();
extern int PASCAL NEAR namebuffer();
extern int PASCAL NEAR namedcmd();
extern int PASCAL NEAR narrow();
extern int PASCAL NEAR newline();
extern int PASCAL NEAR new_col_org();
extern int PASCAL NEAR new_row_org();
extern int PASCAL NEAR newsize();
extern int PASCAL NEAR newwidth();
extern int PASCAL NEAR nextarg();
extern int PASCAL NEAR nextbuffer();
extern int PASCAL NEAR nextdown();
extern int PASCAL NEAR nextup();
extern int PASCAL NEAR nextwind();
extern int PASCAL NEAR nullproc();
extern int PASCAL NEAR onlywind();
extern int PASCAL NEAR openline();
extern int PASCAL NEAR ostring();
extern int PASCAL NEAR outstring();
extern int PASCAL NEAR ovstring();
extern int PASCAL NEAR pipecmd();
extern int PASCAL NEAR popbuffer();
extern int PASCAL NEAR prevwind();
extern int PASCAL NEAR putctext();
extern int PASCAL NEAR putline();
extern int PASCAL NEAR quickexit();
extern int PASCAL NEAR quit();
extern int PASCAL NEAR quote();
extern int PASCAL NEAR rdonly();
extern int PASCAL NEAR readin();
extern int PASCAL NEAR refresh();
extern int PASCAL NEAR remmark();
extern int PASCAL NEAR reposition();
extern int PASCAL NEAR resetkey();
extern int PASCAL NEAR resize();
extern int PASCAL NEAR resizm();
extern int PASCAL NEAR resterr();
extern int PASCAL NEAR restwnd();
extern int PASCAL NEAR savewnd();
extern int PASCAL NEAR scwrite();
extern int PASCAL NEAR setccol();
extern int PASCAL NEAR setekey();
extern int PASCAL NEAR setfillcol();
extern int PASCAL NEAR setgmode();
extern int PASCAL NEAR setmark();
extern int PASCAL NEAR setmod();
extern int PASCAL NEAR setwlist();
extern int PASCAL NEAR shellprog();
extern int PASCAL NEAR showfiles();
extern int PASCAL NEAR listkeymaps();
extern int PASCAL NEAR showcpos();
extern int PASCAL NEAR shrinkwind();
extern int PASCAL NEAR spal();
extern int PASCAL NEAR spawn();
extern int PASCAL NEAR spawncli();
extern int PASCAL NEAR splitwind();
extern int PASCAL NEAR startup();
extern int PASCAL NEAR storeproc();
extern int PASCAL NEAR strinc();
extern int PASCAL NEAR swapmark();
extern int PASCAL NEAR swbuffer();
extern int PASCAL NEAR tab();
extern int PASCAL NEAR trim();
extern int PASCAL NEAR ttclose();
extern int PASCAL NEAR ttflush();
extern int PASCAL NEAR ttgetc();
extern int PASCAL NEAR ttopen();
extern int PASCAL NEAR ttputc();
extern int PASCAL NEAR twiddle();
extern int PASCAL NEAR typahead();
extern int PASCAL NEAR unarg();
extern int PASCAL NEAR unbindchar();
extern int PASCAL NEAR unbindkey();
extern int PASCAL NEAR undent_region();
extern int PASCAL NEAR unmark();
extern int PASCAL NEAR upperregion();
extern int PASCAL NEAR upperword();
extern int PASCAL NEAR usebuffer();
extern int PASCAL NEAR viewfile();
extern int PASCAL NEAR vteeol();
extern int PASCAL NEAR vtmove();
extern int PASCAL NEAR vtputc();
extern int PASCAL NEAR vttidy();
extern int PASCAL NEAR widen();
extern int PASCAL NEAR wordcount();
extern int PASCAL NEAR wrapword();
extern int PASCAL NEAR writemsg();
extern int PASCAL NEAR writeout();
extern int PASCAL NEAR zotbuf();
extern unsigned int PASCAL NEAR chcase();
extern unsigned int PASCAL NEAR getckey();
extern unsigned int PASCAL NEAR stock();
extern VOID CDECL NEAR mlwrite();
extern VOID PASCAL NEAR ab_init();
extern VOID PASCAL NEAR ab_save();
extern VOID PASCAL NEAR ab_expand();
extern VOID PASCAL NEAR clist_buffer();
extern VOID PASCAL NEAR clist_command();
extern VOID PASCAL NEAR clist_file();
extern VOID PASCAL NEAR comp_buffer();
extern VOID PASCAL NEAR comp_command();
extern VOID PASCAL NEAR comp_file();
extern VOID PASCAL NEAR ecrypt();
extern VOID PASCAL NEAR errormesg();
extern VOID PASCAL NEAR flong_asc();
extern VOID PASCAL NEAR freewhile();
extern VOID PASCAL NEAR getdtime();
extern VOID PASCAL NEAR initchars();
extern VOID PASCAL NEAR kdelete();
extern VOID PASCAL NEAR lowercase();
extern VOID PASCAL NEAR mcclear();
extern VOID PASCAL NEAR mlabort();
extern VOID PASCAL NEAR mlerase();
extern VOID PASCAL NEAR mlferase();
extern VOID PASCAL NEAR mlforce();
extern VOID PASCAL NEAR mlout();
extern VOID PASCAL NEAR mlputf();
extern VOID PASCAL NEAR mlputi();
extern VOID PASCAL NEAR mlputli();
extern VOID PASCAL NEAR mlputs();
extern VOID PASCAL NEAR mlrquery();
extern VOID PASCAL NEAR modeline();
extern VOID PASCAL NEAR movecursor();
extern VOID PASCAL NEAR next_kill();
extern VOID PASCAL NEAR pad();
extern VOID PASCAL NEAR reeat();
extern VOID PASCAL NEAR reframe();
extern VOID PASCAL NEAR reform();
extern VOID PASCAL NEAR rmcclear();
extern VOID PASCAL NEAR setbit();
extern VOID PASCAL NEAR setjtable();
extern VOID PASCAL NEAR unbind_buf();
extern VOID PASCAL NEAR unqname();
extern VOID PASCAL NEAR updall();
extern VOID PASCAL NEAR update();
extern VOID PASCAL NEAR update_size();
extern VOID PASCAL NEAR upddex();
extern VOID PASCAL NEAR updext();
extern VOID PASCAL NEAR updgar();
extern VOID PASCAL NEAR updone();
extern VOID PASCAL NEAR updpos();
extern VOID PASCAL NEAR upmode();
extern VOID PASCAL NEAR updupd();
extern VOID PASCAL NEAR uppercase();
extern VOID PASCAL NEAR upwind();
extern VOID PASCAL NEAR varinit();
extern VOID PASCAL NEAR varclean();
extern VOID PASCAL NEAR uv_init();
extern VOID PASCAL NEAR uv_clean();
extern VOID PASCAL NEAR vtfree();
extern VOID cook();
extern VOID qin();
extern VOID qrep();
extern EWINDOW *PASCAL NEAR mousewindow();
extern int PASCAL NEAR wpopup();

# if     CTAGS
extern int PASCAL NEAR tagword();       /* vi-like tagging */
extern int PASCAL NEAR retagword();     /* Try again (if redefined) */
extern int PASCAL NEAR backtagword();   /* return from tagged word */
# endif

/* some library redefinitions */

char *getenv();
char *strcat();
char *strcpy();
char *strrev();
# if     XVT == 0 || XVTDRIVER == 0
int  strlen();
#  if RAMSIZE == 0
char *malloc();
VOID free();
#  endif
char *realloc();
# endif
#endif
/**********************************************************************/



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
