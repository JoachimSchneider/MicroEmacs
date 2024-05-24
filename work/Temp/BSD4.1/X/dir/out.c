



extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	short	_flag;
	char	_file;
} _iob[20];


























struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();







struct	direct
{
	ino_t	d_ino;
	char	d_name[14];
};








int main(argc, argv)
	int	argc;
	char	*argv[];
{
  struct direct de;

    ( memset(&(de), 0, sizeof((de))) );

  argv++;

  while ( *argv ) {
    int fd  = (-1);
    int rc  = 0;

    if ( 0 > (fd  = open(*argv, 0)) ) {
      perror("open fd");
    }

    while ( sizeof(de) == (rc = read(fd, &de, sizeof(de))) )  {
      if ( 0 != de.d_ino )  {
        fprintf((&_iob[1]), "Name = %s\n", de.d_name);
      }
    }

    if ( 0 > rc ) {
      perror("read");
    }

    argv++;
  }

  return 0;
}




