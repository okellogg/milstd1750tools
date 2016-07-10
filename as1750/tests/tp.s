	name tp
	nolist
	include "ms1750.inc"
	list
	global	__main; gcc2_compiled:
	konst    ; data_section
	Konst
Krcsid	data	'$','I','d',':',' ','t','p','.','c',',','v',' ','1','.','6'
	data	' ','1','9','9','5','/','0','7','/','1','8',' ','1','8',':'
	data	'0','6',':','3','5',' ','s','t','e','v','e','n',' ','E','x'
	data	'p',' ','s','t','e','v','e','n',' ','$',0
	konst    ; data_section
Kconfig_span_name
	data	0
	konst    ; data_section
Ktcp_mssdflt	data	512
	konst    ; data_section
Ktcp_mssmin	data	32
	konst    ; data_section
Kconfig_MTU	data	1024
	normal   ; text_section
	global	nothing	; export
nothing
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L1
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Init	; import
	global	tp_initialize	; export
tp_initialize
; regs used in this function:  0 1 2 3 14
	sisp	r15,1  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	nop	
	stc	0,1,r14   
L3
	l	r1,1,r14
	cisp	r1,3
	jc	le,L6
	jc	uc,L4
L6
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	stc	0,bs,r1 ;P_O_A sym + reg   
L5	incm	1,1,r14
	jc	uc,L3
L4
	stc	0,tp_dontblock   
	lim	r0,180  ; 'M' constraint
	st	r0,inactive_seconds
	sjs	r15,tcp_Init   ; no return value
	stc	1,been_initialized   
L2
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,1 ; free up local-var space
	urs	r15
	Static
i.6 	block   1	; local common
	global	gettimeofday	; import
	global	debuglog	; import
	konst
KLC0	data	'%','d',' ','c','l','o','s','e','d','.',10,0
	konst
KLC1	data	's','o','c','k','e','t',' ','%','d',' ','t','i','m','e','d'
	data	' ','o','u','t',10,0
	global	tcp_Abort	; import
	normal   ; text_section
	global	sockets	; export
sockets
; regs used in this function:  0 1 2 3 4 5 6 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	stc	0,i.6 
	nop	
L8
	jc	uc,L10
	jc	uc,L9
L10	xorr	r1,r1
	lim	r0,time  ; 'i' constraint
	sjs	r15,gettimeofday   ; return value in R0
	l	r1,i.6
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	l	r2,bs,r1 ;P_O_A sym + reg
	lr	r2,r2   ; from tstqi
	jc	ez,L11
	l	r1,i.6
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r5,ttcp_socks  ; 'i' constraint
	ar	r5,r1
	st	r5,1,r14
	l	r1,1,r14
	l	r2,3,r1
	cisp	r2,9
	jc	nz,L12
	l	r1,i.6
	lim	r0,LC0  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r1,i.6
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	stc	0,bs,r1 ;P_O_A sym + reg   
	jc	uc,L13
L12
	l	r1,i.6
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r6,bs  ; 'i' constraint
	ar	r6,r1
	st	r6,2,r14
	l	r1,2,r14
	dl	r3,time
	dlr	r5,r3
	ds	r5,17,r1 
	dlr	r1,r5
	l	r3,2,r14
	dc	r1,19,r3 
	jc	le,L14 ; Warning: this should be an *unsigned* test!
	l	r1,i.6
	lim	r0,LC1  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r1,i.6
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lr	r2,r1
	aim	r2,ttcp_socks
	lr	r0,r2
	sjs	r15,tcp_Abort   ; no return value
	l	r1,2,r14
	stc	3,21,r1   
L14
L13
L11	incm	1,i.6
	l	r1,i.6
	cisp	r1,4
	jc	nz,L15
	stc	0,i.6   
L15
	sjs	r15,nothing   ; no return value
	jc	uc,L8
L9
L7
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	konst
KLC2	data	'%','d',' ','<','-',' ','t','p','_','s','o','c','k','e','t'
	data	10,0
	global	bzero	; import
	normal   ; text_section
	global	tp_socket	; export
