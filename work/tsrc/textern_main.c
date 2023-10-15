#include <stdio.h>

#include "textern_mod.h"


#define SIZEOF(x) ( (int)sizeof((x)) )


int main(int argc, char *argv[])
{
  fprintf(stdout, "&arr = 0x%08lx, &*arr = 0x%08lx\n",
                  (unsigned long)&arr, (unsigned long)&*arr);
  fprintf(stdout, "Sizeof(arr) = %d\n", arr_s_);
  fprintf(stdout, "        arr = %s\n", arr);
  
  return 0;
}
