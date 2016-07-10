	name testem
	nolist
	include "ms1750.inc"
	list

	global	__main

; gcc2_compiled:
	global  puts	; import
	konst
KLC0
	data	13	; (ascii)
	data	10	; (ascii)
	data	40	; '('
	data	101	; 'e'
	data	120	; 'x'
	data	112	; 'p'
	data	101	; 'e'
	data	99	; 'c'
	data	116	; 't'
	data	101	; 'e'
	data	100	; 'd'
	data	58	; ':'
	data	32	; ' '
	data	49	; '1'
	data	48	; '0'
	data	46	; '.'
	data	48	; '0'
	data	41	; ')'
	data	32	; ' '
	data	32	; ' '
	data	97	; 'a'
	data	99	; 'c'
	data	116	; 't'
	data	117	; 'u'
	data	97	; 'a'
	data	108	; 'l'
	data	58	; ':'
	data	32	; ' '
	data	0	; (ascii)
	global  pr_efp_num	; import
	konst
KLC1
	data	13	; (ascii)
	data	10	; (ascii)
	data	40	; '('
	data	101	; 'e'
	data	120	; 'x'
	data	112	; 'p'
	data	101	; 'e'
	data	99	; 'c'
	data	116	; 't'
	data	101	; 'e'
	data	100	; 'd'
	data	58	; ':'
	data	32	; ' '
	data	50	; '2'
	data	46	; '.'
	data	53	; '5'
	data	41	; ')'
	data	32	; ' '
	data	32	; ' '
	data	32	; ' '
	data	97	; 'a'
	data	99	; 'c'
	data	116	; 't'
	data	117	; 'u'
	data	97	; 'a'
	data	108	; 'l'
	data	58	; ':'
	data	32	; ' '
	data	0	; (ascii)
	konst
KLC2
	data	13	; (ascii)
	data	10	; (ascii)
	data	40	; '('
	data	101	; 'e'
	data	120	; 'x'
	data	112	; 'p'
	data	101	; 'e'
	data	99	; 'c'
	data	116	; 't'
	data	101	; 'e'
	data	100	; 'd'
	data	58	; ':'
	data	32	; ' '
	data	45	; '-'
	data	53	; '5'
	data	46	; '.'
	data	48	; '0'
	data	41	; ')'
	data	32	; ' '
	data	32	; ' '
	data	97	; 'a'
	data	99	; 'c'
	data	116	; 't'
	data	117	; 'u'
	data	97	; 'a'
	data	108	; 'l'
	data	58	; ':'
	data	32	; ' '
	data	0	; (ascii)
	konst
KLC3
	data	13	; (ascii)
	data	10	; (ascii)
	data	40	; '('
	data	101	; 'e'
	data	120	; 'x'
	data	112	; 'p'
	data	101	; 'e'
	data	99	; 'c'
	data	116	; 't'
	data	101	; 'e'
	data	100	; 'd'
	data	58	; ':'
	data	32	; ' '
	data	50	; '2'
	data	46	; '.'
	data	48	; '0'
	data	41	; ')'
	data	32	; ' '
	data	32	; ' '
	data	32	; ' '
	data	97	; 'a'
	data	99	; 'c'
	data	116	; 't'
	data	117	; 'u'
	data	97	; 'a'
	data	108	; 'l'
	data	58	; ':'
	data	32	; ' '
	data	0	; (ascii)
	konst
KLC4
	data	13	; (ascii)
	data	10	; (ascii)
	data	40	; '('
	data	101	; 'e'
	data	120	; 'x'
	data	112	; 'p'
	data	101	; 'e'
	data	99	; 'c'
	data	116	; 't'
	data	101	; 'e'
	data	100	; 'd'
	data	58	; ':'
	data	32	; ' '
	data	48	; '0'
	data	46	; '.'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	56	; '8'
	data	57	; '9'
	data	41	; ')'
	data	32	; ' '
	data	32	; ' '
	data	32	; ' '
	data	97	; 'a'
	data	99	; 'c'
	data	116	; 't'
	data	117	; 'u'
	data	97	; 'a'
	data	108	; 'l'
	data	58	; ':'
	data	32	; ' '
	data	0	; (ascii)
	konst
KLC5
	data	13	; (ascii)
	data	10	; (ascii)
	data	0	; (ascii)
	konst
KLC6
	dataef	10.000000
	konst
KLC7
	dataef	2.500000
	konst
KLC8
	dataef	-5.000000
	konst
KLC9
	dataef	-1.000000
	konst
KLC10
	dataef	-2.111111

	normal   ; text_section

	global  main	; export
main
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	efl	r3,LC6
	efst	r3,1,r14 
	lim	r0,LC0  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,1,r14
	sjs	r15,pr_efp_num   ; no return value
	efl	r3,LC7
	efst	r3,1,r14 
	lim	r0,LC1  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,1,r14
	sjs	r15,pr_efp_num   ; no return value
	efl	r3,LC8
	efst	r3,1,r14 
	lim	r0,LC2  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,1,r14
	sjs	r15,pr_efp_num   ; no return value
	efl	r3,LC9
	efst	r3,1,r14 
	lim	r0,LC3  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,1,r14
	sjs	r15,pr_efp_num   ; no return value
	efl	r3,LC10
	efst	r3,1,r14 
	lim	r0,LC4  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	efl	r0,1,r14
	sjs	r15,pr_efp_num   ; no return value
	lim	r0,LC5  ; 'i' constraint
	sjs	r15,puts   ; return value in R0
	aisp	r14,3 ; free up local-var space
	lr	r15,r14 ; set stack to return addr
	popm	r14,r14 ; restore prev. frame ptr
	urs	r15

	static
init_srel
LC0 	block 29
LC1 	block 29
LC2 	block 29
LC3 	block 29
LC4 	block 40
LC5 	block 3
LC6 	block 3
LC7 	block 3
LC8 	block 3
LC9 	block 3
LC10 	block 3

	init
	lim	r0,init_srel
	lim	r1,174
	lim	r2,KLC0
	mov	r0,r2

	normal

	end