tp_socket
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,7  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	st	r2,3,r14
	lisn	r4,1
	st	r4,5,r14
	stc	0,4,r14   
L17
	l	r1,4,r14
	cisp	r1,3
	jc	le,L20
	jc	uc,L18
L20
	sjs	r15,nothing   ; no return value
	l	r1,4,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r5,bs  ; 'i' constraint
	ar	r5,r1
	st	r5,7,r14
	l	r1,7,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L21
	l	r1,4,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,6,r14
	l	r5,4,r14
	st	r5,5,r14
	l	r1,5,r14
	lim	r0,LC2  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	lim	r1,22  ; 'M' constraint
	l	r0,7,r14
	sjs	r15,bzero   ; return value in R0
	lim	r1,122  ; 'M' constraint
	l	r0,6,r14
	sjs	r15,bzero   ; return value in R0
	l	r1,7,r14
	stc	1,0,r1 ; P_O_A   
	l	r1,7,r14
	dl	r4,time
	dst	r4,17,r1 
	l	r1,7,r14
	lim	r4,32767 ;movhi cst->reg
	lim	r5,-1  ; range correction (val>0x7FFF) applied
	dst	r5,19,r1 
	jc	uc,L18
L21
L19	incm	1,4,r14
	jc	uc,L17
L18
	l	r0,5,r14
	jc	uc,L16
L16
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,7 ; free up local-var space
	urs	r15
	konst
KLC3	data	't','p','_','c','l','o','s','e','(','%','d',')',10,0
	konst
KLC4	data	't','p','_','c','l','o','s','e',' ','%','d',10,0
	global	tcp_Close	; import
	normal   ; text_section
	global	tp_close	; export
tp_close
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,2,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,3,r14
	sjs	r15,nothing   ; no return value
	l	r1,1,r14
	lim	r0,LC3  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L23
	jc	uc,L22
L23
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L25
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L25
	jc	uc,L24
L25
	jc	uc,L22
L24
	l	r1,1,r14
	lim	r0,LC4  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r0,2,r14
	sjs	r15,tcp_Close   ; no return value
	stc	0,errno   
L22
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	konst
KLC5	data	't','p','_','a','b','o','r','t','(','%','d',')',10,0
	konst
KLC6	data	't','p','_','a','b','o','r','t',' ','%','d',10,0
	normal   ; text_section
	global	tp_abort	; export
tp_abort
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,2,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,3,r14
	l	r1,1,r14
	lim	r0,LC5  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L27
	jc	uc,L26
L27
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L29
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L29
	jc	uc,L28
L29
	jc	uc,L26
L28
	l	r1,3,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L30
	jc	uc,L26
L30
	l	r1,1,r14
	lim	r0,LC6  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r0,2,r14
	sjs	r15,tcp_Abort   ; no return value
	stc	0,errno  
L26
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	konst
KLC7	data	't','p','_','b','i','n','d','(','%','d',')',10,0
	global	bcopy	; import
	normal   ; text_section
	global	tp_bind	; export
tp_bind
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,5  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	st	r2,3,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,4,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,5,r14
	l	r1,1,r14
	lim	r0,LC7  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r1,2,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L32
	l	r1,3,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L32
	l	r1,5,r14
	aisp	r1,3
	l	r2,3,r14
	l	r0,2,r14
	sjs	r15,bcopy   ; return value in R0
L32	xorr	r0,r0
	jc	uc,L31
L31
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,5 ; free up local-var space
	urs	r15
	global	ha_inet_ntoa	; import
	konst
KLC8	data	't','p','_','l','i','s','t','e','n',' ','%','d',' ','%','s'
	data	',','%','d',10,0
	global	tcp_Listen	; import
	normal   ; text_section
	global	tp_listen	; export
