#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>


#define ERROR_EXIT  (43)

static int readn(fd, buf, nbytes)
	int  fd;
	char *buf;
	int  nbytes;
{
	char *bp   = (char *)buf;
	int  nleft = nbytes;
	int  nread = 0;
	int  n     = 0;

	while ( 0 < nleft ) {
		n = read(fd, bp, nleft);

		if ( 0 > n )         { /* ERROR  */
			return n;
		} else if ( 0 == n)  { /* EOF    */
			return nread;
		} else   /* 0 < n */ {
			nread += n;
			nleft -= n;
			bp    += n;
		}
	}

	return nread;
}

static char *qmemset(x, c, l)
	char *x;
	char  c;
	int   l;
{
	int	i = 0;

	for ( i = 0; i < l; i++ ) {
		x[i] = c;
	}

	return x;
}

static char *qmemcpy(x, y, l)
	char *x;
	char *y;
	int   l;
{
	int	i = 0;

	if ( x < y ) {
		for ( i = 0; i < l; i++ ) {
			x[i] = y[i];
		}
	} else {
		for ( i = l - 1; i >= 0 ; i-- ) {
			x[i] = y[i];
		}
	}

	return x;
}

#define ZEROMEM(x)  ( qmemset(&(x), 0, sizeof((x))) )
#define MIN2(x, y)  ( ((x) < (y))? (x) : (y) )


int main(argc, argv)
	int	argc;
	char	*argv[];
{
  struct direct de;
  char   fname[sizeof(de.d_name) + 1];

  ZEROMEM(de);
  ZEROMEM(fname);

  argv++;

  while ( *argv ) {
    int fd  = (-1);
    int rc  = 0;

    if ( 0 > (fd  = open(*argv, 0)) ) {
      perror("open fd");
    }

    while ( sizeof(de) == (rc = readn(fd, &de, sizeof(de))) )  {
      if ( 0 != de.d_ino )  {
        qmemcpy(fname, de.d_name, MIN2(sizeof(fname) - 1, sizeof(de.d_name)));
        fprintf(stdout, "Name = %s\n", fname);
      }
    }

    if ( 0 > rc ) {
      perror("read");
    }

    argv++;
  }

  return 0;
}


/* EOF */

