	name assign
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:

	normal   ; text_section

	global  assign__outi	; export
assign__outi
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r1,1,r14
	st	r1,2,r14
	l	r0,2,r14
	jc	15,L1
L1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,2 ; free up local-var space
	urs	r15
	global  assign__outf	; export
assign__outf
; regs used in this function:  0 1 2 3 14
	sisp	r15,4  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dst	r0,1,r14 
	dl	r2,1,r14
	dst	r2,3,r14 
	dl	r0,3,r14
	jc	15,L2
L2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,4 ; free up local-var space
	urs	r15
	global  assign__outlf	; export
assign__outlf
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,6  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	efst	r0,1,r14 
	efl	r3,1,r14
	efst	r3,4,r14 
	efl	r0,4,r14
	jc	15,L3
L3
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,6 ; free up local-var space
	urs	r15
	global  assign__outfarr	; export
assign__outfarr
; regs used in this function:  0 1 2 3 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	l	r1,2,r14
	l	r3,1,r14
	lisp	r2,6
	lr	r0,r3
	sjs	r15,bcopy   ; no return value
	jc	15,L4
L4
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,2 ; free up local-var space
	urs	r15

	end
