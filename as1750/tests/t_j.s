	name t_jiri
	nolist
	include "ms1750.inc"
	list

;	global	__main

; gcc2_compiled:
	global  printf	; import
	konst
KLC0
	data	84	; 'T'
	data	0	; (ascii)
	global  exit	; import

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lim	r1,567  ; 'M' constraint
	lim	r0,LC0  ; 'i' constraint
	sjs	r15,printf   ; return value in R0
	xorr	r0,r0
	sjs	r15,exit   ; return value in R0
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	urs	r15
	global  schardigs	; export

	static   ; data_section

Kschardigs
	data	48	; '0'
	data	49	; '1'
	data	50	; '2'
	data	51	; '3'
	data	52	; '4'
	data	53	; '5'
	data	54	; '6'
	data	55	; '7'
	data	56	; '8'
	data	57	; '9'
	data	65	; 'A'
	data	66	; 'B'
	data	67	; 'C'
	data	68	; 'D'
	data	69	; 'E'
	data	70	; 'F'
	data	0	; (ascii)
	global  xpf	; import
	global  putchar	; import

	normal   ; text_section

	global  printf	; export
printf
; regs used in this function:  0 1 2 14
	sim	r15,130  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r1,129,r14
	lr	r2,r14
	aim	r2,129
	lr	r1,r0
	lr	r0,r14
	aisp	r0,1
	sjs	r15,xpf   ; return value in R0
	l	r1,1,r14
	lr	r2,r14
	aisp	r2,2
	lr	r1,r1   ; from tstqi
	jc	ez,L4
L5
	lr	r0,r1
	st	r2,130,r14
	sjs	r15,putchar   ; return value in R0
	l	r2,130,r14
	l	r1,0,r2 ; P_O_A
	aisp	r2,1
	lr	r1,r1   ; from tstqi
	bnz	L5
L4
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aim	r14,130 ; free up local-var space
	urs	r15
	global  xpf	; import
	global  sprintf	; export
sprintf
; regs used in this function:  0 1 2 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r2,1,r14
	lr	r2,r14
	aisp	r2,1
	sjs	r15,xpf   ; return value in R0
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	global  sputchar	; import
	global  sprintn	; import
	global  sprintsdec	; import
	global  sputchar	; import
	global  sputchar	; import
	global  xpf	; export
xpf
; regs used in this function:  0 1 2 3 4 5 6 7 14
	sisp	r15,4  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	lr	r6,r1
	xorr	r3,r3
;	st	r3,si
	st	r2,2,r14
	jc	uc,L9
L11
	xorr	r4,r4
	cim	r2,37
	jc	nz,L32
	l	r2,0,r6 ; P_O_A
	aisp	r6,1
	lr	r1,r2
	sim	r1,48
	cisp	r1,9
	jc	le,L14 ; Warning: this should be an *unsigned* test!
	cim	r2,46
	jc	nz,L16
	l	r2,0,r6 ; P_O_A
	aisp	r6,1
	cim	r2,42
	jc	nz,L18
	l	r7,2,r14
	l	r3,0,r7 ; P_O_A
	incm	1,2,r14
	jc	uc,L19
L14
	cim	r2,48
	jc	nz,L18
	lisp	r4,1
	l	r2,0,r6 ; P_O_A
	aisp	r6,1
L18
	lr	r3,r2
	sim	r3,48
L19
	l	r2,0,r6 ; P_O_A
	aisp	r6,1
L16
	cim	r2,100
	jc	ez,L28
	jc	gt,L31
	cim	r2,99
	jc	ez,L21
	jc	uc,L20
L31
	cim	r2,115
	jc	ez,L22
	cim	r2,117
	jc	ez,L27
	jc	uc,L20
L21
	l	r7,2,r14
	l	r1,0,r7 ; P_O_A
	l	r0,1,r14
	st	r6,4,r14
	sjs	r15,sputchar   ; return value in R0
	jc	uc,L35
L22
	l	r7,2,r14
	l	r3,0,r7 ; P_O_A
	l	r2,0,r3 ; P_O_A
	aisp	r3,1
	lr	r2,r2   ; from tstqi
	jc	ez,L20
