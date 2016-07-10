	name outparms
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:

	normal   ; text_section

	global  outparms__outi	; export
outparms__outi
; regs used in this function:  0 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lisp	r0,1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC0
	dataf	0.000000

	normal   ; text_section

	global  outparms__outf	; export
outparms__outf
; regs used in this function:  0 1 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dl	r0,LC0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC1
	dataef	0.000000

	normal   ; text_section

	global  outparms__outlf	; export
outparms__outlf
; regs used in this function:  0 1 2 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	efl	r0,LC1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC2
	dataf	0.000000
	dataf	1.000000
	dataf	2.000000

	normal   ; text_section

	global  outparms__outfarr	; export
outparms__outfarr
; regs used in this function:  0 1 2 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lisp	r2,6
	lr	r1,r0
	lim	r0,LC2  ; 'i' constraint
	sjs	r15,bcopy   ; no return value
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15

	static
init_srel
LC0 	block 2
LC1 	block 3
LC2 	block 2
D1.000000 	block 2
D2.000000 	block 2

	init
	lim	r0,init_srel
	lim	r1,11
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
