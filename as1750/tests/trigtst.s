	name trigtst
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  putstr	; import
	konst
KLC0
	data	's','q','r','t','(','1','.','0',')',' ','=',' ',0
	global  pr_efp_num	; import
	global  sqrt	; import
	global  putchar	; import
	konst
KLC1
	data	's','q','r','t','(','4','.','0',')',' ','=',' ',0
	konst
KLC2
	data	's','q','r','t','(','9','.','0',')',' ','=',' ',0
	konst
KLC3
	data	's','q','r','t','(','2','5','6','.','0',')',' ','=',' ',0
	konst
KLC4
	data	's','i','n','(','0','.','0',')',' ','=',' ',0
	global  sin	; import
	konst
KLC5
	data	's','i','n','(','P','I','/','4',')',' ','=',' ',0
	konst
KLC6
	data	's','i','n','(','P','I','/','2',')',' ','=',' ',0
	konst
KLC7
	data	's','i','n','(','3','*','P','I','/','4',')',' ','=',' ',0
	konst
KLC8
	data	's','i','n','(','P','I',')',' ','=',' ',0
	konst
KLC9
	data	's','i','n','(','1','.','0',')',' ','=',' ',0
	konst
KLC10
	data	'c','o','s','(','0','.','0',')',' ','=',' ',0
	global  cos	; import
	konst
KLC11
	data	'c','o','s','(','P','I','/','4',')',' ','=',' ',0
	konst
KLC12
	data	'c','o','s','(','P','I','/','2',')',' ','=',' ',0
	konst
KLC13
	data	'c','o','s','(','3','*','P','I','/','4',')',' ','=',' ',0
	konst
KLC14
	data	'c','o','s','(','P','I',')',' ','=',' ',0
	konst
KLC15
	data	'c','o','s','(','1','.','0',')',' ','=',' ',0
	konst
KLC16
	dataef	1.000000
	konst
KLC17
	dataef	4.000000
	konst
KLC18
	dataef	9.000000
	konst
KLC19
	dataef	256.000000
	konst
KLC20
	dataef	0.000000
	konst
KLC21
	dataef	0.785398
	konst
KLC22
	dataef	1.570796
	konst
KLC23
	dataef	2.356194
	konst
KLC24
	dataef	3.141593

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lim	r0,LC0  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC16
	sjs	r15,sqrt   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC1  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC17
	sjs	r15,sqrt   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC2  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC18
	sjs	r15,sqrt   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC3  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC19
	sjs	r15,sqrt   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC4  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC20
	sjs	r15,sin   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC5  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC21
	sjs	r15,sin   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC6  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC22
	sjs	r15,sin   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC7  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC23
	sjs	r15,sin   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC8  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC24
	sjs	r15,sin   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC9  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC16
	sjs	r15,sin   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC10  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC20
	sjs	r15,cos   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC11  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC21
	sjs	r15,cos   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC12  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC22
	sjs	r15,cos   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC13  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC23
	sjs	r15,cos   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC14  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC24
	sjs	r15,cos   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lim	r0,LC15  ; 'i' constraint
	sjs	r15,putstr   ; return value in R0
	efl	r0,LC16
	sjs	r15,cos   ; return value in R0
	sjs	r15,pr_efp_num   ; return value in R0
	lisp	r0,10
	sjs	r15,putchar   ; return value in R0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15

	static
init_srel
LC0 	block 13
LC1 	block 13
LC2 	block 13
LC3 	block 15
LC4 	block 12
LC5 	block 13
LC6 	block 13
LC7 	block 15
LC8 	block 11
LC9 	block 12
LC10 	block 12
LC11 	block 13
LC12 	block 13
LC13 	block 15
LC14 	block 11
LC15 	block 12
LC16 	block 3
LC17 	block 3
LC18 	block 3
LC19 	block 3
LC20 	block 3
LC21 	block 3
LC22 	block 3
LC23 	block 3
LC24 	block 3

	init
	lim	r0,init_srel
	lim	r1,233
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
