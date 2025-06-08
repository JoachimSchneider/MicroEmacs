/*======================================================================
 *      FRENCHDOS.H:    French language text strings for
 *                      MicroEMACS 4.00
 *
 *                      (C)opyright 1995 by Daniel Lawrence
 *                      Translated by Bertrand DECOUTY, 10/88
 *                      Modified 12/89 (accent...)
 *                      decouty@caroline.irisa.fr,
 *                      {uunet,mcvax,inria}!irisa!decouty
 *                      IRISA-INRIA, Campus de Beaulieu, F-35042 Rennes Cedex
 *                      Modified 02/93 (code page 850, 8 bits chars)
 *                      picheral@cicb.fr
 *====================================================================*/

/*====================================================================*/
#ifndef FRENCH_H_
#define FRENCH_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/

/**********************************************************************/
#include "eproto.h"
/**********************************************************************/


#define OQUOTE_CHAR     174
#define CQUOTE_CHAR     175


TDCLDEF char TEXT1[NOSZ_]   TINIT_("[Lancement d'un nouvel interpr\202teur]");  /*"[Starting new CLI]"*/
TDCLDEF char TEXT2[NOSZ_]   TINIT_("[\220criture du fichier filtre impossible]");  /*"[Cannot write filter file]"*/
TDCLDEF char TEXT3[NOSZ_]   TINIT_("[L'ex\202cution a \202chou\202]");  /*"[Execution failed]"*/
TDCLDEF char TEXT4[NOSZ_]   TINIT_("Variable TERM non d\202finie");  /*"Shell variable TERM not defined!"*/
TDCLDEF char TEXT5[NOSZ_]   TINIT_("Terminal non 'vt100'!");  /*"Terminal type not 'vt100'!"*/
TDCLDEF char TEXT6[NOSZ_]   TINIT_("\r\n\n[Termin\202]");  /*"\r\n\n[End]"*/
TDCLDEF char TEXT7[NOSZ_]   TINIT_("Aller \205 la ligne: ");  /*"Line to GOTO: "*/
TDCLDEF char TEXT8[NOSZ_]   TINIT_("[Annul\202]");       /*"[Aborted]"*/
TDCLDEF char TEXT9[NOSZ_]   TINIT_("[Marque %d pos\202e]");  /*"[Mark set]"*/
TDCLDEF char TEXT10[NOSZ_]  TINIT_("[Marque %d enlev\202e]");  /*"[Mark removed]"*/
TDCLDEF char TEXT11[NOSZ_]  TINIT_("Pas de marque %d dans cette fen\210tre");  /*"No mark %d in this window"*/
TDCLDEF char TEXT12[NOSZ_]  TINIT_("[Fichier Help absent]");  /*"[Help file is not online]"*/
TDCLDEF char TEXT13[NOSZ_]  TINIT_(": d\202crire-la-touche ");  /*": describe-key "*/
TDCLDEF char TEXT14[NOSZ_]  TINIT_("Non li\202e");       /*"Not Bound"*/
TDCLDEF char TEXT15[NOSZ_]  TINIT_(": lier-une-touche ");  /*": bind-to-key "*/
TDCLDEF char TEXT16[NOSZ_]  TINIT_("[Fonction inexistante]");  /*"[No such function]"*/
TDCLDEF char TEXT17[NOSZ_]  TINIT_("La table des liens est PLEINE!");  /*"Binding table FULL!"*/
TDCLDEF char TEXT18[NOSZ_]  TINIT_(": touche-\205-d\202lier ");  /*": unbind-key "*/
TDCLDEF char TEXT19[NOSZ_]  TINIT_("[Touche non li\202e]");  /*"[Key not bound]"*/
TDCLDEF char TEXT20[NOSZ_]  TINIT_("Cha\214ne a-propos: ");  /*"Apropos string: "*/
TDCLDEF char TEXT21[NOSZ_]  TINIT_("Liste des liens");   /*"Binding list"*/
TDCLDEF char TEXT22[NOSZ_]  TINIT_("Affichage de la liste des liens impossible");  /*"Can not display binding list"*/
TDCLDEF char TEXT23[NOSZ_]  TINIT_("[Cr\202ation de la liste des liens]");  /*"[Building binding list]"*/
TDCLDEF char TEXT24[NOSZ_]  TINIT_("Tampon \205 utiliser");  /*"Use buffer"*/
TDCLDEF char TEXT25[NOSZ_]  TINIT_("Tampon \205 utiliser: ");  /*"Use buffer: "*/
TDCLDEF char TEXT26[NOSZ_]  TINIT_("Tampon \205 d\202truire");  /*"Kill buffer"*/
TDCLDEF char TEXT27[NOSZ_]  TINIT_("Tampon \205 d\202piler: ");  /*"Pop buffer: "*/
TDCLDEF char TEXT28[NOSZ_]  TINIT_("Le tampon est affich\202");  /*"Buffer is being displayed"*/
TDCLDEF char TEXT29[NOSZ_]  TINIT_("Changer le nom du tampon en: ");  /*"Change buffer name to: "*/

