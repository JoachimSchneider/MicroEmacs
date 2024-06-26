# MicroEMACS


This is an unofficial repository for the MicroEMACS Editor by Daniel M. Lawrence.

Its purpose is to keep this wonderful editor usable and alive by doing
necessary source code maintenance. Currently it works on some Unix
operating systems, e.g. AIX, FreeBSD, OpenBSD, Linux and Solaris, on
MSDOS (using Turbo C or DJGPP), on Windows (XP up to 11) with CygWin
and --- thanks to the contribution of Martin Vorlaender --- on OpenVMS:

* Bug fixing,
* porting to current operating systems and
* improving the source code quality (e.g. by using Valgrind and modern C++ compilers).

It started as a *copy* of Peter Chapman's repository
<https://github.com/pmachapman/memacs>. I've frozen the state at the
time of the copy (Aug 6, 2021) in the *archive* branch.

Documentation:

* See `ue500/doc/emacs.pdf` for the original documentation of MicroEMACS.

* Necessary changes and updates to this documentation caused by bug
  fixes and new features will be desribed in *this* file in the section
  called "Documentation Updates".

* For platform specific instructions see
  (`$ find ue500 -type f \( -name 'Readme*' -or -name 'Install*' \)`):

  - ue500/vms/Readme.txt
  - ue500/djgpp/Readme.txt
  - ue500/cygwin/Readme.txt
  - ue500/linux/termcap/Install
  - ue500/linux/ansi/Install
  - ue500/aix5/Install
  - ue500/freebsd/ansi/Install


## Some history:

* I enjoyed MicroEMACS since 1994 --- then version 3.12 for MS-DOS;
  used it to type in the TeX code of my thesis.

* Daniel Michael Lawrence worked on MicroEMACS until 2009. His version
  5.00 was available on <https://www.aquest.com/emacs.htm> (Daniel M.
  Lawrence's MicroEMACS site).

* Daniel Michael Lawrence passed away on June 7, 2010.

* Apr 12, 2017: Peter Chapman created the GitHub repository
  <https://github.com/pmachapman/memacs> "to collect the source code and
  binaries for MicroEMACS".

  He did some changes/improvements to be able to build MicroEMACS on
  Windows XP with recent Borland and Microsoft compilers.

* Aug 6, 2021: Peter Chapman's repository was copied by me (Joachim
  Schneider). First purpose was to get Daniels MicroEMACS running again
  on Linux and OpenBSD.

  Steps used to copy:

  - Fork of Peter Chapmans repository to "memacs".
  - Create Repository "MicroEmacs" on GitHub.
  - `git clone --bare https://github.com/JoachimSchneider/memacs.git`
  - `cd memacs.git`
  - `git push --mirror https://github.com/JoachimSchneider/MicroEmacs.git`
  - Delete "memacs.git" on GitHub


## Branches:

* *master*:   Tested

* *fixes*:    Compiles and runs

* *current*:  Need not even compile --- *Not testet*

* *archive*:  Frozen (Aug 6, 2021) state of the copy of Peter Chapman's repository


## Pull requests

Please use the *fixes* branch for pull requests.


## Change Log

[2024-05-23.00]
  * New escape character flag `~x' will be recognized im MicroEMACS
    macro language:

    Exactly two hexadecimal digits following `~x' are
    evaluated to a character code:
    ~xUV ===> 0xUV, e.g. ~x20 ===> 0x20 ===> ' '.

[2024-05-14.00]
  * Setting $abfull TRUE enables full --- i.e. substrings in
    words --- expansion of abbreviations in ABBREV mode.

[2024-05-08.00]
  * It's possible now to *update* the keymap (it was only possible to
    create new entries before). This affects `set $palette KEYMAP ...'
    and `add-keymap'.

[2024-05-02.00]
 * Startup file won't be loaded from current directory any more:
   It is a security hole.
 * VMS: Lookup startup file first in SYS$LOGIN, then
   in MICROEMACS$LIB.

