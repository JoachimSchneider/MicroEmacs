/* SOF(xmalloc_test)  */


#include <stdio.h>
#include "xmalloc.h"


int main(int aregc, char *argv[])
{
    char  *p0 = NULL;
    char  *p1 = NULL;
    char  *p2 = NULL;
    char  *p3 = NULL;

    int   i0  = 10;
    int   i1  = 100;
    int   i2  = 1000;
    int   i3  = 10000;

    p0  = xmalloc(i0);
    p1  = xmalloc(i1);
    p2  = xmalloc(i2);
    p3  = xmalloc(i3);

    xfree(p3);
    xfree(p2);
    xfree(p1);
    xfree(p0);

    p0  = xmalloc(i0);
    p1  = xmalloc(i1);
    p2  = xmalloc(i2);
    p3  = xmalloc(i3);

    p0  = xrealloc(p0, 3 * i0);
    p1  = xrealloc(p1, 3 * i1);
    p2  = xrealloc(p2, 3 * i2);
    p3  = xrealloc(p3, 3 * i3);


    xfree(p0);
    xfree(p1);
    xfree(p2);
    xfree(p3);

    return 0;
}



/* EOF(xmalloc_test)  */
