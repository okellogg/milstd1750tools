	name t2
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  sw	; import
	global  printf	; import
	konst
KLC0
	data	'i',':',' ','%','d',' ',10,0
	global  exit	; import

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r0,r0
	sjs	r15,sw   ; return value in R0
	lr	r1,r0
	st	r1,1,r14
	l	r1,1,r14
	lim	r0,LC0  ; 'i' constraint
	sjs	r15,printf   ; return value in R0
	xorr	r0,r0
	sjs	r15,exit   ; return value in R0
L1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	global  sw	; export
sw
; regs used in this function:  0 1 2 3 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r1,1,r14
	aisp	r1,1
	cisp	r1,7
	jc	gt,L11 ; Warning: this should be an *unsigned* test!
	lr	r2,r1
	aim	r2,L12
	lr	r1,r2
	l	r2,0,r1 ; P_O_A
	jc	uc,0,r2   ; tablejump label_ref=L12
L12
	data	L10 ;addr_vec_elt
	data	L11 ;addr_vec_elt
	data	L4 ;addr_vec_elt
	data	L5 ;addr_vec_elt
	data	L6 ;addr_vec_elt
	data	L7 ;addr_vec_elt
	data	L8 ;addr_vec_elt
	data	L9 ;addr_vec_elt
L4
	stc	2,2,r14   
	jc	uc,L3
L5
	stc	4,2,r14   
	jc	uc,L3
L6
	stc	6,2,r14   
	jc	uc,L3
L7
	stc	8,2,r14   
	jc	uc,L3
L8
	stc	10,2,r14   
	jc	uc,L3
L9
	stc	12,2,r14   
	jc	uc,L3
L10
	stc	0,2,r14   
	jc	uc,L3
L11
	l	r3,1,r14
	st	r3,2,r14
L3
	l	r0,2,r14
	jc	uc,L2
L2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15

	static
init_srel
LC0 	block 8

	init
	lim	r0,init_srel
	lim	r1,8
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
