/*======================================================================
 * This file provides ID definitions for the menu resource
 *  used under the Microsoft Windows environment on
 * an IBM-PC or compatible computer.
 *
 * Must be compiled with the Resource Compiler or Borland C++ 2.0
 * or MSC 6.0 or later versions
 *
 * It should not be compiled if the WINDOW_MSWIN symbol is not set
 *====================================================================*/

/*====================================================================*/
#ifndef MSWMENU_H_
#define MSWMENU_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#define IDM_POPUP       0x1000      /* popup level increment */
#define IDM_DYNAMIC     0x0800      /* IDs above that are dynamic entries */
#define IDM_NOTVIEW     0x0400      /* item to be grayed in view mode */

#define IDM_NULLPROC    0x03FF

/* File */
#define IDM_FILEFIND    0x010
#define IDM_FILE_POPUP  IDM_FILEFIND+IDM_POPUP
#define IDM_VIEWFILE    0x012
#define IDM_INSFILE     0x013+IDM_NOTVIEW
#define IDM_FILEAPP     0x014+IDM_NOTVIEW
#define IDM_FILEREAD    0x015
#define IDM_FILENAME    0x016
#define IDM_FILESAVE    0x017
#define IDM_FILEWRITE   0x018
#define IDM_SETEKEY     0x019
#define IDM_NEXTBUFFER  0x01B
#define IDM_FILE_BUFFER_POPUP IDM_NEXTBUFFER+IDM_POPUP
#define IDM_USEBUFFER   0x01C
#define IDM_UNMARK      0x01D+IDM_NOTVIEW
#define IDM_NAMEBUFFER  0x01E
#define IDM_KILLBUFFER  0x020
#define IDM_NARROW      0x021
#define IDM_WIDEN       0x022
#define IDM_LISTBUFFERS 0x023
#define IDM_SPLITWIND   0x024
#define IDM_FILE_WINDOW_POPUP IDM_SPLITWIND+IDM_POPUP
#define IDM_DELWIND     0x025
#define IDM_ONLYWIND    0x026
#define IDM_NEXTWIND    0x027
#define IDM_PREVWIND    0x028
#define IDM_MVUPWIND    0x029
#define IDM_FILE_WINDOW_SCROLL_POPUP IDM_MVUPWIND+IDM_POPUP
#define IDM_MVDNWIND    0x02A
#define IDM_NEXTUP      0x02B
#define IDM_NEXTDOWN    0x02C
#define IDM_ENLARGEWIND 0x02D
#define IDM_FILE_WINDOW_SIZE_POPUP IDM_ENLARGEWIND+IDM_POPUP
#define IDM_SHRINKWIND  0x02E
#define IDM_RESIZE      0x02F
#define IDM_MODE        0x030
#define IDM_GLOBMODE    0x031
#define IDM_QUICKEXIT   0x032
#define IDM_QUIT        0x033

/* Edit */
#define IDM_CUTREGION   0x050+IDM_NOTVIEW
#define IDM_EDIT_CLIPBOARD_POPUP IDM_CUTREGION+IDM_POPUP
#define IDM_EDIT_POPUP  IDM_EDIT_CLIPBOARD_POPUP+IDM_POPUP
#define IDM_CLIPREGION  0x051
#define IDM_INSERTCLIP  0x052+IDM_NOTVIEW
#define IDM_SETMARK     0x053
#define IDM_EDIT_MARK_POPUP IDM_SETMARK+IDM_POPUP
#define IDM_REMMARK     0x054
#define IDM_SWAPMARK    0x055
#define IDM_YANK        0x056+IDM_NOTVIEW
#define IDM_KILLREGION  0x057+IDM_NOTVIEW
#define IDM_EDIT_REGION_POPUP IDM_KILLREGION+IDM_POPUP
#define IDM_COPYREGION  0x058
#define IDM_UPPERREGION 0x059+IDM_NOTVIEW
#define IDM_LOWERREGION 0x05A+IDM_NOTVIEW
#define IDM_ENTAB       0x05B+IDM_NOTVIEW
#define IDM_DETAB       0x05C+IDM_NOTVIEW
#define IDM_TRIM        0x05D+IDM_NOTVIEW
/*
** separated out because there seems to be a MIPS compiler problem on NT July 92
** PDK that prevents the preprocesor from breaking this one down.
*/
#define IDM_INDENT_REGION 0x05E + IDM_NOTVIEW
#define IDM_UNDENT_REGION 0x05F+IDM_NOTVIEW
#define IDM_WORDCOUNT     0x06B
#define IDM_EDIT_PARAGRAPH_POPUP IDM_KILLPARA+IDM_POPUP
#define IDM_KILLPARA    0x061+IDM_NOTVIEW
#define IDM_FILLPARA    0x060+IDM_NOTVIEW
#define IDM_KILLTEXT    0x062+IDM_NOTVIEW
#define IDM_EDIT_LINE_POPUP IDM_KILLTEXT+IDM_POPUP
#define IDM_OPENLINE    0x063+IDM_NOTVIEW
#define IDM_DELFWORD    0x064+IDM_NOTVIEW
#define IDM_EDIT_WORD_POPUP IDM_DELFWORD+IDM_POPUP
#define IDM_DELBWORD    0x065+IDM_NOTVIEW
#define IDM_CAPWORD     0x066+IDM_NOTVIEW
#define IDM_LOWERWORD   0x067+IDM_NOTVIEW
#define IDM_UPPERWORD   0x068+IDM_NOTVIEW
#define IDM_DEBLANK     0x069+IDM_NOTVIEW
#define IDM_TWIDDLE     0x06A+IDM_NOTVIEW
#define IDM_TAB         0x06C+IDM_NOTVIEW
#define IDM_QUOTE       0x06D+IDM_NOTVIEW

