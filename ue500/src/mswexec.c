/*======================================================================
 * The routines in this file provide support for external program
 * execution under the Microsoft Windows environment on an IBM-PC or
 * compatible computer.
 *
 * Must be compiled with Borland C++ 2.0 or MSC 6.0 or later versions
 *
 * It should not be compiled if the WINDOW_MSWIN symbol is not set
 *====================================================================*/

/*====================================================================*/
#define MSWEXEC_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include    "estruct.h"
#include    <stdio.h>
#include    "eproto.h"
#include    "edef.h"
#include    "elang.h"

#include    "mswin.h"

#define CMDLENGTH   256+NFILEN

/* local data */
#if WINDOW_MSWIN32
static STARTUPINFO suInfo = { 0 };
static char suTitle [] = PROGNAME "'s subshell";
static PROCESS_INFORMATION pInfo = { 0 };
#else
static HWND hPrgWnd;    /* window handle of the external program task */
#endif


/* HandleTimer: checks the existence of the external program window */
/* ===========                                                      */

static VOID PASCAL  HandleTimer (HWND hDlg)
/* This function uses a 200ms timeout to check the existence of the window
 * indicated by hPrgWnd. When that window handle becomes invalid, it is assumed
 * that the external program has exited and the WAITFORPRG dialog box is
 * terminated with a TRUE result. If there is a problem setting the timer, the
 * dialog box is terminated with a FALSE result after a message box announcing
 * the problem has been displayed. */
{
#if WINDOW_MSWIN32
    /* check for process completion */
    if ( WaitForSingleObject(pInfo.hProcess, 0) == WAIT_TIMEOUT ) {
#else
    /* look for the program window */
    if ( IsWindow (hPrgWnd) ) {
#endif
        while ( !SetTimer (hDlg, 1, 200, NULL) ) {
            /* bad: problem setting the timer */
            if ( MessageBox (hDlg, TEXT337, NULL,
                             MB_RETRYCANCEL) == IDCANCEL ) {
                /* "cannot monitor external program" */
                EndDialog (hDlg, FALSE);    /* give up! */

                return;
            }
            /* else: attempt a retry */
        }
    } else {
        EndDialog (hDlg, TRUE);
    }
} /* HandleTimer */

/* WAITFORPRGDlgProc:   dialog proc for WAITFORPRG dialog box */
/* =================                                          */
int EXPORT FAR PASCAL  WAITFORPRGDlgProc (HWND   hDlg,
                                          UINT   wMsg,
                                          WPARAM wParam,
                                          LPARAM lParam)
{
    switch ( wMsg ) {
    case WM_INITDIALOG:
        SetWindowText (hDlg, PROGNAME);
        HandleTimer (hDlg);

        return TRUE;

    case WM_TIMER:
        HandleTimer (hDlg);
        break;

    case WM_COMMAND:
        if ( LOWORD(wParam) == 2 ) {  /* Cancel */
            KillTimer (hDlg, 1);
            EndDialog (hDlg, FALSE);
        }
        break;

    default:
        return FALSE;
    }

    return FALSE;
} /* WAITFORPRGDlgProc */

/* LaunchPrgEnumProc:   used by LaunchPrg */
/* =================                      */
BOOL EXPORT FAR PASCAL LaunchPrgEnumProc (HWND hWnd, LPARAM lParam)
/* this function sets hPrgWnd when it finds a window that matches the module
 * instance handle passed in lParam */
{
#if !WINDOW_MSWIN32
    if ( GetWindowWord (hWnd, GWW_HINSTANCE) == LOWORD(lParam) ) {
        hPrgWnd = hWnd;

        return FALSE;   /* found it, stop enumerating */
    }
#endif

    return TRUE;
} /* LaunchPrgEnumProc */

/* LaunchPrg:   launches and monitors an external program  */
/* =========                                               */

static BOOL PASCAL  LaunchPrg (char *Cmd,
                               BOOL DOSApp,
                               char *InFile,
                               char *OutFile)
/*
 * Returns TRUE if all went well, FALSE if wait cancelled and FAILD if
 * failed to launch.
 *
 * Cmd is the command string to launch.
 *
 * DOSApp is TRUE if the external program is a DOS program to be run
 * under a DOS shell. If DOSApp is FALSE, the program is launched
 * directly as a Windows application. In that case, the InFile parameter
 * is ignored, and the value of the OutFile parameter is used only to
 * determine if the program should be monitored. the text of the string
 * referenced by OutFile is irrelevant.
 *
 * InFile is the name of the file to pipe into stdin (if NULL, nothing
 * is piped in)
 *
 * OutFile is the name of the file where stdout is expected to be
 * redirected. If it is NULL or an empty string, stdout is not redirected
 *
 * If Outfile is NULL, LaunchPrg returns immediately after starting the
 * DOS box.
 *
 * If OutFile is not NULL, the external program is monitored.
 * LaunchPrg returns only when the external program has terminated or
 * the user has cancelled the wait (in which case LaunchPrg returns
 * FALSE).
 */
{
    char FullCmd[CMDLENGTH];
    HANDLE hModule;
    int nCmdShow;
    BOOL Synchronize;
#if !WINDOW_MSWIN32
    FARPROC ProcInstance;
#endif

    if ( OutFile ) {
        Synchronize = TRUE;
        if ( *OutFile == '\0' ) OutFile = NULL; /* stop worrying about that
                                                 * empty string */
    } else Synchronize = FALSE;

    if ( SetWorkingDir () != 0 ) {
        mlwrite (TEXT334);  /* "[No such directory]" */

        return FALSE;
    }
    if ( DOSApp ) {
#if WINDOW_MSWIN32
        GetPrivateProfileString (ProgName,
                                 "Shell",
                                 "cmd.exe",
                                 FullCmd,
                                 CMDLENGTH,
                                 IniFile);

        /* the Shell profile string should contain the name of the shell to be
         * used for pipe-command, filter buffer, shell-command and i-shell. The
         * ShellExecOption profile string should contain the option flags that
         * cause the execution under that shell of a single command (the text of
         * which is appended to the profile string) so that the shell terminates
         * with that command. */
        if ( Cmd ) {
            char ExecOption[10];

            GetPrivateProfileString (ProgName,
                                     "ShellExecOption",
                                     " /c ",
                                     ExecOption,
                                     10,
                                     IniFile);

            if ( ( STRLEN (FullCmd) + STRLEN (ExecOption) + STRLEN (Cmd) ) >=
                 CMDLENGTH ) return FALSE;

            xstrcat (FullCmd, ExecOption);
            xstrcat (FullCmd, Cmd);
        }
#else
        if ( Synchronize || !Cmd ) {
            GetPrivateProfileString (ProgName,
                                     "DOSExec",
                                     "",
                                     FullCmd,
                                     CMDLENGTH,
                                     IniFile);

            if ( FullCmd[0] == '\0' ) {   /* try to find it on the "path" */
                char    *s;

                if ( ( s = flook ("DOSEXEC.PIF", TRUE, TRUE) ) != NULL )  {
                    xstrcpy (FullCmd, s);
                }
            }
        } else FullCmd[0] = '\0';
        if ( FullCmd[0] == '\0' ) {
            GetPrivateProfileString (ProgName,
                                     "DOSBox",
                                     "",
                                     FullCmd,
                                     CMDLENGTH,
                                     IniFile);

            if ( FullCmd[0] == '\0' ) {   /* try to find it on the "path" */
                char    *s;

                if ( ( s = flook ("DOSBOX.PIF", TRUE, TRUE) ) != NULL ) {
                    xstrcpy (FullCmd, s);
                } else xstrcpy (FullCmd, "command.com");
            }
        }
        /* the DOSBox profileString should be the name of a PIF file for
         * command.com that specifies no arguments and no starting dir. The
         * DOSExec should be similar but specify "Close window on exit" so that
         * synchronization can work */
        if ( Cmd ) {
            if ( (STRLEN (FullCmd) + STRLEN (Cmd) + 4) >=
                 CMDLENGTH ) return FALSE;

            xstrcat (FullCmd, " /c ");
            xstrcat (FullCmd, Cmd);
        }
#endif
        if ( InFile ) {
            if ( (STRLEN (FullCmd) + STRLEN (InFile) + 2) >=
                 CMDLENGTH ) return FALSE;

            xstrcat (FullCmd, " <");
            xstrcat (FullCmd, InFile);
        }
        if ( OutFile ) {
            if ( (STRLEN (FullCmd) + STRLEN (OutFile) + 2) >=
                 CMDLENGTH ) return FALSE;

            xstrcat (FullCmd, " >");
            xstrcat (FullCmd, OutFile);
        }
    }
#if WINDOW_MSWIN32
    /* set the startup window size */
    suInfo.cb = SIZEOF (STARTUPINFO);
    if ( DOSApp && Cmd ) suInfo.lpTitle = suTitle;
    suInfo.wShowWindow =
        (DOSApp && Synchronize) ? SW_SHOWMINIMIZED :SW_SHOWNORMAL;
    suInfo.dwFlags     = STARTF_USESHOWWINDOW;

    /* start the process and get a handle on it */
    if ( CreateProcess (NULL, DOSApp ? FullCmd : Cmd, NULL, NULL,
                        DETACHED_PROCESS, FALSE, NULL, NULL, &suInfo,
                        &pInfo) ) {
        int Result;

        if ( Synchronize ) {
            /* put up a dialog box to wait for termination */
            Result = DialogBox (hEmacsInstance,
                                "WAITFORPRG",
                                hFrameWnd,
                                WAITFORPRGDlgProc);
        } else {
            /* no need to synchronize */
            Result = TRUE;
        }
        CloseHandle(pInfo.hThread);
        CloseHandle(pInfo.hProcess);

        return Result;
    } else return FALSE;

#else
    if ( Win386Enhanced ) {
        if ( DOSApp && Synchronize ) nCmdShow = SW_SHOWMINIMIZED;
        else nCmdShow = SW_SHOWNORMAL;
    } else nCmdShow = SW_SHOWNORMAL;

    hModule = WinExec (DOSApp ? FullCmd : Cmd, nCmdShow);
    /* here we GOoooo */

    if ( hModule < 32 ) {
        mlwrite (TEXT3);    /* "[Execution failed]" */

        return FAILD;
    }
    if ( !Synchronize ) return TRUE; /* no synchronization */

    hPrgWnd = 0;
    ProcInstance =
        MakeProcInstance ( (FARPROC)LaunchPrgEnumProc, hEmacsInstance );
    EnumWindows (ProcInstance, (DWORD)hModule);
    FreeProcInstance (ProcInstance);
    if ( hPrgWnd != 0 ) {
        /*-put up a dialog box to wait for the external program termination */
        int Result;

        ProcInstance = MakeProcInstance ( (FARPROC)WAITFORPRGDlgProc,
                                          hEmacsInstance );
        Result = DialogBox (hEmacsInstance,
                            "WAITFORPRG",
                            hFrameWnd,
                            ProcInstance);
        FreeProcInstance (ProcInstance);

        return Result;
    } else return TRUE; /* we assume it has zipped past us! */

#endif
} /* LaunchPrg */

/* spawncli:    launch DOS shell. Bound to ^X-C */
/* ========                                     */

PASCAL spawncli (int f, int n)
{
    /*-don't allow this command if restricted */
    if ( restflag ) return resterr();

    return LaunchPrg (NULL, TRUE, NULL, NULL);
} /* spawncli */

/* spawn:   run a one-liner in a DOS box. Bound to ^X-! */
/* =====                                                */

PASCAL spawn (int f, int n)
{
    char Line[NLINE];
    int Result;
    char    *SynchOption;
    static char empty[] = "";

    /*-don't allow this command if restricted */
    if ( restflag ) return resterr();

    if ( ( Result = mlreply ("!", Line, NLINE) ) != TRUE ) return Result;

    if ( f ) SynchOption = &empty[0];
    else SynchOption = NULL;

    return LaunchPrg (Line, TRUE, NULL, SynchOption);
} /* spawn */

/* execprg: run another program with arguments. Bound to ^X-$ */
/* =======                                                    */

PASCAL execprg (int f, int n)
{
    char Line[NLINE];
    int Result;
    char    *SynchOption;

    /*-don't allow this command if restricted */
    if ( restflag ) return resterr();

    /*-get the program command line */
    if ( mlreply ("$", Line, NLINE) != TRUE ) return FALSE;

    if ( f ) SynchOption = &Line[0]; /* any not NULL will do */
    else SynchOption = NULL;
    Result = LaunchPrg (Line, FALSE, NULL, SynchOption);
    if ( Result == FAILD ) {
        mlwrite (TEXT3);    /* "[Execution failed]" */
    }

    return Result;
} /* execprg */

/* pipecmd: pipe a one-liner into a window. Bound to ^X-@ */
/* =======                                                */

PASCAL pipecmd (int f, int n)
/* this function fills a buffer named "command" with the output of the DOS
 * one-liner. If the command buffer already exist, it is overwritten only if it
 * has not been changed */
{
    char Line[NLINE];
    char OutFile[NFILEN];
    static char bname[] = "command";
    BUFFER  *bp;
    EWINDOW  *wp;
    int Result;
    int bmode;
    char bflag;
#if WINDOW_MSWIN32
    char TempDir[NFILEN] = "\\";
#endif

    /*-don't allow this command if restricted */
    if ( restflag ) return resterr();

    /*-get the command to pipe-in */
    if ( mlreply ("@", Line, NLINE) != TRUE ) return FALSE;

    /*-find the "command" buffer */
    if ( ( bp = bfind (bname, FALSE, 0) ) != NULL ) {
        /*-make sure the contents can safely be blown away */
        if ( bp->b_flag & BFCHG ) {
            if ( mlyesno (TEXT32) != TRUE ) return FALSE;
            /* discard changes */
        }
    } else if ( ( bp = bfind (bname, TRUE, 0) ) == NULL ) {
        mlwrite (TEXT137);

        /* cannot create buffer */
        return FALSE;
    }
#if WINDOW_MSWIN32
    GetTempPath (NFILEN, TempDir);
    GetTempFileName (TempDir, "UE", 0, OutFile);
#else
    GetTempFileName (0, "UE", 0, OutFile);
#endif
    Result = LaunchPrg (Line, TRUE, NULL, OutFile);
    if ( Result == FAILD ) {
        mlwrite (TEXT3);
        /* [execution failed] */
        umc_unlink (OutFile);
    } else {
        if ( Result == TRUE ) {
            BUFFER  *temp_bp;

            temp_bp = curbp;
            swbuffer (bp);          /* make this buffer the current one */
            bmode = bp->b_mode;
            bp->b_mode &= ~MDVIEW;
            bflag = bp->b_flag;
            bp->b_flag &= ~BFCHG;
            Result = readin (OutFile, FALSE);
            bp->b_fname[0] = '\0';  /* clear file name */
            if ( Result == TRUE ) {
                bp->b_mode |= MDVIEW;   /* force VIEW mode */
                lchange (WFMODE);       /* update all relevant mode lines */
                bp->b_flag &= ~BFCHG;   /* remove by-product BFCHG flag */
            } else {
                bp->b_mode = bmode;     /* restore mode */
                bp->b_flag = bflag;
                swbuffer (temp_bp);
            }
            umc_unlink (OutFile);
            /* note that the file is not deleted if the wait was cancelled */
        }
    }

    return Result;
} /* pipecmd */

/* f_filter:  filter a buffer through a DOS box. Bound to ^X-# */
/* ========                                                    */

PASCAL f_filter (int f, int n)
{
    char Line[NLINE];
    char InFile[NFILEN];
    char OutFile[NFILEN];
    char fname[NFILEN];
    BUFFER  *bp;
    EWINDOW  *wp;
    int Result;
#if WINDOW_MSWIN32
    char TempDir[NFILEN] = "\\";
#endif

    /*-don't allow this command if restricted */
    if ( restflag ) return resterr();

    /*-get the filter command line */
    if ( mlreply ("#", Line, NLINE) != TRUE ) return FALSE;

    bp = curbp;
    xstrcpy (fname, bp->b_fname);
#if WINDOW_MSWIN32
    GetTempPath (NFILEN, TempDir);
    GetTempFileName (TempDir, "UE", 0, InFile);
#else
    GetTempFileName (0, "UE", 0, InFile);
#endif
    Result = writeout (InFile, "w");
    if ( Result != TRUE ) {
        mlwrite (TEXT2);
        /* cannot write filter file */
    } else {
#if WINDOW_MSWIN32
        GetTempFileName (TempDir, "UE", 0, OutFile);
#else
        GetTempFileName (0, "UE", 0, OutFile);
#endif
        Result = LaunchPrg (Line, TRUE, InFile, OutFile);
        if ( Result == FAILD ) {
            mlwrite (TEXT3);
            /* [execution failed] */
            umc_unlink (OutFile);
            umc_unlink (InFile);
        } else {
            if ( Result == TRUE ) {
                Result = readin (OutFile, FALSE);
                umc_unlink (OutFile);
                umc_unlink (InFile);
            }
            /* note that he files are not deleted if the wait was cancelled */
            if ( Result == TRUE ) {
                lchange (WFMODE);   /* update all relevant mode lines */
            }
        }
    }
    xstrcpy (bp->b_fname, fname);    /* restore original file name */

    return Result;
} /* f_filter */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
