;************************************************************
;
;                      f r e x 
;
; Split a floating-point value into mantissa and exponent.
; 
;************************************************************
;
;
; Synopsis: 
; ---------
;
;
;           double frex(fpval,exptr)
;               double fpval
;               double *exptr;
; Usage:
; -----
;  double mant,exp,orig,frex();
;    { ....
;      mant=frex(orig,&expon);
;      ....
; 
; Description:
; -----------
; 
; This routine is used by the square root function to isolate
; the two components of a double floating point value. It returns
; the mantissa directly and loads the exponent into the double
; area pointed to by the parametric pointer exptr;
;
; The mantissa is standardized to a value 0.25 <= mant <= 1.
; And the exponent is set even and then divided by two and
; incremented by one.
;
; Returned value:
; ---------------
; A double directly (the mantissa) and a double indirectly (the
; exponent).
;
;-----------------------------------------------------------
	PAGE
;-----------------------------------------------------------
;
; Progammed by :     G. Elsner Ndby.
; Adapted to AS1750: O. Kellogg  1995-10-27
; Date:              02.12.88
; Version:           0.00
;
;-----------------------------------------------------------

	NAME	FREX   ; free exponent 

	GLOBAL    frex                ; export

	normal
frex
	; R0,R1,R2 : fpval
	; R3       : *exptr
        L       R5,0,R3
        LR      R4,R1
        ANDM    R1,#FF00
        ANDM    R4,#FF
        TBR     8,R4
        BEZ     EXPOS
        ORIM    R4,#FF00
EXPOS
        TBR     15,R4
        BEZ     EVEN 
        AISP    R4,1
        ORIM    R1,#FF
EVEN
        SRL     R4,1
        ANDM    R4,#FF
        AISP    R4,1
        ANDM    R4,#FF
        ST      R4,1,R5
        STC     0,2,R5
        LIM     R4,#4000
        ST      R4,0,R5
FEND
	; Return value in R0,R1,R2
	URS	R15		;RETURN TO CALLER


	END

