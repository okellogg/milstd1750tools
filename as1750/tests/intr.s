; SIM1750 interrupt handling acceptance test
; Following messages should appear upon running this program:
;
;		INTERRUPT LEVEL 15
;		INTERRUPT LEVEL 14
;		INTERRUPT LEVEL 13
;		INTERRUPT LEVEL 12
;		.....
;		INTERRUPT LEVEL 2
;		INTERRUPT LEVEL 1
;		INTERRUPT LEVEL 0
		
		name	intr
		;-----------------------------------------------------------
		; Imports (see end of file)
		global	pr_dec_word, puts
		;-----------------------------------------------------------
		; Main Program
		normal
main
		xio	r0,TAH
		xio	r0,TBH
		xio	r0,ENBL
		lim	r0,0xffff
		xio	r0,SMK
		lim	r15,0xffff
cnt		set	0
	while	`cnt` < 16
intmask		set	1 << `cnt`
		lim	r0,`intmask`
		xio	r0,SPI
cnt		set	`cnt` + 1
	endwhile
		br	0
		;-----------------------------------------------------------
		; Auxiliary data
		konst
msg		datac	"INTERRUPT LEVEL "
crlf		datac	13,10
		;-----------------------------------------------------------
		; Set up interrupt handlers
		normal
cnt		set	0
	while	`cnt` < 16
int_`cnt`	lim	r0,msg
		sjs	r15,puts
		lim	r0,`cnt`
		sjs	r15,pr_dec_word
		lim	r0,crlf
		sjs	r15,puts
		xio	r0,ENBL
lsti_addr	set	0x20 + `cnt` * 2
		lsti	`lsti_addr`
cnt		set	`cnt` + 1
	endwhile
		;-----------------------------------------------------------
		; Set up area where Linkage Pointers point
		static
lp_area		block	48
		;-----------------------------------------------------------
		; Set up area where Service Pointers point
		konst
svp_area
cnt		set	0
	while	`cnt` < 16
		data	0,0,int_`cnt`
cnt		set	`cnt` + 1
	endwhile
		;-----------------------------------------------------------
		; Set up the LP/SVP area
		konst
		org	0x20
cnt		set	0
	while	`cnt` < 16
offset		set	`cnt` * 3
lp_`cnt`	data	lp_area + `offset`
svp_`cnt`	data	svp_area + `offset`
cnt		set	`cnt` + 1
	endwhile
		unorg

;---------------------  AUXILIARY ROUTINES  -------------------------------

	normal
CVIA
            PSHM   R0,R13               ;SAVE REGISTERS
	    lr     r11,r0
            LIM    R13,"  "             ;CLEAR BUFFER
            ST     R13,0,R11
            ST     R13,1,R11
            ST     R13,2,R11
            XORR   R13,R13              ;TERMINATOR
            ST     R13,3,R11
            LIM    R10,' '              ;ASSUME + no sign 
            ORR    R1,R1                ;TEST SIGN
            BGE    P_SIGN
            LIM    R10,'-'              ;CHANGE TO "-"
            NEG    R1,R1
P_SIGN
            XORR   R9,R9                ;DIGIT COUNT
I_DIV_10
            AISP   R9,1                 ;INCR DIGIT COUNT
            DISP   R1,10                ;DIV BY 10 LOOP
            ORIM   R2,'0'              ;ASCII
            TBR    15,R9                ;UPPER OR LOWER BYTE
            BEZ    STHI
            STLB   R2,2,R11
            JC     15,N_DIV
STHI        STUB   R2,2,R11
            SISP   R11,1                ;NEXT WORD
N_DIV       ORR    R1,R1                ;ENDE ?
            JC     5,I_DIV_10
            TBR    15,R9                ;STORE SIGN
            BEZ    LOBY
            STUB   R10,2,R11
            BR     CVIA_END
LOBY        STLB   R10,2,R11
CVIA_END
            POPM   R0,R13
            URS    R15

	  static
HELP      BLOCK      18

	  normal

;----------------------------
	global putchar
putchar   xio r0,co
	  urs r15
;----------------------------

PUTS
            PSHM   R9,R12
            LR     R11,R0              ;SAVE STRINGADD.
            XORR   R0,R0
PUTSL       LUB    R0,0,R11            ;get word
            BEZ    PUTS_END
            SJS    R15,PUTCHAR
            LLB    R0,0,R11
            BEZ    PUTS_END
            SJS    R15,PUTCHAR
            AISP   R11,1
            BR     PUTSL
PUTS_END
            POPM   R9,R12               ;restore R9-R12
            URS    R15

PR_DEC_WORD
            PSHM   R1,R1
	    LR     R1,R0
            LIM    R0,HELP              ;HELP BUFFER
            SJS    R15,CVIA             ;CONVERT TO ASCII
            SJS    R15,PUTS
            POPM   R1,R1
            URS    R15

	END


