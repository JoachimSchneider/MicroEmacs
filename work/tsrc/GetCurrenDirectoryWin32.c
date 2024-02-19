#include <w32api/windows.h>
#include <stdio.h>
#include <unistd.h>


#define LEN (1024)


int main(int argc, char *argv[])
{
    char  Buffer[LEN];
    char  Cwd[LEN];

    getcwd(Cwd, LEN);
    chdir("/");
    if ( 0 < GetCurrentDirectoryA(LEN, Buffer) )  {
        fprintf(stdout, "<%s>\n", Buffer);
    }
    chdir(Cwd);


    return 0;
}


/* EOF */
