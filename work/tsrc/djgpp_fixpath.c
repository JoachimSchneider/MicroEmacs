#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
     char oldpath[100], newpath[FILENAME_MAX];

     scanf("%s", oldpath);
     _fixpath(oldpath, newpath);
     printf("that really is %s\n", newpath);

     return 0;
}
                    
