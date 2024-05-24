/*======================================================================
 *      ENGLISH.H:      English language text strings for
 *                      MicroEMACS 4.00
 *
 *                      (C)opyright 1995 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#ifndef ENGLISH_H_
#define ENGLISH_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/

/**********************************************************************/
#include "eproto.h"
/**********************************************************************/


#define OQUOTE_CHAR     34
#define CQUOTE_CHAR     34

TDCLDEF char TEXT1[NOSZ_]   TINIT_("[Starting new CLI]");
TDCLDEF char TEXT2[NOSZ_]   TINIT_("[Cannot write filter file]");
TDCLDEF char TEXT3[NOSZ_]   TINIT_("[Execution failed]");
TDCLDEF char TEXT4[NOSZ_]   TINIT_("Shell variable TERM not defined!");
TDCLDEF char TEXT5[NOSZ_]   TINIT_("Terminal type not 'vt100'!");
TDCLDEF char TEXT6[NOSZ_]   TINIT_("\r\n\n[End]");
TDCLDEF char TEXT7[NOSZ_]   TINIT_("Line to GOTO: ");
TDCLDEF char TEXT8[NOSZ_]   TINIT_("[Aborted]");
TDCLDEF char TEXT9[NOSZ_]   TINIT_("[Mark %d set]");
TDCLDEF char TEXT10[NOSZ_]  TINIT_("[Mark %d removed]");
TDCLDEF char TEXT11[NOSZ_]  TINIT_("No mark %d in this window");
TDCLDEF char TEXT12[NOSZ_]  TINIT_("[Help file is not online]");
TDCLDEF char TEXT13[NOSZ_]  TINIT_(": describe-key ");
TDCLDEF char TEXT14[NOSZ_]  TINIT_("Not Bound");
TDCLDEF char TEXT15[NOSZ_]  TINIT_(": bind-to-key ");
TDCLDEF char TEXT16[NOSZ_]  TINIT_("[No such function]");
TDCLDEF char TEXT17[NOSZ_]  TINIT_("Binding table FULL!");
TDCLDEF char TEXT18[NOSZ_]  TINIT_(": unbind-key ");
TDCLDEF char TEXT19[NOSZ_]  TINIT_("[Key not bound]");
TDCLDEF char TEXT20[NOSZ_]  TINIT_("Apropos string: ");
TDCLDEF char TEXT21[NOSZ_]  TINIT_("Binding list");
TDCLDEF char TEXT22[NOSZ_]  TINIT_("Can not display binding list");
TDCLDEF char TEXT23[NOSZ_]  TINIT_("[Building binding list]");
TDCLDEF char TEXT24[NOSZ_]  TINIT_("Use buffer");
TDCLDEF char TEXT25[NOSZ_]  TINIT_("Use buffer: ");
TDCLDEF char TEXT26[NOSZ_]  TINIT_("Kill buffer");
TDCLDEF char TEXT27[NOSZ_]  TINIT_("Pop buffer");
TDCLDEF char TEXT28[NOSZ_]  TINIT_("Buffer is being displayed");
TDCLDEF char TEXT29[NOSZ_]  TINIT_("Change buffer name to: ");
TDCLDEF char TEXT30[NOSZ_]  TINIT_("ACTN   Modes        Size Buffer          File");
TDCLDEF char TEXT31[NOSZ_]  TINIT_("         Global Modes");
TDCLDEF char TEXT32[NOSZ_]  TINIT_("Discard changes");
TDCLDEF char TEXT33[NOSZ_]  TINIT_("Encryption String: ");
TDCLDEF char TEXT34[NOSZ_]  TINIT_("File: ");
TDCLDEF char TEXT35[NOSZ_]  TINIT_("another user");
TDCLDEF char TEXT36[NOSZ_]  TINIT_("LOCK ERROR -- ");
TDCLDEF char TEXT37[NOSZ_]  TINIT_("checking for existence of %s\n");
TDCLDEF char TEXT38[NOSZ_]  TINIT_("making directory %s\n");
TDCLDEF char TEXT39[NOSZ_]  TINIT_("creating %s\n");
TDCLDEF char TEXT40[NOSZ_]  TINIT_("could not create lock file");
TDCLDEF char TEXT41[NOSZ_]  TINIT_("pid is %ld\n");
TDCLDEF char TEXT42[NOSZ_]  TINIT_("reading lock file %s\n");
TDCLDEF char TEXT43[NOSZ_]  TINIT_("could not read lock file");
TDCLDEF char TEXT44[NOSZ_]  TINIT_("pid in %s is %ld\n");
TDCLDEF char TEXT45[NOSZ_]  TINIT_("signaling process %ld\n");
TDCLDEF char TEXT46[NOSZ_]  TINIT_("process exists");
TDCLDEF char TEXT47[NOSZ_]  TINIT_("kill was bad");
TDCLDEF char TEXT48[NOSZ_]  TINIT_("kill was good; process exists");
TDCLDEF char TEXT49[NOSZ_]  TINIT_("attempting to unlink %s\n");
TDCLDEF char TEXT50[NOSZ_]  TINIT_("could not remove lock file");
TDCLDEF char TEXT51[NOSZ_]  TINIT_("Variable to set: ");
TDCLDEF char TEXT52[NOSZ_]  TINIT_("%%No such variable as '%s'");
TDCLDEF char TEXT53[NOSZ_]  TINIT_("Value: ");
TDCLDEF char TEXT54[NOSZ_]  TINIT_("[Macro aborted]");
TDCLDEF char TEXT55[NOSZ_]  TINIT_("Variable to display: ");
TDCLDEF char TEXT56[NOSZ_]  TINIT_("Variable list");
TDCLDEF char TEXT57[NOSZ_]  TINIT_("Can not display variable list");
TDCLDEF char TEXT58[NOSZ_]  TINIT_("[Building variable list]");
TDCLDEF char TEXT59[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT60[NOSZ_]  TINIT_("Line %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x");
TDCLDEF char TEXT61[NOSZ_]  TINIT_("<Not Used>");
TDCLDEF char TEXT62[NOSZ_]  TINIT_("Global mode to ");
TDCLDEF char TEXT63[NOSZ_]  TINIT_("Mode to ");
TDCLDEF char TEXT64[NOSZ_]  TINIT_("add: ");
TDCLDEF char TEXT65[NOSZ_]  TINIT_("delete: ");
TDCLDEF char TEXT66[NOSZ_]  TINIT_("No such mode!");
TDCLDEF char TEXT67[NOSZ_]  TINIT_("Message to write: ");
TDCLDEF char TEXT68[NOSZ_]  TINIT_("String to insert: ");
TDCLDEF char TEXT69[NOSZ_]  TINIT_("String to overwrite: ");
TDCLDEF char TEXT70[NOSZ_]  TINIT_("[region copied]");
TDCLDEF char TEXT71[NOSZ_]  TINIT_("%%This buffer is already narrowed");
TDCLDEF char TEXT72[NOSZ_]  TINIT_("%%Must narrow at least 1 full line");
TDCLDEF char TEXT73[NOSZ_]  TINIT_("[Buffer is narrowed]");
TDCLDEF char TEXT74[NOSZ_]  TINIT_("%%This buffer is not narrowed");
TDCLDEF char TEXT75[NOSZ_]  TINIT_("[Buffer is widened]");
TDCLDEF char TEXT76[NOSZ_]  TINIT_("No mark set in this window");
TDCLDEF char TEXT77[NOSZ_]  TINIT_("Bug: lost mark");
TDCLDEF char TEXT78[NOSZ_]  TINIT_("Search ");
TDCLDEF char TEXT79[NOSZ_]  TINIT_("Not found ");
TDCLDEF char TEXT80[NOSZ_]  TINIT_("No pattern set");
TDCLDEF char TEXT81[NOSZ_]  TINIT_("Reverse search ");
TDCLDEF char TEXT82[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT83[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT84[NOSZ_]  TINIT_("Replace ");
TDCLDEF char TEXT85[NOSZ_]  TINIT_("Query replace ");
TDCLDEF char TEXT86[NOSZ_]  TINIT_("with ");
TDCLDEF char TEXT87[NOSZ_]  TINIT_("Replace '");
TDCLDEF char TEXT88[NOSZ_]  TINIT_("' with '");
TDCLDEF char TEXT89[NOSZ_]  TINIT_("Aborted!");
TDCLDEF char TEXT90[NOSZ_]  TINIT_("(Y)es, (N)o, (!)Do rest, (U)ndo last, (^G)Abort, (.)Abort back, (?)Help: ");
TDCLDEF char TEXT91[NOSZ_]  TINIT_("Empty string replaced, stopping.");
TDCLDEF char TEXT92[NOSZ_]  TINIT_("%D substitutions");
TDCLDEF char TEXT93[NOSZ_]  TINIT_("%%ERROR while deleting");
TDCLDEF char TEXT94[NOSZ_]  TINIT_("%%Out of memory");
TDCLDEF char TEXT95[NOSZ_]  TINIT_("%%mceq: what is %d?");
TDCLDEF char TEXT96[NOSZ_]  TINIT_("%%No characters in character class");
TDCLDEF char TEXT97[NOSZ_]  TINIT_("%%Character class not ended");
TDCLDEF char TEXT98[NOSZ_]  TINIT_("No fill column set");
TDCLDEF char TEXT99[NOSZ_]  TINIT_("[OUT OF MEMORY]");   /* UNUSED */
TDCLDEF char TEXT100[NOSZ_] TINIT_("Words %D Chars %D Lines %d Avg chars/word %f");
TDCLDEF char TEXT101[NOSZ_] TINIT_("[Can not search and goto at the same time!]");
TDCLDEF char TEXT102[NOSZ_] TINIT_("[Bogus goto argument]");
TDCLDEF char TEXT103[NOSZ_] TINIT_("[Saving %s]");
TDCLDEF char TEXT104[NOSZ_] TINIT_("Modified buffers exist. Leave anyway");
TDCLDEF char TEXT105[NOSZ_] TINIT_("%%Macro already active");
TDCLDEF char TEXT106[NOSZ_] TINIT_("[Start macro]");
TDCLDEF char TEXT107[NOSZ_] TINIT_("%%Macro not active");
TDCLDEF char TEXT108[NOSZ_] TINIT_("[End macro]");
TDCLDEF char TEXT109[NOSZ_] TINIT_("[Key illegal in VIEW mode]");
TDCLDEF char TEXT110[NOSZ_] TINIT_("[That command is RESTRICTED]");
TDCLDEF char TEXT111[NOSZ_] TINIT_("No macro specified");
TDCLDEF char TEXT112[NOSZ_] TINIT_("Macro number out of range");
TDCLDEF char TEXT113[NOSZ_] TINIT_("Can not create macro");
TDCLDEF char TEXT114[NOSZ_] TINIT_("Procedure name: ");
TDCLDEF char TEXT115[NOSZ_] TINIT_("Execute procedure: ");
TDCLDEF char TEXT116[NOSZ_] TINIT_("No such procedure");
TDCLDEF char TEXT117[NOSZ_] TINIT_("Execute buffer: ");
TDCLDEF char TEXT118[NOSZ_] TINIT_("No such buffer");
TDCLDEF char TEXT119[NOSZ_] TINIT_("%%Out of memory during while scan");
TDCLDEF char TEXT120[NOSZ_] TINIT_("%%!BREAK outside of any !WHILE loop");
TDCLDEF char TEXT121[NOSZ_] TINIT_("%%!ENDWHILE with no preceding !WHILE");
TDCLDEF char TEXT122[NOSZ_] TINIT_("%%!WHILE with no matching !ENDWHILE");
TDCLDEF char TEXT123[NOSZ_] TINIT_("%%Out of Memory during macro execution");
TDCLDEF char TEXT124[NOSZ_] TINIT_("%%Unknown Directive");
TDCLDEF char TEXT125[NOSZ_] TINIT_("Out of memory while storing macro");
TDCLDEF char TEXT126[NOSZ_] TINIT_("%%Internal While loop error");
TDCLDEF char TEXT127[NOSZ_] TINIT_("%%No such label");
TDCLDEF char TEXT128[NOSZ_] TINIT_("(e)val exp, (c/x)ommand, (t)rack exp, (^G)abort, <SP>exec, <META> stop debug");
TDCLDEF char TEXT129[NOSZ_] TINIT_("File to execute: ");
TDCLDEF char TEXT130[NOSZ_] TINIT_("Macro not defined");
TDCLDEF char TEXT131[NOSZ_] TINIT_("Read file");
TDCLDEF char TEXT132[NOSZ_] TINIT_("Insert file");
TDCLDEF char TEXT133[NOSZ_] TINIT_("Find file");
TDCLDEF char TEXT134[NOSZ_] TINIT_("View file");
TDCLDEF char TEXT135[NOSZ_] TINIT_("[Old buffer]");
TDCLDEF char TEXT136[NOSZ_] TINIT_("Buffer name: ");
TDCLDEF char TEXT137[NOSZ_] TINIT_("Cannot create buffer");
TDCLDEF char TEXT138[NOSZ_] TINIT_("[New file]");
TDCLDEF char TEXT139[NOSZ_] TINIT_("[Reading file]");
TDCLDEF char TEXT140[NOSZ_] TINIT_("Read ");
TDCLDEF char TEXT141[NOSZ_] TINIT_("I/O ERROR, ");
TDCLDEF char TEXT142[NOSZ_] TINIT_("OUT OF MEMORY, ");
TDCLDEF char TEXT143[NOSZ_] TINIT_(" line");
TDCLDEF char TEXT144[NOSZ_] TINIT_("Write file: ");
TDCLDEF char TEXT145[NOSZ_] TINIT_("No file name");
TDCLDEF char TEXT146[NOSZ_] TINIT_("Truncated file..write it out");
TDCLDEF char TEXT147[NOSZ_] TINIT_("Narrowed Buffer..write it out");
TDCLDEF char TEXT148[NOSZ_] TINIT_("[Writing...]");
TDCLDEF char TEXT149[NOSZ_] TINIT_("[Wrote ");
TDCLDEF char TEXT150[NOSZ_] TINIT_(", saved as ");
TDCLDEF char TEXT151[NOSZ_] TINIT_("Name: ");
TDCLDEF char TEXT152[NOSZ_] TINIT_("[No such file]");
TDCLDEF char TEXT153[NOSZ_] TINIT_("[Inserting file]");
TDCLDEF char TEXT154[NOSZ_] TINIT_("Inserted ");
TDCLDEF char TEXT155[NOSZ_] TINIT_("Cannot open file for writing");
TDCLDEF char TEXT156[NOSZ_] TINIT_("Error closing file");
TDCLDEF char TEXT157[NOSZ_] TINIT_("Write I/O error");
TDCLDEF char TEXT158[NOSZ_] TINIT_("File read error");
TDCLDEF char TEXT159[NOSZ_] TINIT_("%Need function key number");
TDCLDEF char TEXT160[NOSZ_] TINIT_("%Function key number out of range");
TDCLDEF char TEXT161[NOSZ_] TINIT_("Label contents: ");
TDCLDEF char TEXT162[NOSZ_] TINIT_(" [y/n]? ");
TDCLDEF char TEXT163[NOSZ_] TINIT_("no default");
TDCLDEF char TEXT164[NOSZ_] TINIT_("[search failed]");
TDCLDEF char TEXT165[NOSZ_] TINIT_("ISearch: ");
TDCLDEF char TEXT166[NOSZ_] TINIT_("? Search string too long");
TDCLDEF char TEXT167[NOSZ_] TINIT_("? command too long");
TDCLDEF char TEXT168[NOSZ_] TINIT_("%%Can not insert string");
TDCLDEF char TEXT169[NOSZ_] TINIT_("Inserted");      /* this not used anymore */
TDCLDEF char TEXT170[NOSZ_] TINIT_("bug: linsert");
TDCLDEF char TEXT171[NOSZ_] TINIT_("Replaced");      /* this not used anymore */
TDCLDEF char TEXT172[NOSZ_] TINIT_("%%Out of memory while overwriting");
TDCLDEF char TEXT173[NOSZ_] TINIT_("LOCK ERROR: Lock table full");
TDCLDEF char TEXT174[NOSZ_] TINIT_("Cannot lock, out of memory");
TDCLDEF char TEXT175[NOSZ_] TINIT_("LOCK");
TDCLDEF char TEXT176[NOSZ_] TINIT_("File in use by ");
TDCLDEF char TEXT177[NOSZ_] TINIT_(", overide?");
TDCLDEF char TEXT178[NOSZ_] TINIT_("[can not get system error message]");
TDCLDEF char TEXT179[NOSZ_] TINIT_("  About MicroEmacs");
TDCLDEF char TEXT180[NOSZ_] TINIT_("%%No such resolution");
TDCLDEF char TEXT181[NOSZ_] TINIT_("%%Resolution illegal for this monitor");
TDCLDEF char TEXT182[NOSZ_] TINIT_("Environment variable TERM not defined!");
TDCLDEF char TEXT183[NOSZ_] TINIT_("Unknown terminal type %s!");
TDCLDEF char TEXT184[NOSZ_] TINIT_("termcap entry incomplete (lines)");
TDCLDEF char TEXT185[NOSZ_] TINIT_("Termcap entry incomplete (columns)");
TDCLDEF char TEXT186[NOSZ_] TINIT_("Incomplete termcap entry\n");
TDCLDEF char TEXT187[NOSZ_] TINIT_("Terminal description too big!\n");
TDCLDEF char TEXT188[NOSZ_] TINIT_("[End]");
TDCLDEF char TEXT189[NOSZ_] TINIT_("Cannot find entry for terminal type.\n");
TDCLDEF char TEXT190[NOSZ_] TINIT_("Check terminal type with \"SHOW TERMINAL\" or\n");
TDCLDEF char TEXT191[NOSZ_] TINIT_("try setting with \"SET TERMINAL/INQUIRE\"\n");
TDCLDEF char TEXT192[NOSZ_] TINIT_("The terminal type does not have enough power to run\n");
TDCLDEF char TEXT193[NOSZ_] TINIT_("MicroEMACS.  Try a different terminal or check\n");
TDCLDEF char TEXT194[NOSZ_] TINIT_("type with \"SHOW TERMINAL\".\n");
TDCLDEF char TEXT195[NOSZ_] TINIT_("Cannot open channel to terminal.\n");
TDCLDEF char TEXT196[NOSZ_] TINIT_("Cannot obtain terminal settings.\n");
TDCLDEF char TEXT197[NOSZ_] TINIT_("Cannot modify terminal settings.\n");
TDCLDEF char TEXT198[NOSZ_] TINIT_("I/O error (%d,%d)\n");
TDCLDEF char TEXT199[NOSZ_] TINIT_("[Starting DCL]\r\n");
TDCLDEF char TEXT200[NOSZ_] TINIT_("[Calling DCL]\r\n");
TDCLDEF char TEXT201[NOSZ_] TINIT_("[Not available yet under VMS]");
TDCLDEF char TEXT202[NOSZ_] TINIT_("Terminal type not 'vt52'or 'z19' !");
TDCLDEF char TEXT203[NOSZ_] TINIT_("Window number out of range");
TDCLDEF char TEXT204[NOSZ_] TINIT_("Can not delete this window");
TDCLDEF char TEXT205[NOSZ_] TINIT_("Cannot split a %d line window");
TDCLDEF char TEXT206[NOSZ_] TINIT_("Only one window");
TDCLDEF char TEXT207[NOSZ_] TINIT_("Impossible change");
TDCLDEF char TEXT208[NOSZ_] TINIT_("[No such window exists]");
TDCLDEF char TEXT209[NOSZ_] TINIT_("%%Screen size out of range");
TDCLDEF char TEXT210[NOSZ_] TINIT_("%%Screen width out of range");
TDCLDEF char TEXT211[NOSZ_] TINIT_("Function list");
TDCLDEF char TEXT212[NOSZ_] TINIT_("Can not display function list");
TDCLDEF char TEXT213[NOSZ_] TINIT_("[Building function list]");
TDCLDEF char TEXT214[NOSZ_] TINIT_("%%No such file as %s");
TDCLDEF char TEXT215[NOSZ_] TINIT_(": macro-to-key ");
TDCLDEF char TEXT216[NOSZ_] TINIT_("Cannot read/write directories!!!");
TDCLDEF char TEXT217[NOSZ_] TINIT_("[Not available yet under AOS/VS]");
TDCLDEF char TEXT218[NOSZ_] TINIT_("Append file: ");
TDCLDEF char TEXT219[NOSZ_] TINIT_("%%Macro Failed");
TDCLDEF char TEXT220[NOSZ_] TINIT_("Line %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x%x");
TDCLDEF char TEXT221[NOSZ_] TINIT_("Too many groups");
TDCLDEF char TEXT222[NOSZ_] TINIT_("Group not ended");
TDCLDEF char TEXT223[NOSZ_] TINIT_("%%Column origin out of range");
TDCLDEF char TEXT224[NOSZ_] TINIT_("%%Row origin out of range");
TDCLDEF char TEXT225[NOSZ_] TINIT_("[Switched to screen %s]");
TDCLDEF char TEXT226[NOSZ_] TINIT_("%%Can not kill an executing buffer");
TDCLDEF char TEXT227[NOSZ_] TINIT_("\n--- Press any key to Continue ---");
TDCLDEF char TEXT228[NOSZ_] TINIT_("[Kill ring cleared]");
TDCLDEF char TEXT229[NOSZ_] TINIT_(" in < ");
TDCLDEF char TEXT230[NOSZ_] TINIT_("> at line ");
TDCLDEF char TEXT231[NOSZ_] TINIT_("Abbreviation to set: ");
TDCLDEF char TEXT232[NOSZ_] TINIT_("Abbreviation to delete: ");
TDCLDEF char TEXT233[NOSZ_] TINIT_("[Building Abbreviation list]");
TDCLDEF char TEXT234[NOSZ_] TINIT_("Abbreviation list");
TDCLDEF char TEXT235[NOSZ_] TINIT_("Can not display abbreviation list");
TDCLDEF char TEXT236[NOSZ_] TINIT_("Define Abbreviations in buffer");
TDCLDEF char TEXT240[NOSZ_] TINIT_("[No such screen]");
TDCLDEF char TEXT241[NOSZ_] TINIT_("%%Can't delete current screen");
TDCLDEF char TEXT242[NOSZ_] TINIT_("Find Screen: ");
TDCLDEF char TEXT243[NOSZ_] TINIT_("Delete Screen: ");
TDCLDEF char TEXT244[NOSZ_] TINIT_("%%No such function as '%s'");
TDCLDEF char TEXT245[NOSZ_] TINIT_("%%Division by Zero is illegal");
TDCLDEF char TEXT246[NOSZ_] TINIT_("%%Need function key number");
TDCLDEF char TEXT247[NOSZ_] TINIT_("%%Function key number out of range");
TDCLDEF char TEXT248[NOSZ_] TINIT_("Enter Label String: ");
TDCLDEF char TEXT249[NOSZ_] TINIT_("Global variable to declare: ");
TDCLDEF char TEXT250[NOSZ_] TINIT_("Local variable to declare: ");

/* some of these are just used in the microsoft windows version */
TDCLDEF char TEXT300[NOSZ_] TINIT_("[Incorrect menu]");
TDCLDEF char TEXT301[NOSZ_] TINIT_("[Too many nested popup menus]");
TDCLDEF char TEXT302[NOSZ_] TINIT_("[Lack of resources]");
TDCLDEF char TEXT303[NOSZ_] TINIT_("Menu: ");
TDCLDEF char TEXT304[NOSZ_] TINIT_("Function: ");
TDCLDEF char TEXT305[NOSZ_] TINIT_("Macro: ");
TDCLDEF char TEXT306[NOSZ_] TINIT_("Menu: ");
TDCLDEF char TEXT307[NOSZ_] TINIT_("Help file: ");
TDCLDEF char TEXT308[NOSZ_] TINIT_("Help key: ");
TDCLDEF char TEXT310[NOSZ_] TINIT_("Alt+");
TDCLDEF char TEXT311[NOSZ_] TINIT_("Shift+");
TDCLDEF char TEXT312[NOSZ_] TINIT_("BkSp");
TDCLDEF char TEXT313[NOSZ_] TINIT_("Tab");
TDCLDEF char TEXT314[NOSZ_] TINIT_("Enter");
TDCLDEF char TEXT315[NOSZ_] TINIT_("Esc");
TDCLDEF char TEXT316[NOSZ_] TINIT_("Ctrl+");
TDCLDEF char TEXT317[NOSZ_] TINIT_("Home");
TDCLDEF char TEXT318[NOSZ_] TINIT_("DownArrow");
TDCLDEF char TEXT319[NOSZ_] TINIT_("UpArrow");
TDCLDEF char TEXT320[NOSZ_] TINIT_("LeftArrow");
TDCLDEF char TEXT321[NOSZ_] TINIT_("RightArrow");
TDCLDEF char TEXT322[NOSZ_] TINIT_("End");
TDCLDEF char TEXT323[NOSZ_] TINIT_("PageUp");
TDCLDEF char TEXT324[NOSZ_] TINIT_("PageDown");
TDCLDEF char TEXT325[NOSZ_] TINIT_("Ins");
TDCLDEF char TEXT326[NOSZ_] TINIT_("Del");
TDCLDEF char TEXT327[NOSZ_] TINIT_("F10");
#define CHAR328 'F'
TDCLDEF char TEXT329[NOSZ_] TINIT_("SpaceBar");
TDCLDEF char TEXT330[NOSZ_] TINIT_(" - Message history");
TDCLDEF char TEXT331[NOSZ_] TINIT_("Global modes");
TDCLDEF char TEXT332[NOSZ_] TINIT_("Modes for buffer: ");
TDCLDEF char TEXT333[NOSZ_] TINIT_("File write in progress. Quit later!");
TDCLDEF char TEXT334[NOSZ_] TINIT_("[No such directory]");
TDCLDEF char TEXT335[NOSZ_] TINIT_("Change screen name to: ");
TDCLDEF char TEXT336[NOSZ_] TINIT_("[Screen name already in use]");
TDCLDEF char TEXT337[NOSZ_] TINIT_("cannot monitor external program");



/*====================================================================*/
#endif/**#ifndef ENGLISH_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