L25
	lr	r1,r2
	l	r0,1,r14
	st	r3,3,r14
	st	r6,4,r14
	sjs	r15,sputchar   ; return value in R0
	l	r3,3,r14
	l	r2,0,r3 ; P_O_A
	aisp	r3,1
	l	r6,4,r14
	lr	r2,r2   ; from tstqi
	bnz	L25
	jc	uc,L20
L27
	lr	r5,r4
	lr	r4,r3
	lisp	r3,10
	l	r7,2,r14
	l	r1,0,r7 ; P_O_A  ;extendqihi2
	dsra	r1,16
	l	r0,1,r14
	st	r6,4,r14
	sjs	r15,sprintn   ; return value in R0
	jc	uc,L35
L28
	l	r7,2,r14
	l	r1,0,r7 ; P_O_A  ;extendqihi2
	dsra	r1,16
	l	r0,1,r14
	st	r6,4,r14
	sjs	r15,sprintsdec   ; return value in R0
L35
	l	r6,4,r14
L20
	incm	1,2,r14
	xorr	r3,r3
	jc	uc,L9
L32
	lr	r1,r2
	l	r0,1,r14
	st	r3,3,r14
	st	r6,4,r14
	sjs	r15,sputchar   ; return value in R0
	l	r6,4,r14
	l	r3,3,r14
L9
	l	r2,0,r6 ; P_O_A
	aisp	r6,1
	lr	r2,r2   ; from tstqi
	jc	nz,L11
	lr	r1,r2
	l	r0,1,r14
	sjs	r15,sputchar   ; return value in R0
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aisp	r14,4 ; free up local-var space
	urs	r15
	global  sputchar	; export
sputchar
; regs used in this function:  0 1 2 3 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r2,r0
	lr	r3,r1
	cisp	r3,10
	jc	nz,L39
	lr	r1,r2
;	a	r1,si
	stc	13,0,r1 ; P_O_A   
;	incm	1,si
L39
	lr	r1,r2
;	a	r1,si
	st	r3,0,r1 ; P_O_A
;	incm	1,si
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	urs	r15
	konst
KLC1
	data	48	; '0'
	data	49	; '1'
	data	50	; '2'
	data	51	; '3'
	data	52	; '4'
	data	53	; '5'
	data	54	; '6'
	data	55	; '7'
	data	56	; '8'
	data	57	; '9'
	data	65	; 'A'
	data	66	; 'B'
	data	67	; 'C'
	data	68	; 'D'
	data	69	; 'E'
	data	70	; 'F'
	data	0	; (ascii)

	normal   ; text_section

	global  sprintn	; export
sprintn
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 14
	sisp	r15,10  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	dst	r1,2,r14 
	dst	r3,4,r14 
	st	r5,6,r14
	st	r6,7,r14
	dl	r7,2,r14
	dd	r7,4,r14 
	jc	ez,L42
	l	r6,7,r14
	sisp	r5,1
	dl	r3,4,r14
	dlr	r1,r7
	l	r0,1,r14
	dst	r7,9,r14 
	sjs	r15,sprintn   ; return value in R0
	dl	r7,9,r14
	l	r10,5,r14
	cisp	r10,16
	jc	nz,L43
	dlr	r1,r7
	dsll	r1,4
	jc	uc,L54
L43
	dlr	r1,r7
	dsll	r1,2
	dar	r1,r7
	dsll	r1,1
L54
	dl	r9,2,r14
	dsr	r9,r1
	dst	r9,2,r14 
	jc	uc,L45
L42
	l	r10,6,r14
	jc	ez,L45
	stc	1,8,r14   
	jc	uc,L55
L50
	l	r10,7,r14
	lim	r1,32  ; 'M' constraint
	lr	r10,r10   ; from tstqi
	jc	ez,L51
	lim	r1,48  ; 'M' constraint
L51
	l	r0,1,r14
	sjs	r15,sputchar   ; return value in R0
	incm	1,8,r14
