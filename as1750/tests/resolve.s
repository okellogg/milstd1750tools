	name	resolve	
	include "ms1750.inc"
	global	__main
; gcc2_compiled:
	normal   ; text_section
	global  debuglog	; export
debuglog
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L1	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  ha_bzero ; export
ha_bzero
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L2	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  ha_fflush	; export
ha_fflush; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L3	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  ha_inet_addr	; export
ha_inet_addr; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L4	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  ha_isdigit	; export
ha_isdigit; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L5	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  ha_perror	; export
ha_perror
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L6	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  ha_putchar	; export
ha_putchar; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L7	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  ha_strncpy ; export
ha_strncpy; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L8	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global  lostpeer ; export
lostpeer; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L9
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Init	; export
tcp_Init
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frameL10
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	gettimeofday	; export
gettimeofday
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frameL11
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	bcopy	; export
bcopy
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frameL12
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	bzero	; export
bzero
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frameL13
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	ha_inet_ntoa	; export
ha_inet_ntoa
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frameL14
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Abort	; export
tcp_Abort
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frameL15
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Close	; export
tcp_Close
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frameL16
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Flush	; export
tcp_Flush
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L17
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Listen	; export
tcp_Listen
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L18
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_connect	; export
tcp_connect
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L19
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Open	; export
tcp_Open
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L20
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Write	; export
tcp_Write
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L21
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	tcp_Read	; export
tcp_Read
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L22
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	scheduler	; export
scheduler
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L23
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	global	create_thread	; export
create_thread
; regs used in this function:  14
	pshm	r14,r14 ; push old frame
	lr	r14,r15 ; set new frame
L24
	lr	r15,r14 ; set stack ptr to frame ptr
	popm	r14,r14 ; restore previous frame ptr
	urs	r15
	Static
	global _iob
_iob 	block   90	; local common
	end

