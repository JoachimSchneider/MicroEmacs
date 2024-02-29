/*======================================================================
 *      CYGW32: CygWin Win32 functions
 *              for MicroEMACS 5.0
 *      2024 Joachim Schneider
 *
 *----------------------------------------------------------------------
 *====================================================================*/

/*====================================================================*/
#define CYGW32_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/** Include files **/
#include "estruct.h"            /* Emacs definitions        */
#undef VOID                     /* Re-defined in windows.h  */


/*==============================================================*/
/** Only compile on real CYGWIN **/
#if ( CYGWIN && __CYGWIN__ )
# include <stdio.h>
# include <string.h>
# include <w32api/windows.h>


const char *cygpwd_(void)
{
    static char res[NFILEN];

    int rc  = 0;

    memset(res, 0 sizeof(res));

    rc  = GetCurrentDirectoryA(sizeof(res), res);
    /* Return values:
     * - Success:         strlen(CurrenDirectory)
     * - Error:           0
     * - Buffer to small: strlen(CurrentDirectory) + 1
     */

    if ( 0 < rc && rc < sizeof(res) ) {
        return res;
    } else                            {
        return NULL;
    }
}



#else
int cyww32_hello_(char *s)
{
    return (0);
}
#endif /* CYGWIN && __CYGWIN__ */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
