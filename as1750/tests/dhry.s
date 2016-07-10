	name dhry
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:

	normal   ; text_section

	global  increment	; export
increment
; regs used in this function:  0 1 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r1,r0
	incm	1,0,r1 ; P_O_A
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  bcopy	; import
	global  proc_3	; import
	global  proc_6	; import
	global  proc_7	; import
	global  proc_1	; export
proc_1
; regs used in this function:  0 1 2 3 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	lim	r2,33  ; 'M' constraint
	lim	r1,globrec  ; 'i' constraint
	lr	r3,r0
	l	r0,0,r3 ; P_O_A
	sjs	r15,bcopy   ; no return value
	l	r3,1,r14
	stc	5,2,r3   
	l	r1,0,r3 ; P_O_A
	stc	5,2,r1   
	l	r1,0,r3 ; P_O_A
	l	r0,0,r1 ; P_O_A
	sjs	r15,proc_3   ; return value in R0
	l	r3,1,r14
	l	r1,0,r3 ; P_O_A
	stc	6,2,r1   
	l	r1,0,r3 ; P_O_A
	l	r1,1,r1
	l	r0,1,r3
	sjs	r15,proc_6   ; return value in R0
	l	r3,1,r14
	l	r1,0,r3 ; P_O_A
	l	r3,globrec
	st	r3,0,r1 ; P_O_A
	l	r3,1,r14
	l	r1,0,r3 ; P_O_A
	l	r2,2,r1
	lisp	r1,10
	lr	r0,r2
	sjs	r15,proc_7   ; return value in R0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,1 ; free up local-var space
	urs	r15
	global  proc_2	; export
proc_2
; regs used in this function:  0 1 2 3 4 5 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r2,r0
	l	r1,0,r2 ; P_O_A
	aisp	r1,10
	lim	r4,65  ; 'M' constraint
L4
	l	r0,globc_1
	cr	r0,r4
	jc	nz,L6
	sisp	r1,1
	lr	r5,r1
	s	r5,int_glob
	st	r5,0,r2 ; P_O_A
	xorr	r3,r3
L6
	lr	r3,r3   ; from tstqi
	bnz	L4
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  proc_7	; import
	global  proc_3	; export
proc_3
; regs used in this function:  0 1 2 3 4 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r3,r0
	lim	r0,globrec  ; 'i' constraint
	jc	ez,L10
	lim	r2,33  ; 'M' constraint
	l	r1,globrec
	lr	r0,r3
	sjs	r15,bcopy   ; no return value
	jc	15,L11
L10
	lim	r4,100  ; 'M' constraint
	st	r4,int_glob
L11
	lim	r2,2+globrec  ; 'i' constraint
	l	r1,int_glob
	lisp	r0,10
	sjs	r15,proc_7   ; return value in R0
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  proc_4	; export
proc_4
; regs used in this function:  0 1 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lim	r1,66  ; 'M' constraint
	st	r1,globc_2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  proc_5	; export
proc_5
; regs used in this function:  0 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lim	r0,65  ; 'M' constraint
	st	r0,globc_1
	stc	0,bool_glob   
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  func_3	; import
	global  proc_6	; export
proc_6
; regs used in this function:  0 1 2 3 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	lr	r2,r1
	l	r0,1,r14
	stb	r14,2
	sjs	r15,func_3   ; return value in R0
	lb	r14,2
	lr	r0,r0   ; from tstqi
	jc	nz,L17
	stc	3,0,r2 ; P_O_A   
L17
	l	r3,1,r14
	ucim.m	3,4
	jc	gt,L18 ; Warning: this should be an *unsigned* test!
	l	r1,L26,r3  ; P_O_A reg + label
	jc	15,0,r1   ; tablejump label_ref=L26
L26
	data	L28 ;addr_vec_elt
	data	L20 ;addr_vec_elt
	data	L23 ;addr_vec_elt
	data	L18 ;addr_vec_elt
	data	L25 ;addr_vec_elt
L20
	l	r0,int_glob
	cim	r0,100
	jc	le,L21
L28
	stc	0,0,r2 ; P_O_A   
	jc	15,L18
L21
	stc	3,0,r2 ; P_O_A   
	jc	15,L18
L23
	stc	1,0,r2 ; P_O_A   
	jc	15,L18
L25
	stc	2,0,r2 ; P_O_A   
L18
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,2 ; free up local-var space
	urs	r15
	global  proc_7	; export
proc_7
; regs used in this function:  0 1 2 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	aisp	r0,2
	ar	r1,r0
	st	r1,0,r2 ; P_O_A
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  proc_8	; export
proc_8
; regs used in this function:  0 1 2 3 4 5 6 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r6,r0
	lr	r5,r1
	lim	r4,5,r2  ; LR,inc/dec peephole
	lr	r1,r6
	ar	r1,r4
	st	r3,0,r1 ; P_O_A
	st	r3,1,r1
	st	r4,30,r1
	lr	r3,r4
	lim	r0,6,r2  ; LR,inc/dec peephole
	cr	r4,r0
	jc	gt,L32
	lr	r0,r4
	sll	r0,4
	sr	r0,r4
	sll	r0,1
	ar	r0,r5
	lim	r2,1,r4  ; LR,inc/dec peephole