tp_listen
; regs used in this function:  0 1 2 3 4 5 6 14
	sisp	r15,5  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r5,ttcp_socks  ; 'i' constraint
	ar	r5,r1
	st	r5,3,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r6,bs  ; 'i' constraint
	ar	r6,r1
	st	r6,4,r14
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L34
	lisn	r0,1
	jc	uc,L33
L34
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L36
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L36
	jc	uc,L35
L36	lisn	r0,1
	jc	uc,L33
L35
	l	r5,4,r14
	st	r5,5,r14
	l	r3,4,r14
	l	r0,5,r3
	l	r1,6,r3
	l	r2,7,r3
	l	r3,8,r3
	sjs	r15,ha_inet_ntoa   ; return value in R0
	lr	r1,r0
	l	r6,5,r14
	l	r3,4,r6
	lr	r2,r1
	l	r1,1,r14
	lim	r0,LC8  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r1,4,r14
	dl	r5,time
	dst	r5,17,r1 
	l	r1,4,r14
	lim	r5,32767 ;movhi cst->reg
	lim	r6,-1  ; range correction (val>0x7FFF) applied
	dst	r6,19,r1 
	l	r1,4,r14
	xorr	r3,r3 ;movhi cst->reg
	xorr	r4,r4
	xorr	r2,r2
	l	r1,4,r1
	l	r0,3,r14
	sjs	r15,tcp_Listen   ; no return value
	xorr	r0,r0
	jc	uc,L33
L33
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,5 ; free up local-var space
	urs	r15
	global	tp_status	; import
	konst
KLC9	data	't','p','_','a','c','c','e','p','t',' ','%','d',10,0
	global	tcp_connect	; import
	konst
KLC10	data	'%','d',' ','<','-',' ','t','p','_','a','c','c','e','p','t'
	data	10,0
	normal   ; text_section
	global	tp_accept	; export
tp_accept
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,5  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	st	r2,3,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,4,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r5,bs  ; 'i' constraint
	ar	r5,r1
	st	r5,5,r14
	stc	9,errno   
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L38
	lisn	r0,1
	jc	uc,L37
L38
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L40
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L40
	jc	uc,L39
L40	lisn	r0,1
	jc	uc,L37
L39
	l	r1,5,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L41
	l	r0,1,r14
	sjs	r15,tp_status   ; return value in R0
	lr	r1,r0
	lr	r0,r1
	jc	uc,L37
L41
	l	r1,1,r14
	lim	r0,LC9  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	stc	0,errno   
L42
	l	r0,4,r14
	sjs	r15,tcp_connect   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	nz,L45
	l	r1,5,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L44
	jc	uc,L45
L45
	jc	uc,L43
L44
	sjs	r15,nothing   ; no return value
	jc	uc,L42
L43
	l	r0,4,r14
	sjs	r15,tcp_connect   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ez,L46
	l	r1,5,r14
	dl	r4,time
	dst	r4,17,r1 
	l	r1,5,r14
	l	r2,inactive_seconds  ;extendqihi2
	dsra	r2,16
	dst	r2,19,r1 
	l	r1,1,r14
	lim	r0,LC10  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r0,1,r14
	jc	uc,L37
	jc	uc,L47
L46
	lim	r5,54  ; 'M' constraint
	st	r5,errno
	lisn	r0,1
	jc	uc,L37
L47
L37
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,5 ; free up local-var space
	urs	r15
	global	tp_status	; import
	global	ha_inet_ntoa	; import
	konst
KLC11	data	't','p','_','c','o','n','n','e','c','t',' ','%','d',' ','%'
	data	's',',','%','d',10,0
	global	tcp_Open	; import
	global	tp_write	; import
	konst
KLC12	data	13,0
	global	tp_flush	; import
	konst
KLC13	data	'%','d',' ','<','-',' ','t','p','_','c','o','n','n','e','c'
	data	't',10,0
	normal   ; text_section
	global	tp_connect	; export
