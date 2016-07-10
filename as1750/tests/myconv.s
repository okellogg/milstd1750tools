	include ms1750.inc

         EXPORT    CVIA
         EXPORT    CVLA
         EXPORT    CVFA
         EXPORT    CVEA
;         GLOBAL    CVAI
;         GLOBAL    CVAL
;         GLOBAL    CVAH
;         GLOBAL    CVAF
;
; Constant definitions
; --------------------
         konst
E_TEN    DATAEF     10.0
E_ONE    DATAEF     1.0
MINUS_1  DATAEF     -1.0
NNN      DATAC     "# not normal.        "

         normal
;
;-------------------------------------------------------
;
; C V I A
; -------
;
; Function:
;          Convert 16-bit integer to decimal ASCII.
;          The the result string has a fixed length of
;          4 words. Allways two digits are
;          stored in one word. The string will be
;          terminated by a NULL-word.
;
; Input:   R0 --> Stringaddress (result)
;       
;          R1 --> Integer (1750A - Format)
;
; Output:
;          R0 --> Startaddress of ASCII-String
;
;          R0 -->   sign
;                    digits (1-5)
;                    terminating zeros
;
;          Examples:
;                    dec = +375
;                    int = 177H
;                    str = 2020 2B33 3735 0000
;                    
;                    dec = -10
;                    int = FFF6H
;                    str = 2020 202D 3130 0000
;
;
; Registers changed:
;           PSW
;
;
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
            ORIM   R13,'0'              ;ASCII
            TBR    15,R9                ;UPPER OR LOWER BYTE
            BEZ    STHI
            STLB   R13,2,R11
            JC     15,N_DIV
STHI        STUB   R13,2,R11
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

;-------------------------------------------------------
;
; C V L A
; -------
;
; Function:
;          Convert 32-bit integer to decimal ASCII.
;          The the result string has a fixed length of
;          6 words. Allways two digits are
;          stored in one word. The string will be
;          terminated by a NULL-byte.
;
; Input:   R0  --> Stringaddress (result)
;       
;          R1,R2 --> Long Integer (1750A - Format)
;
; Output:
;          R0 --> Startaddress of ASCII-String
;
;          R0 -->   sign
;                    digits (1-5)
;                    terminating zeros
;
;          Examples:
;                    dec = +375
;                    int = 177H
;                    str = 2020 2020 2020 202B 3337 3500
;                    
;                    dec = -10
;                    int = FFF6H
;                    str = 2020 2020 2020 2020 2D31 3000
;
;
; Registers changed:
;           PSW
;
CVLA
            PSHM   R0,R12               ;SAVE REGISTERS
	    lr     r11,r0
            LIM    R10,"  "             ;CLEAR BUFFER
            LISP   R9,6
CLL         ST     R10,0,R11
            AISP   R11,1
            SOJ    R9,CLL
            SISP   R11,1
            STLB   R9,0,R11             ;TERMINATOR
            LIM    R10,' '              ;ASSUME + 
            ORR    R1,R1                ;TEST SIGN
            BGE    POS_SIGN
            LIM    R10,'-'              ;CHANGE TO "-"
            CIM    R1,#8000             ;TEST 8000 0000
	    BNZ    NOT8
	    LR     R2,R2
            BNZ    NOT8
            AISP   R2,1                ;FEHLER,I KNOW
NOT8        DNEG   R1,R1
POS_SIGN
            XORR   R7,R7
            LISP   R8,10                ;DIVISOR
DIVL_10
            AISP   R9,1                 ;INCR DIGIT COUNT
            DLR    R4,R1                ;X-(X/10)*10=REST
            DLR    R2,R1
            DDR    R2,R7
            DMR    R2,R7
            DSR    R4,R2
            AIM    R5,'0'
            TBR    15,R9
            BEZ    STLOBY               ;LOWER BYTE
            STUB   R5,0,R11
            SISP   R11,1                ;NEXT ADDRESS
            BR     N_DIV_L
STLOBY      STLB   R5,0,R11
N_DIV_L     DDR    R12,R7
            JC     5,DIVL_10
            AISP   R9,1
            TBR    15,R9                ;STORE SIGN
            BNZ    S_HI_BY
            STLB   R10,0,R11
            BR     CVLA_END
S_HI_BY
            STUB   R10,0,R11
CVLA_END
            POPM   R0,R12
            URS    R15

