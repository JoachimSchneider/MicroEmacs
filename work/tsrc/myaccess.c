/**********************************************************************/
/* Test access() with:                                                */
/* - Directories                                                      */
/* - CYGWIN:                                                          */
/*   + C:/Temp/X                                                      */
/*   + C:\Temp\X                                                      */
/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   /* (R|W|X)_OK */
#include <sys/stat.h>
#include <fcntl.h>


#define Cd42  42


#define TRUE  1
#define FALSE 0
#define CONST const


#define ZEROMEM(x)  ( memset(&(x), 0, sizeof((x))) )

/* Linux:
/o Values for the second argument to access.
   These may be OR'd together.  o/
#define R_OK    4               /o Test for read permission.  o/
#define W_OK    2               /o Test for write permission.  o/
#define X_OK    1               /o Test for execute permission.  o/
#define F_OK    0               /o Test for existence.  o/
*/

/* BSD 4.1:
     access checks the given file name for accessibility accord- ing to
     mode, which is 4 (read), 2 (write) or 1 (execute) or a combination
     thereof. Specifying mode 0 tests whether the directories leading
     to the file can be searched and the file exists.
*/              

static int myaccess(CONST char *path, int amod)
{
    int                 fd    = 0;
    int                 rc    = 0;
    struct stat         sb;
    unsigned int        mode  = 0;
    int                 uid   = getuid();
    int                 gid   = getgid();
    CONST unsigned int  pmask = 0007;
    unsigned int        pbits = 0;
    unsigned int        uamod = (unsigned int)amod;
    
    ZEROMEM(sb);

    if ( 0 != (rc  = stat(path, &sb)) ) {
        perror("stat:");
        fprintf(stderr, "stat(%d, &sb) failed\n", fd);
    }

    if ( 0 > (fd = open(path, O_RDONLY)) )  {
        perror("open:");
        fprintf(stderr, "open(%s, O_RDONLY) failed\n", path);

        return (-1);
    }
    
    if ( 0 != (rc  = fstat(fd, &sb)) ) {
        perror("fstat:");
        fprintf(stderr, "fstat(%d, &sb) failed\n", fd);

        return (-1);
    }
    close(fd);

    mode  = sb.st_mode & ~S_IFMT & 0777;

    fprintf(stderr, "Mode of %s = 0%03o\n", path, mode);

    pbits = mode & pmask; /* Everyone has "world" permissions */
    if ( gid == sb.st_gid ) {
        pbits |= (mode & (pmask << 3)) >> 3;  /* Add group mode */
        /* TODO: Add permissions resulting from secondary groups  */
    }
    if ( uid == sb.st_uid ) {
        pbits |= (mode & (pmask << 6)) >> 6;  /* Add owner mode */
    }

    fprintf(stderr, "pbits = 0%o\n", pbits);
    
    return (pbits & uamod) == uamod;
}

static int myaccess_0(CONST char *path, int amod)
{
    int                 rc    = 0;
    struct stat         sb;
    unsigned int        mode  = 0;
    int                 uid   = getuid();
    int                 gid   = getgid();
    CONST unsigned int  pmask = 0007;
    unsigned int        pbits = 0;
    unsigned int        uamod = (unsigned int)amod;
    
    ZEROMEM(sb);

    if ( 0 != (rc  = stat(path, &sb)) ) {
        return rc;
    }

    mode  = sb.st_mode & ~S_IFMT & 0777;

    fprintf(stderr, "Mode of %s = 0%03o\n", path, mode);

    pbits = mode & pmask; /* Everyone has "world" permissions */
    if ( gid == sb.st_gid ) {
        pbits |= (mode & (pmask << 3)) >> 3;  /* Add group mode */
        /* TODO: Add permissions resulting from secondary groups  */
    }
    if ( uid == sb.st_uid ) {
        pbits |= (mode & (pmask << 6)) >> 6;  /* Add owner mode */
    }

    fprintf(stderr, "pbits = 0%o\n", pbits);
    
    return (pbits & uamod) == uamod;
}


static int accessable(CONST char *d)
{
    if ( NULL == d )  {
        return FALSE;
    }

    if ( 0 == myaccess(d, R_OK|W_OK|X_OK) ) {
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
