	name modula
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
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	sisp	r15,15  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r1,1,r14
	stb	r14,2
	cisp	r0,1
	jc	ez,L4
	jc	gt,L8
	lr	r0,r0   ; from tstqi
	jc	ez,L3
	jc	15,L2
L8
	cisp	r0,2
	jc	ez,L5
	jc	15,L2
L3
	lim	r9,hgpar  ; 'i' constraint
	lim	r8,hgvar  ; 'i' constraint
	jc	15,L2
L4
	lim	r9,cgpar  ; 'i' constraint
	lim	r8,cgvar  ; 'i' constraint
	jc	15,L2
L5
	lim	r9,mgpar  ; 'i' constraint
	lim	r8,mgvar  ; 'i' constraint
L2
	stc	0,4,r14   
L12
	l	r12,1,r14
	l	r2,0,r12 ; P_O_A
	jc	lt,L13
	lr	r6,r2
	stc	1,3,r14   
	jc	15,L14
L13
	l	r13,1,r14
	l	r2,0,r13 ; P_O_A
	cim	r2,-32768
	jc	nz,L15
	lim	r6,32767  ; 'M' constraint
	jc	15,L16
L15
	l	r12,1,r14
	l	r2,0,r12 ; P_O_A
	neg	r6,r2
L16
	lisn	r13,1
	st	r13,3,r14
L14
	c	r6,0,r9 ; P_O_A
	jc	gt,L17
	l	r12,1,r8
	st	r12,3,r14
	xorr	r7,r7
	l	r5,4,r9
	jc	15,L18
L17
	s	r6,0,r9 ; P_O_A
	cim	r6,16383
	jc	le,L19
	lim	r6,16383  ; 'M' constraint
L19
	sll	r6,1
	lim	r2,32766  ; 'M' constraint
	sr	r2,r6
	c	r2,1,r9
	jc	lt,L20
	l	r4,1,r9  ;extendqihi2
	dsra	r4,16
	dsll	r4,16
	lr	r2,r2 ;extendqihi2
	dsra	r2,16
	dlr	r0,r4
	efst	r4,6,r14 
	st	r8,10,r14
	st	r9,11,r14
	st	r10,12,r14
	st	r11,13,r14
	sjs	r15,ldiv   ; return value in R0
	da	r0,LC0 
	dsra	r0,1
	lr	r7,r1
	l	r6,8,r14
	lr	r2,r6 ;extendqihi2
	dsra	r2,16
	l	r4,6,r14
	l	r5,7,r14
	dlr	r0,r4
	st	r7,9,r14
	sjs	r15,ldiv   ; return value in R0
	da	r0,LC0 
	dsra	r0,1
	lr	r5,r1
	l	r6,8,r14
	efl	r7,9,r14
	l	r10,12,r14
	l	r11,13,r14
	dc	r0,LC1 
	jc	le,L21
	lim	r5,32767  ; 'M' constraint
L21
	l	r2,4,r9
	sr	r2,r5
	cr	r7,r2
	jc	lt,L18
	l	r1,4,r9
	neg	r0,r6
	st	r8,10,r14
	st	r9,11,r14
	st	r10,12,r14
	st	r11,13,r14
	sjs	r15,frmul   ; return value in R0
	neg	r7,r0
	l	r9,11,r14
	l	r5,4,r9
	sr	r5,r7
	l	r8,10,r14
	l	r10,12,r14
	l	r11,13,r14
	jc	15,L18
L20
	l	r1,4,r9
	neg	r0,r6
	st	r8,10,r14
	st	r9,11,r14
	st	r10,12,r14
	st	r11,13,r14
	sjs	r15,frmul   ; return value in R0
	neg	r7,r0
	l	r9,11,r14
	l	r5,4,r9
	sr	r5,r7
	l	r11,13,r14
	l	r10,12,r14
	l	r8,10,r14
L18
	st	r7,2,r8
	st	r5,3,r8
	l	r3,5,r9
	l	r1,4,r8
	l	r0,3,r14
	xor	r0,1,r8 
	abs	r0,r0
	neg	r0,r0
	srl	r0,15
	l	r2,0,r8 ; P_O_A
	jc	ez,L26
	cr	r1,r7
	jc	ge,L27
	lr	r0,r0   ; from tstqi
	jc	ez,L26
L27
	c	r1,2,r9
	jc	ge,L29
	lr	r1,r1   ; from tstqi
	jc	nz,L28
