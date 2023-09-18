# MicroEMACS

This is an unofficial repository for the MicroEMACS Editor by Daniel M. Lawrence.

Its purpose is to keep this wonderful editor usable and alive by doing
necessary source code maintenance (currently for some Unix operating
systems, e.g. AIX, FreeBSD, OpenBSD, Linux and Solaris, for MSDOS and
--- thanks to the contribution of Martin --- for OpenVMS):

* Bug fixing,
* porting to current operating systems and
* improving the source code quality (e.g. by using Valgrind and modern C++ compilers).

It started as a *copy* of Peter Chapman's repository
<https://github.com/pmachapman/memacs>. I've frozen the state at the
time of the copy (Aug 6, 2021) in the *archive* branch.

Documentation: See `ue500/doc/emacs.pdf`.


## Some history:

* I enjoyed MicroEMACS since the 1994 --- then version 3.12 for MS-DOS;
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


## Pull requests

Please use the *fixes* branch for pull requests.


## Change Log

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


## New features

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


## TODO
- Compile on Windows:
  + Borland C++
  + CygWin
  + MSC
- UTF-8 support
- Try to compile on *really old* compilers:
  + Provide a `stdarg.h' *and* `varargs.h` version for all functions
    whit variable argumebnt lists.



# For completeness I quote Peter Chapman's README.md here:
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
