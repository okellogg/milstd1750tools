	name sfp
	nolist
	include "ms1750.inc"
	list
	global	__main
; gcc2_compiled:
	normal   ; text_section
	global	ha_prerr	; export
ha_prerr
; regs used in this function: 0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
L1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	global	ha_gethostbyname	; export
ha_gethostbyname
; regs used in this function:  0 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
L2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	global	ha_strout	; export
ha_strout
; regs used in this function:  0 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
L3
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	global	init_scheduler	; export
init_scheduler
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L4
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	fp_initialize	; export
fp_initialize
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L5
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	ha_initialize	; export
ha_initialize
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L6
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	ha_strcmp	; export
ha_strcmp
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	xorr	r0,r0
	jc	uc,L7
L7
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	global	nothing	; import
	global	tolower	; im
port
	global	ha_strtolower	; export
ha_strtolower
; regs used in this function:  0 1 2 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
L9
	l	r1,1,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L11
	jc	uc,L10
L11
	sjs	r15,nothing   ; return value in R0
	l	r1,1,r14
	l	r0,0,r1 ; P_O_A
	sjs	r15,tolower   ; return value in R0
	lr	r1,r0
	l	r2,1,r14
	st	r1,0,r2 ; P_O_A
	incm	1,1,r14
	jc	uc,L9
L10
L8
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	global	tolower	; export
tolower
; regs used in this function:  0 1 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r1,1,r14
	andm	r1,96
	cim	r1,96
	jc	nz,L13
	l	r1,1,r14
	andm	r1,-33
	lr	r0,r1
	jc	uc,L12
	jc	uc,L14
L13
	l	r0,1,r14
	jc	uc,L12
L14
L12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	global	start_threads	; export
start_threads
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L15
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst    ; data_section
	Konst
Kconnected	data	0
	global	nothing	; import
	global	tp_read	; import
	global	debuglog	; import
	konst
KLC0	data	'%','d',' ','<','-',' ','t','p','_','r','e','a','d',' ','%'
	data	'c',0
	global	ha_fflush	; import
	global	_iob	; import
	global	lostpeer	; import
	konst
KLC1	data	'4','2','1',' ','S','e','r','v','i','c','e',' ','n','o','t'
	data	' ','a','v','a','i','l','a','b','l','e',',',' ','r','e','m'
	data	'o','t','e',' ','s','e','r','v','e','r',' ','h','a','s',' '
	data	'c','l','o','s','e','d',' ','c','o','n','n','e','c','t','i'
	data	'o','n',10,0
	global	ha_fflush	; import
	global	ha_isdigit	; import
	global	ha_putchar	; import
	global	ha_fflush	; import
	global	lostpeer	; import
	normal   ; text_section
	global	getreply	; export
getreply
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,9  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	stc	0,4,r14   
	stc	0,5,r14   
L17
	sjs	r15,nothing   ; return value in R0
	stc	0,code   
	stc	0,7,r14   
	stc	0,8,r14   
	lim	r4,reply_string  ; 'i' constraint
	st	r4,9,r14
	stc	0,3,r14   
L20
	l	r1,3,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L22
	jc	uc,L21
L22
	lr	r1,r14
	aisp	r1,2
	lisp	r2,1
	l	r0,sctrl
	sjs	r15,tp_read   ; return value in R0
	lr	r1,r0
	st	r1,6,r14
	l	r2,2,r14
	l	r1,6,r14
	lim	r0,LC0  ; 'i' constraint
	sjs	r15,debuglog   ; return value in R0
	l	r1,2,r14
	lr	r2,r1
	xorm	r2,10
	abs	r1,r2
	lr	r5,r1
	sisp	r5,1
	st	r5,3,r14
	l	r1,3,r14
	lr	r2,r1
	srl	r2,15
	st	r2,3,r14
	incm	1,8,r14
	l	r1,6,r14
	lr	r1,r1   ; from tstqi
	jc	nz,L23
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L24
	lim	r4,221  ; 'M' constraint
	st	r4,code
	lim	r0,6+_iob  ; 'i' constraint
	sjs	r15,ha_fflush   ; return value in R0
	xorr	r0,r0
	jc	uc,L16
