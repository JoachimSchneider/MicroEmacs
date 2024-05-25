/* SOF(Terminal_VMS.c)  */

/*====================================================================*/
/* Lookup headers in SYS$COMMON:[DECC$LIB.REFERENCE.DECC$RTLDEF] and  */
/*                in SYS$COMMON:[DECC$LIB.REFERENCE.SYS$STARLET_C]    */
/*                                                                    */
/* Documentation:                                                     */
/* - HP C User's Guide for OpenVMS Systems (Jan. 2005)                */
/* - Introduction to VMS System Services (Nov. 1991)                  */
/* - VMS System Services Reference Manual (Nov. 1991)                 */
/* - VSI OpenVMS RTL Library (LIB$) Manual (Aug. 2021)                */
/* - HP OpenVMS Programming Concepts Manual, Volume  I (Jan. 2005)    */
/* - HP OpenVMS Programming Concepts Manual, Volume II (Jan. 2005)    */
/* - OpenVMS Programming Concepts Manual Volume I, 7.3-1 (Jun. 2002)  */
/* - HP OpenVMS I/O User's Reference Manual (Jun. 2010)               */
/* - VMS 1/0 User's Reference Manual: Part I (Jun. 1990)              */
/* - VAX/VMS I/O User's Guide (Aug. 1987)                             */
/*                                                                    */
/* APIs:                                                              */
/*  - SYS$GETDVIW:  Get Device/Volume Information                     */
/*  - SYS$QIOW:     Queue I/O Request and Wait                        */
/*  - SYS$ASSIGN:   Provides a process with an I/O channel so that    */
/*                  input/output operations can be performed on a     */
/*                  device.                                           */
/*  - LIB$GET_COMMAND:  The Get Line from SYS$COMMAND routine gets    */
/*                      one record of ASCII text from the current     */
/*                      controlling input device, specified by the    */
/*                      logical name SYS$COMMAND.                     */
/*====================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <socket.h>       /* select(), struct timeval                               */
#include <lib$routines.h> /* lib$get_command()                                      */
#include <starlet.h>      /* sys$qiow(), sys$getdviw(), sys$assign(), sys$dassgn()  */
#include <ssdef.h>        /* SS$_NORMAL                                             */
#include <dcdef.h>        /* DC$_TERM                                               */
#include <dvidef.h>       /* DVI$_DEVCLASS                                          */
#include <tt2def.h>       /* TT2$M_PASTHRU                                          */
#include <iodef.h>        /* IO$_SENSEMODE, IO$_SETMODE, IO$_READVBLK,
                              IO$M_NOECHO, IO$M_TRMNOECHO, IO$M_NOFILTR             */
#include <descrip.h>      /* $DESCRIPTOR, dsc$descriptor_s,
                              DSC$K_DTYPE_T, DSC$K_CLASS_S                          */

/*======================================================================
 *  If the user has used /pointer_size=short or /pointer_size=long, we will
 *  begin in a 32-bit pointer size context.
 *====================================================================*/
#ifdef  __INITIAL_POINTER_SIZE
# if    __INITIAL_POINTER_SIZE
#  pragma __pointer_size __save
#  pragma __pointer_size 32
# endif
#endif
/*====================================================================*/


#define TRACE_MODE      (  0 )
#define TEST_MODE       (  0 )
#define READ_WITH_TOUT  ( !0 )
#define READ_WAIT_MS    ( 100 )


typedef unsigned char       uchar_t;
typedef unsigned short int  ushort_t;
typedef unsigned       int  uint_t;
typedef unsigned long  int  ulong_t;

typedef struct xile3_s  {
  ushort_t  buf_size;     /* Length of buffer in bytes            */
  ushort_t  item_code;    /* Item code value                      */
  void      *buf_addr;    /* Buffer address                       */
  ushort_t  *retlen_addr; /* Address of word for returned length  */
} xile3_t;
typedef struct xiosb_s  {
  ushort_t  status;
  ushort_t  nwritten;
  uint_t    dev_depend;
} xiosb_t;
typedef struct terminal_s {
  uint_t    dev_class;
  ushort_t  tt_chan;
} terminal_t;
typedef struct ttchar_s {
  /* Terminal characteristics buffer */
  uchar_t   catgy;
  uchar_t   type;
  ushort_t  width;
  uint_t    tt1:24;
  uchar_t   page;
  ulong_t   tt2;
} ttchar_t;
typedef struct ttchariosb_s {
  /* More terminal characteristics (hidden in the status block) */
  short     status;
  char      txspeed;
  char      rxspeed;
  long      trash;
} ttchariosb_t;


