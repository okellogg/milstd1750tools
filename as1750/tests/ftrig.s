	name ftrig
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	konst
KLC0
	dataef	-0.504401
	konst
KLC1
	dataef	-3.696891
	konst
KLC2
	dataef	-1.851576
	konst
KLC3
	dataef	-1.241444
	konst
KLC4
	dataef	1.000000

	normal   ; text_section

	global  fauxasin	; export
fauxasin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	sisp	r15,6  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dlr	r5,r0
	fmr	r5,r0
	xorr	r10,r10   ;extendhftqf2
	dlr	r8,r0
	efst	r8,1,r14 
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r5
	eflr.m	11,5
	efm	r11,LC0 
	eflr.m	8,5
	efa	r8,LC1 
	efst	r8,4,r14 
	efa	r5,LC2 
	efl	r2,LC3
	efdr	r2,r5
	efa	r2,4,r14 
	efst	r2,4,r14 
	efd	r11,4,r14 
	efa	r11,LC4 
	efl	r2,1,r14
	efmr	r2,r11
	dlr	r0,r2  ;trunctqfhf2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,6 ; free up local-var space
	urs	r15
	global  fsqrt	; import
	konst
KLC5
	dataf	0.000000
	konst
KLC6
	dataef	1.000000
	konst
KLC7
	dataef	0.500000
	konst
KLC8
	dataef	1.570796

	normal   ; text_section

	global  fasin	; export
fasin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dl	r5,LC5
	stc	0,1,r14   
	dlr	r7,r0
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r0
	jc	ge,L4
	fneg	r7,r0
L4
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r7
	efc	r2,LC6 
	jc	gt,L3
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r0
	jc	ge,L5
	fneg	r0,r0
	stc	1,1,r14   
L5
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r0
	efc	r2,LC7 
	jc	gt,L6
	sjs	r15,fauxasin   ; return value in R0
	dlr	r5,r0
	jc	15,L7
L6
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r0
	efl	r2,LC6
	efsr	r2,r5
	eflr.m	0,2
	efm	r0,LC7 
	sjs	r15,fsqrt   ; return value in R0
	flt	r0,r0
	sjs	r15,fauxasin   ; return value in R0
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r0
	efar	r5,r5
	efl	r2,LC8
	efsr	r2,r5
	dlr	r5,r2  ;trunctqfhf2
L7
	l	r9,1,r14
	jc	ez,L3
	fneg	r5,r5
L3
	dlr	r0,r5
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,1 ; free up local-var space
	urs	r15
	konst
KLC9
	dataef	-0.000036
	konst
KLC10
	dataef	0.002490
	konst
KLC11
	dataef	-0.080745
	konst
KLC12
	dataef	0.785398

	normal   ; text_section

	global  frsin	; export
frsin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dlr	r5,r0
	fmr	r5,r0
	xorr	r10,r10   ;extendhftqf2
	dlr	r8,r0
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r5
	eflr.m	2,5
	efm	r2,LC9 
	efa	r2,LC10 
	efmr	r2,r5
	efa	r2,LC11 
	efmr	r2,r5
	efa	r2,LC12 
	efmr	r8,r2
	dlr	r0,r8  ;trunctqfhf2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC13
	dataef	-0.000319
	konst
KLC14
	dataef	0.015850
	konst
KLC15
	dataef	-0.308424
	konst
KLC16
	dataef	1.000000

	normal   ; text_section

	global  frcos	; export
frcos
; regs used in this function:  0 1 2 3 4 5 6 7 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dlr	r5,r0
	fmr	r5,r0
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r5
	eflr.m	2,5
	efm	r2,LC13 
	efa	r2,LC14 
	efmr	r2,r5
	efa	r2,LC15 
	efmr	r2,r5
	efa	r2,LC16 
	dlr	r0,r2  ;trunctqfhf2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC17
	dataef	1.000000
	konst
KLC18
	dataf	0.000000

	normal   ; text_section

	global  fsincos	; export
fsincos
; regs used in this function:  0 1 2 3 4 5 6 7 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dlr	r3,r2
	lr	r0,r1
	lr	r2,r0
	jc	ge,L23
	aisp	r2,7
L23
	andm	r2,-8
	lr	r1,r0
	sr	r1,r2
	ucim.m	1,7
	jc	gt,L21 ; Warning: this should be an *unsigned* test!
	l	r2,L22,r1  ; P_O_A reg + label
	jc	15,0,r2   ; tablejump label_ref=L22
