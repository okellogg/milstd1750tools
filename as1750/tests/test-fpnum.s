	name test-fpnum
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  puts	; import
	konst
KLC0
	data	13,10,'(','e','x','p','e','c','t','e','d',':',' ','1','0'
	data	'.','0',')',' ',' ','a','c','t','u','a','l',':',' ',0
	global  pr_efp_num	; import
	konst
KLC1
	data	13,10,'(','e','x','p','e','c','t','e','d',':',' ','2','.'
	data	'5',')',' ',' ',' ','a','c','t','u','a','l',':',' ',0
	konst
KLC2
	data	13,10,'(','e','x','p','e','c','t','e','d',':',' ','-','5'
	data	'.','0',')',' ',' ','a','c','t','u','a','l',':',' ',0
	konst
KLC3
	data	13,10,'(','e','x','p','e','c','t','e','d',':',' ','2','.'
	data	'0',')',' ',' ',' ','a','c','t','u','a','l',':',' ',0
	konst
KLC4
	data	13,10,'(','e','x','p','e','c','t','e','d',':',' ','0','.'
	data	'8','8','8','8','8','8','8','8','8','8','8','9',')',' ',' '
	data	' ','a','c','t','u','a','l',':',' ',0
	konst
KLC5
	data	13,10,0
	konst
KLC6
	dataef	10.000000
	konst
KLC7
	dataef	2.500000
	konst
KLC8
	dataef	-5.000000
	konst
KLC9
	dataef	-1.000000
	konst
KLC10
	dataef	-2.111111

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lim	r0,LC0  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,LC6
	sjs	r15,pr_efp_num   ; no return value
	lim	r0,LC1  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,LC7
	sjs	r15,pr_efp_num   ; no return value
	lim	r0,LC2  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,LC8
	sjs	r15,pr_efp_num   ; no return value
	lim	r0,LC3  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,LC9
	sjs	r15,pr_efp_num   ; no return value
	lim	r0,LC4  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,LC10
	sjs	r15,pr_efp_num   ; no return value
	lim	r0,LC5  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15

	static
init_srel
LC0 	block 29
LC1 	block 29
LC2 	block 29
LC3 	block 29
LC4 	block 40
LC5 	block 3
LC6 	block 3
LC7 	block 3
LC8 	block 3
LC9 	block 3
LC10 	block 3

	init
	lim	r0,init_srel
	lim	r1,174
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
