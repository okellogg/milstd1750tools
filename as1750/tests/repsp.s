	name repsp
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:

	normal   ; text_section

	global  _ada_repsp	; export
_ada_repsp
; regs used in this function:  0 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	l	r0,1,r14
	orim	r0,7
	st	r0,1,r14  ; eliminated previous redundant load
	andm	r0,-505
	st	r0,1,r14  ; eliminated previous redundant load
	orim	r0,248
	st	r0,1,r14  ; eliminated previous redundant load
	andm	r0,-3585
	st	r0,1,r14  ; eliminated previous redundant load
	orim	r0,-4096
	st	r0,1,r14
	l	r0,3,r14
	andm	r0,-256
	st	r0,3,r14  ; eliminated previous redundant load
	andm	r0,255
	st	r0,3,r14  ; eliminated previous redundant load
	orim	r0,256
	st	r0,3,r14
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,3 ; free up local-var space
	urs	r15

	end