L22
	data	L13 ;addr_vec_elt
	data	L14 ;addr_vec_elt
	data	L15 ;addr_vec_elt
	data	L16 ;addr_vec_elt
	data	L17 ;addr_vec_elt
	data	L18 ;addr_vec_elt
	data	L19 ;addr_vec_elt
	data	L20 ;addr_vec_elt
L13
	dlr	r0,r3
	sjs	r15,frsin   ; return value in R0
	jc	15,L12
L14
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r3
	efl	r2,LC17
	efsr	r2,r5
	dlr	r0,r2  ;trunctqfhf2
	sjs	r15,frcos   ; return value in R0
	jc	15,L12
L15
	dlr	r0,r3
	sjs	r15,frcos   ; return value in R0
	jc	15,L12
L16
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r3
	efl	r2,LC17
	efsr	r2,r5
	dlr	r0,r2  ;trunctqfhf2
	sjs	r15,frsin   ; return value in R0
	jc	15,L12
L17
	dlr	r0,r3
	jc	15,L24
L18
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r3
	efl	r2,LC17
	efsr	r2,r5
	dlr	r0,r2  ;trunctqfhf2
	jc	15,L25
L19
	dlr	r0,r3
L25
	sjs	r15,frcos   ; return value in R0
	fneg	r0,r0
	jc	15,L12
L20
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r3
	efl	r2,LC17
	efsr	r2,r5
	dlr	r0,r2  ;trunctqfhf2
L24
	sjs	r15,frsin   ; return value in R0
	fneg	r0,r0
	jc	15,L12
L21
	dl	r0,LC18
L12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC19
	dataef	1.273240
	konst
KLC20
	dataef	1.000000
	konst
KLC21
	datal	-1
	konst
KLC22
	datal	4

	normal   ; text_section

	global  fsin	; export
fsin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dlr	r8,r0
	dlr	r5,r8
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r8
	jc	ge,L27
	fneg	r5,r8
L27
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r5
	efm	r2,LC19 
	dlr	r2,r2  ;trunctqfhf2
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r2
	efix	r0,r5
	eflt	r5,r0
	dlr	r5,r5  ;trunctqfhf2
	fsr	r2,r5
	dlr	r5,r2
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r5
	jc	ge,L28
	efa	r2,LC20 
	dlr	r5,r2  ;trunctqfhf2
	da	r0,LC21 
L28
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r8
	jc	ge,L29
	da	r0,LC22 
L29
	dlr	r2,r5
	sjs	r15,fsincos   ; return value in R0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  sincos	; import
	konst
KLC23
	dataef	1.273240
	konst
KLC24
	dataef	1.000000
	konst
KLC25
	datal	-1
	konst
KLC26
	datal	2

	normal   ; text_section

	global  fcos	; export
fcos
; regs used in this function:  0 1 2 3 4 5 6 7 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r0
	jc	ge,L31
	fneg	r0,r0
L31
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r0
	efm	r2,LC23 
	dlr	r2,r2  ;trunctqfhf2
	xorr	r7,r7   ;extendhftqf2
	dlr	r5,r2
	efix	r0,r5
	eflt	r5,r0
	dlr	r5,r5  ;trunctqfhf2
	fsr	r2,r5
	dlr	r5,r2
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r5
	jc	ge,L32
	efa	r2,LC24 
	dlr	r5,r2  ;trunctqfhf2
	da	r0,LC25 
L32
	xorr	r4,r4   ;extendhftqf2
	dlr	r2,r5
	da	r0,LC26 
	sjs	r15,sincos   ; return value in R0
	flt	r0,r0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15

	static
init_srel
LC0 	block 3
LC1 	block 3
LC2 	block 3
LC3 	block 3
LC4 	block 3
LC5 	block 2
LC6 	block 3
LC7 	block 3
LC8 	block 3
LC9 	block 3
LC10 	block 3
LC11 	block 3
LC12 	block 3
LC13 	block 3
LC14 	block 3
LC15 	block 3
LC16 	block 3
LC17 	block 3
LC18 	block 2
LC19 	block 3
LC20 	block 3
LC21 	block 2
LC22 	block 2
LC23 	block 3
LC24 	block 3
LC25 	block 2
LC26 	block 2

	init
	lim	r0,init_srel
	lim	r1,75
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
