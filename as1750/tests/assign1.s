	name assign1
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:

	normal   ; text_section

	global  assign1__outi	; export
assign1__outi
; regs used in this function:  0 1 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r0,r1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  assign1__outf	; export
assign1__outf
; regs used in this function:  0 1 2 3 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	dlr	r0,r2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  assign1__outlf	; export
assign1__outlf
; regs used in this function:  0 1 2 3 4 5 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	eflr.m	0,3
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  assign1__outfarr	; export
assign1__outfarr
; regs used in this function:  0 1 2 3 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r3,r1
	lr	r1,r2
	lisp	r2,6
	lr	r0,r3
	sjs	r15,bcopy   ; no return value
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15

	end
