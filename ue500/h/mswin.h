/*======================================================================
 * MSWIN: This file provides definitions for the C source and resource
 *        scripts for support of operation under the Microsoft Windows
 *        environment on an IBM-PC or compatible computer.
 *
 *        Must be compiled with Borland C++ 2.0 or MSC 6.0 or later
 *        versions
 *
 *        It should not be compiled if the WINDOW_MSWIN symbol is not
 *        set
 *====================================================================*/

/*====================================================================*/
#ifndef MSWIN_H_
#define MSWIN_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/**********************************************************************/
#include "estruct.h"
/**********************************************************************/


/* compile flags */
#define GRINDERS    8   /* 0 to use the hourglass, 8 to use the animated grinder
                         * (8 is the number of animation steps) */

#define MEMTRACE    1   /* adds a debugging trace to mswmem.c (used only if
                         * SUBALLOC (below) is 1) */

/*
 * The SUBALLOC #define implements malloc/free/realloc by segment suballocation
 * routines (needed for Borland C 2.0, but not necessary under Borland C 3.0).
 * Not needed under MSC 7.0 either.
 */
#if WINDOW_MSWIN32
# define SUBALLOC    0
#else
# if TURBO
#  if defined(__BORLANDC__) && (__TURBOC__<0x300)
#   define SUBALLOC   1   /* BC++ 2.0             */
#  else
#   define SUBALLOC   0   /* TC++ 1.0 or BC++ 3.x */
#  endif  /* defined(__BORLANDC__) && (__TURBOC__<0x300) */
# endif /* TURBO */

# if MSC
#  if _MSC_VER < 700
#   define SUBALLOC   1
#  else
#   define SUBALLOC   0
#  endif  /* _MSC_VER < 700 */
# endif /* MSC */

#endif  /* WINDOW_MSWIN32 */


#include  <shellapi.h>
#include  "mswrid.h"    /* contains all the Resource IDs */


#if WINDOW_MSWIN32
# define LPDATA(p) ( (LPARAM)(LPSTR)(p) )
# define HDROP HANDLE
# define NOTIFICATION_CODE HIWORD(wParam)   /* for WM_COMMAND parsing */
#else
# define LPDATA(p) ( (LPSTR)(p) )
# define NOTIFICATION_CODE HIWORD(lParam)
# define WNDPROC FARPROC
#endif

/* macros */
#define MLSIZE  NSTRING   /* message line buffer size */

/* offsets for the screen windows extra bytes */
#define GWL_SCRPTR  0   /* SCREEN_T structure pointer */
#if WINXP
# define GWW_SCRCX  ( GWL_SCRPTR+sizeof (LONG_PTR) ) /* client area width   */
#else
# define GWW_SCRCX  ( GWL_SCRPTR+sizeof (LONG) )     /* client area width   */
#endif
#define GWW_SCRCY   ( GWW_SCRCX+sizeof (WORD) )      /* client area height  */
#define SCRWNDEXTRA ( GWW_SCRCY+sizeof (WORD) )

/* offsets for the frame window extra bytes */
#define GWW_FRMID 0   /* Id seen by other emacs apps */
#define FRMWNDEXTRA ( GWW_FRMID+sizeof (WORD) )

/* structures */
typedef struct  CellMetrics { /* coordinate-related parameters of a font */
    short OffsetX, OffsetY;       /* offset of column 0 or row 0, relative to the
                                   * client area upper left corner  */
    short LeadingY, HalfLeadingY; /* external leading between rows  */
    short SizeX, SizeY;           /* character cell size            */
    short MLHeight;               /* height of the message line     */
} CellMetrics;


#ifdef  termdef   /* in mswdrv.c only */

char IniFile[]        = "EMACS.INI";
char ProgName []      = PROGNAME;
int CurrentRow        = 0;
int CurrentCol        = 0;
BOOL MouseTracking    = FALSE;
BOOL InternalRequest  = FALSE;
BOOL TakingANap       = FALSE;
int caret_shape       = 0;

/* Global uninitialized variables */
HWND        hFrameWnd;                /* main (frame) window          */
HWND        hMDIClientWnd;            /* MDI client window            */
HANDLE      hEmacsInstance;           /* module instance              */
BOOL        Win386Enhanced;           /* Windows 386 enhanced mode    */
BOOL        Win31API;                 /* Windows 3.1 or later version */
HCURSOR     hScreenCursor, hTrackCursor, hNotQuiescentCursor, hHourglass;
char        *ScreenClassName;         /* MDI childs class name        */
char        *MLBuf;                   /* message line buffer          */
HFONT       hEmacsFont;               /* font used for display        */
CellMetrics EmacsCM;                  /* cell metrics of that font    */
# if GRINDERS != 0
HCURSOR     GrinderCursor[GRINDERS];  /* handles for the grinder animation      */
int         GrinderIndex;
# endif
char        *MainHelpFile;            /* Emac's help file name and useage flag  */
BOOL        MainHelpUsed;
char        HelpEngineFile[NFILEN];   /* user help file and useage flag         */
BOOL        ColorDisplay;             /* TRUE if the display is color-capable   */

#else/**#ifdef  termdef**/