[2024-04-18.00]
* VAX (ANSI/SMG): Enabled keymap reconfiguration via
  `set $palette KEYMAP <keybinding> <escape-sequence>'.

[2024-03-01.01]
* MicroEMACAS compiles and runs with DJGPP: Testet on MS-DOS 6.22 in
  Linux DOSEMU, DOSBox with cwsdpmi, Windows 2000 and Windows XP. See
  ue500/djgpp/Readme.txt.

  It runs standalone (outside of a DJGPP environment) if a DPMI server
  (e.g. cwsdpmi or the built-in Windows DPMI) is available.

[2024-03-01.00]
* MicroEMACAS compiles and runs with CygWin: Testet on Windows XP,
  Windows 10 and Windows 11. See ue500/cygwin/Readme.txt.

  It runs standalone (outside of a CygWin environment) if the
  cygwin1.dll is located in the Windows search Path.

[2023-10-03.00]
* Enabled ANSI terminal driver for UNIX

[2023-09-27.00]
* pcdos/ansi build works (Uses ANSI escape sequences for screen control).

[2023-09-20.01]
* Use room/reroom *everywhere*.
* Use CLROOM() insted of free().

[2023-09-20.00]
* Modified the source to compile in a pre-ANSI, and varargs.h
  environment.

* Checked the *old* (pre-ANSI, varargs.h) `PROTO == 0, VARG == 1`
  settings on Solaris 5.7: It works.

[2023-09-07.00]
* On the UNIX platforms and on MSDOS it compiles *without warnings*
  using the C++ compilers. On VMS the C++ compile works well and
  produces only some informational messages (e.g. about unreachable
  code).

  Purpose: Improvement of code quality.

[2023-08-29.00]
* It compiles and runs on AIX 5.

[2023-08-01.00]
* MSDOS with Borland C (pcdos/turbo): Fixed a regression introduced with
  UE500: NSTRING was increased from 128 to 512 leading to `stack overflow'
  which could only be detected (on MSDOS) by compliling with the `-N' switch!
* Introduced Macros for safe string copy.

[2023-08-14.00]
* Fixed core dump when !endm was missing.

[2023-07-25.00]
* Added stack ADT for static returned strings (eval.c:gtfun()) to
  allow (indirect) recursive function calls.

[2023-07-20.01]
* It works on MSDOS with Borland C (pcdos/turbo)

[2023-07-20.00]
* Regression in eval.c:gtfun() resolved.

[2023-07-17.00]
* It compiles on MSDOS with Borland C (pcdos/turbo)
* ... but there is an issue (arrow-navigation in help ends in an
  infinite macro-loop).

[2023-07-13.00]
* Cleared all warnings.
* Fixed error in eval.c:gtfun() returning address of a stack variable.
* Introduced macro ISSPACE to avoid (maybee unsigned) character indexes
  for arrays (some isspace-macro-implementations use lookup arrays).

[2023-05-15]
* It works on OpenVMS VAX 7.3 (Compaq C V6.4-005), OpenVMS Alpha V7.3-2
(HP C V7.1-015), OpenVMS I64 V8.4-2L1 (VSI C V7.4-001) and OpenVMS
x86_64 E9.2-1 (VSI C X7.4-726 (GEM 50X23)).

[2021-08-22.00]
* Code was reformatted with uncrustify.

[2021-08-05.00]
* It works on Linux, FreeBSD, OpenBSD and Solaris (tested with SunOS 5.7).
* Unified ifdefs for Unix Operating Systems.

[2021-07-14.00]
* Activated support for file locking.

[...] Added several bug-fixes that make the code compile and work on
modern Linux and FreeBSD systems:

* Introduced the functions xstr(n)cpy (in char.c) which work correctly
  with overlapping buffers.

  This fixed e.g. the error that there appeared a text "Functinn"
  instead of "Function" in the upper part of the screen when using the
  standard .emacsrc.

* Validated using valgrind.

* Temporary files used by functions in unix.c are now created in /tmp
  and *not* in the current directory.

* In the unix-case the commands "filter-buffer" and "pipe-command" are
  able to use complex shell constructs like "echo abc; sleep 10; date".

* More defensive parsing of "set $palette" statements for screen colors
    (e.g.. set ``$palette "CLRMAP  7 ~e[37m" ; white``): Use ``[ ][ ]*``
    instead of ``' '`` as field separator.

* The `Undo' feature was broken by some "optimized" fill-functions
  which did'nt record the undo information: Fixed.

