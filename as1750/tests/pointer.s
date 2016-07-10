	name pointer
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  global	; import
	global  do_sumthn	; import

	normal   ; text_section

	global  pointer	; export
pointer
; regs used in this function:  0 1 2 3 4 5 6 7 8 14
	sisp	r15,12  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	efst	r0,1,r14 
	efst	r3,4,r14 
	efst	r6,7,r14 
	lb	r14,20
	efl	r2,0,r2 ; P_O_A
	efs	r2,global 
	efst	r2,10,r14 
	lim	r5,10,r14  ; LR,inc/dec peephole
	lim	r4,17,r14  ; LR,inc/dec peephole
	lim	r3,14,r14  ; LR,inc/dec peephole
	lim	r2,7,r14  ; LR,inc/dec peephole
	lim	r1,4,r14  ; LR,inc/dec peephole
	lim	r0,1,r14  ; LR,inc/dec peephole
	sjs	r15,do_sumthn   ; no return value
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,12 ; free up local-var space
	urs	r15

	end
