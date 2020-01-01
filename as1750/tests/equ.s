s1	equ	1
FALSE	EQU	0
TRUE	EQU	1
MAX	EQU	2
MAX2	EQU	3
MMU	EQU	TRUE

    IF MMU = TRUE
	lisp r1, MAX2
    ELSE
	lisp r2, MAX
	LISP r3, MAX+1
    ENDIF
	lim	r0,s1
	end
