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

DCLDEF NOSHARE char TEXT1[NOSZ_]    INIT_("[Starting new CLI]");
DCLDEF NOSHARE char TEXT2[NOSZ_]    INIT_("[Cannot write filter file]");
DCLDEF NOSHARE char TEXT3[NOSZ_]    INIT_("[Execution failed]");
DCLDEF NOSHARE char TEXT4[NOSZ_]    INIT_("Shell variable TERM not defined!");
DCLDEF NOSHARE char TEXT5[NOSZ_]    INIT_("Terminal type not 'vt100'!");
DCLDEF NOSHARE char TEXT6[NOSZ_]    INIT_("\r\n\n[End]");
DCLDEF NOSHARE char TEXT7[NOSZ_]    INIT_("Line to GOTO: ");
DCLDEF NOSHARE char TEXT8[NOSZ_]    INIT_("[Aborted]");
DCLDEF NOSHARE char TEXT9[NOSZ_]    INIT_("[Mark %d set]");
DCLDEF NOSHARE char TEXT10[NOSZ_]   INIT_("[Mark %d removed]");
DCLDEF NOSHARE char TEXT11[NOSZ_]   INIT_("No mark %d in this window");
DCLDEF NOSHARE char TEXT12[NOSZ_]   INIT_("[Help file is not online]");
DCLDEF NOSHARE char TEXT13[NOSZ_]   INIT_(": describe-key ");
DCLDEF NOSHARE char TEXT14[NOSZ_]   INIT_("Not Bound");
DCLDEF NOSHARE char TEXT15[NOSZ_]   INIT_(": bind-to-key ");
DCLDEF NOSHARE char TEXT16[NOSZ_]   INIT_("[No such function]");
DCLDEF NOSHARE char TEXT17[NOSZ_]   INIT_("Binding table FULL!");
DCLDEF NOSHARE char TEXT18[NOSZ_]   INIT_(": unbind-key ");
DCLDEF NOSHARE char TEXT19[NOSZ_]   INIT_("[Key not bound]");
DCLDEF NOSHARE char TEXT20[NOSZ_]   INIT_("Apropos string: ");
DCLDEF NOSHARE char TEXT21[NOSZ_]   INIT_("Binding list");
DCLDEF NOSHARE char TEXT22[NOSZ_]   INIT_("Can not display binding list");
DCLDEF NOSHARE char TEXT23[NOSZ_]   INIT_("[Building binding list]");
DCLDEF NOSHARE char TEXT24[NOSZ_]   INIT_("Use buffer");
DCLDEF NOSHARE char TEXT25[NOSZ_]   INIT_("Use buffer: ");
DCLDEF NOSHARE char TEXT26[NOSZ_]   INIT_("Kill buffer");
DCLDEF NOSHARE char TEXT27[NOSZ_]   INIT_("Pop buffer");
DCLDEF NOSHARE char TEXT28[NOSZ_]   INIT_("Buffer is being displayed");
DCLDEF NOSHARE char TEXT29[NOSZ_]   INIT_("Change buffer name to: ");
DCLDEF NOSHARE char TEXT30[NOSZ_]   INIT_("ACTN   Modes        Size Buffer          File");
DCLDEF NOSHARE char TEXT31[NOSZ_]   INIT_("         Global Modes");
DCLDEF NOSHARE char TEXT32[NOSZ_]   INIT_("Discard changes");
DCLDEF NOSHARE char TEXT33[NOSZ_]   INIT_("Encryption String: ");
DCLDEF NOSHARE char TEXT34[NOSZ_]   INIT_("File: ");
DCLDEF NOSHARE char TEXT35[NOSZ_]   INIT_("another user");
DCLDEF NOSHARE char TEXT36[NOSZ_]   INIT_("LOCK ERROR -- ");
DCLDEF NOSHARE char TEXT37[NOSZ_]   INIT_("checking for existence of %s\n");
DCLDEF NOSHARE char TEXT38[NOSZ_]   INIT_("making directory %s\n");
DCLDEF NOSHARE char TEXT39[NOSZ_]   INIT_("creating %s\n");
DCLDEF NOSHARE char TEXT40[NOSZ_]   INIT_("could not create lock file");
DCLDEF NOSHARE char TEXT41[NOSZ_]   INIT_("pid is %ld\n");
DCLDEF NOSHARE char TEXT42[NOSZ_]   INIT_("reading lock file %s\n");
DCLDEF NOSHARE char TEXT43[NOSZ_]   INIT_("could not read lock file");
DCLDEF NOSHARE char TEXT44[NOSZ_]   INIT_("pid in %s is %ld\n");
DCLDEF NOSHARE char TEXT45[NOSZ_]   INIT_("signaling process %ld\n");
DCLDEF NOSHARE char TEXT46[NOSZ_]   INIT_("process exists");
DCLDEF NOSHARE char TEXT47[NOSZ_]   INIT_("kill was bad");
DCLDEF NOSHARE char TEXT48[NOSZ_]   INIT_("kill was good; process exists");
DCLDEF NOSHARE char TEXT49[NOSZ_]   INIT_("attempting to unlink %s\n");
DCLDEF NOSHARE char TEXT50[NOSZ_]   INIT_("could not remove lock file");
DCLDEF NOSHARE char TEXT51[NOSZ_]   INIT_("Variable to set: ");
DCLDEF NOSHARE char TEXT52[NOSZ_]   INIT_("%%No such variable as '%s'");
DCLDEF NOSHARE char TEXT53[NOSZ_]   INIT_("Value: ");
DCLDEF NOSHARE char TEXT54[NOSZ_]   INIT_("[Macro aborted]");
DCLDEF NOSHARE char TEXT55[NOSZ_]   INIT_("Variable to display: ");
DCLDEF NOSHARE char TEXT56[NOSZ_]   INIT_("Variable list");
DCLDEF NOSHARE char TEXT57[NOSZ_]   INIT_("Can not display variable list");
DCLDEF NOSHARE char TEXT58[NOSZ_]   INIT_("[Building variable list]");
DCLDEF NOSHARE char TEXT59[NOSZ_]   INIT_("");                  /* UNUSED */
DCLDEF NOSHARE char TEXT60[NOSZ_]   INIT_("Line %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x");
DCLDEF NOSHARE char TEXT61[NOSZ_]   INIT_("<Not Used>");
DCLDEF NOSHARE char TEXT62[NOSZ_]   INIT_("Global mode to ");
DCLDEF NOSHARE char TEXT63[NOSZ_]   INIT_("Mode to ");
DCLDEF NOSHARE char TEXT64[NOSZ_]   INIT_("add: ");
DCLDEF NOSHARE char TEXT65[NOSZ_]   INIT_("delete: ");
DCLDEF NOSHARE char TEXT66[NOSZ_]   INIT_("No such mode!");
DCLDEF NOSHARE char TEXT67[NOSZ_]   INIT_("Message to write: ");
DCLDEF NOSHARE char TEXT68[NOSZ_]   INIT_("String to insert: ");
DCLDEF NOSHARE char TEXT69[NOSZ_]   INIT_("String to overwrite: ");
DCLDEF NOSHARE char TEXT70[NOSZ_]   INIT_("[region copied]");
DCLDEF NOSHARE char TEXT71[NOSZ_]   INIT_("%%This buffer is already narrowed");
DCLDEF NOSHARE char TEXT72[NOSZ_]   INIT_("%%Must narrow at least 1 full line");
DCLDEF NOSHARE char TEXT73[NOSZ_]   INIT_("[Buffer is narrowed]");
DCLDEF NOSHARE char TEXT74[NOSZ_]   INIT_("%%This buffer is not narrowed");
DCLDEF NOSHARE char TEXT75[NOSZ_]   INIT_("[Buffer is widened]");
DCLDEF NOSHARE char TEXT76[NOSZ_]   INIT_("No mark set in this window");
DCLDEF NOSHARE char TEXT77[NOSZ_]   INIT_("Bug: lost mark");
DCLDEF NOSHARE char TEXT78[NOSZ_]   INIT_("Search ");
DCLDEF NOSHARE char TEXT79[NOSZ_]   INIT_("Not found ");
DCLDEF NOSHARE char TEXT80[NOSZ_]   INIT_("No pattern set");
DCLDEF NOSHARE char TEXT81[NOSZ_]   INIT_("Reverse search ");
DCLDEF NOSHARE char TEXT82[NOSZ_]   INIT_("");                  /* UNUSED */
DCLDEF NOSHARE char TEXT83[NOSZ_]   INIT_("");                  /* UNUSED */
DCLDEF NOSHARE char TEXT84[NOSZ_]   INIT_("Replace ");
DCLDEF NOSHARE char TEXT85[NOSZ_]   INIT_("Query replace ");
DCLDEF NOSHARE char TEXT86[NOSZ_]   INIT_("with ");
DCLDEF NOSHARE char TEXT87[NOSZ_]   INIT_("Replace '");
DCLDEF NOSHARE char TEXT88[NOSZ_]   INIT_("' with '");
DCLDEF NOSHARE char TEXT89[NOSZ_]   INIT_("Aborted!");
DCLDEF NOSHARE char TEXT90[NOSZ_]   INIT_("(Y)es, (N)o, (!)Do rest, (U)ndo last, (^G)Abort, (.)Abort back, (?)Help: ");
DCLDEF NOSHARE char TEXT91[NOSZ_]   INIT_("Empty string replaced, stopping.");
DCLDEF NOSHARE char TEXT92[NOSZ_]   INIT_("%D substitutions");
DCLDEF NOSHARE char TEXT93[NOSZ_]   INIT_("%%ERROR while deleting");
DCLDEF NOSHARE char TEXT94[NOSZ_]   INIT_("%%Out of memory");
DCLDEF NOSHARE char TEXT95[NOSZ_]   INIT_("%%mceq: what is %d?");
DCLDEF NOSHARE char TEXT96[NOSZ_]   INIT_("%%No characters in character class");
DCLDEF NOSHARE char TEXT97[NOSZ_]   INIT_("%%Character class not ended");
DCLDEF NOSHARE char TEXT98[NOSZ_]   INIT_("No fill column set");
DCLDEF NOSHARE char TEXT99[NOSZ_]   INIT_("[OUT OF MEMORY]");   /* UNUSED */
DCLDEF NOSHARE char TEXT100[NOSZ_]  INIT_("Words %D Chars %D Lines %d Avg chars/word %f");
DCLDEF NOSHARE char TEXT101[NOSZ_]  INIT_("[Can not search and goto at the same time!]");
DCLDEF NOSHARE char TEXT102[NOSZ_]  INIT_("[Bogus goto argument]");
DCLDEF NOSHARE char TEXT103[NOSZ_]  INIT_("[Saving %s]");
DCLDEF NOSHARE char TEXT104[NOSZ_]  INIT_("Modified buffers exist. Leave anyway");
DCLDEF NOSHARE char TEXT105[NOSZ_]  INIT_("%%Macro already active");
DCLDEF NOSHARE char TEXT106[NOSZ_]  INIT_("[Start macro]");
DCLDEF NOSHARE char TEXT107[NOSZ_]  INIT_("%%Macro not active");
DCLDEF NOSHARE char TEXT108[NOSZ_]  INIT_("[End macro]");
DCLDEF NOSHARE char TEXT109[NOSZ_]  INIT_("[Key illegal in VIEW mode]");
DCLDEF NOSHARE char TEXT110[NOSZ_]  INIT_("[That command is RESTRICTED]");
DCLDEF NOSHARE char TEXT111[NOSZ_]  INIT_("No macro specified");
DCLDEF NOSHARE char TEXT112[NOSZ_]  INIT_("Macro number out of range");
DCLDEF NOSHARE char TEXT113[NOSZ_]  INIT_("Can not create macro");
DCLDEF NOSHARE char TEXT114[NOSZ_]  INIT_("Procedure name: ");
DCLDEF NOSHARE char TEXT115[NOSZ_]  INIT_("Execute procedure: ");
DCLDEF NOSHARE char TEXT116[NOSZ_]  INIT_("No such procedure");
DCLDEF NOSHARE char TEXT117[NOSZ_]  INIT_("Execute buffer: ");
DCLDEF NOSHARE char TEXT118[NOSZ_]  INIT_("No such buffer");
DCLDEF NOSHARE char TEXT119[NOSZ_]  INIT_("%%Out of memory during while scan");
DCLDEF NOSHARE char TEXT120[NOSZ_]  INIT_("%%!BREAK outside of any !WHILE loop");
DCLDEF NOSHARE char TEXT121[NOSZ_]  INIT_("%%!ENDWHILE with no preceding !WHILE");
DCLDEF NOSHARE char TEXT122[NOSZ_]  INIT_("%%!WHILE with no matching !ENDWHILE");
DCLDEF NOSHARE char TEXT123[NOSZ_]  INIT_("%%Out of Memory during macro execution");
DCLDEF NOSHARE char TEXT124[NOSZ_]  INIT_("%%Unknown Directive");
DCLDEF NOSHARE char TEXT125[NOSZ_]  INIT_("Out of memory while storing macro");
DCLDEF NOSHARE char TEXT126[NOSZ_]  INIT_("%%Internal While loop error");
DCLDEF NOSHARE char TEXT127[NOSZ_]  INIT_("%%No such label");
DCLDEF NOSHARE char TEXT128[NOSZ_]  INIT_("(e)val exp, (c/x)ommand, (t)rack exp, (^G)abort, <SP>exec, <META> stop debug");
DCLDEF NOSHARE char TEXT129[NOSZ_]  INIT_("File to execute: ");
DCLDEF NOSHARE char TEXT130[NOSZ_]  INIT_("Macro not defined");
DCLDEF NOSHARE char TEXT131[NOSZ_]  INIT_("Read file");
DCLDEF NOSHARE char TEXT132[NOSZ_]  INIT_("Insert file");
DCLDEF NOSHARE char TEXT133[NOSZ_]  INIT_("Find file");
DCLDEF NOSHARE char TEXT134[NOSZ_]  INIT_("View file");
DCLDEF NOSHARE char TEXT135[NOSZ_]  INIT_("[Old buffer]");
DCLDEF NOSHARE char TEXT136[NOSZ_]  INIT_("Buffer name: ");
DCLDEF NOSHARE char TEXT137[NOSZ_]  INIT_("Cannot create buffer");
DCLDEF NOSHARE char TEXT138[NOSZ_]  INIT_("[New file]");
DCLDEF NOSHARE char TEXT139[NOSZ_]  INIT_("[Reading file]");
DCLDEF NOSHARE char TEXT140[NOSZ_]  INIT_("Read ");
DCLDEF NOSHARE char TEXT141[NOSZ_]  INIT_("I/O ERROR, ");
DCLDEF NOSHARE char TEXT142[NOSZ_]  INIT_("OUT OF MEMORY, ");
DCLDEF NOSHARE char TEXT143[NOSZ_]  INIT_(" line");
DCLDEF NOSHARE char TEXT144[NOSZ_]  INIT_("Write file: ");
DCLDEF NOSHARE char TEXT145[NOSZ_]  INIT_("No file name");
DCLDEF NOSHARE char TEXT146[NOSZ_]  INIT_("Truncated file..write it out");
DCLDEF NOSHARE char TEXT147[NOSZ_]  INIT_("Narrowed Buffer..write it out");
DCLDEF NOSHARE char TEXT148[NOSZ_]  INIT_("[Writing...]");
DCLDEF NOSHARE char TEXT149[NOSZ_]  INIT_("[Wrote ");
DCLDEF NOSHARE char TEXT150[NOSZ_]  INIT_(", saved as ");
DCLDEF NOSHARE char TEXT151[NOSZ_]  INIT_("Name: ");
DCLDEF NOSHARE char TEXT152[NOSZ_]  INIT_("[No such file]");
DCLDEF NOSHARE char TEXT153[NOSZ_]  INIT_("[Inserting file]");
DCLDEF NOSHARE char TEXT154[NOSZ_]  INIT_("Inserted ");
DCLDEF NOSHARE char TEXT155[NOSZ_]  INIT_("Cannot open file for writing");
DCLDEF NOSHARE char TEXT156[NOSZ_]  INIT_("Error closing file");
DCLDEF NOSHARE char TEXT157[NOSZ_]  INIT_("Write I/O error");
DCLDEF NOSHARE char TEXT158[NOSZ_]  INIT_("File read error");
DCLDEF NOSHARE char TEXT159[NOSZ_]  INIT_("%Need function key number");
DCLDEF NOSHARE char TEXT160[NOSZ_]  INIT_("%Function key number out of range");
DCLDEF NOSHARE char TEXT161[NOSZ_]  INIT_("Label contents: ");
DCLDEF NOSHARE char TEXT162[NOSZ_]  INIT_(" [y/n]? ");
DCLDEF NOSHARE char TEXT163[NOSZ_]  INIT_("no default");
DCLDEF NOSHARE char TEXT164[NOSZ_]  INIT_("[search failed]");
DCLDEF NOSHARE char TEXT165[NOSZ_]  INIT_("ISearch: ");
DCLDEF NOSHARE char TEXT166[NOSZ_]  INIT_("? Search string too long");
DCLDEF NOSHARE char TEXT167[NOSZ_]  INIT_("? command too long");
DCLDEF NOSHARE char TEXT168[NOSZ_]  INIT_("%%Can not insert string");
DCLDEF NOSHARE char TEXT169[NOSZ_]  INIT_("Inserted");      /* this not used anymore */
DCLDEF NOSHARE char TEXT170[NOSZ_]  INIT_("bug: linsert");
DCLDEF NOSHARE char TEXT171[NOSZ_]  INIT_("Replaced");      /* this not used anymore */
DCLDEF NOSHARE char TEXT172[NOSZ_]  INIT_("%%Out of memory while overwriting");
DCLDEF NOSHARE char TEXT173[NOSZ_]  INIT_("LOCK ERROR: Lock table full");
DCLDEF NOSHARE char TEXT174[NOSZ_]  INIT_("Cannot lock, out of memory");
DCLDEF NOSHARE char TEXT175[NOSZ_]  INIT_("LOCK");
DCLDEF NOSHARE char TEXT176[NOSZ_]  INIT_("File in use by ");
DCLDEF NOSHARE char TEXT177[NOSZ_]  INIT_(", overide?");
DCLDEF NOSHARE char TEXT178[NOSZ_]  INIT_("[can not get system error message]");
DCLDEF NOSHARE char TEXT179[NOSZ_]  INIT_("  About MicroEmacs");
DCLDEF NOSHARE char TEXT180[NOSZ_]  INIT_("%%No such resolution");
DCLDEF NOSHARE char TEXT181[NOSZ_]  INIT_("%%Resolution illegal for this monitor");
DCLDEF NOSHARE char TEXT182[NOSZ_]  INIT_("Environment variable TERM not defined!");
DCLDEF NOSHARE char TEXT183[NOSZ_]  INIT_("Unknown terminal type %s!");
DCLDEF NOSHARE char TEXT184[NOSZ_]  INIT_("termcap entry incomplete (lines)");
DCLDEF NOSHARE char TEXT185[NOSZ_]  INIT_("Termcap entry incomplete (columns)");
DCLDEF NOSHARE char TEXT186[NOSZ_]  INIT_("Incomplete termcap entry\n");
DCLDEF NOSHARE char TEXT187[NOSZ_]  INIT_("Terminal description too big!\n");
DCLDEF NOSHARE char TEXT188[NOSZ_]  INIT_("[End]");
DCLDEF NOSHARE char TEXT189[NOSZ_]  INIT_("Cannot find entry for terminal type.\n");
DCLDEF NOSHARE char TEXT190[NOSZ_]  INIT_("Check terminal type with \"SHOW TERMINAL\" or\n");
DCLDEF NOSHARE char TEXT191[NOSZ_]  INIT_("try setting with \"SET TERMINAL/INQUIRE\"\n");
DCLDEF NOSHARE char TEXT192[NOSZ_]  INIT_("The terminal type does not have enough power to run\n");
DCLDEF NOSHARE char TEXT193[NOSZ_]  INIT_("MicroEMACS.  Try a different terminal or check\n");
DCLDEF NOSHARE char TEXT194[NOSZ_]  INIT_("type with \"SHOW TERMINAL\".\n");
DCLDEF NOSHARE char TEXT195[NOSZ_]  INIT_("Cannot open channel to terminal.\n");
DCLDEF NOSHARE char TEXT196[NOSZ_]  INIT_("Cannot obtain terminal settings.\n");
DCLDEF NOSHARE char TEXT197[NOSZ_]  INIT_("Cannot modify terminal settings.\n");
DCLDEF NOSHARE char TEXT198[NOSZ_]  INIT_("I/O error (%d,%d)\n");
DCLDEF NOSHARE char TEXT199[NOSZ_]  INIT_("[Starting DCL]\r\n");
DCLDEF NOSHARE char TEXT200[NOSZ_]  INIT_("[Calling DCL]\r\n");
DCLDEF NOSHARE char TEXT201[NOSZ_]  INIT_("[Not available yet under VMS]");
DCLDEF NOSHARE char TEXT202[NOSZ_]  INIT_("Terminal type not 'vt52'or 'z19' !");
DCLDEF NOSHARE char TEXT203[NOSZ_]  INIT_("Window number out of range");
DCLDEF NOSHARE char TEXT204[NOSZ_]  INIT_("Can not delete this window");
DCLDEF NOSHARE char TEXT205[NOSZ_]  INIT_("Cannot split a %d line window");
DCLDEF NOSHARE char TEXT206[NOSZ_]  INIT_("Only one window");
DCLDEF NOSHARE char TEXT207[NOSZ_]  INIT_("Impossible change");
DCLDEF NOSHARE char TEXT208[NOSZ_]  INIT_("[No such window exists]");
DCLDEF NOSHARE char TEXT209[NOSZ_]  INIT_("%%Screen size out of range");
DCLDEF NOSHARE char TEXT210[NOSZ_]  INIT_("%%Screen width out of range");
DCLDEF NOSHARE char TEXT211[NOSZ_]  INIT_("Function list");
DCLDEF NOSHARE char TEXT212[NOSZ_]  INIT_("Can not display function list");
DCLDEF NOSHARE char TEXT213[NOSZ_]  INIT_("[Building function list]");
DCLDEF NOSHARE char TEXT214[NOSZ_]  INIT_("%%No such file as %s");
DCLDEF NOSHARE char TEXT215[NOSZ_]  INIT_(": macro-to-key ");
DCLDEF NOSHARE char TEXT216[NOSZ_]  INIT_("Cannot read/write directories!!!");
DCLDEF NOSHARE char TEXT217[NOSZ_]  INIT_("[Not available yet under AOS/VS]");
DCLDEF NOSHARE char TEXT218[NOSZ_]  INIT_("Append file: ");
DCLDEF NOSHARE char TEXT219[NOSZ_]  INIT_("%%Macro Failed");
DCLDEF NOSHARE char TEXT220[NOSZ_]  INIT_("Line %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x%x");
DCLDEF NOSHARE char TEXT221[NOSZ_]  INIT_("Too many groups");
DCLDEF NOSHARE char TEXT222[NOSZ_]  INIT_("Group not ended");
DCLDEF NOSHARE char TEXT223[NOSZ_]  INIT_("%%Column origin out of range");
DCLDEF NOSHARE char TEXT224[NOSZ_]  INIT_("%%Row origin out of range");
DCLDEF NOSHARE char TEXT225[NOSZ_]  INIT_("[Switched to screen %s]");
DCLDEF NOSHARE char TEXT226[NOSZ_]  INIT_("%%Can not kill an executing buffer");
DCLDEF NOSHARE char TEXT227[NOSZ_]  INIT_("\n--- Press any key to Continue ---");
DCLDEF NOSHARE char TEXT228[NOSZ_]  INIT_("[Kill ring cleared]");
DCLDEF NOSHARE char TEXT229[NOSZ_]  INIT_(" in < ");
DCLDEF NOSHARE char TEXT230[NOSZ_]  INIT_("> at line ");
DCLDEF NOSHARE char TEXT231[NOSZ_]  INIT_("Abbreviation to set: ");
DCLDEF NOSHARE char TEXT232[NOSZ_]  INIT_("Abbreviation to delete: ");
DCLDEF NOSHARE char TEXT233[NOSZ_]  INIT_("[Building Abbreviation list]");
DCLDEF NOSHARE char TEXT234[NOSZ_]  INIT_("Abbreviation list");
DCLDEF NOSHARE char TEXT235[NOSZ_]  INIT_("Can not display abbreviation list");
DCLDEF NOSHARE char TEXT236[NOSZ_]  INIT_("Define Abbreviations in buffer");
DCLDEF NOSHARE char TEXT240[NOSZ_]  INIT_("[No such screen]");
DCLDEF NOSHARE char TEXT241[NOSZ_]  INIT_("%%Can't delete current screen");
DCLDEF NOSHARE char TEXT242[NOSZ_]  INIT_("Find Screen: ");
DCLDEF NOSHARE char TEXT243[NOSZ_]  INIT_("Delete Screen: ");
DCLDEF NOSHARE char TEXT244[NOSZ_]  INIT_("%%No such function as '%s'");
DCLDEF NOSHARE char TEXT245[NOSZ_]  INIT_("%%Division by Zero is illegal");
DCLDEF NOSHARE char TEXT246[NOSZ_]  INIT_("%%Need function key number");
DCLDEF NOSHARE char TEXT247[NOSZ_]  INIT_("%%Function key number out of range");
DCLDEF NOSHARE char TEXT248[NOSZ_]  INIT_("Enter Label String: ");
DCLDEF NOSHARE char TEXT249[NOSZ_]  INIT_("Global variable to declare: ");
DCLDEF NOSHARE char TEXT250[NOSZ_]  INIT_("Local variable to declare: ");

