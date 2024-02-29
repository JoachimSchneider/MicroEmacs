#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>


#define CONST const
#define SIZEOF(x)   ( (int)sizeof((x)) )

#define ZEROMEM(x)  ( memset(&(x), 0, SIZEOF((x))) )


extern int xsystem(CONST char *shell, CONST char *shell_flag, CONST char *cmdstring);


int main(int argc, char *argv[])
{
    int rc  = 0;
    
    rc  = xsystem("C:\\Windows\\system32\\cmd.exe", "/c", argv[1]);

    return rc;
}
 


/* See
 * W. Richard Stevens:  Advanced Programming in the UNIX Environment. Reading, MA, 1992.
 *                      Program 10.20
 */
int xsystem(CONST char *shell, CONST char *shell_flag, CONST char *cmdstring)
{
    pid_t             pid     = 0;
    int               status  = 0;
    CONST char        *arg0   = NULL;
    int               l       = 0;
    struct sigaction  ignore;
    struct sigaction  saveintr;
    struct sigaction  savequit;
    sigset_t          chldmask;
    sigset_t          savemask;

    ZEROMEM(ignore);
    ZEROMEM(saveintr);
    ZEROMEM(savequit);
    ZEROMEM(chldmask);
    ZEROMEM(savemask);
    
    if ( NULL == cmdstring )  {
        if ( NULL == shell )  {
            return ( 0 );
        } else                {
            return ( !0 );
        }
    }
    for ( l = strlen(shell) - 1; l >= 0; l-- )  {
        if ( '/' == shell[l] || '\\' == shell[l] )  {
            l++;
            
            break;
        }
    }
    arg0  = shell + l;

    ignore.sa_handler = SIG_IGN;        /* ignore SIGINT and SIGQUIT  */
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    if ( sigaction(SIGINT, &ignore, &saveintr) < 0 )  {
        return ( -1 );
    }
    if ( sigaction(SIGQUIT, &ignore, &savequit) < 0 ) {
        return ( -1 );
    }

    sigemptyset(&chldmask);             /* now block SIGCHLD  */
    sigaddset(&chldmask, SIGCHLD);
    if ( sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0 ) {
        return(-1);
    }

    if        ( (pid = fork()) < 0) {
        status  = -1;                   /* probably out of processes  */
    } else if ( pid == 0 )          {   /* child  */
        /* restore previous signal actions & reset signal mask  */
        sigaction(SIGINT, &saveintr, NULL);
        sigaction(SIGQUIT, &savequit, NULL);
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        if ( NULL != shell_flag ) {
            execl(shell, arg0, shell_flag, cmdstring, (char *) 0);
        } else                    {
            execl(shell, arg0, cmdstring, (char *) 0);
        }
        
        _exit(127);                     /* exec error */
    } else                          {   /* parent */
        while ( waitpid(pid, &status, 0) < 0 )  {
            if ( errno != EINTR)  {
                status  = -1;           /* error other than EINTR from
                                         * waitpid()  */
                                         
                break;
            }
        }
    }

    /* restore previous signal actions & reset signal mask  */
    if ( sigaction(SIGINT, &saveintr, NULL) < 0 )         {
        return (-1);
    }
    if ( sigaction(SIGQUIT, &savequit, NULL) < 0 )        {
        return (-1);
    }
    if ( sigprocmask(SIG_SETMASK, &savemask, NULL) < 0 )  {
        return(-1);
    }

    return WEXITSTATUS(status);
}



/**
 * EOF
 **/
