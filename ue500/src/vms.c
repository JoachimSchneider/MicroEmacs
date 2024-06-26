/*======================================================================
 *      VMS.C   Operating system specific I/O and spawning functions
 *              For VAX/VMS operating system
 *              for MicroEMACS 4.00
 *              Copyright 1995 by Jeffrey A. Lomicka and Daniel M. Lawrence
 *
 *      All-new code replaces the previous VMS/SMG implementation which
 *      prevented using non-SMG terminal drivers (ansi, termcap).  New
 *      approach to terminal I/O, new (and more) subprocess control
 *      functions, Kept emacs support, mail/notes interface.
 *
 *      Some of the above may still be wishlist.
 *
 *      12-Dec-89       Kevin A. Mitchell
 *                      Start work on RMSIO code.
 *====================================================================*/

/*====================================================================*/
#define VMS_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#if     VMS
#include        "edef.h"
#include        "elang.h"

#include <ssdef.h>
#include <descrip.h>
#include <jpidef.h>
#include <iodef.h>
#include <ttdef.h>
#include <tt2def.h>
#include <msgdef.h>
#include <rms.h>
#include <ctype.h>
#include <time.h>
#include <starlet.h>
#include <lib$routines.h>
#include <str$routines.h>

static VOID next_read P1_(int, flag);

/*
        Compaq C 6.4 on VMS/VAX wants system API routines in lowercase
*/
#ifdef __VAX
#pragma message disable DUPEXTERN
#endif

/*
        These are the lib$spawn mode flags.  There's no .h for
        them in VAX C V2.4.
*/
#ifdef NEED_CLIDEF
#define CLI$M_NOCONTROL 32
#define CLI$M_NOCLISYM 2
#define CLI$M_NOLOGNAM 4
#define CLI$M_NOKEYPAD 8
#define CLI$M_NOTIFY 16
#define CLI$M_NOWAIT 1
#else
#include <clidef.h>
#endif
/*
        test macro is used to signal errors from system services
*/
#define test(s) do { int st; st = (s); if ((st&1)==0) lib$signal( st); } while ( 0 )

/*
        This routine returns a pointer to a descriptor of the supplied
        string. The descriptors are static allocated, and up to
        "NUM_DESCRIPTORS" may be used at once.  After that, the old ones
        are re-used. Be careful!

        The primary use of this routine is to allow passing of C strings into
        VMS system facilities and RTL functions.

        There are three forms:

                descrp( s, l)   String descriptor for buffer s, length l
                descptr( s)     String descriptor for asciz buffer s
                DESCPTR( s)     String descriptor for buffer s, using SIZEOF()
*/
#define NUM_DESCRIPTORS 10
struct dsc$descriptor_s *descrp P2_(CONST char *, s, int, l)
{
    static int  next_d  = 0;
    static struct dsc$descriptor_s dsclist[NUM_DESCRIPTORS];

    if (next_d >= NUM_DESCRIPTORS)
        next_d = 0;
    dsclist[next_d].dsc$w_length = l;
    dsclist[next_d].dsc$b_dtype = DSC$K_DTYPE_T;
    dsclist[next_d].dsc$b_class = DSC$K_CLASS_S;
    dsclist[next_d].dsc$a_pointer = (char *)s;
    return (&dsclist[next_d++]);
}

/*
 * Make pointer to descriptor from Asciz string.
 */
struct dsc$descriptor_s *descptr P1_(CONST char *, s)
{
    return (descrp(s, STRLEN(s)));
}

#define DESCPTR(s)      descrp( s, SIZEOF(s)-1)

/*
        These two structures, along with ttdef.h, are good for manipulating
        terminal characteristics.
*/

typedef struct {
    /* Terminal characteristics buffer */
    unsigned char   catgy;  /* Renamed from `class' to allow C++ compile  */
    unsigned char   type;
    unsigned short  width;
    unsigned int    tt1:24;
    unsigned char   page;
    unsigned long   tt2;
} TTCHAR;

typedef struct {
    /* More terminal characteristics (hidden in the status block) */
    short     status;
    char      txspeed;
    char      rxspeed;
    long      trash;
} TTCHARIOSB;

typedef struct {
    /* Status block for ordinary terminal reads */
    unsigned short status, len, term, tlen;
} TTIOSB;

typedef struct {
    /* Status block for mailbox reads */
    unsigned short status, len;
    long      sender_pid;
} MBIOSB;

typedef struct {
    /* Messages from the terminal or TW driver */
    short     msgtype;          /* Expecting MSG$_TRMHANGUP */
    short     unit;             /* Controller unit number */
    char      ctrl_len;         /* Length of controller name (should be 3) */
    char      ctrl[15];         /* Controller name (should be TWA) */
    short     brdcnt;           /* Broadcast message byte count, if
                                 * MSG$TRMBRDCST */
    char      message[514];     /* First two bytes of broadcast message */
} TTMESSAGE;

static CONST int noterm[] = {0, 0};  /* Terminator list of NONE */
static int newbrdcst = FALSE;   /* Flag - is message in Emacs buffer yet. */

#define MINREAD 128             /* Smallest read to queue */
#define TYPSIZE 1024            /* Typeahead buffer size, must be several
                                 * times MINREAD */

static unsigned char tybuf[TYPSIZE];    /* Typeahead buffer */
static unsigned tyin, tyout, tylen, tymax;      /* Inptr, outptr, and length */

#define NINCHAR   64                    /* Input buffer size          */
static int inbuf[NINCHAR];              /* Input buffer               */
static int * inbufh = inbuf;            /* Head of input buffer       */
static int * inbuft = inbuf;            /* Tail of input buffer       */
#define TRACE_inbuf(where) do  {                                  \
    int i = 0;                                                    \
                                                                  \
    TRC(("%12s: inbuft = %d, inbufh = %d, inbuf = ",              \
          (char *)(where), (int)(inbuft - inbuf),                 \
          (int)(inbufh - inbuf)));                                \
    for ( i = 0; i < NELEM(inbuf) - 1; i++ )  {                   \
        TRC(("0x%04X, ", inbuf[i]));                              \
    }                                                             \
    TRC(("0x%04X\n", inbuf[i]));                                  \
} while ( 0 )

