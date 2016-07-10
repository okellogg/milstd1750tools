; bcopy.s
;
; C call interface:
; void bcopy(const char *s1, char *s2, int len)
;
; Copies n bytes from memory area s1 to s2.
;
; Parameters:
;	R0 = source address
;	R1 = destination address
;	R2 = length
;

	name	bcopy
	global	bcopy
bcopy	xwr	r0,r1
	xwr	r1,r2
	mov	r0,r2
	urs	r15
	
	end