extern char IniFile[DUMMYSZ];
extern char ProgName[DUMMYSZ];  /* used all over the place for captions, etc... */
extern int  CurrentRow;
extern int  CurrentCol;         /* caret positions (in text coordinates)        */
extern BOOL MouseTracking;      /* TRUE if mouse in tracking/dragging mode      */
extern BOOL InternalRequest;
/*
 * TRUE for a request that originates from the mswxxx modules. This is meant to
 * avoid infinite recursions for requests that could come from both MS-Windows
 * or the editor's core (for instance resizing, screen activation...)
 */
extern BOOL TakingANap;     /* TRUE during execution of TakeANap()      */
extern int  caret_shape;    /* 0 = HorzBar, 1 = VertBar, 2 = CellSize   */

/* Global uninitialized variables */
extern HWND         hFrameWnd;          /* main (frame) window          */
extern HWND         hMDIClientWnd;      /* MDI client window            */
extern HANDLE       hEmacsInstance;     /* module instance              */
extern BOOL         Win386Enhanced;     /* Windows 386 enhanced mode    */
extern BOOL         Win31API;           /* Windows 3.1 or later version */
extern HCURSOR      hScreenCursor, hTrackCursor, hNotQuiescentCursor, hHourglass;
extern char         *ScreenClassName;   /* MDI childs class name        */
extern char         *MLBuf;             /* message line buffer          */
extern HFONT        hEmacsFont;         /* font used for display        */
extern CellMetrics  EmacsCM;            /* cell metrics of that font    */
# if GRINDERS != 0
extern HCURSOR      GrinderCursor[GRINDERS];  /* handles for the grinder animation      */
extern int          GrinderIndex;
# endif
extern char         *MainHelpFile;            /* Emac's help file name and useage flag  */
extern BOOL         MainHelpUsed;

extern char         HelpEngineFile[NFILEN];   /* user help file and useage flag         */
extern BOOL         ColorDisplay;             /* TRUE if the display is color-capable   */

#endif/**#ifdef  termdef**/


/* input stream access functions */
VOID  in_init (VOID);
BOOL  in_room (int n);
BOOL  in_check (VOID);
VOID  in_put (int event);
int   in_get (VOID);

/* Windows-implementation specific functions */
LONG EXPORT FAR PASCAL ScrWndProc (HWND   hWnd,
                                   UINT   wMsg,
                                   WPARAM wParam,
                                   LPARAM lParam);
LONG EXPORT FAR PASCAL FrameWndProc (HWND   hWnd,
                                     UINT   wMsg,
                                     WPARAM wParam,
                                     LPARAM lParam);
int FAR PASCAL GetInput (VOID);
int FAR PASCAL TakeANap (int t);

VOID FAR PASCAL GenerateMenuSeq (UINT ID);
KEYTAB * FAR PASCAL FindKeyBinding (VOIDP Func);
VOID FAR PASCAL InitMenuPopup (HMENU hMenu, LPARAM lParam);
BOOL FAR PASCAL MenuCommand (WPARAM wParam, LPARAM lParam);
HMENU FAR PASCAL GetScreenMenuHandle (VOID);

BOOL FAR PASCAL EatKey (UINT MsgCode, WPARAM wParam, LPARAM lParam);
VOID FAR PASCAL MouseMessage (HWND hWnd, UINT wMsg, WPARAM wParam,
                              LPARAM lParam);
VOID FAR PASCAL DropMessage (HWND hWnd, HANDLE hDrop);

VOID FAR PASCAL BuildCellMetrics (CellMetrics *cm, HFONT hFont);
VOID FAR PASCAL InvalidateCells (HWND hWnd,
                                 int  leftcol,
                                 int  toprow,
                                 int  rightcol,
                                 int  bottomrow);
VOID FAR PASCAL MinimumClientSize (HWND hWnd,
                                   int  NCols,
                                   int  NRows,
                                   int  *Width,
                                   int  *Height);
int FAR PASCAL DisplayableRows (HWND hWnd, int Height, CellMetrics *cm);
int FAR PASCAL DisplayableColumns (HWND hWnd, int Width, CellMetrics *cm);
VOID FAR PASCAL EmacsCaret (BOOL Show);
VOID FAR PASCAL MoveEmacsCaret (HWND hWnd, int col, int row);
VOID FAR PASCAL ShowEmacsCaret (BOOL Show);
VOID FAR PASCAL CellToClient (HWND hWnd, POINT Cell, LPPOINT Client);
VOID FAR PASCAL ClientToCell (HWND hWnd, POINT Client, LPPOINT Cell);
VOID FAR PASCAL GetMinMaxInfo (HWND hWnd, LPPOINT rgpt);
BOOL FAR PASCAL ScrReSize (HWND hWnd, WPARAM wParam, WORD cx, WORD cy);
VOID FAR PASCAL ScrPaint (HWND hWnd);
VOID FAR PASCAL MLPaint (VOID);

BOOL FAR PASCAL InMessageLine (VOID);

VOID FAR PASCAL ClipboardCleanup (VOID);
VOID FAR PASCAL ScrollMessage (HWND hWnd, UINT wMsg, WORD ScrlCode, int Pos);
VOID FAR PASCAL ScrollBars (VOID);

int FAR PASCAL SetWorkingDir (VOID);

VOID FAR PASCAL InitializeFarStorage (VOID);
VOID FAR PASCAL JettisonFarStorage (VOID);

HFONT FAR PASCAL SelectFont (HDC hDC, HFONT hFont);
BOOL FAR PASCAL PickEmacsFont (VOID);
VOID FAR PASCAL FontInit (VOID);



/*====================================================================*/
#endif/**#ifndef MSWIN_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