static TTIOSB ttiosb;           /* Terminal I/O status block */
static MBIOSB mbiosb;           /* Associated mailbox status block */
static TTMESSAGE mbmsg;         /* Associated mailbox message */
unsigned  NOSHARE short vms_iochan;     /* VMS I/O channel open on terminal */
static short mbchan;            /* VMS I/O channel open on associated mbx */
static short waiting;           /* Flag FALSE if read already pending */
static short stalled;           /* Flag TRUE if I/O stalled by full buffer */


/*
        If we come from ME$EDIT, the "suspend-emacs" is not allowed, since
        it will tend to wake itself up and re-hiberneate itself, which is
        a problem.
*/
static short called = 0;        /* TRUE if called from ME$EDIT */
/*
        short_time[ 0] is the negative number of 100ns units of time to
        wait.  -10000 is 1ms, therefore 200ms (2 tenths of a second) is
        -2,000,000.  Hopefully this is long enough for the network delay
        that might be involved between seeing the ESC and seeing the
        characters that follow it.

        This will be initialized from the environment variable
        MICROEMACS$SHORTWAIT.
*/
static long short_time[2] = {-4000000, -1};

static unsigned char tobuf[TYPSIZE];    /* Output buffer */
static unsigned tolen;          /* Ammount used */
NOSHARE   TTCHAR orgchar;       /* Original characteristics */
static TTCHARIOSB orgttiosb;    /* Original IOSB characteristics */

static VOID readast P0_()
{
    /* Data arrived from the terminal */
    waiting = 1;
    if ((ttiosb.status == SS$_TIMEOUT) || (ttiosb.status & 1)) {
        /* Read completed okay, process the data */
        if (ttiosb.len) {
            /* Got some data, adjust input queue parameters */
            tylen += ttiosb.len;
            tyin += ttiosb.len;
            test(sys$wake(0, 0));
            next_read(1);
        } else {
            /*
             * The user seems to have stopped typing, issue a read that will
             * wake us up when the next character is typed
             */
            if (!mbchan)
                next_read(0);
        }
    } else if (ttiosb.status != SS$_ABORT)
        lib$signal(ttiosb.status);
}

/*
 * flag = TRUE to use timeout of 0.
 */
static VOID next_read P1_(int, flag)
{
    /* No current read outstanding, submit one */
    if (waiting || stalled) {
        unsigned  size;
        /*
         * Wrap the input pointer if out of room.
         */
        waiting = 0;
        if (SIZEOF(tybuf) - tyin < MINREAD) {
            tymax = tyin;
            tyin = 0;
        }

        size = tymax - tylen;

        if (tyin + size > SIZEOF(tybuf))
            size = SIZEOF(tybuf) - tyin;

        if (size >= MINREAD) {
            /* Only read if there is enough room */
            test(sys$qio(0, vms_iochan,
                         flag ?
                         IO$_READVBLK | IO$M_NOECHO | IO$M_TRMNOECHO |
                         IO$M_NOFILTR | IO$M_TIMED
                         :
                         IO$_READVBLK | IO$M_NOECHO | IO$M_TRMNOECHO |
                         IO$M_NOFILTR,
                         &ttiosb, readast, 0, &tybuf[tyin], flag ? size : 1,
                         0, noterm, 0, 0
                         ));
            stalled = 0;
        } else
            stalled = 1;
    }
}

/***********************************************************
* FUNCTION - RemoveEscapes - remove ANSI escapes from string
* (for broadcast messages that contain 'formatting')
***********************************************************/
static VOID RemoveEscapes P1_(char *, str)
{
    unsigned char *in   = (unsigned char *)str;
    char          *out  = str;

    while (*in) {
        switch (*in) {
        case 0x1b:              /* skip escape */
            in++;
            if (*in != '[') {   /* not a CSI */
                switch (*in) {
                    /* skip special characters */
                case ';':
                case '?':
                case '0':
                    in++;
                }

                /* skip any intermediate characters 0x20 to 0x2f */
                while (*in >= 0x20 && *in <= 0x2f)
                    in++;

                /* skip any final characters 0x30 to 0x7e */
                if (*in >= 0x30 && *in <= 0x7e)
                    in++;
                break;
            }
            /* fall through to CSI */
        case 0x9b:              /* skip CSI */
            in++;

            /* skip any parameters 0x30 to 0x3f */
            while (*in >= 0x30 && *in <= 0x3f)
                in++;

            /* skip any intermediates 0x20 to 0x2f */
            while (*in >= 0x20 && *in <= 0x2f)
                in++;

            /* skip one final character 0x40 to 0x7e */
            if (*in >= 0x40 && *in <= 0x7e)
                in++;
            break;
        default:
            *out++ = *in++;
        }
    }
    *out = 0;
}

/*
 * The argument msgbuf points to the buffer we want to
 * insert our broadcast message into.
 */
static int  brdaddline P1_(BUFFER *, msgbuf)
{
    REGISTER  EWINDOW *wp;

    if (addline(msgbuf, brdcstbuf) == FALSE)
        return FALSE;

    /*
     * If one of the windows has the buffer
     * displayed, flag that window for update.
     */
    wp = wheadp;
    while (wp != NULL) {
        if (wp->w_bufp == msgbuf)
            wp->w_flag |= WFMODE | WFHARD;

        wp = wp->w_wndp;
    }
    update(FALSE);

    return (TRUE);
}

static VOID chkbrdcst P0_()
{
    BUFFER   *msgbuf;           /* buffer containing messages */

    if (newbrdcst) {
        int       oldrow = ttrow, oldcol = ttcol;

        sys$setast(0);

        msgbuf = bfind("[-messages-]", TRUE, BFINVS);

        if (msgbuf) {
            msgbuf->b_mode |= MDVIEW;
            brdaddline(msgbuf);
        }

        newbrdcst = FALSE;
        movecursor(oldrow, oldcol);
        TTmove(oldrow, oldcol);
        sys$setast(1);
    }
}

