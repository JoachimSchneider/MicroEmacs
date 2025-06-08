/*======================================================================
 * LATIN.H:     Latin language text strings for MicroEMACS 4.00
 *              (C)Copyright 1995 by Daniel M. Lawrence
 *              translated 1991 by Anthony Appleyard
 *====================================================================*/

/*====================================================================*/
#ifndef LATIN_H_
#define LATIN_H_
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

TDCLDEF char TEXT1[NOSZ_]   TINIT_("[incipio novum CLI]");
TDCLDEF char TEXT2[NOSZ_]   TINIT_("[non possum scribere filum percolationis]");
TDCLDEF char TEXT3[NOSZ_]   TINIT_("[executio cecidit]");
TDCLDEF char TEXT4[NOSZ_]   TINIT_("involucri variabilis TERM non definitus est!");
TDCLDEF char TEXT5[NOSZ_]   TINIT_("terminalis genus non est 'vt100'!");
TDCLDEF char TEXT6[NOSZ_]   TINIT_("\r\n\n[finis]");
TDCLDEF char TEXT7[NOSZ_]   TINIT_("linea adeunda: ");
TDCLDEF char TEXT8[NOSZ_]   TINIT_("[interrupistis]");
TDCLDEF char TEXT9[NOSZ_]   TINIT_("[indicium %d locavi]");
TDCLDEF char TEXT10[NOSZ_]  TINIT_("[indicium %d delevi]");
TDCLDEF char TEXT11[NOSZ_]  TINIT_("non est indicium %d in hac fenestra");
TDCLDEF char TEXT12[NOSZ_]  TINIT_("[non possum invenire filum adjuvationis]");
TDCLDEF char TEXT13[NOSZ_]  TINIT_(": explicare-clavem ");
TDCLDEF char TEXT14[NOSZ_]  TINIT_("non ligata est");
TDCLDEF char TEXT15[NOSZ_]  TINIT_(": ligare-ad-clavem ");
TDCLDEF char TEXT16[NOSZ_]  TINIT_("[non est ista functio]");
TDCLDEF char TEXT17[NOSZ_]  TINIT_("ligationum tabula PLENA est!");
TDCLDEF char TEXT18[NOSZ_]  TINIT_(": laxare-clavem ");
TDCLDEF char TEXT19[NOSZ_]  TINIT_("[ista clavis non ligata est]");
TDCLDEF char TEXT20[NOSZ_]  TINIT_("verba 'apropos'-ica: ");
TDCLDEF char TEXT21[NOSZ_]  TINIT_("tabula ligationum");
TDCLDEF char TEXT22[NOSZ_]  TINIT_("non possum ostendere tabulam ligationum");
TDCLDEF char TEXT23[NOSZ_]  TINIT_("[struo tabulam ligationum]");
TDCLDEF char TEXT24[NOSZ_]  TINIT_("apotheca utenda");
TDCLDEF char TEXT25[NOSZ_]  TINIT_("apotheca utenda: ");
TDCLDEF char TEXT26[NOSZ_]  TINIT_("apotheca occidenda");
TDCLDEF char TEXT27[NOSZ_]  TINIT_("apotheca occidenda: ");
TDCLDEF char TEXT28[NOSZ_]  TINIT_("apotheca ostenditur");
TDCLDEF char TEXT29[NOSZ_]  TINIT_("mutare nomen apothecae ad: ");
TDCLDEF char TEXT30[NOSZ_]  TINIT_("ACTN   Modi        Magn Apothec         Filum");
TDCLDEF char TEXT31[NOSZ_]  TINIT_("         Modi Globales");
TDCLDEF char TEXT32[NOSZ_]  TINIT_("reicere mutationes");
TDCLDEF char TEXT33[NOSZ_]  TINIT_("linum encryptionis: ");
TDCLDEF char TEXT34[NOSZ_]  TINIT_("filum: ");
TDCLDEF char TEXT35[NOSZ_]  TINIT_("alius utor");
TDCLDEF char TEXT36[NOSZ_]  TINIT_("CLAUSTRI ERROR -- ");
TDCLDEF char TEXT37[NOSZ_]  TINIT_("peto sietne %s\n");
TDCLDEF char TEXT38[NOSZ_]  TINIT_("creo directorium %s\n");
TDCLDEF char TEXT39[NOSZ_]  TINIT_("creo %s\n");
TDCLDEF char TEXT40[NOSZ_]  TINIT_("non potui creare filum claustri");
TDCLDEF char TEXT41[NOSZ_]  TINIT_("pid est %ld\n");
TDCLDEF char TEXT42[NOSZ_]  TINIT_("lego filum claustri %s\n");
TDCLDEF char TEXT43[NOSZ_]  TINIT_("non potui legere filum claustri");
TDCLDEF char TEXT44[NOSZ_]  TINIT_("pid in %s est %ld\n");
TDCLDEF char TEXT45[NOSZ_]  TINIT_("signum do de processu %ld\n");
TDCLDEF char TEXT46[NOSZ_]  TINIT_("processus exsistet");
TDCLDEF char TEXT47[NOSZ_]  TINIT_("occisio mala erat");
TDCLDEF char TEXT48[NOSZ_]  TINIT_("occisio bona erat; processus exsistet");
TDCLDEF char TEXT49[NOSZ_]  TINIT_("cono solvere %s\n");
TDCLDEF char TEXT50[NOSZ_]  TINIT_("non potui removere filum claustri");
TDCLDEF char TEXT51[NOSZ_]  TINIT_("variabilis definiendus: ");
TDCLDEF char TEXT52[NOSZ_]  TINIT_("%%non est variabilis '%s'");
TDCLDEF char TEXT53[NOSZ_]  TINIT_("Valor: ");
TDCLDEF char TEXT54[NOSZ_]  TINIT_("[interrupistis macronem]");
TDCLDEF char TEXT55[NOSZ_]  TINIT_("variabilis ostendendus: ");
TDCLDEF char TEXT56[NOSZ_]  TINIT_("tabula variabilium");
TDCLDEF char TEXT57[NOSZ_]  TINIT_("non possum variabilium ostendere tabula");
TDCLDEF char TEXT58[NOSZ_]  TINIT_("[struo tabula variabilium]");
TDCLDEF char TEXT59[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT60[NOSZ_]  TINIT_("Linea %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x");
TDCLDEF char TEXT61[NOSZ_]  TINIT_("%%argumentum negativum occisionus nefas est");
TDCLDEF char TEXT62[NOSZ_]  TINIT_("modus globalis ");
TDCLDEF char TEXT63[NOSZ_]  TINIT_("modus ");
TDCLDEF char TEXT64[NOSZ_]  TINIT_("addendus: ");
TDCLDEF char TEXT65[NOSZ_]  TINIT_("delendus: ");
TDCLDEF char TEXT66[NOSZ_]  TINIT_("non est iste modus!");
TDCLDEF char TEXT67[NOSZ_]  TINIT_("nuntius scribendus: ");
TDCLDEF char TEXT68[NOSZ_]  TINIT_("linum inserendum: ");
TDCLDEF char TEXT69[NOSZ_]  TINIT_("linum superscribendum: ");
TDCLDEF char TEXT70[NOSZ_]  TINIT_("[duplicavi regionem]");
TDCLDEF char TEXT71[NOSZ_]  TINIT_("%%hic apotheca jam contracta est");
TDCLDEF char TEXT72[NOSZ_]  TINIT_("%%debes contrahere certe 1 integra linea");
TDCLDEF char TEXT73[NOSZ_]  TINIT_("[contraxi apothecam]");
TDCLDEF char TEXT74[NOSZ_]  TINIT_("%%hic apotheca non contracta est");
TDCLDEF char TEXT75[NOSZ_]  TINIT_("[dilatavi apothecam]");
TDCLDEF char TEXT76[NOSZ_]  TINIT_("Nullum indicium definitum est in hac fenestra");
TDCLDEF char TEXT77[NOSZ_]  TINIT_("Emacs erravit: indicium perditum est");
TDCLDEF char TEXT78[NOSZ_]  TINIT_("petere ");
TDCLDEF char TEXT79[NOSZ_]  TINIT_("non inveni");
TDCLDEF char TEXT80[NOSZ_]  TINIT_("Nullum exemplar definitum est");
TDCLDEF char TEXT81[NOSZ_]  TINIT_("petere retro ");
#define TEXT82  UNUSED                          /* "respicio %d cum saltu %d" */
#define TEXT83  UNUSED                          /* "ejus alius casus est %d" 'instance' vel ut A/a B/b etc? */
TDCLDEF char TEXT84[NOSZ_]  TINIT_("in loco hujus ");
TDCLDEF char TEXT85[NOSZ_]  TINIT_("quaerens, in loco hujus ");
TDCLDEF char TEXT86[NOSZ_]  TINIT_("ponere hoc ");
TDCLDEF char TEXT87[NOSZ_]  TINIT_("in loco hujus: '");
TDCLDEF char TEXT88[NOSZ_]  TINIT_("' ponere hoc: '");
TDCLDEF char TEXT89[NOSZ_]  TINIT_("interrupistis!");
TDCLDEF char TEXT90[NOSZ_]  TINIT_("Y=ita,N=non,!=fac rest.,U=solve prec.,^G=exire,.=exire retro,?=adjuv.: ");
TDCLDEF char TEXT91[NOSZ_]  TINIT_("linum vacuum substitui, consisto igitur.");
TDCLDEF char TEXT92[NOSZ_]  TINIT_("%D substitutiones");
TDCLDEF char TEXT93[NOSZ_]  TINIT_("%%ERROR dum delebam");
TDCLDEF char TEXT94[NOSZ_]  TINIT_("%%memoria est plena");
TDCLDEF char TEXT95[NOSZ_]  TINIT_("%%mceq: quid est %d?");
TDCLDEF char TEXT96[NOSZ_]  TINIT_("%%nulli characteres in ordone characterum");
TDCLDEF char TEXT97[NOSZ_]  TINIT_("%%ordo characterum non finitus est");
TDCLDEF char TEXT98[NOSZ_]  TINIT_("columna impletionis non definita est");
TDCLDEF char TEXT99[NOSZ_]  TINIT_("[MEMORIA EST PLENA]");
TDCLDEF char TEXT100[NOSZ_] TINIT_("Verba %D Char'es %D Lineae %d Medium char'um/verba %f");
TDCLDEF char TEXT101[NOSZ_] TINIT_("[non possum simul petere et adire!]");
TDCLDEF char TEXT102[NOSZ_] TINIT_("[argumentum falsum adeundum]");
TDCLDEF char TEXT103[NOSZ_] TINIT_("[conservo %s]");
TDCLDEF char TEXT104[NOSZ_] TINIT_("etsi apothecae mutatae sunt, exeo");
TDCLDEF char TEXT105[NOSZ_] TINIT_("%%macro jam currit");
TDCLDEF char TEXT106[NOSZ_] TINIT_("[macronis initium]");
TDCLDEF char TEXT107[NOSZ_] TINIT_("%%macro non currit");
TDCLDEF char TEXT108[NOSZ_] TINIT_("[macronis finis]");
TDCLDEF char TEXT109[NOSZ_] TINIT_("[clavis illegalis est in modo VIEW]");
TDCLDEF char TEXT110[NOSZ_] TINIT_("[istum jussum RESTRICTUS est]");
TDCLDEF char TEXT111[NOSZ_] TINIT_("non denotavistis macronem");
TDCLDEF char TEXT112[NOSZ_] TINIT_("macronis numerus non validus est");
TDCLDEF char TEXT113[NOSZ_] TINIT_("non possum creare macronem");
TDCLDEF char TEXT114[NOSZ_] TINIT_("Procedurae nomen: ");
TDCLDEF char TEXT115[NOSZ_] TINIT_("oboedire proceduram ");
TDCLDEF char TEXT116[NOSZ_] TINIT_("non est ista procedura");
TDCLDEF char TEXT117[NOSZ_] TINIT_("oboedire apothecam: ");
TDCLDEF char TEXT118[NOSZ_] TINIT_("non est ista apotheca");
TDCLDEF char TEXT119[NOSZ_] TINIT_("%%memoria est plena dum petebam de WHILE");
TDCLDEF char TEXT120[NOSZ_] TINIT_("%%!BREAK sine circumdante circulo !WHILE");
TDCLDEF char TEXT121[NOSZ_] TINIT_("%%!ENDWHILE sine coeunde !WHILE in '%s'");
TDCLDEF char TEXT122[NOSZ_] TINIT_("%%!WHILE sine coeunde !ENDWHILE in '%s'");
TDCLDEF char TEXT123[NOSZ_] TINIT_("%%memoria est plena dum oboedio macronem");
TDCLDEF char TEXT124[NOSZ_] TINIT_("%%directivus incognitus");
TDCLDEF char TEXT125[NOSZ_] TINIT_("memoria est plena dum reponabam macronem");
TDCLDEF char TEXT126[NOSZ_] TINIT_("%%error internalis dum oboedio circulum WHILE");
TDCLDEF char TEXT127[NOSZ_] TINIT_("%%non est istum labellum");
TDCLDEF char TEXT128[NOSZ_] TINIT_("(e)val exp,(c/x)=jussum,(t)=sequi exp,(^G)exire,<SP>=oboed,<META>=sistere debug");
TDCLDEF char TEXT129[NOSZ_] TINIT_("filum oboediendum: ");
TDCLDEF char TEXT130[NOSZ_] TINIT_("iste macro non definitus est");
TDCLDEF char TEXT131[NOSZ_] TINIT_("filum legendum");
TDCLDEF char TEXT132[NOSZ_] TINIT_("filum Inserendum");
TDCLDEF char TEXT133[NOSZ_] TINIT_("filum inveniendum");
TDCLDEF char TEXT134[NOSZ_] TINIT_("filum adspiciendum");
TDCLDEF char TEXT135[NOSZ_] TINIT_("[apotheca vetus]");
TDCLDEF char TEXT136[NOSZ_] TINIT_("nomen apothecae: ");
TDCLDEF char TEXT137[NOSZ_] TINIT_("non possum creare apothecam");
TDCLDEF char TEXT138[NOSZ_] TINIT_("[filum novum]");
TDCLDEF char TEXT139[NOSZ_] TINIT_("[lego filum]");
TDCLDEF char TEXT140[NOSZ_] TINIT_("legere ");
TDCLDEF char TEXT141[NOSZ_] TINIT_("ERROR LEGENDI VEL SCRIBENDI, ");
TDCLDEF char TEXT142[NOSZ_] TINIT_("MEMORIA PLENA EST, ");
TDCLDEF char TEXT143[NOSZ_] TINIT_(" linea");
TDCLDEF char TEXT144[NOSZ_] TINIT_("scribere filum: ");
TDCLDEF char TEXT145[NOSZ_] TINIT_("nullum fili nomen");
TDCLDEF char TEXT146[NOSZ_] TINIT_("truncavi filum..escribetur");
TDCLDEF char TEXT147[NOSZ_] TINIT_("contraxi apothecam..escribetur");
TDCLDEF char TEXT148[NOSZ_] TINIT_("[scribo...]");
TDCLDEF char TEXT149[NOSZ_] TINIT_("[scripsi ");
TDCLDEF char TEXT150[NOSZ_] TINIT_(", reposui id ut ");
TDCLDEF char TEXT151[NOSZ_] TINIT_("nomen: ");
TDCLDEF char TEXT152[NOSZ_] TINIT_("[non est istud filum]");
TDCLDEF char TEXT153[NOSZ_] TINIT_("[Insero filum]");
TDCLDEF char TEXT154[NOSZ_] TINIT_("Insevi ");
TDCLDEF char TEXT155[NOSZ_] TINIT_("non possum aperire filum scriptioni");
TDCLDEF char TEXT156[NOSZ_] TINIT_("erravi claudens filum");
TDCLDEF char TEXT157[NOSZ_] TINIT_("erravi scribens");
TDCLDEF char TEXT158[NOSZ_] TINIT_("errevi legens filum");
TDCLDEF char TEXT159[NOSZ_] TINIT_("%necesse est numerus clavis functionis");
TDCLDEF char TEXT160[NOSZ_] TINIT_("%numerus clavis functionis invalidus est");
TDCLDEF char TEXT161[NOSZ_] TINIT_("quod est in labello: ");  /* quid est 'label' Latine recte? */
TDCLDEF char TEXT162[NOSZ_] TINIT_(" [y(ita)/n(non)]? ");
TDCLDEF char TEXT163[NOSZ_] TINIT_("no default");
TDCLDEF char TEXT164[NOSZ_] TINIT_("[petens non inveni]");
TDCLDEF char TEXT165[NOSZ_] TINIT_("petere incrementaliter: ");
TDCLDEF char TEXT166[NOSZ_] TINIT_("? linum petendum nimis longum est");
TDCLDEF char TEXT167[NOSZ_] TINIT_("? jussus nimis longus est");
TDCLDEF char TEXT168[NOSZ_] TINIT_("%%non possum inserere linum");
TDCLDEF char TEXT169[NOSZ_] TINIT_("insevi");            /* non nunc usum */
TDCLDEF char TEXT170[NOSZ_] TINIT_("Emacs erravit in 'linsert'");
TDCLDEF char TEXT171[NOSZ_] TINIT_("substitui");         /* non nunc usum*/
TDCLDEF char TEXT172[NOSZ_] TINIT_("%%memoria est plena dum superscribo");
TDCLDEF char TEXT173[NOSZ_] TINIT_("ERROR CLAUSTRI: claustri tabula plena est");
TDCLDEF char TEXT174[NOSZ_] TINIT_("non possum claustrare, memoria est plena");
TDCLDEF char TEXT175[NOSZ_] TINIT_("CLAUSTRUM");
TDCLDEF char TEXT176[NOSZ_] TINIT_("filum utitur ");
TDCLDEF char TEXT177[NOSZ_] TINIT_(", eum superare?");
TDCLDEF char TEXT178[NOSZ_] TINIT_("[non possum capere nuntius erroris systemae]");
TDCLDEF char TEXT179[NOSZ_] TINIT_("  de systema MicroEmacs");
TDCLDEF char TEXT180[NOSZ_] TINIT_("%%non est ista resolutio");
TDCLDEF char TEXT181[NOSZ_] TINIT_("%%ista resolutio nefas est in hac monitore");
TDCLDEF char TEXT182[NOSZ_] TINIT_("variabilis conditionis systemae TERM non definitus est!");
TDCLDEF char TEXT183[NOSZ_] TINIT_("Non sapio terminalium genus %s!");
TDCLDEF char TEXT184[NOSZ_] TINIT_("inscriptio termcap non integra est (de lineis)");
TDCLDEF char TEXT185[NOSZ_] TINIT_("inscriptio termcap non integra est (de columnis)");
TDCLDEF char TEXT186[NOSZ_] TINIT_("inscriptio termcap non integra\n");
TDCLDEF char TEXT187[NOSZ_] TINIT_("terminalis descriptio nimis magna est!\n");
TDCLDEF char TEXT188[NOSZ_] TINIT_("[finis]");
TDCLDEF char TEXT189[NOSZ_] TINIT_("non possum invenire inscriptionem terminalis generis.\n");
TDCLDEF char TEXT190[NOSZ_] TINIT_("verifica terminalis genus scribens \"SHOW TERMINAL\" aut\n");
TDCLDEF char TEXT191[NOSZ_] TINIT_("cona definire scribens \"SET TERMINAL/INQUIRE\"\n");
TDCLDEF char TEXT192[NOSZ_] TINIT_("hoc terminalium genus non potest ferre MicroEMACS.\n");
TDCLDEF char TEXT193[NOSZ_] TINIT_("Cona uti aliud terminale, aut verifica terminalis genus\n");
TDCLDEF char TEXT194[NOSZ_] TINIT_("scribens \"SHOW TERMINAL\".\n");
TDCLDEF char TEXT195[NOSZ_] TINIT_("non possum aperire canalem ad terminale.\n");
TDCLDEF char TEXT196[NOSZ_] TINIT_("non possum capere terminalis positiones.\n");
TDCLDEF char TEXT197[NOSZ_] TINIT_("non possum mutare terminalis positiones.\n");
TDCLDEF char TEXT198[NOSZ_] TINIT_("erravi legens aut scribens (%d,%d)\n");
TDCLDEF char TEXT199[NOSZ_] TINIT_("[incipio DCL]\r\n");
TDCLDEF char TEXT200[NOSZ_] TINIT_("[advoco DCL]\r\n");
TDCLDEF char TEXT201[NOSZ_] TINIT_("[non jam possum istud facere sub VMS]");
TDCLDEF char TEXT202[NOSZ_] TINIT_("terminalis genus non est 'vt52' aut 'z19' !");
TDCLDEF char TEXT203[NOSZ_] TINIT_("fenestrae numerus invalidus est");
TDCLDEF char TEXT204[NOSZ_] TINIT_("non possum delere hanc fenestram");
TDCLDEF char TEXT205[NOSZ_] TINIT_("non possum scindere fenestram habentem solum %d lineas");
TDCLDEF char TEXT206[NOSZ_] TINIT_("est una sola fenestra");
TDCLDEF char TEXT207[NOSZ_] TINIT_("non possum sic mutare");
TDCLDEF char TEXT208[NOSZ_] TINIT_("[ista fenestra non est]");
TDCLDEF char TEXT209[NOSZ_] TINIT_("%%ostendaculum est nimis magnum");  /* quid est 'screen'? */
TDCLDEF char TEXT210[NOSZ_] TINIT_("%%ostendaculum est nimis latum");  /* quid est 'screen'? */
TDCLDEF char TEXT211[NOSZ_] TINIT_("Functionum tabula");
TDCLDEF char TEXT212[NOSZ_] TINIT_("non possum ostendere functionum tabulam");
TDCLDEF char TEXT213[NOSZ_] TINIT_("[struo functionum tabulam]");
TDCLDEF char TEXT214[NOSZ_] TINIT_("%%non est filum %s");
TDCLDEF char TEXT215[NOSZ_] TINIT_(": macro-ad-clavem ");
TDCLDEF char TEXT216[NOSZ_] TINIT_("non possum legere vel scribere directoria!!!");
TDCLDEF char TEXT217[NOSZ_] TINIT_("[non possum adhuc istud facere sub AOS/VS]");
TDCLDEF char TEXT218[NOSZ_] TINIT_("Append file: ");
TDCLDEF char TEXT219[NOSZ_] TINIT_("%%Macro Failed");
TDCLDEF char TEXT220[NOSZ_] TINIT_("Linea %D/%D Col %d/%d Char %D/%D (%d%%) char = 0x%x%x");
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

/**TODO: Translate to Latin**/
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
#endif/**#ifndef LATIN_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