tp_connect
; regs used in this function:  0 1 2 3 4 5 6 7 14
	sisp	r15,6  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	st	r2,3,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r6,ttcp_socks  ; 'i' constraint
	ar	r6,r1
	st	r6,4,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r7,bs  ; 'i' constraint
	ar	r7,r1
	st	r7,5,r14
	stc	9,errno   
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L49
	lisn	r0,1
	jc	uc,L48
L49
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L51
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L51
	jc	uc,L50
L51	lisn	r0,1
	jc	uc,L48
L50
	l	r1,5,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L52
	l	r0,1,r14
	sjs	r15,tp_status   ; return value in R0
	lr	r1,r0
	lr	r0,r1
	jc	uc,L48
L52
	l	r6,2,r14
	aisp	r6,1
	st	r6,6,r14
	l	r3,2,r14
	aisp	r3,3
	l	r0,0,r3 ; P_O_A
	l	r1,1,r3
	l	r2,2,r3
	l	r3,3,r3
	sjs	r15,ha_inet_ntoa   ; return value in R0
	lr	r1,r0
	l	r7,6,r14
	l	r3,0,r7 ; P_O_A
	lr	r2,r1
	l	r1,1,r14
	lim	r0,LC11  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	stc	0,errno   
	l	r1,5,r14
	dl	r6,time
	dst	r6,17,r1 
	l	r1,5,r14
	l	r2,inactive_seconds  ;extendqihi2
	dsra	r2,16
	dst	r2,19,r1 
	l	r1,2,r14
	l	r2,2,r14
	xorr	r5,r5
	l	r4,1,r1
	dl	r2,2,r2
	xorr	r1,r1
	l	r0,4,r14
	sjs	r15,tcp_Open   ; no return value
L53
	l	r0,4,r14
	sjs	r15,tcp_connect   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	nz,L56
	l	r1,5,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L55
	jc	uc,L56
L56
	jc	uc,L54
L55
	sjs	r15,nothing   ; no return value
	jc	uc,L53
L54
	l	r0,4,r14
	sjs	r15,tcp_connect   ; return value in R0
	lr	r1,r0
	lr	r1,r1   ; from tstqi
	jc	ez,L57
	lisp	r2,1
	lim	r1,LC12  ; 'i' constraint
	l	r0,1,r14
	sjs	r15,tp_write   ; return value in R0
	l	r0,1,r14
	sjs	r15,tp_flush   ; no return value
	l	r1,5,r14
	dl	r6,time
	dst	r6,17,r1 
	l	r1,5,r14
	l	r2,inactive_seconds  ;extendqihi2
	dsra	r2,16
	dst	r2,19,r1 
	sjs	r15,nothing   ; no return value
	l	r1,1,r14
	lim	r0,LC13  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r0,1,r14
	jc	uc,L48
L57
	l	r1,errno
	lr	r1,r1   ; from tstqi
	jc	nz,L58
	lim	r7,53  ; 'M' constraint
	st	r7,errno
L58	lisn	r0,1
	jc	uc,L48
L48
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,6 ; free up local-var space
	urs	r15
	global	tp_status	; import
	global	tcp_Write	; import
	konst
KLC14	data	'%','d',' ','<','-',' ','t','p','_','w','r','i','t','e','('
	data	'%','d',')',10,0
	normal   ; text_section
	global	tp_write	; export
tp_write
; regs used in this function:  0 1 2 3 4 5 14
	sisp	r15,7  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	st	r2,3,r14
	l	r4,3,r14
	st	r4,5,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r5,ttcp_socks  ; 'i' constraint
	ar	r5,r1
	st	r5,6,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,7,r14
	sjs	r15,nothing   ; no return value
	stc	9,errno   
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L60
	lisn	r0,1
	jc	uc,L59
L60
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L62
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L62
	jc	uc,L61
L62	lisn	r0,1
	jc	uc,L59
L61
	l	r1,7,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L63
	l	r0,1,r14
	sjs	r15,tp_status   ; return value in R0
	lisn	r0,1
	jc	uc,L59