L24
	sjs	r15,lostpeer   ; return value in R0
	lim	r0,LC1  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
	lim	r0,6+_iob  ; 'i' constraint
	sjs	r15,ha_fflush   ; return value in R0
	lim	r5,421  ; 'M' constraint
	st	r5,code
	lisp	r0,4
	jc	uc,L16
L23
	l	r4,8,r14
	cisp	r4,3
	jc	gt,L25
	l	r0,2,r14
	sjs	r15,ha_isdigit   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ez,L25
	l	r1,code
	lr	r3,r1
	lr	r2,r3
	sll	r2,2
	ar	r2,r1
	lr	r1,r2
	sll	r1,1
	lr	r2,r1
	sim	r2,48
	lr	r5,r2
	a	r5,2,r14
	st	r5,code
L25
	l	r4,8,r14
	cisp	r4,4
	jc	nz,L26
	l	r1,2,r14
	cim	r1,45
	jc	nz,L26
	l	r1,5,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L27
	stc	0,code   
L27	incm	1,5,r14
L26
	l	r5,7,r14
	lr	r5,r5   ; from tstqi
	jc	nz,L28
	l	r4,2,r14
	st	r4,7,r14
L28
	l	r5,9,r14
	cim	r5,255+reply_string
	jc	ge,L29 ; Warning: this should be an *unsigned* test!
	l	r5,2,r14
	l	r4,9,r14
	st	r5,0,r4 ; P_O_A
	incm	1,9,r14
L29
	jc	uc,L20
L21
	l	r0,2,r14
	sjs	r15,ha_putchar   ; return value in R0
	lim	r0,6+_iob  ; 'i' constraint
	sjs	r15,ha_fflush   ; return value in R0
	l	r1,5,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L30
	l	r1,code
	c	r1,4,r14 
	jc	ez,L30
	l	r1,4,r14
	lr	r1,r1   ; from tstqi
	jc	nz,L31
	l	r4,code
	st	r4,4,r14
L31
	jc	uc,L19
L30
	l	r5,9,r14
	stc	0,0,r5 ; P_O_A   
	l	r1,code
	cim	r1,421
	jc	ez,L33
	l	r1,4,r14
	cim	r1,421
	jc	ez,L33
	jc	uc,L32
L33
	sjs	r15,lostpeer   ; return value in R0
L32
	l	r1,7,r14
	sim	r1,48
	lr	r0,r1
	jc	uc,L16
L19
	jc	uc,L17
L18
L16
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,9 ; free up local-var space
	urs	r15
	Static
hostnamebuf.24 	block   80	; local common
	global	ha_bzero	; import
	global	ha_inet_addr	; import
	global	ha_strncpy	; import
	konst
KLC2	data	's','f','p','.','c',0
	global	ha_strncpy	; import
	global	tp_socket	; import
	global	ha_perror	; import
	konst
KLC3	data	'f','t','p',':',' ','s','o','c','k','e','t',0
	global	tp_connect	; import
	konst
KLC4	data	'C','o','u','l','d',' ','n','o','t',' ','c','o','n','n','e'
	data	'c','t',' ','t','o',' ',0
	konst
KLC5	data	'.',10,0
	global	nothing	; import
	konst
KLC6	data	'C','o','n','n','e','c','t','e','d',' ','t','o',' ',0
	global	tp_close	; import
	konst
KLC7	datal	-1
	normal   ; text_section
	global	hookup	; export
hookup
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,4  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	stc	0,4,r14   
	lisp	r1,14
	lim	r0,hisctladdr  ; 'i' constraint
	sjs	r15,ha_bzero   ; return value in R0
	l	r0,1,r14
	sjs	r15,ha_inet_addr   ; return value in R0
	lr	r1,r0
	lr	r2,r1 ;extendqihi2
	dsra	r2,16
	dst	r2,2+hisctladdr ;P_O_A const 
	dl	r4,2+hisctladdr ;P_O_A const
	dc	r4,LC7 
	jc	ez,L35
	stc	2,hisctladdr   
	lim	r2,80  ; 'M' constraint
	l	r1,1,r14
	lim	r0,hostnamebuf.24  ; 'i' constraint
	sjs	r15,ha_strncpy   ; return value in R0
	jc	uc,L36