#define SUCCESS   (  0 )
#define ERRRET    ( -1 )  /* MUST BE .LT. 0 */


#define SIZEOF(x)       ( (int)sizeof((x)) )
#define ZEROMEM(x)      ( memset(&(x), 0, sizeof((x))) )
#define ASRT(x)         do {                              \
    if ( !(x) ) {                                         \
      fprintf(stderr, "[%s:%d]: ASSERTION `%s' failed\n", \
              __FILE__, __LINE__, #x);                    \
                                                          \
      abort();                                            \
    }                                                     \
  } while ( 0 )
#if ( !TRACE_MODE )
# define TRC(e)
#else
static int              g_line_   = 0;
static const char       *g_file_  = NULL;
# define TRC(e)          do {                 \
    g_file_ = __FILE__;                       \
    g_line_ = __LINE__;                       \
    printf("TRC[%s:%d]: ", g_file_, g_line_); \
    printf e;                                 \
    printf("%s\n", "");                       \
  } while ( 0 )
#endif
#define ALLOC(x)        ( calloc(1, (x)) )
#define ALLOCATE(x)     ASRT(NULL != (*&(x) = ALLOC(SIZEOF(*(x)))))
#define FREE(x)         do {  \
    char  **xp_  = NULL;      \
                              \
    xp_ = (char **)&(x);      \
    if ( NULL != *xp_ ) {     \
      free(*xp_);             \
      *xp_  = NULL;           \
    }                         \
  } while ( 0 )


#include "terminal_vms.h"


static ttchar_t   TerminalRaw(const terminal_t *tp);
static ttchar_t   TerminalCooked(const terminal_t *tp);
static ttchar_t   TerminalRestore(const terminal_t *tp, const ttchar_t *ttcharp);
static terminal_t *TerminalOpen(void);
static int        TerminalClose(const terminal_t *tp);
/* Return number of bytes read  */
static int        TerminalRead(const terminal_t *tp, char *buf, int bufsiz, int read_echo);
static void       microsleep(int us);


int TerminalReadVMS(char *buf, int bufsiz, int read_echo)
{
  static terminal_t *tp = NULL;
  ttchar_t          orgttchar;
  int               rc  = 0;

  ZEROMEM(orgttchar);

  if ( NULL == tp ) {
    tp  = TerminalOpen();
    ASRT(NULL != tp);
  }
  orgttchar = TerminalRaw(tp);
  rc  = TerminalRead(tp, buf, bufsiz, read_echo);
  TerminalRestore(tp, &orgttchar);

  return rc;
}


static ttchar_t TerminalRaw(const terminal_t *tp)
{
  /* This is `$ set terminal /PASTHRU'    */
  /* Could be executed via system()       */
  int           status  = 0;
  ttchar_t      oldchar;
  ttchar_t      newchar;
  ttchariosb_t  orgttiosb;

  ZEROMEM(oldchar);
  ZEROMEM(newchar);
  ZEROMEM(orgttiosb);

  ASRT(NULL != tp);

  status  = sys$qiow(0, tp->tt_chan, IO$_SENSEMODE, &orgttiosb,
                     0, 0, &oldchar, SIZEOF(oldchar), 0, 0, 0, 0);
  if ( 0 == (status & 1) )  {
    TRC(("status = %d", status));

    return oldchar;
  }

  newchar = oldchar;
  newchar.tt2 |= TT2$M_PASTHRU; /* Gives us back ^U, ^X, ^C, and ^Y. */

  status = sys$qiow(0, tp->tt_chan, IO$_SETMODE, 0,
                    0, 0, &newchar, SIZEOF(newchar), 0, 0, 0, 0);
  if ( 0 == (status & 1) )  {
    TRC(("status = %d", status));
  }

  return oldchar;
}

static ttchar_t TerminalCooked(const terminal_t *tp)
{
  /* This is `$ set terminal /NOPASTHRU'  */
  /* Could be executed via system()       */
  int           status  = 0;
  ttchar_t      oldchar;
  ttchar_t      newchar;
  ttchariosb_t  orgttiosb;

  ZEROMEM(oldchar);
  ZEROMEM(newchar);
  ZEROMEM(orgttiosb);

  ASRT(NULL != tp);

  status  = sys$qiow(0, tp->tt_chan, IO$_SENSEMODE, &orgttiosb,
                     0, 0, &oldchar, SIZEOF(oldchar), 0, 0, 0, 0);
  if ( 0 == (status & 1) )  {
    TRC(("status = %d", status));

    return oldchar;
  }

  newchar = oldchar;
  newchar.tt2 &= ~TT2$M_PASTHRU;  /* Switch on ^U, ^X, ^C, and ^Y processing  */

  status = sys$qiow(0, tp->tt_chan, IO$_SETMODE, 0,
                    0, 0, &newchar, SIZEOF(newchar), 0, 0, 0, 0);
  if ( 0 == (status & 1) )  {
    TRC(("status = %d", status));
  }

  return oldchar;
}