L63
	stc	0,errno   
	l	r1,7,r14
	l	r2,2,r1
	lr	r2,r2   ; from tstqi
	jc	ez,L64
	lim	r5,55  ; 'M' constraint
	st	r5,errno
	lisn	r0,1
	jc	uc,L59
L64
	l	r1,7,r14
	l	r4,2,r14
	st	r4,1,r1
	l	r1,7,r14
	l	r5,3,r14
	st	r5,2,r1
L65
	l	r1,7,r14
	l	r2,2,r1
	lr	r2,r2   ; from tstqi
	jc	ez,L68
	l	r1,7,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L67
	jc	uc,L68
L68
	jc	uc,L66
L67
	l	r1,7,r14
	dl	r4,time
	dst	r4,17,r1 
	l	r1,7,r14
	l	r3,7,r14
	l	r2,2,r1
	l	r1,1,r3
	l	r0,6,r14
	sjs	r15,tcp_Write   ; return value in R0
	lr	r1,r0
	st	r1,4,r14
	sjs	r15,nothing   ; no return value
	l	r1,7,r14
	l	r2,7,r14
	l	r5,1,r2
	a	r5,4,r14
	st	r5,1,r1
	l	r1,7,r14
	l	r2,7,r14
	l	r4,2,r2
	s	r4,4,r14
	st	r4,2,r1
	sjs	r15,nothing   ; no return value
	jc	uc,L65
L66
	l	r1,7,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L69
	lim	r5,53  ; 'M' constraint
	st	r5,errno
	l	r2,1,r14
	lisn	r1,1
	lim	r0,LC14  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	lisn	r0,1
	jc	uc,L59
	jc	uc,L70
L69
	stc	0,errno   
	l	r2,1,r14
	l	r1,5,r14
	lim	r0,LC14  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r0,5,r14
	jc	uc,L59
L70
L59
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,7 ; free up local-var space
	urs	r15
	konst
KLC15	data	't','p','_','f','l','u','s','h','(','%','d',')',10,0
	global	tp_status	; import
	global	tcp_Flush	; import
	normal   ; text_section
	global	tp_flush	; export
tp_flush
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,2,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,3,r14
	sjs	r15,nothing   ; no return value
	l	r1,1,r14
	lim	r0,LC15  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	stc	9,errno   
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L72
	jc	uc,L71
L72
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L74
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L74
	jc	uc,L73
L74
	jc	uc,L71
L73
	l	r1,3,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L75
	l	r0,1,r14
	sjs	r15,tp_status   ; return value in R0
	jc	uc,L71
L75
	stc	0,errno   
	l	r0,2,r14
	sjs	r15,tcp_Flush   ; no return value
L71
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	konst
KLC16	data	't','p','_','r','e','a','d','(','%','d',',',' ','%','d',','
	data	' ','%','d',')',10,0
	global	tp_status	; import
	global	tcp_Read	; import
	konst
KLC17	data	'%','d',' ','<','-',' ','t','c','p','_','R','e','a','d',10
	data	0
	global	scheduler	; import
	konst
KLC18	data	'%','d',' ','<','-','t','p','_','r','e','a','d',' ','e','r'
	data	'r','n','o',':','%','d',10,0
	normal   ; text_section
	global	tp_read	; export
tp_read
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,7  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	st	r2,3,r14
	stc	1,5,r14   
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,6,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,7,r14
	l	r3,3,r14
	l	r2,2,r14
	l	r1,1,r14
	lim	r0,LC16  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	sjs	r15,nothing   ; no return value
	stc	9,errno   
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L77
	xorr	r0,r0
	jc	uc,L76
L77
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L79
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L79
	jc	uc,L78
L79	xorr	r0,r0
	jc	uc,L76
L78
	l	r1,7,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L80
	l	r0,1,r14
	sjs	r15,tp_status   ; return value in R0
	xorr	r0,r0
	jc	uc,L76
L80
	stc	0,errno   
L81
	jc	uc,L83
	jc	uc,L82
