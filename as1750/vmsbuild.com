$ set verify
$!
$! To use as1750 under VMS, you need to define a symbol (e.g. in
$! SYLOGIN.COM or your local LOGIN.COM) as follows:
$!
$!        $ as1750 :== your_device:[your_path]as1750.exe
$!
$! where <your_device> and <your_path> should point to the directory
$! where the AS1750.EXE resides.
$!
$ cc/decc/debug/define=("AS1750","MYALLOC") utils
$ cc/decc/debug/define=("AS1750","MYALLOC") tekhex
$ cc/decc/debug/define=("AS1750","MYALLOC") tldldm
$ cc/decc/debug/define=("AS1750","MYALLOC") flt1750
$ cc/decc/debug/define=("AS1750","MYALLOC") mexpr
$ cc/decc/debug/define=("AS1750","MYALLOC") syms
$ cc/decc/debug/define=("AS1750","MYALLOC") as1750
$ cc/decc/debug/define=("AS1750","MYALLOC") main
$ link/exe=as1750 main,as1750,syms,mexpr,tekhex,tldldm,flt1750,utils
$ set noverify
