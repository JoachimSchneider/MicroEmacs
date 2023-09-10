/*	me.c	MicroEmacs command interface to kept microemacs
		For VAX/VMS operating system
		for MicroEMACS 3.10
		Copyright 1989 by Jeffrey A. Lomicka and Daniel M. Lawrence

	This is a separate program that is used to start and re-attach to
	a microemacs that's kept in a subprocess.  To use the kept
	facility, point the microemacs symbol at this program instead of
	MESHR.EXE, and store MESHR.EXE in the SYS$SHARE search list.

	To use microemacs in the normal way, just point the microemacs
	command directly at tghe MESHR.EXE image.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unixlib.h>
#include <starlet.h>
#include <lib$routines.h>
#include <clidef.h>
#include <ssdef.h>
#include <descrip.h>
#include <jpidef.h>
#include <iodef.h>
#include <accdef.h>
#include <clidef.h>

/*                                                                          */
/*  $SETDDIR                                                                */
/*                                                                          */
/*    Read/Change the process' default directory                            */
/*                                                                          */
/*	$SETDDIR [newdir] ,[curlen] ,[curdir]                               */
/*                                                                          */
/*	newdir	= address of the new default directory                      */
/*		  string descriptor                                         */
/*	curlen	= address of a word to receive length of current            */
/*                default directory                                         */
/*	curdir	= address of string descriptor to receive current           */
/*		  default directory                                         */
/*                                                                          */
int sys$setddir(__unknown_params);

/*
	test macro is used to signal errors from system services
*/
#define test( s) {int st; st = (s); if( (st&1)==0) lib$signal( st);}

/*
	This routine returns a pointer to a descriptor of the supplied
	string. The descriptors are static allocated, and up to
	"NUM_DESCRIPTORS" may be used at once.  After that, the old ones
	are re-used. Be careful!

	The primary use of this routine is to allow passing of C strings into
	VMS system facilities and RTL functions.

	There are three forms:

		descrp( s, l)	String descriptor for buffer s, length l
		descptr( s)	String descriptor for asciz buffer s
		DESCPTR( s)	String descriptor for buffer s, using sizeof()
*/
#define NUM_DESCRIPTORS 10
struct  dsc$descriptor_s *descrp(
  char *s,     /* String to make descriptor for */
  int l                /* length of string */
)
    {
    static int next_d = 0;
    static struct dsc$descriptor_s dsclist[ NUM_DESCRIPTORS];

    if( next_d >= NUM_DESCRIPTORS) next_d = 0;
    dsclist[ next_d].dsc$w_length = l;
    dsclist[ next_d].dsc$b_dtype =  DSC$K_DTYPE_T;
    dsclist[ next_d].dsc$b_class =  DSC$K_CLASS_S;
    dsclist[ next_d].dsc$a_pointer = s;
    return( &dsclist[ next_d++]);
    }

struct  dsc$descriptor_s *descptr(     /* Make pointer to descriptor */
  char *s              /* Asciz string to make descriptor for */
)
    {
    return( descrp( s, strlen( s)));
    }

#define DESCPTR( s)	descrp( s, sizeof(s)-1)

static long g_pid, g_status, g_efn;
/* originally:
struct acc$record msg;		 Message buffer */
struct accdef msg;		/* Message buffer */
static int good_reason = 0;	/* True if good reason to exit */

void me_exited(void)
   {
   test( lib$put_output( DESCPTR( "Microemacs has exited.")));
   good_reason = 1;
   test( sys$wake( 0, 0));
   }