/* some of these are just used in the microsoft windows version */
DCLDEF NOSHARE char TEXT300[NOSZ_]  INIT_("[Incorrect menu]");
DCLDEF NOSHARE char TEXT301[NOSZ_]  INIT_("[Too many nested popup menus]");
DCLDEF NOSHARE char TEXT302[NOSZ_]  INIT_("[Lack of resources]");
DCLDEF NOSHARE char TEXT303[NOSZ_]  INIT_("Menu: ");
DCLDEF NOSHARE char TEXT304[NOSZ_]  INIT_("Function: ");
DCLDEF NOSHARE char TEXT305[NOSZ_]  INIT_("Macro: ");
DCLDEF NOSHARE char TEXT306[NOSZ_]  INIT_("Menu: ");
DCLDEF NOSHARE char TEXT307[NOSZ_]  INIT_("Help file: ");
DCLDEF NOSHARE char TEXT308[NOSZ_]  INIT_("Help key: ");
DCLDEF NOSHARE char TEXT310[NOSZ_]  INIT_("Alt+");
DCLDEF NOSHARE char TEXT311[NOSZ_]  INIT_("Shift+");
DCLDEF NOSHARE char TEXT312[NOSZ_]  INIT_("BkSp");
DCLDEF NOSHARE char TEXT313[NOSZ_]  INIT_("Tab");
DCLDEF NOSHARE char TEXT314[NOSZ_]  INIT_("Enter");
DCLDEF NOSHARE char TEXT315[NOSZ_]  INIT_("Esc");
DCLDEF NOSHARE char TEXT316[NOSZ_]  INIT_("Ctrl+");
DCLDEF NOSHARE char TEXT317[NOSZ_]  INIT_("Home");
DCLDEF NOSHARE char TEXT318[NOSZ_]  INIT_("DownArrow");
DCLDEF NOSHARE char TEXT319[NOSZ_]  INIT_("UpArrow");
DCLDEF NOSHARE char TEXT320[NOSZ_]  INIT_("LeftArrow");
DCLDEF NOSHARE char TEXT321[NOSZ_]  INIT_("RightArrow");
DCLDEF NOSHARE char TEXT322[NOSZ_]  INIT_("End");
DCLDEF NOSHARE char TEXT323[NOSZ_]  INIT_("PageUp");
DCLDEF NOSHARE char TEXT324[NOSZ_]  INIT_("PageDown");
DCLDEF NOSHARE char TEXT325[NOSZ_]  INIT_("Ins");
DCLDEF NOSHARE char TEXT326[NOSZ_]  INIT_("Del");
DCLDEF NOSHARE char TEXT327[NOSZ_]  INIT_("F10");
#define CHAR328 'F'
DCLDEF NOSHARE char TEXT329[NOSZ_]  INIT_("SpaceBar");
DCLDEF NOSHARE char TEXT330[NOSZ_]  INIT_(" - Message history");
DCLDEF NOSHARE char TEXT331[NOSZ_]  INIT_("Global modes");
DCLDEF NOSHARE char TEXT332[NOSZ_]  INIT_("Modes for buffer: ");
DCLDEF NOSHARE char TEXT333[NOSZ_]  INIT_("File write in progress. Quit later!");
DCLDEF NOSHARE char TEXT334[NOSZ_]  INIT_("[No such directory]");
DCLDEF NOSHARE char TEXT335[NOSZ_]  INIT_("Change screen name to: ");
DCLDEF NOSHARE char TEXT336[NOSZ_]  INIT_("[Screen name already in use]");
DCLDEF NOSHARE char TEXT337[NOSZ_]  INIT_("cannot monitor external program");



/*====================================================================*/
#endif/**#ifndef ENGLISH_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
