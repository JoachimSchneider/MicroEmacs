/**********************************************************************/
/* Test access() with:                                                */
/* - Directories                                                      */
/* - CYGWIN:                                                          */
/*   + C:/Temp/X                                                      */
/*   + C:\Temp\X                                                      */
/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>


#define Cd42  42


#define TRUE  1
#define FALSE 0
#define CONST const


static int accessable(CONST char *d)
{
    if ( NULL == d )  {
        return FALSE;
    }

    if ( 0 == access(d, R_OK|W_OK|X_OK) ) {
        return TRUE;
    } else                                {
        return FALSE;
    }
}

int main(int argc, char *argv[])
{
    if ( 2 != argc )  {
        fprintf(stderr, "%s: Usage: %s <dir>\n", argv[0], argv[0]);

        exit(Cd42);
    }

    if ( accessable(argv[1]) )  {
        fprintf(stdout, "%s\n", "YES");

        return 0;
    } else                      {
        fprintf(stdout, "%s\n", "NO");

        return 1;
    }
}



/*
 * EOF
 */