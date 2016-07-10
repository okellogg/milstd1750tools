	name subtyp
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:

	normal   ; text_section

	global  _ada_subtyp	; export
_ada_subtyp
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	jc	15,L1
L1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15

	end
