/** SOF **/


#include <stdio.h>
#include <stdlib.h>
#undef NDEBUG
#include <assert.h>
#include <errno.h>

#define ASRT      assert
#define SIZEOF(x) ( (int)sizeof((x)) )

#if ( 1 )
# define UEGETC_BUFSIZ  (8192)
#else
# define UEGETC_BUFSIZ  (13)
#endif


int uegetc(FILE *fp)
{
    static char buf_[UEGETC_BUFSIZ];
    /* Position of next character to read. The buffer is filled by fread()
     * which reads at most SIZEOF(buf_) charcters.
     * First position: 0
     * Last  position: SIZEOF(buf_) - 1
     */
    static int  buf_pos_  = (-1);
    static int  read_res_ = (-1);

    if ( 0  > buf_pos_  || read_res_ - 1 < buf_pos_  )  {
        errno = 0;
        if ( 0 == (read_res_ = fread(buf_, 1, SIZEOF(buf_), fp)) )  {
            return EOF;
        }

        buf_pos_ = 0;
    }

    return (int)(unsigned char)buf_[buf_pos_++];
}


int main(int argc, char *argv[])
{
    int c = 0;

#if ( 1 )
    while ( EOF != (c = uegetc(stdin)) )  {
        /**fputc(c, stdout);**/
    }
#else
    while ( EOF != (c = getc(stdin)) )  {
        /**fputc(c, stdout);**/
    }
#endif

    return 0;
}



/** EOF **/
