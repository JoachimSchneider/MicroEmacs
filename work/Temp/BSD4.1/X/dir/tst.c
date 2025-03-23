#include <stdio.h>

#define CONCAT_(x, y) x/**/y
#define CONCAT(x, y) CONCAT_(x,y)
#define CASRT__(y, x)	typedef int jes_/**/y/**/[1 / ((x)? 1 : 0)]
#define CASRT_(y, x)    CASRT__(y, x)
#define CASRT(x)        CASRT_(__LINE__, x)

int main()
{
	static int i = 0;
	static int j = sizeof(i);
	static int k = (int)sizeof(i);
	static int l[(int)sizeof(i)];
	static int m[(int)(1)];

	/*CASRT_(10, sizeof(int) == 3);*/
	CASRT(sizeof(int) == 3);

	return 0;
}