static VOID mbreadast P0_()
{
    if (mbiosb.status & 1) {
        /* Read completed okay, check for hangup message */
        if (mbmsg.msgtype == MSG$_TRMHANGUP) {
            /* Got a termination message, process it */
        } else if (mbmsg.msgtype == MSG$_TRMUNSOLIC) {
            /* Got unsolicited input, get it */
            next_read(1);
        } else if (mbmsg.msgtype == MSG$_TRMBRDCST) {
            /* Got broadcast, get it */
            /* Hard-coding the mbmsg.brdcnt to 511 is a temp solution. */
            mbmsg.brdcnt = 511;
            memcpy(brdcstbuf, mbmsg.message, 511);
            brdcstbuf[511] = 0;

            RemoveEscapes(brdcstbuf);
            pending_msg = newbrdcst = TRUE;
        } else {
          /**EMPTY**/
        }
        test(sys$qio(           /* Post a new read to the associated mailbox */
                     0, mbchan, IO$_READVBLK, &mbiosb,
                     mbreadast, 0, &mbmsg, SIZEOF(mbmsg),
                     0, 0, 0, 0
                     ));
    } else if (mbiosb.status != SS$_ABORT)
        lib$signal(mbiosb.status);
}

int PASCAL NEAR ttopen P0_()
{
    TTCHAR    newchar;          /* Adjusted characteristics */
    int       status;
    char     *waitstr;
    size_t    mbmsg_size = sizeof(mbmsg);

    xstrcpy(os, "VMS");
    tyin = 0;
    tyout = 0;
    tylen = 0;
    tymax = SIZEOF(tybuf);
    status = lib$asn_wth_mbx(   /* Create a new PY/TW pair */
                             descptr("SYS$OUTPUT:"),
                             &mbmsg_size,
                             &mbmsg_size,
                             &vms_iochan,
                             &mbchan);
    if ((status & 1) == 0) {
        /* The assign channel failed, was it because of the mailbox? */
        if (status == SS$_DEVACTIVE) {
            /* We've been called from NOTES, so we can't use the mailbox */
            test(sys$assign(descptr("SYS$OUTPUT:"), &vms_iochan, 0, 0));
            mbchan = 0;
        } else
            lib$signal(status);
    }
    waiting = 0;                /* Block unsolicited input from issuing read */
    stalled = 0;                /* Don't start stalled */
    if (mbchan)
        test(sys$qio(           /* Post a read to the associated mailbox */
                     0, mbchan, IO$_READVBLK, &mbiosb,
                     mbreadast, 0, &mbmsg, SIZEOF(mbmsg),
                     0, 0, 0, 0
                     ));
/*
        Fetch the characteristics and adjust ourself for proper operation.
*/
    test(sys$qiow(0, vms_iochan, IO$_SENSEMODE, &orgttiosb,
                  0, 0, &orgchar, SIZEOF(orgchar), 0, 0, 0, 0));
    newchar = orgchar;
    newchar.tt2 |= TT2$M_PASTHRU;       /* Gives us back ^U, ^X, ^C, and ^Y. */
    newchar.tt2 |= TT2$M_BRDCSTMBX;     /* Get broadcast messages */
    newchar.tt1 &= ~TT$M_MBXDSABL;      /* Make sure mailbox is on */
    newchar.tt1 |= TT$M_NOBRDCST;       /* Don't trash the screen with these */

/*
        Hostsync allows super-fast typing (workstation paste, PC
        send-file) without loss of data, as long as terminal supports
        XON/XOFF.  VWS and DECWindows terminal emulators require HOSTSYNC
        for PASTE operations to work, even though there is no wire involved.
*/
    newchar.tt1 |= TT$M_HOSTSYNC;
/*
        If you MUST, and if you know you don't need XON/XOFF
        synchronization, you can get ^S and ^Q back as data by defining
        XONDATA in ESTRUCT.H.  This is guarnteed to fail on VT125, VT100's
        over 3600 baud, any serial line terminal with smooth scroll
        enabled, VT200's over 4800 baud.  This is guarnteed to WORK if you
        are using a VT330/340 with SSU enabled, a VWS or DECWindows
        terminal emulator.  Note that if XONDATA is not set, I trust the
        settings the user has, so you just $ SET TERM /[NO]TTSYNC as you wish.
*/
#if XONDATA
    newchar.tt1 &= ~TT$M_TTSYNC;
#endif
/*
        I checked in DISPLAY.C, and verified that the mrow and mcol
        numbers aren't used until after ttopen() is called.  I override
        the terminal-supplied numbers with large numbers of my own, so
        that workstation terminal resizes will work to reasonable limits.

        I don't just use the current sizes as the maximum, becuase it's
        possible to resize the terminal emulator after Emacs is started,
        or even to disconnect and reconnect with a new terminal size, so
        the maximums must not change over multiple calls to ttopen().

        Also note that I do the changes to newchar, so that the actual
        terminal window will be reduced to the maximum values Microemacs
        will support.
*/
    term.t_mrow = 72;           /* 72 is European full page */
    term.t_mcol = 256;          /* 256 is Wider than any termnal I've tried */
    if (newchar.page > term.t_mrow)
        newchar.page = term.t_mrow;
    term.t_nrow = newchar.page - 1;
    if (newchar.width > term.t_mcol)
        newchar.width = term.t_mcol;
    term.t_ncol = newchar.width;
/*
        Set these new characteristics
*/
    test(sys$qiow(0, vms_iochan, IO$_SETMODE, 0,
                  0, 0, &newchar, SIZEOF(newchar), 0, 0, 0, 0));
/*
        For some unknown reason, if I don't post this read (which will
        likely return right away) then I don't get started properly.
        It has something to do with priming the unsolicited input system.
*/
    test(sys$qio(0, vms_iochan,
                 IO$_READVBLK | IO$M_NOECHO | IO$M_TRMNOECHO |
                 IO$M_NOFILTR | IO$M_TIMED,
                 &ttiosb, readast, 0, tybuf, SIZEOF(tybuf),
                 0, noterm, 0, 0
                 ));
/*
        Initialize the short_time value for esc-reads.  Larger values may
        be needed on network links.  I'm still experimeinting to get the
        best numbers.
*/
    waitstr = getenv("MICROEMACS$SHORTWAIT");
    if (waitstr)
        short_time[0] = -asc_int(waitstr);

    return 0;
}

