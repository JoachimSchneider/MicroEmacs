/* SOF */


#include <spawn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>


#define ZEROMEM(x)  ( memset(&(x), 0, sizeof(x)) )
#define ASSERT(x)   do { if ( !(x) )  abort();  } while ( 0 )


static int mysystem (const char *cmd);


int main(int argc, char *argv[])
{
    if ( 2 != argc )  {
        fprintf(stderr, "Usage %s <Command>\n", argv[0]);

        exit(42);
    }

    return mysystem(argv[1]);
}


/* No braces `()' here! */
#define C_10  10
#define C_20  20
#define C_30  30
#define C_40  40
#define SHELL_    "/bin/sh"
#define SHELL_C_  "-c"
static int mysystem(const char *cmd)
{
    pid_t             child_pid = 0;
    int               status    = 0;
    int               errno_sv  = 0;
    sigset_t          mask;
    posix_spawnattr_t attr;
    char  * const     sargv[] = { (char *)SHELL_, (char *)SHELL_C_, (char *)cmd, NULL };

    extern char **environ;

    ZEROMEM(mask);
    ZEROMEM(attr);

    if ( NULL == cmd )  {
        cmd = "";
    }

    /* Create an attributes object and add a "set signal mask"
       action to it. */

    if ( 0 != posix_spawnattr_init(&attr) ) {
        return ( -C_20 );
    }
    if ( 0 != posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGMASK) ) {
        return ( -C_30 );
    }
    sigfillset(&mask);
    if ( 0 != posix_spawnattr_setsigmask(&attr, &mask) )  {
        return ( -C_40 );
    }

    if ( 0 != posix_spawnp(&child_pid, SHELL_, NULL, &attr, sargv, environ) )  {
        errno_sv  = errno;
    }

    /* Destroy any objects that we created earlier. */
    if ( 0 != posix_spawnattr_destroy(&attr) )  {
        /**EMPTY**/
    }

    if ( 0 != errno_sv )  {
        return ( (-1) * errno_sv );
    }

    /* Monitor status of the child until it terminates. */
    do {
        if ( 0 > waitpid(child_pid, &status, WUNTRACED | WCONTINUED) )  {
            return ( -1 );
        }

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            return WTERMSIG(status);
        } else if (WIFSTOPPED(status)) {
            /**EMPTY**/
        } else if (WIFCONTINUED(status)) {
            /**EMPTY**/
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    return WEXITSTATUS ( status );
}



/* EOF */