void start_microemacs(
  int argc,
  char *argv[]
)
    {
    char scrap[ 512], imagename[ 256], *i;
    int a;
    unsigned spawn_flags = CLI$M_NOWAIT;

    test( lib$put_output( DESCPTR( "Starting a new Microemacs process.")));
/*
	Locate the microemacs sharable image
*/
    a = 0;
    test( lib$find_file(
	DESCPTR( "MESHR"),
	DESCPTR( imagename),
	&a,
	DESCPTR( "SYS$SHARE:.EXE"),
	0, 0, 0));
    test( lib$find_file_end( &a));
/*
	Make it asciz by terminating it at the ";"
*/
    for( i=imagename; *i != ';'; i++);
    *i = 0;
/*
	Build a command line that invokes it
*/
    strcpy( scrap, "MCR ");
    strcat( scrap, imagename);
    for( a=1; a<argc; a++)
	{ /* Build command string */
	strcat( scrap, " ");
	strcat( scrap, argv[ a]);
	}
    test( lib$get_ef( &g_efn));
    test( lib$spawn( descptr( scrap), DESCPTR( "NLA0:"), 0, &spawn_flags,
	0, &g_pid, &g_status, &g_efn, me_exited, &g_pid, 0, 0));
    }

int main(
  int argc,
  char *argv[]
)
    {
    char *env;
    char scrap[ 512];
    char defdir[ 512];
    int a;
    unsigned long pid;
    short len;
    unsigned jpi_pid = JPI$_PID,
             jpi_tmbu = JPI$_TMBU;
/*
	Identify where we come from
*/
    test( lib$getjpi( &jpi_pid, 0, 0, &pid, 0, 0));
    sprintf( scrap, "%ld", pid);
    test( lib$set_logical(
	DESCPTR( "MICROEMACS$PARENT"),
	descptr( scrap),
	DESCPTR( "LNM$JOB")));
/*
	Identify where we are going
*/
    env = getenv( "MICROEMACS$PROCESS");
    if( env == NULL) start_microemacs( argc, argv);
    else
	{ /* Microemacs exists, send command line to it and wake it up */
	pid = atoi( env);
/*
	Get rid of the old definition.  We know our wakeup is legitimate
	when either a new definition is created, or when our AST routine
	is executed.
*/
	test( lib$delete_logical(
	    DESCPTR( "MICROEMACS$PROCESS"),
	    DESCPTR( "LNM$JOB")));
/*
	Fetch the current default directory
*/
	len = 0;
	test( sys$setddir( 0, &len, DESCPTR( defdir)));
	defdir[ len] = 0;
/*
	Encode an argv string for emacs to decode.  Using "\200" instead
	of spaces just in case quotes were involved.
*/
	strcpy( scrap, getenv( "SYS$DISK"));
	strcat( scrap, "\200");
	strcat( scrap, defdir);
	for( a=0; a<argc; a++)
	    { /* Build command string */
	    strcat( scrap, "\200");
	    strcat( scrap, argv[ a]);
	    }
	test( lib$set_logical(
	    DESCPTR( "MICROEMACS$COMMAND"),
	    descptr( scrap),
	    DESCPTR( "LNM$JOB")));
/*
	Attempt to kiss our sleeping beauty, and thus wake it up
*/
	test( lib$put_output( DESCPTR( "Using existing Microemacs process.")));
	a = sys$wake( &pid, 0);
	if( a == SS$_NONEXPR)
	    { /* Failed to wake it up, try starting a new one instead */
	    start_microemacs( argc, argv);
	    }
	else
	    { /* Successful at waking up microemacs, set up for termination */
	    test( a);
           test( lib$getjpi( &jpi_tmbu, &pid, 0, &a, 0, 0));
	    sprintf( scrap, "MBA%d:", a);
	    test( sys$assign( descptr( scrap), &a, 0, 0));
	    test( sys$qio( 0, a, IO$_READVBLK, 0, me_exited, 0,
		msg, sizeof( msg), 0, 0, 0, 0));
	    }
	}
/*
	If Emacs was started, hibernate until it comes back
*/
    while( !good_reason)
	{ /* Hibernate until winter is over */
	test( sys$hiber());
	env = getenv( "MICROEMACS$PROCESS");
	if( env != NULL) break;
	}

    return 0;
    }
