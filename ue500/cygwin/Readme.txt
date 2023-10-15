MicroEMACS for CygWin
=====================

* At runtime the only needed CygWin component ist the cygwin1.dll which
  must be somewhere in the search path.

* To compile one needs a CygWin environment including gcc, git and make
  One may also use a MobaXTerm environment
  <https://mobaxterm.mobatek.net/>

* Testet on Microsoft Windows [Version 10.0.19045.3570].

* It needs a Windows Console supporting ANSI Escape Sequences --- the
  version mentioned above supports them out of the box --- for earlier
  Windows 10 versions this hint may help:

  <https://stackoverflow.com/questions/16755142/how-to-make-win32-console-recognize-ansi-vt100-escape-sequences-in-c>

  ```
  FYI, in latest Windows 10, you can enable ANSI in conhost via the
  following reghack -- in HKCU\Console create a DWORD named
  VirtualTerminalLevel and set it to 0x1; then restart cmd.exe.
  ...

  BrainSlugs83

  Oct 27, 2018 at 21:17
  ```


This simple session script shows how to get and compile the MicroEmacs:

We are inside of a cygwin bash terminal:

$ git clone https://github.com/JoachimSchneider/MicroEmacs
$ # git switch fixes
$ cd MicroEmacs/ue500/cygwin
$ make clean
$ make
# We assume that C:\bin is in your Windows path
$ mkdir -p C:/bin
$ mkdir -p C:/Pgm/MicroEmacs/bin
$ mkdir -p C:/Pgm/MicroEmacs/cmd
$ cp -p ../cmd/*  C:/Pgm/MicroEmacs/cmd/
$ cp -p emacs.rc  C:/Pgm/MicroEmacs/bin/
$ cp -p emacs.exe C:/Pgm/MicroEmacs/bin/
$ cat << 'EOC'  | sed 's/\n/\r\n/' > C:/bin/uemacs.cmd
@REM uemacs.cmd
@echo off
set PATH=C:\Pgm\MicroEmacs\bin;C:\Pgm\MicroEmacs\cmd;%PATH%
C:\Pgm\MicroEmacs\bin\emacs.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
REM EOF
EOC


The emacs.rc file is searched for in this order in these directories:
(1) %HOME%
(2) %PATH%
(3) <directories specified in epath.h>
See the function flook() in binc.c


When using MicroEMACS use forward slashes instead oof backslashes when
specifing paths.



# EOF
