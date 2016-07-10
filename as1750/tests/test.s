	name test
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	konst
KLC0
	datal	100000

	normal   ; text_section

test__p.0
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r13,1,r14
	lr	r11,r0
	sll	r11,1
	dd	r1,LC0 
	flt	r8,r11
	far	r3,r8 
	eflt	r8,r1
	efsr	r5,r8
	st	r11,0,r12 ; P_O_A
	dst	r1,1,r12 
	dst	r3,3,r12 
	efst	r5,5,r12 
	lr	r0,r12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	konst
KLC1
	dataef	10.000000
	konst
KLC2
	dataf	1.000000

	normal   ; text_section

	global  _ada_test	; export
_ada_test
; regs used in this function:  0 1 2 3 4 5 6 7 12 13 14
	sisp	r15,8  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r12,r14
	aisp	r12,1
	lr	r13,r12
	efl	r5,LC1
	dl	r3,LC2
	lim	r1,21845 ;movhi cst->reg
	lim	r2,-21846  ; range correction (val>0x7FFF) applied
	xorr	r0,r0
	sjs	r15,test__p.0   ; no return value
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,8 ; free up local-var space
	urs	r15

	static
init_srel
LC0 	block 2
LC1 	block 3
LC2 	block 2

	init
	lim	r0,init_srel
	lim	r1,7
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
