store-procedure ret-A
  !return "A"
!endm

store-procedure ret-B
  !return "B"
!endm

store-procedure fac0 %n
  local %res
  set %res 0

  !if &equ 1 %n
    !return 1
  !else
    !return &tim %n &call fac0 &sub %n 1 
  !endif
!endm

store-procedure fac1 %n
  local %res
  local %i
  set %i   1
  set %res %i

  !while &les %i %n
    set %i &add %i 1
    set %res &tim %res %i
  !endwhile

  !return %res
!endm


store-procedure add3 %n0 %n1 %n2
  local %res
  set %res 0

  set %res &add &add  %n0 %n1 %n2

  !return %res
!endm

store-procedure addnums %n0 %n1 %n2 %n3 %n4 %n5 %n6 %n7 %n8 %n9
  local %res
  set %res 0

  set %res &add &add &add &add &add &add &add &add &add %n0 %n1 %n2 %n3 %n4 %n5 %n6 %n7 %n8 %n9

  !return %res
!endm


; Run tests:


store-procedure tst-00
  set %res_00   &cat &call ret-A &call ret-B
!endm

store-procedure tst-01
  set %res_01   &cat &call fac0 2 &call fac0 3
!endm

store-procedure tst-02
  set %res_02   &cat &call fac1 4 &call fac1 3
!endm

store-procedure tst-03
  set %res_03 &call addnums 1 2 3 4 5
!endm

store-procedure tst-04
  fac0 &call addnums 1 2 3
  set %res_04 $rval
!endm

store-procedure tst-05
  set %res_05 &call fac0 &call addnums 1 2 3
!endm

store-procedure tst-06
  fac0 &sub &call add3 1 2 3 2
  set %res_06 $rval
!endm


store-procedure main
  local %curbuf
  local %tbuf

  tst-00
  tst-01
  tst-02
  tst-03
  tst-04
  tst-05
  tst-06
  set %TRES  &cat &cat &cat &cat &cat &cat &cat &cat &cat &cat &cat &cat &cat "res_00 = " %res_00 ", res_01 = " %res_01 ", res_02 = " %res_02 ", res_03 = " %res_03 ", res_04 = " %res_04 ", res_05 = " %res_05 ", res_06 = " %res_06

  set %tbuf "**runtest-result**"
  !force delete-buffer %tbuf
  set %curbuf $cbufname

  1 select-buffer %tbuf
  insert-string &cat &cat "Expected result: " "res_00 = AB, res_01 = 26, res_02 = 246, res_03 = 15, res_04 = 720, res_05 = 720, res_05 = 24" "~n"
  insert-string                 "-------------------------------------------------------------------------------------------------------------~n"
  insert-string &cat &cat "Actual Result:   " %TRES "~n"
  insert-string                 "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^~n"
!endm


main



; END