L35
	l	r0,1,r14
	sjs	r15,ha_gethostbyname   ; return value in R0
	st	r0,4,r14
	l	r5,4,r14
	lr	r5,r5   ; from tstqi
	jc	nz,L37
	lim	r1,144  ; 'M' constraint
	lim	r0,LC2  ; 'i' constraint
	sjs	r15,ha_prerr   ; no return value
	lisn	r4,1
	st	r4,code
	xorr	r0,r0
	jc	uc,L34
L37
	l	r5,4,r14
	l	r5,2,r5
	st	r5,hisctladdr
	l	r4,4,r14
	l	r1,4,r4
	l	r5,4,r14
	l	r2,3,r5
	l	r1,0,r1 ; P_O_A
	lim	r0,2+hisctladdr  ; 'i' constraint
	sjs	r15,ha_strncpy   ; return value in R0
	lim	r2,80  ; 'M' constraint
	l	r4,4,r14
	l	r1,0,r4 ; P_O_A
	lim	r0,hostnamebuf.24  ; 'i' constraint
	sjs	r15,ha_strncpy   ; return value in R0
L36
	lim	r5,hostnamebuf.24  ; 'i' constraint
	st	r5,hostname
	xorr	r2,r2
	lisp	r1,1
	l	r0,hisctladdr
	sjs	r15,tp_socket   ; return value in R0
	lr	r1,r0
	st	r1,sctrl
	l	r1,sctrl
	lr	r1,r1   ; from tstqi
	jc	ge,L38
	lim	r0,LC3  ; 'i' constraint
	sjs	r15,ha_perror   ; return value in R0
	lisn	r4,1
	st	r4,code
	xorr	r0,r0
	jc	uc,L34
L38
	l	r5,2,r14
	st	r5,1+hisctladdr ;P_O_A const
	lisp	r2,14
	lim	r1,hisctladdr  ; 'i' constraint
	l	r0,sctrl
	sjs	r15,tp_connect   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ge,L39
	lim	r0,LC4  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
	l	r0,hostname
	sjs	r15,ha_strout   ; no return value
	lim	r0,LC5  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
	jc	uc,L40
L39
	sjs	r15,nothing   ; return value in R0
	lim	r0,LC6  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
	sjs	r15,nothing   ; return value in R0
	l	r0,hostname
	sjs	r15,ha_strout   ; no return value
	sjs	r15,nothing   ; return value in R0
	lim	r0,LC5  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
	sjs	r15,nothing   ; return value in R0
	xorr	r0,r0
	sjs	r15,getreply   ; return value in R0
	lr	r1,r0
	cisp	r1,2
	jc	le,L41
	lisn	r4,1
	st	r4,code
	jc	uc,L40
L41
	l	r0,hostname
	jc	uc,L34
L40
	l	r0,sctrl
	sjs	r15,tp_close   ; return value in R0	lisn	r5,1
	st	r5,sctrl
	xorr	r0,r0
	jc	uc,L34
L34
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,4 ; free up local-var space
	urs	r15
	global	cmdopen	; export
cmdopen
; regs used in this function:  0 1 2 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	l	r2,2,r14
	aisp	r2,1
	lim	r1,2121  ; 'M' constraint
	l	r0,0,r2 ; P_O_A
	sjs	r15,hookup   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ez,L43
	stc	1,connected   
L43
L42
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	global	cmdclose	; export
cmdclose
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	xorr	r0,r0
	jc	uc,L44
L44
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	global	cmdquit	; export
cmdquit
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	xorr	r0,r0
	jc	uc,L45
L45
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	global	cmdget	; export
cmdget
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	xorr	r0,r0
	jc	uc,L46
L46
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	global	cmdput	; export
cmdput
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	xorr	r0,r0
	jc	uc,L47