L34
	lr	r1,r0
	ar	r1,r3
	st	r4,0,r1 ; P_O_A
	aisp	r3,1
	cr	r3,r2
	ble	L34
L32
	lr	r1,r4
	sll	r1,5
	sr	r1,r4
	ar	r1,r5
	incm	1,-1,r1
	lr	r2,r6
	ar	r2,r4
	l	r2,0,r2 ; P_O_A
	st	r2,600,r1
	stc	5,int_glob   
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  func_1	; export
func_1
; regs used in this function:  0 1 2 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r2,r0
	xorr	r0,r0
	cr	r2,r1
	jc	nz,L37
	lisp	r0,1
L37
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  strcmp	; import
	global  func_2	; export
func_2
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	lisp	r3,2
L43
	l	r1,2,r14
	ar	r1,r3
	lb	r14,1
	ar	r2,r3
	l	r1,1,r1
	l	r0,0,r2 ; P_O_A
	st	r3,3,r14
	sjs	r15,func_1   ; return value in R0
	l	r3,3,r14
	lr	r0,r0   ; from tstqi
	jc	nz,L41
	aisp	r3,1
L41
	cisp	r3,2
	ble	L43
	lim	r0,65  ; 'M' constraint
	lisn	r0,1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,3 ; free up local-var space
	urs	r15
	global  func_3	; export
func_3
; regs used in this function:  0 1 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lr	r1,r0
	lisn	r0,1
	cisp	r1,2
	jc	ez,L52
	xorr	r0,r0
L52
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC0
	data	'D','H','R','Y','S','T','O','N','E',' ','P','R','O','G','R'
	data	'A','M',',',' ','2','N','D',' ','S','T','R','I','N','G'
	data	0

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,5  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	stc	0,1,r14   
L58
	lim	r0,1,r14  ; LR,inc/dec peephole
	sjs	r15,increment   ; return value in R0
	l	r0,1,r14
	cim	r0,9999
	ble	L58
	stc	0,1,r14   
L62
	sjs	r15,proc_5   ; return value in R0
	sjs	r15,proc_4   ; return value in R0
	lisp	r4,2
	lim	r1,LC0  ; 'i' constraint
	l	r0,3,r14
	st	r4,5,r14
	sjs	r15,func_2   ; return value in R0
	abs	r0,r0
	sisp	r0,1
	srl	r0,15
	st	r0,bool_glob
	l	r4,5,r14
L65
	lr	r0,r4
	sll	r0,2
	ar	r0,r4
	sisp	r0,3
	st	r0,2,r14
	lim	r2,2,r14  ; LR,inc/dec peephole
	lisp	r1,3
	lr	r0,r4
	st	r4,5,r14
	sjs	r15,proc_7   ; return value in R0
	l	r4,5,r14
	aisp	r4,1
	cisp	r4,3
	blt	L65
	l	r3,2,r14
	lr	r2,r4
	lim	r1,gl_arr2  ; 'i' constraint
	lim	r0,gl_arr1  ; 'i' constraint
	st	r4,5,r14
	sjs	r15,proc_8   ; return value in R0
	lim	r0,globrec  ; 'i' constraint
	sjs	r15,proc_1   ; return value in R0
	lim	r2,65  ; 'M' constraint
	l	r4,5,r14
	c	r2,globc_2
	jc	gt,L68
L70
	lim	r1,67  ; 'M' constraint
	lr	r0,r2
	stb	r14,4
	st	r4,5,r14
	sjs	r15,func_1   ; return value in R0
	lb	r14,4
	l	r4,5,r14
	lisp	r5,1
	cr	r5,r0
	jc	nz,L69
	lisp	r1,1
	xorr	r0,r0
	stb	r14,4
	st	r4,5,r14
	sjs	r15,proc_6   ; return value in R0
	l	r4,5,r14
	lb	r14,4
L69
	aisp	r2,1
	c	r2,globc_2
	ble	L70
L68
	lisp	r0,3
	msr	r0,r4
	st	r0,2,r14
	lr	r0,r4
	sjs	r15,proc_2   ; return value in R0
	lim	r0,1,r14  ; LR,inc/dec peephole
	sjs	r15,increment   ; return value in R0
	l	r0,1,r14
	cim	r0,9999
	jc	le,L62
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,5 ; free up local-var space
	urs	r15
	Static
	common  globrec,33
	common  int_glob,1
	common  bool_glob,1
	common  globc_1,1
	common  globc_2,1
	common  gl_arr1,50
	common  gl_arr2,900
	common  pointer_glob_next,1

	static
init_srel
LC0 	block 30

	init
	lim	r0,init_srel
	lim	r1,30
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
