	name m
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  ldiv	; import
	global  frmul	; import
	global  frmul	; import
	konst
KLC0
	datal	32767

	normal   ; text_section

	global  modula	; export
modula
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 14
	sisp	r15,9  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r1,1,r14
	stc	0,3,r14   
L5
	l	r9,1,r14
	l	r7,0,r9 ; P_O_A
	c	r7,0,r8 ; P_O_A
	jc	gt,L6
	l	r10,4,r14
	l	r10,1,r10
	st	r10,2,r14
	xorr	r6,r6
	l	r1,4,r8
	jc	15,L7
L6
	s	r7,0,r8 ; P_O_A
	lim	r2,16383  ; 'M' constraint
	cr	r7,r2
	jc	le,L8
	lr	r7,r2
L8
	sll	r7,1
	lim	r2,32766  ; 'M' constraint
	sr	r2,r7
	c	r2,1,r8
	jc	lt,L9
	l	r4,1,r8  ;extendqihi2
	dsra	r4,16
	dsll	r4,16
	lr	r2,r2 ;extendqihi2
	dsra	r2,16
	dlr	r0,r4
	st	r4,5,r14
	st	r5,6,r14
	dst	r7,8,r14 
	sjs	r15,ldiv   ; return value in R0
	lr	r6,r0
	aisp	r6,1
	sra	r6,1
	l	r7,8,r14
	lr	r2,r7 ;extendqihi2
	dsra	r2,16
	l	r4,5,r14
	l	r5,6,r14
	dlr	r0,r4
	st	r6,7,r14
	sjs	r15,ldiv   ; return value in R0
	xorr	r2,r2 ;movhi cst->reg
	xorr	r3,r3
	lr	r1,r3
	l	r6,7,r14
	dl	r7,8,r14
	dc	r2,LC0 
	jc	le,L10
	lim	r1,32767  ; 'M' constraint
L10
	l	r2,4,r8
	sr	r2,r1
	cr	r6,r2
	jc	lt,L7
L9
	l	r1,4,r8
	neg	r0,r7
	st	r8,9,r14
	sjs	r15,frmul   ; return value in R0
	neg	r6,r0
	l	r8,9,r14
	l	r1,4,r8
	sr	r1,r6
L7
	l	r9,4,r14
	st	r6,2,r9
	st	r1,3,r9
	l	r10,2,r14
	st	r10,1,r9
	incm	1,3,r14
	aisp	r8,6
	incm	5,4,r14
	incm	1,1,r14
	l	r9,3,r14
	cisp	r9,2
	jc	le,L5
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,9 ; free up local-var space
	urs	r15

	static
init_srel
LC0 	block 2

	init
	lim	r0,init_srel
	lim	r1,2
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