int PASCAL NEAR ttclose P0_()
{
    if (tolen > 0) {
        /* Buffer not empty, flush out last stuff */
        test(sys$qiow(0, vms_iochan, IO$_WRITEVBLK | IO$M_NOFORMAT,
                      0, 0, 0, tobuf, tolen, 0, 0, 0, 0));
        tolen = 0;
    }
    test(sys$cancel(vms_iochan));       /* Cancel any pending read */
    test(sys$qiow(0, vms_iochan, IO$_SETMODE, 0,
                  0, 0, &orgchar, SIZEOF(orgchar), 0, 0, 0, 0));
    if (mbchan)
        test(sys$dassgn(mbchan));
    test(sys$dassgn(vms_iochan));

    return 0;
}

int PASCAL NEAR ttputc P1_(int, c)
{
    tobuf[tolen++] = c;
    if (tolen >= SIZEOF(tobuf)) {
        /* Buffer is full, send it out */
        test(sys$qiow(0, vms_iochan, IO$_WRITEVBLK | IO$M_NOFORMAT,
                      0, 0, 0, tobuf, tolen, 0, 0, 0, 0));
        tolen = 0;
    }

    return 0;
}

int PASCAL NEAR ttflush P0_()
{
    /*
            I choose to ignore any flush requests if there is typeahead
            pending.  Speeds DECNet network operation by leaps and bounds
            (literally).
    */
    if (tylen == 0) {
        if (tolen != 0) {
            /* No typeahead, send it out */
            test(sys$qiow(0, vms_iochan, IO$_WRITEVBLK | IO$M_NOFORMAT,
                          0, 0, 0, tobuf, tolen, 0, 0, 0, 0));
            tolen = 0;
        }
    }

    return 0;
}

/*
        grabnowait is a routine that tries to read another character,
        and if one doesn't come in as fast as we expect function keys
        sequences to arrive, we return grabnowait_TIMEOUT. This is
        called after receving ESC to check for next character. It's
        okay to wait too long, but the length of the delay controls how
        log the user waits after hitting ESC before results are seen.

        Note that we also wake from hibernation if a character arrives,
        so this never causes an undue delay if the user it actually
        typing.
*/
unsigned char PASCAL NEAR grabnowait P0_()
{
    if (tylen == 0) {
        /* Nothing immediately available, hibernate for a short time */
        test(sys$schdwk(0, 0, short_time, 0));
        test(sys$hiber());
    }

    return ((tylen == 0) ? grabnowait_TIMEOUT : grabwait());
}

unsigned char PASCAL NEAR grabwait P0_()
{
    REGISTER unsigned ret;

    chkbrdcst();
    while (tylen == 0) {
        /* Nothing to send, wait for something interesting */
        ttflush();
        test(sys$hiber());
        chkbrdcst();
    }

    /*
     * Got something, return it.
     */
    sys$setast(0);
    ret = tybuf[tyout++];

    if (tyout >= tymax) {
        tyout = 0;
        tymax = SIZEOF(tybuf);
    }

    tylen--;                    /* Should be ADD_INTERLOCKED */

    if (stalled && (tylen < 2 * MINREAD)) {
        test(sys$dclast(next_read, 1, 0));
    }

    sys$setast(1);

    return (ret);
}

# if ( !SMG ) /* Has it's own qin()/qrep()  */
/* QIN:
 *
 * Queue in a character to the input buffer.
 */
VOID qin P1_(int, ch)
{
#  if ( 0 )
    TRACE_inbuf("BEGIN qin");
#  endif
    /* Check for overflow */
    if ( inbuft == &inbuf[NELEM(inbuf)] ) {
        /* Annoy user */
        term.t_beep();

        return;
    }

    /* Add character */
    *inbuft++ = ch;
#  if ( 0 )
    TRACE_inbuf("  END qin");
#  endif
}

/* QREP:
 *
 * Replace a key sequence with a single character in the input buffer.
 */
VOID qrep P1_(int, ch)
{
#  if ( 0 )
    TRACE_inbuf("BEGIN qrep");
#  endif
    inbuft = inbuf;
    qin(ch);
#  if ( 0 )
    TRACE_inbuf("  END qrep");
#  endif
}
# endif /* ( !SMG ) */

/** Return cooked characters **/
int PASCAL NEAR ttgetc P0_()
{
    int ch  = 0;

    ttflush();
    /* Loop until character is in input buffer */
    while ( inbufh == inbuft )
        cook();

    /* Get input from buffer, now that it is available */
    ch = *inbufh++;

    /* reset us to the beginning of the buffer if there are no more pending
     * characters */
    if ( inbufh == inbuft )
        inbufh = inbuft = inbuf;

    /* Return next character */
# if ( 0 )
    TRC(("ttgetc(): 0x%04X", (unsigned int)ch));
# endif
    return (ch);
}

int ttgetc_nowait P0_()
{
    int ch  = 0;

    ttflush();
    /* Loop until character is in input buffer */
    while ( inbufh == inbuft )  {
        if ( !cook_nowait() ) {
            return grabnowait_TIMEOUT;
        }
    }

    /* Get input from buffer, now that it is available */
    ch = *inbufh++;

    /* reset us to the beginning of the buffer if there are no more pending
     * characters */
    if ( inbufh == inbuft )
        inbufh = inbuft = inbuf;

    /* Return next character */
# if ( 0 )
    TRC(("ttgetc_nowait(): 0x%04X", (unsigned int)ch));
# endif
    return (ch);
}

/*
 * Typahead - any characters pending?
 */
