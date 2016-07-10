; Externals 
; ---------
;
          GLOBAL     CVIA        ;convert integer to ASCII
          GLOBAL     CVLA        ;long integer to ASCII
          GLOBAL     CVFA        ;floating point to ASCII
          GLOBAL     CVEA        ;ext. fp to ASCII
;
	  static
HELP      BLOCK      18

	  normal

	global putchar
putchar   xio r0,co
	  urs r15

;------------------------------------------------
;
; P U T S
; -------
;
; Function:
;           The function writes a character string
;           to stdout. The string must be NULL
;           terminated.
; Input:
;           R0 ---> Startaddress of string
; Output:
;           none
;
; Registers changed :
;           R0
;-------------------------------------------------
;
	GLOBAL puts
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

;--------------------------------------------------
;
; P R _ N I B B L E
; -----------------
;
; Function:
;           Output hex digit as  ASCII character to
;           stdout.
; Input:
;           R12 --> digit
;
; Output:
;           R12 --> ASCII character
;
; Registers changed:
;           R12,PSW
;
	GLOBAL pr_nibble
PR_NIBBLE
	    PSHM   R0,R0
            ANDM   R0,#000F            ;CLEAR UPPER BITS
            CISP   R0,9
            BGT    A_F
            AIM    R0,'0'              ;0-9
            BR     PR_N
A_F
            AIM    R0,'7'              ;A-F
PR_N
            SJS    R15,PUTCHAR
	    POPM   R0,R0
            URS    R15
;

;--------------------------------------------------
;
; P R _ H E X _ B Y T E
; ---------------------
;
; Function:
;           Output byte as hexadecimal ASCII to
;           stdout.
; Input:
;           R0 --> byte
;
; Output:   none
;
; Registers changed:
;           PSW
;
	GLOBAL pr_hex_byte
PR_HEX_BYTE
            PSHM   R1,R1
            LR     R1,R0
            SRL    R0,4
            SJS    R15,PR_NIBBLE
            LR     R0,R1
            SJS    R15,PR_NIBBLE
            POPM   R1,R1
            URS    R15
;

;--------------------------------------------------
;
; P R _ H E X _ W O R D
; ---------------------
;
; Function:
;           Output word as hexadecimal ASCII to
;           stdout.
; Input:
;           R0 --> word
;
; Output:   none
;--------------------------------------------------
;
;
; Registers changed:
;           PSW
;
	GLOBAL	pr_hex_word
PR_HEX_WORD
	    PSHM   R1,R1
	    LR     R1,R0
            XBR    R0
            SJS    R15,PR_HEX_BYTE
            LR     R0,R1
            SJS    R15,PR_HEX_BYTE
	    POPM   R1,R1
            URS    R15

;--------------------------------------------------
;
; P R _ D E C _ W O R D
; ---------------------
;
; Function:
;           Display 16-bit integer as decimal ASCII.
;
; Input:
;           R0 --> integer number
;
; Output:
;           none
;
; Registers changed:
;           PSW
;
	GLOBAL pr_dec_word
PR_DEC_WORD
            PSHM   R1,R1
	    LR     R1,R0
            LIM    R0,HELP              ;HELP BUFFER
            SJS    R15,CVIA             ;CONVERT TO ASCII
            SJS    R15,PUTS
            POPM   R1,R1
            URS    R15

;--------------------------------------------------
;
; P R _ L O N G _ W O R D
; -----------------------
;
; Function:
;           Display 32-bit integer as decimal ASCII.
;
; Input:
;           R0,R1  --> 32-bit integer number
;
; Output:
;           none
;
; Registers changed:
;           PSW
;
	GLOBAL pr_long_word
PR_LONG_WORD
            PSHM   R0,R2
	    DLR    R1,R0
            LIM    R0,HELP              ;HELP BUFFER
            SJS    R15,CVLA             ;CONVERT TO ASCII
            SJS    R15,PUTS
            POPM   R0,R2
            URS    R15

;--------------------------------------------------
;
; P R _ F P _ N U M B E R
; -----------------------
;
; Function:
;           Display 32-bit floating point number
;           as decimal ASCII.
;
; Input:
;           R0,R1  --> 32-bit fp number
;
; Output:
;           none
;
; Registers changed:
;           PSW
;
	GLOBAL pr_fp_num
PR_FP_NUM
            PSHM   R0,R2
	    DLR    R1,R0
            LIM    R0,HELP              ;HELP BUFFER
            SJS    R15,CVFA             ;CONVERT TO ASCII
            SJS    R15,PUTS
            POPM   R0,R2
            URS    R15

;--------------------------------------------------
;
; P R _ E F P _ N U M B E R
; -------------------------
;
; Function:
;           Display 48-bit floating point number
;           as decimal ASCII.
;
; Input:
;           R0,R1,R2  --> 48-bit fp number
;
; Output:
;           none
;
; Registers changed:
;           PSW
;
	GLOBAL pr_efp_num
PR_EFP_NUM
            PSHM   R0,R3
	    EFLR.M 1,0
            LIM    R0,HELP              ;HELP BUFFER
            SJS    R15,CVEA             ;CONVERT TO ASCII
            SJS    R15,PUTS
            POPM   R0,R3
            URS    R15

	END

