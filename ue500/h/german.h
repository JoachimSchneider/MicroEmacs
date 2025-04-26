/*======================================================================
 *      GERMAN.H:       German language text strings for
 *                      MicroEMACS 4.00
 *
 *                      translated by Martin Neitzel
 *                      unido!sigserv!neitzel@uunet.uu.net
 *                      (C)opyright 1995 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#ifndef GERMAN_H_
#define GERMAN_H_
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

TDCLDEF char TEXT1[NOSZ_]   TINIT_("[neuer Kommando-Interpreter wird gestartet]");
/* if `CLI' is the Amiga-specific term, better keep the keyword: */
/* #define TEXT1    "[neuer CLI wird gestartet]" */

TDCLDEF char TEXT2[NOSZ_]   TINIT_("[Kann Filter-Datei nicht schreiben]");
TDCLDEF char TEXT3[NOSZ_]   TINIT_("[Ausfuehrung schlug fehl]");
TDCLDEF char TEXT4[NOSZ_]   TINIT_("Shell-Variable TERM nicht definiert!");
TDCLDEF char TEXT5[NOSZ_]   TINIT_("Terminal ist kein 'vt100'!");
TDCLDEF char TEXT6[NOSZ_]   TINIT_("\r\n\n[Ende]");
TDCLDEF char TEXT7[NOSZ_]   TINIT_("Gehe zu Zeile: ");
TDCLDEF char TEXT8[NOSZ_]   TINIT_("[abgebrochen]");
TDCLDEF char TEXT9[NOSZ_]   TINIT_("[Marke %d gesetzt]");
TDCLDEF char TEXT10[NOSZ_]  TINIT_("[Marke %d entfernt]");
TDCLDEF char TEXT11[NOSZ_]  TINIT_("Keine Marke %d in diesem Fenster");
TDCLDEF char TEXT12[NOSZ_]  TINIT_("['Hilfe'-Datei nicht gefunden]");
TDCLDEF char TEXT13[NOSZ_]  TINIT_(": zeige Tastenbelegung fuer ");
/* >    leave the colon here...(and elsewhere) it pretends to be a command
 * prompt
 */
TDCLDEF char TEXT14[NOSZ_]  TINIT_("ist nicht belegt");
TDCLDEF char TEXT15[NOSZ_]  TINIT_(": definiere Taste fuer Funktion ");
TDCLDEF char TEXT16[NOSZ_]  TINIT_("[Keine solche Funktion]");
TDCLDEF char TEXT17[NOSZ_]  TINIT_("Definitions-Tabelle VOLL!");
TDCLDEF char TEXT18[NOSZ_]  TINIT_(": entferne Funktions-Definition fuer Taste ");
TDCLDEF char TEXT19[NOSZ_]  TINIT_("[Taste hat keine Definition]");
TDCLDEF char TEXT20[NOSZ_]  TINIT_("Apropos zu Text: ");
TDCLDEF char TEXT21[NOSZ_]  TINIT_("Definitions-Tabelle");
TDCLDEF char TEXT22[NOSZ_]  TINIT_("Kann Definitions-Tabelle nicht anzeigen");
TDCLDEF char TEXT23[NOSZ_]  TINIT_("[Defintions-Tabelle wird erstellt...]");
TDCLDEF char TEXT24[NOSZ_]  TINIT_("Wechsel zu Buffer");
TDCLDEF char TEXT25[NOSZ_]  TINIT_("Wechsel zu Buffer: ");
TDCLDEF char TEXT26[NOSZ_]  TINIT_("Loesche Buffer");
TDCLDEF char TEXT27[NOSZ_]  TINIT_("Pop buffer");
TDCLDEF char TEXT28[NOSZ_]  TINIT_("Buffer wird gerade angezeigt");
TDCLDEF char TEXT29[NOSZ_]  TINIT_("Aendere Buffer-Namen zu: ");
TDCLDEF char TEXT30[NOSZ_]  TINIT_("ACTN   Modi    Groesse Buffer          Datei");
/* >   ^^^The spacing must match on this line. It is the header for the buffer
 * list.
 *  (I extended "size" to the left to make "Groesse" fit.)
 */

