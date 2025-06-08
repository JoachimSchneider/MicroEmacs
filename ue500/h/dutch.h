/*======================================================================
 *      NEDERLANDS.H:   Dutch language text strings for
 *                      MicroEMACS 4.00
 *
 *                      Translated by Paul De Bra
 *                      debra@research.att.com
 *                      (C)opyright 1995 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#ifndef DUTCH_H_
#define DUTCH_H_
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

TDCLDEF char TEXT1[NOSZ_]   TINIT_("[Opstarten nieuwe CLI]");
TDCLDEF char TEXT2[NOSZ_]   TINIT_("[Kan niet schrijven in filter file]");
TDCLDEF char TEXT3[NOSZ_]   TINIT_("[Uitvoering mislukt]");
TDCLDEF char TEXT4[NOSZ_]   TINIT_("Shell variabele TERM niet gedefinieerd!");
TDCLDEF char TEXT5[NOSZ_]   TINIT_("Terminal type is geen 'vt100'!");
TDCLDEF char TEXT6[NOSZ_]   TINIT_("\r\n\n[Einde]");
TDCLDEF char TEXT7[NOSZ_]   TINIT_("Lijn om naar te springen: ");
TDCLDEF char TEXT8[NOSZ_]   TINIT_("[Onderbroken]");
TDCLDEF char TEXT9[NOSZ_]   TINIT_("[Markering %d gezet]");
TDCLDEF char TEXT10[NOSZ_]  TINIT_("[Markering %d verwijderd]");
TDCLDEF char TEXT11[NOSZ_]  TINIT_("Geen markering %d in dit venster");
TDCLDEF char TEXT12[NOSZ_]  TINIT_("[Hulp file is niet aanwezig]");
TDCLDEF char TEXT13[NOSZ_]  TINIT_(": beschrijf-toets ");
TDCLDEF char TEXT14[NOSZ_]  TINIT_("Niet gebonden");
TDCLDEF char TEXT15[NOSZ_]  TINIT_(": bind-aan-toets ");
TDCLDEF char TEXT16[NOSZ_]  TINIT_("[Onbestaande functie]");
TDCLDEF char TEXT17[NOSZ_]  TINIT_("Verbindings-tabel VOL!");
TDCLDEF char TEXT18[NOSZ_]  TINIT_(": verwijder verbinding-toets ");
TDCLDEF char TEXT19[NOSZ_]  TINIT_("[Toets niet gebonden]");
TDCLDEF char TEXT20[NOSZ_]  TINIT_("Beschrijf woord: ");
TDCLDEF char TEXT21[NOSZ_]  TINIT_("Verbindings lijst");
TDCLDEF char TEXT22[NOSZ_]  TINIT_("Kan verbindings-lijst niet tonen");
TDCLDEF char TEXT23[NOSZ_]  TINIT_("[Opbouwen van verbindings-lijst]");
TDCLDEF char TEXT24[NOSZ_]  TINIT_("Gebruik buffer");
TDCLDEF char TEXT25[NOSZ_]  TINIT_("Gebruik buffer: ");
TDCLDEF char TEXT26[NOSZ_]  TINIT_("Vernietig buffer");
TDCLDEF char TEXT27[NOSZ_]  TINIT_("Pop buffer");
TDCLDEF char TEXT28[NOSZ_]  TINIT_("Buffer wordt nog getoond");
TDCLDEF char TEXT29[NOSZ_]  TINIT_("Verander buffer-naam in: ");
TDCLDEF char TEXT30[NOSZ_]  TINIT_("ACTN   Modus   Grootte Buffer          File");
/*   ^^^The underlining will not match exactly for this line, but it's close*/
TDCLDEF char TEXT31[NOSZ_]  TINIT_("         Globale Modi");
TDCLDEF char TEXT32[NOSZ_]  TINIT_("Vergeet Veranderingen");
TDCLDEF char TEXT33[NOSZ_]  TINIT_("Encryptie Sleutel: ");
TDCLDEF char TEXT34[NOSZ_]  TINIT_("File: ");
TDCLDEF char TEXT35[NOSZ_]  TINIT_("een andere gebruiker");
/*The UNIX terms should stay the same in the next few entryies*/
TDCLDEF char TEXT36[NOSZ_]  TINIT_("LOCK FOUT -- ");
TDCLDEF char TEXT37[NOSZ_]  TINIT_("test naar het bestaan van %s\n");
TDCLDEF char TEXT38[NOSZ_]  TINIT_("creeren van directory %s\n");
TDCLDEF char TEXT39[NOSZ_]  TINIT_("creeren van %s\n");
TDCLDEF char TEXT40[NOSZ_]  TINIT_("kon lock-file niet creeren");
TDCLDEF char TEXT41[NOSZ_]  TINIT_("proces nummer is %ld\n");
TDCLDEF char TEXT42[NOSZ_]  TINIT_("lezen van lock-file %s\n");
TDCLDEF char TEXT43[NOSZ_]  TINIT_("kon lock-file niet lezen");
TDCLDEF char TEXT44[NOSZ_]  TINIT_("process nummer in %s is %ld\n");
TDCLDEF char TEXT45[NOSZ_]  TINIT_("signaal gestuurd naar proces %ld\n");
TDCLDEF char TEXT46[NOSZ_]  TINIT_("proces bestaat");
TDCLDEF char TEXT47[NOSZ_]  TINIT_("signaleren mislukt");
TDCLDEF char TEXT48[NOSZ_]  TINIT_("signaleren gelukt; proces bestaat");
TDCLDEF char TEXT49[NOSZ_]  TINIT_("poging om %s te verwijderen\n");
TDCLDEF char TEXT50[NOSZ_]  TINIT_("kon lock-file niet verwijderen");
TDCLDEF char TEXT51[NOSZ_]  TINIT_("Variabele om te zetten: ");
TDCLDEF char TEXT52[NOSZ_]  TINIT_("%%Er bestaat geen variabele '%s'");
TDCLDEF char TEXT53[NOSZ_]  TINIT_("Waarde: ");
TDCLDEF char TEXT54[NOSZ_]  TINIT_("[Macro onderbroken]");
TDCLDEF char TEXT55[NOSZ_]  TINIT_("Variabele om te tonen: ");
TDCLDEF char TEXT56[NOSZ_]  TINIT_("Lijst van variabelen");
TDCLDEF char TEXT57[NOSZ_]  TINIT_("Kan lijst van variabelen niet tonen");
TDCLDEF char TEXT58[NOSZ_]  TINIT_("[Opbouwen lijst van variabelen]");
TDCLDEF char TEXT59[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT60[NOSZ_]  TINIT_("Lijn %D/%D Kolom %d/%d Kar. %D/%D (%d%%) Kar. = 0x%x");
TDCLDEF char TEXT61[NOSZ_]  TINIT_("<NOT USED>");
TDCLDEF char TEXT62[NOSZ_]  TINIT_("Globale modus om ");
TDCLDEF char TEXT63[NOSZ_]  TINIT_("Modus om ");
TDCLDEF char TEXT64[NOSZ_]  TINIT_("toe te voegen: ");
TDCLDEF char TEXT65[NOSZ_]  TINIT_("te verwijderen: ");
TDCLDEF char TEXT66[NOSZ_]  TINIT_("onbestaande modus!");
TDCLDEF char TEXT67[NOSZ_]  TINIT_("Te schrijven bericht: ");
TDCLDEF char TEXT68[NOSZ_]  TINIT_("In te voegen woord: ");
TDCLDEF char TEXT69[NOSZ_]  TINIT_("Te overschrijven woord: ");
TDCLDEF char TEXT70[NOSZ_]  TINIT_("[geselecteerd stuk gecopieerd]");
TDCLDEF char TEXT71[NOSZ_]  TINIT_("%%Deze buffer is al vernauwd");
TDCLDEF char TEXT72[NOSZ_]  TINIT_("%%Moet minstens 1 hele lijn vernauwen");
TDCLDEF char TEXT73[NOSZ_]  TINIT_("[Buffer is vernauwd]");
TDCLDEF char TEXT74[NOSZ_]  TINIT_("%%Deze buffer is niet vernauwd");
TDCLDEF char TEXT75[NOSZ_]  TINIT_("[Buffer is verbreed]");
TDCLDEF char TEXT76[NOSZ_]  TINIT_("Geen markering gezet in dit venster");
TDCLDEF char TEXT77[NOSZ_]  TINIT_("Interne Fout: markering verloren");
TDCLDEF char TEXT78[NOSZ_]  TINIT_("Zoek");
TDCLDEF char TEXT79[NOSZ_]  TINIT_("Niet gevonden");
TDCLDEF char TEXT80[NOSZ_]  TINIT_("Geen patroon gezet");
TDCLDEF char TEXT81[NOSZ_]  TINIT_("Zoek achterwaards ");
TDCLDEF char TEXT82[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT83[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT84[NOSZ_]  TINIT_("Vervang");
TDCLDEF char TEXT85[NOSZ_]  TINIT_("Vervang voorwaardelijk");
TDCLDEF char TEXT86[NOSZ_]  TINIT_("door ");
TDCLDEF char TEXT87[NOSZ_]  TINIT_("Vervangen van '");
TDCLDEF char TEXT88[NOSZ_]  TINIT_("' door '");
TDCLDEF char TEXT89[NOSZ_]  TINIT_("Onderbroken!");
TDCLDEF char TEXT90[NOSZ_]  TINIT_("(Y)Ja, (N)ee, (!)Doe rest, (U)Ongedaan, (^G)Breek af, (.)Stop, (?)Help: ");
TDCLDEF char TEXT91[NOSZ_]  TINIT_("Leeg woord vervangen en gestopt.");
TDCLDEF char TEXT92[NOSZ_]  TINIT_("%D vervangingen");
TDCLDEF char TEXT93[NOSZ_]  TINIT_("%%FOUT tijdens wegvegen");
TDCLDEF char TEXT94[NOSZ_]  TINIT_("%%Geen geheugen meer");
TDCLDEF char TEXT95[NOSZ_]  TINIT_("%%mceq: wat is %d?");
TDCLDEF char TEXT96[NOSZ_]  TINIT_("%%Geen karakters in karakter klasse");
TDCLDEF char TEXT97[NOSZ_]  TINIT_("%%Karakter klasse niet beeindigd");
TDCLDEF char TEXT98[NOSZ_]  TINIT_("Geen rechter marge gezet");
TDCLDEF char TEXT99[NOSZ_]  TINIT_("[GEEN GEHEUGEN MEER]");
TDCLDEF char TEXT100[NOSZ_] TINIT_("Woorden %D Karakters %D Lijnen %d Gem kars/woord %f");
TDCLDEF char TEXT101[NOSZ_] TINIT_("[Kan niet zoeken en springen tegelijkertijd!]");
TDCLDEF char TEXT102[NOSZ_] TINIT_("[Zinloos sprong argument]");
TDCLDEF char TEXT103[NOSZ_] TINIT_("[Bewaren %s]");
TDCLDEF char TEXT104[NOSZ_] TINIT_("Er zijn veranderde buffers. Toch stoppen");
TDCLDEF char TEXT105[NOSZ_] TINIT_("%%Macro is al aktief");
TDCLDEF char TEXT106[NOSZ_] TINIT_("[Start macro]");
TDCLDEF char TEXT107[NOSZ_] TINIT_("%%Macro is niet aktief");
TDCLDEF char TEXT108[NOSZ_] TINIT_("[Einde macro]");
TDCLDEF char TEXT109[NOSZ_] TINIT_("[Toets niet toegelaten in BEKIJK modus]");
TDCLDEF char TEXT110[NOSZ_] TINIT_("[Dit commando is GERESERVEERD]");
TDCLDEF char TEXT111[NOSZ_] TINIT_("Geen macro opgegeven");
TDCLDEF char TEXT112[NOSZ_] TINIT_("Macro nummer bestaat niet");
TDCLDEF char TEXT113[NOSZ_] TINIT_("Kan macro niet creeren");
TDCLDEF char TEXT114[NOSZ_] TINIT_("Procedure naam: ");
TDCLDEF char TEXT115[NOSZ_] TINIT_("Uit te voeren procedure: ");
TDCLDEF char TEXT116[NOSZ_] TINIT_("Onbestaande procedure");
TDCLDEF char TEXT117[NOSZ_] TINIT_("Uit te voeren buffer: ");
TDCLDEF char TEXT118[NOSZ_] TINIT_("Onbestaande buffer");
TDCLDEF char TEXT119[NOSZ_] TINIT_("%%Geen geheugen meer tijdens opzoeking");
TDCLDEF char TEXT120[NOSZ_] TINIT_("%%!BREAK buiten !WHILE lus");
TDCLDEF char TEXT121[NOSZ_] TINIT_("%%!ENDWHILE zonder voorafgaande !WHILE");
TDCLDEF char TEXT122[NOSZ_] TINIT_("%%!WHILE zonder overeenkomstige !ENDWHILE");
TDCLDEF char TEXT123[NOSZ_] TINIT_("%%Geen geheugen meer tijdens uitvoering van macro");
TDCLDEF char TEXT124[NOSZ_] TINIT_("%%Onbekende Verwijzing");
TDCLDEF char TEXT125[NOSZ_] TINIT_("Geen geheugen meer tijdens opslaan van macro");
TDCLDEF char TEXT126[NOSZ_] TINIT_("%%Interne While lus fout");
TDCLDEF char TEXT127[NOSZ_] TINIT_("%%Onbestaand label");
TDCLDEF char TEXT128[NOSZ_] TINIT_("(e)val, (c/x)ommando, (t)Volg, (^G)breek af, <SP>doe, <META> stop debug");
TDCLDEF char TEXT129[NOSZ_] TINIT_("Uit te voeren file: ");
TDCLDEF char TEXT130[NOSZ_] TINIT_("Macro niet gedefinieerd");
TDCLDEF char TEXT131[NOSZ_] TINIT_("Lees file");
TDCLDEF char TEXT132[NOSZ_] TINIT_("In te voegen file");
TDCLDEF char TEXT133[NOSZ_] TINIT_("Vind file");
TDCLDEF char TEXT134[NOSZ_] TINIT_("Bekijk file");
TDCLDEF char TEXT135[NOSZ_] TINIT_("[Oude buffer]");
TDCLDEF char TEXT136[NOSZ_] TINIT_("Buffer naam: ");
TDCLDEF char TEXT137[NOSZ_] TINIT_("Kan buffer niet creeren");
TDCLDEF char TEXT138[NOSZ_] TINIT_("[Nieuwe file]");
TDCLDEF char TEXT139[NOSZ_] TINIT_("[Lezen van file]");
TDCLDEF char TEXT140[NOSZ_] TINIT_("Lees ");
TDCLDEF char TEXT141[NOSZ_] TINIT_("I/O FOUT, ");
TDCLDEF char TEXT142[NOSZ_] TINIT_("GEEN GEHEUGEN MEER, ");
TDCLDEF char TEXT143[NOSZ_] TINIT_(" lijn");
TDCLDEF char TEXT144[NOSZ_] TINIT_("Schrijf file: ");
TDCLDEF char TEXT145[NOSZ_] TINIT_("Geen filenaam");
TDCLDEF char TEXT146[NOSZ_] TINIT_("Verkort file..bewaren");
TDCLDEF char TEXT147[NOSZ_] TINIT_("Vernauwde Buffer..bewaren");
TDCLDEF char TEXT148[NOSZ_] TINIT_("[Bewaren...]");
TDCLDEF char TEXT149[NOSZ_] TINIT_("[Bewaard als ");
TDCLDEF char TEXT150[NOSZ_] TINIT_(", bewaard als ");
TDCLDEF char TEXT151[NOSZ_] TINIT_("Naam: ");
TDCLDEF char TEXT152[NOSZ_] TINIT_("[File bestaat niet]");
TDCLDEF char TEXT153[NOSZ_] TINIT_("[Invoegen file]");
TDCLDEF char TEXT154[NOSZ_] TINIT_("Ingevoegd ");
TDCLDEF char TEXT155[NOSZ_] TINIT_("Kan file niet bewaren");
TDCLDEF char TEXT156[NOSZ_] TINIT_("Kan file niet afsluiten");
TDCLDEF char TEXT157[NOSZ_] TINIT_("Bewaar I/O fout");
TDCLDEF char TEXT158[NOSZ_] TINIT_("File lees-fout");
TDCLDEF char TEXT159[NOSZ_] TINIT_("%Heb nummer van functietoets nodig");
TDCLDEF char TEXT160[NOSZ_] TINIT_("%Functietoets met dit nummer bestaat niet");
TDCLDEF char TEXT161[NOSZ_] TINIT_("Label inhoud: ");
TDCLDEF char TEXT162[NOSZ_] TINIT_(" [y(ja)/n]? ");
TDCLDEF char TEXT163[NOSZ_] TINIT_("geen default");
TDCLDEF char TEXT164[NOSZ_] TINIT_("[opzoeken mislukt]");
TDCLDEF char TEXT165[NOSZ_] TINIT_("Zoek Incrementeel: ");
TDCLDEF char TEXT166[NOSZ_] TINIT_("? Te zoeken woord te lang");
TDCLDEF char TEXT167[NOSZ_] TINIT_("? commando te lang");
TDCLDEF char TEXT168[NOSZ_] TINIT_("%%Kan woord niet invoegen");
TDCLDEF char TEXT169[NOSZ_] TINIT_("Ingevoegd");
TDCLDEF char TEXT170[NOSZ_] TINIT_("interne fout: linsert");
TDCLDEF char TEXT171[NOSZ_] TINIT_("Vervangen");
TDCLDEF char TEXT172[NOSZ_] TINIT_("%%Geen geheugen meer tijdens overschrijven");
TDCLDEF char TEXT173[NOSZ_] TINIT_("LOCK FOUT: Lock tabel vol");
TDCLDEF char TEXT174[NOSZ_] TINIT_("Kan geen lock zetten, geen geheugen meer");
TDCLDEF char TEXT175[NOSZ_] TINIT_("LOCK");
TDCLDEF char TEXT176[NOSZ_] TINIT_("File in gebruik door ");
TDCLDEF char TEXT177[NOSZ_] TINIT_(", toch proberen?");
TDCLDEF char TEXT178[NOSZ_] TINIT_("[kan foutenboodschap van systeem niet ontvangen]");
TDCLDEF char TEXT179[NOSZ_] TINIT_("  Over MicroEmacs");
TDCLDEF char TEXT180[NOSZ_] TINIT_("%%Onbestaande resolutie");
TDCLDEF char TEXT181[NOSZ_] TINIT_("%%Onmogelijke Resolutie voor dit scherm");
TDCLDEF char TEXT182[NOSZ_] TINIT_("Omgevings-variable TERM not defined!");
TDCLDEF char TEXT183[NOSZ_] TINIT_("Onbekend terminal type %s!");
TDCLDEF char TEXT184[NOSZ_] TINIT_("termcap definitie onvolledig (lijnen)");
TDCLDEF char TEXT185[NOSZ_] TINIT_("Termcap definitie onvolledig (kolommen)");
TDCLDEF char TEXT186[NOSZ_] TINIT_("Onvolledige termcap definitie\n");
TDCLDEF char TEXT187[NOSZ_] TINIT_("Beschrijving van terminal is te lang!\n");
TDCLDEF char TEXT188[NOSZ_] TINIT_("[Einde]");
TDCLDEF char TEXT189[NOSZ_] TINIT_("Kan geen definitie voor dit terminal type.\n");
TDCLDEF char TEXT190[NOSZ_] TINIT_("Controleer terminal type met \"SHOW TERMINAL\" of\n");
TDCLDEF char TEXT191[NOSZ_] TINIT_("probeer het te zetten met \"SET TERMINAL/INQUIRE\"\n");
TDCLDEF char TEXT192[NOSZ_] TINIT_("Deze terminal is niet krachtig genoeg om MicroEMACS\n");
TDCLDEF char TEXT193[NOSZ_] TINIT_("te gebruiken. Probeer een andere terminal of controleer\n");
TDCLDEF char TEXT194[NOSZ_] TINIT_("het type met \"SHOW TERMINAL\".\n");
TDCLDEF char TEXT195[NOSZ_] TINIT_("Kan communicatie met terminal niet openen.\n");
TDCLDEF char TEXT196[NOSZ_] TINIT_("Kan instelling van de terminal niet opvragen.\n");
TDCLDEF char TEXT197[NOSZ_] TINIT_("Kan instelling van de terminal niet veranderen.\n");
TDCLDEF char TEXT198[NOSZ_] TINIT_("I/O fout (%d,%d)\n");
TDCLDEF char TEXT199[NOSZ_] TINIT_("[Start DCL]\r\n");
TDCLDEF char TEXT200[NOSZ_] TINIT_("[Oproepen DCL]\r\n");
TDCLDEF char TEXT201[NOSZ_] TINIT_("[Nog niet beschikbaar voor VMS]");
TDCLDEF char TEXT202[NOSZ_] TINIT_("Terminal is geen 'vt52'of 'z19' !");
TDCLDEF char TEXT203[NOSZ_] TINIT_("Venster met dit nummer bestaat niet");
TDCLDEF char TEXT204[NOSZ_] TINIT_("Kan dit venster niet verwijderen");
TDCLDEF char TEXT205[NOSZ_] TINIT_("Kan een venster met %d lijnen niet opsplitsen");
TDCLDEF char TEXT206[NOSZ_] TINIT_("Er is maar 1 venster");
TDCLDEF char TEXT207[NOSZ_] TINIT_("Onmogelijke verandering");
TDCLDEF char TEXT208[NOSZ_] TINIT_("[Dat venster bestaat niet]");
TDCLDEF char TEXT209[NOSZ_] TINIT_("%%Deze scherm grootte wordt niet ondersteund");
TDCLDEF char TEXT210[NOSZ_] TINIT_("%%Deze scherm breedte wordt niet ondersteund");
TDCLDEF char TEXT211[NOSZ_] TINIT_("Lijst van functies");
TDCLDEF char TEXT212[NOSZ_] TINIT_("Kan lijst van functie niet tonen");
TDCLDEF char TEXT213[NOSZ_] TINIT_("[Opbouwen lijst van functie]");
TDCLDEF char TEXT214[NOSZ_] TINIT_("%%File niet bekend als %s");
TDCLDEF char TEXT215[NOSZ_] TINIT_(": macro-aan-toets ");
TDCLDEF char TEXT216[NOSZ_] TINIT_("Kan directories niet lezen/schrijven");
TDCLDEF char TEXT217[NOSZ_] TINIT_("[Nog niet beschikbaar onder AOS/VS]");
TDCLDEF char TEXT218[NOSZ_] TINIT_("Toevoegen file: ");
TDCLDEF char TEXT219[NOSZ_] TINIT_("%%Macro Fout");
TDCLDEF char TEXT220[NOSZ_] TINIT_("Lijn %D/%D Kolom %d/%d Kar. %D/%D (%d%%) Kar. = 0x%x%x");
TDCLDEF char TEXT221[NOSZ_] TINIT_("Te veel groepen");
TDCLDEF char TEXT222[NOSZ_] TINIT_("Groep niet beeindigd");
TDCLDEF char TEXT223[NOSZ_] TINIT_("%%Kolom start buiten toegelaten waarde");
TDCLDEF char TEXT224[NOSZ_] TINIT_("%%Rij start buiten toegelaten waarde");
TDCLDEF char TEXT225[NOSZ_] TINIT_("[Gegaan naar scherm %s]");
TDCLDEF char TEXT226[NOSZ_] TINIT_("%%Kan executing buffer niet verwijderen");
TDCLDEF char TEXT227[NOSZ_] TINIT_("\n--- Druk op een toets om verder te gaan ---");
TDCLDEF char TEXT228[NOSZ_] TINIT_("[Kill ring gewist]");
TDCLDEF char TEXT229[NOSZ_] TINIT_(" in < ");
TDCLDEF char TEXT230[NOSZ_] TINIT_("> at line ");

/**TODO: Translate to Dutch**/
TDCLDEF char TEXT231[NOSZ_] TINIT_("Abbreviation to set: ");
TDCLDEF char TEXT232[NOSZ_] TINIT_("Abbreviation to delete: ");
TDCLDEF char TEXT233[NOSZ_] TINIT_("[Building Abbreviation list]");
TDCLDEF char TEXT234[NOSZ_] TINIT_("Abbreviation list");
TDCLDEF char TEXT235[NOSZ_] TINIT_("Can not display abbreviation list");
TDCLDEF char TEXT236[NOSZ_] TINIT_("Define Abbreviations in buffer");
/**========================**/

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
#endif/**#ifndef DUTCH_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/

