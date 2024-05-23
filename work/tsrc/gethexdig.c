/*
 * SOF(gethexdig.c)
 */


#include <stdio.h>

#define C_1       1
#define NELEM(a)  ( (int) sizeof((a))/sizeof(*(a)) )


static int  GetHexDigVal(char d)
{
    static int  FirstCall = !0;
    static int  digtab[(int)(unsigned char)(-1) + 1];
# define SET_DIG_VAL_(d, v) digtab[(int)(unsigned char) (d)]  = (v)

    if ( FirstCall )  {
        int i = 0;

        for ( i = 0; i < NELEM(digtab); i++ ) {
            digtab[i] = (-1) * C_1;
        }
	SET_DIG_VAL_('0', 0);
	SET_DIG_VAL_('1', 1);
	SET_DIG_VAL_('2', 2);
	SET_DIG_VAL_('3', 3);
	SET_DIG_VAL_('4', 4);
	SET_DIG_VAL_('5', 5);
	SET_DIG_VAL_('6', 6);
	SET_DIG_VAL_('7', 7);
	SET_DIG_VAL_('8', 8);
	SET_DIG_VAL_('9', 9);

	SET_DIG_VAL_('a', 10);
	SET_DIG_VAL_('b', 11);
	SET_DIG_VAL_('c', 12);
	SET_DIG_VAL_('d', 13);
	SET_DIG_VAL_('e', 14);
	SET_DIG_VAL_('f', 15);

	SET_DIG_VAL_('A', 10);
	SET_DIG_VAL_('B', 11);
	SET_DIG_VAL_('C', 12);
	SET_DIG_VAL_('D', 13);
	SET_DIG_VAL_('E', 14);
	SET_DIG_VAL_('F', 15);

        FirstCall = 0;
    }

    return digtab[(unsigned char)d];
#undef  SET_DIG_VAL_
}



int main(int argc, char *argv[])
{
    char  inp = '\0';
    int   res = 0;

    argv++;
    argc--;

    inp = '\0';
    res = GetHexDigVal(inp);
    fprintf(stdout, "`%c' ===> %2d\n", inp, res);

    while ( 0 < argc )  {
        inp = argv[0][0];
        res = GetHexDigVal(inp);
        fprintf(stdout, "`%c' ===> %2d\n", inp, res);

        argv++;
        argc--;
    }

    inp = '\0';
    res = GetHexDigVal(inp);
    fprintf(stdout, "`%c' ===> %2d\n", inp, res);

    return 0;
}



/*
 * EOF(gethexdig.c)
 */