/*              "ACTN   Modes      Size  Buffer          File"*/
TDCLDEF char TEXT30[NOSZ_]  TINIT_("ACTN   Modes    Taille  Tampon          Fichier");

TDCLDEF char TEXT31[NOSZ_]  TINIT_("         Modes Globaux");  /*"         Global Modes"*/
TDCLDEF char TEXT32[NOSZ_]  TINIT_("Annuler les modifications");  /*"Discard changes"*/
TDCLDEF char TEXT33[NOSZ_]  TINIT_("Cha\214ne d'encryptage: ");  /*"Encryption String: "*/
TDCLDEF char TEXT34[NOSZ_]  TINIT_("Fichier: ");         /*"File: "*/
TDCLDEF char TEXT35[NOSZ_]  TINIT_("autre utilisateur");  /*"another user"*/
TDCLDEF char TEXT36[NOSZ_]  TINIT_("ERREUR AU VERROUILLAGE");  /*"LOCK ERROR -- "*/
TDCLDEF char TEXT37[NOSZ_]  TINIT_("v\202rification de l'existence de %s\n");  /*"checking for existence of %s\n"*/
TDCLDEF char TEXT38[NOSZ_]  TINIT_("cr\202ation du r\202pertoire %s\n");  /*"making directory %s\n"*/
TDCLDEF char TEXT39[NOSZ_]  TINIT_("en cours de cr\202ation de %s\n");  /*"creating %s\n"*/
TDCLDEF char TEXT40[NOSZ_]  TINIT_("cr\202ation d'un fichier verrouill\202 impossible");  /*"could not create lock file"*/
TDCLDEF char TEXT41[NOSZ_]  TINIT_("pid: %ld\n");        /*"pid is %ld\n"*/
TDCLDEF char TEXT42[NOSZ_]  TINIT_("lecture du fichier verrouill\202 %s\n");  /*"reading lock file %s\n"*/
TDCLDEF char TEXT43[NOSZ_]  TINIT_("lecture du fichier verrouill\202 impossible");  /*"could not read lock file"*/
TDCLDEF char TEXT44[NOSZ_]  TINIT_("le pid de %s est %ld\n");  /*"pid in %s is %ld\n"*/
TDCLDEF char TEXT45[NOSZ_]  TINIT_("envoi d'un signal au processus %ld\n");  /*"signaling process %ld\n"*/
TDCLDEF char TEXT46[NOSZ_]  TINIT_("le processus existe");  /*"process exists"*/
TDCLDEF char TEXT47[NOSZ_]  TINIT_("kill a echou\202");  /*"kill was bad"*/
TDCLDEF char TEXT48[NOSZ_]  TINIT_("succ\212s du kill; le processus existe");  /*"kill was good; process exists"*/
TDCLDEF char TEXT49[NOSZ_]  TINIT_("unlink de %s en cours\n");  /*"attempting to unlink %s\n"*/
TDCLDEF char TEXT50[NOSZ_]  TINIT_("destruction impossible d'un fichier verrouill\202");  /*"could not remove lock file"*/
TDCLDEF char TEXT51[NOSZ_]  TINIT_("Variable \205 positionner: ");  /*"Variable to set: "*/
TDCLDEF char TEXT52[NOSZ_]  TINIT_("%%La variable '%s' n'existe pas");  /*"%%No such variable as '%s'"*/
TDCLDEF char TEXT53[NOSZ_]  TINIT_("Valeur: ");          /*"Value: "*/
TDCLDEF char TEXT54[NOSZ_]  TINIT_("[Macro termin\202e anormalement]");  /*"[Macro aborted]"*/
TDCLDEF char TEXT55[NOSZ_]  TINIT_("Affichage de la variable: ");  /*"Variable to display: "*/
TDCLDEF char TEXT56[NOSZ_]  TINIT_("Liste des variables");  /*"Variable list"*/
TDCLDEF char TEXT57[NOSZ_]  TINIT_("Affichage impossible de la liste des variables");  /*"Can not display variable list"*/
TDCLDEF char TEXT58[NOSZ_]  TINIT_("[Construction de la liste des variables]");  /*"[Building variable list]"*/
TDCLDEF char TEXT59[NOSZ_]  TINIT_("");                  /* UNUSED */

/*              "Line %d/%d Col %d/%d Char %D/%D (%d%%) char = 0x%x"*/
TDCLDEF char TEXT60[NOSZ_]  TINIT_("Ligne %D/%D Col %d/%d Car %D/%D (%d%%) car = 0x%x");

