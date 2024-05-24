/* SOF */

#include <stdio.h>
#include <assert.h>
#include <errno.h>


extern int errno;
extern int open();
extern int read();

#ifndef O_RDONLY
#  define O_RDONLY  (0)
#endif


int main(argc, argv)
  int   argc;
  char *argv[];
{
  int   fd  = 0;
  char  c   = '\0';
  int   rc  = 0;

  assert(1 < argc);
  assert(NULL != argv[1]);

  if ( 0 > (fd = open(argv[1], O_RDONLY)) ) {
    perror("open()");
  }
  if ( 0 > (rc = read(fd, &c, sizeof(c))) ) {
#ifdef EISDIR
    if ( EISDIR == errno )  {
      fprintf(stderr, "%s ist ein Verzeichnis\n", argv[1]);
    }
#else
    perror("read()");
#endif
  }

  return 0;
}


/* EOF */

