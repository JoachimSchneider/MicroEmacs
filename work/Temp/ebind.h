/*======================================================================
 *      EBIND:          Initial default key to function bindings for
 *                      MicroEMACS 4.00
 *====================================================================*/

/*====================================================================*/
#ifndef EBIND_H_
#define EBIND_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#ifdef MAIN_C_

/*
 * Command table. This table  is *roughly* in ASCII order, left to right across
 * the characters of the command. It is being searched sequentially for
 * keystrokes and the &bind function. Thus I am placing more frequent keys at
 * the top . . .
 */
NOSHARE KEYTAB keytab[NBINDS] =
{

#if     MOUSE
    { MOUS|'m',           BINDFNC, { mmove } },
    { MOUS|CTRF|'m',      BINDFNC, { mmove } },
    { MOUS|SHFT|'m',      BINDFNC, { mmove } },
    { MOUS|CTRF|SHFT|'m', BINDFNC, { mmove } },
    { MOUS|'a',           BINDFNC, { movemd } },
    { MOUS|'b',           BINDFNC, { movemu } },
    { MOUS|'e',           BINDFNC, { mregdown } },
    { MOUS|'f',           BINDFNC, { mregup } },
    { MOUS|'1',           BINDFNC, { resizm } },
    { MOUS|'2',           BINDFNC, { resizm2 } },
#endif

    { CTRF|'A',           BINDFNC, { gotobol } },
    { CTRF|'B',           BINDFNC, { backchar } },
    { CTRF|'C',           BINDFNC, { insspace } },
    { CTRF|'D',           BINDFNC, { forwdel } },
    { CTRF|'E',           BINDFNC, { gotoeol } },
    { CTRF|'F',           BINDFNC, { forwchar } },
    { CTRF|'G',           BINDFNC, { ctrlg } },
    { CTRF|'H',           BINDFNC, { backdel } },
    { CTRF|'I',           BINDFNC, { tab } },
    { CTRF|'J',           BINDFNC, { indent } },
    { CTRF|'K',           BINDFNC, { killtext } },
    { CTRF|'L',           BINDFNC, { refresh } },
    { CTRF|'M',           BINDFNC, { newline } },
    { CTRF|'N',           BINDFNC, { forwline } },
    { CTRF|'O',           BINDFNC, { openline } },
    { CTRF|'P',           BINDFNC, { backline } },
    { CTRF|'Q',           BINDFNC, { quote } },
    { CTRF|'R',           BINDFNC, { backsearch } },
    { CTRF|'S',           BINDFNC, { forwsearch } },
    { CTRF|'T',           BINDFNC, { twiddle } },
    { CTRF|'U',           BINDFNC, { unarg } },
    { CTRF|'V',           BINDFNC, { forwpage } },
    { CTRF|'W',           BINDFNC, { killregion } },
    { CTRF|'X',           BINDFNC, { cex } },
    { CTRF|'Y',           BINDFNC, { yank } },
    { CTRF|'Z',           BINDFNC, { backpage } },
    { CTRF|'[',           BINDFNC, { f_meta } },
    { CTRF|'\\',          BINDFNC, { forwsearch } },
    { CTRF|'^',           BINDFNC, { quote } },
    { CTRF|'_',           BINDFNC, { undo } },
    { CTLX|CTRF|'A',      BINDFNC, { fileapp } },
    { CTLX|CTRF|'B',      BINDFNC, { listbuffers } },
    { CTLX|CTRF|'C',      BINDFNC, { quit } },
    { CTLX|CTRF|'D',      BINDFNC, { detab } },
    { CTLX|CTRF|'E',      BINDFNC, { entab } },
    { CTLX|CTRF|'F',      BINDFNC, { filefind } },
    { CTLX|CTRF|'I',      BINDFNC, { insfile } },
    { CTLX|CTRF|'K',      BINDFNC, { macrotokey } },
    { CTLX|CTRF|'L',      BINDFNC, { lowerregion } },
    { CTLX|CTRF|'M',      BINDFNC, { delmode } },
    { CTLX|CTRF|'N',      BINDFNC, { mvdnwind } },
    { CTLX|CTRF|'O',      BINDFNC, { deblank } },
    { CTLX|CTRF|'P',      BINDFNC, { mvupwind } },
    { CTLX|CTRF|'R',      BINDFNC, { fileread } },
    { CTLX|CTRF|'S',      BINDFNC, { filesave } },
    { CTLX|CTRF|'T',      BINDFNC, { trim } },
    { CTLX|CTRF|'U',      BINDFNC, { upperregion } },
    { CTLX|CTRF|'V',      BINDFNC, { viewfile } },
    { CTLX|CTRF|'W',      BINDFNC, { filewrite } },
    { CTLX|CTRF|'X',      BINDFNC, { swapmark } },
    { CTLX|CTRF|'Z',      BINDFNC, { shrinkwind } },
    { CTLX|CTRF|'\\',     BINDFNC, { filesave } },
    { CTLX|'?',           BINDFNC, { deskey } },
    { CTLX|'!',           BINDFNC, { spawn } },
    { CTLX|'@',           BINDFNC, { pipecmd } },
    { CTLX|'#',           BINDFNC, { f_filter } },
    { CTLX|'$',           BINDFNC, { execprg } },
    { CTLX|'=',           BINDFNC, { showcpos } },
    { CTLX|'(',           BINDFNC, { ctlxlp } },
    { CTLX|')',           BINDFNC, { ctlxrp } },
    { CTLX|'<',           BINDFNC, { narrow } },
    { CTLX|'>',           BINDFNC, { widen } },
    { CTLX|'^',           BINDFNC, { enlargewind } },
    { CTLX|' ',           BINDFNC, { remmark } },
    { CTLX|'0',           BINDFNC, { delwind } },
    { CTLX|'1',           BINDFNC, { onlywind } },
    { CTLX|'2',           BINDFNC, { splitwind } },
    { CTLX|'A',           BINDFNC, { setvar } },
    { CTLX|'B',           BINDFNC, { usebuffer } },
    { CTLX|'C',           BINDFNC, { spawncli } },
#if ( IS_UNIX() || VMS )
    { CTLX|'D',           BINDFNC, { bktoshell } },
#endif
    { CTLX|'E',           BINDFNC, { ctlxe } },
    { CTLX|'G',           BINDFNC, { dispvar } },
    { CTLX|'K',           BINDFNC, { killbuffer } },
    { CTLX|'M',           BINDFNC, { setmod } },
    { CTLX|'N',           BINDFNC, { filename } },
    { CTLX|'O',           BINDFNC, { nextwind } },
    { CTLX|'P',           BINDFNC, { prevwind } },
#if     ISRCH
    { CTLX|'R',           BINDFNC, { risearch } },
    { CTLX|'S',           BINDFNC, { fisearch } },
#endif
    { CTLX|'U',           BINDFNC, { undo_list } },
    { CTLX|'W',           BINDFNC, { resize } },
    { CTLX|'X',           BINDFNC, { nextbuffer } },
    { CTLX|'Y',           BINDFNC, { cycle_ring } },
    { CTLX|'Z',           BINDFNC, { enlargewind } },
    { META|CTRF|'C',      BINDFNC, { wordcount } },
    { META|CTRF|'E',      BINDFNC, { execproc } },
    { META|CTRF|'F',      BINDFNC, { getfence } },
    { META|CTRF|'G',      BINDFNC, { gotomark } },
    { META|CTRF|'H',      BINDFNC, { delbword } },
    { META|CTRF|'K',      BINDFNC, { unbindkey } },
    { META|CTRF|'L',      BINDFNC, { reposition } },
    { META|CTRF|'M',      BINDFNC, { delgmode } },
    { META|CTRF|'N',      BINDFNC, { namebuffer } },
    { META|CTRF|'R',      BINDFNC, { qreplace } },
    { META|CTRF|'S',      BINDFNC, { execfile } },
    { META|CTRF|'V',      BINDFNC, { nextdown } },
    { META|CTRF|'U',      BINDFNC, { undo_delete } },
    { META|CTRF|'W',      BINDFNC, { killpara } },
    { META|CTRF|'X',      BINDFNC, { execcmd } },
    { META|CTRF|'Y',      BINDFNC, { clear_ring } },
    { META|CTRF|'Z',      BINDFNC, { nextup } },
    { META|CTRF|'\\',     BINDFNC, { execfile } },
    { META|' ',           BINDFNC, { setmark } },
    { META|'?',           BINDFNC, { help } },
    { META|'!',           BINDFNC, { reposition } },
    { META|')',           BINDFNC, { trRegIndent } },
    { META|'(',           BINDFNC, { trRegUndent } },
    { META|'.',           BINDFNC, { setmark } },
    { META|'>',           BINDFNC, { gotoeob } },
    { META|'<',           BINDFNC, { gotobob } },
    { META|'~',           BINDFNC, { unmark } },
    { META|'A',           BINDFNC, { apro } },
    { META|'B',           BINDFNC, { backword } },
    { META|'C',           BINDFNC, { capword } },
    { META|'D',           BINDFNC, { delfword } },
#if     CRYPT
    { META|'E',           BINDFNC, { setekey } },
#endif
    { META|'F',           BINDFNC, { forwword } },
    { META|'G',           BINDFNC, { gotoline } },
    { META|'K',           BINDFNC, { bindtokey } },
    { META|'L',           BINDFNC, { lowerword } },
    { META|'M',           BINDFNC, { setgmode } },
    { META|'N',           BINDFNC, { gotoeop } },
    { META|'P',           BINDFNC, { gotobop } },
    { META|'Q',           BINDFNC, { trParFill } },
    { META|'R',           BINDFNC, { sreplace } },
#if ( IS_UNIX() || VMS )
    { META|'S',           BINDFNC, { bktoshell } },
#endif
    { META|'U',           BINDFNC, { upperword } },
    { META|'V',           BINDFNC, { backpage } },
    { META|'W',           BINDFNC, { copyregion } },
    { META|'X',           BINDFNC, { namedcmd } },
    { META|'Y',           BINDFNC, { yank_pop } },
    { META|'Z',           BINDFNC, { quickexit } },
 /**{ META|CTRF|'?',      BINDFNC, { delbword} } },**/  /*???*/
    { ALTD|'B',           BINDFNC, { list_screens } },
    { ALTD|'C',           BINDFNC, { cycle_screens } },
    { ALTD|'D',           BINDFNC, { delete_screen } },
    { ALTD|'F',           BINDFNC, { find_screen } },
    { ALTD|'N',           BINDFNC, { rename_screen } },
    { ALTD|'R',           BINDFNC, { backhunt } },
    { ALTD|'S',           BINDFNC, { forwhunt } },
    { SPEC|'<',           BINDFNC, { gotobob } },           /* Home */
    { SPEC|'P',           BINDFNC, { backline } },          /* up */
    { SPEC|'Z',           BINDFNC, { backpage } },          /* PgUp */
    { SPEC|'B',           BINDFNC, { backchar } },          /* left */

    { SPEC|'L',           BINDFNC, { reposition } },        /* NP5 */

    { SPEC|'F',           BINDFNC, { forwchar } },          /* right */
    { SPEC|'>',           BINDFNC, { gotoeob } },           /* End */
    { SPEC|'N',           BINDFNC, { forwline } },          /* down */
    { SPEC|'V',           BINDFNC, { forwpage } },          /* PgDn */
    { SPEC|'C',           BINDFNC, { insspace } },          /* Ins */
    { SPEC|'D',           BINDFNC, { forwdel } },           /* Del */

    { SPEC|CTRF|'B',      BINDFNC, { backword } },          /* ctrl left */
    { SPEC|CTRF|'F',      BINDFNC, { forwword } },          /* ctrl right */
    { SPEC|CTRF|'Z',      BINDFNC, { gotobop } },           /* ctrl PgUp */
    { SPEC|CTRF|'V',      BINDFNC, { gotoeop } },           /* ctrl PgDn */

#if     WINDOW_MSWIN    /* IBM-CUA clipboard bindings */
    { SPEC|CTRF|'C',      BINDFNC, { clipregion } },        /* ctrl Ins */
    { SPEC|SHFT|'C',      BINDFNC, { insertclip } },        /* shift Ins */
    { SPEC|SHFT|'D',      BINDFNC, { cutregion } },         /* shift Del */
#endif

#if ATKBD | OS2

/* The following keys are some of the extra ones that can be returned by an
 * AT-style keyboard.  By default we bind them to the same functions as the
 * equivalent PC keys.  However they, and the ones not mentioned here, are
 * available to the user for rebinding to functions of choice.
 *
 *  See keyboard.c for a full list of available keys.
 */
    { SPEC|'a',           BINDFNC, { gotobob } },           /* grey Home */
    { SPEC|'b',           BINDFNC, { backline } },          /* grey up */
    { SPEC|'c',           BINDFNC, { backpage } },          /* grey PgUp */
    { SPEC|'d',           BINDFNC, { backchar } },          /* grey left */
    { SPEC|'e',           BINDFNC, { reposition } },        /* grey center */
    { SPEC|'f',           BINDFNC, { forwchar } },          /* grey right */
    { SPEC|'g',           BINDFNC, { gotoeob } },           /* grey End */
    { SPEC|'h',           BINDFNC, { forwline } },          /* grey down */
    { SPEC|'i',           BINDFNC, { forwpage } },          /* grey PgDn */
    { SPEC|'j',           BINDFNC, { insspace } },          /* grey Ins */
    { SPEC|'k',           BINDFNC, { forwdel } },           /* grey Del */
    { SPEC|':',           BINDFNC, { execcmd } },           /* pause */

    { SPEC|CTRF|'c',      BINDFNC, { gotobop } },           /* ctrl grey PgUp */
    { SPEC|CTRF|'d',      BINDFNC, { backword } },          /* ctrl grey left */
    { SPEC|CTRF|'f',      BINDFNC, { forwword } },          /* ctrl grey right */
    { SPEC|CTRF|'i',      BINDFNC, { gotoeop } },           /* ctrl grey PgDn */

#endif

#if     HP150
    { SPEC|32,            BINDFNC, { backline } },
    { SPEC|33,            BINDFNC, { forwline } },
    { SPEC|35,            BINDFNC, { backchar } },
    { SPEC|34,            BINDFNC, { forwchar } },
    { SPEC|44,            BINDFNC, { gotobob } },
    { SPEC|46,            BINDFNC, { forwpage } },
    { SPEC|47,            BINDFNC, { backpage } },
    { SPEC|82,            BINDFNC, { nextwind } },
    { SPEC|68,            BINDFNC, { openline } },
    { SPEC|69,            BINDFNC, { killtext } },
    { SPEC|65,            BINDFNC, { forwdel } },
    { SPEC|64,            BINDFNC, { ctlxe } },
    { SPEC|67,            BINDFNC, { refresh } },
    { SPEC|66,            BINDFNC, { reposition } },
    { SPEC|83,            BINDFNC, { help } },
    { SPEC|81,            BINDFNC, { deskey } },
#endif

#if     HP110
    { SPEC|0x4b,          BINDFNC, { backchar } },
    { SPEC|0x4d,          BINDFNC, { forwchar } },
    { SPEC|0x48,          BINDFNC, { backline } },
    { SPEC|0x50,          BINDFNC, { forwline } },
    { SPEC|0x43,          BINDFNC, { help } },
    { SPEC|0x73,          BINDFNC, { backword } },
    { SPEC|0x74,          BINDFNC, { forwword } },
    { SPEC|0x49,          BINDFNC, { backpage } },
    { SPEC|0x51,          BINDFNC, { forwpage } },
#endif

#if     AMIGA
    { SPEC|'?',           BINDFNC, { help } },
    { SPEC|'A',           BINDFNC, { backline } },
    { SPEC|'B',           BINDFNC, { forwline } },
    { SPEC|'C',           BINDFNC, { forwchar } },
    { SPEC|'D',           BINDFNC, { backchar } },
    { SPEC|'T',           BINDFNC, { backpage } },
    { SPEC|'S',           BINDFNC, { forwpage } },
    { SPEC|'a',           BINDFNC, { backword } },
    { SPEC|'`',           BINDFNC, { forwword } },
    { 127, BINDFNC, { forwdel } },
#endif

#if     TOS
    { SPEC|CTRF|'5',      BINDFNC, { help } },
#endif

#if  WANGPC
    { SPEC|0xE0,          BINDFNC, { quit } },              /* Cancel */
    { SPEC|0xE1,          BINDFNC, { help } },              /* Help */
    { SPEC|0xF1,          BINDFNC, { help } },              /* ^Help */
    { SPEC|0xE3,          BINDFNC, { ctrlg } },             /* Print */
    { SPEC|0xF3,          BINDFNC, { ctrlg } },             /* ^Print */
    { SPEC|0xC0,          BINDFNC, { backline } },          /* North */
    { SPEC|0xD0,          BINDFNC, { gotobob } },           /* ^North */
    { SPEC|0xC1,          BINDFNC, { forwchar } },          /* East */
    { SPEC|0xD1,          BINDFNC, { gotoeol } },           /* ^East */
    { SPEC|0xC2,          BINDFNC, { forwline } },          /* South */
    { SPEC|0xD2,          BINDFNC, { gotobop } },           /* ^South */
    { SPEC|0xC3,          BINDFNC, { backchar } },          /* West */
    { SPEC|0xD3,          BINDFNC, { gotobol } },           /* ^West */
    { SPEC|0xC4,          BINDFNC, { ctrlg } },             /* Home */
    { SPEC|0xD4,          BINDFNC, { gotobob } },           /* ^Home */
    { SPEC|0xC5,          BINDFNC, { filesave } },          /* Execute */
    { SPEC|0xD5,          BINDFNC, { ctrlg } },             /* ^Execute */
    { SPEC|0xC6,          BINDFNC, { insfile } },           /* Insert */
    { SPEC|0xD6,          BINDFNC, { ctrlg } },             /* ^Insert */
    { SPEC|0xC7,          BINDFNC, { forwdel } },           /* Delete */
    { SPEC|0xD7,          BINDFNC, { killregion } },        /* ^Delete */
    { SPEC|0xC8,          BINDFNC, { backpage } },          /* Previous */
    { SPEC|0xD8,          BINDFNC, { prevwind } },          /* ^Previous */
    { SPEC|0xC9,          BINDFNC, { forwpage } },          /* Next */
    { SPEC|0xD9,          BINDFNC, { nextwind } },          /* ^Next */
    { SPEC|0xCB,          BINDFNC, { ctrlg } },             /* Erase */
    { SPEC|0xDB,          BINDFNC, { ctrlg } },             /* ^Erase */
    { SPEC|0xDC,          BINDFNC, { ctrlg } },             /* ^Tab */
    { SPEC|0xCD,          BINDFNC, { ctrlg } },             /* BackTab */
    { SPEC|0xDD,          BINDFNC, { ctrlg } },             /* ^BackTab */
    { SPEC|0x80,          BINDFNC, { ctrlg } },             /* Indent */
    { SPEC|0x90,          BINDFNC, { ctrlg } },             /* ^Indent */
    { SPEC|0x81,          BINDFNC, { ctrlg } },             /* Page */
    { SPEC|0x91,          BINDFNC, { ctrlg } },             /* ^Page */
    { SPEC|0x82,          BINDFNC, { ctrlg } },             /* Center */
    { SPEC|0x92,          BINDFNC, { ctrlg } },             /* ^Center */
    { SPEC|0x83,          BINDFNC, { ctrlg } },             /* DecTab */
    { SPEC|0x93,          BINDFNC, { ctrlg } },             /* ^DecTab */
    { SPEC|0x84,          BINDFNC, { ctrlg } },             /* Format */
    { SPEC|0x94,          BINDFNC, { ctrlg } },             /* ^Format */
    { SPEC|0x85,          BINDFNC, { ctrlg } },             /* Merge */
    { SPEC|0x95,          BINDFNC, { ctrlg } },             /* ^Merge */
    { SPEC|0x86,          BINDFNC, { setmark } },           /* Note */
    { SPEC|0x96,          BINDFNC, { ctrlg } },             /* ^Note */
    { SPEC|0x87,          BINDFNC, { ctrlg } },             /* Stop */
    { SPEC|0x97,          BINDFNC, { ctrlg } },             /* ^Stop */
    { SPEC|0x88,          BINDFNC, { forwsearch } },        /* Srch */
    { SPEC|0x98,          BINDFNC, { backsearch } },        /* ^Srch */
    { SPEC|0x89,          BINDFNC, { sreplace } },          /* Replac */
    { SPEC|0x99,          BINDFNC, { qreplace } },          /* ^Replac */
    { SPEC|0x8A,          BINDFNC, { ctrlg } },             /* Copy */
    { SPEC|0x9A,          BINDFNC, { ctrlg } },             /* ^Copy */
    { SPEC|0x8B,          BINDFNC, { ctrlg } },             /* Move */
    { SPEC|0x9B,          BINDFNC, { ctrlg } },             /* ^Move */
    { SPEC|0x8C,          BINDFNC, { namedcmd } },          /* Command */
    { SPEC|0x9C,          BINDFNC, { spawn } },             /* ^Command */
    { SPEC|0x8D,          BINDFNC, { ctrlg } },             /* ^ */
    { SPEC|0x9D,          BINDFNC, { ctrlg } },             /* ^^ */
    { SPEC|0x8E,          BINDFNC, { ctrlg } },             /* Blank */
    { SPEC|0x9E,          BINDFNC, { ctrlg } },             /* ^Blank */
    { SPEC|0x8F,          BINDFNC, { gotoline } },          /* GoTo */
    { SPEC|0x9F,          BINDFNC, { usebuffer } },         /* ^GoTo */
#endif

    { CTRF|'?',           BINDFNC, { backdel } },
    { 0,                  BINDNUL, { NULL } }
};

#endif  /* MAIN_C_  */



/*====================================================================*/
#endif/**#ifndef EBIND_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