TDCLDEF char TEXT61[NOSZ_]  TINIT_("<NOT USED>");        /*"not used"*/
TDCLDEF char TEXT62[NOSZ_]  TINIT_("Mode global \205 ");  /*"Global mode to "*/
TDCLDEF char TEXT63[NOSZ_]  TINIT_("Mode \205 ");        /*"Mode to "*/
TDCLDEF char TEXT64[NOSZ_]  TINIT_("ajouter: ");         /*"add: "*/
TDCLDEF char TEXT65[NOSZ_]  TINIT_("supprimer: ");       /*"delete: "*/
TDCLDEF char TEXT66[NOSZ_]  TINIT_("Mode inexistant!");  /*"No such mode!"*/
TDCLDEF char TEXT67[NOSZ_]  TINIT_("Message \205 \202crire: ");  /*"Message to write: "*/
TDCLDEF char TEXT68[NOSZ_]  TINIT_("Cha\214ne \205 ins\202rer: ");  /*"String to insert<META>: "*/
TDCLDEF char TEXT69[NOSZ_]  TINIT_("Cha\214ne \205 substituer: ");  /*"String to overwrite<META>: "*/
TDCLDEF char TEXT70[NOSZ_]  TINIT_("[R\202gion copi\202e]");  /*"[region copied]"*/
TDCLDEF char TEXT71[NOSZ_]  TINIT_("%%Ce tampon est d\202j\205 diminu\202");  /*"%%This buffer is already narrowed"*/
/*              "%%Must narrow at least 1 full line"*/
TDCLDEF char TEXT72[NOSZ_]  TINIT_("%%La diminution doit \210tre d'au moins une ligne compl\212te");
TDCLDEF char TEXT73[NOSZ_]  TINIT_("[Le tampon est diminu\202]");  /*"[Buffer is narrowed]"*/
TDCLDEF char TEXT74[NOSZ_]  TINIT_("%%Ce tampon n'est pas diminu\202");  /*"%%This buffer is not narrowed"*/
TDCLDEF char TEXT75[NOSZ_]  TINIT_("[Tampon \202largi]");  /*"[Buffer is widened]"*/
TDCLDEF char TEXT76[NOSZ_]  TINIT_("Pas de marque dans cette fen\210tre");  /*"No mark set in this window"*/
TDCLDEF char TEXT77[NOSZ_]  TINIT_("Bogue: marque perdue");  /*"Bug: lost mark"*/
TDCLDEF char TEXT78[NOSZ_]  TINIT_("Recherche avant ");  /*"Search"*/
TDCLDEF char TEXT79[NOSZ_]  TINIT_("Pas trouv\202");     /*"Not found"*/
TDCLDEF char TEXT80[NOSZ_]  TINIT_("Cha\214ne non initialis\202e");  /*"No pattern set"*/
TDCLDEF char TEXT81[NOSZ_]  TINIT_("Recherche arri\212re ");  /*"Reverse search"*/
TDCLDEF char TEXT82[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT83[NOSZ_]  TINIT_("");                  /* UNUSED */
TDCLDEF char TEXT84[NOSZ_]  TINIT_("Remplacer ");        /*"Replace"*/
TDCLDEF char TEXT85[NOSZ_]  TINIT_("Remplacer avec confirmation ");  /*"Query replace"*/
TDCLDEF char TEXT86[NOSZ_]  TINIT_("par ");              /*"with"*/
TDCLDEF char TEXT87[NOSZ_]  TINIT_("Remplacer '");       /*"Replace '"*/
TDCLDEF char TEXT88[NOSZ_]  TINIT_("' par '");           /*"' with '"*/
TDCLDEF char TEXT89[NOSZ_]  TINIT_("Annul\202!");        /*"Aborted!"*/

/*              "(Y)es, (N)o, (!)Do rest, (U)ndo last, (^G)Abort, (.)Abort back, (?)Help: "*/
TDCLDEF char TEXT90[NOSZ_]  TINIT_("(O)Oui, (N)on, (!)Remp.tout, (U)ndo prec., (^G)Fin,(.)Fin debut, (?)Aide: ");

TDCLDEF char TEXT91[NOSZ_]  TINIT_("Cha\214ne vide remplac\202e, arr\210t");  /*"Empty string replaced, stopping."*/
TDCLDEF char TEXT92[NOSZ_]  TINIT_("%D substitutions");  /* same in French */
TDCLDEF char TEXT93[NOSZ_]  TINIT_("%%ERREUR pendant la suppression");  /*"%%ERROR while deleting"*/
TDCLDEF char TEXT94[NOSZ_]  TINIT_("%%Plus de m\202moire disponible");  /*"%%Out of memory"*/
TDCLDEF char TEXT95[NOSZ_]  TINIT_("%%mceq: qu'est-ce que %d?");  /*"%%mceq: what is %d?"*/
TDCLDEF char TEXT96[NOSZ_]  TINIT_("%%Pas de caract\212res dans cette classe");  /*"%%No characters in character class"*/
TDCLDEF char TEXT97[NOSZ_]  TINIT_("%%Classe de caract\212res non termin\202e");  /*"%%Character class not ended"*/
TDCLDEF char TEXT98[NOSZ_]  TINIT_("Marge droite non initialis\202e");  /*"No fill column set"*/
TDCLDEF char TEXT99[NOSZ_]  TINIT_("[PLUS DE M\220MOIRE]");  /*"[OUT OF MEMORY]"*/
/*              "Words %D Chars %D Lines %d Avg chars/word %f"*/
TDCLDEF char TEXT100[NOSZ_] TINIT_("Mots %D Car %D Lignes %d Moyenne car/mots %f");
TDCLDEF char TEXT101[NOSZ_] TINIT_("[Je ne peux pas chercher et me d\202placer en m\210me temps!]");  /*"[Can not search and goto at the same time!]"*/
TDCLDEF char TEXT102[NOSZ_] TINIT_("[Aller-a: Param\212tre erron\202]");  /*"[Bogus goto argument]"*/
TDCLDEF char TEXT103[NOSZ_] TINIT_("[Sauvegarde de %s]");  /*"[Saving %s]"*/
TDCLDEF char TEXT104[NOSZ_] TINIT_("Des tampons ont \202t\202 modifi\202s. On quitte malgr\202 tout");  /*"Modified buffers exist. Leave anyway"*/
TDCLDEF char TEXT105[NOSZ_] TINIT_("%%Macro d\202j\205 active");  /*"%%Macro already active"*/
TDCLDEF char TEXT106[NOSZ_] TINIT_("[D\202but de la macro]");  /*"[Start macro]"*/
TDCLDEF char TEXT107[NOSZ_] TINIT_("%%Macro non active");  /*"%%Macro not active"*/
TDCLDEF char TEXT108[NOSZ_] TINIT_("[Fin de la macro]");  /*"[End macro]"*/
TDCLDEF char TEXT109[NOSZ_] TINIT_("[Touche interdite dans le mode VIEW]");  /*"[Key illegal in VIEW mode]"*/
TDCLDEF char TEXT110[NOSZ_] TINIT_("[Commande \205 usage RESTREINT]");  /*"[That command is RESTRICTED]"*/
TDCLDEF char TEXT111[NOSZ_] TINIT_("Pas de macro indiqu\202e");  /*"No macro specified"*/
TDCLDEF char TEXT112[NOSZ_] TINIT_("Num\202ro de macro hors limite");  /*"Macro number out of range"*/
TDCLDEF char TEXT113[NOSZ_] TINIT_("Cr\202ation de macro impossible");  /*"Can not create macro"*/
TDCLDEF char TEXT114[NOSZ_] TINIT_("Nom de la proc\202dure: ");  /*"Procedure name: "*/
TDCLDEF char TEXT115[NOSZ_] TINIT_("Ex\202cuter la proc\202dure");  /*"Execute procedure"*/
TDCLDEF char TEXT116[NOSZ_] TINIT_("Proc\202dure inexistante: ");  /*"No such procedure"*/
TDCLDEF char TEXT117[NOSZ_] TINIT_("Ex\202cuter le tampon: ");  /*"Execute buffer"*/
TDCLDEF char TEXT118[NOSZ_] TINIT_("Tampon inexistant");  /*"No such buffer"*/
TDCLDEF char TEXT119[NOSZ_] TINIT_("%%Plus de m\202moire pendant la recherche");  /*"%%Out of memory during while scan"*/
TDCLDEF char TEXT120[NOSZ_] TINIT_("%%!BREAK en-dehors d'une boucle !WHILE");  /*"%%!BREAK outside of any !WHILE loop"*/
TDCLDEF char TEXT121[NOSZ_] TINIT_("%%!ENDWHILE sans !WHILE");  /*"%%!ENDWHILE with no preceding !WHILE in '%s'"*/
TDCLDEF char TEXT122[NOSZ_] TINIT_("%%!WHILE sans !ENDWHILE");  /*"%%!WHILE with no matching !ENDWHILE in '%s'"*/
TDCLDEF char TEXT123[NOSZ_] TINIT_("%%Plus de m\202moire pendant l'ex\202cution d'une macro");  /*"%%Out of Memory during macro execution"*/
TDCLDEF char TEXT124[NOSZ_] TINIT_("%%Directive inconnue");  /*"%%Unknown Directive"*/
TDCLDEF char TEXT125[NOSZ_] TINIT_("Plus de m\202moire pour l'enregistrement d'une macro");  /*"Out of memory while storing macro"*/
TDCLDEF char TEXT126[NOSZ_] TINIT_("%%Erreur interne boucle While");  /*"%%Internal While loop error"*/
TDCLDEF char TEXT127[NOSZ_] TINIT_("%%Etiquette inexistante");  /*"%%No such label"*/

/*              "(e)val exp, (c/x)ommand, (t)rack exp, (^G)abort, <SP>exec, <META>stop debug"*/
TDCLDEF char TEXT128[NOSZ_] TINIT_("(e)val exp, (c/x)ommand, (t)rack exp, (^G)avorte,<SP>exec, <META> stop debug");

TDCLDEF char TEXT129[NOSZ_] TINIT_("Ex\202cuter le fichier: ");  /*"File to execute: "*/
TDCLDEF char TEXT130[NOSZ_] TINIT_("Macro non d\202finie");  /*"Macro not defined"*/
TDCLDEF char TEXT131[NOSZ_] TINIT_("Lire le fichier ");  /*"Read file: "*/
TDCLDEF char TEXT132[NOSZ_] TINIT_("Ins\202rer le fichier ");  /*"Insert file: "*/
TDCLDEF char TEXT133[NOSZ_] TINIT_("Charger le fichier ");  /*"Find file: "*/
TDCLDEF char TEXT134[NOSZ_] TINIT_("Voir le fichier ");  /*"View file: "*/
TDCLDEF char TEXT135[NOSZ_] TINIT_("[Ancien tampon]");   /*"[Old buffer]"*/
TDCLDEF char TEXT136[NOSZ_] TINIT_("Nom du tampon: ");   /*"Buffer name: "*/
TDCLDEF char TEXT137[NOSZ_] TINIT_("Cr\202ation du tampon impossible");  /*"Cannot create buffer"*/
TDCLDEF char TEXT138[NOSZ_] TINIT_("[Nouveau fichier]");  /*"[New file]"*/
TDCLDEF char TEXT139[NOSZ_] TINIT_("[Lecture du fichier en cours]");  /*"[Reading file]"*/
TDCLDEF char TEXT140[NOSZ_] TINIT_("Lu ");               /*"Read "*/
TDCLDEF char TEXT141[NOSZ_] TINIT_("ERREUR ENTR\220E/SORTIE");  /*"I/O ERROR, "*/
TDCLDEF char TEXT142[NOSZ_] TINIT_("PLUS DE M\220MOIRE");  /*"OUT OF MEMORY, "*/
TDCLDEF char TEXT143[NOSZ_] TINIT_(" ligne");            /*" line"*/
TDCLDEF char TEXT144[NOSZ_] TINIT_("\220crire le fichier ");  /*"Write file"*/
TDCLDEF char TEXT145[NOSZ_] TINIT_("Pas de nom de fichier");  /*"No file name"*/
TDCLDEF char TEXT146[NOSZ_] TINIT_("Fichier tronqu\202..\205 \202crire ailleurs");  /*"Truncated file..write it out"*/
TDCLDEF char TEXT147[NOSZ_] TINIT_("Tampon raccourci.. \205 sauvegarder");  /*"Narrowed Buffer..write it out"*/
TDCLDEF char TEXT148[NOSZ_] TINIT_("[\220criture en cours...]");  /*"[Writing...]"*/
TDCLDEF char TEXT149[NOSZ_] TINIT_("[\220crit ");        /*"[Wrote "*/
TDCLDEF char TEXT150[NOSZ_] TINIT_(", sauvegard\202 sous le nom ");  /*", saved as "*/
TDCLDEF char TEXT151[NOSZ_] TINIT_("Nom: ");             /*"Name: "*/
TDCLDEF char TEXT152[NOSZ_] TINIT_("[Fichier inexistant]");  /*"[No such file]"*/
TDCLDEF char TEXT153[NOSZ_] TINIT_("[Insertion du fichier en cours]");  /*"[Inserting file]"*/
TDCLDEF char TEXT154[NOSZ_] TINIT_("Ins\202r\202 ");     /*"Inserted "*/
TDCLDEF char TEXT155[NOSZ_] TINIT_("Ouverture du fichier en \202criture impossible");  /*"Cannot open file for writing"*/
TDCLDEF char TEXT156[NOSZ_] TINIT_("Erreur \205 la fermeture du fichier");  /*"Error closing file"*/
TDCLDEF char TEXT157[NOSZ_] TINIT_("Erreur en \202criture E/S");  /*"Write I/O error"*/
TDCLDEF char TEXT158[NOSZ_] TINIT_("Erreur en lecture du fichier");  /*"File read error"*/
TDCLDEF char TEXT159[NOSZ_] TINIT_("%J'ai besoin d'un num\202ro de touche de fonction");  /*"%Need function key number"*/
TDCLDEF char TEXT160[NOSZ_] TINIT_("%Num\202ro de touche de fonction hors limite");  /*"%Function key number out of range"*/
TDCLDEF char TEXT161[NOSZ_] TINIT_("Contenu de l'\202tiquette: ");  /*"Label contents: "*/
TDCLDEF char TEXT162[NOSZ_] TINIT_(" [o/n]? ");          /*" [y/n]? "*/
TDCLDEF char TEXT163[NOSZ_] TINIT_("pas de valeur par d\202faut");  /*"no default"*/
TDCLDEF char TEXT164[NOSZ_] TINIT_("[\202chec de la recherche]");  /*"[search failed]"*/
TDCLDEF char TEXT165[NOSZ_] TINIT_("Recherche Incr\202mentale: ");  /*"ISearch: "*/
TDCLDEF char TEXT166[NOSZ_] TINIT_("? Cha\214ne de recherche trop longue");  /*"? Search string too long"*/
TDCLDEF char TEXT167[NOSZ_] TINIT_("? commande trop longue");  /*"? command too long"*/
TDCLDEF char TEXT168[NOSZ_] TINIT_("%%Insertion de cha\214ne impossible");  /*"%%Can not insert string"*/
TDCLDEF char TEXT169[NOSZ_] TINIT_("Ins\202r\202(e)");   /*"Inserted"*/
TDCLDEF char TEXT170[NOSZ_] TINIT_("bogue: linsert");    /* same */
TDCLDEF char TEXT171[NOSZ_] TINIT_("Remplac\202(e)");    /*"Replaced"*/
TDCLDEF char TEXT172[NOSZ_] TINIT_("%%Plus de m\202moire en mode remplacement");  /*"%%Out of memory while overwriting"*/
TDCLDEF char TEXT173[NOSZ_] TINIT_("ERREUR VERROUILLAGE: table de verrouillage pleine");  /*"LOCK ERROR: Lock table full"*/
TDCLDEF char TEXT174[NOSZ_] TINIT_("Verrouillage impossible, plus de m\202moire");  /*"Cannot lock, out of memory"*/
TDCLDEF char TEXT175[NOSZ_] TINIT_("VERROUILL\220");     /*"LOCK"*/
TDCLDEF char TEXT176[NOSZ_] TINIT_("Fichier utilis\202 par ");  /*"File in use by "*/
TDCLDEF char TEXT177[NOSZ_] TINIT_(", on passe outre ?");  /*", overide?"*/
TDCLDEF char TEXT178[NOSZ_] TINIT_("[Obtention d'un message d'erreur systeme impossible]");  /*"[can not get system error message]"*/
TDCLDEF char TEXT179[NOSZ_] TINIT_("  A-propos de MicroEmacs");  /*"  About MicroEmacs"*/
TDCLDEF char TEXT180[NOSZ_] TINIT_("%%R\202solution inexistante");  /*"%%No such resolution"*/
TDCLDEF char TEXT181[NOSZ_] TINIT_("%%R\202solution interdite pour ce type d'\202cran");  /*"%%Resolution illegal for this monitor"*/
TDCLDEF char TEXT182[NOSZ_] TINIT_("Variable TERM non d\202finie");  /*"Environment variable TERM not defined!"*/
TDCLDEF char TEXT183[NOSZ_] TINIT_("%s: type de terminal inconnu");  /*"Unknown terminal type %s!"*/
TDCLDEF char TEXT184[NOSZ_] TINIT_("Description termcap incompl\212te (lignes)");  /*"termcap entry incomplete (lines)"*/
TDCLDEF char TEXT185[NOSZ_] TINIT_("Description termcap incompl\212te (colonnes)");  /*"Termcap entry incomplete (columns)"*/
TDCLDEF char TEXT186[NOSZ_] TINIT_("Description termcap incompl\212te");  /*"Incomplete termcap entry\n"*/
TDCLDEF char TEXT187[NOSZ_] TINIT_("Description du terminal trop importante");  /*"Terminal description too big!\n"*/
TDCLDEF char TEXT188[NOSZ_] TINIT_("[Termin\202]");      /*"[End]"*/
TDCLDEF char TEXT189[NOSZ_] TINIT_("Description du terminal non trouv\202e\n");  /*"Cannot find entry for terminal type.\n"*/

/*              "Check terminal type with \"SHOW TERMINAL\" or\n"*/
TDCLDEF char TEXT190[NOSZ_] TINIT_("V\202rifiez le type de terminal avec \"SHOW TERMINAL\" ou\n");

/*              "try setting with \"SET TERMINAL/INQUIRE\"\n"*/
TDCLDEF char TEXT191[NOSZ_] TINIT_("essayez de l'initialiser avec \"SET TERMINAL/INQUIRE\"\n");

/*              "The terminal type does not have enough power to run\n"*/
TDCLDEF char TEXT192[NOSZ_] TINIT_("Ce terminal n'est pas assez performant pour \n");

/*              "MicroEMACS.  Try a different terminal or check\n"*/
TDCLDEF char TEXT193[NOSZ_] TINIT_("MicroEMACS. Essayez un autre terminal ou v\202rifiez\n");

/*              "type with \"SHOW TERMINAL\".\n"*/
TDCLDEF char TEXT194[NOSZ_] TINIT_("celui-ci avec \"SHOW TERMINAL\".\n");

/*              "Cannot open channel to terminal.\n"*/
TDCLDEF char TEXT195[NOSZ_] TINIT_("Ouverture du canal associ\202 au terminal impossible.\n");

/*              "Cannot obtain terminal settings.\n"*/
TDCLDEF char TEXT196[NOSZ_] TINIT_("Obtention des caract\202ristiques du terminal impossible.\n");

/*              "Cannot modify terminal settings.\n"*/
TDCLDEF char TEXT197[NOSZ_] TINIT_("Modification des caract\202ristiques du terminal impossible.\n");

TDCLDEF char TEXT198[NOSZ_] TINIT_("Erreur Entr\202e/Sortie (%d,%d)\n");  /*"I/O error (%d,%d)\n"*/
TDCLDEF char TEXT199[NOSZ_] TINIT_("[Lancement de DCL]\r\n");  /*"[Starting DCL]\r\n"*/
TDCLDEF char TEXT200[NOSZ_] TINIT_("[Appel de DCL]\r\n");  /*"[Calling DCL]\r\n"*/
TDCLDEF char TEXT201[NOSZ_] TINIT_("[Non encore disponible sous VMS]");  /*"[Not available yet under VMS]"*/
TDCLDEF char TEXT202[NOSZ_] TINIT_("Terminal non 'vt52' ou 'z19'!");  /*"Terminal type not 'vt52'or 'z19' !"*/
TDCLDEF char TEXT203[NOSZ_] TINIT_("Num\202ro de fen\210tre hors limite");  /*"Window number out of range"*/
TDCLDEF char TEXT204[NOSZ_] TINIT_("Destruction de cette fen\210tre impossible");  /*"Can not delete this window"*/
TDCLDEF char TEXT205[NOSZ_] TINIT_("D\202coupage impossible d'une fen\210tre de %d lignes");  /*"Cannot split a %d line window"*/
TDCLDEF char TEXT206[NOSZ_] TINIT_("Une seule fen\210tre");  /*"Only one window"*/
TDCLDEF char TEXT207[NOSZ_] TINIT_("Changement impossible");  /*"Impossible change"*/
TDCLDEF char TEXT208[NOSZ_] TINIT_("[Fen\210tre inexistante]");  /*"[No such window exists]"*/
TDCLDEF char TEXT209[NOSZ_] TINIT_("%%Taille d'\202cran hors limite");  /*"%%Screen size out of range"*/
TDCLDEF char TEXT210[NOSZ_] TINIT_("%%Largeur d'\202cran hors limite");  /*"%%Screen width out of range"*/
TDCLDEF char TEXT211[NOSZ_] TINIT_("Liste des Fonctions");  /*"Function list"*/
TDCLDEF char TEXT212[NOSZ_] TINIT_("Affichage de la liste des fonctions impossible");  /*"Can not display function list"*/
TDCLDEF char TEXT213[NOSZ_] TINIT_("[Cr\202ation de la liste des fonctions]");  /*"[Building function list]"*/
TDCLDEF char TEXT214[NOSZ_] TINIT_("%%Fichier %s inexistant");  /*"%%No such file as %s"*/
TDCLDEF char TEXT215[NOSZ_] TINIT_(": macro-une-touche ");  /*": macro-to-key "*/
TDCLDEF char TEXT216[NOSZ_] TINIT_("Impossible de  lire/\202crire des r\202pertoires !!!");  /*"Cannot read/write directories!!!"*/
TDCLDEF char TEXT217[NOSZ_] TINIT_("[Pas encore disponible sous AOS/VS]");  /*"[Not available yet under AOS/VS]"*/
TDCLDEF char TEXT218[NOSZ_] TINIT_("Compl\202 le fichier ");  /* "Append file"  */
TDCLDEF char TEXT219[NOSZ_] TINIT_("%%\220chec de la macro");  /* "%%Macro Failed" */
TDCLDEF char TEXT220[NOSZ_] TINIT_("Ligne %D/%D Col %d/%d Car %D/%D (%d%%) car = 0x%x%x");
TDCLDEF char TEXT221[NOSZ_] TINIT_("Trop de groupes");   /* "Too many groups" */
TDCLDEF char TEXT222[NOSZ_] TINIT_("Groupe non ferm\202");  /* "Group not ended" */
TDCLDEF char TEXT223[NOSZ_] TINIT_("%%Colonne origine hors limite");  /* "%%Column origin out of range" */
TDCLDEF char TEXT224[NOSZ_] TINIT_("%%Ligne origine hors limite");  /* "%%Row origin out of range" */
TDCLDEF char TEXT225[NOSZ_] TINIT_("[Changement pour l'\202cran %s]");  /* "[Switched to screen %s]" */
TDCLDEF char TEXT226[NOSZ_] TINIT_("%%Impossible de d\202truire un tampon en cours d'ex\202cution");  /* "%%Can not kill an executing buffer" */
TDCLDEF char TEXT227[NOSZ_] TINIT_("\n--- Appuyer sur une touche quelconque pour continuer ---");  /* "\n--- Press any key to Continue ---" */
TDCLDEF char TEXT228[NOSZ_] TINIT_("[Anneau des effacements vid\202]\n");
TDCLDEF char TEXT229[NOSZ_] TINIT_(" dans <");           /*" in <"*/
TDCLDEF char TEXT230[NOSZ_] TINIT_("> ligne ");          /*" > at line "*/
TDCLDEF char TEXT231[NOSZ_] TINIT_("Abr\202viation \205 d\202finir: ");  /*"Abbreviation to set: "*/
TDCLDEF char TEXT232[NOSZ_] TINIT_("Abr\202viation \205 supprimer: ");  /*"Abbreviation to delete: "*/
TDCLDEF char TEXT233[NOSZ_] TINIT_("[Construction liste des abr\202viations]");  /*"[Building Abbreviation list]"*/
TDCLDEF char TEXT234[NOSZ_] TINIT_("Liste des abr\202viations");  /*"Abbreviation list"*/
TDCLDEF char TEXT235[NOSZ_] TINIT_("Impossible d'afficher la liste des abr\202viations");  /*"Can not display abbreviation list"*/
TDCLDEF char TEXT236[NOSZ_] TINIT_("D\202finition abr\202viations dans un tampon");  /*"Define Abbreviations in buffer"*/
TDCLDEF char TEXT240[NOSZ_] TINIT_("[Aucun \202cran de ce nom]");  /*"[No such screen]"*/
TDCLDEF char TEXT241[NOSZ_] TINIT_("%%Impossible de supprimer l'\202cran courant");  /*"%%Can't delete current screen"*/
TDCLDEF char TEXT242[NOSZ_] TINIT_("Chercher l'\202cran: ");  /*"Find Screen: "*/
TDCLDEF char TEXT243[NOSZ_] TINIT_("Supprimer l'\202cran: ");  /*"Delete Screen: "*/
TDCLDEF char TEXT244[NOSZ_] TINIT_("%%Fonction '%s' inexistante");  /*"%%No such function as '%s'"*/
TDCLDEF char TEXT245[NOSZ_] TINIT_("%%Division par z\202ro interdite");  /*"%%Division by Zero is illegal"*/
TDCLDEF char TEXT246[NOSZ_] TINIT_("%%Il faut un num\202ro de touche fonction");  /*"%%Need function key number"*/
TDCLDEF char TEXT247[NOSZ_] TINIT_("%%num\202ro de touche fonction hors limite");  /*"%%Function key number out of range"*/*/
TDCLDEF char TEXT248[NOSZ_] TINIT_("Entrer une \202tiquette: ");  /*"Enter Label String: "*/
TDCLDEF char TEXT249[NOSZ_] TINIT_("Variable globale \205 d\202clarer : ");
TDCLDEF char TEXT250[NOSZ_] TINIT_("Variable locale \205 d\202clarer : ");

/* some of these are just used in the microsoft windows version */
TDCLDEF char TEXT300[NOSZ_] TINIT_("[Menu incorrect]");  /*"[Incorrect menu]"*/
TDCLDEF char TEXT301[NOSZ_] TINIT_("[Trop de menus imbriqu\202s]");  /*"[Too many nested popup menus]"*/
TDCLDEF char TEXT302[NOSZ_] TINIT_("[Manque de ressources]");  /*"[Lack of resources]"*/

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
TDCLDEF char TEXT330[NOSZ_] TINIT_(" - Historique des messages");  /*" - Message history"*/
TDCLDEF char TEXT331[NOSZ_] TINIT_("Modes globaux");     /*"Global modes"*/
TDCLDEF char TEXT332[NOSZ_] TINIT_("Modes pour le tampon: ");  /*"Modes for buffer: "*/
TDCLDEF char TEXT333[NOSZ_] TINIT_("\220criture d'un fichier en cours. Quitter plus tard !");  /*"File write in progress. Quit later !"*/
TDCLDEF char TEXT334[NOSZ_] TINIT_("[R\202pertoire inexistant]");  /*"[No such directory]"*/
TDCLDEF char TEXT335[NOSZ_] TINIT_("Changer le nom de l'\202cran en: ");  /*"Change screen name to: "*/
TDCLDEF char TEXT336[NOSZ_] TINIT_("[Nom d'\202cran d\202j\205 utilis\202]");  /*"[Screen name already in use]"*/
TDCLDEF char TEXT337[NOSZ_] TINIT_("impossible de contr\223ler un programme externe");  /*"cannot monitor external program"*/



/*====================================================================*/
#endif/**#ifndef FRENCH_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
