/*======================================================================
 *      ELANG.H:        Language specific definitions for
 *                      MicroEMACS 4.00
 *                      (C)opyright 1995 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#ifndef ELANG_H_
#define ELANG_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#if     ENGLISH
# include        "english.h"
# define LANGUAGE "english"
#endif
#if     FRENCH
# if ( WINDOW_MSWIN || IS_UNIX() )
#  include        "frenchis.h"  /* ISO version */
# else
#  include        "french.h"    /* IBM-PC char set */
# endif
# define LANGUAGE "french"
#endif
#if     SPANISH
# include        "spanish.h"
# define LANGUAGE "spanish"
#endif
#if     GERMAN
# include        "german.h"
# define LANGUAGE "german"
#endif
#if     DUTCH
# include        "dutch.h"
# define LANGUAGE "dutch"
#endif
#if     PLATIN
# include        "platin.h"
# define LANGUAGE "pig latin"
#endif
#if     JAPAN
# include        "japan.h"
# define LANGUAGE "japaneese"
#endif
#if     LATIN
# include        "latin.h"
# define LANGUAGE "latin"
#endif



/*====================================================================*/
#endif/**#ifndef ELANG_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