L47
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	konst    ; data_section
Kcmdtab	data	LC8;in ASM_OUTPUT_CHAR without label_pending
	data	cmdopen;in ASM_OUTPUT_CHAR without label_pending
	data	LC9;in ASM_OUTPUT_CHAR without label_pending
	data	cmdclose;in ASM_OUTPUT_CHAR without label_pending
	data	LC10;in ASM_OUTPUT_CHAR without label_pending
	data	cmdquit;in ASM_OUTPUT_CHAR without label_pending
	data	LC11;in ASM_OUTPUT_CHAR without label_pending
	data	cmdget;in ASM_OUTPUT_CHAR without label_pending
	data	LC12;in ASM_OUTPUT_CHAR without label_pending
	data	cmdput;in ASM_OUTPUT_CHAR without label_pending
	data	LC13
	global	tolowerx	; import
;in ASM_OUTPUT_CHAR without label_pending
	data	tolowerx;in ASM_OUTPUT_CHAR without label_pending
	data	0; in ASM_OUTPUT_SKIP: size=1
	konst
KLC13	data	't','o','l','o','w','e','r',0
	konst
KLC12	data	'p','u','t',0
	konst
KLC11	data	'g','e','t',0
	konst
KLC10	data	'q','u','i','t',0
	konst
KLC9	data	'c','l','o','s','e',0
	konst
KLC8	data	'o','p','e','n',0
	normal   ; text_section
	global	tolowerx	; export
tolowerx
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	xorr	r0,r0
	jc	uc,L48
L48
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	global	tcp	; export
tcp
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L49
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	ha_getc	; export
ha_getc
; regs used in this function:  0 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	xorr	r0,r0
	jc	uc,L50
L50
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	konst
KLC14	data	's','f','p','>',' ',0
	global	nothing	; import
	normal   ; text_section
	global	getline	; export
getline
; regs used in this function:  0 1 2 3 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	stc	0,3,r14   
	decm	1,2,r14
	lim	r0,LC14  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
L52
	jc	uc,L54
	jc	uc,L53
L54
	sjs	r15,nothing   ; return value in R0
	l	r1,1,r14
	a	r1,3,r14
	lr	r0,r1
	sjs	r15,ha_getc   ; return value in R0
	lr	r1,r0
	l	r3,3,r14
	ar	r3,r1
	st	r3,3,r14
	l	r1,1,r14
	a	r1,3,r14
	stc	0,0,r1 ; P_O_A   
	l	r1,3,r14
	c	r1,2,r14 
	jc	nz,L55
	decm	1,3,r14
L55
	l	r1,3,r14
	cisp	r1,1
	jc	nz,L56
	l	r1,1,r14
	l	r2,0,r1 ; P_O_A
	cisp	r2,10
	jc	nz,L56
	stc	0,3,r14   
	lim	r0,LC14  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
	jc	uc,L57
L56
	l	r1,3,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L58
	l	r1,3,r14
	a	r1,1,r14
	lr	r2,r1
	sisp	r2,1
	l	r1,0,r2 ; P_O_A
	cisp	r1,10
	jc	nz,L58
	l	r1,3,r14
	a	r1,1,r14
	lr	r2,r1
	sisp	r2,1
	stc	0,0,r2 ; P_O_A   
	jc	uc,L51
L58
L57
	jc	uc,L52
L53
L51
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	global	nothing	; import
	global	makemargv	; export
makemargv
; regs used in this function:  0 1 2 3 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r3,1,r14
	st	r3,2,r14
	stc	1,3,r14   
	stc	0,margc   
L60
	l	r1,2,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L62
	jc	uc,L61
L62
	sjs	r15,nothing   ; return value in R0
	l	r1,3,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L63
	l	r1,2,r14
	l	r2,0,r1 ; P_O_A
	cim	r2,32
	jc	ez,L64
	l	r1,margc
	aim	r1,margv
	l	r3,2,r14
	st	r3,0,r1 ; P_O_A
	incm	1,margc
	l	r1,margc
	cisp	r1,10
	jc	nz,L65
	jc	uc,L61
L65
	stc	0,3,r14
L64
	jc	uc,L66
L63
	l	r1,2,r14
	l	r2,0,r1 ; P_O_A
	cim	r2,32
	jc	nz,L67
	l	r1,2,r14
	stc	0,0,r1 ; P_O_A   
	stc	1,3,r14   
L67
L66	incm	1,2,r14
	jc	uc,L60
L61
	l	r0,margc
	jc	uc,L59
L59
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	global	nothing	; import
	global	lookup_execute	; export
