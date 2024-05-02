/* SOF(TERMINAL_VMS.H)  */
#ifndef TERMINAL_VMS_H_
#define TERMINAL_VMS_H_
/*======================================================================
 *  If the user has used /pointer_size=short or /pointer_size=long, we will
 *  begin in a 32-bit pointer size context.
 *====================================================================*/
#ifdef  __INITIAL_POINTER_SIZE
# if    __INITIAL_POINTER_SIZE
#  pragma __pointer_size __save
#  pragma __pointer_size 32
# endif
#endif
/*====================================================================*/


extern int  TerminalReadVMS(char *buf, int bufsiz, int read_echo);


/*======================================================================
 *  Restore the users pointer context
 *====================================================================*/
#ifdef  __INITIAL_POINTER_SIZE
# if    __INITIAL_POINTER_SIZE
#  pragma __pointer_size __restore
# endif
#endif
/*====================================================================*/
#endif
/* EOF(TERMINAL_VMS.H)  */