L83
	sjs	r15,nothing   ; no return value
	l	r2,3,r14
	l	r1,2,r14
	l	r0,6,r14
	sjs	r15,tcp_Read   ; return value in R0
	lr	r1,r0
	st	r1,4,r14
	l	r1,4,r14
	lr	r1,r1   ; from tstqi
	jc	ez,L84
	l	r1,4,r14

	lim	r0,LC17  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
L84
	l	r1,4,r14
	lr	r1,r1   ; from tstqi
	jc	nz,L85
	l	r1,7,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L86
	lim	r4,54  ; 'M' constraint
	st	r4,errno
	jc	uc,L82
L86
	l	r1,tp_dontblock
	lr	r1,r1   ; from tstqi
	jc	ez,L87
	stc	0,tp_dontblock   
	jc	uc,L82
L87
	jc	uc,L88
L85
	l	r1,4,r14
	lr	r1,r1   ; from tstqi
	jc	ge,L89
	l	r1,24+scheduler ;P_O_A const
	l	r4,1,r1
	st	r4,errno
	l	r1,errno
	cim	r1,35
	jc	nz,L90
	l	r1,tp_dontblock
	lr	r1,r1   ; from tstqi
	jc	ez,L91
	stc	0,4,r14   
	stc	0,errno   
	jc	uc,L82
	jc	uc,L92
L91
	jc	uc,L81
L92
L90
	jc	uc,L82
	jc	uc,L93
L89
	jc	uc,L82
L93
L88
	jc	uc,L81
L82
	l	r2,errno
	l	r1,4,r14
	lim	r0,LC18  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r0,4,r14
	jc	uc,L76
L76
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,7 ; free up local-var space
	urs	r15
	global	tp_status	; export
tp_status
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,2,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,3,r14
	l	r2,3,r14
	l	r1,21,r2
	cisp	r1,2
	jc	ez,L97
	cisp	r1,2
	jc	gt,L101
	cisp	r1,1
	jc	ez,L96
	jc	uc,L99
L101	cisp	r1,3
	jc	ez,L98
	jc	uc,L99
L96
	lim	r4,54  ; 'M' constraint
	st	r4,errno
	jc	uc,L95
L97
	lim	r4,53  ; 'M' constraint
	st	r4,errno
	jc	uc,L95
L98
	lim	r4,60  ; 'M' constraint
	st	r4,errno
	jc	uc,L95
L99
	stc	0,errno   
	jc	uc,L95
L95	lisn	r0,1
	jc	uc,L94
L94
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	konst
KLC19	data	't','p','_','s','h','u','t','d','o','w','n','a','l','l','('
	data	')',10,0
	normal   ; text_section
	global	tp_shutdownall	; export
tp_shutdownall
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,3  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	lim	r0,LC19  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	stc	0,3,r14   
L103
	l	r1,3,r14
	cisp	r1,3
	jc	le,L106
	jc	uc,L104
L106
	sjs	r15,nothing   ; no return value
	l	r1,3,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,1,r14
	l	r1,3,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,2,r14
	l	r1,2,r14
	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	ez,L107
	l	r0,1,r14
	sjs	r15,tcp_Abort   ; no return value
L107
L105	incm	1,3,r14
	jc	uc,L103
L104
L102
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,3 ; free up local-var space
	urs	r15
	konst
KLC20	data	't','p','_','s','e','t','s','o','c','k','t','i','m','e','o'
	data	'u','t','(','%','d',',',' ','%','d',')',10,0
	normal   ; text_section
	global	tp_setsocktimeout	; export
tp_setsocktimeout
; regs used in this function:  0 1 2 3 4 14
	sisp	r15,4  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	st	r1,2,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,4
	sr	r2,r1
	lr	r3,r2
	sll	r3,2
	ar	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,ttcp_socks  ; 'i' constraint
	ar	r4,r1
	st	r4,3,r14
	l	r1,1,r14
	lr	r3,r1
	lr	r2,r3
	sll	r2,1
	ar	r2,r1
	lr	r3,r2
	sll	r3,2
	sr	r3,r1
	lr	r1,r3
	sll	r1,1
	lim	r4,bs  ; 'i' constraint
	ar	r4,r1
	st	r4,4,r14
	l	r2,2,r14
	l	r1,1,r14
	lim	r0,LC20  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	sjs	r15,nothing   ; no return value
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L109
	jc	uc,L108
