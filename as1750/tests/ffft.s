	name fft

	konst
n_radix_2	data	4
newline		data	10,13,0
	normal

	global	reverse
reverse	; input: R0  ; output : R0
	lr	r2,r0
	xorr	r0,r0
	lisp	r3,16
	s	r3,n_radix_2
	lisp	r1,15
loop	tvbr	r3,r2
	bez	next
	svbr	r1,r0
next	aisp	r3,1
	sisp	r1,1
	bge	loop
	urs	r15

	konst
KLC0	data 0

	normal   ; text_section

	global  init_fft	; export
init_fft
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r3,r3
L12
	lr	r0,r3
	st	r3,1,r14
	sjs	r15,reverse   ; return value in R0
	lr	r1,r0
	sll	r1,1
	l	r3,1,r14
	lr	r2,r3
	sll	r2,1
	dl	r4,sample,r2  ; P_O_A reg + sym
	dst	r4,spectrum,r1  ; P_O_A reg + sym 
	dl	r4,LC0
	dst	r4,32+spectrum,r1 ;P_O_A reg + const expr 
	aisp	r3,1
	cisp	r3,15
	ble	L12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,1 ; free up local-var space
	urs	r15
	global  fsqrt	; import
	global  compute_output	; export
compute_output
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r5,r5
L18
	lr	r4,r5
	sll	r4,1
	dl	r0,spectrum,r4  ; P_O_A reg + sym
	dl	r2,32+spectrum,r4 ;P_O_A reg + const expr
	fmr	r0,r0
	fmr	r2,r2
	far	r0,r2
	dst	r4,1,r14 
	sjs	r15,fsqrt   ; return value in R0
	l	r4,1,r14
	dst	r0,64+spectrum,r4 ;P_O_A reg + const expr 
	l	r5,2,r14
	aisp	r5,1
	cisp	r5,15
	ble	L18
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,2 ; free up local-var space
	urs	r15
	global  fcos	; import
	global  fsin	; import
	konst
KLC1
	dataf	-3.1415926

	normal   ; text_section

	global  fft	; export
fft
; regs used in this function:  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	sisp	r15,9  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	stc	1,2,r14   
L23
	l	r11,2,r14
	sll	r11,1
	st	r11,1,r14
	xorr	r13,r13
	c	r13,2,r14
	jc	ge,L25
L27
	flt	r2,r13
	dlr	r0,r2
	fm	r0,LC1 
	l	r11,2,r14
	flt	r2,r11
	fdr	r0,r2
	dlr	r2,r0
	dst	r2,5,r14 
	st	r13,7,r14
	sjs	r15,fcos   ; return value in R0
	dstb	r14,3
	dl	r2,5,r14
	dlr	r0,r2
	sjs	r15,fsin   ; return value in R0
	dstb	r14,8
	l	r13,7,r14
	lr	r10,r13
	cisp	r13,16
	jc	ge,L26
L31
	l	r6,2,r14
	ar	r6,r10
	sll	r6,1
	lim	r9,spectrum,r6  ; LR,inc/dec peephole
	dl	r4,3,r14
	fm	r4,0,r9 ; P_O_A 
	aim	r6,32+spectrum
	dl	r2,8,r14
	fm	r2,0,r6 ; P_O_A 
	fsr	r4,r2
	dlb	r14,8
	fm	r0,0,r9 ; P_O_A 
	dl	r2,3,r14
	fm	r2,0,r6 ; P_O_A 
	far	r0,r2
	lr	r7,r10
	sll	r7,1
	lim	r8,spectrum,r7  ; LR,inc/dec peephole
	dl	r2,0,r8 ; P_O_A
	fsr	r2,r4
	dst	r2,0,r9 ; P_O_A 
	aim	r7,32+spectrum
	dl	r2,0,r7 ; P_O_A
	fsr	r2,r0
	dst	r2,0,r6 ; P_O_A 
	fa	r4,0,r8 ; P_O_A 
	dst	r4,0,r8 ; P_O_A 
	fa	r0,0,r7 ; P_O_A 
	dst	r0,0,r7 ; P_O_A 
	a	r10,1,r14
	cisp	r10,16
	blt	L31
L26
	aisp	r13,1
	c	r13,2,r14
	blt	L27
L25
	l	r11,1,r14
	st	r11,2,r14
	cisp	r11,16
	blt	L23
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,9 ; free up local-var space
	urs	r15
	global  pr_fp_num	; import
	konst
KLC2
	dataf	-1.000000
	konst
KLC3
	dataf	1.000000

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	xorr	r3,r3
	lim	r0,sample  ; 'i' constraint
L39
	lr	r2,r3
	sll	r2,1
	ar	r2,r0
	dl	r4,LC2
	dst	r4,0,r2 ; P_O_A 
	aisp	r3,1
	cisp	r3,7
	ble	L39
	lisp	r3,8
	lim	r0,sample  ; 'i' constraint
L44
	lr	r2,r3
	sll	r2,1
	ar	r2,r0
	dl	r4,LC3
	dst	r4,0,r2 ; P_O_A 
	aisp	r3,1
	cisp	r3,15
	ble	L44
	sjs	r15,init_fft   ; no return value
	sjs	r15,fft   ; no return value
	sjs	r15,compute_output   ; no return value
	xorr	r3,r3
L49
	lr	r2,r3
	sll	r2,1
	xorr	r2,r2   ;extendhftqf2
	dl	r0,64+spectrum,r2 ;P_O_A reg + const expr
	st	r3,1,r14
	sjs	r15,pr_fp_num   ; return value in R0
	lim	r0,newline
	sjs	r15,putstr
	l	r3,1,r14
	aisp	r3,1
	cisp	r3,8
	ble	L49
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r15,1 ; free up local-var space
	urs	r15
	Static
	common  sample,32
	common  spectrum,96

	static
init_srel
LC0 	block 2
LC1 	block 2
LC2 	block 2
LC3 	block 2

	init
	lim	r0,init_srel
	lim	r1,8
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