L55
	l	r9,8,r14
	c	r9,6,r14 
	jc	lt,L50 ; Warning: this should be an *unsigned* test!
L45
	l	r1,3,r14
	aim	r1,LC1
	l	r1,0,r1 ; P_O_A
	l	r0,1,r14
	sjs	r15,sputchar   ; return value in R0
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aisp	r14,10 ; free up local-var space
	urs	r15
	global  sprinthex	; export
sprinthex
; regs used in this function:  0 1 2 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	sisp	r2,1
	andm	r2,7
	sll	r2,2
	jc	lt,L59
L61
	l	r1,2,r14
	neg	r2,r2
	sar	r1,r2 
	andm	r1,15
	l	r1,schardigs,r1  ; P_O_A reg + sym
	l	r0,1,r14
	st	r2,3,r14
	sjs	r15,sputchar   ; return value in R0
	l	r2,3,r14
	sisp	r2,4
	bge	L61
L59
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	global  sprintsdec	; export
sprintsdec
; regs used in this function:  0 1 2 3 4 5 6 7 14
	sisp	r15,5  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	dlr	r6,r1
	st	r3,2,r14
	st	r4,3,r14
	jc	ge,L64
	lim	r1,45  ; 'M' constraint
	l	r0,1,r14
	dst	r6,4,r14 
	sjs	r15,sputchar   ; return value in R0
	dl	r6,4,r14
	dneg	r6,r6
L64
	l	r5,3,r14
	l	r4,2,r14
	lisp	r3,10
	dlr	r1,r6
	l	r0,1,r14
	sjs	r15,sprintn   ; return value in R0
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aisp	r14,5 ; free up local-var space
	urs	r15
	global  sfloate	; import
	konst
KLC2
	dataef	0.000000
	konst
KLC3
	dataef	100000000.000000
	konst
KLC4
	dataef	1.000000
	konst
KLC5
	dataef	10.000000
	konst
KLC6
	dataef	0.500000

	normal   ; text_section

	global  sfloatf	; export
sfloatf
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 14
	sisp	r15,8  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	eflr.m	9,1
	st	r4,2,r14
	xorr	r1,r1
	dlr	r9,r9   ; from tsttqf
	jc	ge,L66
	efl	r6,LC2
	efsr	r6,r9
	lisp	r1,1
	jc	uc,L67
L66
	eflr.m	6,9
L67
	efc	r6,LC3 
	jc	le,L68
	l	r4,2,r14
	eflr.m	1,9
	l	r0,1,r14
	sjs	r15,sfloate   ; return value in R0
	jc	uc,L69
L68
	l	r12,2,r14
	jc	gt,L70
	stc	1,2,r14   
L70
	l	r12,2,r14
	cisp	r12,8
	jc	le,L71
	stc	8,2,r14   
L71
	lr	r1,r1   ; from tstqi
	jc	ez,L72
	efc	r6,LC4 
	jc	ge,L72
	lim	r1,45  ; 'M' constraint
	l	r0,1,r14
	efst	r6,3,r14 
	efst	r9,6,r14 
	sjs	r15,sputchar   ; return value in R0
	efl	r9,6,r14
	efl	r6,3,r14
L72
	efix	r4,r9
	xorr	r3,r3
	xorr	r2,r2
	lr	r1,r5
	l	r0,1,r14
	efst	r6,3,r14 
	sjs	r15,sprintsdec   ; return value in R0
	lim	r1,46  ; 'M' constraint
	l	r0,1,r14
	sjs	r15,sputchar   ; return value in R0
	efl	r6,3,r14
	efix	r1,r6
	eflt	r1,r1
	efsr	r6,r1
	xorr	r1,r1
	c	r1,2,r14 
	jc	ge,L74
	efl	r2,LC5
L76
	efmr	r6,r2
	aisp	r1,1
	c	r1,2,r14 
	blt	L76
L74
	efa	r6,LC6
	efix	r4,r6
	lisp	r3,1
	l	r2,2,r14
	lr	r1,r5
	l	r0,1,r14
	sjs	r15,sprintsdec   ; return value in R0
L69
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aisp	r14,8 ; free up local-var space
	urs	r15
	konst
