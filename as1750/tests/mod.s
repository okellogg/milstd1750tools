	name mod
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  hgpar	; import
	global  hgvar	; import
	global  cgpar	; import
	global  cgvar	; import
	global  mgpar	; import
	global  mgvar	; import
	global  ldiv	; import
	global  frmul	; import
	konst
KLC0
	datal	1
	konst
KLC1
	datal	32767

	normal   ; text_section

	global  modula	; export
modula
; regs used in this function:  0 1 2 3 4 5 6 7 8 14
	sisp	r15,8  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r2,r0
	cisp	r2,1
	jc	ez,L4
	jc	gt,L8
	lr	r2,r2   ; from tstqi
	jc	ez,L3
	jc	15,L2
L8
	cisp	r2,2
	jc	ez,L5
	jc	15,L2
L3
	lim	r7,hgpar  ; 'i' constraint
	lim	r8,hgvar  ; 'i' constraint
	jc	15,L19
L4
	lim	r7,cgpar  ; 'i' constraint
	lim	r8,cgvar  ; 'i' constraint
	jc	15,L19
L5
	lim	r7,mgpar  ; 'i' constraint
	lim	r8,mgvar  ; 'i' constraint
L19
	st	r8,4,r14
L2
	stc	0,1,r14   
	lim	r8,32766  ; 'M' constraint
	s	r8,2,r14
	st	r8,3,r14
L12
	l	r8,3,r14
	c	r8,1,r7
	jc	lt,L13
	l	r2,1,r7  ;extendqihi2
	dsra	r2,16
	dlr	r5,r2
	dsll	r5,16
	l	r2,3,r14  ;extendqihi2
	dsra	r2,16
	dlr	r0,r5
	st	r5,6,r14
	st	r6,7,r14
	st	r7,8,r14
	sjs	r15,ldiv   ; return value in R0
	da	r0,LC0 
	dsra	r0,1
	lr	r4,r1
	l	r2,2,r14  ;extendqihi2
	dsra	r2,16
	l	r5,6,r14
	l	r6,7,r14
	dlr	r0,r5
	st	r4,5,r14
	sjs	r15,ldiv   ; return value in R0
	da	r0,LC0 
	dsra	r0,1
	lr	r3,r1
	l	r4,5,r14
	l	r7,8,r14
	dc	r0,LC1 
	jc	le,L14
	lim	r3,32767  ; 'M' constraint
L14
	l	r2,4,r7
	sr	r2,r3
	cr	r4,r2
	jc	lt,L17
L13
	l	r1,4,r7
	l	r8,2,r14
	neg	r0,r8
	st	r7,8,r14
	sjs	r15,frmul   ; return value in R0
	l	r7,8,r14
	l	r3,4,r7
	ar	r3,r0
L17
	l	r8,4,r14
	st	r3,1,r8
	incm	1,1,r14
	aisp	r7,6
	incm	5,4,r14
	l	r8,1,r14
	cisp	r8,2
	ble	L12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,8 ; free up local-var space
	urs	r15

	static
init_srel
LC0 	block 2
LC1 	block 2

	init
	lim	r0,init_srel
	lim	r1,4
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