lookup_execute
; regs used in this function:  0 1 2 3 14
	sisp	r15,4  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	stc	0,4,r14   
	l	r1,2,r14
	l	r0,0,r1 ; P_O_A
	sjs	r15,ha_strtolower   ; no return value
	lim	r3,cmdtab  ; 'i' constraint
	st	r3,3,r14
L69
	l	r1,3,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L71
	jc	uc,L70
L71
	sjs	r15,nothing   ; return value in R0
	l	r1,2,r14
	l	r2,3,r14
	l	r1,0,r1 ; P_O_A
	l	r0,0,r2 ; P_O_A
	sjs	r15,ha_strcmp   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	nz,L72
	stc	1,4,r14   
	l	r1,3,r14
	l	r2,1,r1
	l	r1,2,r14
	l	r0,1,r14
	sjs	r15,0,r2 ; P_O_A   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ge,L73
	lisn	r3,1
	st	r3,4,r14
L73
	jc	uc,L70
L72	incm	2,3,r14
	jc	uc,L69
L70
	l	r0,4,r14
	jc	uc,L68
L68
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,4 ; free up local-var space
	urs	r15
	global	nothing	; import
	konst
KLC15	data	'E','R','R','O','R',10,0
	normal   ; text_section
	global	client_app	; export
client_app
; regs used in this function:  0 1 14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	nop	
L75
	jc	uc,L77
	jc	uc,L76
L77
	sjs	r15,nothing   ; return value in R0
	lim	r1,80  ; 'M' constraint
	lim	r0,cmdline  ; 'i' constraint
	sjs	r15,getline   ; no return value
	lim	r0,cmdline  ; 'i' constraint
	sjs	r15,makemargv   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ez,L78
	lim	r1,margv  ; 'i' constraint
	l	r0,margc
	sjs	r15,lookup_execute   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ge,L79
	lim	r0,LC15  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
L79
L78
	jc	uc,L75
L76
L74
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	konst
KLC16	data	'C','o','m','m','a','n','d',' ','l','i','n','e',' ','a','r'
	data	'g','u','m','e','n','t','s',' ','a','r','e',' ','i','g','n'
	data	'o','r','e','d','.',' ',' ','S','o','r','r','y','.',10,0
	global	scheduler	; import
	global	create_thread	; import
	global	sockets	
; import
	normal   ; text_section
	global	main	; export
main
; regs used in this function:  0 1 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	l	r1,1,r14
	cisp	r1,1
	jc	ez,L81
	lim	r0,LC16  ; 'i' constraint
	sjs	r15,ha_strout   ; no return value
L81
	sjs	r15,init_scheduler   ; no return value
	lim	r0,tcp  ; 'i' constraint
	sjs	r15,create_thread   ; return value in R0
	lr	r1,r0
	st	r1,6+scheduler ;P_O_A const
	lim	r0,sockets  ; 'i' constraint
	sjs	r15,create_thread   ; return value in R0
	lr	r1,r0
	st	r1,7+scheduler ;P_O_A const
	lim	r0,client_app  ; 'i' constraint
	sjs	r15,create_thread   ; return value in R0
	lr	r1,r0
	st	r1,8+scheduler ;P_O_A const
	sjs	r15,fp_initialize   ; no return value
	sjs	r15,ha_initialize   ; no return value
	sjs	r15,start_threads   ; no return value
L80
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	Static
	common  sys_memory,31
cmdline 	block   80	; local common
reply_string 	block   256	; local common
margc 	block   1	; local common
margv 	block   10	; local common
hisctladdr 	block   14	; local common
hostname 	block   1	; local common
sctrl 	block   1	; local common
code 	block   1	; local common
	static
init_srel
connected 	block 1
LC0 	block 17
LC1 	block 64
LC2 	block 6
LC3 	block 12
LC4 	block 22
LC5 	block 3
LC6 	block 14
LC7 	block 2
cmdtab 	block 1
LC13 	block 8
LC12 	block 4
LC11 	block 4
LC10 	block 5
LC9 	block 6
LC8 	block 5
LC14 	block 6
LC15 	block 7
LC16 	block 45
	init
	lim	r0,init_srel
	lim	r1,232
	lim	r2,Kconnected
	mov	r0,r2
	normal
	end

