	name t_assign
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  assign__outi	; import
	global  system__secondary_stack__ss_mark	; import
	global  ada__text_io__put_line$2	; import
	global  system__img_int__image_integer	; import
	global  system__secondary_stack__ss_release	; import

	normal   ; text_section

	global  _ada_t_assign	; export
_ada_t_assign
; regs used in this function:  0 1 2 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lisp	r0,1
	sjs	r15,assign__outi   ; return value in R0
	st	r0,1,r14
	sjs	r15,system__secondary_stack__ss_mark   ; return value in R0
	st	r0,2,r14
	l	r0,1,r14
	sjs	r15,system__img_int__image_integer   ; return value in R0
	dlr	r1,r0
	dlr	r0,r1
	sjs	r15,ada__text_io__put_line$2   ; no return value
	l	r0,2,r14
	sjs	r15,system__secondary_stack__ss_release   ; no return value
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,2 ; free up local-var space
	urs	r15

	end
