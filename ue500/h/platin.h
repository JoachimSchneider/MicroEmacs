/*======================================================================
 *      PLATIN.H        Pig Latin Text strings for
 *                       MicroEMACS 4.00
 *
 *                      (C)opyright 1995 by Daniel Lawrence
 *                      translated by Daniel Lawrence
 *                      (using a complex but neat MicroEMACS macro)
 *====================================================================*/

/*====================================================================*/
#ifndef PLATIN_H_
#define PLATIN_H_
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

TDCLDEF char TEXT1[NOSZ_]   TINIT_("[Tartingsay ewnay LICAY]");
TDCLDEF char TEXT2[NOSZ_]   TINIT_("[Annotcay riteway ilterfay ilefay]");
TDCLDEF char TEXT3[NOSZ_]   TINIT_("[Executionway ailedfay]");
TDCLDEF char TEXT4[NOSZ_]   TINIT_("Hellsay ariablevay ERMTAY otnay efinedday!");
TDCLDEF char TEXT5[NOSZ_]   TINIT_("Erminaltay ypetay otnay 't100vay'!");
TDCLDEF char TEXT6[NOSZ_]   TINIT_("\r\n\n[Endway]");
TDCLDEF char TEXT7[NOSZ_]   TINIT_("Inelay otay OTOGAY: ");
TDCLDEF char TEXT8[NOSZ_]   TINIT_("[Abortedway]");
TDCLDEF char TEXT9[NOSZ_]   TINIT_("[Arkmay %d etsay]");
TDCLDEF char TEXT10[NOSZ_]  TINIT_("[Arkmay %d emovedray]");
TDCLDEF char TEXT11[NOSZ_]  TINIT_("Onay arkmay %d inway histay indowway");
TDCLDEF char TEXT12[NOSZ_]  TINIT_("[Elphay ilefay isway otnay onlineway]");
TDCLDEF char TEXT13[NOSZ_]  TINIT_(": escribeday-eykay ");
TDCLDEF char TEXT14[NOSZ_]  TINIT_("Otnay Oundbay");
TDCLDEF char TEXT15[NOSZ_]  TINIT_(": indbay-otay-eykay ");
TDCLDEF char TEXT16[NOSZ_]  TINIT_("[Onay uchsay unctionfay]");
TDCLDEF char TEXT17[NOSZ_]  TINIT_("Indingbay abletay ULLFAY!");
TDCLDEF char TEXT18[NOSZ_]  TINIT_(": unbindway-eykay ");
TDCLDEF char TEXT19[NOSZ_]  TINIT_("[Eykay otnay oundbay]");
TDCLDEF char TEXT20[NOSZ_]  TINIT_("Aproposway tringsay: ");
TDCLDEF char TEXT21[NOSZ_]  TINIT_("Indingbay istlay");
TDCLDEF char TEXT22[NOSZ_]  TINIT_("Ancay otnay isplayday indingbay istlay");
TDCLDEF char TEXT23[NOSZ_]  TINIT_("[Uildingbay indingbay istlay]");
TDCLDEF char TEXT24[NOSZ_]  TINIT_("Useway ufferbay");
TDCLDEF char TEXT25[NOSZ_]  TINIT_("Useway ufferbay: ");
TDCLDEF char TEXT26[NOSZ_]  TINIT_("Illkay ufferbay");
TDCLDEF char TEXT27[NOSZ_]  TINIT_("Oppay ufferbay");
TDCLDEF char TEXT28[NOSZ_]  TINIT_("Ufferbay isway eingbay isplayedday");
TDCLDEF char TEXT29[NOSZ_]  TINIT_("Hangecay ufferbay amenay otay: ");
TDCLDEF char TEXT30[NOSZ_]  TINIT_("ACTN   Odesmay   Izesay Ufferbay        Ilefay");
TDCLDEF char TEXT31[NOSZ_]  TINIT_("         Global Modes");
TDCLDEF char TEXT32[NOSZ_]  TINIT_("Iscardday hangescay");
TDCLDEF char TEXT33[NOSZ_]  TINIT_("Encryptionway Tringsay: ");
TDCLDEF char TEXT34[NOSZ_]  TINIT_("Ilefay: ");
TDCLDEF char TEXT35[NOSZ_]  TINIT_("anotherway userway");
TDCLDEF char TEXT36[NOSZ_]  TINIT_("OCKLAY ERRORWAY -- ");
TDCLDEF char TEXT37[NOSZ_]  TINIT_("heckingcay orfay existenceway ofway %s\n");
TDCLDEF char TEXT38[NOSZ_]  TINIT_("akingmay irectoryday %s\n");
TDCLDEF char TEXT39[NOSZ_]  TINIT_("reatingcay %s\n");
TDCLDEF char TEXT40[NOSZ_]  TINIT_("ouldcay otnay reatecay ocklay ilefay");
TDCLDEF char TEXT41[NOSZ_]  TINIT_("idpay isway %ld\n");
TDCLDEF char TEXT42[NOSZ_]  TINIT_("eadingray ocklay ilefay %s\n");
TDCLDEF char TEXT43[NOSZ_]  TINIT_("ouldcay otnay eadray ocklay ilefay");
TDCLDEF char TEXT44[NOSZ_]  TINIT_("idpay inway %s isway %ld\n");
TDCLDEF char TEXT45[NOSZ_]  TINIT_("ignalingsay rocesspay %ld\n");
TDCLDEF char TEXT46[NOSZ_]  TINIT_("rocesspay existsway");
TDCLDEF char TEXT47[NOSZ_]  TINIT_("illkay asway adbay");
TDCLDEF char TEXT48[NOSZ_]  TINIT_("illkay asway oodgay; rocesspay existsway");
TDCLDEF char TEXT49[NOSZ_]  TINIT_("attemptingway otay unlinkway %s\n");
TDCLDEF char TEXT50[NOSZ_]  TINIT_("ouldcay otnay emoveray ocklay ilefay");
TDCLDEF char TEXT51[NOSZ_]  TINIT_("Ariablevay otay etsay: ");
TDCLDEF char TEXT52[NOSZ_]  TINIT_("%%Onay uchsay ariablevay asway '%s'");
TDCLDEF char TEXT53[NOSZ_]  TINIT_("Aluevay: ");
TDCLDEF char TEXT54[NOSZ_]  TINIT_("[Acromay abortedway]");
TDCLDEF char TEXT55[NOSZ_]  TINIT_("Ariablevay otay isplayday: ");
TDCLDEF char TEXT56[NOSZ_]  TINIT_("Ariablevay istlay");
TDCLDEF char TEXT57[NOSZ_]  TINIT_("Ancay otnay isplayday ariablevay istlay");
TDCLDEF char TEXT58[NOSZ_]  TINIT_("[Uildingbay ariablevay istlay]");
#define TEXT59  UNUSEDWAY
TDCLDEF char TEXT60[NOSZ_]  TINIT_("Inelay %D/%D Olcay %d/%d Archay %D/%D (%d%%) archay = 0x%x");
TDCLDEF char TEXT61[NOSZ_]  TINIT_("<OTNAY USEDWAY>");
TDCLDEF char TEXT62[NOSZ_]  TINIT_("Lobalgay odemay otay ");
TDCLDEF char TEXT63[NOSZ_]  TINIT_("Odemay otay ");
TDCLDEF char TEXT64[NOSZ_]  TINIT_("addway: ");
TDCLDEF char TEXT65[NOSZ_]  TINIT_("eleteday: ");
TDCLDEF char TEXT66[NOSZ_]  TINIT_("Onay uchsay odemay!");
TDCLDEF char TEXT67[NOSZ_]  TINIT_("Essagemay otay riteway: ");
TDCLDEF char TEXT68[NOSZ_]  TINIT_("Tringsay otay insertway: ");
TDCLDEF char TEXT69[NOSZ_]  TINIT_("Tringsay otay overwriteway: ");
TDCLDEF char TEXT70[NOSZ_]  TINIT_("[egionray opiedcay]");
TDCLDEF char TEXT71[NOSZ_]  TINIT_("%%Histay ufferbay isway alreadyway arrowednay");
TDCLDEF char TEXT72[NOSZ_]  TINIT_("%%Ustmay arrownay atway eastlay 1 ullfay inelay");
TDCLDEF char TEXT73[NOSZ_]  TINIT_("[Ufferbay isway arrowednay]");
TDCLDEF char TEXT74[NOSZ_]  TINIT_("%%Histay ufferbay isway otnay arrowednay");
TDCLDEF char TEXT75[NOSZ_]  TINIT_("[Ufferbay isway idenedway]");
TDCLDEF char TEXT76[NOSZ_]  TINIT_("Onay arkmay etsay inway histay indowway");
TDCLDEF char TEXT77[NOSZ_]  TINIT_("Ugbay: ostlay arkmay");
TDCLDEF char TEXT78[NOSZ_]  TINIT_("Earchsay ");
TDCLDEF char TEXT79[NOSZ_]  TINIT_("Otnay oundfay");
TDCLDEF char TEXT80[NOSZ_]  TINIT_("Onay atternpay etsay");
TDCLDEF char TEXT81[NOSZ_]  TINIT_("Everseray earchsay ");
TDCLDEF char TEXT82[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT83[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT84[NOSZ_]  TINIT_("Eplaceray ");
TDCLDEF char TEXT85[NOSZ_]  TINIT_("Ueryqay eplaceray ");
TDCLDEF char TEXT86[NOSZ_]  TINIT_("ithway ");
TDCLDEF char TEXT87[NOSZ_]  TINIT_("Eplaceray '");
TDCLDEF char TEXT88[NOSZ_]  TINIT_("' ithway '");
TDCLDEF char TEXT89[NOSZ_]  TINIT_("Abortedway!");
TDCLDEF char TEXT90[NOSZ_]  TINIT_("(Y)es, (N)o, (!)Do rest, (U)ndo last, (^G)Abort, (.)Abort back, (?)Help: ");
TDCLDEF char TEXT91[NOSZ_]  TINIT_("Emptyway tringsay eplacedray, toppingsay.");
TDCLDEF char TEXT92[NOSZ_]  TINIT_("%D ubstitutionssay");
TDCLDEF char TEXT93[NOSZ_]  TINIT_("%%ERRORWAY hileway eletingday");
TDCLDEF char TEXT94[NOSZ_]  TINIT_("%%Outway ofway emorymay");
TDCLDEF char TEXT95[NOSZ_]  TINIT_("%%mceq: hatway isway %d?");
TDCLDEF char TEXT96[NOSZ_]  TINIT_("%%Onay haracterscay inway haractercay lasscay");
TDCLDEF char TEXT97[NOSZ_]  TINIT_("%%Haractercay lasscay otnay endedway");
TDCLDEF char TEXT98[NOSZ_]  TINIT_("Onay illfay olumncay etsay");
TDCLDEF char TEXT99[NOSZ_]  TINIT_("[OUTWAY OFWAY EMORYMAY]");
TDCLDEF char TEXT100[NOSZ_] TINIT_("Ordsway %D Harscay %D Ineslay %d Avgway harscay/ordway %f");
TDCLDEF char TEXT101[NOSZ_] TINIT_("[Ancay otnay earchsay andway otogay atway hetay amesay imetay!]");
TDCLDEF char TEXT102[NOSZ_] TINIT_("[Ogusbay otogay argumentway]");
TDCLDEF char TEXT103[NOSZ_] TINIT_("[Avingsay %s]");
TDCLDEF char TEXT104[NOSZ_] TINIT_("Odifiedmay uffersbay existway. Eavelay anywayway");
TDCLDEF char TEXT105[NOSZ_] TINIT_("%%Acromay alreadyway activeway");
TDCLDEF char TEXT106[NOSZ_] TINIT_("[Tartsay acromay]");
TDCLDEF char TEXT107[NOSZ_] TINIT_("%%Acromay otnay activeway");
TDCLDEF char TEXT108[NOSZ_] TINIT_("[Endway acromay]");
TDCLDEF char TEXT109[NOSZ_] TINIT_("[Eykay illegalway inway Iewvay odemay]");
TDCLDEF char TEXT110[NOSZ_] TINIT_("[Hattay ommandcay isway ESTRICTEDRAY]");
TDCLDEF char TEXT111[NOSZ_] TINIT_("Onay acromay pecifiedsay");
TDCLDEF char TEXT112[NOSZ_] TINIT_("Acromay umbernay outway ofway angeray");
TDCLDEF char TEXT113[NOSZ_] TINIT_("Ancay otnay reatecay acromay");
TDCLDEF char TEXT114[NOSZ_] TINIT_("Rocedurepay amenay: ");
TDCLDEF char TEXT115[NOSZ_] TINIT_("Executeway rocedurepay: ");
TDCLDEF char TEXT116[NOSZ_] TINIT_("Onay uchsay rocedurepay");
TDCLDEF char TEXT117[NOSZ_] TINIT_("Executeway ufferbay: ");
TDCLDEF char TEXT118[NOSZ_] TINIT_("Onay uchsay ufferbay");
TDCLDEF char TEXT119[NOSZ_] TINIT_("%%Outway ofway emorymay uringday hileway cansay");
TDCLDEF char TEXT120[NOSZ_] TINIT_("%%!REAKBAY outsideway ofway anyway !HILEWAY ooplay");
TDCLDEF char TEXT121[NOSZ_] TINIT_("%%!ENDWHILEWAY ithway onay recedingpay !HILEWAY");
TDCLDEF char TEXT122[NOSZ_] TINIT_("%%!HILEWAY ithway onay atchingmay !ENDWHILEWAY");
TDCLDEF char TEXT123[NOSZ_] TINIT_("%%Outway ofway Emorymay uringday acromay executionway");
TDCLDEF char TEXT124[NOSZ_] TINIT_("%%Unknownway Irectiveday");
TDCLDEF char TEXT125[NOSZ_] TINIT_("Outway ofway emorymay hileway toringsay acromay");
TDCLDEF char TEXT126[NOSZ_] TINIT_("%%Internalway Hileway ooplay errorway");
TDCLDEF char TEXT127[NOSZ_] TINIT_("%%Onay uchsay abellay");
TDCLDEF char TEXT128[NOSZ_] TINIT_("(e)val exp, (c/x)ommand, (t)rack exp, (^G)abort, <SP>exec, <META> stop debug");
TDCLDEF char TEXT129[NOSZ_] TINIT_("Ilefay otay executeway: ");
TDCLDEF char TEXT130[NOSZ_] TINIT_("Acromay otnay efinedday");
TDCLDEF char TEXT131[NOSZ_] TINIT_("Eadray ilefay");
TDCLDEF char TEXT132[NOSZ_] TINIT_("Insertway ilefay");
TDCLDEF char TEXT133[NOSZ_] TINIT_("Indfay ilefay");
TDCLDEF char TEXT134[NOSZ_] TINIT_("Iewvay ilefay");
TDCLDEF char TEXT135[NOSZ_] TINIT_("[Oldway ufferbay]");
TDCLDEF char TEXT136[NOSZ_] TINIT_("Ufferbay amenay: ");
TDCLDEF char TEXT137[NOSZ_] TINIT_("Annotcay reatecay ufferbay");
TDCLDEF char TEXT138[NOSZ_] TINIT_("[Ewnay ilefay]");
TDCLDEF char TEXT139[NOSZ_] TINIT_("[Eadingray ilefay]");
TDCLDEF char TEXT140[NOSZ_] TINIT_("Eadray ");
TDCLDEF char TEXT141[NOSZ_] TINIT_("I/O ERRORWAY, ");
TDCLDEF char TEXT142[NOSZ_] TINIT_("OUTWAY OFWAY EMORYMAY, ");
TDCLDEF char TEXT143[NOSZ_] TINIT_(" inelay");
TDCLDEF char TEXT144[NOSZ_] TINIT_("Riteway ilefay: ");
TDCLDEF char TEXT145[NOSZ_] TINIT_("Onay ilefay amenay");
TDCLDEF char TEXT146[NOSZ_] TINIT_("Runcatedtay ilefay..riteway itway outway");
TDCLDEF char TEXT147[NOSZ_] TINIT_("Arrowednay Ufferbay..riteway itway outway");
TDCLDEF char TEXT148[NOSZ_] TINIT_("[Ritingway...]");
TDCLDEF char TEXT149[NOSZ_] TINIT_("[Roteway ");
TDCLDEF char TEXT150[NOSZ_] TINIT_(", avedsay asway ");
TDCLDEF char TEXT151[NOSZ_] TINIT_("Amenay: ");
TDCLDEF char TEXT152[NOSZ_] TINIT_("[Onay uchsay ilefay]");
TDCLDEF char TEXT153[NOSZ_] TINIT_("[Insertingway ilefay]");
TDCLDEF char TEXT154[NOSZ_] TINIT_("Insertedway ");
TDCLDEF char TEXT155[NOSZ_] TINIT_("Annotcay openway ilefay orfay ritingway");
TDCLDEF char TEXT156[NOSZ_] TINIT_("Errorway losingcay ilefay");
TDCLDEF char TEXT157[NOSZ_] TINIT_("Riteway I/O errorway");
TDCLDEF char TEXT158[NOSZ_] TINIT_("Ilefay eadray errorway");
TDCLDEF char TEXT159[NOSZ_] TINIT_("%Eednay unctionfay eykay umbernay");
TDCLDEF char TEXT160[NOSZ_] TINIT_("%Unctionfay eykay umbernay outway ofway angeray");
TDCLDEF char TEXT161[NOSZ_] TINIT_("Abellay ontentscay: ");
TDCLDEF char TEXT162[NOSZ_] TINIT_(" [y/n]? ");
TDCLDEF char TEXT163[NOSZ_] TINIT_("onay efaultday");
TDCLDEF char TEXT164[NOSZ_] TINIT_("[earchsay ailedfay]");
TDCLDEF char TEXT165[NOSZ_] TINIT_("Isearchway: ");
TDCLDEF char TEXT166[NOSZ_] TINIT_("? Earchsay tringsay ootay onglay");
TDCLDEF char TEXT167[NOSZ_] TINIT_("? ommandcay ootay onglay");
TDCLDEF char TEXT168[NOSZ_] TINIT_("%%Ancay otnay insertway tringsay");
TDCLDEF char TEXT169[NOSZ_] TINIT_("Insertedway");       /* this not used anymore */
TDCLDEF char TEXT170[NOSZ_] TINIT_("ugbay: insertlay");
TDCLDEF char TEXT171[NOSZ_] TINIT_("Eplacedray");        /* this not used anymore */
TDCLDEF char TEXT172[NOSZ_] TINIT_("%%Outway ofway emorymay hileway overwritingway");
TDCLDEF char TEXT173[NOSZ_] TINIT_("OCKLAY ERRORWAY: Ocklay abletay ullfay");
TDCLDEF char TEXT174[NOSZ_] TINIT_("Annotcay ocklay, outway ofway emorymay");
TDCLDEF char TEXT175[NOSZ_] TINIT_("OCKLAY");
TDCLDEF char TEXT176[NOSZ_] TINIT_("Ilefay inway useway ybay ");
TDCLDEF char TEXT177[NOSZ_] TINIT_(", overideway?");
TDCLDEF char TEXT178[NOSZ_] TINIT_("[ancay otnay etgay ystemsay errorway essagemay]");
TDCLDEF char TEXT179[NOSZ_] TINIT_("  Aboutway Icroemacsmay");
TDCLDEF char TEXT180[NOSZ_] TINIT_("%%Onay uchsay esolutionray");
TDCLDEF char TEXT181[NOSZ_] TINIT_("%%Esolutionray illegalway orfay histay onitormay");
TDCLDEF char TEXT182[NOSZ_] TINIT_("Environmentway ariablevay ERMTAY otnay efinedday!");
TDCLDEF char TEXT183[NOSZ_] TINIT_("Unknownway erminaltay ypetay %s!");
TDCLDEF char TEXT184[NOSZ_] TINIT_("ermcaptay entryway incompleteway (ineslay)");
TDCLDEF char TEXT185[NOSZ_] TINIT_("Ermcaptay entryway incompleteway (olumnscay)");
TDCLDEF char TEXT186[NOSZ_] TINIT_("Incompleteway ermcaptay entryway\n");
TDCLDEF char TEXT187[NOSZ_] TINIT_("Erminaltay escriptionday ootay igbay!\n");
TDCLDEF char TEXT188[NOSZ_] TINIT_("[Endway]");
TDCLDEF char TEXT189[NOSZ_] TINIT_("Annotcay indfay entryway orfay erminaltay ypetay.\n");
TDCLDEF char TEXT190[NOSZ_] TINIT_("Heckcay erminaltay ypetay ithway \"SHOW TERMINAL\" orway\n");
TDCLDEF char TEXT191[NOSZ_] TINIT_("rytay ettingsay ithway \"SET TERMINAL/INQUIRE\"\n");
TDCLDEF char TEXT192[NOSZ_] TINIT_("Hetay erminaltay ypetay oesday otnay avehay enoughway owerpay otay unray\n");
TDCLDEF char TEXT193[NOSZ_] TINIT_("IcroEMACSMAY.  Rytay away ifferentday erminaltay orway heckcay\n");
TDCLDEF char TEXT194[NOSZ_] TINIT_("ypetay ithway \"SHOW TERMINAL\".\n");
TDCLDEF char TEXT195[NOSZ_] TINIT_("Annotcay openway hannelcay otay erminaltay.\n");
TDCLDEF char TEXT196[NOSZ_] TINIT_("Annotcay obtainway erminaltay ettingssay.\n");
TDCLDEF char TEXT197[NOSZ_] TINIT_("Annotcay odifymay erminaltay ettingssay.\n");
TDCLDEF char TEXT198[NOSZ_] TINIT_("I/O errorway (%d,%d)\n");
TDCLDEF char TEXT199[NOSZ_] TINIT_("[Tartingsay DCL]\r\n");
TDCLDEF char TEXT200[NOSZ_] TINIT_("[Allingcay DCL]\r\n");
TDCLDEF char TEXT201[NOSZ_] TINIT_("[Otnay availableway etyay underway VMS]");
TDCLDEF char TEXT202[NOSZ_] TINIT_("Erminaltay ypetay otnay 'vt52'orway 'z19' !");
TDCLDEF char TEXT203[NOSZ_] TINIT_("Indowway umbernay outway ofway angeray");
TDCLDEF char TEXT204[NOSZ_] TINIT_("Ancay otnay eleteday histay indowway");
TDCLDEF char TEXT205[NOSZ_] TINIT_("Annotcay plitsay away %d inelay indowway");
TDCLDEF char TEXT206[NOSZ_] TINIT_("Onlyway oneway indowway");
TDCLDEF char TEXT207[NOSZ_] TINIT_("Impossibleway hangecay");
TDCLDEF char TEXT208[NOSZ_] TINIT_("[Onay uchsay indowway existsway]");
TDCLDEF char TEXT209[NOSZ_] TINIT_("%%Creensay izesay outway ofway angeray");
TDCLDEF char TEXT210[NOSZ_] TINIT_("%%Creensay idthway outway ofway angeray");
TDCLDEF char TEXT211[NOSZ_] TINIT_("Unctionfay istlay");
TDCLDEF char TEXT212[NOSZ_] TINIT_("Ancay otnay isplayday unctionfay istlay");
TDCLDEF char TEXT213[NOSZ_] TINIT_("[Uildingbay unctionfay istlay]");
TDCLDEF char TEXT214[NOSZ_] TINIT_("%%Onay uchsay ilefay asway %s");
TDCLDEF char TEXT215[NOSZ_] TINIT_(": acromay-otay-eykay ");
TDCLDEF char TEXT216[NOSZ_] TINIT_("Annotcay eadray/riteway irectoriesday!!!");
TDCLDEF char TEXT217[NOSZ_] TINIT_("[Otnay availableway et yay underway AOS/VS]");
TDCLDEF char TEXT218[NOSZ_] TINIT_("Appendway ilefay: ");
TDCLDEF char TEXT219[NOSZ_] TINIT_("%%Acromay Ailedfay");
TDCLDEF char TEXT220[NOSZ_] TINIT_("Inelay %D/%D Olcay %d/%d Archay %D/%D (%d%%) archay = 0x%x%x");
TDCLDEF char TEXT221[NOSZ_] TINIT_("Ootay anymay oupsgray");
TDCLDEF char TEXT222[NOSZ_] TINIT_("Oupgray otnay endedway");
TDCLDEF char TEXT223[NOSZ_] TINIT_("%%Olumncay originway outway ofway angeray");
TDCLDEF char TEXT224[NOSZ_] TINIT_("%%Owray originway outway ofway angeray");
TDCLDEF char TEXT225[NOSZ_] TINIT_("[Witchedsay otay eenscray %s]");
TDCLDEF char TEXT226[NOSZ_] TINIT_("%%Ancay otnay illkay anway executingway ufferbay");
TDCLDEF char TEXT227[NOSZ_] TINIT_("\n--- Esspray anyway eykay otay Ontinuecay ---");
TDCLDEF char TEXT228[NOSZ_] TINIT_("[Illkay ingray earedclay]");
TDCLDEF char TEXT229[NOSZ_] TINIT_(" inway < ");
TDCLDEF char TEXT230[NOSZ_] TINIT_("> atay inelay ");
TDCLDEF char TEXT231[NOSZ_] TINIT_("Abbreviationway otay etsay: ");
TDCLDEF char TEXT232[NOSZ_] TINIT_("Abbreviationway otay eleteday: ");
TDCLDEF char TEXT233[NOSZ_] TINIT_("[Uildingbay Abbreviationway istlay]");
TDCLDEF char TEXT234[NOSZ_] TINIT_("Abbreviationway istlay");
TDCLDEF char TEXT235[NOSZ_] TINIT_("Ancay otnay isplayday abbreviationway istlay");
TDCLDEF char TEXT236[NOSZ_] TINIT_("Efineday Abbreviationways inway ufferbay");
TDCLDEF char TEXT240[NOSZ_] TINIT_("[Onay Uchsay Eenscray]");
TDCLDEF char TEXT241[NOSZ_] TINIT_("%%An'tcay eleteday urrentcay eenscray");
TDCLDEF char TEXT242[NOSZ_] TINIT_("Indfay Eenscray: ");
TDCLDEF char TEXT243[NOSZ_] TINIT_("Eleteday EenScray: ");
TDCLDEF char TEXT244[NOSZ_] TINIT_("%%Onay uchsay unctionfay asway '%s'");
TDCLDEF char TEXT245[NOSZ_] TINIT_("%%Ivisionday ybay Erozay isway illegalway");
TDCLDEF char TEXT246[NOSZ_] TINIT_("%%Eednay unctionfay eykay umbernay");
TDCLDEF char TEXT247[NOSZ_] TINIT_("%%Unctionfay eykay umbernay outway ofway angeray");
TDCLDEF char TEXT248[NOSZ_] TINIT_("Enterway Abelay Ingstray: ");
TDCLDEF char TEXT249[NOSZ_] TINIT_("Obalglay ariablevay otay eclareday: ");
TDCLDEF char TEXT250[NOSZ_] TINIT_("Ocallay ariablevay otay eclareday: ");

/* some of these are just used in the microsoft windows version */
TDCLDEF char TEXT300[NOSZ_] TINIT_("[Incorrectway enumay]");
TDCLDEF char TEXT301[NOSZ_] TINIT_("[Ootay anymay estednay opupay enusmay]");
TDCLDEF char TEXT302[NOSZ_] TINIT_("[Acklay ofway esourcesway]");
TDCLDEF char TEXT303[NOSZ_] TINIT_("Enumay: ");
TDCLDEF char TEXT304[NOSZ_] TINIT_("Unctionfay: ");
TDCLDEF char TEXT305[NOSZ_] TINIT_("Acromay: ");
TDCLDEF char TEXT306[NOSZ_] TINIT_("Enumay: ");
TDCLDEF char TEXT307[NOSZ_] TINIT_("Elphay ilefay: ");
TDCLDEF char TEXT308[NOSZ_] TINIT_("Elphay eykay: ");
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
TDCLDEF char TEXT330[NOSZ_] TINIT_(" - Essagemay istoryhay");
TDCLDEF char TEXT331[NOSZ_] TINIT_("Lobalgay odesmay");
TDCLDEF char TEXT332[NOSZ_] TINIT_("Odesmay orfay ufferbay: ");
TDCLDEF char TEXT333[NOSZ_] TINIT_("Ilefay itewray inway ogresspray. Itquay aterlay!");
TDCLDEF char TEXT334[NOSZ_] TINIT_("[Onay uchsay irectoryday]");
TDCLDEF char TEXT335[NOSZ_] TINIT_("Angechay eenscray amenay otay: ");
TDCLDEF char TEXT336[NOSZ_] TINIT_("[Eenscray amenay alreadyway inway useway]");
TDCLDEF char TEXT337[NOSZ_] TINIT_("annotcay onitormay externalway ogrampray");



/*====================================================================*/
#endif/**#ifndef PLATIN_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