KLC7
	dataef	0.500000
	konst
KLC8
	dataef	0.000000
	konst
KLC9
	dataef	10.000000
	konst
KLC10
	dataef	1.000000

	normal   ; text_section

	global  sfloate	; export
sfloate
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 14
	sisp	r15,16  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r4,2,r14
	efl	r5,LC7
	efst	r5,3,r14 
	stc	0,6,r14   
	lr	r4,r4   ; from tstqi
	jc	gt,L79
	stc	1,2,r14   
L79
	l	r6,2,r14
	cisp	r6,8
	jc	le,L80
	stc	8,2,r14   
L80
	dlr	r1,r1   ; from tsttqf
	jc	ge,L81
	efl	r5,LC8
	efsr	r5,r1
	efst	r5,14,r14 
	lisn	r4,1
	jc	uc,L82
L81
	efst	r1,14,r14 
	lisp	r4,1
L82
	efl	r5,14,r14
	efc	r5,LC9 
	jc	lt,L84
	efl	r1,LC9
L85
	efl	r5,14,r14
	efdr	r5,r1
	efst	r5,14,r14 
	incm	1,6,r14
	efcr	r5,r1
	bge	L85
L84
	efl	r1,LC10
	eflr.m	9,1
	efl	r5,14,r14
	efcr	r5,r1
	jc	ge,L102
	efl	r1,LC9
L89
	efl	r5,14,r14
	efmr	r5,r1
	efst	r5,14,r14 
	decm	1,6,r14
	efcr	r5,r9
	blt	L89
L102
	lr	r1,r4 ;extendqihi2
	dsra	r1,16
	eflt	r1,r1
	efm	r1,14,r14 
	efst	r1,11,r14 
	efix	r3,r1
	xorr	r2,r2
	lr	r1,r4
	l	r0,1,r14
	sjs	r15,sprintsdec   ; return value in R0
	lim	r1,46  ; 'M' constraint
	l	r0,1,r14
	sjs	r15,sputchar   ; return value in R0
	xorr	r9,r9
	c	r9,2,r14 
	jc	ge,L92
	efl	r1,LC9
L94
	efl	r5,3,r14
	efdr	r5,r1
	efst	r5,3,r14 
	aisp	r9,1
	c	r9,2,r14 
	blt	L94
L92
	efl	r5,14,r14
	efa	r5,3,r14
	efst	r5,14,r14 
	xorr	r9,r9
	c	r9,2,r14 
	jc	ge,L97
L99
	efl	r5,14,r14
	efix	r1,r5
	eflt	r1,r1
	efsr	r5,r1
	efm	r5,LC9 
	efst	r5,14,r14 
	efix	r6,r5
	dst	r6,11,r14 
	xorr	r3,r3
	xorr	r2,r2
	l	r1,12,r14
	l	r0,1,r14
	st	r9,10,r14
	sjs	r15,sprintsdec   ; return value in R0
	l	r9,10,r14
	aisp	r9,1
	c	r9,2,r14 
	blt	L99
L97
	lim	r1,69  ; 'M' constraint
	l	r0,1,r14
	sjs	r15,sputchar   ; return value in R0
	l	r7,6,r14
	jc	lt,L101
	lim	r1,43  ; 'M' constraint
	l	r0,1,r14
	sjs	r15,sputchar   ; return value in R0
L101
	lisp	r3,1
	lisp	r2,2
	l	r1,6,r14
	l	r0,1,r14
	sjs	r15,sprintsdec   ; return value in R0
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	aisp	r14,16 ; free up local-var space
	urs	r15
	global  exit	; export
exit
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L106
	jc	uc,L106
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	urs	r15
;	Static
;si 	block   1	; local common

	static
init_srel
LC0 	block 2
schardigs 	block 17
LC1 	block 17
LC2 	block 3
LC3 	block 3
LC4 	block 3
LC5 	block 3
LC6 	block 3
LC7 	block 3
LC8 	block 3
LC9 	block 3
LC10 	block 3

	init
	lim	r0,init_srel
	lim	r1,80
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