/* Search */
#define IDM_FORWSEARCH  0x080
#define IDM_SEARCH_POPUP IDM_FORWSEARCH+IDM_POPUP
#define IDM_BACKSEARCH  0x081
#define IDM_FORWHUNT    0x082
#define IDM_BACKHUNT    0x083
#define IDM_FISEARCH    0x084
#define IDM_RISEARCH    0x085
#define IDM_SREPLACE    0x086+IDM_NOTVIEW
#define IDM_QREPLACE    0x087+IDM_NOTVIEW
#define IDM_GOTOMARK    0x088
#define IDM_SEARCH_GOTO_POPUP IDM_GOTOMARK+IDM_POPUP
#define IDM_GOTOLINE    0x089
#define IDM_GETFENCE    0x08A
#define IDM_GOTOBOB     0x08B
#define IDM_GOTOEOB     0x08C
#define IDM_FORWPAGE    0x08D
#define IDM_SEARCH_PAGE_POPUP IDM_FORWPAGE+IDM_POPUP
#define IDM_BACKPAGE    0x08E
#define IDM_GOTOEOP     0x08F
#define IDM_SEARCH_PARAGRAPH_POPUP IDM_GOTOEOP+IDM_POPUP
#define IDM_GOTOBOP     0x090
#define IDM_FORWLINE    0x091
#define IDM_SEARCH_LINE_POPUP IDM_FORWLINE+IDM_POPUP
#define IDM_BACKLINE    0x092
#define IDM_GOTOBOL     0x093
#define IDM_GOTOEOL     0x094
#define IDM_FORWWORD    0x095
#define IDM_SEARCH_WORD_POPUP IDM_FORWWORD+IDM_POPUP
#define IDM_BACKWORD    0x096
#define IDM_ENDWORD     0x097

/* Execute */
#define IDM_EXECPRG     0x0B0
#define IDM_EXECUTE_POPUP IDM_EXECPRG+IDM_POPUP
#define IDM_SPAWNCLI    0x0B1
#define IDM_SPAWN       0x0B2
#define IDM_PIPECMD     0x0B3
#define IDM_FILTER      0x0B4+IDM_NOTVIEW
#define IDM_CTLXE       0x0B5
#define IDM_EXECUTE_KEYBOARD_POPUP IDM_CTLXE+IDM_POPUP
#define IDM_CTLXLP      0x0B6
#define IDM_CTLXRP      0x0B7
#define IDM_NAMEDCMD    0x0B8
#define IDM_EXECUTE_RUN_POPUP IDM_NAMEDCMD+IDM_POPUP
#define IDM_EXECCMD     0x0B9
#define IDM_EXECPROC    0x0BA
#define IDM_EXECBUF     0x0BB
#define IDM_EXECFILE    0x0BC
#define IDM_CTRLG       0x0BD

/* Miscellaneous */
#define IDM_BINDTOKEY   0x0D0
#define IDM_MISC_BINDTOKEY_POPUP IDM_BINDTOKEY+IDM_POPUP
#define IDM_MISC_POPUP  IDM_MISC_BINDTOKEY_POPUP+IDM_POPUP
#define IDM_UNBINDKEY   0x0D2
#define IDM_DESKEY      0x0D3
#define IDM_MISC_MACROTOKEY_POPUP IDM_MACROTOKEY+IDM_POPUP
#define IDM_MACROTOKEY  0x0D1
#define IDM_BINDTOMENU  0x0C0
#define IDM_MACROTOMENU 0x0C1
#define IDM_UNBINDMENU  0x0C2
#define IDM_SETVAR      0x0D4
#define IDM_MISC_VARIABLE_POPUP IDM_SETVAR+IDM_POPUP
#define IDM_DISPVAR     0x0D5
#define IDM_DESVARS     0x0D6
#define IDM_SHOWCPOS    0x0DA

/* Screen */
#define IDM_CASCADE     0x110
#define IDM_SCREEN_POPUP IDM_CASCADE+IDM_POPUP
#define IDM_TILE_HORIZONTALLY 0x111
#define IDM_TILE_POPUP IDM_TILE_HORIZONTALLY
#define IDM_TILE_VERTICALLY 0x112
#define IDM_ARRANGEICONS 0x113
#define IDM_FIND_SCREEN 0x100
#define IDM_RENAME_SCREEN 0x105
#define IDM_NEWSIZE     0x101
#define IDM_SCREEN_SIZE_POPUP IDM_NEWSIZE+IDM_POPUP
#define IDM_NEWWIDTH    0x102
#define IDM_NORMALIZE   0x103
#define IDM_FONT        0x104

/* Help */
#define IDM_WHELPINDEX  0x120
#define IDM_HELP_POPUP  IDM_WHELPINDEX+IDM_POPUP
#define IDM_WHELPKEYBOARD 0x121
#define IDM_WHELPCOMMANDS 0x122
#define IDM_WHELPPROCEDURES 0x123
#define IDM_DESBIND     0x125
#define IDM_HELP_LIST_POPUP IDM_DESBIND+IDM_POPUP
#define IDM_DESFUNC     0x126
/* define IDM_DESVARS   already done above */
#define IDM_APRO        0x127
#define IDM_ABOUT       0x128

#if 0   /* put a 1 here to add the "Break to debugger" menu item */
# define IDM_DEBUG       0x200
#endif



/*====================================================================*/
#endif/**#ifndef MSWMENU_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
