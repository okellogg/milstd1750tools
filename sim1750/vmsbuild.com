$ set verify
$ cc/decc/g_float arith
$ cc/decc/g_float break
$ cc/decc/g_float cmd
$ cc/decc/g_float cpu
$ cc/decc/g_float dism1750
$ cc/decc/g_float do_xio
$ cc/decc/g_float exec
$ cc/decc/g_float fltcnv
$ cc/decc/g_float lic
$ cc/decc/g_float loadfile
$ cc/decc/g_float load_coff
$ cc/decc/g_float main
$ cc/decc/g_float phys_mem
$ cc/decc/g_float peekpoke
$ cc/decc/g_float sdisasm
$ cc/decc/g_float smemacc
$ cc/decc/g_float status
$ cc/decc/g_float tekhex
$ cc/decc/g_float tekops
$ cc/decc/g_float tldldm
$ cc/decc/g_float utils
$ cc/decc/g_float xiodef
$ link/exe=sim1750 arith,break,cmd,cpu,dism1750,do_xio,exec,-
   fltcnv,lic,loadfile,load_coff,main,phys_mem,peekpoke,sdisasm,smemacc,-
   status,tekhex,tekops,tldldm,utils,xiodef
$ set noverify
