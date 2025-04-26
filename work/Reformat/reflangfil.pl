#!/usr/bin/env perl
#=======================================================================
# Transform the language files (english.h, latin.h) from old to
# new format.
#=======================================================================
use strict;


my $prevline  = "";
my $lno       = 0;

while ( my $line = <> ) {
  $lno++;
  chop $line;
  #printf STDOUT "%s\n", $line;

  if ( 17 == $lno ) {
    printf STDOUT "%s\n", "/**********************************************************************/";
    printf STDOUT "%s\n", "#include \"eproto.h\"";
    printf STDOUT "%s\n", "/**********************************************************************/";
    printf STDOUT "%s\n", "";
  }
  # Handle continuation lines
  if ( "" ne $prevline )  {
    $line     = "${prevline} ${line}";
    $prevline = "";
  }
  if ( $line =~ /^.*\\[[:space:]]*$/ )  {
    $line     =~ s/\\[[:space:]]*$//;
    $prevline = $line;

    next;
  }

  my ( $name, $val, $comment ) = $line
    =~ /^[[:space:]]*#[[:space:]]*define[[:space:]]+([^[:space:]]+)[[:space:]]+(".*")[[:space:]]*(\/\*.*\*\/)?[[:space:]]*$/;
  if ( "" ne $name && "" ne $val )  {
    #printf STDOUT "<%s>: <%s> <%s>\n", $name, $val, $comment;
    # TDCLDEF char TEXT99[NOSZ_]  TINIT_("[OUT OF MEMORY]");   /* UNUSED */
    my $vardef  = sprintf("%s[NOSZ_]", $name);
    my $stmt    = sprintf "TDCLDEF char %-14s TINIT_(%s);", $vardef, $val;
    if ( "" ne $comment ) {
      printf STDOUT "%-55s  %s", $stmt, $comment;
    } else                {
      printf STDOUT "%s", $stmt;
    }
    printf STDOUT "%s", "\n";
  } else                            {
    my ( $name, $unused ) = $line
        =~ /^[[:space:]]*#[[:space:]]*define[[:space:]]+([^[:space:]]+)[[:space:]]+(UNUSED)[[:space:]]*$/;
    if ( "" ne $name && "" ne $unused ) {
      my $vardef  = sprintf("%s[NOSZ_]", $name);
      my $stmt    = sprintf "TDCLDEF char %-14s TINIT_(\"%s\");", $vardef, "";
      printf STDOUT "%-55s  %s\n", $stmt, "/* UNUSED */";
    } else                              {
      printf STDOUT "%s\n", $line;
    }
  }
}



# EOF