L109
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L111
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L111
	jc	uc,L110
L111
	jc	uc,L108
L110
	l	r1,4,r14

	l	r2,0,r1 ; P_O_A
	lr	r2,r2   ; from tstqi
	jc	nz,L112
	jc	uc,L108
L112
	l	r1,4,r14
	l	r2,2,r14  ;extendqihi2
	dsra	r2,16
	dst	r2,19,r1 
L108
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,4 ; free up local-var space
	urs	r15
	konst
KLC21	data	't','p','_','s','l','e','e','p','(','%','d',')',10,0
	normal   ; text_section
	global	tp_sleep	; export
tp_sleep
; regs used in this function:  0 1 2 3 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	l	r3,1+time ;P_O_A const
	st	r3,2,r14
	l	r1,1,r14
	lim	r0,LC21  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
L114
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	nz,L116
	jc	uc,L115
L116
	l	r1,2,r14  ;extendqihi2
	dsra	r1,16
	dc	r1,time 
	jc	ez,L117
	decm	1,1,r14
	l	r3,1+time ;P_O_A const
	st	r3,2,r14
L117
	sjs	r15,nothing   ; no return value
	jc	uc,L114
L115
L113
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	konst
KLC22	data	'%','d',' ','<','-',' ','t','p','_','e','r','r','o','r','('
	data	'%','d',')',10,0
	normal   ; text_section
	global	tp_error	; export
tp_error
; regs used in this function:  0 1 2 3 14
	sisp	r15,2  ; reserve local-variable space
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
	st	r0,1,r14
	sjs	r15,nothing   ; no return value
	l	r1,been_initialized
	lr	r1,r1   ; from tstqi
	jc	nz,L119
	lisp	r0,9
	jc	uc,L118
L119
	l	r1,1,r14
	lr	r1,r1   ; from tstqi
	jc	lt,L121
	l	r1,1,r14
	cisp	r1,3
	jc	gt,L121
	jc	uc,L120
L121	lisp	r0,9
	jc	uc,L118
L120
	stc	0,errno   
	l	r0,1,r14
	sjs	r15,tp_status   ; return value in R0
	l	r3,errno
	st	r3,2,r14
	stc	0,errno   
	l	r2,1,r14
	l	r1,2,r14
	lim	r0,LC22  ; 'i' constraint
	sjs	r15,debuglog   ; no return value
	l	r0,2,r14
	jc	uc,L118
L118
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	aisp	r14,2 ; free up local-var space
	urs	r15
	Static
	common  sys_memory,31
tp_dontblock 	block   1	; local common
time 	block   4	; local common
errno 	block   1	; local common
been_initialized 	block   1	; local common
inactive_seconds 	block   1	; local common
bs 	block   88	; local common
ttcp_socks 	block   488	; local common
	static
init_srel
rcsid 	block 56
config_span_name 	block 1
tcp_mssdflt 	block 1
tcp_mssmin 	block 1
config_MTU 	block 1
LC0 	block 12
LC1 	block 21
LC2 	block 17
LC3 	block 14
LC4 	block 13
LC5 	block 14
LC6 	block 13
LC7 	block 13
LC8 	block 20
LC9 	block 14
LC10 	block 17
LC11 	block 21
LC12 	block 2
LC13 	block 18
LC14 	block 20
LC15 	block 14
LC16 	block 21
LC17 	block 16
LC18 	block 23
LC19 	block 18
LC20 	block 27
LC21 	block 14
LC22 	block 20
	init
	lim	r0,init_srel
	lim	r1,442
	lim	r2,Krcsid
	mov	r0,r2
	normal
	end