* Color support by using .emacsrc statements. (XTerm as well as  Linux
  and FreeBSD text console). Find the correct Escape-Sequences with ``tput
    setf [1-7]`` or  ``tput setb [1-7]``.

* Const-Correctness:

    Functions modified to get const-correctness:
```
    bind.c:stock()      /* Convert (function-)keyname intp internal Code.   */
    eval.c():gtfun()    /* Call internal function via name.                 */
    eval.c:gtusr()      /* look up a user var's value                       */
    file.c:makename()   /* Create buffer name out of file name.             */
```
    In all of these functions local modifyable copies are created using
    xstrdup().


## Documentation Updates

* New user functions (efunc.h) fill-buffer and fill-region. With fill-region
  if the region starts in the middle of a line the start of the line will be
  used as a prefix for all lines:
  ```
       | --- Mark
       v
  ===> Text text text text text text text text text text text text ...
  ```
  will be filled to
  ```
  ===> Text text text text text text text text text text text text text
  ===> text text text text text text text text text text text text text
  ===> text text text text text text text text text text text text text
  ===> text text text text text text text text text text text text text
  ```

  * fill-(paragraph|buffer|region) now accepts an numeric argument to be
    used as parindent for the first line of a paragraph. The default is
    set  set by the new environment variable $parindent (default: 0).

  * fill-(paragraph|buffer|region) filled texts  start at th indent of
    the first line (if not specified as above in the ``===>`` example:
  ```
       Text text text text text text text text text text text text ...
    text text text text text text text text text text text ...
  ```
  will be filled to
  ```
       Text text text text text text text text text text text text text
       text text text text text text text text text text text text text
       text text text text text text text text text text text text text
       text text text text text text text text text text text text text
  ```

* The startup file (.emacsrc, emacs.rc, ...) is not searched in the
  current directory any more; this obviously was a security hole.

* VMS: emacs.rc is first searched in SYS$LOGIN and then in
  MICROEMACS$LIB.

* VMS: $ssave defaults to FALSE now: This way the VMS versions work out
  of the box.

* Escape character flag `~x' will be recognized im MicroEMACS
  macro language:

  Exactly two hexadecimal digits following `~x' are
  evaluated to a character code:
  ~xUV ===> 0xUV, e.g. ~x20 ===> 0x20 ===> ' '.

* Setting $abfull TRUE enables full --- i.e. substrings in words ---
  expansion of abbreviations. This is a stronger form of the $abquick
  sub mode:

  Example use of these flags when in ABBREV mode:

  Assume the abbreviations

    Aa  Alpha
    Xx  X-Ray

  are defined

  - $abfull == FALSE, $abquick == FALSE

    Then only the *isolated* words Aa or Xx will be substituted by
    `Alpha' and 'X-Ray' during typing:

    ```
    "Aa Xx "    ===>    "Alpha X-Ray "
    "AaXx"      ===>    "AaXx"
    ```

  - $abfull == FALSE, $abquick == TRUE

    Aa and Xx will be substituted as soon as the appear at the
    beginning of words or follow another substitution:

    ```
    "Aa Xx "    ===>    "Alpha X-Ray "
    "AaXx"      ===>    "AlphaX-Ray"
    "yAaXx"     ===>    "yAaXx"
    ```

  - $abfull == TRUE ($abquick setting doesn't matter):

    Every occurence of Aa and Xx will be substituted as soon as they
    are typed in:

    ```
    "Aa Xx "    ===>    "Alpha X-Ray "
    "AaXx"      ===>    "AlphaX-Ray"
    "yAaXx"     ===>    "yAlphaX-Ray"
    "yAaXxu"    ===>    "yAlphaX-Rayu"
    ```

  So indeed the the $abfull somehow improves $abquick's behaviour
  which we keep here for compatibility.

  A more realistc examle: Suppose you need to use the HTML commands for
  german umlauts. To simplify this task you decide to use MicroEMACS
  abreviations which let you type the umlauts in the LaTeX style:

  * Create the file `.uemacs-abbrevs` in your HOME directory:
    ```
    ; SOF(Abbreviations for uemacs)


    ; LaTeX like German Umlauts (`~"': `~' must be used to escape `"'):
    add-abbrev "~"a"    "&auml;"
    add-abbrev "~"o"    "&ouml;"
    add-abbrev "~"u"    "&uuml;"
    add-abbrev "~"A"    "&Auml;"
    add-abbrev "~"O"    "&Ouml;"
    add-abbrev "~"U"    "&Uuml;"
    add-abbrev "~"s"    "&szlig;"



    ; EOF(Abbreviations for uemacs)
    ```

    In your `.emacsrc` add the lines

    ```
    ...
    store-procedure get-abbrevs
            !if &seq &find .uemacs-abbrevs ""
                    write-message "[Can not find .uemacs-abbrevs]"
                    !return
            !endif
            execute-file .uemacs-abbrevs
    !endm
    execute-procedure get-abbrevs
    ...
    set $abfull TRUE
    ...
    ```

    Then you simply type M"archen which will expand to M&auml;rchen
    *while* you are typing.

    You may even extend this to type LATIN1 or UTF8 charcters for the Umlauts:

    ```
    ; SOF(Abbreviations for uemacs)


    ; LaTeX like German Umlauts as HTML escapes (the `"' needs to be escaped
    ; by `~'):
    add-abbrev "~"a"    "&auml;"
    add-abbrev "~"o"    "&ouml;"
    add-abbrev "~"u"    "&uuml;"
    add-abbrev "~"A"    "&Auml;"
    add-abbrev "~"O"    "&Ouml;"
    add-abbrev "~"U"    "&Uuml;"
    add-abbrev "~"s"    "&szlig;"

    ; German Umlauts as LATIN1 (`~xUV': Character with code 0xUV):
    add-abbrev "'a"    "~xe4"
    add-abbrev "'o"    "~xf6"
    add-abbrev "'u"    "~xfc"
    add-abbrev "'A"    "~xc4"
    add-abbrev "'O"    "~xd6"
    add-abbrev "'U"    "~xdc"
    add-abbrev "'s"    "~xdf"

    ; German Umlauts as UTF8 (`~xUV': Character with code 0xUV):
    add-abbrev "''a"    "~xc3~xa4"
    add-abbrev "''o"    "~xc3~xb6"
    add-abbrev "''u"    "~xc3~xbc"
    add-abbrev "''A"    "~xc3~x84"
    add-abbrev "''O"    "~xc3~x96"
    add-abbrev "''U"    "~xc3~x9c"
    add-abbrev "''s"    "~xc3~x9f"



    ; EOF(Abbreviations for uemacs)
    ```


## TODO
- Compile on Windows:
  + Borland C++
  + MSC
- UTF-8 support
- Try to compile on *really old* compilers (Pre-ANSI-C with varargs.h
  only --- set VARG to TRUE in eproto.h). First (successful) tests were
  done on AIX5.



## For completeness I quote Peter Chapman's README.md here:

```
## Introduction
I have found it hard to track down the source code for MicroEMACS,
and with Daniel's passing I have noticed that links from his home page are breaking.

This project seeks to collect the source code and binaries for MicroEMACS, and provide them all from one place

## Readme

Please refer to [DOC/README](doc/readme) for the complete MicroEMACS readme file.

## Copyright

This code retains the copyright Daniel placed on this work (refer to [LICENSE.md](LICENSE.md)), and on his passing I assume it is now with his estate.

I will endeavour to respect this, and request that you do the same, until notified otherwise.
```



# EOF