;-------------------------------------------------------
;
; C V F A
; -------
;
; Function:
;          Convert 32-bit floating point number to
;          decimal ASCII.
;          The the result string has a fixed length of
;          8 words. Allways two digits are
;          stored in one word. The string will be
;          terminated by a NULL-byte.
;
; Input:   R0 --> Stringaddress (result)
;       
;          R1,R2 --> Floating point number (1750A - Format)
;
; Output:
;          R0 --> Startaddress of ASCII-String
;
;          R0 -->   sign
;                    digit
;                    dec. point
;                    6 digits
;                    space
;                    E
;                    sign
;                    2 digits 
;                    terminating zeros
;
;          Examples:
;                    dec = +37.2
;                    str = +3.720000 E+01
;                    
;
;
; Registers changed:
;           PSW
;
;-------------------------------------------------------
;
CVFA
           PSHM   R0,R14               ;SAVE REGISTERS
	   lr     r11,r0
	   dlr    r12,r1
           XORR   R0,R0
           ST     R0,7,R11             ;TERMINATOR
           XORR   R14,R14
           LISP   R1,6                 ;6 DIGITS
           SJS    R15,CVFF
           POPM   R0,R14
           URS    R15
           PAGE
;-----------------------------------------------------
; C V E A
; -------
;
; Function:
;          Convert 48-bit floating point number to
;          decimal ASCII.
;          The the result string has a fixed length of
;          14 words. Allways two digits are
;          stored in one word. The string will be
;          terminated by a NULL-byte.
;
; Input:   R0 --> Stringaddress (result)
;       
;          R1,R2,R3 --> Floating point number (1750A - Format)
; Output:
;          R0 --> Startaddress of ASCII-String
;
;          R0 -->   sign
;                    digit
;                    dec. point
;                    11 digits
;                    space
;                    E
;                    sign
;                    2 digits 
;                    terminating zeros
;
;          Examples:
;                    dec = +37.2
;                    str = +3.72000000000 E+01
;                    
;
;
; Registers changed:
;           PSW
;
;-------------------------------------------------------
;
CVEA
           PSHM   R0,R14               ;SAVE REGISTERS
	   LR     R11,R0
	   EFLR.M 12,1
           XORR   R0,R0
           ST     R0,10,R11            ;TERMINATOR
           LISP   R1,11                ;11 DIGITS
           SJS    R15,CVFF
           POPM   R0,R14
           URS    R15
;-----------------------------------------------------
;
CVFF
           TBR    0,R12                ;TEST SIGN
           BEZ    F_POS
           TBR    1,R12
           BNZ    NOTNORM
           LIM    R0,"- "              ;CHANGE TO -
           EFM    R12,MINUS_1          ;absolute
           BR     F_OK
NOTNORM
           LIM    R10,NNN
           LISP   R12,7
           CISP   R1,6
           BEZ    NODD
           LISP   R12,10
NODD       MOV    R11,R10
           JC     15,CVFA_END
F_POS
           ORR    R0,R12               ;CHECK 0
           ORR    R0,R13
           ORR    R0,R14
           BEZ    F_OK1
           TBR    1,R12
           BEZ    NOTNORM
F_OK1      LIM    R0,"+ "
F_OK       ST     R0,0,R11             ;TO RESULT
           XORR   R2,R2
           TBR    8,R13                ;SIGN OF EX
           BNZ    EX_NEG
EX_POS     EFC    R12,E_TEN            ;< 10.0
           BLT    EXP_END
           EFD    R12,E_TEN            ; FP:=FP/10
EPP        AISP   R2,1
           BR     EX_POS
EXP_END    LIM    R0,"E+"
           BR     ENDEX
EX_NEG     EFC    R12,E_ONE
           JC     6,EXN_END
           EFM    R12,E_TEN            ; FP:+FP*10
           AISP   R2,1
           BR     EX_NEG
EXN_END
           LIM    R0,"E-"
ENDEX      CISP   R1,6
           BEZ    NODDD
           ST     R0,8,R11
           BR     SSSS
NODDD      ST     R0,5,R11
SSSS       DISP   R2,10
           XBR    R2
           ORR    R2,R3
           ORIM   R2,#3030
           CISP   R1,6
           BEZ    S0
           ST     R2,9,R11
           BR     S00
S0         ST     R2,6,R11              ;STORE EXP.
S00        XORR   R7,R7
           XORR   R2,R2
           DLR    R4,R12
           LR     R6,R14
           EFIX   R2,R4                 ;first digit
           EFLT   R4,R2
           EFSR   R12,R4
           AIM    R3,'0'
           STLB   R3,0,R11
           LIM    R3,'.'
           STUB   R3,1,R11
MANTISSA
           EFM    R12,E_TEN            ; FP:=FP*10.0
           DLR    R4,R12
           LR     R6,R14
           EFIX   R2,R4
           EFLT   R4,R2
           EFSR   R12,R4
           AIM    R3,'0'
           AISP   R7,1                 ;LOWER UPPER ?
           TBR    15,R7
           BNZ    STLOB
           STUB   R3,1,R11
           BR     MEND
STLOB      STLB   R3,1,R11
           AISP   R11,1
MEND       SOJ    R1,MANTISSA
           LIM    R3,"  "
           ST     R3,1,R11
CVFA_END
           URS    R15

