	name t_fsqrt
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  pr_fp_num	; import
	global  fsqrt	; import

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 3 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lisp	r3,1
L5
	lr	r2,r3
	msr	r2,r3
	flt	r0,r2
	st	r3,1,r14
	sjs	r15,fsqrt   ; return value in R0
	sjs	r15,pr_fp_num   ; no return value
	l	r3,1,r14
	aisp	r3,1
	cisp	r3,8
	ble	L5
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,1 ; free up local-var space
	urs	r15

	end
