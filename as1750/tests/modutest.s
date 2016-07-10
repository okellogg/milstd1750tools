	name modutest
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  modula	; import
	global  incr	; import
	konst
KLC0
	datal	1999

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,8  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	stc	0,hgvar   
	stc	0,1+hgvar ;P_O_A const   
	lim	r3,49  ; 'M' constraint
	st	r3,2+hgvar ;P_O_A const
	stc	4,3+hgvar ;P_O_A const   
	lisp	r4,16
	st	r4,4+hgvar ;P_O_A const
	stc	0,5+hgvar ;P_O_A const   
	stc	1,6+hgvar ;P_O_A const   
	stc	1,7+hgvar ;P_O_A const   
	stc	5,8+hgvar ;P_O_A const   
	lim	r3,23  ; 'M' constraint
	st	r3,9+hgvar ;P_O_A const
	stc	1,10+hgvar ;P_O_A const   
	lisn	r4,1
	st	r4,11+hgvar ;P_O_A const
	stc	0,12+hgvar ;P_O_A const   
	lim	r3,85  ; 'M' constraint
	st	r3,13+hgvar ;P_O_A const
	lim	r4,32  ; 'M' constraint
	st	r4,14+hgvar ;P_O_A const
	lim	r3,100  ; 'M' constraint
	st	r3,hgpar
	stc	5,1+hgpar ;P_O_A const   
	stc	1,2+hgpar ;P_O_A const   
	stc	1,3+hgpar ;P_O_A const   
	lim	r4,32512  ; 'M' constraint
	st	r4,4+hgpar ;P_O_A const
	stc	7,5+hgpar ;P_O_A const   
	st	r3,6+hgpar ;P_O_A const
	stc	5,7+hgpar ;P_O_A const   
	stc	1,8+hgpar ;P_O_A const   
	stc	1,9+hgpar ;P_O_A const   
	st	r4,10+hgpar ;P_O_A const
	stc	7,11+hgpar ;P_O_A const   
	st	r3,12+hgpar ;P_O_A const
	stc	1,13+hgpar ;P_O_A const   
	stc	1,14+hgpar ;P_O_A const   
	stc	1,15+hgpar ;P_O_A const   
	st	r4,16+hgpar ;P_O_A const
	stc	7,17+hgpar ;P_O_A const   
	xorr	r3,r3 ;movhi cst->reg
	xorr	r4,r4
	dst	r3,7,r14 
L4
	lim	r2,4,r14  ; LR,inc/dec peephole
	lim	r1,1,r14  ; LR,inc/dec peephole
	xorr	r0,r0
	sjs	r15,modula   ; return value in R0
	lim	r0,7,r14  ; LR,inc/dec peephole
	sjs	r15,incr   ; return value in R0
	dl	r3,7,r14
	dc	r3,LC0 
	ble	L4
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,8 ; free up local-var space
	urs	r15
	Static
	common  hgpar,18
	common  cgpar,18
	common  mgpar,18
	common  hgvar,15
	common  cgvar,15
	common  mgvar,15

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
