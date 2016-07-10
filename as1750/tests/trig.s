	name trig
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

	global  auxasin	; export
auxasin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	efst	r0,1,r14 
	eflr.m	5,0
	efm	r5,1,r14 
	eflr.m	11,5
	efm	r11,LC0 
	eflr.m	8,5
	efa	r8,LC1 
	efa	r5,LC2 
	efl	r2,LC3
	efdr	r2,r5
	efar	r8,r2
	efdr	r11,r8
	efa	r11,LC4 
	efl	r0,1,r14
	efmr	r0,r11
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,3 ; free up local-var space
	urs	r15
	global  fabs	; import
	global  sqrt	; import
	konst
KLC5
	dataef	0.000000
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

	global  asin	; export
asin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	eflr.m	5,0
	efl	r8,LC5
	stc	0,1,r14   
	eflr.m	2,5
	jc	ge,L4
	eflr.m	2,8
	efsr	r2,r5
L4
	efc	r2,LC6 
	jc	gt,L3
	dlr	r5,r5   ; from tsttqf
	jc	ge,L5
	efl	r2,LC5
	efsr	r2,r5
	eflr.m	5,2
	stc	1,1,r14   
L5
	efc	r5,LC7 
	jc	gt,L6
	eflr.m	0,5
	sjs	r15,auxasin   ; return value in R0
	eflr.m	8,0
	jc	15,L7
L6
	efl	r2,LC6
	efsr	r2,r5
	eflr.m	0,2
	efm	r0,LC7 
	sjs	r15,sqrt   ; return value in R0
	sjs	r15,auxasin   ; return value in R0
	eflr.m	5,0
	efar	r5,r0
	efl	r2,LC8
	eflr.m	8,2
	efsr	r8,r5
L7
	l	r11,1,r14
	jc	ez,L3
	efl	r2,LC5
	efsr	r2,r8
	eflr.m	8,2
L3
	eflr.m	0,8
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

	global  rsin	; export
rsin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	eflr.m	8,0
	eflr.m	5,8
	efmr	r5,r8
	eflr.m	2,5
	efm	r2,LC9 
	efa	r2,LC10 
	efmr	r2,r5
	efa	r2,LC11 
	efmr	r2,r5
	efa	r2,LC12 
	efmr	r8,r2
	eflr.m	0,8
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

	global  rcos	; export
rcos
; regs used in this function:  0 1 2 3 4 5 6 7 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	eflr.m	5,0
	efmr	r5,r0
	eflr.m	2,5
	efm	r2,LC13 
	efa	r2,LC14 
	efmr	r2,r5
	efa	r2,LC15 
	efmr	r2,r5
	eflr.m	0,2
	efa	r0,LC16 
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC17
	dataef	1.000000
	konst
KLC18
	dataef	0.000000

	normal   ; text_section

	global  sincos	; export
sincos
; regs used in this function:  0 1 2 3 4 5 6 7 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	eflr.m	5,2
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
	eflr.m	0,5
	jc	15,L24
L14
	efl	r2,LC17
	eflr.m	0,2
	efsr	r0,r5
	jc	15,L25
L15
	eflr.m	0,5
L25
	sjs	r15,rcos   ; return value in R0
	eflr.m	3,0
	jc	15,L12
L16
	efl	r2,LC17
	eflr.m	0,2
	efsr	r0,r5
L24
	sjs	r15,rsin   ; return value in R0
	eflr.m	3,0
	jc	15,L12
L17
	eflr.m	0,5
	jc	15,L26
L18
	efl	r2,LC17
	eflr.m	0,2
	efsr	r0,r5
	sjs	r15,rcos   ; return value in R0
	jc	15,L27
L19
	eflr.m	0,5
	sjs	r15,rcos   ; return value in R0
	jc	15,L27
L20
	efl	r2,LC17
	eflr.m	0,2
	efsr	r0,r5
L26
	sjs	r15,rsin   ; return value in R0
L27
	efl	r3,LC18
	efsr	r3,r0
	jc	15,L12
L21
	efl	r3,LC18
L12
	eflr.m	0,3
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC19
	dataef	0.000000
	konst
KLC20
	dataef	1.273240
	konst
KLC21
	dataef	1.000000
	konst
KLC22
	datal	-1
	konst
KLC23
	datal	4

	normal   ; text_section

	global  sin	; export
sin
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	eflr.m	8,0
	eflr.m	5,8
	jc	ge,L29
	efl	r2,LC19
	eflr.m	5,2
	efsr	r5,r8
L29
	efm	r5,LC20 
	efix	r0,r5
	eflt	r2,r0
	efsr	r5,r2
	eflr.m	2,5
	jc	ge,L30
	efa	r2,LC21 
	da	r0,LC22 
L30
	dlr	r8,r8   ; from tsttqf
	jc	ge,L31
	da	r0,LC23 
L31
	sjs	r15,sincos   ; return value in R0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC24
	dataef	0.000000
	konst
KLC25
	dataef	1.273240
	konst
KLC26
	dataef	1.000000
	konst
KLC27
	datal	-1
	konst
KLC28
	datal	2

	normal   ; text_section

	global  cos	; export
cos
; regs used in this function:  0 1 2 3 4 5 6 7 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	eflr.m	5,0
	jc	ge,L33
	efl	r2,LC24
	efsr	r2,r5
	eflr.m	5,2
L33
	efm	r5,LC25 
	efix	r0,r5
	eflt	r2,r0
	efsr	r5,r2
	eflr.m	2,5
	jc	ge,L34
	efa	r2,LC26 
	da	r0,LC27 
L34
	da	r0,LC28 
	sjs	r15,sincos   ; return value in R0
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
LC5 	block 3
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
LC18 	block 3
LC19 	block 3
LC20 	block 3
LC21 	block 3
LC22 	block 2
LC23 	block 2
LC24 	block 3
LC25 	block 3
LC26 	block 3
LC27 	block 2
LC28 	block 2

	init
	lim	r0,init_srel
	lim	r1,83
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