int PASCAL NEAR typahead P0_()
{
    return (tylen != 0);
}

/*
 * Shell out to DCL.
 */
int PASCAL NEAR spawncli P2_(int, f, int, n)
{
    /*
     * Don't allow this command if restricted
     */
    if (restflag)
        return (resterr());
    movecursor(term.t_nrow, 0); /* Seek to last line.   */
    TTclose();                  /* stty to old settings */
    test(lib$spawn(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    sgarbf = TRUE;
    TTopen();

    return (TRUE);
}

/*
 * Spawn a command.
 */
int PASCAL NEAR spawn P2_(int, f, int, n)
{
    REGISTER int s;
    char      line[NLINE];
    /*
     * Don't allow this command if restricted.
     */
    if (restflag)
        return (resterr());

    if ((s = mlreply("!", line, NLINE)) != TRUE)
        return (s);
    TTputc('\n');               /* Already have '\r' */
    TTflush();
    TTclose();                  /* stty to old modes */
    system(line);
    TTopen();
    TTflush();

    /* if we are interactive, pause here */
    if (clexec == FALSE) {
        mlputs(TEXT6);
/*                     "\r\n\n[End]" */
        tgetc();
    }
    sgarbf = TRUE;

    return (TRUE);
}

/*
 * Run an external program with arguments. When it returns, wait for a single
 * character to be typed, then mark the screen as garbage so a full repaint is
 * done. Bound to "C-X $".
 */
int PASCAL NEAR execprg P2_(int, f, int, n)
{
    REGISTER int s;
    char      line[NLINE];

    /* Don't allow this command if restricted. */
    if (restflag)
        return (resterr());

    if ((s = mlreply("!", line, NLINE)) != TRUE)
        return (s);
    TTputc('\n');               /* Already have '\r' */
    TTflush();
    TTclose();                  /* stty to old modes */
    system(line);
    TTopen();
    mlputs(TEXT188);            /* Pause. */
/*             "[End]" */
    TTflush();
    while ((s = tgetc()) != '\r' && s != ' ')
        ;
    sgarbf = TRUE;

    return (TRUE);
}

int PASCAL NEAR pipecmd P2_(int, f, int, n)
{
    REGISTER int s;             /* return status from CLI */
    REGISTER  EWINDOW *wp;      /* pointer to new window */
    REGISTER  BUFFER *bp;       /* pointer to buffer to zot */
    char      line[NLINE];      /* command line send to shell */
    static char bname[] = "command.log";

    static char filnam[NFILEN] = "command.log";

    /* don't allow this command if restricted */
    if (restflag)
        return (resterr());

    /* get the command to pipe in */
    if ((s = mlreply("@", line, NLINE)) != TRUE)
        return (s);

    /* get rid of the command output buffer if it exists */
    if ((bp = bfind(bname, FALSE, 0)) != FALSE) {
        /* try to make sure we are off screen */
        wp = wheadp;
        while (wp != NULL) {
            if (wp->w_bufp == bp) {
                onlywind(FALSE, 1);
                break;
            }
            wp = wp->w_wndp;
        }
        if (zotbuf(bp) != TRUE)

            return (FALSE);
    }

    TTputc('\n');               /* Already have '\r'     */
    TTflush();
    TTclose();                  /* stty to old modes    */

    test(lib$spawn(descptr(line), DESCPTR("NL:"), descptr(filnam),
                   0, 0, 0, 0, 0, 0, 0, 0));
    TTopen();
    TTflush();
    sgarbf = TRUE;
    s = TRUE;

    if (s != TRUE)
        return (s);

    /* split the current window to make room for the command output */
    if (splitwind(FALSE, 1) == FALSE)
        return (FALSE);

    /* and read the stuff in */
    if (getfile(filnam, FALSE) == FALSE)
        return (FALSE);

    /* make this window in VIEW mode, update all mode lines */
    curwp->w_bufp->b_mode |= MDVIEW;
    wp = wheadp;
    while (wp != NULL) {
        wp->w_flag |= WFMODE;
        wp = wp->w_wndp;
    }

    /* and get rid of the temporary file */
    umc_unlink(filnam);

    return (TRUE);
}

int PASCAL NEAR f_filter P2_(int, f, int, n)
{
     /* REGISTER */ int s;      /* return status from CLI */
    REGISTER  BUFFER *bp;       /* pointer to buffer to zot */
    char      line[NLINE];      /* command line send to shell */
    char      tmpnam[NFILEN];   /* place to store real file name */
    static char bname1[] = "fltinp.com";

    static char filnam1[] = "fltinp.com";
    static char filnam2[] = "fltout.log";

    /* don't allow this command if restricted */
    if (restflag)
        return (resterr());

    if (curbp->b_mode & MDVIEW) /* don't allow this command if  */
        return (rdonly());      /* we are in read only mode     */

    /* get the filter name and its args */
    if ((s = mlreply("#", line, NLINE)) != TRUE)
        return (s);

    /* setup the proper file names */
    bp = curbp;
    xstrcpy(tmpnam, bp->b_fname);       /* save the original name */
    xstrcpy(bp->b_fname, bname1);       /* set it to our new one */

    /* write it out, checking for errors */
    if (writeout(filnam1, "w") != TRUE) {
        mlwrite(TEXT2);
/*                      "[Cannot write filter file]" */
        xstrcpy(bp->b_fname, tmpnam);
        return (FALSE);
    }

    TTputc('\n');               /* Already have '\r'    */
    TTflush();
    TTclose();                  /* stty to old modes    */
    s = 1;

    test(lib$spawn(descptr(line), descptr(filnam1), descptr(filnam2),
                   0, 0, 0, &s, 0, 0, 0, 0));
    TTopen();
    TTflush();
    sgarbf = TRUE;
    s &= 1;

    /* on failure, escape gracefully */
    if (!s || (readin(filnam2, FALSE) == FALSE)) {
        mlwrite(TEXT3);
/*                      "[Execution failed]" */
        xstrcpy(bp->b_fname, tmpnam);
        umc_unlink(filnam1);
        umc_unlink(filnam2);
        return (s);
    }

    /* reset file name */
    xstrcpy(bp->b_fname, tmpnam);       /* restore name */
    bp->b_flag |= BFCHG;        /* flag it as changed */

    /* and get rid of the temporary file */
    umc_unlink(filnam1);
    umc_unlink(filnam2);

    return (TRUE);
}

/*
        The rename() function is built into the VMS C RTL, and need not be
        duplicated here.
*/

char * PASCAL NEAR  timeset P0_()
{
    REGISTER char *sp;          /* temp string pointer */
    time_t    buf;              /* time data buffer */

    time(&buf);
    sp = ctime(&buf);
    sp[STRLEN(sp) - 1] = 0;

    return (sp);
}

/*      FILE Directory routines         */

static char fname[NFILEN];      /* path of file to find */
static char path[NFILEN];       /* path of file to find */
static char rbuf[NFILEN];       /* return file buffer */
static char *ctxtp = NULL;      /* context pointer */
static struct dsc$descriptor pat_desc;  /* descriptor for pattern */
static struct dsc$descriptor rbuf_desc; /* descriptor for returned file name */

/*
 * Do a wild card directory search (for file name completion)
 * fspec is the pattern to match.
 */
char * PASCAL NEAR  getffile P1_(char *, fspec)
{

    REGISTER int index;         /* index into various strings */
    REGISTER int point;         /* index into other strings */
    REGISTER int extflag;       /* does the file have an extention? */
    REGISTER int verflag;       /* does the file have a version? */
    REGISTER char *cp, c;

    /* first parse the file path off the file spec */
    xstrcpy(path, fspec);
    index = STRLEN(path) - 1;
    while (index >= 0 && (path[index] != ']' && path[index] != ':'))
        --index;

    path[index + 1] = 0;

    /* check for a version number */
    point = STRLEN(fspec) - 1;
    verflag = FALSE;
    while (point >= 0) {
        if ((c = fspec[point]) == ';') {
            verflag = TRUE;
            break;
        } else if (c == '.' || c == ']' || c == ':')
            break;
        point--;
    }

    /* check for an extension */
    point = STRLEN(fspec) - 1;
    extflag = FALSE;
    while (point >= 0) {
        if ((c = fspec[point]) == '.') {
            extflag = TRUE;
            break;
        } else if (c == ']' || c == ':')
            break;
        point--;
    }

    /* construct the composite wild card spec */
    xstrcpy(fname, path);
    xstrcat(fname, &fspec[index + 1]);
    xstrcat(fname, "*");
    if (!extflag)
        xstrcat(fname, ".*");
    if (!verflag)
        xstrcat(fname, ";*");

    pat_desc.dsc$a_pointer = fname;
    pat_desc.dsc$w_length = STRLEN(fname);
    pat_desc.dsc$b_dtype = DSC$K_DTYPE_T;
    pat_desc.dsc$b_class = DSC$K_CLASS_S;

    for (cp = rbuf; cp != rbuf + NFILEN; *cp++ = ' ');
    rbuf_desc.dsc$a_pointer = rbuf;
    rbuf_desc.dsc$w_length = NFILEN;
    rbuf_desc.dsc$b_dtype = DSC$K_DTYPE_T;
    rbuf_desc.dsc$b_class = DSC$K_CLASS_S;

    lib$find_file_end(&ctxtp);
    ctxtp = NULL;
    if (lib$find_file(&pat_desc, &rbuf_desc, &ctxtp) != RMS$_SUC)
        return (NULL);

    /*
     * return the first file name! isolate the filename and extension and
     * append that on to the original path
     */
    for (cp = rbuf; *cp != ' ' && cp != rbuf + NFILEN - 1; cp++);
    *cp = 0;
    for (cp--; *cp != ';' && cp >= rbuf; cp--);
    *cp = 0;
    for (cp--; *cp != ']' && cp >= rbuf; cp--);
    xstrcat(path, ++cp);
    mklower(path);

    return (path);
}

char * PASCAL NEAR  getnfile P0_()
{
    REGISTER char *cp;

    /* and call for the next file */
    for (cp = rbuf; cp != rbuf + NFILEN; *cp++ = ' ');
    if (lib$find_file(&pat_desc, &rbuf_desc, &ctxtp) != RMS$_SUC)
        return (NULL);

    /*
     * return the next file name!
     * isolate the original path,
     * isolate the filename and extension,
     * and append filename/extension on to the original path
     */
    for (cp = path + STRLEN(path) - 1; *cp != ']' && cp >= path; cp--)
        ;

    *++cp = 0;
    for (cp = rbuf; *cp != ' ' && cp != rbuf + NFILEN - 1; cp++)
        ;
    *cp = 0;
    for (cp--; *cp != ';' && cp >= rbuf; cp--)
        ;
    *cp = 0;
    for (cp--; *cp != ']' && cp >= rbuf; cp--)
        ;
    xstrcat(path, ++cp);
    mklower(path);

    return (path);
}

#ifdef __cplusplus
extern "C" {
#endif
/*
        The following ME$EDIT entry point is used when MicroEmacs is
        called up from MAIL or NOTES.  Note that it may be called more than
        once, and that "main()" is never called.

        Mail/Notes entry point.  Should be declared UNIVERSAL in ME.OPT.
*/
int ME$EDIT P2_(struct dsc$descriptor *, infile, struct dsc$descriptor *, outfile)
{
    static int first_time = 1;
    char     *instr, *outstr;
    REGISTER int status;
    REGISTER  BUFFER *bp;       /* buffer list pointer */
    char      bname[NBUFN];     /* buffer name of file to read */

    eexitflag = FALSE;
    called = 1;
    if (first_time) {
        first_time = 0;
        vtinit();
        if (eexitflag)
            goto abortrun;
        edinit(mainbuf);        /* Buffers, windows */
        varinit();              /* user variables */
        initchars();            /* character set definitions */
        dcline(0, NULL, TRUE);
    } else
        TTopen();

    outstr = xstrncpy((char *)calloc(1, 1 + outfile->dsc$w_length),
                      outfile->dsc$a_pointer, outfile->dsc$w_length);

    if (infile->dsc$w_length == 0)
        instr = outstr;
    else
        instr = xstrncpy((char *)calloc(1, 1 + infile->dsc$w_length),
                         infile->dsc$a_pointer, infile->dsc$w_length);

    makename(bname, outstr);
    unqname(bname);
    bp = bfind(bname, TRUE, 0);
    xstrcpy(bp->b_fname, instr);
    bp->b_active = FALSE;
    swbuffer(bp);
    xstrcpy(bp->b_fname, outstr);
    bp->b_flag |= BFCHG;        /* flag it as changed */
    free(instr);
    free(outstr);
    sgarbf = TRUE;
    status = editloop();

abortrun:
    TTclose();

    return (status);
}
#ifdef __cplusplus
}
#endif

int PASCAL NEAR bktoshell P2_(int, f, int, n)
{
    /*
            Pause this process and wait for it to be woken up
    */
    unsigned  pid;
    char     *env;
    int       argc;
    char     *argv[16];
    unsigned  jpi_pid = JPI$_PID;

    if (called) {
        mlwrite("Called MicroEMACS can't be suspended.");
        return (FALSE);
    }

    env = getenv("MICROEMACS$PARENT");

    if (env == NULL) {
        mlwrite("No parent process.");
        return (FALSE);
    }
    movecursor(term.t_nrow, 0);
    TTclose();

    test(lib$delete_logical(DESCPTR("MICROEMACS$PARENT"),
                            DESCPTR("LNM$JOB")));
    test(lib$getjpi(&jpi_pid, 0, 0, &pid, 0, 0));
    test(lib$set_logical(DESCPTR("MICROEMACS$PROCESS"),
                         descptr(int_asc(pid)),
                         DESCPTR("LNM$JOB")));
    pid = asc_int(env);
    test(sys$wake(&pid, 0));

    for (;;) {
        /* Hibernate until MICROEMACS$COMMAND is defined */
        test(sys$hiber());
        env = getenv("MICROEMACS$COMMAND");     /* Command line arguments */
        if (env != NULL)
            break;              /* Winter is over */
    }

    test(lib$delete_logical(DESCPTR("MICROEMACS$COMMAND"),
                            DESCPTR("LNM$JOB")));

    TTopen();

    argv[0] = env;
    argc = 1;
    for (;;) {
        /* Define each argument */
        if ((unsigned char)*env == 0x80) {
            /* Seperator */
            argv[argc++] = env + 1;
            if (argc > 15)
                break;
            *env++ = 0;
        } else if (*env++ == 0)
            break;
    }
/*
        First parameter is default device
*/
    test(lib$set_logical(DESCPTR("SYS$DISK"),
                         descptr(argv[0]),
                         0));
/*
        Second argument is default directory
*/
    test(sys$setddir(descptr(argv[1]), 0, 0));
/*
        Remaining came from command line
*/
    sgarbf = TRUE;
    dcline(argc - 2, &argv[2], FALSE);

    return (TRUE);
}

#if RMSIO
/*
 * Here are the much faster I/O routines.  Skip the C stuff, use
 * the VMS I/O calls.  Puts the files in standard VMS format, too.
 */
#define successful(s)   ((s) & 1)
#define unsuccessful(s) (!((s) & 1))

static struct FAB fab;          /* a file access block */
static struct RAB rab;          /* a record access block */

/*
 * Open a file for reading.
 */
int PASCAL NEAR ffropen P1_(CONST char *, fn)
{
    unsigned long status;

    /* initialize structures */
    fab = cc$rms_fab;
    rab = cc$rms_rab;

    fab.fab$l_fna = (char *)fn;
    fab.fab$b_fns = STRLEN(fn);
    fab.fab$b_fac = FAB$M_GET;
    fab.fab$b_shr = FAB$M_SHRGET;
    fab.fab$l_fop = FAB$M_SQO;

    rab.rab$l_fab = &fab;
    rab.rab$l_rop = RAB$M_RAH;  /* read-ahead for multibuffering */

    status = sys$open(&fab);
    if (status == RMS$_FLK) {
        /*
         * File locking problem:
         * Add the SHRPUT option, allowing shareability
         * with other writers. This lets us read batch
         * logs and stuff like that. I don't turn it on
         * automatically since adding this sharing
         * eliminates the read-ahead
         */
        fab.fab$b_shr |= FAB$M_SHRPUT;
        status = sys$open(&fab);
    }

    if (successful(status)) {
        if (unsuccessful(sys$connect(&rab))) {
            sys$close(&fab);
            return (FIOFNF);
        }
    } else
        return (FIOFNF);

    return (FIOSUC);
}

/*
 * PASCAL NEAR ffwopen(char *fn, char *mode)
 *
 * fn = file name, mode = mode to open file.
 */
int PASCAL NEAR ffwopen P2_(CONST char *, fn, CONST char *, mode)
{
    unsigned long status;

    /* initialize structures */
    fab = cc$rms_fab;
    rab = cc$rms_rab;

    fab.fab$l_fna = (char *)fn;
    fab.fab$b_fns = STRLEN(fn);
    fab.fab$b_fac = FAB$M_PUT;  /* writing this file */
    fab.fab$b_shr = FAB$M_NIL;  /* no other writers */
    fab.fab$l_fop = FAB$M_SQO;  /* sequential ops only */
    fab.fab$b_rat = FAB$M_CR;   /* carriage returns on ends */
    fab.fab$b_rfm = FAB$C_VAR;  /* variable length file */

    rab.rab$l_fab = &fab;
    rab.rab$l_rop = RAB$M_WBH;  /* write behind - multibuffer */

    if (*mode == 'a') {
        /* append mode */
        rab.rab$l_rop = RAB$M_EOF;
        status = sys$open(&fab);
        if (status == RMS$_FNF)
            status = sys$create(&fab);
    } else {                    /* *mode == 'w' */
        /* write mode */
        fab.fab$l_fop |= FAB$M_MXV;     /* always make a new version */
        status = sys$create(&fab);
    }

    if (successful(status)) {
        status = sys$connect(&rab);
        if (unsuccessful(status))
            sys$close(&fab);
    }

    if (unsuccessful(status)) {
        mlwrite(TEXT155);
/*                      "Cannot open file for writing" */
        return (FIOERR);
    }

    return (FIOSUC);
}

/*
 * Close a file. Should look at the status in all systems.
 */
int PASCAL NEAR ffclose P0_()
{
    unsigned long status;

    /* free this since we do not need it anymore */
    if (fline) {
        free(fline);
        fline = NULL;
    }

    status = sys$disconnect(&rab);
    if (successful(status))
        status = sys$close(&fab);
    else
        sys$close(&fab);

    if (unsuccessful(status)) {
        mlwrite(TEXT156);
/*                      "Error closing file" */
        return (FIOERR);
    }
    return (FIOSUC);
}

/*
 * Write a line to the already opened file. The "buf" points to the buffer,
 * and the "nbuf" is its length, less the free newline. Return the status.
 * Check only at the newline.
 */
int PASCAL NEAR ffputline P2_(char *, buf, int, nbuf)
{
    REGISTER char *obuf = buf;

#if     CRYPT
    if (cryptflag) {
        /* get a reasonable buffer */
        if (fline && flen < nbuf) {
            free(fline);
            fline = NULL;
        }

        if (fline == NULL) {
            if ((fline = (char *)malloc(flen = nbuf + NSTRING)) == NULL) {
                return (FIOMEM);
            }
        }

        /* copy data */
        memcpy(fline, buf, nbuf);

        /* encrypt it */
        ecrypt(fline, nbuf);

        /* repoint output buffer */
        obuf = fline;
    }
#endif

    /* set output buffer */
    rab.rab$l_rbf = obuf;
    rab.rab$w_rsz = nbuf;

    if (unsuccessful(sys$put(&rab))) {
        mlwrite(TEXT157);
/*                      "Write I/O error" */
        return (FIOERR);
    }

    return (FIOSUC);
}

/*
 * Read a line from a file, and store the bytes in the supplied buffer. The
 * "nbuf" is the length of the buffer. Complain about long lines and lines
 * at the end of the file that don't have a newline present. Check for I/O
 * errors too. Return status.
 */
int PASCAL NEAR ffgetline P1_(int *, nbytes)
/* nbytes:  Save our caller hassle, calc the line length  */
{
    unsigned long status;

    /* if we don't have an fline, allocate one */
    if (fline == NULL)
        if ((fline = (char *)malloc(flen = fab.fab$w_mrs ? fab.fab$w_mrs + 1 : 32768)) == NULL)
            return (FIOMEM);

    /* read the line in */
    rab.rab$l_ubf = fline;
    rab.rab$w_usz = flen;

    status = sys$get(&rab);
    *nbytes = rab.rab$w_rsz;
    if (status == RMS$_EOF)
        return (FIOEOF);
    if (unsuccessful(status)) {
        mlwrite(TEXT158);
/*                      "File read error" */
        return (FIOERR);
    }

    /* terminate and decrypt the string */
    fline[rab.rab$w_rsz] = 0;
#if     CRYPT
    if (cryptflag)
        ecrypt(fline, *nbytes);
#endif

    return (FIOSUC);
}

#endif

/***********************************************************
* FUNCTION - addspec - utility function for expandargs
***********************************************************/
#define ADDSPEC_INCREMENT 10
static VOID PASCAL NEAR addspec P4_(struct dsc$descriptor,  dsc,
                                    int *,                  pargc,
                                    char ***,               pargv,
                                    int *,                  pargcapacity)
{
    char     *s;

    /* reallocate the argument array if necessary */
    if (*pargc == *pargcapacity)
        *pargv = (char **)realloc(*pargv, SIZEOF(**pargv) * (*pargcapacity += ADDSPEC_INCREMENT));

    /* allocate new argument */
    s = xstrncpy((char *)malloc(dsc.dsc$w_length + 1), dsc.dsc$a_pointer, dsc.dsc$w_length);
    s[dsc.dsc$w_length] = 0;

    /* put into array */
    (*pargv)[(*pargc)++] = s;
}

/***********************************************************
* FUNCTION - expandargs - massage argc and argv to expand
* wildcards by calling VMS.
***********************************************************/
VOID PASCAL NEAR  expandargs P2_(int *, pargc, char ***, pargv)
{
    int       argc = *pargc;
    char    **argv = *pargv;

    int       nargc = 0;
    char    **nargv = NULL;
    int       nargcapacity = 0;

    struct dsc$descriptor result_filespec = {0, DSC$K_DTYPE_T, DSC$K_CLASS_D, NULL};

    /* loop over all arguments */
    while (argc--) {
        struct dsc$descriptor filespec = {STRLEN(*argv), DSC$K_DTYPE_T, DSC$K_CLASS_S, *argv};
        unsigned long context = 0;

        /* should check for wildcards: %, *, and "..." */
        if (**argv != '-' && (strchr(*argv, '%') || strchr(*argv, '*') ||
                              strstr(*argv, "..."))) {
            /* search for all matching filenames */
            while ((lib$find_file(&filespec, &result_filespec, &context)) & 1) {
                int       i;

                /* lib$find_file returns uppercase. Lowercase it */
                for (i = 0; i < result_filespec.dsc$w_length; i++)
                    if (is_upper(result_filespec.dsc$a_pointer[i]))
                        result_filespec.dsc$a_pointer[i] = tolower(result_filespec.dsc$a_pointer[i]);

                addspec(result_filespec, &nargc, &nargv, &nargcapacity);
            }
        } else
            addspec(filespec, &nargc, &nargv, &nargcapacity);

        lib$find_file_end(&context);

        argv++;
    }

    str$free1_dx(&result_filespec);

    *pargc = nargc;
    *pargv = nargv;
}

#else

VOID PASCAL NEAR  vms_hello P0_()
{
}

#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
