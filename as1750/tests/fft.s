	name fft
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:

	normal   ; text_section

	global  reverse	; export
reverse
; regs used in this function:  0 1 2 3 4 5 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r3,r0
	xorr	r0,r0
	lr	r1,r0
	lisp	r4,1
L5
	neg	r2,r1
	lr	r5,r3
	sar	r5,r2 
	lr	r2,r5
	andm	r2,1
	jc	ez,L4
	lisp	r2,3
	sr	r2,r1
	lr	r5,r4
	slr	r5,r2 
	lr	r2,r5
	orr	r0,r2
L4
	aisp	r1,1
	cisp	r1,3
	ble	L5
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC0
	dataef	0.000000

	normal   ; text_section

	global  init_fft	; export
init_fft
; regs used in this function:  0 1 2 3 4 5 6 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r3,r3
L12
	lr	r0,r3
	st	r3,1,r14
	sjs	r15,reverse   ; return value in R0
	lr	r1,r0
	sll	r1,1
	ar	r1,r0
	l	r3,1,r14
	lr	r2,r3
	sll	r2,1
	ar	r2,r3
	efl	r4,sample,r2  ; P_O_A reg + sym
	efst	r4,spectrum,r1  ; P_O_A reg + sym 
	efl	r4,LC0
	efst	r4,48+spectrum,r1 ;P_O_A reg + const expr 
	aisp	r3,1
	cisp	r3,15
	ble	L12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,1 ; free up local-var space
	urs	r15
	global  sqrt	; import
	global  compute_output	; export
compute_output
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r9,r9
L18
	lr	r8,r9
	sll	r8,1
	ar	r8,r9
	efl	r5,spectrum,r8  ; P_O_A reg + sym
	efl	r2,48+spectrum,r8 ;P_O_A reg + const expr
	efmr	r5,r5
	efmr	r2,r2
	efar	r5,r2
	eflr.m	0,5
	dst	r8,1,r14 
	sjs	r15,sqrt   ; return value in R0
	l	r8,1,r14
	efst	r0,96+spectrum,r8 ;P_O_A reg + const expr 
	l	r9,2,r14
	aisp	r9,1
	cisp	r9,15
	ble	L18
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,2 ; free up local-var space
	urs	r15
	global  cos	; import
	global  sin	; import
	konst
KLC1
	dataef	-3.141593

	normal   ; text_section

	global  fft	; export
fft
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	sisp	r15,15  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	stc	1,2,r14   
L23
	l	r6,2,r14
	sll	r6,1
	st	r6,1,r14
	stc	0,3,r14   
	l	r7,3,r14
	c	r7,2,r14
	jc	ge,L25
	l	r2,2,r14  ;extendqihi2
	dsra	r2,16
	eflt	r6,r2
	efst	r6,10,r14 
L27
	l	r3,3,r14  ;extendqihi2
	dsra	r3,16
	eflt	r3,r3
	efm	r3,LC1 
	efd	r3,10,r14 
	eflr.m	0,3
	efst	r3,13,r14 
	sjs	r15,cos   ; return value in R0
	efst	r0,4,r14 
	efl	r3,13,r14
	eflr.m	0,3
	sjs	r15,sin   ; return value in R0
	efst	r0,7,r14 
	l	r0,3,r14
	cisp	r0,16
	jc	ge,L26
L31
	lb	r14,2
	ar	r2,r0
	lr	r11,r2
	sll	r11,1
	ar	r11,r2
	lim	r13,spectrum,r11  ; LR,inc/dec peephole
	efl	r8,4,r14
	efm	r8,0,r13 ; P_O_A 
	aim	r11,48+spectrum
	efl	r2,7,r14
	efm	r2,0,r11 ; P_O_A 
	efsr	r8,r2
	efl	r5,7,r14
	efm	r5,0,r13 ; P_O_A 
	efl	r2,4,r14
	efm	r2,0,r11 ; P_O_A 
	efar	r5,r2
	lr	r1,r0
	sll	r1,1
	ar	r1,r0
	lim	r12,spectrum,r1  ; LR,inc/dec peephole
	efl	r2,0,r12 ; P_O_A
	efsr	r2,r8
	efst	r2,0,r13 ; P_O_A 
	aim	r1,48+spectrum
	efl	r2,0,r1 ; P_O_A
	efsr	r2,r5
	efst	r2,0,r11 ; P_O_A 
	efa	r8,0,r12 ; P_O_A 
	efst	r8,0,r12 ; P_O_A 
	efa	r5,0,r1 ; P_O_A 
	efst	r5,0,r1 ; P_O_A 
	a	r0,1,r14
	cisp	r0,16
	blt	L31
L26
	incm	1,3,r14
	l	r7,3,r14
	c	r7,2,r14
	blt	L27
L25
	l	r8,1,r14
	st	r8,2,r14
	cisp	r8,16
	jc	lt,L23
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,15 ; free up local-var space
	urs	r15
	global  pr_fp_num	; import
	konst
KLC2
	dataef	-1.000000
	konst
KLC3
	dataef	1.000000

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 3 4 5 6 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r3,r3
	lim	r0,sample  ; 'i' constraint
L39
	lr	r2,r3
	sll	r2,1
	ar	r2,r3
	ar	r2,r0
	efl	r4,LC2
	efst	r4,0,r2 ; P_O_A 
	aisp	r3,1
	cisp	r3,7
	ble	L39
	lisp	r3,8
	lim	r0,sample  ; 'i' constraint
L44
	lr	r2,r3
	sll	r2,1
	ar	r2,r3
	ar	r2,r0
	efl	r4,LC3
	efst	r4,0,r2 ; P_O_A 
	aisp	r3,1
	cisp	r3,15
	ble	L44
	sjs	r15,init_fft   ; no return value
	sjs	r15,fft   ; no return value
	sjs	r15,compute_output   ; no return value
	xorr	r3,r3
L49
	lr	r2,r3
	sll	r2,1
	ar	r2,r3
	efl	r0,96+spectrum,r2 ;P_O_A reg + const expr
	st	r3,1,r14
	sjs	r15,pr_fp_num   ; return value in R0
	l	r3,1,r14
	aisp	r3,1
	cisp	r3,8
	ble	L49
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,1 ; free up local-var space
	urs	r15
	Static
	common  sample,48
	common  spectrum,144

	static
init_srel
LC0 	block 3
LC1 	block 3
LC2 	block 3
LC3 	block 3

	init
	lim	r0,init_srel
	lim	r1,12
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