static ttchar_t TerminalRestore(const terminal_t *tp, const ttchar_t *ttcharp)
{
  int           status  = 0;
  ttchar_t      oldchar;
  ttchar_t      newchar;
  ttchariosb_t  orgttiosb;

  ZEROMEM(oldchar);
  ZEROMEM(newchar);
  ZEROMEM(orgttiosb);

  ASRT(NULL != tp);
  ASRT(NULL != ttcharp);

  status  = sys$qiow(0, tp->tt_chan, IO$_SENSEMODE, &orgttiosb,
                     0, 0, &oldchar, SIZEOF(oldchar), 0, 0, 0, 0);
  if ( 0 == (status & 1) )  {
    TRC(("status = %d", status));

    return oldchar;
  }

  newchar = *ttcharp;

  status = sys$qiow(0, tp->tt_chan, IO$_SETMODE, 0,
                    0, 0, &newchar, SIZEOF(newchar), 0, 0, 0, 0);
  if ( 0 == (status & 1) )  {
    TRC(("status = %d", status));
  }

  return oldchar;
}

static terminal_t *TerminalOpen(void)
{
  xile3_t     dvi_list[2] = {0,0,0,0,0,0,0,0};
  int         status      = 0;
  terminal_t  *tp         = NULL;
  /* $DESCRIPTOR()-Macro creates a structure definition from a string literal */
  /* SYS$COMMAND: A logical, similar to UNIX's `/dev/tty' */
  $DESCRIPTOR(tt_desc, "SYS$COMMAND");
  xiosb_t     iosb;

  ZEROMEM(iosb);

  ALLOCATE(tp);
  TRC(("tp = 0x%08lX", (ulong_t)tp));
  /* Setup GETDVI item list:  */
  dvi_list[0].buf_size  = SIZEOF (tp->dev_class);
  dvi_list[0].item_code = DVI$_DEVCLASS;
  dvi_list[0].buf_addr  = &tp->dev_class;

  /* Get device type for SYS$COMMAND: */
  status = sys$getdviw (0, 0, &tt_desc, &dvi_list, &iosb, 0, 0, 0);
  if ( status & 1 ) {
    status = iosb.status;
  }
  if ( status != SS$_NORMAL ) {
    TRC(("status = %d, SS$_NORMAL = %d", (int)status, (int)SS$_NORMAL));
    FREE(tp);

    return NULL;
  }

  /* Assign channel to the terminal:  */
  status = sys$assign(&tt_desc, &tp->tt_chan, 0, 0);
  if (! (status & 1) )  {
    TRC(("status = %d", (int)status));
    FREE(tp);

    return NULL;
  }

  return tp;
}

static int  TerminalClose(const terminal_t *tp)
{
  int status  = 0;

  if ( NULL == tp ) {
    return SUCCESS;
  }

  /* Deassign channel from terminal:  */
  status = sys$dassgn (&tp->tt_chan);
  FREE(tp);

  if ( !(status & 1) )  {
    return ERRRET;
  } else                {
    return SUCCESS;
  }
}