L29
	xorr	r1,r1
	lr	r0,r0   ; from tstqi
	jc	nz,L77
	st	r1,0,r8 ; P_O_A
	jc	15,L34
L28
	lr	r0,r0   ; from tstqi
	jc	ez,L33
	lr	r2,r3
	s	r2,2,r9
	lr	r5,r1
	ar	r5,r2
L33
	l	r7,2,r9
	jc	15,L34
L26
	cr	r1,r5
	jc	ge,L36
	lr	r0,r0   ; from tstqi
	jc	ez,L34
L36
	c	r1,3,r9
	jc	lt,L37
	xorr	r1,r1
	stc	1,0,r8 ; P_O_A   
	l	r12,3,r14
	st	r12,1,r8
	jc	15,L34
L37
	l	r5,3,r9
	lr	r0,r0   ; from tstqi
	jc	ez,L34
	xorr	r1,r1
	stc	1,0,r8 ; P_O_A   
L77
	l	r13,3,r14
	st	r13,1,r8
L34
	xorr	r4,r4
	l	r2,0,r8 ; P_O_A
	jc	ez,L40
	lr	r2,r7
	sr	r2,r1
	cr	r2,r3
	jc	lt,L41
	ar	r1,r3
	lr	r0,r3
	lr	r10,r4
	lr	r3,r3   ; from tstqi
	jc	le,L43
L45
	lr	r2,r10
	sll	r2,1
	lim	r10,1,r2  ; LR,inc/dec peephole
	sisp	r0,1
	bgt	L45
L43
	lisp	r2,8
	sr	r2,r3
	slr	r10,r2 
	jc	15,L48
L41
	xorr	r1,r1
	lr	r11,r2
	jc	15,L53
L40
	lr	r2,r5
	sr	r2,r1
	cr	r2,r3
	jc	lt,L49
	ar	r1,r3
	xorr	r10,r10
	jc	15,L48
L49
	lr	r11,r7
	lr	r1,r2
	lisp	r4,1
L48
	lr	r4,r4   ; from tstqi
	jc	ez,L52
L53
	xorr	r10,r10
	lr	r4,r7
	ar	r4,r5
	cr	r1,r3
	jc	ge,L55
L56
	lr	r2,r3
	sr	r2,r1
	cr	r11,r2
	jc	le,L57
	lr	r11,r2
L57
	lr	r11,r11   ; from tstqi
	jc	ge,L58
	xorr	r11,r11
L58
	st	r11,5,r14
	a	r1,5,r14
	lr	r2,r3
	sr	r2,r1
	st	r4,15,r14
	cr	r4,r2
	jc	le,L59
	stb	r14,15
L59
	l	r12,15,r14
	jc	ge,L61
	stc	0,15,r14   
L61
	ar	r1,r5
	xorr	r2,r2
	lr	r11,r11   ; from tstqi
	jc	le,L62
	lr	r0,r11
L66
	sll	r2,1
	aisp	r2,1
	sisp	r0,1
	bgt	L66
L62
	ar	r10,r2
	l	r13,15,r14
	slr	r10,r13 
	lr	r11,r7
	cr	r1,r3
	blt	L56
L55
	lisp	r2,8
	sr	r2,r3
	slr	r10,r2 
	l	r12,15,r14
	jc	nz,L69
	l	r13,5,r14
	cr	r13,r7
	jc	ge,L69
	stc	1,0,r8 ; P_O_A   
	l	r1,5,r14
	jc	15,L52
L69
	stc	0,0,r8 ; P_O_A   
	l	r1,15,r14
L52
	st	r1,4,r8
	l	r2,1,r8
	cisn	r2,1
	jc	ez,L73
	cisp	r2,1
	jc	nz,L74
	lr	r13,r10
	andm	r13,255
	jc	15,L78
L73
	lr	r13,r10
	orim	r13,-256
L78
	l	r12,2,r14
	st	r13,0,r12 ; P_O_A
	jc	15,L71
L74
	l	r12,2,r14
	stc	0,0,r12 ; P_O_A   
L71
	l	r13,3,r14
	st	r13,1,r8
	incm	1,4,r14
	aisp	r9,6
	aisp	r8,5
	incm	1,1,r14
	incm	1,2,r14
	l	r12,4,r14
	cisp	r12,2
	jc	le,L12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,15 ; free up local-var space
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