TDCLDEF char TEXT31[NOSZ_]  TINIT_("         Globale Modi");
TDCLDEF char TEXT32[NOSZ_]  TINIT_("Aenderungen wegwerfen");
TDCLDEF char TEXT33[NOSZ_]  TINIT_("Verschluesselungs-Text: ");
TDCLDEF char TEXT34[NOSZ_]  TINIT_("Datei: ");
TDCLDEF char TEXT35[NOSZ_]  TINIT_("ein anderer Benutzer");

/* >The UNIX terms should stay the same in the next few entryies */
TDCLDEF char TEXT36[NOSZ_]  TINIT_("LOCK FEHLER -- ");
TDCLDEF char TEXT37[NOSZ_]  TINIT_("Ueberpruefen der Existenz von %s\n");
TDCLDEF char TEXT38[NOSZ_]  TINIT_("Einrichten von directory %s\n");
TDCLDEF char TEXT39[NOSZ_]  TINIT_("erzeuge %s\n");
TDCLDEF char TEXT40[NOSZ_]  TINIT_("konnte \"lock\"-Datei nicht einrichten");
TDCLDEF char TEXT41[NOSZ_]  TINIT_("pid ist %ld\n");
TDCLDEF char TEXT42[NOSZ_]  TINIT_("lese lock-Datei %s\n");
TDCLDEF char TEXT43[NOSZ_]  TINIT_("konnte lock-Datei nicht lesen");
TDCLDEF char TEXT44[NOSZ_]  TINIT_("pid in %s ist %ld\n");
TDCLDEF char TEXT45[NOSZ_]  TINIT_("Benachrichtigung (`kill') an Prozess %ld\n");
TDCLDEF char TEXT46[NOSZ_]  TINIT_("Prozess beendet");
TDCLDEF char TEXT47[NOSZ_]  TINIT_("`kill' war nicht in Ordnung");
TDCLDEF char TEXT48[NOSZ_]  TINIT_("`kill' war OK; Prozess beendet");
TDCLDEF char TEXT49[NOSZ_]  TINIT_("Versuch: unlink %s\n");
TDCLDEF char TEXT50[NOSZ_]  TINIT_("konnte lock-Datei nicht loeschen");
TDCLDEF char TEXT51[NOSZ_]  TINIT_("zu setzende Variable: ");
TDCLDEF char TEXT52[NOSZ_]  TINIT_("%%Keine solche Variable '%s'");
TDCLDEF char TEXT53[NOSZ_]  TINIT_("Wert: ");
TDCLDEF char TEXT54[NOSZ_]  TINIT_("[Makro abgebrochen]");
TDCLDEF char TEXT55[NOSZ_]  TINIT_("anzuzeigende Variable: ");
TDCLDEF char TEXT56[NOSZ_]  TINIT_("Variablen-Liste");
TDCLDEF char TEXT57[NOSZ_]  TINIT_("kann Variablen-Liste nicht anzeigen");
TDCLDEF char TEXT58[NOSZ_]  TINIT_("[Variablen-Liste wird aufgebaut...]");
TDCLDEF char TEXT59[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT60[NOSZ_]  TINIT_("Zeile %D/%D Spalte %d/%d Zeichen %D/%D (%d%%) Zeichen = 0x%x");
TDCLDEF char TEXT61[NOSZ_]  TINIT_("<NOT USED>");
TDCLDEF char TEXT62[NOSZ_]  TINIT_("Globaler Modus, der ");
TDCLDEF char TEXT63[NOSZ_]  TINIT_("Modus, der ");
TDCLDEF char TEXT64[NOSZ_]  TINIT_("hinzugenommen werden soll: ");
TDCLDEF char TEXT65[NOSZ_]  TINIT_("entfernt werden soll: ");
TDCLDEF char TEXT66[NOSZ_]  TINIT_("Kein solcher Modus!");
TDCLDEF char TEXT67[NOSZ_]  TINIT_("Auszugebender Text: ");
TDCLDEF char TEXT68[NOSZ_]  TINIT_("Einzufuegender Text: ");
TDCLDEF char TEXT69[NOSZ_]  TINIT_("Zu ueberschreibender Text: ");
TDCLDEF char TEXT70[NOSZ_]  TINIT_("[Region kopiert]");
TDCLDEF char TEXT71[NOSZ_]  TINIT_("%%Dieser Buffer ist bereits auf einen Bereich eingeschraenkt");
TDCLDEF char TEXT72[NOSZ_]  TINIT_("%%Bereich muss mind. eine ganze Zeile umfassen");
TDCLDEF char TEXT73[NOSZ_]  TINIT_("[Buffer ist auf Bereich eingeschraenkt]");
TDCLDEF char TEXT74[NOSZ_]  TINIT_("%%Dieser Buffer ist nicht eingeschraenkt.");
TDCLDEF char TEXT75[NOSZ_]  TINIT_("[Buffer ist ohne Bereichs-Einschraenkung.]");
TDCLDEF char TEXT76[NOSZ_]  TINIT_("Keine Marke in diesem Fenster gesetzt");
TDCLDEF char TEXT77[NOSZ_]  TINIT_("Fehler: Marke verloren (nicht tragisch)");
TDCLDEF char TEXT78[NOSZ_]  TINIT_("Suche ");
TDCLDEF char TEXT79[NOSZ_]  TINIT_("Nicht gefunden");
TDCLDEF char TEXT80[NOSZ_]  TINIT_("Kein Such-Muster gesetzt");
TDCLDEF char TEXT81[NOSZ_]  TINIT_("Rueckwaerts-Suche ");
TDCLDEF char TEXT82[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT83[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT84[NOSZ_]  TINIT_("Ersetze ");
TDCLDEF char TEXT85[NOSZ_]  TINIT_("Ersetze interaktiv ");
TDCLDEF char TEXT86[NOSZ_]  TINIT_("durch ");
TDCLDEF char TEXT87[NOSZ_]  TINIT_("Ersetzen von '");
TDCLDEF char TEXT88[NOSZ_]  TINIT_("' durch '");
TDCLDEF char TEXT89[NOSZ_]  TINIT_("Abgebrochen!");
/*
 * The following couldn't be translated without interferring with the expected
 * input characters, so I let it untouched.
 */
TDCLDEF char TEXT90[NOSZ_]  TINIT_("(Y)es, (N)o, (!)Do rest, (U)ndo last, (^G)Abort, (.)Abort back, (?)Help: ");
/* >  ^^^^   make sure this is not longer than 72 characters */

TDCLDEF char TEXT91[NOSZ_]  TINIT_("leeren Text ersetzt, gestoppt.");
TDCLDEF char TEXT92[NOSZ_]  TINIT_("%D Ersetzungen");
TDCLDEF char TEXT93[NOSZ_]  TINIT_("%%Fehler waehrend des Loeschens");
TDCLDEF char TEXT94[NOSZ_]  TINIT_("%%Kein Speicherplatz mehr frei");
TDCLDEF char TEXT95[NOSZ_]  TINIT_("%%mceq: Was ist %d?");
TDCLDEF char TEXT96[NOSZ_]  TINIT_("%%Keine Zeichen in Zeichenklasse");
TDCLDEF char TEXT97[NOSZ_]  TINIT_("%%Zeichenklasse nicht beendet");
TDCLDEF char TEXT98[NOSZ_]  TINIT_("Keine Spalte fuer Umbruch (fill-column) definiert");
TDCLDEF char TEXT99[NOSZ_]  TINIT_("[FREIER SPEICHERPLATZ AUFGEBRAUCHT]");
TDCLDEF char TEXT100[NOSZ_] TINIT_("%D Worte, %D Zeichen, %d Zeilen, durchschn. Wortlaenge %f");
TDCLDEF char TEXT101[NOSZ_] TINIT_("[kann nicht zugleich suchen und zu einer Zeile gehen!]");
TDCLDEF char TEXT102[NOSZ_] TINIT_("[Unsinniges Argument fuer `goto']");
TDCLDEF char TEXT103[NOSZ_] TINIT_("[Abspeichern von %s]");
TDCLDEF char TEXT104[NOSZ_] TINIT_("Geaenderte Buffer existieren.  Trotzdem beenden");
TDCLDEF char TEXT105[NOSZ_] TINIT_("%%Makro bereits aktiv");
TDCLDEF char TEXT106[NOSZ_] TINIT_("[Anfang von Makro]");
TDCLDEF char TEXT107[NOSZ_] TINIT_("%%Makro nicht aktiv");
TDCLDEF char TEXT108[NOSZ_] TINIT_("[Ende von Makro]");
TDCLDEF char TEXT109[NOSZ_] TINIT_("[Taste unzulaessig im VIEW-Modus]");
TDCLDEF char TEXT110[NOSZ_] TINIT_("[Dieses Kommando ist nicht freigegeben!]");
TDCLDEF char TEXT111[NOSZ_] TINIT_("Kein Makro angegeben");
TDCLDEF char TEXT112[NOSZ_] TINIT_("Makro-Nummer ausserhalb des gueltigen Bereichs");
TDCLDEF char TEXT113[NOSZ_] TINIT_("Kann Makro nicht erzeugen");
TDCLDEF char TEXT114[NOSZ_] TINIT_("Prozedur-Name: ");
TDCLDEF char TEXT115[NOSZ_] TINIT_("Fuehre Prozedur aus: ");
TDCLDEF char TEXT116[NOSZ_] TINIT_("Keine solche Prozedur");
TDCLDEF char TEXT117[NOSZ_] TINIT_("Fuehre Buffer aus: ");
TDCLDEF char TEXT118[NOSZ_] TINIT_("Kein solcher Buffer");
TDCLDEF char TEXT119[NOSZ_] TINIT_("%%freier Speicher aufgebraucht waehrend `while-scan'");

/* > Directive names and keywords are not translated... */
TDCLDEF char TEXT120[NOSZ_] TINIT_("%%!BREAK ausserhalb einer !WHILE-Schleife");
TDCLDEF char TEXT121[NOSZ_] TINIT_("%%!ENDWHILE ohne vorausgegangenes !WHILE");
TDCLDEF char TEXT122[NOSZ_] TINIT_("%%!WHILE ohne zugehoeriges !ENDWHILE");
TDCLDEF char TEXT123[NOSZ_] TINIT_("%%freier Speicher waehrend Makroausfuehrung aufgebraucht");
TDCLDEF char TEXT124[NOSZ_] TINIT_("%%Unbekannte Direktive");
TDCLDEF char TEXT125[NOSZ_] TINIT_("freier Speicher beim Abspeichern eines Makros aufgebraucht");
TDCLDEF char TEXT126[NOSZ_] TINIT_("%%interner Fehler bei While-Schleife");
TDCLDEF char TEXT127[NOSZ_] TINIT_("%%Kein solches Label");
TDCLDEF char TEXT128[NOSZ_] TINIT_("(e)val exp, (c/x)ommand, (t)rack exp, (^G)abort, <SP>exec, <META> stop debug");
/* > ^^^ No longer than 72 chars again
 * And, again too, no translation because of expected input characters
 */
TDCLDEF char TEXT129[NOSZ_] TINIT_("Auszufuehrende Datei: ");
TDCLDEF char TEXT130[NOSZ_] TINIT_("Makro nicht definiert");
TDCLDEF char TEXT131[NOSZ_] TINIT_("Einzulesende Datei");
TDCLDEF char TEXT132[NOSZ_] TINIT_("Einzufuegende Datei");
TDCLDEF char TEXT133[NOSZ_] TINIT_("Finde Datei");
TDCLDEF char TEXT134[NOSZ_] TINIT_("Zu betrachtende Datei");
TDCLDEF char TEXT135[NOSZ_] TINIT_("[alter Buffer]");
TDCLDEF char TEXT136[NOSZ_] TINIT_("Buffer-Name: ");
TDCLDEF char TEXT137[NOSZ_] TINIT_("Kann Buffer nicht einrichten");
TDCLDEF char TEXT138[NOSZ_] TINIT_("[neue Datei]");
TDCLDEF char TEXT139[NOSZ_] TINIT_("[Einlesen von Datei]");

TDCLDEF char TEXT140[NOSZ_] TINIT_("Lese ");
/* probably a bad choice, has to be checked against its context. */

TDCLDEF char TEXT141[NOSZ_] TINIT_("I/O FEHLER, ");
TDCLDEF char TEXT142[NOSZ_] TINIT_("FREIER SPEICHERPLATZ VERBRAUCHT, ");
TDCLDEF char TEXT143[NOSZ_] TINIT_(" Zeile");
TDCLDEF char TEXT144[NOSZ_] TINIT_("Abspeichern in Datei: ");
TDCLDEF char TEXT145[NOSZ_] TINIT_("Kein Dateiname");
/*
 * I wonder how the next two msgs are meant... I guess the user is encouraged
 * to do a write-file instead of a save-file.
 * #define      TEXT146 "Truncated file..write it out"
 * #define      TEXT147 "Narrowed Buffer..write it out"
 */
TDCLDEF char TEXT146[NOSZ_] TINIT_("Unvollstaendige Datei; [`write-file' benutzen]");
TDCLDEF char TEXT147[NOSZ_] TINIT_("Buffer auf Bereich eingeschraenkt; [`write-file' benutzen]");

TDCLDEF char TEXT148[NOSZ_] TINIT_("[Datei wird geschrieben...]");
TDCLDEF char TEXT149[NOSZ_] TINIT_("[Datei geschrieben: ");
TDCLDEF char TEXT150[NOSZ_] TINIT_(", gespeichert als ");
TDCLDEF char TEXT151[NOSZ_] TINIT_("Name: ");
TDCLDEF char TEXT152[NOSZ_] TINIT_("[Keine solche Datei]");
TDCLDEF char TEXT153[NOSZ_] TINIT_("[Datei wird eingefuegt]");
TDCLDEF char TEXT154[NOSZ_] TINIT_("Eingefuegt: ");
TDCLDEF char TEXT155[NOSZ_] TINIT_("Kann Datei nicht zum Schreiben oeffnen");
TDCLDEF char TEXT156[NOSZ_] TINIT_("Fehler beim Schliessen der Datei");
TDCLDEF char TEXT157[NOSZ_] TINIT_("Schreib-I/O-Fehler");
TDCLDEF char TEXT158[NOSZ_] TINIT_("Fehler beim Lesen der Datei");
TDCLDEF char TEXT159[NOSZ_] TINIT_("%Brauche Funktionstasten-Nummer");
TDCLDEF char TEXT160[NOSZ_] TINIT_("%Nummer fuer Funktionstaste ausserhalb vom gueltigen Bereich");
TDCLDEF char TEXT161[NOSZ_] TINIT_("Label-Inhalt: ");

/* Again, unchanged due to expected characters: */
TDCLDEF char TEXT162[NOSZ_] TINIT_(" [y/n]? ");

TDCLDEF char TEXT163[NOSZ_] TINIT_("Kein Default");
TDCLDEF char TEXT164[NOSZ_] TINIT_("[Suche erfolglos]");
TDCLDEF char TEXT165[NOSZ_] TINIT_("inkrementelle Suche: ");
TDCLDEF char TEXT166[NOSZ_] TINIT_("? Suchtext zu lang");
TDCLDEF char TEXT167[NOSZ_] TINIT_("? Kommando zu lang");
TDCLDEF char TEXT168[NOSZ_] TINIT_("%%Kann Text nicht einfuegen");
TDCLDEF char TEXT169[NOSZ_] TINIT_("Eingefuegt");
TDCLDEF char TEXT170[NOSZ_] TINIT_("bug: linsert");
TDCLDEF char TEXT171[NOSZ_] TINIT_("Ersetzt");
TDCLDEF char TEXT172[NOSZ_] TINIT_("%%freier Speicherplatz beim Ueberschreiben verbraucht");
TDCLDEF char TEXT173[NOSZ_] TINIT_("LOCK FEHLER: Lock-Tabelle voll");
TDCLDEF char TEXT174[NOSZ_] TINIT_("Kann kein `LOCK' auf Datei legen, freier Speicherplatz verbraucht");
TDCLDEF char TEXT175[NOSZ_] TINIT_("LOCK (Dateizugriff reserviert)");

/* If ", override?" in TEXT177 refers to a lock and not to a file, better use
 * ", ignorieren?" (==ignore) or ", uebernehmen?" (==take over) instead.
 */
TDCLDEF char TEXT176[NOSZ_] TINIT_("Datei wird benutzt von ");
TDCLDEF char TEXT177[NOSZ_] TINIT_(", ueberschreiben?");

TDCLDEF char TEXT178[NOSZ_] TINIT_("[kann System-Fehlertext nicht herausfinden]");
TDCLDEF char TEXT179[NOSZ_] TINIT_("  Ueber MicroEmacs");
TDCLDEF char TEXT180[NOSZ_] TINIT_("%%Keine solche Aufloesung");
TDCLDEF char TEXT181[NOSZ_] TINIT_("%%Aufloesung unzulaessig fuer diesen Bildschirm");
TDCLDEF char TEXT182[NOSZ_] TINIT_("Environment-Variable TERM nicht definiert!");
TDCLDEF char TEXT183[NOSZ_] TINIT_("Unbekannter Terminal-Typ %s!");
TDCLDEF char TEXT184[NOSZ_] TINIT_("Termcap-Eintrag unvollstaendig (`lines')");
TDCLDEF char TEXT185[NOSZ_] TINIT_("Termcap-Eintrag unvollstaendig (`columns')");
TDCLDEF char TEXT186[NOSZ_] TINIT_("Unvollstaendiger Termcap-Eintrag\n");
TDCLDEF char TEXT187[NOSZ_] TINIT_("Terminal-Beschreibung zu lang!\n");
TDCLDEF char TEXT188[NOSZ_] TINIT_("[Ende]");
TDCLDEF char TEXT189[NOSZ_] TINIT_("Kann Eintrag fuer Terminal-Typ nicht finden.\n");

/* > Don't translate the VMS commands in the next 5 lines */
TDCLDEF char TEXT190[NOSZ_] TINIT_("Ueberpruefe Terminal-Typ mit \"SHOW TERMINAL\" oder\n");
TDCLDEF char TEXT191[NOSZ_] TINIT_("versuche eine Einstellung mit \"SET TERMINAL/INQUIRE\"\n");
TDCLDEF char TEXT192[NOSZ_] TINIT_("Der Terminaltyp hat nicht genuegend Faehigkeiten fuer\n");
TDCLDEF char TEXT193[NOSZ_] TINIT_("MicroEMACS.  Versuche ein anderes Terminal oder ueberpruefe\n");
TDCLDEF char TEXT194[NOSZ_] TINIT_("den Typ mit \"SHOW TERMINAL\".\n");

TDCLDEF char TEXT195[NOSZ_] TINIT_("Kann Kanal zum Terminal nicht oeffnen.\n");
TDCLDEF char TEXT196[NOSZ_] TINIT_("Kann Terminal-Einstellungen nicht herausfinden.\n");
TDCLDEF char TEXT197[NOSZ_] TINIT_("Kann Terminal-Einstellungen nicht aendern.\n");
TDCLDEF char TEXT198[NOSZ_] TINIT_("I/O error (%d,%d)\n");
TDCLDEF char TEXT199[NOSZ_] TINIT_("[DCL wird gestartet]\r\n");
TDCLDEF char TEXT200[NOSZ_] TINIT_("[DCL wird aufgerufen]\r\n");
TDCLDEF char TEXT201[NOSZ_] TINIT_("[Noch nicht unter VMS verfuegbar]");
TDCLDEF char TEXT202[NOSZ_] TINIT_("Terminal-Typ weder 'vt52' noch 'z19' !");
TDCLDEF char TEXT203[NOSZ_] TINIT_("Fenster-Nummer ausserhalb des zulaessigen Bereichs");
TDCLDEF char TEXT204[NOSZ_] TINIT_("Kann dieses Fenster nicht loeschen");
TDCLDEF char TEXT205[NOSZ_] TINIT_("Kann ein %d-Zeilen-Fenster nicht weiter teilen");
TDCLDEF char TEXT206[NOSZ_] TINIT_("Nur ein Fenster");
TDCLDEF char TEXT207[NOSZ_] TINIT_("Unmoegliche Aenderung");
TDCLDEF char TEXT208[NOSZ_] TINIT_("[Kein solches Fenster existiert]");
TDCLDEF char TEXT209[NOSZ_] TINIT_("%%Bildschirmgroesse ausserhalb des gueltigen Bereichs");
TDCLDEF char TEXT210[NOSZ_] TINIT_("%%Bildschirmbreite ausserhalb des gueltigen Bereichs");
TDCLDEF char TEXT211[NOSZ_] TINIT_("Funktion-Liste");
TDCLDEF char TEXT212[NOSZ_] TINIT_("kann Funktion-Liste nicht anzeigen");
TDCLDEF char TEXT213[NOSZ_] TINIT_("[Funktion-Liste wird aufgebaut...]");
TDCLDEF char TEXT214[NOSZ_] TINIT_("%%No such file as %s");
TDCLDEF char TEXT215[NOSZ_] TINIT_(": definiere Makro fuer Funktion ");
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

/**TODO: Translate to German**/
TDCLDEF char TEXT231[NOSZ_] TINIT_("Abbreviation to set: ");
TDCLDEF char TEXT232[NOSZ_] TINIT_("Abbreviation to delete: ");
TDCLDEF char TEXT233[NOSZ_] TINIT_("[Building Abbreviation list]");
TDCLDEF char TEXT234[NOSZ_] TINIT_("Abbreviation list");
TDCLDEF char TEXT235[NOSZ_] TINIT_("Can not display abbreviation list");
TDCLDEF char TEXT236[NOSZ_] TINIT_("Define Abbreviations in buffer");
/**=========================**/

TDCLDEF char TEXT240[NOSZ_] TINIT_("[No such screen]");
TDCLDEF char TEXT241[NOSZ_] TINIT_("%%Can't delete current screen");
TDCLDEF char TEXT242[NOSZ_] TINIT_("Find Screen: ");
TDCLDEF char TEXT243[NOSZ_] TINIT_("Delete Screen: ");
TDCLDEF char TEXT244[NOSZ_] TINIT_("%%Funktion '%s' ist nicht verfuegbar");
TDCLDEF char TEXT245[NOSZ_] TINIT_("%%Division durch Null ist nicht moeglich");
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
#endif/**#ifndef GERMAN_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