/* Return number of bytes read  */
static int  TerminalRead(const terminal_t *tp, char *buf, int bufsiz, int read_echo)
{
#define TT_READ_BUFSZ (1024)
  int                     rc            = 0;
  int                     read_flags    = 0;
  int                     status        = 0;
  xiosb_t                 iosb;
  const int               noterm[]      = {0, 0}; /* Terminator list of NONE */
  char                    tmp_buf[TT_READ_BUFSZ];

  ZEROMEM(iosb);
  ZEROMEM(tmp_buf);

  ASRT(NULL != tp);
  ASRT(0 >= bufsiz || NULL != buf);

  if ( 0 >= bufsiz )                      {
    TRC(("bufsiz = %d", (int)bufsiz));

    return 0;
  } else if ( SIZEOF(tmp_buf) < bufsiz )  {
    TRC(("SIZEOF(tmp_buf) =  %d, bufsiz = %d", SIZEOF(tmp_buf), (int)bufsiz));

    return ERRRET;
  }

  /* Determine if the input device is a terminal: */
  if ( tp->dev_class == DC$_TERM )  {
    if ( !read_echo ) {
      read_flags  = IO$_READVBLK | IO$M_NOECHO | IO$M_TRMNOECHO | IO$M_NOFILTR;
    } else            {
      read_flags  = IO$_READVBLK                                | IO$M_NOFILTR;
    }

    /*  Read from the terminal: */
#if ( !READ_WITH_TOUT )
    status = sys$qiow (0, tp->tt_chan, read_flags, &iosb, 0, 0,
                       tmp_buf,
                       bufsiz,
                       0, noterm, 0, 0);
    if ( status != SS$_NORMAL && !(iosb.status & 1) )  {
      TRC(("status = %d, SS$_NORMAL = %d, iosb.status = %d",
           (int)status, (int)SS$_NORMAL, (int)iosb.status));
      rc  = 0;
    } else                                                    {
      rc = iosb.nwritten;
      TRC(("rc = %d", rc));
    }
#else
# define TOUT_S (0) /* Read timeout in seconds  */
again:
    status = sys$qiow (0, tp->tt_chan, read_flags | IO$M_TIMED, &iosb, 0, 0,
                       tmp_buf,
                       bufsiz,
                       TOUT_S, noterm, 0, 0);
    if ( status != SS$_NORMAL && !(iosb.status & 1) )  {
      TRC(("status = %d, SS$_NORMAL = %d, iosb.status = %d",
           (int)status, (int)SS$_NORMAL, (int)iosb.status));
      rc  = 0;
    } else                                                    {
      rc = iosb.nwritten;
      if ( 0 == rc )  {
        microsleep(READ_WAIT_MS * 1000);
        goto again;
      }
      TRC(("rc = %d", rc));
    }
#endif
  } else                        { /* Not a terminal:  */
    struct dsc$descriptor_s InputDesc = { 0, DSC$K_DTYPE_T,
                                            DSC$K_CLASS_S, 0
                                         };
    ushort_t                InputLen  = 0;

    InputDesc.dsc$w_length  = bufsiz;
    InputDesc.dsc$a_pointer = tmp_buf;
    status = lib$get_command (&InputDesc, 0, &InputLen);
    if ( status != SS$_NORMAL ) {
      TRC(("status = %d, SS$_NORMAL = %d", (int)status, (int)SS$_NORMAL));
      rc  = ERRRET;
    } else                      {
      rc  = InputLen;
    }
  }
  if ( 0 < rc ) {
    tmp_buf[rc] = '\0';
    strcpy (buf, tmp_buf);
  }
  TRC(("Returning: rc = %d", rc));

  return rc;
#undef  TT_READ_BUFSZ
}

static void microsleep(int us)
{
  struct timeval  timeout;

  ZEROMEM(timeout);
  ASRT(0 <= us);

  timeout.tv_sec  = 0;
  timeout.tv_usec = us;
  select(0, NULL, NULL, NULL, &timeout);

  return;
}


int main_A(int argc, char *argv[])
{
  terminal_t  *tp = NULL;
  ttchar_t    orgttchar;
  char        c   = 0;

  ZEROMEM(orgttchar);

  tp  = TerminalOpen();
  ASRT(NULL != tp);
  orgttchar = TerminalRaw(tp);
  while ( 0 < TerminalRead(tp, &c, 1, 0) ) {
    /**TerminalCooked(tp);**/
    TerminalRestore(tp, &orgttchar);
    fprintf(stdout, ">%d<\n", (int)(unsigned char)c); fflush(stdout);
    if ( 'q' == c ) {
      fprintf(stdout, "\n%s\n", "QUIT"); fflush(stdout);
      break;
    }
    TerminalRaw(tp);
  }
  /**TerminalCooked(tp);**/
  TerminalRestore(tp, &orgttchar);
  TerminalClose(tp);

  return 0;
}


#if ( TEST_MODE )
int main(int argc, char *argv[])
{
  char        c   = 0;

  while ( 0 < TerminalReadVMS(&c, 1, 0) ) {
    fprintf(stdout, ">%d<\n", (int)(unsigned char)c); fflush(stdout);
    if ( 'q' == c ) {
      fprintf(stdout, "\n%s\n", "QUIT"); fflush(stdout);
      break;
    }
  }

  return 0;
}
#endif


/*======================================================================
 *  Restore the users pointer context
 *====================================================================*/
#ifdef  __INITIAL_POINTER_SIZE
# if    __INITIAL_POINTER_SIZE
#  pragma __pointer_size __restore
# endif
#endif
/*====================================================================*/



/* EOF(Terminal_VMS.c)  */
